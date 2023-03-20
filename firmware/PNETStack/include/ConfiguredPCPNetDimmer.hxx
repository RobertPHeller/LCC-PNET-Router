// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Mar 20 12:19:01 2023
//  Last Modified : <230320.1323>
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

#ifndef __CONFIGUREDPCPNETDIMMER_HXX
#define __CONFIGUREDPCPNETDIMMER_HXX

#include "openlcb/Node.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "PNETStack.hxx"
#include "PNETWriteHelper.hxx"
#include "PNETDimmerService.hxx"

CDI_GROUP(PCPNetDimmerConfig);
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<16>, //
                Name("Description"), 
                Description("User name of this dimmer."))
CDI_GROUP_ENTRY(event_produced, openlcb::EventConfigEntry,
                Name("Event Produced"),
                Description("(P) This event is produced when a matching Dimmer message is received."));
CDI_GROUP_ENTRY(event_consumed, openlcb::EventConfigEntry,
                Name("Event Consumed"),
                Description("(C) This event will cause the defined Dimmer to be sent."));
CDI_GROUP_ENTRY(enabled, openlcb::Uint8ConfigEntry,
                Min(0), Max(1), Default(0),
                MapValues("<relation><property>0</property><value>No</value></relation>"
                          "<relation><property>1</property><value>Yes</value></relation>"),
                Name("Enable"), Description("Enable this dimmer."));
CDI_GROUP_ENTRY(slot, openlcb::Uint8ConfigEntry,
                Min(0), Max(31), Default(0),
                Name("Slot"),
                Description("The dimmer slot number (0-31)."));
CDI_GROUP_ENTRY(value_a, openlcb::Uint8ConfigEntry,
                Default(0),
                Name("Value A"),
                Description("Dimmer value A."));
CDI_GROUP_ENTRY(value_b, openlcb::Uint8ConfigEntry,
                Default(0),
                Name("Value B"),
                Description("Dimmer value B."));
CDI_GROUP_ENTRY(value_c, openlcb::Uint8ConfigEntry,
                Default(0),
                Name("Value C"),
                Description("Dimmer value C."));
CDI_GROUP_ENTRY(value_d, openlcb::Uint8ConfigEntry,
                Default(0),
                Name("Value D"),
                Description("Dimmer value D."));
CDI_GROUP_END();

class PCPNetDimmer : public DefaultConfigUpdateListener,
                           public openlcb::SimpleEventHandler,
                           public pnet::DimmerProcess
{
public:
    PCPNetDimmer(openlcb::Node *node, pnet::PNETCanStack *pnetstack,
                  const PCPNetDimmerConfig &cfg)
                : DefaultConfigUpdateListener()
          , node_(node)
          , cfg_(cfg)
          , pnetstack_(pnetstack)
          , event_produced_(0)
          , event_consumed_(0)
          , enabled_(false)
          , slot_(0)
          , value_a_(0)
          , value_b_(0)
          , value_c_(0)
          , value_d_(0)
          
    {
        if (enabled_) register_dimmer_handler();
    }
    ~PCPNetDimmer()
    {
        if (enabled_) unregister_dimmer_handler();
    }
    UpdateAction apply_configuration(int fd, 
                                     bool initial_load, 
                                     BarrierNotifiable *done) OVERRIDE
    {
        AutoNotify n(done);
        const openlcb::EventId cfg_event_produced = cfg_.event_produced().read(fd);
        const openlcb::EventId cfg_event_consumed = cfg_.event_consumed().read(fd);
        const uint8_t cfg_slot = cfg_.slot().read(fd);
        const uint8_t cfg_value_a = cfg_.value_a().read(fd);
        const uint8_t cfg_value_b = cfg_.value_b().read(fd);
        const uint8_t cfg_value_c = cfg_.value_c().read(fd);
        const uint8_t cfg_value_d = cfg_.value_d().read(fd);
        bool cfg_enabled = (bool) cfg_.enabled().read(fd);
        if (cfg_slot != slot_ || cfg_value_a != value_a_ ||
            cfg_value_b != value_b_ || cfg_value_c != value_c_ ||
            cfg_value_d != value_d_ ||
            cfg_enabled != enabled_)
        {
            if (enabled_) unregister_dimmer_handler();
            slot_ = cfg_slot;
            value_a_ = cfg_value_a;
            value_b_ = cfg_value_b;
            value_c_ = cfg_value_c;
            value_d_ = cfg_value_d;
            enabled_ = cfg_enabled;
            if (enabled_) register_dimmer_handler();
        }
        if (cfg_event_produced != event_produced_ ||
            cfg_event_consumed != event_consumed_)
        {
            if (!initial_load) unregister_event_handler();
            event_produced_ = cfg_event_produced;
            event_consumed_ = cfg_event_consumed;
            register_event_handler();
            return REINIT_NEEDED;
        }
        return UPDATED;
    }
    void factory_reset(int fd) OVERRIDE
    {
        cfg_.description().write(fd, "");
        CDI_FACTORY_RESET(cfg_.slot);
        CDI_FACTORY_RESET(cfg_.value_a);
        CDI_FACTORY_RESET(cfg_.value_b);
        CDI_FACTORY_RESET(cfg_.value_c);
        CDI_FACTORY_RESET(cfg_.value_d);
        CDI_FACTORY_RESET(cfg_.enabled);
    }
    void handle_identify_global(const openlcb::EventRegistryEntry &registry_entry, 
                                openlcb::EventReport *event, 
                                BarrierNotifiable *done) override
    {
        if (event->dst_node && event->dst_node != node_)
        {
            return done->notify();
        }
        event->event_write_helper<1>()->WriteAsync(node_,
                                                   openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN,
                                                   openlcb::WriteHelper::global(),
                                                   openlcb::eventid_to_buffer(event_produced_), done->new_child());
        event->event_write_helper<2>()->WriteAsync(node_,
                                                   openlcb::Defs::MTI_CONSUMER_IDENTIFIED_UNKNOWN,
                                                   openlcb::WriteHelper::global(),
                                                   openlcb::eventid_to_buffer(event_consumed_), done->new_child());
        done->maybe_done();
    }
    void handle_identify_producer(const openlcb::EventRegistryEntry &registry_entry, 
                                  openlcb::EventReport *event, 
                                  BarrierNotifiable *done) override
    {
        if (event->dst_node && event->dst_node != node_)
        {
            return done->notify();
        }
        event->event_write_helper<3>()->WriteAsync(node_,
                                                   openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN,
                                                   openlcb::WriteHelper::global(),
                                                   openlcb::eventid_to_buffer(event_produced_), done->new_child());
        done->maybe_done();
    }
    void handle_identify_consumer(const openlcb::EventRegistryEntry &registry_entry, 
                                  openlcb::EventReport *event, 
                                  BarrierNotifiable *done) override
    {
        if (event->dst_node && event->dst_node != node_)
        {
            return done->notify();
        }
        event->event_write_helper<4>()->WriteAsync(node_,
                                                   openlcb::Defs::MTI_CONSUMER_IDENTIFIED_UNKNOWN,
                                                   openlcb::WriteHelper::global(),
                                                   openlcb::eventid_to_buffer(event_consumed_), done->new_child());
        done->maybe_done();
    }
    void handle_event_report(const openlcb::EventRegistryEntry &registry_entry, 
                             openlcb::EventReport *event, 
                             BarrierNotifiable *done) override
    {
        if (enabled_ && event->event == event_consumed_)
        {
            pnet::DimmerData td(slot_, value_a_, value_b_, value_c_, 
                                value_d_);
            pnet::GenMessage m;
            td.FillMessage(&m);
            dimmer_message_helper.WriteAsync(pnetstack_,
                                              m.identifier,
                                              m.payload,
                                              done);
        }
        else
        {
            done->notify();
        }
    }
    void process_dimmer(const pnet::DimmerData &td,
                         BarrierNotifiable *done)
    {
        dimmer_event_helper.WriteAsync(node_,openlcb::Defs::MTI_EVENT_REPORT,
                                        openlcb::WriteHelper::global(),
                                        openlcb::eventid_to_buffer(event_produced_),
                                        done);
    }
private:
    void register_event_handler()
    {
        openlcb::EventRegistry::instance()->register_handler(openlcb::EventRegistryEntry(this,event_consumed_), 0);
        openlcb::EventRegistry::instance()->register_handler(openlcb::EventRegistryEntry(this,event_produced_), 0);
    }
    void unregister_event_handler()
    {
        openlcb::EventRegistry::instance()->unregister_handler(this);
    }
    void register_dimmer_handler()
    {
        pnet::DimmerHandler::instance()->register_handler(pnet::DimmerRegistryEntry(this,pnet::DimmerData(slot_,value_a_,value_b_,value_c_,value_d_)));
    }
    void unregister_dimmer_handler()
    {
        pnet::DimmerHandler::instance()->unregister_handler(pnet::DimmerRegistryEntry(this,pnet::DimmerData(slot_,value_a_,value_b_,value_c_,value_d_)));
    }
    openlcb::Node *node_;
    const PCPNetDimmerConfig cfg_;
    pnet::PNETCanStack *pnetstack_;
    openlcb::EventId event_produced_;
    openlcb::EventId event_consumed_;
    bool enabled_;
    uint8_t slot_;
    uint8_t value_a_;
    uint8_t value_b_;
    uint8_t value_c_;
    uint8_t value_d_;
    openlcb::WriteHelper dimmer_event_helper;
    pnet::WriteHelper dimmer_message_helper;
    static vector<PCPNetDimmer *> dimmers;
public:
    template <unsigned N>
          __attribute__((noinline))
          static void Init(openlcb::Node *node, 
                           pnet::PNETCanStack *pnetstack,
                           const openlcb::RepeatedGroup<PCPNetDimmerConfig, N> &config,
                           unsigned size)
    {
        HASSERT(size == N);
        openlcb::ConfigReference offset_(config);
        openlcb::RepeatedGroup<PCPNetDimmerConfig, UINT_MAX> grp_ref(offset_.offset());
        for (unsigned i = 0; i < size; i++)
        {
            dimmers.push_back(new PCPNetDimmer(node,pnetstack,grp_ref.entry(i)));
        }
    }
};

#define DIMMERS vector<PCPNetDimmer *> PCPNetDimmer::dimmers

#endif // __CONFIGUREDPCPNETDIMMER_HXX

