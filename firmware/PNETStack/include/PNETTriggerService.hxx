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
//  Last Modified : <230319.1320>
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
#include "utils/Singleton.hxx"
#include "executor/Notifiable.hxx"

namespace pnet
{

struct TriggerData 
{
    TriggerData()
                : slot(0), trigger(0)
    {
    }
    TriggerData(uint8_t slot_, uint8_t trigger_)
                : slot(slot_), trigger(trigger_)
    {
    }
    TriggerData(GenMessage *m)
    {
        const unsigned char *buf = 
              (const unsigned char *) m->payload.data();
        slot = buf[3];
        trigger = buf[4];
    }
    void InitFromGenMessage(GenMessage *m)
    {
        const unsigned char *buf = 
              (const unsigned char *) m->payload.data();
        slot = buf[3];
        trigger = buf[4];
    }
    void FillMessage(GenMessage *m)
    {
        m->identifier = Defs::Trigger;
        unsigned char buf[8];
        buf[0] = 0x55;
        buf[1] = 0xaa;
        buf[2] = 0x55;
        buf[3] = slot;
        buf[4] = trigger;
        buf[5] = 0;
        buf[6] = 0;
        buf[7] = 0;
        m->payload.assign((const char *)(buf), 8);
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

bool operator==(const TriggerData& lhs, const TriggerData& rhs);

struct TriggerDataHash
{
    std::size_t operator()(TriggerData const& td) const 
    {
        return ((td.slot << 8) | td.trigger);
    }
};

class TriggerRegistryIterator 
{
public:
    TriggerRegistryIterator()
    {
        clear_iteration();
    }
    ~TriggerRegistryIterator()
    {
    }
    TriggerRegistryEntry *next_entry()
    {
        if (it_ == last_) return nullptr;
        TriggerRegistryEntry * h = &(it_->second);
        ++it_;
        return h;
    }
    void clear_iteration()
    {
        it_ = registry_.end();
        last_ = registry_.end();
    }
    void init_iteration(TriggerData td)
    {
        auto range = registry_.equal_range(td);
        it_ = range.first;
        last_ = range.second;
    }
    void register_handler(const TriggerRegistryEntry &entry);
    void unregister_handler(const TriggerRegistryEntry &entry);
private:
    typedef std::unordered_multimap<TriggerData, TriggerRegistryEntry,
          TriggerDataHash> TriggerRegistryContainer;
    TriggerRegistryContainer registry_;
    TriggerRegistryContainer::iterator it_;
    TriggerRegistryContainer::iterator last_;
};

class TriggerHandler : public Service,
                       public Singleton<TriggerHandler>
{
public:
    TriggerHandler(If *iface);
    ~TriggerHandler();
    
    void register_handler(const TriggerRegistryEntry &entry);
    void unregister_handler(const TriggerRegistryEntry &entry);
    class Impl;
    Impl *impl()
    {
        return impl_.get();
    }
private:
    std::unique_ptr<Impl> impl_;
};

}

#endif // __PNETTRIGGERSERVICE_HXX

