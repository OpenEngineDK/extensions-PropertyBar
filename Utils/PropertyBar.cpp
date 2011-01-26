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
    , isTree(false) {
    SetIconify(false);
    
    
    AddNode("",NULL,node);

}
PropertyBar::PropertyBar(string name, PropertyTree* tree) : TweakBar(name)
    , node(tree->GetRootNode())
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

            logger.error << "array, add " << (*itr)->GetNodePath() 
                         << " to " << g2->GetName() << logger.end;
            AddNode(key, g2, *itr);
        }                
    } else if (node->kind == PropertyTreeNode::SCALAR) {
        string n = node->nodePath;
        TweakVar::Type type = TweakVar::STDSTRING;
        if (node->GetType() == PropertyTree::FLOAT) {
            type = TweakVar::FLOAT;
        }
        PropertyBarVar* v = new PropertyBarVar(n,name, type, node);
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
     , node(n) {
    node->PropertiesChangedEvent().Attach(*this);
}

void PropertyBar::PropertyBarVar::GetValue(void* value) {
    if (GetType() == FLOAT) {
        
        *((float*)value) = node->Get<float>(0.0);
    } else { 
        std::string *destPtr = static_cast<std::string *>(value);
        string val = node->value;
        TwCopyStdStringToLibrary(*destPtr, val);
    }     
}

void PropertyBar::PropertyBarVar::SetValue(const void* value) {
    if (GetType() == FLOAT) {
        node->Set<float>(*(float*)value);
    } else {
        const std::string *srcPtr = static_cast<const std::string *>(value);        
        node->SetValue(*srcPtr);
    }
}

void PropertyBar::PropertyBarVar::Handle(PropertiesChangedEventArg arg) {
    if (arg.IsTypeChange() ) {
        if (arg.GetNode()->GetType() == PropertyTree::FLOAT)
            SetType(TweakVar::FLOAT);
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

