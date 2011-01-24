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

#include <Display/ITweakBar.h>

namespace OpenEngine {
namespace Utils {

class PropertyTreeNode;
/**
 * Short description.
 *
 * @class PropertyBar PropertyBar.h ons/PropertyBar/Utils/PropertyBar.h
 */
class PropertyBar : public Display::ITweakBar {
private:

public:
    PropertyBar(string name, PropertyTreeNode* node) : ITweakBar(name) {

    }

    void AddFields(Display::AntTweakBar& m);
};
} // NS Utils
} // NS OpenEngine

#endif // _OE_PROPERTY_BAR_H_
