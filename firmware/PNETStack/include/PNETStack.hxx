// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Mar 15 15:13:10 2023
//  Last Modified : <230315.1540>
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

#ifndef __PNETSTACK_HXX
#define __PNETSTACK_HXX

#include <fcntl.h>
#include "executor/Executor.hxx"
#include "utils/HubDevice.hxx"
#include "utils/HubDeviceNonBlock.hxx"
#include "utils/CanIf.hxx"
#include "utils/HubDeviceSelect.hxx"

namespace pnet
{

class PNETStackBase
{
protected:
    class PhysicalIf
    {
    public:
        virtual ~PhysicalIf()
        {
        }
        virtual If *iface() = 0;
    };
public:
    static const unsigned EXECUTOR_PRIORITIES = 5;
    
    PNETStackBase(
       std::function<std::unique_ptr<PhysicalIf>()> create_if_helper);
    Executor<EXECUTOR_PRIORITIES> *executor()
    {
        return &executor_;
    }
    Service *service()
    {
        return &service_;
    }
    If *iface()
    {
        return iface_;
    }
    void restart_stack();
    void loop_executor(bool delay_start = false)
    {
        start_stack(delay_start);
        executor_.thread_body();
    }
    void start_after_delay();
    void start_executor_thread(const char *name, int priority,
                               size_t stack_size, 
                               bool delay_start = false)
    {
        start_stack(delay_start);
        executor_.start_thread(name, priority, stack_size);
    }
protected:
    void start_stack(bool delay_start);
    virtual void start_iface(bool restart) = 0;
    Executor<EXECUTOR_PRIORITIES> executor_ {NO_THREAD()};
    Service service_ {&executor_};
    std::unique_ptr<PhysicalIf> ifaceHolder_;
    If *iface_ {ifaceHolder_->iface()};
    
    std::vector<std::unique_ptr<Destructable>> additionalComponents_;
};

    
class PNETCanStack : public PNETStackBase
{
public:
    PNETCanStack();
    CanHubFlow *can_hub()
    {
        return &static_cast<CanPhysicalIf *>(ifaceHolder_.get())->canHub0_;
    }
    void add_socketcan_port_select(const char *device, int loopback = 1);
    
};

}


#endif // __PNETSTACK_HXX

