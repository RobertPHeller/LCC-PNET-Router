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
//  Last Modified : <230318.1413>
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

#include "PNETTriggerService.hxx"

namespace pnet
{

TriggerHandler::TriggerHandler(If *iface)
      : Service(iface->executor())
{
    impl_.reset(new Impl(this));
    impl()->ownedFlows_.emplace_back(new TriggerInteratorFlow(iface,this))
}

TriggerHandler::~TriggerHandler()
{
}

TriggerInteratorFlow::TriggerInteratorFlow(If *if, 
                                           TriggerHandler *trigger_handler)
      : IncomingMessageStateFlow(if)
      , trigger_handler_(trigger_handler)
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


bool operator==(const TriggerData& lhs, const TriggerData& rhs)
{
    return (lhs.slot == rhs.slot && lhs.trigger == rhs.trigger);
}

StateFlowBase::Action TriggerHandler::entry()
{
    GenMessage *m = message()->data();
    TriggerData trigger(m);
    auto range = registry_.equal_range(trigger);
    for (auto it = range.first; it != range.second; it++)
    {
        TriggerData td = it->first;
        TriggerRegistryEntry tre = it->second;
        n_.reset(this);
        tre.handler->process_trigger(td,&n_);
    }
    return release_and_exit();
}

void TriggerHandler::register_handler(const TriggerRegistryEntry &entry)
{
    auto range = registry_.equal_range(entry.td);
    for (auto it = range.first; it != range.second; it++)
    {
        TriggerRegistryEntry tre = it->second;
        if (tre.handler == entry.handler) return;
    }
    registry_.insert(std::pair<TriggerData, TriggerRegistryEntry>(entry.td,entry));
}

void TriggerHandler::unregister_handler(const TriggerRegistryEntry &entry)
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

//DEFINE_SINGLETON_INSTANCE(TriggerHandler);

}


