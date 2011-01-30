// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------


#ifndef _OE_PROPERTY_BAR_H_
#define _OE_PROPERTY_BAR_H_

#include <Utils/TweakBar.h>
#include <Utils/TweakVar.h>
#include <Core/IListener.h>
#include <Utils/PropertyTree.h>

namespace OpenEngine {
namespace Utils {

class PropertyTreeNode;
class PropertyTree;

/**
 * Short description.
 *
 * @class PropertyBar PropertyBar.h ons/PropertyBar/Utils/PropertyBar.h
 */
class PropertyBar : public TweakBar, Core::IListener<PropertiesChangedEventArg> {
private:
    PropertyTreeNode* node;
    set<PropertyTreeNode*> rootNodes;
    PropertyTreeNode* metaNode;
    bool isTree;

    class PropertyBarGroup : public TweakGroup
                           , public Core::IListener<PropertiesChangedEventArg> {
        PropertyTreeNode* node;
        set<PropertyTreeNode*> nodes;
        PropertyBar* bar;
    public:
        PropertyBarGroup(string name, string label, PropertyTreeNode* node, PropertyBar* bar);
        void AddNode(PropertyTreeNode* n);
        void Handle(PropertiesChangedEventArg arg);
    };

    class PropertyBarVar : public TweakVar
                         , public Core::IListener<PropertiesChangedEventArg> {
        PropertyTreeNode* node;
        PropertyTreeNode* metaNode;        
    public:
        PropertyBarVar(string name, string label, TweakVar::Type t, PropertyTreeNode* node);
        void GetValue(void* value);
        void SetValue(const void* value);
        void Handle(PropertiesChangedEventArg arg);
        void SetMetaNode(PropertyTreeNode* mn);
        void RefreshMeta();
    };    

    void AddNode(string name, PropertyBarGroup* group, PropertyTreeNode* node);
    void AddButtons();
public:
    PropertyBar(string name, PropertyTree* tree);
    PropertyBar(string name, PropertyTree* tree, PropertyTreeNode* metaNode);
    PropertyBar(string name, PropertyTreeNode* node);
    PropertyBar(string name, PropertyTreeNode* node, PropertyTreeNode* metaNode);

    void Handle(PropertiesChangedEventArg arg); 

};
} // NS Utils
} // NS OpenEngine

#endif // _OE_PROPERTY_BAR_H_
