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
#include <Utils/PropertyTreeNode.h>

namespace OpenEngine {
namespace Utils {
    
PropertyBar::PropertyBar(string name, PropertyTreeNode* node) 
: TweakBar(name)
    , node(node) {
    SetIconify(false);
    
    
    AddNode("",NULL,node);

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
    } else if (node->kind == PropertyTreeNode::SCALAR) {
        string n = node->nodePath;
        
        TweakVar<PropertyTreeNode> *v = new TweakVar<PropertyTreeNode>(n,name,node,this);
        if (group)
            group->AddItem(v);
        else 
            AddItem(v);

    } 
}


void PropertyBar::SetTweakVar(const void *value, PropertyTreeNode* ctx) {
    const std::string *srcPtr = static_cast<const std::string *>(value);
    
    ctx->SetValue(*srcPtr);

}
void PropertyBar::GetTweakVar(void *value, PropertyTreeNode* ctx) {
    std::string *destPtr = static_cast<std::string *>(value);
    string val = ctx->value;
    TwCopyStdStringToLibrary(*destPtr, val);
}

} // NS Utils
} // NS OpenEngine

