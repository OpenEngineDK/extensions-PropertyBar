// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------

#include "PropertyBar.h"
#include <Logging/Logger.h>
#include <Utils/PropertyBar.h>
#include <Utils/PropertyTreeNode.h>
#include <Utils/Convert.h>

namespace OpenEngine {
namespace Utils {
    
PropertyBar::PropertyBar(string name, PropertyTreeNode* node) : TweakBar(name)
    , node(node)
    , metaNode(NULL)
    , isTree(false) {
    SetIconify(false);       
    AddNode("",NULL,node);
}
PropertyBar::PropertyBar(string name, PropertyTreeNode* node, PropertyTreeNode* mn)
 : TweakBar(name)
    , node(node)
    , metaNode(mn)
    , isTree(false) {
    SetIconify(false);        
    AddNode("",NULL,node);
}

PropertyBar::PropertyBar(string name, PropertyTree* tree) : TweakBar(name)
    , node(tree->GetRootNode())
    , metaNode(NULL)
    , isTree(true) {
    SetIconify(false);    
    AddNode("",NULL,node);
    AddButtons();
}
PropertyBar::PropertyBar(string name, PropertyTree* tree, PropertyTreeNode* metaNode)
: TweakBar(name)
    , node(tree->GetRootNode())    
    , metaNode(metaNode)
    , isTree(true) {
    SetIconify(false);
    AddNode("",NULL,node);
    AddButtons();
}


void PropertyBar::AddButtons() {
    TweakButton<PropertyTree> *btn = 
        new TweakButton<PropertyTree>("save", 
                                      node->GetTree(),
                                      &PropertyTree::Save);

    TweakButton<PropertyTree> *btnC = 
        new TweakButton<PropertyTree>("save (comments)", 
                                      node->GetTree(),
                                      &PropertyTree::SaveWithComments);
    
    AddItem(btn);
    AddItem(btnC);
}

 void PropertyBar::AddNode(string name, PropertyBarGroup* group, PropertyTreeNode* node) {
    if (node->kind == PropertyTreeNode::MAP) {
        PropertyBarGroup *g2 = NULL;
        if (name.compare("") != 0) {
            g2 = new PropertyBarGroup(node->GetNodePath(),name, node, this);            
            g2->Close();
            if (group) {
                group->AddItem(g2);
                group->AddNode(node);
            }
            else 
                AddItem(g2);
        }
        for (map<string,PropertyTreeNode*>::iterator itr = node->subNodes.begin();
             itr != node->subNodes.end();
             itr++) {
            string key = itr->first;        
            AddNode(key, g2, itr->second);

        }
    } else if (node->kind == PropertyTreeNode::ARRAY) {
        PropertyBarGroup *g2 = NULL;
        if (name.compare("") != 0) {
            g2 = new PropertyBarGroup(node->GetNodePath(), name, node, this);
            g2->Close();
            if (group) {
                group->AddItem(g2);
                group->AddNode(node);
            }
            else
                AddItem(g2);
        }
        int i = 0;
        for (vector<PropertyTreeNode*>::iterator itr = node->subNodesArray.begin();
             itr != node->subNodesArray.end();
             itr++) {
            string key = Convert::ToString(i++);
            AddNode(key, g2, *itr);
        }                
    } else if (node->kind == PropertyTreeNode::SCALAR) {
        string n = node->nodePath;
        TweakVar::Type type = TweakVar::STDSTRING;
        if (node->GetType() == PropertyTree::FLOAT) {
            type = TweakVar::FLOAT;
        } else if (node->GetType() == PropertyTree::UINT32) {
            type = TweakVar::UINT32;
        } else if (node->GetType() == PropertyTree::BOOL) {
            type = TweakVar::BOOL;
        }
        PropertyBarVar* v = new PropertyBarVar(n, name, type, node);
        if (metaNode && metaNode->HaveNodePath(n)) {
                PropertyTreeNode* mn = metaNode->GetNodePath(n);
                v->SetMetaNode(mn);
                
        }            
        if (group) {
            group->AddItem(v);
            group->AddNode(node);            
        }
        else 
            AddItem(v);

    } 
}

PropertyBar::PropertyBarVar::PropertyBarVar(string na, string l, TweakVar::Type t, PropertyTreeNode* n)
: TweakVar(na,l,t)
     , node(n)
     , metaNode(NULL) {
    node->PropertiesChangedEvent().Attach(*this);
}

void PropertyBar::PropertyBarVar::SetMetaNode(PropertyTreeNode* mn) {
    metaNode = mn;
    
    metaNode->PropertiesChangedEvent().Attach(*this);

    RefreshMeta();
}

void PropertyBar::PropertyBarVar::RefreshMeta() {
    if (metaNode->HaveNode("step"))
        SetStepSize(metaNode->GetPath("step",1.0f));
    if (metaNode->HaveNode("min"))
        SetMinValue(metaNode->GetPath("min",1.0f));
    if (metaNode->HaveNode("max"))
        SetMaxValue(metaNode->GetPath("max",1.0f));
    
}

void PropertyBar::PropertyBarVar::GetValue(void* value) {
    if (GetType() == FLOAT) {
        *((float*)value) = node->Get<float>(0.0);
    } else if (GetType() == UINT32) {
        *((unsigned int*)value) = node->Get<unsigned int>(0);
    } else if (GetType() == BOOL) {
        *((bool*)value) = node->Get<bool>(0);
    } else { 
        std::string *destPtr = static_cast<std::string *>(value);
        string val = node->value;
        TwCopyStdStringToLibrary(*destPtr, val);
    }     
}

void PropertyBar::PropertyBarVar::SetValue(const void* value) {
    if (GetType() == FLOAT) {
        node->Set<float>(*(float*)value);
    } else if (GetType() == UINT32) {
        node->Set<unsigned int>(*(unsigned int*)value);        
    } else if (GetType() == BOOL) {
        node->Set<bool>(*(bool*)value);        
    } else {
        const std::string *srcPtr = static_cast<const std::string *>(value);        
        node->SetValue(*srcPtr);
    }
}

void PropertyBar::PropertyBarVar::Handle(PropertiesChangedEventArg arg) {
    if (arg.IsTypeChange() && arg.GetNode() == node) {
        if (arg.GetNode()->GetType() == PropertyTree::FLOAT)
            SetType(TweakVar::FLOAT);
        if (arg.GetNode()->GetType() == PropertyTree::UINT32)
            SetType(TweakVar::UINT32);
        if (arg.GetNode()->GetType() == PropertyTree::BOOL)
            SetType(TweakVar::BOOL);
    } else if (arg.IsValueChange() && arg.GetNode() == metaNode) {
        RefreshMeta();
    }
}

PropertyBar::PropertyBarGroup::PropertyBarGroup(string na,
                                                string la,
                                                PropertyTreeNode* n,
                                                PropertyBar* bar) 
: TweakGroup(na, la), node(n), bar(bar) {
    node->PropertiesChangedEvent().Attach(*this);
}

void PropertyBar::PropertyBarGroup::AddNode(PropertyTreeNode* n) {
    nodes.insert(n);
}

void PropertyBar::PropertyBarGroup::Handle(PropertiesChangedEventArg arg) {
    if (arg.IsStructureChange() && arg.GetNode() == node) {
        if (node->IsMap()) {
            for (map<string,PropertyTreeNode*>::iterator itr = node->subNodes.begin();
                 itr != node->subNodes.end();
                 itr++) {
                string key = itr->first;
                PropertyTreeNode* n = itr->second;
                if (nodes.count(n) == 0) {                    
                    bar->AddNode(key, this, n);
                }
            }
        } else if (node->IsArray()) {
            int i=0;
            for (vector<PropertyTreeNode*>::iterator itr = node->subNodesArray.begin();
                 itr != node->subNodesArray.end();
                 itr++) {
                PropertyTreeNode* n = *itr;
                string key = Convert::ToString(i++);
                if (nodes.count(n) == 0) {
                    bar->AddNode(key, this, n);
                }
            }

        }
    }
}




} // NS Utils
} // NS OpenEngine

