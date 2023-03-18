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
//  Last Modified : <230318.1642>
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
#include "PNETTriggerService.hxx"
#include "PNETTriggerServiceImpl.hxx"

namespace pnet
{

TriggerHandler::TriggerHandler(If *iface)
      : Service(iface->executor())
{
    impl_.reset(new Impl(this));
    impl()->ownedFlows_.emplace_back(new TriggerInteratorFlow(iface,this));
}

TriggerHandler::~TriggerHandler()
{
}

TriggerInteratorFlow::TriggerInteratorFlow(If *iface_, 
                                           TriggerHandler *trigger_handler)
      : IncomingMessageStateFlow(iface_)
      , trigger_handler_(trigger_handler)
      , iterator_(trigger_handler->impl()->iterator())
{
    
    iface()->dispatcher()->register_handler(this,
                                            Defs::Trigger,
                                            0xffffffff);
}

TriggerInteratorFlow::~TriggerInteratorFlow()
{
    iface()->dispatcher()->unregister_handler(this,
                                              Defs::Trigger,
                                              0xffffffff);
}

StateFlowBase::Action TriggerInteratorFlow::entry()
{
    td_.InitFromGenMessage(nmsg()); 
    incomingDone_ = message()->new_child();
    release();
    iterator_->init_iteration(td_);
    return yield_and_call(STATE(iterate_next));
}

StateFlowBase::Action TriggerInteratorFlow::iterate_next()
{
    TriggerRegistryEntry *entry = iterator_->next_entry();
    if (!entry)
    {
        if (incomingDone_)
        {
            incomingDone_->notify();
            incomingDone_ = nullptr;
        }
        return exit();
    }
    return dispatch_trigger(entry);
}

StateFlowBase::Action TriggerInteratorFlow::dispatch_trigger(const TriggerRegistryEntry *entry)
{
    Buffer<TriggerHandlerCall> *b;
    trigger_handler_->impl()->callerFlow_.pool()->alloc(&b, nullptr);
    HASSERT(b);
    b->data()->reset(entry, &td_);
    n_.reset(this);
    b->set_done(&n_);
    trigger_handler_->impl()->callerFlow_.send(b);
    return wait();
}

bool operator==(const TriggerData& lhs, const TriggerData& rhs)
{
    return (lhs.slot == rhs.slot && lhs.trigger == rhs.trigger);
}

void TriggerRegistryIterator::register_handler(const TriggerRegistryEntry &entry)
{
    auto range = registry_.equal_range(entry.td);
    for (auto it = range.first; it != range.second; it++)
    {
        TriggerRegistryEntry tre = it->second;
        if (tre.handler == entry.handler) return;
    }
    registry_.insert(std::pair<TriggerData, TriggerRegistryEntry>(entry.td,entry));
}

void TriggerHandler::register_handler(const TriggerRegistryEntry &entry)
{
    impl()->iterator()->register_handler(entry);
}

void TriggerRegistryIterator::unregister_handler(const TriggerRegistryEntry &entry)
{
    auto range = registry_.equal_range(entry.td);
    for (auto it = range.first; it != range.second; it++)
    {
        TriggerRegistryEntry tre = it->second;
        if (tre.handler == entry.handler)
        {
            registry_.erase(it);
            return;
        }
    }
}

void TriggerHandler::unregister_handler(const TriggerRegistryEntry &entry)
{
    impl()->iterator()->unregister_handler(entry);
}

TriggerHandler::Impl::Impl(TriggerHandler *service)
      : callerFlow_(service)
{
}

TriggerHandler::Impl::~Impl()
{
}

//DEFINE_SINGLETON_INSTANCE(TriggerHandler);

}


