// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Fri Mar 17 16:10:40 2023
//  Last Modified : <230317.1724>
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

#ifndef __PNETTRIGGERSERVICE_HXX
#define __PNETTRIGGERSERVICE_HXX

#include <stdint.h>
#include <unordered_map>
#include "PNETIf.hxx"
#include "PNETDefs.hxx"

namespace pnet
{

struct TriggerData 
{
    TriggerData()
                : slot(0), trigger(0)
    {
    }
    TriggerData(GenMessage *m)
    {
        const unsigned char *buf = 
              (const unsigned char *) m->payload.data();
        slot = buf[3];
        trigger = buf[4];
    }
    uint8_t slot;
    uint8_t trigger;
};

class TriggerProcess
{
public:
    virtual ~TriggerProcess()
    {
    }
    
    virtual void process_trigger(const TriggerData &td, 
                                 BarrierNotifiable *done) = 0;
};

class TriggerRegistryEntry
{
public:
    TriggerData td;
    TriggerProcess *handler;
    uint32_t user_arg;
    TriggerRegistryEntry(TriggerProcess *_handler, 
                         const TriggerData &_td)
                : td(_td)
          , handler(_handler)
          , user_arg(0)
    {
    }
    TriggerRegistryEntry(TriggerProcess *_handler, 
                         const TriggerData &_td,
                         unsigned _user_arg)
                : td(_td)
          , handler(_handler)
          , user_arg(_user_arg)
    {
    }
};

bool operator==(const TriggerData& lhs, const TriggerData& rhs) 
{
    return (lhs.slot == rhs.slot && lhs.trigger == rhs.trigger);
}

struct TriggerDataHash
{
    std::size_t operator()(TriggerData const& td) const 
    {
        return ((td.slot << 8) | td.trigger);
    }
};

typedef std::unordered_multimap<TriggerData, TriggerRegistryEntry,
TriggerDataHash> TriggerRegistryContainer;


class TriggerHandler : public IncomingMessageStateFlow
{
public:
    TriggerHandler(If *service) : IncomingMessageStateFlow(service)
    {
        iface()->dispatcher()->register_handler(this,
                                                Defs::Trigger,
                                                0xffffffff);
    }
    ~TriggerHandler()
    {
        iface()->dispatcher()->unregister_handler(this,
                                                  Defs::Trigger,
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

#endif // __PNETTRIGGERSERVICE_HXX

