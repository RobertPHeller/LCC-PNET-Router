// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Mar 16 14:36:31 2023
//  Last Modified : <230317.1323>
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

#ifndef __PNETIFCANIMPL_HXX
#define __PNETIFCANIMPL_HXX

#include "executor/StateFlow.hxx"
#include "PNETIfImpl.hxx"

namespace pnet
{

/** Implements the write-side conversion logic from generic messages to CAN
 *  * frames. */
class CanMessageWriteFlow : public WriteFlowBase
{
public:
    CanMessageWriteFlow(IfCan *if_can) : WriteFlowBase(if_can)
    {
    }
    
    IfCan *if_can()
    {
        return static_cast<IfCan *>(async_if());
    }
    
protected:
    Action send_to_hardware() override
    {
        if (nmsg()->payload.size())
        {
            // We have limited space for counting offsets. In practice this
            // value will be max 10 for certain traction control protocol
            // messages. Longer data usually travels via datagrams or streams.
            HASSERT(nmsg()->payload.size() <= 8);
        }
        return allocate_and_call(if_can()->frame_write_flow(),
                                 STATE(fill_can_frame_buffer));
    }
private:
    virtual Action fill_can_frame_buffer()
    {
        auto *b = get_allocation_result(if_can()->frame_write_flow());
        b->set_done(message()->new_child());
        struct can_frame *f = b->data()->mutable_frame();
        // Sets the CAN id.
        SET_CAN_FRAME_ID_EFF(*f, nmsg()->id());
        
        const string &data = nmsg()->payload;
        if (data.size())
        {
            HASSERT(data.size() <= 8); // too big frame for global msg
            memcpy(f->data, data.data(), data.size());
            f->can_dlc = data.size();
        }
        if_can()->frame_write_flow()->send(b);
        return call_immediately(STATE(send_finished));
    }
};
    
}

#endif // __PNETIFCANIMPL_HXX

