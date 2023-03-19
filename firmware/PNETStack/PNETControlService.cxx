// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Mar 18 09:22:40 2023
//  Last Modified : <230319.1353>
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

#include "PNETDefs.hxx"
#include "PNETControlService.hxx"
#include "PNETControlServiceImpl.hxx"

namespace pnet
{

ControlHandler::ControlHandler(If *iface)
      : Service(iface->executor())
{
    impl_.reset(new Impl(this));
    impl()->ownedFlows_.emplace_back(new ControlInteratorFlow(iface,this));
}

ControlHandler::~ControlHandler()
{
}

ControlInteratorFlow::ControlInteratorFlow(If *iface_, 
                                           ControlHandler *Control_handler)
      : IncomingMessageStateFlow(iface_)
      , Control_handler_(Control_handler)
      , iterator_(Control_handler->impl()->iterator())
{
    
    iface()->dispatcher()->register_handler(this,
                                            Defs::Control,
                                            0xffffffff);
}

ControlInteratorFlow::~ControlInteratorFlow()
{
    iface()->dispatcher()->unregister_handler(this,
                                              Defs::Control,
                                              0xffffffff);
}

StateFlowBase::Action ControlInteratorFlow::entry()
{
    td_.InitFromGenMessage(nmsg()); 
    incomingDone_ = message()->new_child();
    release();
    iterator_->init_iteration(td_);
    return yield_and_call(STATE(iterate_next));
}

StateFlowBase::Action ControlInteratorFlow::iterate_next()
{
    ControlRegistryEntry *entry = iterator_->next_entry();
    if (!entry)
    {
        if (incomingDone_)
        {
            incomingDone_->notify();
            incomingDone_ = nullptr;
        }
        return exit();
    }
    return dispatch_Control(entry);
}

StateFlowBase::Action ControlInteratorFlow::dispatch_Control(const ControlRegistryEntry *entry)
{
    Buffer<ControlHandlerCall> *b;
    Control_handler_->impl()->callerFlow_.pool()->alloc(&b, nullptr);
    HASSERT(b);
    b->data()->reset(entry, &td_);
    n_.reset(this);
    b->set_done(&n_);
    Control_handler_->impl()->callerFlow_.send(b);
    return wait();
}

bool operator==(const ControlData& lhs, const ControlData& rhs)
{
    return (lhs.slot == rhs.slot && lhs.value == rhs.value &&
            lhs.attributes == rhs.attributes);
}

void ControlRegistryIterator::register_handler(const ControlRegistryEntry &entry)
{
    auto range = registry_.equal_range(entry.cd);
    for (auto it = range.first; it != range.second; it++)
    {
        ControlRegistryEntry tre = it->second;
        if (tre.handler == entry.handler) return;
    }
    registry_.insert(std::pair<ControlData, ControlRegistryEntry>(entry.cd,entry));
}

void ControlHandler::register_handler(const ControlRegistryEntry &entry)
{
    impl()->iterator()->register_handler(entry);
}

void ControlRegistryIterator::unregister_handler(const ControlRegistryEntry &entry)
{
    auto range = registry_.equal_range(entry.cd);
    for (auto it = range.first; it != range.second; it++)
    {
        ControlRegistryEntry tre = it->second;
        if (tre.handler == entry.handler)
        {
            registry_.erase(it);
            return;
        }
    }
}

void ControlHandler::unregister_handler(const ControlRegistryEntry &entry)
{
    impl()->iterator()->unregister_handler(entry);
}

ControlHandler::Impl::Impl(ControlHandler *service)
      : callerFlow_(service)
{
}

ControlHandler::Impl::~Impl()
{
}

StateFlowBase::Action ControlProcessCallerFlow::entry()
{
    ControlHandlerCall *c = message()->data();
    n_.reset(this);
    c->registry_entry->handler->process_control(c->registry_entry->cd, &n_);
    return wait_and_call(STATE(call_done));
}

StateFlowBase::Action ControlProcessCallerFlow::call_done()
{
    return release_and_exit();
}



}


