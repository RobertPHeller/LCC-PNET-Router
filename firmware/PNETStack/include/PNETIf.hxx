// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Mar 16 14:38:39 2023
//  Last Modified : <230316.1628>
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

#ifndef __PNETIF_HXX
#define __PNETIF_HXX

#include <string>

#include "PNETDefs.hxx"
#include "executor/Dispatcher.hxx"
#include "executor/Service.hxx"
#include "executor/Executor.hxx"
#include "executor/StateFlow.hxx"
#include "utils/Buffer.hxx"
#include "utils/Queue.hxx"
#include "utils/Map.hxx"

namespace pnet
{

/// Container that carries the data bytes in a PNET message.
typedef string Payload;

/** A global class / variable for empty or not-yet-initialized payloads. */
extern string EMPTY_PAYLOAD;

struct GenMessage
{
    GenMessage()
          : identifier((Defs::Identifier)0), flagsSrc(0), flagsDst(0)
    {
    }
    
    void clear()
    {
        reset((Defs::Identifier)0, EMPTY_PAYLOAD);
    }
    
    void reset(Defs::Identifier id, string payload)
    {
        this->identifier = id;
        this->payload = std::move(payload);
        this->flagsSrc = 0;
        this->flagsDst = 0;
    }
    
    // PNET Message ID.
    Defs::Identifier identifier;
    // Data content in the message body. Owned by the dispatcher.
    string payload;
    unsigned flagsSrc : 4;
    unsigned flagsDst : 4;
    unsigned get_flags_src() {
        return flagsSrc;
    }
    unsigned get_flags_dst() {
        return flagsDst;
    }
    void set_flag_src(unsigned flags) {
        flagsSrc |= flags;
    }
    void clear_flag_src(unsigned flags) {
        flagsSrc &= ~flags;
    }
        /** Returns true if src flags has all the specified flags set. */
    bool has_flag_src(unsigned flags) {
        return ((flagsSrc & flags) == flags);
    }
    void set_flag_dst(unsigned flags) {
        flagsDst |= flags;
    }
    void clear_flag_dst(unsigned flags) {
        flagsDst &= ~flags;
    }
    /** Returns true if src flags has all the specified flags set. */
    bool has_flag_dst(unsigned flags) {
        return ((flagsDst & flags) == flags);
    }

    typedef uint32_t id_type;
    id_type id() const
    {
        return static_cast<uint32_t>(identifier);
    }
    enum DstFlags {
        // 1, 2, 4, 8: free
    };
    enum SrcFlags {
        // 1, 2, 4, 8: free
    };
};

/// Interface class for all handlers that can be registered in the dispatcher
/// to receive incoming PNET messages.
typedef FlowInterface<Buffer<GenMessage>> MessageHandler;

/// Abstract class representing an PNET Interface. All interaction between
/// the local software stack and the physical bus has to go through this
/// class. The API that's not specific to the wire protocol appears here. The
/// implementations of this class would be specific to the wire protocol
/// (e.g. IfCan for CAN, and a not-yet-implemented class for TCP).
class If : public Service
{
public:
    /** Constructs an PNET interface.
     * @param executor is the thread that will be used for all processing on
     * this interface.
     */
    If(ExecutorBase *executor);

    /** Destructor */
    virtual ~If()
    {
    }

    /** @return Flow to send global messages to the PNET bus. */
    MessageHandler *global_message_write_flow()
    {
        HASSERT(globalWriteFlow_);
        return globalWriteFlow_;
    }

    /** Type of the dispatcher of incoming PNET messages. */
    typedef DispatchFlow<Buffer<GenMessage>, 4> MessageDispatchFlow;

    /** @return Dispatcher of incoming PNET messages. */
    MessageDispatchFlow *dispatcher()
    {
        return &dispatcher_;
    }

    /** Transfers ownership of a module to the interface. It will be brought
     * down in the destructor. The destruction order is guaranteed such that
     * all supporting structures are still available when the flow is destryed,
     * but incoming messages can not come in anymore.
     *
     * @todo(balazs.racz) revise whether this needs to be virtual. */
    virtual void add_owned_flow(Executable *e) = 0;

protected:
    /// Allocator containing the global write flows.
    MessageHandler *globalWriteFlow_;

private:
    /// Flow responsible for routing incoming messages to handlers.
    MessageDispatchFlow dispatcher_;

    DISALLOW_COPY_AND_ASSIGN(If);
};

/// Message handlers that are implemented as state flows should derive from
/// this class.
typedef StateFlow<Buffer<GenMessage>, QList<4>> MessageStateFlowBase;

/** Base class for incoming message handler flows. */
class IncomingMessageStateFlow
    : public MessageStateFlowBase
{
public:
    IncomingMessageStateFlow(If *iface)
        : MessageStateFlowBase(iface)
    {
    }

    If *iface()
    {
        return static_cast<If *>(service());
    }

    /// Returns the PNET message we received.
    GenMessage *nmsg()
    {
        return message()->data();
    }
};

}

#endif // __PNETIF_HXX

