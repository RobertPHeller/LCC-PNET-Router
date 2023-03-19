// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Mar 16 16:37:58 2023
//  Last Modified : <230319.1229>
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

static const char rcsid[] = "@(#) : $Id$";

#if defined(__linux__) || defined(__MACH__)
#include <net/if.h>
#include <termios.h> /* tc* functions */
#endif
#if defined(__linux__)
#include "utils/HubDeviceSelect.hxx"
#include <linux/sockios.h>
#include <sys/ioctl.h>
#endif

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "PNETStack.hxx"

namespace pnet
{

PNETStackBase::PNETStackBase(std::function<std::unique_ptr<PhysicalIf>()> create_if_helper,
                             ExecutorBase *executor)
      : executor_(executor)
, service_(executor_)
, ifaceHolder_(create_if_helper())
{
}

PNETCanStack::PNETCanStack(ExecutorBase *executor)
      : PNETStackBase(std::bind(&PNETCanStack::create_if, this),executor)
{
}

std::unique_ptr<PNETCanStack::PhysicalIf> PNETCanStack::create_if()
{
    return std::unique_ptr<PhysicalIf>(new CanPhysicalIf(service()));
}

void PNETCanStack::start_iface(bool restart)
{
    if (restart)
    {
    }
}

void PNETCanStack::add_socketcan_port_select(const char *device, int loopback)
{
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    
    // Set the blocking limit to the minimum allowed, typically 1024 in Linux
    int sndbuf = 0;
    setsockopt(s, SOL_SOCKET, SO_SNDBUF, &sndbuf, sizeof(sndbuf));
    
    // turn on/off loopback
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback));
    
    // setup error notifications
    can_err_mask_t err_mask = CAN_ERR_TX_TIMEOUT | CAN_ERR_LOSTARB |
          CAN_ERR_CRTL | CAN_ERR_PROT | CAN_ERR_TRX | CAN_ERR_ACK |
          CAN_ERR_BUSOFF | CAN_ERR_BUSERROR | CAN_ERR_RESTARTED;
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, &err_mask, sizeof(err_mask));
    strcpy(ifr.ifr_name, device);
    
    ::ioctl(s, SIOCGIFINDEX, &ifr);
    
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    
    bind(s, (struct sockaddr *)&addr, sizeof(addr));
    
    auto *port = new HubDeviceSelect<CanHubFlow>(can_hub(), s);
    additionalComponents_.emplace_back(port);
}

}
