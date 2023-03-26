# LCC-PNET-Router

This is a "router" between a LCC network and a Pricom Lighting PNET network.
It translates between LCC event reports and PNET Trigger, Control, and Dimmer 
messages.  The PCB contains CAN transceivers for the two CAN IFs on a 
PocketBeagle, with one wired to LCC network connectors (RJ45s) and the other 
wired for PNET (RJ12 6-6).  The "firmware" compiles and runs on a 32-bit ARM
Linux SBC (tested on a Beagle Bone using GridConnect over Tcp/Ip for the LCC
network and the Virtual CAN driver (vcan) for the PNET network.
