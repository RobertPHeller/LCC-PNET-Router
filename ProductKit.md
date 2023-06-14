This is a "router" between a LCC network and a Pricom Lighting PNET network.
It translates between LCC event reports and PNET Trigger, Control, and Dimmer 
messages.  The PCB contains CAN transceivers for the two CAN IFs on a 
PocketBeagle, with one wired to LCC network connectors (RJ45s) and the other 
wired for PNET (RJ12 6-6).  Both networks have data activity LEDs and an 
on-board termination jumper header.  And the LCC network has power injection
and tap screw terminals.

This kit contains:

- The PCB with all of the SMD parts assembled.
- The through-hole parts (to be soldered on by the end user):
    - 2 RJ45 8-8 connectors
    - 2 RJ12 6-6 connectors
    - 2 2.54mm (.1") screw terminal blocks
    - 2 2x18 2.54mm (.1") pitch socket headers
    - 1 2x40 2.54mm (.1") pitch "breakaway" pin headers
    - 1 USB A connector
- 1 MicroSD card containing the firmware image
- 4 2.54mm (.1") shorting jumpers (for the termination headers)
 
Not included: a [PocketBeagle](https://duckduckgo.com/?t=ffab&q=pocketBeagle&iax=shopping&ia=shopping).

The included firmware is built to provide 32 each of PNET controls, triggers,
and dimmers.  Each PNET message item maps two LCC events to the PNET message,
one eventid is consumed to generate the PNET message and the other is produced
when the matching PNET message is seen on the PNET network.  The PNET message
processing is configured using the LCC CDI configuration (eg with JMRI's 
PanelPro or DWS's OpenLCB program).

