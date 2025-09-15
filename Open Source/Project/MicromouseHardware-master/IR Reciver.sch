EESchema Schematic File Version 2
LIBS:1_PJC-Sandbox
LIBS:Micromouse1.2-rescue
LIBS:Micromouse1.2-cache
LIBS:74xgxx
LIBS:74xx
LIBS:ac-dc
LIBS:actel
LIBS:adc-dac
LIBS:allegro
LIBS:Altera
LIBS:analog_devices
LIBS:analog_switches
LIBS:atmel
LIBS:audio
LIBS:battery_management
LIBS:bbd
LIBS:bosch
LIBS:brooktre
LIBS:cmos_ieee
LIBS:cmos4000
LIBS:conn
LIBS:contrib
LIBS:cypress
LIBS:dc-dc
LIBS:device
LIBS:digital-audio
LIBS:diode
LIBS:display
LIBS:dsp
LIBS:elec-unifil
LIBS:ESD_Protection
LIBS:ftdi
LIBS:gennum
LIBS:graphic
LIBS:hc11
LIBS:intel
LIBS:interface
LIBS:ir
LIBS:Lattice
LIBS:leds
LIBS:linear
LIBS:logo
LIBS:maxim
LIBS:mechanical
LIBS:memory
LIBS:microchip
LIBS:microchip_dspic33dsc
LIBS:microchip_pic10mcu
LIBS:microchip_pic12mcu
LIBS:microchip_pic16mcu
LIBS:microchip_pic18mcu
LIBS:microchip_pic24mcu
LIBS:microchip_pic32mcu
LIBS:microcontrollers
LIBS:modules
LIBS:motor_drivers
LIBS:motorola
LIBS:motors
LIBS:msp430
LIBS:nordicsemi
LIBS:nxp
LIBS:nxp_armmcu
LIBS:onsemi
LIBS:opto
LIBS:Oscillators
LIBS:philips
LIBS:power
LIBS:Power_Management
LIBS:powerint
LIBS:pspice
LIBS:references
LIBS:regul
LIBS:relays
LIBS:rfcom
LIBS:sensors
LIBS:silabs
LIBS:siliconi
LIBS:stm8
LIBS:stm32
LIBS:supertex
LIBS:switches
LIBS:texas
LIBS:transf
LIBS:transistors
LIBS:triac_thyristor
LIBS:ttl_ieee
LIBS:valves
LIBS:video
LIBS:wiznet
LIBS:Worldsemi
LIBS:Xicor
LIBS:xilinx
LIBS:zetex
LIBS:Zilog
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 8
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L D_Photo D1
U 1 1 5945F7BF
P 3800 3125
F 0 "D1" H 3820 3195 50  0000 L CNN
F 1 "~" H 3760 3015 50  0000 C CNN
F 2 "micromouse:IR_Reciver" H 3750 3125 50  0001 C CNN
F 3 "" H 3750 3125 50  0001 C CNN
F 4 "TEFT4300" H 3800 3125 60  0001 C CNN "manf#"
	1    3800 3125
	0    -1   -1   0   
$EndComp
$Comp
L R R9
U 1 1 5945F8D6
P 3800 3800
F 0 "R9" V 3880 3800 50  0000 C CNN
F 1 "1.8k" V 3800 3800 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3730 3800 50  0001 C CNN
F 3 "" H 3800 3800 50  0001 C CNN
F 4 "DNP" H 3880 3900 50  0001 C CNN "manf#"
	1    3800 3800
	1    0    0    -1  
$EndComp
$Comp
L D_Photo D3
U 1 1 5945FA4C
P 4775 3100
F 0 "D3" H 4795 3170 50  0000 L CNN
F 1 "~" H 4735 2990 50  0000 C CNN
F 2 "micromouse:IR_Reciver" H 4725 3100 50  0001 C CNN
F 3 "" H 4725 3100 50  0001 C CNN
F 4 "TEFT4300" H 4775 3100 60  0001 C CNN "manf#"
	1    4775 3100
	0    -1   -1   0   
$EndComp
$Comp
L R R11
U 1 1 5945FA52
P 4775 3775
F 0 "R11" V 4855 3775 50  0000 C CNN
F 1 "1.8k" V 4775 3775 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4705 3775 50  0001 C CNN
F 3 "" H 4775 3775 50  0001 C CNN
F 4 "DNP" H 4855 3875 50  0001 C CNN "manf#"
	1    4775 3775
	1    0    0    -1  
$EndComp
$Comp
L D_Photo D5
U 1 1 5945FADF
P 5775 3100
F 0 "D5" H 5795 3170 50  0000 L CNN
F 1 "~" H 5735 2990 50  0000 C CNN
F 2 "micromouse:IR_Reciver" H 5725 3100 50  0001 C CNN
F 3 "" H 5725 3100 50  0001 C CNN
F 4 "TEFT4300" H 5775 3100 60  0001 C CNN "manf#"
	1    5775 3100
	0    -1   -1   0   
$EndComp
$Comp
L R R13
U 1 1 5945FAE5
P 5775 3775
F 0 "R13" V 5855 3775 50  0000 C CNN
F 1 "1.8k" V 5775 3775 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5705 3775 50  0001 C CNN
F 3 "" H 5775 3775 50  0001 C CNN
F 4 "DNP" H 5855 3875 50  0001 C CNN "manf#"
	1    5775 3775
	1    0    0    -1  
$EndComp
$Comp
L D_Photo D6
U 1 1 5945FB3E
P 6775 3100
F 0 "D6" H 6795 3170 50  0000 L CNN
F 1 "~" H 6735 2990 50  0000 C CNN
F 2 "micromouse:IR_Reciver" H 6725 3100 50  0001 C CNN
F 3 "" H 6725 3100 50  0001 C CNN
F 4 "TEFT4300" H 6775 3100 60  0001 C CNN "manf#"
	1    6775 3100
	0    -1   -1   0   
$EndComp
$Comp
L R R14
U 1 1 5945FB44
P 6775 3775
F 0 "R14" V 6855 3775 50  0000 C CNN
F 1 "1.8k" V 6775 3775 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6705 3775 50  0001 C CNN
F 3 "" H 6775 3775 50  0001 C CNN
F 4 "DNP" H 6855 3875 50  0001 C CNN "manf#"
	1    6775 3775
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 5945FE6A
P 7600 3400
F 0 "C5" H 7625 3500 50  0000 L CNN
F 1 "0.1uF" H 7625 3300 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7638 3250 50  0001 C CNN
F 3 "" H 7600 3400 50  0001 C CNN
F 4 "DNP" H 7625 3600 50  0001 C CNN "manf#"
	1    7600 3400
	1    0    0    -1  
$EndComp
$Comp
L GNDA #PWR017
U 1 1 5945FEE7
P 3800 4525
F 0 "#PWR017" H 3800 4275 50  0001 C CNN
F 1 "GNDA" H 3800 4375 50  0000 C CNN
F 2 "" H 3800 4525 50  0001 C CNN
F 3 "" H 3800 4525 50  0001 C CNN
	1    3800 4525
	1    0    0    -1  
$EndComp
$Comp
L D_Photo D2
U 1 1 594637C3
P 4250 3100
F 0 "D2" H 4270 3170 50  0000 L CNN
F 1 "~" H 4210 2990 50  0000 C CNN
F 2 "micromouse:IR_Reciver" H 4200 3100 50  0001 C CNN
F 3 "" H 4200 3100 50  0001 C CNN
F 4 "TEFT4300" H 4250 3100 60  0001 C CNN "manf#"
	1    4250 3100
	0    -1   -1   0   
$EndComp
$Comp
L R R10
U 1 1 594637C9
P 4250 3775
F 0 "R10" V 4330 3775 50  0000 C CNN
F 1 "1.8k" V 4250 3775 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4180 3775 50  0001 C CNN
F 3 "" H 4250 3775 50  0001 C CNN
F 4 "DNP" H 4330 3875 50  0001 C CNN "manf#"
	1    4250 3775
	1    0    0    -1  
$EndComp
$Comp
L D_Photo D4
U 1 1 59463894
P 5275 3100
F 0 "D4" H 5295 3170 50  0000 L CNN
F 1 "~" H 5235 2990 50  0000 C CNN
F 2 "micromouse:IR_Reciver" H 5225 3100 50  0001 C CNN
F 3 "" H 5225 3100 50  0001 C CNN
F 4 "TEFT4300" H 5275 3100 60  0001 C CNN "manf#"
	1    5275 3100
	0    -1   -1   0   
$EndComp
$Comp
L R R12
U 1 1 5946389A
P 5275 3775
F 0 "R12" V 5355 3775 50  0000 C CNN
F 1 "1.8k" V 5275 3775 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5205 3775 50  0001 C CNN
F 3 "" H 5275 3775 50  0001 C CNN
F 4 "DNP" H 5355 3875 50  0001 C CNN "manf#"
	1    5275 3775
	1    0    0    -1  
$EndComp
Text GLabel 8000 2400 2    59   Input ~ 12
VA3.3
Text GLabel 3800 3350 0    39   Input ~ 8
L_RECEIVER
Text GLabel 4250 3350 0    39   Input ~ 8
LD_RECEIVER
Text GLabel 4775 3350 0    39   Input ~ 8
LF_RECEIVER
Text GLabel 5275 3350 0    39   Input ~ 8
RF_RECEIVER
Text GLabel 5775 3350 0    39   Input ~ 8
RD_RECEIVER
Text GLabel 6775 3350 0    39   Input ~ 8
R_RECEIVER
Text Notes 4150 1300 0    394  ~ 79
IR Reciver
Wire Wire Line
	3800 3325 3800 3650
Wire Wire Line
	4775 3300 4775 3625
Wire Wire Line
	5775 3300 5775 3625
Wire Wire Line
	6775 3300 6775 3625
Wire Wire Line
	3800 3950 3800 4525
Wire Wire Line
	4775 3925 4775 4250
Wire Wire Line
	3800 4250 7600 4250
Wire Wire Line
	5775 4250 5775 3925
Wire Wire Line
	6775 4250 6775 3925
Wire Wire Line
	7600 4250 7600 3550
Wire Wire Line
	8000 2400 3800 2400
Wire Wire Line
	3800 2400 3800 3025
Wire Wire Line
	4775 3000 4775 2400
Wire Wire Line
	5775 3000 5775 2400
Wire Wire Line
	6775 3000 6775 2400
Wire Wire Line
	7600 3250 7600 2400
Wire Wire Line
	4250 3300 4250 3625
Wire Wire Line
	5275 3300 5275 3625
Wire Wire Line
	4250 3000 4250 2400
Wire Wire Line
	5275 3000 5275 2400
Wire Wire Line
	5275 3925 5275 4250
Wire Wire Line
	4250 3925 4250 4250
Connection ~ 3800 4250
Connection ~ 4775 4250
Connection ~ 5775 4250
Connection ~ 6775 4250
Connection ~ 4775 2400
Connection ~ 5775 2400
Connection ~ 6775 2400
Connection ~ 7600 2400
Connection ~ 4250 2400
Connection ~ 5275 2400
Connection ~ 5275 4250
Connection ~ 4250 4250
$EndSCHEMATC
