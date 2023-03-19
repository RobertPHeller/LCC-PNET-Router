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
//  Last Modified : <230319.1358>
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

#ifndef __PNETDIMMERSERVICE_HXX
#define __PNETDIMMERSERVICE_HXX

#include <stdint.h>
#include <unordered_map>
#include "PNETIf.hxx"
#include "PNETDefs.hxx"
#include "utils/Singleton.hxx"
#include "executor/Notifiable.hxx"

namespace pnet
{

struct DimmerData
{
    DimmerData()
                : slot(0), va(0), vb(0), vc(0), vd(0)
    {
    }
    DimmerData(uint8_t slot_, uint8_t va_, uint8_t vb_, uint8_t vc_, uint8_t vd_)
                : slot(slot_), va(va_), vb(vb_), vc(vc_), vd(vd_)
    {
    }
    DimmerData(GenMessage *m)
    {
        const unsigned char *buf = 
              (const unsigned char *) m->payload.data();
        slot = buf[1];
        va = buf[2];
        vb = buf[3];
        vc = buf[4];
        vd = buf[5];
    }
    void InitFromGenMessage(GenMessage *m)
    {
        const unsigned char *buf = 
              (const unsigned char *) m->payload.data();
        slot = buf[1];
        va = buf[2];
        vb = buf[3];
        vc = buf[4];
        vd = buf[5];
    }
    void FillMessage(GenMessage *m)
    {
        m->identifier = Defs::Dimmer;
        unsigned char buf[8];
        buf[0] = 0xdd;
        buf[1] = slot;
        buf[2] = va;
        buf[3] = vb;
        buf[4] = vc;
        buf[5] = vd;
        buf[6] = 0;
        buf[7] = 0;
        m->payload.assign((const char *)(buf), 8);
    }
    uint8_t slot;
    uint8_t va;
    uint8_t vb;
    uint8_t vc;
    uint8_t vd;
};

class DimmerProcess
{
public:
    virtual ~DimmerProcess()
    {
    }
    
    virtual void process_dimmer(const DimmerData &cd,
                                 BarrierNotifiable *done) = 0;
};

class DimmerRegistryEntry
{
public:
    DimmerData cd;
    DimmerProcess *handler;
    uint32_t user_arg;
    DimmerRegistryEntry(DimmerProcess *_handler,
                         const DimmerData &_cd)
                : cd(_cd)
          , handler(_handler)
          , user_arg(0)
    {
    }
    DimmerRegistryEntry(DimmerProcess *_handler,
                         const DimmerData &_cd,
                         unsigned _user_arg)
                : cd(_cd)
          , handler(_handler)
          , user_arg(_user_arg)
    {
    }
};

bool operator==(const DimmerData& lhs, const DimmerData& rhs);
struct DimmerDataHash
{
    std::size_t operator()(DimmerData const& cd) const
    {
        return ((cd.va << 24) | (cd.vb << 16) | (cd.vc << 8) | cd.vd);
    }
};

class DimmerRegistryIterator
{
public:
    DimmerRegistryIterator()
    {
        clear_iteration();
    }
    ~DimmerRegistryIterator()
    {
    }
    DimmerRegistryEntry *next_entry()
    {
        if (it_ == last_) return nullptr;
        DimmerRegistryEntry * h = &(it_->second);
        ++it_; 
        return h; 
    }
    void clear_iteration()
    {
        it_ = registry_.end();
        last_ = registry_.end();
    }
    void init_iteration(DimmerData cd)
    {
        auto range = registry_.equal_range(cd);
        it_ = range.first;
        last_ = range.second;
    }
    void register_handler(const DimmerRegistryEntry &entry);
    void unregister_handler(const DimmerRegistryEntry &entry);
private:
    typedef std::unordered_multimap<DimmerData, DimmerRegistryEntry,
          DimmerDataHash> DimmerRegistryContainer;
    DimmerRegistryContainer registry_;
    DimmerRegistryContainer::iterator it_;
    DimmerRegistryContainer::iterator last_;
};

          

class DimmerHandler : public Service,
                       public Singleton<DimmerHandler>
{
public:
    DimmerHandler(If *service);
    ~DimmerHandler();
    void register_handler(const DimmerRegistryEntry &entry);
    void unregister_handler(const DimmerRegistryEntry &entry);
    class Impl;
    Impl *impl()
    {
        return impl_.get();
    }
private:
    std::unique_ptr<Impl> impl_;
};


}


#endif // __PNETDIMMERSERVICE_HXX

