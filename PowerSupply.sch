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
LIBS:ap63205
LIBS:LCC-PNET-Router-cache
EELAYER 25 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 4 4
Title "LCC PNET Gateway"
Date "3 mar 2019"
Rev "A"
Comp "Deepwoods Software"
Comment1 "Power Supply"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L AP63205 U401
U 1 1 6411E144
P 4275 4775
F 0 "U401" H 4275 4775 60  0000 C CNN
F 1 "AP63205" H 4275 4875 60  0000 C CNN
F 2 "TO_SOT_Packages_SMD:TSOT-23-6_MK06A" H 4275 4775 60  0001 C CNN
F 3 "" H 4275 4775 60  0001 C CNN
F 4 "621-AP63205WU-7" H 4275 4775 60  0001 C CNN "Mouser Part Number"
	1    4275 4775
	1    0    0    -1  
$EndComp
$Comp
L CP1_Small C401
U 1 1 6411E145
P 3600 4800
F 0 "C401" H 3610 4870 50  0000 L CNN
F 1 "10uf" H 3610 4720 50  0000 L CNN
F 2 "Capacitors_SMD:CP_Elec_5x5.3" H 3600 4800 50  0001 C CNN
F 3 "" H 3600 4800 50  0001 C CNN
F 4 "710-865230542002" H 3600 4800 60  0001 C CNN "Mouser Part Number"
	1    3600 4800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR029
U 1 1 6411E146
P 3600 5075
F 0 "#PWR029" H 3600 4825 50  0001 C CNN
F 1 "GND" H 3600 4925 50  0000 C CNN
F 2 "" H 3600 5075 50  0001 C CNN
F 3 "" H 3600 5075 50  0001 C CNN
	1    3600 5075
	1    0    0    -1  
$EndComp
$Comp
L C_Small C402
U 1 1 6411E147
P 4900 4675
F 0 "C402" H 4910 4745 50  0000 L CNN
F 1 "100nf" H 4910 4595 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4900 4675 50  0001 C CNN
F 3 "" H 4900 4675 50  0001 C CNN
F 4 "710-885012206020" H 4900 4675 60  0001 C CNN "Mouser Part Number"
	1    4900 4675
	1    0    0    -1  
$EndComp
$Comp
L L_Small L401
U 1 1 6411E148
P 5175 4775
F 0 "L401" H 5205 4815 50  0000 L CNN
F 1 "4.7uH" H 5205 4735 50  0000 L CNN
F 2 "Inductors_SMD:L_7.3x7.3_H4.5" H 5175 4775 50  0001 C CNN
F 3 "" H 5175 4775 50  0001 C CNN
F 4 "81-B1047AS-4R7NP3" H 5175 4775 60  0001 C CNN "Mouser Part Number"
	1    5175 4775
	0    1    1    0   
$EndComp
$Comp
L C_Small C403
U 1 1 6411E149
P 5375 4875
F 0 "C403" H 5385 4945 50  0000 L CNN
F 1 "22uf" H 5385 4795 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5375 4875 50  0001 C CNN
F 3 "" H 5375 4875 50  0001 C CNN
F 4 "603-CC0603MRX6S5B226" H 5375 4875 60  0001 C CNN "Mouser Part Number"
	1    5375 4875
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 4600 3850 4600
Wire Wire Line
	3850 4600 3850 4775
Wire Wire Line
	3600 4700 3600 4600
Connection ~ 3600 4600
Wire Wire Line
	3600 4900 3600 5075
Wire Wire Line
	4775 4550 4775 4600
Wire Wire Line
	4775 4550 4900 4550
Wire Wire Line
	4900 4550 4900 4575
Wire Wire Line
	4775 4775 5075 4775
Connection ~ 4900 4775
Wire Wire Line
	5275 4775 5600 4775
$Comp
L C_Small C404
U 1 1 6411E14A
P 5600 4875
F 0 "C404" H 5610 4945 50  0000 L CNN
F 1 "22uf" H 5610 4795 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5600 4875 50  0001 C CNN
F 3 "" H 5600 4875 50  0001 C CNN
F 4 "603-CC0603MRX6S5B226" H 5600 4875 60  0001 C CNN "Mouser Part Number"
	1    5600 4875
	1    0    0    -1  
$EndComp
Connection ~ 5375 4775
Wire Wire Line
	5600 4775 5600 4700
Wire Wire Line
	3875 5225 5375 5225
Wire Wire Line
	3875 5225 3875 5000
Wire Wire Line
	3875 5000 3600 5000
Connection ~ 3600 5000
Wire Wire Line
	5275 4950 4775 4950
Wire Wire Line
	5275 4950 5275 4775
Wire Wire Line
	5600 4975 5375 4975
Wire Wire Line
	5375 4975 5375 5225
Connection ~ 4275 5225
Text HLabel 3350 4600 0    60   Input ~ 0
12-15 Volt input
Text HLabel 5600 4700 2    60   Output ~ 0
5V Out
$EndSCHEMATC
