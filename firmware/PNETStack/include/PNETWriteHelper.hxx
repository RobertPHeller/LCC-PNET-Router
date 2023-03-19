// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Mar 19 15:56:37 2023
//  Last Modified : <230319.1629>
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

#ifndef __PNETWRITEHELPER_HXX
#define __PNETWRITEHELPER_HXX

#include <string>
#include "PNETIf.hxx"
#include "PNETStack.hxx"
#include "PNETDefs.hxx"

namespace pnet
{

class WriteHelper : public Executable
{
public:
    typedef PNETCanStack *stack_type;
    typedef string payload_type;
    WriteHelper()
    {
    }
    const payload_type &last_payload()
    {
        return buffer_;
    }
    void clear_last_payload()
    {
        buffer_.clear();
    }
    /** Originates a PNET message.
     * @param stack is the PNET Stack.
     * @param ident is the message identifier.
     * @param buffer is the message payload.
     * @param done will be notified when the packet has been enqueued to the
     * physical layer. If done == nullptr, the sending is invoked synchronously.
     */
    void WriteAsync(PNETCanStack *stack, Defs::Identifier ident,
                    const payload_type &buffer, Notifiable *done)
    {
        if (done)
        {
            done_.reset(done);
        }
        else
        {
            // We don't support synchronous sending anymore.
            HASSERT(0);
        }
        stack_ = stack;
        ident_ = ident;
        buffer_ = buffer;
        stack->iface()->global_message_write_flow()->alloc_async(this);
    }
private:
    // Callback from the allocator.
    void alloc_result(QMember *entry) override
    {
        auto *f = stack_->iface()->global_message_write_flow();
        Buffer<GenMessage> *b = f->cast_alloc(entry);
        b->data()->reset(ident_,buffer_);
        b->set_done(&done_);
        f->send(b);
    }
    void run() override
    {
        HASSERT(0);
    }
    Defs::Identifier ident_;
    PNETCanStack *stack_;
    payload_type buffer_;
    BarrierNotifiable done_;
};


}

#endif // __PNETWRITEHELPER_HXX

