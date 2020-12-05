# ESP8266HomekitThermostat

This project is used to modify the "Eqiva Thermostat Model N, 132231K0A" to enable Apple Homekit using an ESP8266-01.
The source code is based on Mixiaoxio Arduino-HomeKit-ESP8266 project (see https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266).

The project includes the following parts:
  - ESP8266 source code
  - 3d model to interate the ESP into the thermostat (not perfect but works)
  - electronic schematic and layout
  
## Components/Costs
  - Eqiva Thermostat Model N, 132231K0A   10,00€
  - 2x NPN-transistor:                    0,700€
  - 4x 1k resistor:                       0,070€
  - 2x 10k resistor:                      0,035€
  - 1x xx-xx stripe grid board:           0,500€
  - 1x esp8266-01:                        3,000€
  - 1x 5V-3.3V regulator:                 0.700€
  - some wires                            -
### Sum:                                    15,00€
  
## Images
 <div class="row">
  <div class="column">
    <img src="https://github.com/TobiasNiggemeyer/ESP8266HomekitThermostat/blob/main/electronicHousing/electronicHousing.png" height="250">
    <img src="https://github.com/TobiasNiggemeyer/ESP8266HomekitThermostat/blob/main/electronicHousing/mountedThermostat_0.jpg" height="250">
    <img src="https://github.com/TobiasNiggemeyer/ESP8266HomekitThermostat/blob/main/electronicHousing/mountedThermostat_1.jpg" height="250">
    <img src="https://github.com/TobiasNiggemeyer/ESP8266HomekitThermostat/blob/main/electronicHousing/openHousing_0.jpg" height="250">
    <img src="https://github.com/TobiasNiggemeyer/ESP8266HomekitThermostat/blob/main/electronicHousing/openHousing_1.jpg" height="250">
    <img src="https://github.com/TobiasNiggemeyer/ESP8266HomekitThermostat/blob/main/electronicHousing/Homekit.jpeg" height="250">
  </div>
</div> 


## Schematics
<img src="https://github.com/TobiasNiggemeyer/ESP8266HomekitThermostat/blob/main/electronicHousing/schematics.png" height="500" class="center">

## Instructions

- change SSID and password in wifi_info.h
- compile and flash software to esp8266-01
  - power the esp: 3.3V to pin 8 and 4, GND to pin 1
  - connect usb to seral converter: TX->RX(7), RX->TX(2), GND->GNC(1)
  - pull GPIO-0(5) to GND (use 10k resistor)
  - reset esp by connection Reset(6) to GND
  - disconnect GPIO-0(5) and Reset(6) from ground
  - compile and flash esp8266-01 using the arduino IDE
<img src="https://github.com/TobiasNiggemeyer/ESP8266HomekitThermostat/blob/main/electronicHousing/esp8266Pinout.png" height="500" height="250"> 
  
