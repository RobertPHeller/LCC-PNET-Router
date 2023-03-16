// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Mar 16 14:44:16 2023
//  Last Modified : <230316.1451>
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

#ifndef __PNETIFIMPL_HXX
#define __PNETIFIMPL_HXX

#include "PNETIf.hxx"

namespace pnet
{

/** Implementation of the hardware-independent parts of the write flows. */
class WriteFlowBase : public StateFlow<Buffer<GenMessage>, QList<4>>
{
public:
    WriteFlowBase(If *async_if)
        : StateFlow<Buffer<GenMessage>, QList<4>>(async_if)
    {
    }

protected:
    /** This function will be called (on the main executor) to initiate sending
     * this message to the hardware. The flow will then execute the returned
     * action.
     *
     * NOTE: it is possible that this functon will never be called for a given
     * flow. */
    virtual Action send_to_hardware() = 0;

    /** Virtual method called after the send is completed, i.e., all the frames
     * are generated and sent to the hardware. Various flows might need to take
     * additional steps afterwards. */
    virtual Action send_finished()
    {
        return release_and_exit();
    }

    /// @returns the interface that this flow is assigned to.
    If *async_if()
    {
        return static_cast<If *>(service());
    }

    /** Implementations shall call this function when they are done with
     * sending the packet.
     */
    // void cleanup();

    /// Returns the PNET message we are trying to send.
    GenMessage *nmsg()
    {
        return message()->data();
    }

protected:

    /** Global write flows should return to this state AFTER sending the
     * message to the hardware. They should ensure the message is still
     * intact. They will not get back control. */
    Action global_entry();
};

}

#endif // __PNETIFIMPL_HXX

