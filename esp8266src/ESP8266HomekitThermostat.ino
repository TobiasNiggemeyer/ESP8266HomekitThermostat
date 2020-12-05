/*
 * Example05_WS2812_Neopixel.ino
 *
 *  Created on: 2020-10-01
 *      Author: Juergen Fink
 *	Thanks to all the other helpful people commenting here.
 *
 * This example allows to change brightness and color of a connected neopixel strip/matrix
 *
 * You should:
 * 1. read and use the Example01_TemperatureSensor with detailed comments
 *    to know the basic concept and usage of this library before other examples。
 * 2. erase the full flash or call homekit_storage_reset() in setup()
 *    to remove the previous HomeKit pairing storage and
 *    enable the pairing with the new accessory of this new HomeKit example.
 */


#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"
#include <homekit/types.h>

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);
int gpioToThermo1 = 0;
int gpioToThermo2 = 2;
void setup() {
	Serial.begin(115200);
	wifi_connect(); // in wifi_info.h
  //homekit_storage_reset();
  delay(1000);
  pinMode(gpioToThermo1, OUTPUT);
  pinMode(gpioToThermo2, OUTPUT);
	my_homekit_setup();
}

void loop() {
	my_homekit_loop();
	delay(10);
}

//==============================
// HomeKit setup and loop
//==============================

// access your HomeKit characteristics defined in my_accessory.c

extern "C" homekit_server_config_t accessory_config;
extern "C" homekit_characteristic_t currState;
extern "C" homekit_characteristic_t reqState;
extern "C" homekit_characteristic_t currTemp;
extern "C" homekit_characteristic_t reqTemp;
extern "C" homekit_characteristic_t unitDisp;
extern "C" uint8_t returnState;
extern "C" float returnTemp;

static uint32_t next_heap_millis = 0;
bool sendTempToHomekitDevice = FALSE;

void my_homekit_setup() {
  reqState.setter = setState;               // setter function, which is called if the apple decive changes the state
  reqTemp.setter  = setTemp;                // setter function, which is called if the apple device changes the temperature
  unitDisp.setter = setDispUnit;            // setter function, which is called if the apple device changes the display unit  
	arduino_homekit_setup(&accessory_config); // lets create the apple homekit accessory
}

int  gradToZero    = 35;    // since we need to set the thermostat to zero (no feedback) we need the max value of the thermostat
bool setThermoTemp = FALSE; // Flag which is used to set the temperature in main loop
int  setZeroState  = 0;     // different states, which include the state of each GPIO
bool setThermoZero = FALSE; // Flag which is used to set the temperature to zero in main loop
int  setZeroCnt    = 0;     // count how much celsius are decreased 
int  setTempState  = 0;     // different states, which include the state of each GPIO
int  setTempCnt    = 0;     // count how much ceksius are increased
float targetTemp   = 10;     // requested target temperature send by the apple device
float lastSetTemp  = 10;     // remember the last set value
int   delaySwtTimeSetThermo   = 10; // difference time between rising and falling edges: 10 ms
int   delayOnTimeSetThermo    = 50; // time the GPIO stays on and off: 50 ms
static uint32_t nextSetThermo = 0;  // time which triggers the next state change of the GPIO



// main loop for homekit device
void my_homekit_loop() {
	arduino_homekit_loop();
	const uint32_t t = millis();
	if (t > next_heap_millis) { 
		// show heap info every 5 seconds
		next_heap_millis = t + 5 * 1000;
		LOG_D("Free heap: %d, HomeKit clients: %d",ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
    if(sendTempToHomekitDevice == TRUE){
      //float temperature_value = random_value(10, 30); // FIXME, read your real sensor here.
      currTemp.value.float_value = targetTemp;
      homekit_characteristic_notify(&currTemp, currTemp.value);      
    }
	}
  // set the thermostat if needed
  if(t > nextSetThermo){
    if(setThermoTemp){
      if(setThermoZero){
        switch(setZeroState){
          case 0:
            nextSetThermo = t + delaySwtTimeSetThermo;
            digitalWrite(gpioToThermo1,LOW);
            setZeroState = 1;
          break;
          case 1:
            nextSetThermo = t + delayOnTimeSetThermo;
            digitalWrite(gpioToThermo2,LOW);
            setZeroState = 2;
          break;
          case 2:
            nextSetThermo = t + delaySwtTimeSetThermo;
            digitalWrite(gpioToThermo1,HIGH);
            setZeroState = 3;
          break;
          case 3:
            nextSetThermo = t + delayOnTimeSetThermo;
            digitalWrite(gpioToThermo2,HIGH);
            setZeroState = 0;
          break;
          }
          setZeroCnt = setZeroCnt + 1;
          if(setZeroCnt >= gradToZero*4){
            Serial.print("setZeroCnt: ");
            Serial.println(setZeroCnt);
            Serial.print("setZeroState: ");
            Serial.println(setZeroState);
            setZeroCnt = 0;
            setTempCnt = 0;
            setThermoZero = FALSE;
          }
      }
      else if(!setThermoZero && returnState != 0){
        switch(setTempState){
          case 0:
            nextSetThermo = t + delaySwtTimeSetThermo;
            digitalWrite(gpioToThermo2,LOW);
            setTempState = 1;
          break;
          case 1:
            nextSetThermo = t + delayOnTimeSetThermo;
            digitalWrite(gpioToThermo1,LOW);
            setTempState = 2;
          break;
          case 2:
            nextSetThermo = t + delaySwtTimeSetThermo;
            digitalWrite(gpioToThermo2,HIGH);
            setTempState = 3;
          break;
          case 3:
            nextSetThermo = t + delayOnTimeSetThermo;
            digitalWrite(gpioToThermo1,HIGH);
            setTempState = 0;
          break;
          }
          setTempCnt = setTempCnt + 1;
          if(setTempCnt >= ((int)targetTemp-5)*4+2){
            Serial.print("setTempCnt: ");
            Serial.println(setTempCnt);
            Serial.print("setTempState: ");
            Serial.println(setTempState);
            setTempCnt = 0;
            setZeroCnt = 0;
	    setTempState = 0;
            setThermoZero = TRUE;
            setThermoTemp = FALSE;
          }    
      }
      else{
          setThermoZero = TRUE;
          setThermoTemp = FALSE;        
      }
    }
    else{
      nextSetThermo = t + delayOnTimeSetThermo;
    }
    
  }
}

void setState(const homekit_value_t v) {
  Serial.println("setState");
  sendTempToHomekitDevice = TRUE;
  uint8_t state = v.int_value;
  returnState = state;
  reqState.value.int_value = state;
  if(state == 0){
    returnTemp = 10;
    targetTemp = 10;
    setThermoTemp = TRUE;
    setThermoZero = TRUE;
    reqTemp.value.float_value = 10;
  }
  else{
    returnTemp = lastSetTemp;
    targetTemp = lastSetTemp;
    setThermoTemp = TRUE;
    setThermoZero = TRUE;
    reqTemp.value.float_value = lastSetTemp;    
  }
}

void setTemp(const homekit_value_t v) {
  Serial.println("setTemp");
  float temp = v.float_value;
  returnTemp = temp;
  targetTemp = temp;
  lastSetTemp = temp;
  setThermoTemp = TRUE;
  setThermoZero = TRUE;
  reqTemp.value.float_value = temp;
}

void setDispUnit(const homekit_value_t v) {
  Serial.println("setDispUnit");
  uint8_t dispUnit = v.int_value;
  reqTemp.value.float_value = dispUnit;
}
