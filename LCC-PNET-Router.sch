EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:pocketbeagle
LIBS:lm2574n-5
LIBS:sn65hvd233-ht
LIBS:LCC-PNET-Router-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 4
Title ""
Date "3 mar 2019"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L POCKETBEAGLE-P1 P1
U 1 1 5C7C1FE0
P 2850 2550
F 0 "P1" H 2850 2550 60  0000 C CNN
F 1 "POCKETBEAGLE-P1" H 2850 3700 60  0000 C CNN
F 2 "" H 2850 2550 60  0000 C CNN
F 3 "" H 2850 2550 60  0000 C CNN
	1    2850 2550
	1    0    0    -1  
$EndComp
$Comp
L POCKETBEAGLE-P2 P2
U 1 1 5C7C1FEF
P 5150 2550
F 0 "P2" H 5150 2550 60  0000 C CNN
F 1 "POCKETBEAGLE-P2" H 5150 3700 60  0000 C CNN
F 2 "" H 5150 2550 60  0000 C CNN
F 3 "" H 5150 2550 60  0000 C CNN
	1    5150 2550
	1    0    0    -1  
$EndComp
$Sheet
S 1375 4175 2150 1875
U 63C030FD
F0 "LCC CAN Transceiver" 60
F1 "LCC-CAN-Transceiver.sch" 60
F2 "LCC_CAN_TX" I R 3525 4300 60 
F3 "LCC_CAN_RX" O R 3525 4500 60 
F4 "LCC_CAN_12V" O R 3525 5000 60 
$EndSheet
$Sheet
S 4375 4225 2275 1825
U 63C034A4
F0 "PNET CAN Transceiver" 60
F1 "PNET-CAN-Transceiver.sch" 60
F2 "PNET_CAN_TX" I L 4375 4350 60 
F3 "PNET_CAN_RX" O L 4375 4575 60 
$EndSheet
$Sheet
S 7575 4025 2625 1800
U 63C062DF
F0 "Power Supply" 60
F1 "PowerSupply.sch" 60
F2 "5V Out" O L 7575 4225 60 
F3 "12-15 Volt input" I L 7575 4475 60 
$EndSheet
Text Label 7575 4475 2    60   ~ 0
CAN_12V
Text Label 3525 5000 0    60   ~ 0
CAN_12V
$Comp
L +5V #PWR01
U 1 1 63C08525
P 7500 4225
F 0 "#PWR01" H 7500 4075 50  0001 C CNN
F 1 "+5V" H 7500 4365 50  0000 C CNN
F 2 "" H 7500 4225 50  0001 C CNN
F 3 "" H 7500 4225 50  0001 C CNN
	1    7500 4225
	1    0    0    -1  
$EndComp
Wire Wire Line
	7575 4225 7500 4225
$Comp
L +5V #PWR02
U 1 1 63C0874C
P 2400 1650
F 0 "#PWR02" H 2400 1500 50  0001 C CNN
F 1 "+5V" H 2400 1790 50  0000 C CNN
F 2 "" H 2400 1650 50  0001 C CNN
F 3 "" H 2400 1650 50  0001 C CNN
	1    2400 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 1650 2275 1650
Wire Wire Line
	2275 1650 2275 1950
Wire Wire Line
	2275 1850 2400 1850
$Comp
L USB_A J101
U 1 1 63C087AA
P 1675 2150
F 0 "J101" H 1475 2600 50  0000 L CNN
F 1 "USB_A" H 1475 2500 50  0000 L CNN
F 2 "" H 1825 2100 50  0001 C CNN
F 3 "" H 1825 2100 50  0001 C CNN
	1    1675 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 2150 1975 2150
Wire Wire Line
	2275 1950 1975 1950
Connection ~ 2275 1850
Wire Wire Line
	1975 2250 2125 2250
Wire Wire Line
	2125 2250 2125 2050
Wire Wire Line
	2125 2050 2400 2050
Wire Wire Line
	2400 2250 2400 2350
$Comp
L GND #PWR03
U 1 1 63C08982
P 1575 2700
F 0 "#PWR03" H 1575 2450 50  0001 C CNN
F 1 "GND" H 1575 2550 50  0000 C CNN
F 2 "" H 1575 2700 50  0001 C CNN
F 3 "" H 1575 2700 50  0001 C CNN
	1    1575 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1575 2550 1575 2700
Wire Wire Line
	1675 2550 1575 2550
Wire Wire Line
	2400 2350 1975 2350
Wire Wire Line
	1975 2350 1975 2700
Wire Wire Line
	1975 2700 1575 2700
$Comp
L +3V3 #PWR04
U 1 1 63C08B7C
P 3450 2250
F 0 "#PWR04" H 3450 2100 50  0001 C CNN
F 1 "+3V3" H 3450 2390 50  0000 C CNN
F 2 "" H 3450 2250 50  0001 C CNN
F 3 "" H 3450 2250 50  0001 C CNN
	1    3450 2250
	0    1    1    0   
$EndComp
Wire Wire Line
	3450 2250 3300 2250
$Comp
L +3V3 #PWR05
U 1 1 63C08EDB
P 4575 2750
F 0 "#PWR05" H 4575 2600 50  0001 C CNN
F 1 "+3V3" H 4575 2890 50  0000 C CNN
F 2 "" H 4575 2750 50  0001 C CNN
F 3 "" H 4575 2750 50  0001 C CNN
	1    4575 2750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4700 2750 4575 2750
Wire Wire Line
	4700 2350 4525 2350
Wire Wire Line
	4525 2350 4525 2650
Wire Wire Line
	4525 2650 4700 2650
Wire Wire Line
	3300 2350 3475 2350
Wire Wire Line
	3475 2350 3475 2650
Wire Wire Line
	3475 2650 3300 2650
$Comp
L GND #PWR06
U 1 1 63C08FA9
P 4425 2450
F 0 "#PWR06" H 4425 2200 50  0001 C CNN
F 1 "GND" H 4425 2300 50  0000 C CNN
F 2 "" H 4425 2450 50  0001 C CNN
F 3 "" H 4425 2450 50  0001 C CNN
	1    4425 2450
	0    1    1    0   
$EndComp
Wire Wire Line
	4425 2450 4525 2450
Connection ~ 4525 2450
$Comp
L GND #PWR07
U 1 1 63C09020
P 3575 2450
F 0 "#PWR07" H 3575 2200 50  0001 C CNN
F 1 "GND" H 3575 2300 50  0000 C CNN
F 2 "" H 3575 2450 50  0001 C CNN
F 3 "" H 3575 2450 50  0001 C CNN
	1    3575 2450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3575 2450 3475 2450
Connection ~ 3475 2450
Text Label 3300 2850 0    60   ~ 0
CAN0_TX
Text Label 3300 2950 0    60   ~ 0
CAN0_RX
Text Label 4700 2050 2    60   ~ 0
CAN1_RX
Text Label 4700 2150 2    60   ~ 0
CAN1_TX
Text Label 3525 4300 0    60   ~ 0
CAN0_TX
Text Label 3525 4500 0    60   ~ 0
CAN0_RX
Text Label 4375 4350 2    60   ~ 0
CAN1_TX
Text Label 4375 4575 2    60   ~ 0
CAN1_RX
NoConn ~ 2400 1750
NoConn ~ 2400 1950
NoConn ~ 2400 2450
NoConn ~ 2400 2550
NoConn ~ 2400 2650
NoConn ~ 2400 2750
NoConn ~ 2400 2850
NoConn ~ 2400 2950
NoConn ~ 2400 3050
NoConn ~ 2400 3150
NoConn ~ 2400 3250
NoConn ~ 2400 3350
NoConn ~ 3300 3350
NoConn ~ 4700 3350
NoConn ~ 4700 3250
NoConn ~ 4700 3150
NoConn ~ 4700 3050
NoConn ~ 4700 2950
NoConn ~ 3300 3050
NoConn ~ 3300 3150
NoConn ~ 3300 3250
NoConn ~ 4700 2850
NoConn ~ 3300 2750
NoConn ~ 4700 2550
NoConn ~ 4700 2450
NoConn ~ 3300 2450
NoConn ~ 3300 2550
NoConn ~ 4700 1650
NoConn ~ 4700 1750
NoConn ~ 4700 1850
NoConn ~ 4700 1950
NoConn ~ 4700 2250
NoConn ~ 3300 1650
NoConn ~ 3300 1750
NoConn ~ 3300 1850
NoConn ~ 3300 1950
NoConn ~ 3300 2050
NoConn ~ 3300 2150
NoConn ~ 5600 1650
NoConn ~ 5600 1750
NoConn ~ 5600 1850
NoConn ~ 5600 1950
NoConn ~ 5600 2050
NoConn ~ 5600 2150
NoConn ~ 5600 2250
NoConn ~ 5600 2350
NoConn ~ 5600 2450
NoConn ~ 5600 2550
NoConn ~ 5600 2650
NoConn ~ 5600 2750
NoConn ~ 5600 2850
NoConn ~ 5600 2950
NoConn ~ 5600 3050
NoConn ~ 5600 3150
NoConn ~ 5600 3250
NoConn ~ 5600 3350
$EndSCHEMATC
