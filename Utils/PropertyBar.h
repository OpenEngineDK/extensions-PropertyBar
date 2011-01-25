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
    PropertyTreeNode* node;
public:
    PropertyBar(string name, PropertyTreeNode* node);

    void AddFields(Display::AntTweakBar& m);

    void AddNode(TwBar* twBar, list<string> group, string name, PropertyTreeNode* node);

    static void AntSetCallback(const void *value, void* ctx);
    static void AntGetCallback(void *value, void* ctx);
};
} // NS Utils
} // NS OpenEngine

#endif // _OE_PROPERTY_BAR_H_
