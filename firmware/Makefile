# BMW Ibus Bluetooth CDC emulator - Emulates CD changer on IBus
# Copyright (C) 2017-2099 victor naumov <vicnaumov@gmail.com
#
# This file is part of BMW Ibus Bluetooth CDC emulator.
#
# BMW Ibus Bluetooth CDC emulator is free software: you can redistribute
# it and/or modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# BMW Ibus Bluetooth CDC emulator is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with BMW Ibus Bluetooth CDC emulator.  If not, see
# <http://www.gnu.org/licenses/>.

# This is a prototype Makefile. Modify it according to your needs.
# You should at least check the settings for
# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected. We recommend that you leave it undefined and
#                add settings like this to your ~/.avrduderc file:
#                   default_programmer = "stk500v2"
#                   default_serial = "avrdoper"
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.

DEVICE     = atmega328p
# DEVICE     = atmega88p
CLOCK      = 16000000
PROGRAMMER = -c usbasp -P usb
OBJECTS    = main.o ibus.o sdebug.o tq.o csr.o ibus_dispatch.o menu.o
#
FUSES      = -U lfuse:w:0xee:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m

# ATMega8 fuse bits used above (fuse bits for other devices are different!):
# Example for 8 MHz internal oscillator
# Fuse high byte:
# 0xd9 = 1 1 0 1   1 0 0 1 <-- BOOTRST (boot reset vector at 0x0000)
#        ^ ^ ^ ^   ^ ^ ^------ BOOTSZ0
#        | | | |   | +-------- BOOTSZ1
#        | | | |   +---------- EESAVE (set to 0 to preserve EEPROM over chip erase)
#        | | | +-------------- CKOPT (clock option, depends on oscillator type)
#        | | +---------------- SPIEN (if set to 1, serial programming is disabled)
#        | +------------------ WDTON (if set to 0, watchdog is always on)
#        +-------------------- RSTDISBL (if set to 0, RESET pin is disabled)
# Fuse low byte:
# 0x24 = 0 0 1 0   0 1 0 0
#        ^ ^ \ /   \--+--/
#        | |  |       +------- CKSEL 3..0 (8M internal RC)
#        | |  +--------------- SUT 1..0 (slowly rising power)
#        | +------------------ BODEN (if 0, brown-out detector is enabled)
#        +-------------------- BODLEVEL (if 0: 4V, if 1: 2.7V)
#
# For computing fuse byte values for other devices and options see
# the fuse bit calculator at http://www.engbedded.com/fusecalc/


# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)
#COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -DDEBUG_SERIAL

# symbolic targets:
all:	main.hex

.c.o:
	$(COMPILE) -std=gnu99 -g -c $< -o $@

#preproc:
#	$(COMPILE) -E -std=gnu99 -c menu.c -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)
reset:
	$(AVRDUDE)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -f main.hex main.elf $(OBJECTS)

# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -S main.elf

cpp:
	$(COMPILE) -E main.c
