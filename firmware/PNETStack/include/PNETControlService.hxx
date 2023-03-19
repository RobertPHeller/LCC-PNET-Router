// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Fri Mar 17 16:13:53 2023
//  Last Modified : <230319.1356>
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

#ifndef __PNETCONTROLSERVICE_HXX
#define __PNETCONTROLSERVICE_HXX

#include <stdint.h>
#include <unordered_map>
#include "PNETIf.hxx"
#include "PNETDefs.hxx"
#include "utils/Singleton.hxx"
#include "executor/Notifiable.hxx"

namespace pnet
{

struct ControlData
{
    ControlData()
                : slot(0), value(0), attributes(0)
    {
    }
    ControlData(uint8_t slot_, uint8_t value_, uint8_t attributes_)
                : slot(slot_), value(value_), attributes(attributes_)
    {
    }
    ControlData(GenMessage *m)
    {
        const unsigned char *buf = 
              (const unsigned char *) m->payload.data();
        slot = buf[3];
        value = buf[4];
        attributes = buf[5];
    }
    void InitFromGenMessage(GenMessage *m)
    {
        const unsigned char *buf = 
              (const unsigned char *) m->payload.data();
        slot = buf[3];
        value = buf[4];
        attributes = buf[5];
    }
    void FillMessage(GenMessage *m)
    {
        m->identifier = Defs::Control;
        unsigned char buf[8];
        buf[0] = 0xaa;
        buf[1] = 0xaa;
        buf[2] = 0x55;
        buf[3] = slot;
        buf[4] = value;
        buf[5] = attributes;
        buf[6] = 0;
        buf[7] = 0;
        m->payload.assign((const char *)(buf), 8);
    }
    uint8_t slot;
    uint8_t value;
    uint8_t attributes;
};

class ControlProcess
{
public:
    virtual ~ControlProcess()
    {
    }
    
    virtual void process_control(const ControlData &cd,
                                 BarrierNotifiable *done) = 0;
};

class ControlRegistryEntry
{
public:
    ControlData cd;
    ControlProcess *handler;
    uint32_t user_arg;
    ControlRegistryEntry(ControlProcess *_handler,
                         const ControlData &_cd)
                : cd(_cd)
          , handler(_handler)
          , user_arg(0)
    {
    }
    ControlRegistryEntry(ControlProcess *_handler,
                         const ControlData &_cd,
                         unsigned _user_arg)
                : cd(_cd)
          , handler(_handler)
          , user_arg(_user_arg)
    {
    }
};

bool operator==(const ControlData& lhs, const ControlData& rhs);
struct ControlDataHash
{
    std::size_t operator()(ControlData const& cd) const
    {
        return ((cd.slot << 16) | (cd.value << 8) | cd.attributes);
    }
};

class ControlRegistryIterator
{
public:
    ControlRegistryIterator()
    {
        clear_iteration();
    }
    ~ControlRegistryIterator()
    {
    }
    ControlRegistryEntry *next_entry()
    {
        if (it_ == last_) return nullptr;
        ControlRegistryEntry * h = &(it_->second);
        ++it_; 
        return h; 
    }
    void clear_iteration()
    {
        it_ = registry_.end();
        last_ = registry_.end();
    }
    void init_iteration(ControlData cd)
    {
        auto range = registry_.equal_range(cd);
        it_ = range.first;
        last_ = range.second;
    }
    void register_handler(const ControlRegistryEntry &entry);
    void unregister_handler(const ControlRegistryEntry &entry);
private:
    typedef std::unordered_multimap<ControlData, ControlRegistryEntry,
          ControlDataHash> ControlRegistryContainer;
    ControlRegistryContainer registry_;
    ControlRegistryContainer::iterator it_;
    ControlRegistryContainer::iterator last_;
};

          

class ControlHandler : public Service,
                       public Singleton<ControlHandler>
{
public:
    ControlHandler(If *service);
    ~ControlHandler();
    void register_handler(const ControlRegistryEntry &entry);
    void unregister_handler(const ControlRegistryEntry &entry);
    class Impl;
    Impl *impl()
    {
        return impl_.get();
    }
private:
    std::unique_ptr<Impl> impl_;
};


}


#endif // __PNETCONTROLSERVICE_HXX

