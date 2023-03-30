/* -*- C -*- //////////////////////////////////////////////////////////////
 *
 *  System        : 
 *  Module        : 
 *  Object Name   : $RCSfile$
 *  Revision      : $Revision$
 *  Date          : $Date$
 *  Author        : $Author$
 *  Created By    : Robert Heller
 *  Created       : Thr Mar 30 10:49:54 2023
 *  Last Modified : <230330.1658>
 *
 *  Description	
 *
 *  Notes
 *
 *  History
 *	
 ****************************************************************************/
/** @copyright
 *  Copyright (c) $YEAR$, Robert Heller
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are  permitted provided that the following conditions are met:
 * 
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 * 
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 * 
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *  @file KitBooklet.h
 *  @author Robert Heller
 *  @date Thr Mar 30 10:49:54 2023
 * 
 * @page assembly Assembly
 * 
 * Assembly is straight forward.  The SMD parts are already soldered,
 * so only the through hole part need to be soldered to the board. 
 * Start with the shortese parts and work towards the tallest.  The
 * pin headers are in the form of a single 2x40 "break away" header.
 * This header can be cut with a pair of wire cutters.  You will need
 * two 2x2 and two 2x18 headers.  The two 2x2 headers are for the 
 * termination headers and the two 2x18 headers will go in the Pocket
 * Beagle.  Also be sure to face the wire openings of the screw 
 * terminals out from the board.
 * 
 * @image html LCC-PNET-ROUTER-3D.png
 * @image latex LCC-PNET-ROUTER-3D.png "3D top view of the PCB"
 * 
 * @page download Downloadables and Software Support
 * 
 * The included microSD card contains the code under  
 * @c /opt/LCC-PNET-Router.  The image also has the necessary boot
 * overlays and such set up to enable and start both CAN IFs.  As
 * shipped, the router firmware is not started, since local 
 * configuration is required.
 */ 
/* More documentation needed: 
   https://linuxhandbook.com/create-systemd-services/.
 */
/**
 * 
 * @page wiring General Wiring Notes
 * @section termination Termination of the networks.
 * @section connecting Connecting the networks,
 * @section usbhost USB Host connector.
 * 
 ****************************************************************************/
