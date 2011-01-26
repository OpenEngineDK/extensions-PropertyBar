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
    AddItem(btn);
}

 void PropertyBar::AddNode(string name, TweakGroup* group, PropertyTreeNode* node) {
    if (node->kind == PropertyTreeNode::MAP) {
        TweakGroup *g2 = NULL;
        if (name.compare("") != 0) {
            g2 = new TweakGroup(name);
            if (group) 
                group->AddItem(g2);            
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
        TweakGroup *g2 = NULL;
        if (name.compare("") != 0) {
            g2 = new TweakGroup(name);
            if (group) 
                group->AddItem(g2);            
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
        }
        PropertyBarVar* v = new PropertyBarVar(n,name, type, node);
        if (group)
            group->AddItem(v);
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

} // NS Utils
} // NS OpenEngine

