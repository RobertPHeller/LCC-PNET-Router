// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Mar 16 10:18:14 2023
//  Last Modified : <230317.1331>
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

#ifndef __PNETIFCAN_HXX
#define __PNETIFCAN_HXX

#include <memory>

#include "executor/StateFlow.hxx"
#include "PNETIf.hxx"
#include "PNETDefs.hxx"
#include "utils/CanIf.hxx"

namespace pnet
{

/// Implementation of the PNET interface abstraction for the CAN-bus
/// interface standard. This contains the parsers for CAN frames, dispatcher
/// for the different frame types, and the flows responsible for sending outgoing
/// messages. Inherits handling of the PNET-indexed messages from the base class
/// @ref If.
class IfCan : public If, public CanIf
{
public:
    /**
     * Creates a CAN interface.
     *
     * @param executor will be used to process incoming (and outgoing) messages.
     *
     * @param device is a CanHub. The interface will add a member to this pipe
     * to handle incoming and outgoing traffic. The caller should add the
     * necessary hardware device, GridConnect bridge or mock interface to this
     * pipe (before this constructor or else outgoing packets might be lost).
     *
     */
    IfCan(ExecutorBase *executor, CanHubFlow *device);

    ~IfCan();

    void add_owned_flow(Executable *e) override;
private:
    friend class CanFrameWriteFlow; // accesses the device and the hubport.
    
    
    /// Various implementation control flows that this interface owns.
    std::vector<std::unique_ptr<Executable>> ownedFlows_;

    DISALLOW_COPY_AND_ASSIGN(IfCan);
};

/** Base class for incoming CAN frame handlers. */
class CanFrameStateFlow : public StateFlow<Buffer<CanMessageData>, QList<1>>
{
public:
    CanFrameStateFlow(IfCan *service)
        : StateFlow<Buffer<CanMessageData>, QList<1>>(service)
    {
    }

    IfCan *if_can()
    {
        return static_cast<IfCan *>(service());
    }
};

}

#endif // __PNETIFCAN_HXX

