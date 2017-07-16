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
LIBS:csr8645
LIBS:th3122
LIBS:bmw_bt_cdcemu_analog-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "IBus BT analog module"
Date "2017-06-01"
Rev "1"
Comp "the vty"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L NE5532 U1
U 1 1 59064EC9
P 6475 3875
F 0 "U1" H 6475 4075 50  0000 L CNN
F 1 "NE5532" H 6475 3675 50  0000 L CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_Socket_LongPads" H 6475 3875 50  0001 C CNN
F 3 "" H 6475 3875 50  0000 C CNN
	1    6475 3875
	1    0    0    1   
$EndComp
$Comp
L NE5532 U1
U 2 1 590651E2
P 6475 2575
F 0 "U1" H 6475 2775 50  0000 L CNN
F 1 "NE5532" H 6475 2375 50  0000 L CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_Socket_LongPads" H 6475 2575 50  0001 C CNN
F 3 "" H 6475 2575 50  0000 C CNN
	2    6475 2575
	1    0    0    1   
$EndComp
$Comp
L R_Small R1
U 1 1 5906529C
P 6300 2000
F 0 "R1" V 6200 1875 50  0000 L CNN
F 1 "10k" V 6200 2025 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" H 6300 2000 50  0001 C CNN
F 3 "" H 6300 2000 50  0000 C CNN
	1    6300 2000
	0    1    1    0   
$EndComp
$Comp
L C_Small C1
U 1 1 590653C7
P 6300 2125
F 0 "C1" V 6250 2000 50  0000 L CNN
F 1 "33p" V 6250 2175 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 6300 2125 50  0001 C CNN
F 3 "" H 6300 2125 50  0000 C CNN
	1    6300 2125
	0    1    1    0   
$EndComp
$Comp
L R_Small R7
U 1 1 590656C8
P 5900 2675
F 0 "R7" V 5975 2600 50  0000 L CNN
F 1 "10k" V 5800 2625 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" H 5900 2675 50  0001 C CNN
F 3 "" H 5900 2675 50  0000 C CNN
	1    5900 2675
	0    1    1    0   
$EndComp
$Comp
L R_Small R4
U 1 1 5906574F
P 5900 2475
F 0 "R4" V 5725 2425 50  0000 L CNN
F 1 "10k" V 5800 2425 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" H 5900 2475 50  0001 C CNN
F 3 "" H 5900 2475 50  0000 C CNN
	1    5900 2475
	0    1    1    0   
$EndComp
$Comp
L R_Small R8
U 1 1 59066501
P 6025 2925
F 0 "R8" H 6075 2875 50  0000 L CNN
F 1 "10k" H 6050 3025 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" H 6025 2925 50  0001 C CNN
F 3 "" H 6025 2925 50  0000 C CNN
	1    6025 2925
	-1   0    0    1   
$EndComp
$Comp
L C_Small C8
U 1 1 590665B0
P 6175 2925
F 0 "C8" H 6200 2875 50  0000 L CNN
F 1 "33p" H 6025 3000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 6175 2925 50  0001 C CNN
F 3 "" H 6175 2925 50  0000 C CNN
	1    6175 2925
	-1   0    0    1   
$EndComp
Text GLabel 6025 3175 0    60   Input ~ 0
VGND
Text GLabel 5375 2675 0    60   Input ~ 0
SPK_LP
Text GLabel 5375 2475 0    60   Input ~ 0
SPK_LN
$Comp
L CP_Small C4
U 1 1 590771BE
P 5625 2475
F 0 "C4" V 5525 2425 50  0000 L CNN
F 1 "1u" V 5525 2550 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D5.0mm_P2.50mm" H 5625 2475 50  0001 C CNN
F 3 "" H 5625 2475 50  0000 C CNN
	1    5625 2475
	0    1    1    0   
$EndComp
$Comp
L CP_Small C5
U 1 1 59077635
P 5625 2675
F 0 "C5" V 5525 2625 50  0000 L CNN
F 1 "1u" V 5525 2750 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D5.0mm_P2.50mm" H 5625 2675 50  0001 C CNN
F 3 "" H 5625 2675 50  0000 C CNN
	1    5625 2675
	0    1    1    0   
$EndComp
$Comp
L R_Small R11
U 1 1 59078693
P 6300 3300
F 0 "R11" V 6200 3175 50  0000 L CNN
F 1 "10k" V 6200 3325 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" H 6300 3300 50  0001 C CNN
F 3 "" H 6300 3300 50  0000 C CNN
	1    6300 3300
	0    1    1    0   
$EndComp
$Comp
L C_Small C9
U 1 1 59078699
P 6300 3425
F 0 "C9" V 6250 3300 50  0000 L CNN
F 1 "33p" V 6250 3475 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 6300 3425 50  0001 C CNN
F 3 "" H 6300 3425 50  0000 C CNN
	1    6300 3425
	0    1    1    0   
$EndComp
$Comp
L R_Small R14
U 1 1 5907869F
P 5900 3975
F 0 "R14" V 5975 3925 50  0000 L CNN
F 1 "10k" V 5800 3925 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" H 5900 3975 50  0001 C CNN
F 3 "" H 5900 3975 50  0000 C CNN
	1    5900 3975
	0    1    1    0   
$EndComp
$Comp
L R_Small R13
U 1 1 590786A5
P 5900 3775
F 0 "R13" V 5700 3750 50  0000 L CNN
F 1 "10k" V 5800 3725 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" H 5900 3775 50  0001 C CNN
F 3 "" H 5900 3775 50  0000 C CNN
	1    5900 3775
	0    1    1    0   
$EndComp
$Comp
L R_Small R15
U 1 1 590786AB
P 6025 4225
F 0 "R15" H 6075 4175 50  0000 L CNN
F 1 "10k" H 6050 4325 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" H 6025 4225 50  0001 C CNN
F 3 "" H 6025 4225 50  0000 C CNN
	1    6025 4225
	-1   0    0    1   
$EndComp
$Comp
L C_Small C17
U 1 1 590786B1
P 6175 4225
F 0 "C17" H 6200 4175 50  0000 L CNN
F 1 "33p" H 6025 4300 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 6175 4225 50  0001 C CNN
F 3 "" H 6175 4225 50  0000 C CNN
	1    6175 4225
	-1   0    0    1   
$EndComp
Text GLabel 6025 4475 0    60   Input ~ 0
VGND
Text GLabel 5375 3975 0    60   Input ~ 0
SPK_RP
Text GLabel 5375 3775 0    60   Input ~ 0
SPK_RN
$Comp
L CP_Small C12
U 1 1 590786CB
P 5625 3775
F 0 "C12" V 5525 3625 50  0000 L CNN
F 1 "1u" V 5525 3800 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D5.0mm_P2.50mm" H 5625 3775 50  0001 C CNN
F 3 "" H 5625 3775 50  0000 C CNN
	1    5625 3775
	0    1    1    0   
$EndComp
$Comp
L CP_Small C13
U 1 1 590786D1
P 5625 3975
F 0 "C13" V 5525 3825 50  0000 L CNN
F 1 "1u" V 5525 4000 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D5.0mm_P2.50mm" H 5625 3975 50  0001 C CNN
F 3 "" H 5625 3975 50  0000 C CNN
	1    5625 3975
	0    1    1    0   
$EndComp
$Comp
L CP_Small C3
U 1 1 5907A398
P 6875 2575
F 0 "C3" V 6675 2500 50  0000 L CNN
F 1 "10u" V 6775 2500 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D5.0mm_P2.50mm" H 6875 2575 50  0001 C CNN
F 3 "" H 6875 2575 50  0000 C CNN
	1    6875 2575
	0    -1   -1   0   
$EndComp
$Comp
L CP_Small C11
U 1 1 5907A7CD
P 6875 3875
F 0 "C11" V 6675 3800 50  0000 L CNN
F 1 "10u" V 6775 3800 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D5.0mm_P2.50mm" H 6875 3875 50  0001 C CNN
F 3 "" H 6875 3875 50  0000 C CNN
	1    6875 3875
	0    -1   -1   0   
$EndComp
$Comp
L CONN_01X03 P2
U 1 1 5907B070
P 7575 3250
F 0 "P2" H 7575 3450 50  0000 C CNN
F 1 "AUD_OUT" V 7675 3250 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm" H 7575 3250 50  0001 C CNN
F 3 "" H 7575 3250 50  0000 C CNN
	1    7575 3250
	1    0    0    -1  
$EndComp
$Comp
L GNDA #PWR01
U 1 1 5907BB65
P 6375 2275
F 0 "#PWR01" H 6375 2025 50  0001 C CNN
F 1 "GNDA" H 6375 2125 50  0000 C CNN
F 2 "" H 6375 2275 50  0000 C CNN
F 3 "" H 6375 2275 50  0000 C CNN
	1    6375 2275
	0    1    1    0   
$EndComp
$Comp
L GNDA #PWR03
U 1 1 5908CE6F
P 6375 3575
F 0 "#PWR03" H 6375 3325 50  0001 C CNN
F 1 "GNDA" H 6375 3425 50  0000 C CNN
F 2 "" H 6375 3575 50  0000 C CNN
F 3 "" H 6375 3575 50  0000 C CNN
	1    6375 3575
	0    1    1    0   
$EndComp
$Comp
L R_Small R19
U 1 1 5908D540
P 5075 6050
F 0 "R19" H 5105 6070 50  0000 L CNN
F 1 "2.2k" H 5105 6010 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" H 5075 6050 50  0001 C CNN
F 3 "" H 5075 6050 50  0000 C CNN
	1    5075 6050
	1    0    0    -1  
$EndComp
$Comp
L CP_Small C32
U 1 1 5908D700
P 5375 6600
F 0 "C32" H 5225 6550 50  0000 L CNN
F 1 "47u" H 5175 6625 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D5.0mm_P2.50mm" H 5375 6600 50  0001 C CNN
F 3 "" H 5375 6600 50  0000 C CNN
	1    5375 6600
	-1   0    0    1   
$EndComp
$Comp
L C_Small C33
U 1 1 5908DB91
P 5650 6600
F 0 "C33" H 5550 6550 50  0000 L CNN
F 1 "100n" H 5400 6625 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 5650 6600 50  0001 C CNN
F 3 "" H 5650 6600 50  0000 C CNN
	1    5650 6600
	-1   0    0    1   
$EndComp
Text GLabel 5650 6325 1    60   Input ~ 0
VGND
$Comp
L CP_Small C29
U 1 1 5909706E
P 4500 6300
F 0 "C29" H 4525 6375 50  0000 L CNN
F 1 "100u" H 4525 6225 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D6.3mm_P2.50mm" H 4500 6300 50  0001 C CNN
F 3 "" H 4500 6300 50  0000 C CNN
	1    4500 6300
	-1   0    0    -1  
$EndComp
$Comp
L GNDA #PWR06
U 1 1 59098D4F
P 7375 3250
F 0 "#PWR06" H 7375 3000 50  0001 C CNN
F 1 "GNDA" H 7375 3100 50  0000 C CNN
F 2 "" H 7375 3250 50  0000 C CNN
F 3 "" H 7375 3250 50  0000 C CNN
	1    7375 3250
	0    1    1    0   
$EndComp
$Comp
L Screw_Terminal_1x01 J1
U 1 1 5909A31F
P 10725 725
F 0 "J1" H 10725 875 50  0000 C TNN
F 1 "Screw" H 10550 750 50  0000 C TNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_DIN965_Pad" H 10725 600 50  0001 C CNN
F 3 "" H 10725 625 50  0001 C CNN
	1    10725 725 
	1    0    0    -1  
$EndComp
$Comp
L Screw_Terminal_1x01 J2
U 1 1 5909A436
P 10725 975
F 0 "J2" H 10725 1125 50  0000 C TNN
F 1 "Screw" H 10550 1025 50  0000 C TNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_DIN965_Pad" H 10725 850 50  0001 C CNN
F 3 "" H 10725 875 50  0001 C CNN
	1    10725 975 
	1    0    0    -1  
$EndComp
NoConn ~ 10925 725 
NoConn ~ 10925 975 
$Comp
L PWR_FLAG #FLG07
U 1 1 5909CB9A
P 5075 5775
F 0 "#FLG07" H 5075 5870 50  0001 C CNN
F 1 "PWR_FLAG" H 5075 5955 50  0000 C CNN
F 2 "" H 5075 5775 50  0000 C CNN
F 3 "" H 5075 5775 50  0000 C CNN
	1    5075 5775
	1    0    0    -1  
$EndComp
$Comp
L R_Small R26
U 1 1 592DCCB9
P 3075 5925
F 0 "R26" H 2875 5900 50  0000 L CNN
F 1 "10k" H 2875 5825 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" H 3075 5925 50  0001 C CNN
F 3 "" H 3075 5925 50  0000 C CNN
	1    3075 5925
	1    0    0    -1  
$EndComp
$Comp
L C_Small C35
U 1 1 592DE2B1
P 2575 5875
F 0 "C35" H 2400 5825 50  0000 L CNN
F 1 "0.22u" H 2350 5950 50  0000 L CNN
F 2 "Capacitors_SMD:C_1210" H 2575 5875 50  0001 C CNN
F 3 "" H 2575 5875 50  0000 C CNN
	1    2575 5875
	-1   0    0    1   
$EndComp
$Comp
L R_Small R3
U 1 1 59310FA2
P 7350 2725
F 0 "R3" V 7250 2600 50  0000 L CNN
F 1 "10k" V 7250 2750 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" H 7350 2725 50  0001 C CNN
F 3 "" H 7350 2725 50  0000 C CNN
	1    7350 2725
	0    1    1    0   
$EndComp
$Comp
L R_Small R9
U 1 1 59311134
P 7350 3725
F 0 "R9" V 7250 3600 50  0000 L CNN
F 1 "10k" V 7250 3750 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" H 7350 3725 50  0001 C CNN
F 3 "" H 7350 3725 50  0000 C CNN
	1    7350 3725
	0    1    1    0   
$EndComp
$Comp
L GNDA #PWR08
U 1 1 59311286
P 7450 2725
F 0 "#PWR08" H 7450 2475 50  0001 C CNN
F 1 "GNDA" H 7450 2575 50  0000 C CNN
F 2 "" H 7450 2725 50  0000 C CNN
F 3 "" H 7450 2725 50  0000 C CNN
	1    7450 2725
	0    -1   -1   0   
$EndComp
$Comp
L GNDA #PWR09
U 1 1 593113B7
P 7450 3725
F 0 "#PWR09" H 7450 3475 50  0001 C CNN
F 1 "GNDA" H 7450 3575 50  0000 C CNN
F 2 "" H 7450 3725 50  0000 C CNN
F 3 "" H 7450 3725 50  0000 C CNN
	1    7450 3725
	0    -1   -1   0   
$EndComp
$Comp
L C_Small C38
U 1 1 593317CA
P 3875 6825
F 0 "C38" H 3725 6775 50  0000 L CNN
F 1 "0.22u" H 3650 6900 50  0000 L CNN
F 2 "Capacitors_SMD:C_1210" H 3875 6825 50  0001 C CNN
F 3 "" H 3875 6825 50  0000 C CNN
	1    3875 6825
	-1   0    0    1   
$EndComp
$Comp
L R_Small R30
U 1 1 59331934
P 3875 6475
F 0 "R30" H 3700 6450 50  0000 L CNN
F 1 "1k" H 3750 6525 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" H 3875 6475 50  0001 C CNN
F 3 "" H 3875 6475 50  0000 C CNN
	1    3875 6475
	-1   0    0    1   
$EndComp
$Comp
L CP_Small C37
U 1 1 59331D25
P 3575 6825
F 0 "C37" H 3375 6875 50  0000 L CNN
F 1 "22u" H 3375 6750 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D5.0mm_P2.50mm" H 3575 6825 50  0001 C CNN
F 3 "" H 3575 6825 50  0000 C CNN
	1    3575 6825
	-1   0    0    -1  
$EndComp
$Comp
L R_Small R31
U 1 1 593320D7
P 3725 6650
F 0 "R31" V 3875 6575 50  0000 L CNN
F 1 "10k" V 3800 6600 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" H 3725 6650 50  0001 C CNN
F 3 "" H 3725 6650 50  0000 C CNN
	1    3725 6650
	0    -1   -1   0   
$EndComp
$Comp
L R_Small R32
U 1 1 59333054
P 4200 6825
F 0 "R32" H 4000 6750 50  0000 L CNN
F 1 "220k" H 3975 6825 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" H 4200 6825 50  0001 C CNN
F 3 "" H 4200 6825 50  0000 C CNN
	1    4200 6825
	-1   0    0    1   
$EndComp
$Comp
L R_Small R28
U 1 1 59333B96
P 3075 6275
F 0 "R28" H 2875 6250 50  0000 L CNN
F 1 "10k" H 2875 6175 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" H 3075 6275 50  0001 C CNN
F 3 "" H 3075 6275 50  0000 C CNN
	1    3075 6275
	1    0    0    -1  
$EndComp
$Comp
L C_Small C36
U 1 1 5933583D
P 4775 6300
F 0 "C36" H 4600 6250 50  0000 L CNN
F 1 "0.22u" H 4550 6375 50  0000 L CNN
F 2 "Capacitors_SMD:C_1210" H 4775 6300 50  0001 C CNN
F 3 "" H 4775 6300 50  0000 C CNN
	1    4775 6300
	-1   0    0    1   
$EndComp
$Comp
L R_Small R29
U 1 1 593376B4
P 3375 6475
F 0 "R29" H 3425 6400 50  0000 L CNN
F 1 "15k" H 3425 6475 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" H 3375 6475 50  0001 C CNN
F 3 "" H 3375 6475 50  0000 C CNN
	1    3375 6475
	-1   0    0    1   
$EndComp
$Comp
L R_Small R20
U 1 1 5908D61A
P 5075 6600
F 0 "R20" H 5105 6620 50  0000 L CNN
F 1 "2.2k" H 5105 6560 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" H 5075 6600 50  0001 C CNN
F 3 "" H 5075 6600 50  0000 C CNN
	1    5075 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6775 2000 6775 2575
Wire Wire Line
	6775 2000 6400 2000
Wire Wire Line
	6400 2125 6775 2125
Connection ~ 6775 2125
Wire Wire Line
	6000 2475 6175 2475
Wire Wire Line
	6000 2675 6175 2675
Wire Wire Line
	6200 2000 6100 2000
Wire Wire Line
	6100 2000 6100 2475
Connection ~ 6100 2475
Wire Wire Line
	6200 2125 6100 2125
Connection ~ 6100 2125
Wire Wire Line
	6025 2825 6025 2675
Connection ~ 6025 2675
Wire Wire Line
	6175 2675 6175 2825
Wire Wire Line
	6025 3025 6025 3175
Wire Wire Line
	6025 3175 6175 3175
Wire Wire Line
	6175 3175 6175 3025
Wire Wire Line
	6775 3300 6775 3875
Wire Wire Line
	6775 3300 6400 3300
Wire Wire Line
	6400 3425 6775 3425
Connection ~ 6775 3425
Wire Wire Line
	6000 3775 6175 3775
Wire Wire Line
	6000 3975 6175 3975
Wire Wire Line
	6200 3300 6100 3300
Wire Wire Line
	6100 3300 6100 3775
Connection ~ 6100 3775
Wire Wire Line
	6200 3425 6100 3425
Connection ~ 6100 3425
Wire Wire Line
	6025 4125 6025 3975
Connection ~ 6025 3975
Wire Wire Line
	6175 3975 6175 4125
Wire Wire Line
	6025 4325 6025 4475
Wire Wire Line
	6025 4475 6175 4475
Wire Wire Line
	6175 4475 6175 4325
Connection ~ 6175 2675
Connection ~ 6175 3975
Connection ~ 7150 3725
Wire Wire Line
	7150 3150 7375 3150
Wire Wire Line
	7375 3350 7150 3350
Connection ~ 6775 2575
Connection ~ 6775 3875
Wire Wire Line
	5075 6325 5650 6325
Connection ~ 5375 6325
Connection ~ 5075 6325
Wire Wire Line
	7250 2725 7150 2725
Wire Wire Line
	7250 3725 7150 3725
Wire Wire Line
	2075 5775 3475 5775
Wire Wire Line
	3375 5550 3375 5900
Connection ~ 3375 5775
Wire Wire Line
	3375 6300 3375 6375
Wire Wire Line
	3375 6575 3375 6650
Wire Wire Line
	3375 6650 3625 6650
Wire Wire Line
	3575 6725 3575 6650
Connection ~ 3575 6650
Wire Wire Line
	3825 6650 4200 6650
Wire Wire Line
	4200 6650 4200 6725
Wire Wire Line
	3875 6575 3875 6725
Connection ~ 3875 6650
Wire Wire Line
	3075 5775 3075 5825
Connection ~ 3075 5775
Wire Wire Line
	3075 6025 3075 6175
Connection ~ 3075 6100
Wire Wire Line
	3075 6375 3075 6525
Wire Wire Line
	3875 5775 5375 5775
Wire Wire Line
	4075 5550 4075 6075
Connection ~ 4075 5775
Connection ~ 2575 5775
Wire Wire Line
	5075 5775 5075 5950
Wire Wire Line
	4500 6200 4500 5775
Connection ~ 4500 5775
Wire Wire Line
	5075 6150 5075 6500
Wire Wire Line
	4775 6200 4775 5775
Connection ~ 4775 5775
Wire Wire Line
	5375 6500 5375 6325
Wire Wire Line
	5650 6325 5650 6500
Wire Wire Line
	5075 7025 5075 6700
Wire Wire Line
	2075 7025 5650 7025
Wire Wire Line
	4200 7025 4200 6925
Wire Wire Line
	5375 7025 5375 6700
Connection ~ 5075 7025
Wire Wire Line
	5650 7025 5650 6700
Connection ~ 5375 7025
Wire Wire Line
	4775 6400 4775 7025
Connection ~ 4775 7025
Wire Wire Line
	4500 6400 4500 7025
Connection ~ 4500 7025
Connection ~ 5075 5775
Wire Wire Line
	3575 6925 3575 7025
Connection ~ 4200 7025
Wire Wire Line
	3875 6925 3875 7025
Connection ~ 3875 7025
Wire Wire Line
	5375 2675 5525 2675
Wire Wire Line
	5725 2675 5800 2675
Wire Wire Line
	5375 2475 5525 2475
Wire Wire Line
	5725 2475 5800 2475
Wire Wire Line
	5375 3775 5525 3775
Wire Wire Line
	5375 3975 5525 3975
Wire Wire Line
	5725 3975 5800 3975
Wire Wire Line
	5800 3775 5725 3775
$Comp
L D D6
U 1 1 5934962B
P 3675 5550
F 0 "D6" H 3675 5725 50  0000 C CNN
F 1 "1N4148" H 3675 5650 50  0000 C CNN
F 2 "Diodes_SMD:D_MELF" H 3675 5550 50  0001 C CNN
F 3 "" H 3675 5550 50  0000 C CNN
	1    3675 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3375 5550 3525 5550
Wire Wire Line
	3825 5550 4075 5550
Wire Wire Line
	7150 2575 7150 3150
Wire Wire Line
	7150 2575 6975 2575
Connection ~ 7150 2725
Wire Wire Line
	7150 3875 6975 3875
Wire Wire Line
	7150 3350 7150 3875
Wire Wire Line
	3075 6925 3075 7025
Connection ~ 3575 7025
Connection ~ 3075 7025
Text GLabel 4300 3125 2    60   Input ~ 0
SPK_LN
Text GLabel 4300 3025 2    60   Input ~ 0
SPK_LP
Text GLabel 4300 3325 2    60   Input ~ 0
SPK_RN
Text GLabel 4300 3225 2    60   Input ~ 0
SPK_RP
$Comp
L R_Small R6
U 1 1 5933E72E
P 2775 6875
F 0 "R6" H 2625 6875 50  0000 L CNN
F 1 "10k" H 2600 6950 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" H 2775 6875 50  0001 C CNN
F 3 "" H 2775 6875 50  0000 C CNN
	1    2775 6875
	-1   0    0    1   
$EndComp
Wire Wire Line
	2775 6725 2775 6775
Wire Wire Line
	2775 6975 2775 7025
Text Label 2725 5775 0    60   ~ 0
B+
$Comp
L R_Small R2
U 1 1 59340F79
P 5425 2575
F 0 "R2" H 5375 2425 50  0000 L CNN
F 1 "680" H 5325 2750 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" H 5425 2575 50  0001 C CNN
F 3 "" H 5425 2575 50  0000 C CNN
	1    5425 2575
	-1   0    0    1   
$EndComp
Connection ~ 5425 2475
Connection ~ 5425 2675
$Comp
L R_Small R5
U 1 1 59341A13
P 5425 3875
F 0 "R5" H 5375 3725 50  0000 L CNN
F 1 "680" H 5325 4050 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" H 5425 3875 50  0001 C CNN
F 3 "" H 5425 3875 50  0000 C CNN
	1    5425 3875
	-1   0    0    1   
$EndComp
Connection ~ 5425 3775
Connection ~ 5425 3975
$Comp
L GNDA #PWR011
U 1 1 5933BE66
P 4200 7025
F 0 "#PWR011" H 4200 6775 50  0001 C CNN
F 1 "GNDA" H 4200 6875 50  0000 C CNN
F 2 "" H 4200 7025 50  0000 C CNN
F 3 "" H 4200 7025 50  0000 C CNN
	1    4200 7025
	1    0    0    -1  
$EndComp
$Comp
L GNDA #PWR012
U 1 1 59342CFB
P 2575 5975
F 0 "#PWR012" H 2575 5725 50  0001 C CNN
F 1 "GNDA" H 2575 5825 50  0000 C CNN
F 2 "" H 2575 5975 50  0000 C CNN
F 3 "" H 2575 5975 50  0000 C CNN
	1    2575 5975
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG013
U 1 1 59342F65
P 5650 7025
F 0 "#FLG013" H 5650 7120 50  0001 C CNN
F 1 "PWR_FLAG" H 5650 7205 50  0000 C CNN
F 2 "" H 5650 7025 50  0000 C CNN
F 3 "" H 5650 7025 50  0000 C CNN
	1    5650 7025
	0    1    1    0   
$EndComp
$Comp
L CONN_01X04 P1
U 1 1 59354F91
P 4100 3175
F 0 "P1" H 4100 3425 50  0000 C CNN
F 1 "DIFF_IN" V 4200 3175 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 4100 3175 50  0001 C CNN
F 3 "" H 4100 3175 50  0000 C CNN
	1    4100 3175
	-1   0    0    -1  
$EndComp
$Comp
L CONN_01X03 P3
U 1 1 5935600F
P 1875 6725
F 0 "P3" H 1875 6925 50  0000 C CNN
F 1 "PWR_EN" V 1975 6725 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm" H 1875 6725 50  0001 C CNN
F 3 "" H 1875 6725 50  0000 C CNN
	1    1875 6725
	-1   0    0    1   
$EndComp
$Comp
L R_Small R10
U 1 1 593566DF
P 2500 6725
F 0 "R10" V 2400 6650 50  0000 L CNN
F 1 "1k" V 2500 6675 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" H 2500 6725 50  0001 C CNN
F 3 "" H 2500 6725 50  0000 C CNN
	1    2500 6725
	0    1    1    0   
$EndComp
Wire Wire Line
	2775 6725 2600 6725
Text Label 2125 6725 0    60   ~ 0
EN12
Connection ~ 2775 6725
$Comp
L Q_NPN_BEC Q3
U 1 1 59359548
P 3875 6175
F 0 "Q3" V 3825 5950 50  0000 L CNN
F 1 "B847" V 4075 6050 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 4075 6275 50  0001 C CNN
F 3 "" H 3875 6175 50  0000 C CNN
	1    3875 6175
	0    -1   -1   0   
$EndComp
$Comp
L Q_PNP_BEC Q1
U 1 1 59359611
P 3675 5875
F 0 "Q1" V 3875 5975 50  0000 L CNN
F 1 "BC807" V 3875 5625 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 3875 5975 50  0001 C CNN
F 3 "" H 3675 5875 50  0000 C CNN
	1    3675 5875
	0    1    -1   0   
$EndComp
$Comp
L Q_PMOS_GSD Q2
U 1 1 593596F1
P 3275 6100
F 0 "Q2" H 3475 6150 50  0000 L CNN
F 1 "Si2309CDS" H 3400 6325 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 3475 6200 50  0001 C CNN
F 3 "" H 3275 6100 50  0000 C CNN
	1    3275 6100
	1    0    0    1   
$EndComp
$Comp
L Q_NMOS_GSD Q4
U 1 1 593597B8
P 2975 6725
F 0 "Q4" H 3175 6775 50  0000 L CNN
F 1 "2N7002" H 3175 6675 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 3175 6825 50  0001 C CNN
F 3 "" H 2975 6725 50  0000 C CNN
	1    2975 6725
	1    0    0    -1  
$EndComp
Text Label 7350 3150 0    60   ~ 0
L
Text Label 7350 3350 0    60   ~ 0
R
$Comp
L CONN_01X01 J3
U 1 1 596A670A
P 4500 5575
F 0 "J3" H 4500 5675 50  0000 C CNN
F 1 "Mic Bias" V 4600 5575 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x01_Pitch2.54mm" H 4500 5575 50  0001 C CNN
F 3 "" H 4500 5575 50  0001 C CNN
	1    4500 5575
	0    -1   -1   0   
$EndComp
$Comp
L +9V #PWR?
U 1 1 596DAC67
P 6375 4175
F 0 "#PWR?" H 6375 4025 50  0001 C CNN
F 1 "+9V" V 6375 4350 50  0000 C CNN
F 2 "" H 6375 4175 50  0001 C CNN
F 3 "" H 6375 4175 50  0001 C CNN
	1    6375 4175
	0    1    1    0   
$EndComp
$Comp
L +9V #PWR?
U 1 1 596DAFA4
P 6375 2875
F 0 "#PWR?" H 6375 2725 50  0001 C CNN
F 1 "+9V" V 6375 3050 50  0000 C CNN
F 2 "" H 6375 2875 50  0001 C CNN
F 3 "" H 6375 2875 50  0001 C CNN
	1    6375 2875
	0    1    1    0   
$EndComp
$Comp
L +9V #PWR?
U 1 1 596DB11B
P 5375 5775
F 0 "#PWR?" H 5375 5625 50  0001 C CNN
F 1 "+9V" V 5375 5950 50  0000 C CNN
F 2 "" H 5375 5775 50  0001 C CNN
F 3 "" H 5375 5775 50  0001 C CNN
	1    5375 5775
	0    1    1    0   
$EndComp
Wire Wire Line
	2075 6625 2075 5775
Wire Wire Line
	2075 6725 2400 6725
Wire Wire Line
	2075 6825 2075 7025
Connection ~ 2775 7025
$EndSCHEMATC