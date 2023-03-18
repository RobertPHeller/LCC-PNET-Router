// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Mar 18 12:46:33 2023
//  Last Modified : <230318.1405>
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

#ifndef __PNETTRIGGERSERVICEIMPL_HXX
#define __PNETTRIGGERSERVICEIMPL_HXX
#include <stdint.h>
#include <unordered_map>
#include "PNETIf.hxx"
#include "PNETDefs.hxx"
#include "utils/Singleton.hxx"
#include "executor/Notifiable.hxx"
#include "PNETTriggerService.hxx"

namespace pnet
{

struct TriggerHandlerCall
{
    const TriggerRegistryEntry *registry_entry;
    TriggerData *td;
    void reset(const TriggerRegistryEntry *entry, TriggerData *td)
    {
        this->registry_entry = entry;
        this->td = td;
    }
};

class TriggerProcessCallerFlow 
      : public StateFlow<Buffer<TriggerHandlerCall>, QList<5>>
{
public:
    TriggerProcessCallerFlow(Service *service)
                : StateFlow<Buffer<TriggerHandlerCall>, QList<5>>(service) {};
private:
    virtual Action entry() override;
    Action call_done();
    BarrierNotifiable n_;
};

class TriggerHandler::Impl
{
public:
    Impl(TriggerHandler *service);
    ~Impl();
    std::vector<std::unique_ptr<StateFlowWithQueue>> ownedFlows_;
    TriggerProcessCallerFlow callerFlow_;
    TriggerRegistryContainer registry_;
};

class TriggerInteratorFlow : public IncomingMessageStateFlow
{
public:
    TriggerInteratorFlow(If *iface, TriggerHandler *trigger_handler);
    ~TriggerInteratorFlow();
private:
    TriggerHandler *trigger_handler_;
};
    

}


#endif // __PNETTRIGGERSERVICEIMPL_HXX

