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
    
PropertyBar::PropertyBar(string name, PropertyTreeNode* node) : ITweakBar(name), node(node) {
    SetIconify(false);
}

    void PropertyBar::AddNode(TwBar* twBar, list<string> group, string name, PropertyTreeNode* node) {
    if (node->kind == PropertyTreeNode::MAP) {
        for (map<string,PropertyTreeNode*>::iterator itr = node->subNodes.begin();
             itr != node->subNodes.end();
             itr++) {
            string key = itr->first;
            group.push_back(name);
            AddNode(twBar, group, key, itr->second);

        }
    } else if (node->kind == PropertyTreeNode::SCALAR) {
        string n = node->nodePath;
        //logger.warning << group << " || " << name << " || " << n << logger.end;
        

        string def = "label=" + name + " group=a/b/d";
        // if (group.size()) {
        //     //def = "group=" + group.back();
        //     //group.pop_back();
        // }
        TwAddVarCB(twBar,
                   n.c_str(),
                   TW_TYPE_STDSTRING, 
                   &PropertyBar::AntSetCallback,
                   &PropertyBar::AntGetCallback,
                   node,
                   def.c_str());
        string lg = n;

        string barName = TwGetBarName(twBar);

        // for (list<string>::reverse_iterator itr = group.rbegin();
        //      itr != group.rend();
        //      itr++) {
        //     string setGroup = barName + "/" + lg + " group=" + *itr;
        //     lg = *itr;
        //     TwDefine(setGroup.c_str());
        //     logger.warning << setGroup << logger.end;
        // }
        // logger.warning << " -- " << logger.end;
    } 
}


void PropertyBar::AntSetCallback(const void *value, void* ctx) {
    const std::string *srcPtr = static_cast<const std::string *>(value);
    PropertyTreeNode* node = static_cast<PropertyTreeNode*>(ctx);
    node->SetValue(*srcPtr);

}
void PropertyBar::AntGetCallback(void *value, void* ctx) {
    std::string *destPtr = static_cast<std::string *>(value);
    PropertyTreeNode* node = static_cast<PropertyTreeNode*>(ctx);
    string val = node->value;
    TwCopyStdStringToLibrary(*destPtr, val);
}


void PropertyBar::AddFields(Display::AntTweakBar& m) {
    TwBar *twBar = GetBar();
    
    AddNode(twBar, list<string>(), "node", node);
}

} // NS Utils
} // NS OpenEngine

