// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Mar 19 14:31:22 2023
//  Last Modified : <230320.1236>
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

#ifndef __CONFIGUREDPCPNETTRIGGER_HXX
#define __CONFIGUREDPCPNETTRIGGER_HXX

#include "openlcb/Node.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "PNETStack.hxx"
#include "PNETWriteHelper.hxx"
#include "PNETTriggerService.hxx"

CDI_GROUP(PCPNetTriggerConfig);
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<16>, //
                Name("Description"), 
                Description("User name of this trigger."))
CDI_GROUP_ENTRY(event_produced, openlcb::EventConfigEntry,
                Name("Event Produced"),
                Description("(P) This event is produced when a matching Trigger message is received."));
CDI_GROUP_ENTRY(event_consumed, openlcb::EventConfigEntry,
                Name("Event Consumed"),
                Description("(C) This event will cause the defined Trigger to be sent."));
CDI_GROUP_ENTRY(enabled, openlcb::Uint8ConfigEntry,
                Min(0), Max(1), Default(0),
                MapValues("<relation><property>0</property><value>No</value></relation>"
                          "<relation><property>1</property><value>Yes</value></relation>"),
                Name("Enable"), Description("Enable this trigger."));
CDI_GROUP_ENTRY(slot, openlcb::Uint8ConfigEntry,
                Min(0), Max(31), Default(0),
                Name("Slot"),
                Description("The trigger slot number (0-31)."));
CDI_GROUP_ENTRY(trigger, openlcb::Uint8ConfigEntry,
                Min(1), Max(4), Default(1),
                Name("Trigger Number"),
                Description("The trigger number."));
CDI_GROUP_END();

class PCPNetTrigger : public DefaultConfigUpdateListener,
                           public openlcb::SimpleEventHandler,
                           public pnet::TriggerProcess
{
public:
    PCPNetTrigger(openlcb::Node *node, pnet::PNETCanStack *pnetstack,
                  const PCPNetTriggerConfig &cfg)
                : DefaultConfigUpdateListener()
          , node_(node)
          , cfg_(cfg)
          , pnetstack_(pnetstack)
          , event_produced_(0)
          , event_consumed_(0)
          , enabled_(false)
          , slot_(0)
          , trigger_(1)
    {
        if (enabled_) register_trigger_handler();
    }
    ~PCPNetTrigger()
    {
        if (enabled_) unregister_trigger_handler();
    }
    UpdateAction apply_configuration(int fd, 
                                     bool initial_load, 
                                     BarrierNotifiable *done) OVERRIDE
    {
        AutoNotify n(done);
        const openlcb::EventId cfg_event_produced = cfg_.event_produced().read(fd);
        const openlcb::EventId cfg_event_consumed = cfg_.event_consumed().read(fd);
        const uint8_t cfg_slot = cfg_.slot().read(fd);
        const uint8_t cfg_trigger = cfg_.trigger().read(fd);
        bool cfg_enabled = (bool) cfg_.enabled().read(fd);
        if (cfg_slot != slot_ || cfg_trigger != trigger_ ||
            cfg_enabled != enabled_)
        {
            if (enabled_) unregister_trigger_handler();
            slot_ = cfg_slot;
            trigger_ = cfg_trigger;
            enabled_ = cfg_enabled;
            if (enabled_) register_trigger_handler();
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
        CDI_FACTORY_RESET(cfg_.trigger);
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
            pnet::TriggerData td(slot_, trigger_);
            pnet::GenMessage m;
            td.FillMessage(&m);
            trigger_message_helper.WriteAsync(pnetstack_,
                                              m.identifier,
                                              m.payload,
                                              done);
        }
        else
        {
            done->notify();
        }
    }
    void process_trigger(const pnet::TriggerData &td,
                         BarrierNotifiable *done)
    {
        trigger_event_helper.WriteAsync(node_,openlcb::Defs::MTI_EVENT_REPORT,
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
    void register_trigger_handler()
    {
        pnet::TriggerHandler::instance()->register_handler(pnet::TriggerRegistryEntry(this,pnet::TriggerData(slot_,trigger_)));
    }
    void unregister_trigger_handler()
    {
        pnet::TriggerHandler::instance()->unregister_handler(pnet::TriggerRegistryEntry(this,pnet::TriggerData(slot_,trigger_)));
    }
    openlcb::Node *node_;
    const PCPNetTriggerConfig cfg_;
    pnet::PNETCanStack *pnetstack_;
    openlcb::EventId event_produced_;
    openlcb::EventId event_consumed_;
    bool enabled_;
    uint8_t slot_;
    uint8_t trigger_;
    openlcb::WriteHelper trigger_event_helper;
    pnet::WriteHelper trigger_message_helper;
    static vector<PCPNetTrigger *> triggers;
public:
    template <unsigned N>
          __attribute__((noinline))
          static void Init(openlcb::Node *node, 
                           pnet::PNETCanStack *pnetstack,
                           const openlcb::RepeatedGroup<PCPNetTriggerConfig, N> &config,
                           unsigned size)
    {
        HASSERT(size == N);
        openlcb::ConfigReference offset_(config);
        openlcb::RepeatedGroup<PCPNetTriggerConfig, UINT_MAX> grp_ref(offset_.offset());
        for (unsigned i = 0; i < size; i++)
        {
            triggers.push_back(new PCPNetTrigger(node,pnetstack,grp_ref.entry(i)));
        }
    }
};

#define TRIGGERS vector<PCPNetTrigger *> PCPNetTrigger::triggers

#endif // __CONFIGUREDPCPNETTRIGGER_HXX

