// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Mar 16 10:51:07 2023
//  Last Modified : <230316.1100>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2023  Robert Heller D/B/A Deepwoods Software
//			51 Locke Hill Road
//			Wendell, MA 01379-9728
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// 
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __PNETDEFS_HXX
#define __PNETDEFS_HXX

#include <cstdint>

#include "utils/macros.h"

namespace pnet
{

struct Defs
{
    /** Known Message  identifiers.
     */
    enum Identifier
    {
        Trigger =          0x00052000, // CAN message type for Trigger INPUT
        Control =          0x000A4000, // CAN message type for Control OUTPUT
        Dimmer  =          0x000D4000, // CAN message type for PNET Dimmer
        Loconet =          0x000D2000, // CAN message type for LocoNet Messages
        FirmwareRequest =  0x07000000, // CAN message type for Firmware Messages
        FirmwareTransfer = 0x08000000  // CAN message type for Firmware Transfer
    };
    
    /** Status of the pysical layer link */
    enum LinkStatus
    {
        LINK_UP,  /**< link is up and ready for transmit */
        LINK_DOWN /**< link is down and unable to transmit */
    };
    
private:
    /** This struct should not be instantiated. */
    Defs();
};


}

#endif // __PNETDEFS_HXX

