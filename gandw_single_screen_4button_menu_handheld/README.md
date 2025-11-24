# Single Screen 4 Button Handheld with Menu


# Disclaimer: 

This project is a work in progress.

I assume no responsibility or liability for any errors, omissions, or outcomes resulting from the use of the information provided within this project.

You are strongly advised to conduct your own thorough research and ensure you have a comprehensive understanding of best practices for the safe charging and handling of Li-ion batteries. Improper handling can lead to serious risks, including but not limited to fire, explosion, or personal injury.

Use the information provided in this project at your own risk.

<table>
<tr><td colspan="2"><img src="../assets/4button_menu_handheld_1.jpg" alt="4 Button Handheld" width="800"></td></tr>
<tr><td colspan="2"><img src="../assets/4button_menu_handheld_2.jpg" alt="4 Button Handheld" width="800"></td></tr>
<tr>
    <td><img src="../assets/4button_handheld_3.jpg" alt="4 Button Handheld" width="400"></td>
    <td><img src="../assets/4button_handheld_4.jpg" alt="4 Button Handheld" width="400"></td>
</tr>
<tr>
    <td><img src="../assets/4button_handheld_5.jpg" alt="4 Button Handheld" width="400"></td>
    <td><img src="../assets/4button_handheld_6.jpg" alt="4 Button Handheld" width="400"></td>
</tr>
</table>


# Features

## 22 Games
Plays 22 two and four button game and watch games. 
Use the left and right buttons in the menu to select a game, then press Game A to start the game.
You can return to the menu by pressing the left and right buttons for a few seconds.

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
- Display: 2.4 inch SPI panel 240x320 LCD with ILI9341 driver
  (No board - LCD only, No Touch. AliExpress seller: XunMing Store)
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

To charge 10440 batteries, resistor R3 should be replaced with a 4.7kΩ resistor. This will charge the battery at approximately 250mA. To make it easier, after removing resistor R3, you can connect a wire to pin 2 of the main IC and mount the 4.7kΩ resistor on a perfboard with the other end connected to ground.

A mosfet (AO3401A) is used to disconnect the load from the battery while it is charging. This can be skipped if you remember to always turn off the device when it is charging. If you plan on using the mosfet, to make it easier you can connect a wire to pin 4 of the main IC and connect the other end to the schottky diode leading to the mosfet gate.

The TP4056 can only be mounted via the holes in the USB voltage pads. When you insert a screw into the pads it might break the connection between the top and bottom pads. If you plan to use these pads check to make sure the connection is not broken after you screw them in place.


## Battery Level Indicator

The battery voltage is sent to GPIO 16 for a battery level indicator. The ESP32-S3 ADC can measure voltages up to 3.1V so a voltage divider is used to drop the voltage to a suitable level.

With a few changes, the AO3401A mosfet can be setup to power the load from USB power while the battery is changing, however if you choose to do this you will also need to alter the voltage divider values to make sure the voltage at GPIO 16 does not exceed 3.1V.

## Jumper

A jumper has been placed between the ESP32 5v pin and the switch. This jumper should be open when connecting the ESP32 to USB for debugging. Failure to open the jumper will result in voltage from ESP32 making its way to the batteries if the switch is also turned on. You could skip the jumper installation, and always make sure the switch is off when debugging, but you will need the jumper installed and open if you want to debug the battery level indicator code.

## 3D Files

View 3D files: [Single Screen 4 Button Handheld 3D Files](../assets/3d_files/gandw_single_screen_4button_handheld/)

## Button Circuit Boards

The buttons need to be mounted on 2.54mm pitch perfboard. 

The alarm and ACL buttons need to be mounted slightly high on the board. Use the spacer (alarm_acl_button_spacer.stl) to mount them at the correct height.

The diagrams below show how to mount the buttons:

<table>
<tr>
    <td><img src="../assets/perfboard_small_buttons_narrow.jpg" alt="" width="200"></td>
    <td><img src="../assets/perfboard_main_double_button.jpg" alt="" width="200"></td>
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
- LEDK1:      GND  

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
- LEFT DOWN:    GPIO 2
- RIGHT DOWN:   GPIO 1
- LEFT UP:      GPIO 17
- RIGHT UP:     GPIO 18
- ALARM:        GPIO 7
- ACL:          GPIO 8

## Battery Level
- Battery +:   GPIO 16 (via voltage divider)

# Roms

The roms must be in .gw format.

These can be created using LCD-Game-Shrinker.

https://github.com/bzhxx/LCD-Game-Shrinker

Below is a link to a guide describing how to use LCD-Game-Shrinker to generate the files.

https://gist.github.com/DNA64/16fed499d6bd4664b78b4c0a9638e4ef

Below is a list of the gw files required:

<table cellpadding="2" cellspacing="0">
<tr><td>Mame Rom</td><td>GW File</td></tr>
<tr><td>gnw_pchute</td><td>Game & Watch Parachute.gw</td></tr>
<tr><td>gnw_octopus</td><td>Game & Watch Octopus.gw</td></tr>
<tr><td>gnw_fire</td><td>Game & Watch Fire (Wide Screen).gw</td></tr>
<tr><td>gnw_chef</td><td>Game & Watch Chef.gw</td></tr>
<tr><td>gnw_popeye</td><td>Game & Watch Popeye (Wide Screen).gw</td></tr>
<tr><td>gnw_tbridge</td><td>Game & Watch Turtle Bridge.gw</td></tr>
<tr><td>gnw_vermin</td><td>Game & Watch Vermin.gw</td></tr>
<tr><td>gnw_ball</td><td>Game & Watch Ball.gw</td></tr>
<tr><td>gnw_fires</td><td>Game & Watch Fire (Silver).gw</td></tr>
<tr><td>gnw_helmet</td><td>Game & Watch Helmet (CN-17 version).gw</td></tr>
<tr><td>gnw_mariotj</td><td>Game & Watch Mario The Juggler.gw</td></tr>
<tr><td>gnw_tfish</td><td>Game & Watch Tropical Fish.gw</td></tr>
<tr><td>gnw_egg</td><td>Game & Watch Egg.gw</td></tr>
<tr><td>gnw_fireatk</td><td>Game & Watch Fire Attack.gw</td></tr>
<tr><td>gnw_flagman</td><td>Game & Watch Flagman.gw</td></tr>
<tr><td>gnw_judge</td><td>Game & Watch Judge (green version).gw</td></tr>
<tr><td>gnw_lion</td><td>Game & Watch Lion.gw</td></tr>
<tr><td>gnw_manhole</td><td>Game & Watch Manhole (New Wide Screen).gw</td></tr>
<tr><td>gnw_manholeg</td><td>Game & Watch Manhole (Gold).gw</td></tr>
<tr><td>gnw_mariocm</td><td>Game & Watch Mario's Cement Factory (New Wide Screen).gw</td></tr>
<tr><td>gnw_mmouse</td><td>Game & Watch Mickey Mouse (Wide Screen).gw</td></tr>
<tr><td>gnw_stennis</td><td>Game & Watch Snoopy Tennis.gw</td></tr>
</table>



# Building
Install the ESP-IDF framework. The version I used was v5.4.1, but it should also work with newer versions.
https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/get-started/index.html

Make sure you can build the hello_world example project.

Download the code from this repo.

Use LCD-Game-Shrinker to generate a game files (see above) and place the gw files in the /gandw_single_screen_4button_menu_handheld/main/ directory.

Go back to /gandw_single_screen_4button_menu_handheld/ directory and open the terminal or cmd window in this directory.

## Linux
These instructions are for linux. If you are using windows follow the same steps you did when building the hello_world example project.

Run the export.sh script or get_idf if you set it up.

Export script should be similar to: 

```
. $HOME/esp/esp-idf/export.sh
```

Build: 
```
idf.py build
```

Then: 
```
idf.py -p /dev/ttyACM0 flash
```

You can also monitor by adding monitor at the end: 
```
idf.py -p /dev/ttyACM0 flash monitor 
```

Press Ctrl and ] to exit monitor.

If you are getting errors try a fullclean before building: 
```
idf.py fullclean
```

# LCD Display

## Orientation

In the setup_lcd_spi function, there are 2 commands that you can use to change the screen orientation.

- esp_lcd_panel_swap_xy
- esp_lcd_panel_mirror

## Pixel Data

All the ILI9341 lcd panels i used required the pixel data to be byte swapped for the image to be displayed correctly.

I have swapped the byte order for the background and segment pixel data in lcd game emulator.

If the image displayed on your screen looks incorrect (wrong colours) you can change the byte swap setting.

In the file: /main/lcd_game_emulator/src/gw_sys/gw_system.h, set BYTE_SWAP to 0.

```
#define BYTE_SWAP 0
```


# Acknowledgements

## LCD-Game-Emulator
https://github.com/bzhxx/lcd-game-emulator

## Retro-Go
https://github.com/ducalex/retro-go

The LCD-Game-Emulator version used by the project was copied from Retro-Go because it contains additional code to handle the loading of the gw files.







