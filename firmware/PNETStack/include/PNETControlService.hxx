// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Fri Mar 17 16:13:53 2023
//  Last Modified : <230317.1716>
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

#ifndef __PNETCONTROLSERVICE_HXX
#define __PNETCONTROLSERVICE_HXX

#include "PNETIf.hxx"

namespace pnet
{

class ControlHandler : public IncomingMessageStateFlow
{
public:
    ControlHandler(If *service) : IncomingMessageStateFlow(service)
    {
        iface()->dispatcher()->register_handler(this,
                                                Defs::Control,
                                                0xffffffff);
    }
    ~ControlHandler()
    {
        iface()->dispatcher()->unregister_handler(this,
                                                  Defs::Control,
                                                  0xffffffff);
    }
    /// Handler callback for incoming messages.
    Action entry() override
    {
        //GenMessage *m = message()->data();
        return release_and_exit();
    }
private:
};


}


#endif // __PNETCONTROLSERVICE_HXX

