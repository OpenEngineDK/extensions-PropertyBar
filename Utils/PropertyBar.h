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
class PropertyBar : public TweakBar {
private:
    PropertyTreeNode* node;
    bool isTree;

    class PropertyBarVar : public TweakVar
                         , public Core::IListener<PropertiesChangedEventArg> {
        PropertyTreeNode* node;
    public:
        PropertyBarVar(string name, string label, TweakVar::Type t, PropertyTreeNode* node);
        void GetValue(void* value);
        void SetValue(const void* value);
        void Handle(PropertiesChangedEventArg arg);
    };

    void AddNode(string name, TweakGroup* group, PropertyTreeNode* node);
    void AddButtons();
public:
    PropertyBar(string name, PropertyTree* tree);
    PropertyBar(string name, PropertyTreeNode* node);


    

;


};
} // NS Utils
} // NS OpenEngine

#endif // _OE_PROPERTY_BAR_H_
