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

namespace OpenEngine {
namespace Utils {

class PropertyTreeNode;
/**
 * Short description.
 *
 * @class PropertyBar PropertyBar.h ons/PropertyBar/Utils/PropertyBar.h
 */
class PropertyBar : public TweakBar
                  , public TweakVarHandler<PropertyTreeNode> {
private:
    PropertyTreeNode* node;
public:
    PropertyBar(string name, PropertyTreeNode* node);


    void AddNode(string name, TweakGroup* group, PropertyTreeNode* node);

    void SetTweakVar(const void* value, PropertyTreeNode* context);
    void GetTweakVar(void* value, PropertyTreeNode* context) ;


};
} // NS Utils
} // NS OpenEngine

#endif // _OE_PROPERTY_BAR_H_
