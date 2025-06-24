# Single Screen Handheld


# Disclaimer: 

This project is a work in progress.

I assume no responsibility or liability for any errors, omissions, or outcomes resulting from the use of the information provided within this project.

You are strongly advised to conduct your own thorough research and ensure you have a comprehensive understanding of best practices for the safe charging and handling of Li-ion batteries. Improper handling can lead to serious risks, including but not limited to fire, explosion, or personal injury.

Use the information provided in this project at your own risk.

<table>
<tr><td colspan="2"><img src="../assets/front.jpg" alt="Front" width="800"></td></tr>
<tr><td colspan="2"><img src="../assets/internal.jpg" alt="Internal" width="800"></td></tr>
<tr><td><img src="../assets/charge.jpg" alt="Internal" width="800"></td><td><img src="../assets/side.jpg" alt="Internal" width="555"></td></tr>
</table>


# Features

## 12 Games
Plays 12 two button game and watch games. 
Use button combinations on power up to select the game (See source code).
Remembers last game selected if no buttons pressed on power up.

## Volume Adjustment
Volume adjustment with on screen display.
Volume down: Time and Left buttons.
Volume up: Time and Right buttons.
Remembers volume level on power up.

## Power
Two 10440 batteries, USB-C charging and on/off switch.

## Battery Level Indicator
Battery level is displayed on power up.

# Components
- ESP32-S3-Zero
- Lcd: 240x320 2.4 inch SPI panel with ILI9341 driver
- Audio: Max98357 I2S Amp
- Speaker: 20mm
- USB-C TP4056 Battery Charger Module 
- 2 x 10440 Li-ion batteries
- Mini Push Buttons 4.5mm x 4.5mm x 3.8mm
- 3mm SS12d00 mini slide switch.
- AO3401A mosfet
- 1N5817 schottky diode
- Resistors: 2 x 4.7K, 10K and 100K
- Screws M2 - 4mm, 6mm and 8mm long.
- Screws to mount the TP4056 module. 2 x M1.5


<table>
<tr>
    <td><img src="../assets/esp32_s3_zero.jpg" alt="" width="400"></td>
    <td><img src="../assets/ILI9341_lcd_no_pcb.jpg" alt="" width="400"></td>
    <td><img src="../assets/max98357.jpg" alt="" width="400"></td>    
    <td><img src="../assets/20mm_speaker.jpg" alt="" width="400"></td>
</tr>
</table>

<table>
<tr>
    <td><img src="../assets/tp4056.jpg" alt="" width="400"></td>
    <td><img src="../assets/10440.jpg" alt="" width="400"></td>
    <td><img src="../assets/mini_push_button.jpg" alt="" width="400"></td>
    <td><img src="../assets/mini_slide_switch.jpg" alt="" width="400"></td>
</tr>
</table>

# Power

<img src="../assets/handheld_power.png" alt="" width="1000">

## TP4056

A TP4056 module is used to charge the battery. These modules are designed for charging 18650 batteries at 1A.

For charing 10440 batteries, resistor R3 should be replaced with a 4.7kΩ resistor. This will charge the battery at approximately 250mA.

A mosfet (AO3401A) is used to disconnect the load from the battery while it is charging. This can be skipped if you remember to always turn off the device when it is charging.

The TP4056 can only be mounted via the holes in the USB voltage pads. When you insert a screw into the pads it might break the connection between the top and bottom pads. Check after connecting and if the connection is broken you might need to use a jumper wire to fix the issue.

## Battery Level Indicator

The battery voltage is sent to GPIO 16 for a battery level indicator. The ESP32-S3 ADC can measure voltages up to 3.1V so a voltage divider is used to drop the voltage to a suitable level.

With a few changes, the AO3401A mosfet can be setup to power the load from USB power while the battery is changing, however if you choose to do this you will also need to alter the voltage divider values to make sure the voltage at GPIO 16 does not exceed 3.1V.

## Jumper

A jumper has been placed between the ESP32 5v pin and the switch. This jumper should be open when connecting the ESP32 to USB for debugging. Failure to open the jumper will result in voltage from ESP32 making its way to the batteries if the switch is also turned on. You could skip the jumper installation, and always make sure the switch is off when debugging, but you will need the jumper installed and open if you want to debug the battery level indicator code.

## 3D Files

View 3D files: [Single Screen Handheld 3D Files](../assets/3d_files/gandw_single_screen_handheld/)

## Button Circuit Boards

The buttons need to be mounted on 2.54mm perfboard. 

The alarm and ACL buttons need to be mounted slightly high on the board. Use the spacer (alarm_acl_button_spacer.stl) to mount them at the correct height.

The diagrams below show how to mount the buttons:

<table>
<tr>
    <td><img src="../assets/perfboard_small_buttons.jpg" alt="" width="200"></td>
    <td><img src="../assets/perfboard_main_button.jpg" alt="" width="200"></td>
</tr>
</table>



# Wiring

## LCD ILI9341
- VCC:        ESP32 3.3v
- GND:        GND
- CS:         GND
- RESET:      GPIO 13
- DC:         GPIO 9
- MOSI        GPIO 11
- SCK:        GPIO 12
- LEDA:       ESP32 3.3v
- LEDK1       GND  

## Max98357
- VIN:        ESP32 3.3v
- GND:        GND
- LRC (WS):   GPIO 4    
- BCLK:       GPIO 10
- DIN:        GPIO 6

## Buttons
- GAME A:       GPIO 14
- GAME B:       GPIO 5
- TIME:         GPIO 3
- LEFT:         GPIO 2
- RIGHT:        GPIO 1
- ALARM:        GPIO 7
- ACL:          GPIO 8

## Battery Level
- Battery +:   GPIO 16 (via voltage divider)



# Acknowledgements

## LCD-Game-Emulator
https://github.com/bzhxx/lcd-game-emulator

## Retro-Go
https://github.com/ducalex/retro-go

The LCD-Game-Emulator version used by the project was copied from Retro-Go because it contains additional code to handle the loading of the gw files.







