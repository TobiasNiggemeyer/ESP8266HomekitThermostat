# ESP8266HomekitThermostat

This project is used to modify the "Eqiva Thermostat Model N, 132231K0A" to enable Apple Homekit using an ESP8266-01.
The source code is based on Mixiaoxio Arduino-HomeKit-ESP8266 project (see https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266).
The GPIOs 0 and 2 of the esp are connected to the rotary encoder of the thermostat and simulate it (see https://www.youtube.com/watch?v=m0ZxRcsp5y4). To use this extension for the thermostat, connect the two outputs of the rotary encoder to T1 and T2 of the PCB and the GND pin of the rotary encoder to GND of the PCB (see images). Moreover, the voltage level of the outputs of the rotary encoder must be high since the system pulls the signals to GND (turn rotary encoder once -> check if the system works -> if not turn again). 

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

### Hint: 
R1 and R4 are used to pull the GPIOs of the ESP to a floating level of around 1V to enable the direct boot of the esp (esp enters different modes if one or both GPIOs are pulled to GND).


## Instructions

- change SSID and password in wifi_info.h
- compile and flash software to esp8266-01
  - power the esp: 3.3V to pin 8 and 4, GND to pin 1
  - connect usb to seral converter: TX->RX(7), RX->TX(2), GND->GNC(1)
  - pull GPIO-0(5) to GND (use 10k resistor)
  - reset esp by connection Reset(6) to GND
  - disconnect GPIO-0(5) and Reset(6) from ground
  - compile and flash esp8266-01 using the arduino IDE
<img src="https://github.com/TobiasNiggemeyer/ESP8266HomekitThermostat/blob/main/electronicHousing/esp8266Pinout.png" height="250" height="250"> 


## Open Points

- Improve housing for the electronics
  - smaller
  - better fit
- removal of power supply by battery 
- create detailed/professional schematics
- add an LED to indicate if the signal of the rotary encoder is high (needed to simulate the rotary encoder with the esp)

### Feel free to ask questions and improve the thermostat
