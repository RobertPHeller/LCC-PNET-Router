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
//  Last Modified : <230319.1346>
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

#ifndef __PNETDIMMERSERVICEIMPL_HXX
#define __PNETDIMMERSERVICEIMPL_HXX
#include <stdint.h>
#include <unordered_map>
#include "PNETIf.hxx"
#include "PNETDefs.hxx"
#include "utils/Singleton.hxx"
#include "executor/Notifiable.hxx"
#include "PNETDimmerService.hxx"

namespace pnet
{

struct DimmerHandlerCall
{
    const DimmerRegistryEntry *registry_entry;
    DimmerData *td;
    void reset(const DimmerRegistryEntry *entry, DimmerData *td)
    {
        this->registry_entry = entry;
        this->td = td;
    }
};

class DimmerProcessCallerFlow 
      : public StateFlow<Buffer<DimmerHandlerCall>, QList<5>>
{
public:
    DimmerProcessCallerFlow(Service *service)
                : StateFlow<Buffer<DimmerHandlerCall>, QList<5>>(service) {};
private:
    virtual Action entry() override;
    Action call_done();
    BarrierNotifiable n_;
};

class DimmerHandler::Impl
{
public:
    Impl(DimmerHandler *service);
    ~Impl();
    std::vector<std::unique_ptr<StateFlowWithQueue>> ownedFlows_;
    DimmerRegistryIterator *iterator()
    {
        return &iterator_;
    }
    DimmerProcessCallerFlow callerFlow_;
    DimmerRegistryIterator iterator_;
};

class DimmerInteratorFlow : public IncomingMessageStateFlow
{
public:
    DimmerInteratorFlow(If *iface, DimmerHandler *Dimmer_handler);
    ~DimmerInteratorFlow();
protected:
    virtual Action entry() override;
    Action iterate_next();
private:
    Action dispatch_Dimmer(const DimmerRegistryEntry *entry);
    DimmerHandler *Dimmer_handler_;
    DimmerData td_;
    DimmerRegistryIterator *iterator_;
    Notifiable *incomingDone_;
    BarrierNotifiable n_;
};
    

}


#endif // __PNETDIMMERSERVICEIMPL_HXX

