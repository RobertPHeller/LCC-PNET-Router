// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Mar 16 14:35:22 2023
//  Last Modified : <230318.1502>
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

static const char rcsid[] = "@(#) : $Id$";


#include "PNETIfCan.hxx"

#include "utils/StlMap.hxx"
#include "utils/Singleton.hxx"
#include "PNETIfImpl.hxx"
#include "PNETIfCanImpl.hxx"
#include "can_frame.h"
#include "PNETTriggerService.hxx"
#include "PNETControlService.hxx"
#include "PNETDimmerService.hxx"

namespace pnet
{

/** This write flow inherits all the business logic from the parent, jus
 * t
 * maintains a separate allocation queue. This allows global messages to go out
 * even if addressed messages are waiting for destination address
 * resolution. */
class GlobalCanMessageWriteFlow : public CanMessageWriteFlow
{
public:
    GlobalCanMessageWriteFlow(IfCan *if_can)
                : CanMessageWriteFlow(if_can)
    {
    }
    
protected:
    Action entry() override
    {
        return call_immediately(STATE(send_to_hardware));
    }
    
    Action send_finished() override
    {
        return call_immediately(STATE(global_entry));
    }
};

/** This class listens for incoming CAN frames of PNET messages, 
 * computing its identifier. The resulting message is then passed to 
 * the generic If for dispatching. 
 */
class FrameToGlobalMessageParser : public CanFrameStateFlow
{
public:
    FrameToGlobalMessageParser(IfCan *service)
                : CanFrameStateFlow(service)
    {
        if_can()->frame_dispatcher()->register_handler(this,0,0);
    }
    ~FrameToGlobalMessageParser()
    {
        if_can()->frame_dispatcher()->unregister_handler(this,0,0);
    }
    /// Handler entry for incoming messages.
    Action entry() OVERRIDE
    {
        struct can_frame *f = message()->data();
        id_ = GET_CAN_FRAME_ID_EFF(*f);
        if (f->can_dlc)
        {
            buf_.assign((const char *)(&f->data[0]), f->can_dlc);
        }
        else
        {
            buf_.clear();
        }
        release();
        return allocate_and_call(if_can()->dispatcher(), STATE(send_to_if));
    }
    
    Action send_to_if()
    {
        auto *b = get_allocation_result(if_can()->dispatcher());
        GenMessage *m = b->data();
        m->identifier = (Defs::Identifier)id_;
        m->payload = buf_;
        if_can()->dispatcher()->send(b/*, b->data()->priority()*/);
        return exit();
    }
private:
    /// CAN frame ID, saved from the incoming frame.
    uint32_t id_;
    /// Payload for the MTI message.
    string buf_;
};

IfCan::IfCan(ExecutorBase *executor, CanHubFlow *device)
      : If(executor)
      , CanIf(this, device)
{
    auto *gflow = new GlobalCanMessageWriteFlow(this);
    globalWriteFlow_ = gflow;
    add_owned_flow(gflow);
    add_owned_flow(new FrameToGlobalMessageParser(this));
}    

IfCan::~IfCan()
{
}

void IfCan::add_owned_flow(Executable *e)
{
    ownedFlows_.push_back(std::unique_ptr<Executable>(e));
}



}
