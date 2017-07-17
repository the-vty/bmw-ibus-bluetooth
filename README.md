# BMW IBus CDC emulator with Bluetooth audio

The goal of this project was to create a device that emulates CD changer protocol being connected to IBus.
It works with the radio e46 "Business CD" and enables CD changer menu and inputs. Blusetooth functionality 
is based on the F-3188 variant of CSR8645 module (A2DP/AVRCP/HFP/HSP).

* Ibus interface is implemented using MELEXIS - K-Bus Transceiver TH3122.
* Interface between IBus and CSR8645 is implemented on Atmega328p.
* Uses steering wheel buttons, buttons on the radio for pairing, switching tracks, rewind, making calls, 
transfer phone calls between hands-free and a phone, play audio files.
* Wakes up on the bus activity
* Switches off automatically when the bus is inactive for 60 seconds.

## Buttons assignments

* Default mode is activated when the radio is switched to the CDC mode (button **[MODE]**)
  * Steering wheel and radio buttons **[>]** and **[<]** switch tracks on short press and rewind/fast forward on long press
  * **[1]** play/pause (AVRCP mode), answers/ends the calls
  * **[2]** cancels the calls
  * **[3]** transfers audio from hands-free to the phone and back
  * **[4]** mute
  * **[5]** redial of the last number
  * **[6]** play/pause (AVRCP mode)
  * **SCAN** switches to the service menu (**CHOICE?** is displayed on the radio display)
* Service mode
  * **[1]** sets pairing mode (displays **Pairing?**)
  * **[2]** resets paired device list (displays **Reset PDL?**)
  * **[3]** enables multipoint (displays **Enable MP?**)
  * **[4]** disables multipoint (displays **Disable MP?**)
  * **[5]** volume down (displays **Vol Down**)
  * **[6]** volume up (displays **Vol Up**)
 
 Note: The service modes **[1]** .. **[4]** need confimation by pressing **[RND]** button. Service mode gets deactivated after 5 seconds timeout.
 
 ## CSR8645 Configuration
 
 The Bluetooth module must be configured and flashed using **CSR86XX ConfigurationTool 3.0.70** and **csr8645/final.psr** file. 
 The **csr8645/final.psr** config changes:
 
 * the LED0 flashing patterns to make it easily parsable by the microcontroller.
 * buttons configuration and timings.
 
 Other PSR files:
 * **csr8645/final_dump_pstool.psr** file contains a full dump of the PSR config
 * **csr8645/EQ.psr** file has some DSP equalizer tweaks in poles of 32Hz, 100Hz, 16000Hz lifting them up 1-2 db to make it sounding better to my taste.
   This file must be merged with the **csr8645/final_dump_pstool.psr** and flashed using **BlueSuite 2.6.6**.
   
DSP tweaks can be done in **CSR8600 ROM Series A03 Release Parameter Manager**

The PCB has the pin headers for connecting [Flashing tool](https://github.com/lorf/csr-spi-ftdi).

Nice [Videos](https://www.youtube.com/user/lipton5001) explaining how to use CSR86XX tools.

 ## Hardware
 * **/bmw_bt_cdcemu_digital/** - KiCAD digital board. Contains the IBus transceiver, BT module and Atmega328p microcontroller
 * **/bmw_bt_cdcemu_analog/** - KiCAD analog board. Contains the differential to single ended amplifier and 
   power line filtering with power off functionality controlled by **EN12** pin on the digital board
   
