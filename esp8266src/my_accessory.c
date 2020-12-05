/*
 * my_accessory.c
 * Define the accessory in C language using the Macro in characteristics.h
 *
 *  Created on: 2020-05-15
 *      Author: Mixiaoxiao (Wang Bin)
 */

#include <homekit/homekit.h>
#include <homekit/characteristics.h>

void my_accessory_identify(homekit_value_t _value) {
	printf("accessory identify\n");
}

// target temperature and target state
uint8_t returnState = 0;
float returnTemp = 10; // init target temperature with min value of 10 

// callback function which returns the target state set by apple device
homekit_value_t getState()
{
    printf("FunctionCall: getState()");
    return HOMEKIT_UINT8(returnState);
}
// callback funcion which returns the target temperature set by apple device
homekit_value_t getTemp()
{
    //homekit_value_t temp;
    //temp.format = homekit_format_float;
    //temp.float_value = 12;
    printf("Functioncall: getTemp()");
    return HOMEKIT_FLOAT(returnTemp);
}
// callback function which return the target disply unit
homekit_value_t getDispUnit()
{
    printf("Functioncall: getDispUnit()");
    return HOMEKIT_UINT8(0);
}

// Define the required homekit_characteristics to fullfill the homekit protocol
homekit_characteristic_t currState = HOMEKIT_CHARACTERISTIC_(
                                                              CURRENT_HEATING_COOLING_STATE,              // current state of the system, in this case 0: Off and 1: Heating
                                                              (uint8_t) 0,                                // Init value 0: Off
                                                              .valid_values={                             // adjust the valid values, since there are multiple not supproted states 2: cooling 3: auto
                                                                              .count=2, 
                                                                              .values=(uint8_t[]) {0, 1}}
                                                             );
                                                             
homekit_characteristic_t reqState  = HOMEKIT_CHARACTERISTIC_(
                                                              TARGET_HEATING_COOLING_STATE,               // required state of the system
                                                              (uint8_t) 0,                                // Init value 0: Off
                                                              .getter = getState, 
                                                              .valid_values={
                                                                              .count=2, 
                                                                              .values=(uint8_t[]) {0, 1}}
                                                             );
                                                             
homekit_characteristic_t currTemp  = HOMEKIT_CHARACTERISTIC_(
                                                              CURRENT_TEMPERATURE,                        // Current temperature meassured by the thermostat (in this case the target value
                                                              (float) 0                                   // no sensor available
                                                             );
                                                             
homekit_characteristic_t reqTemp   = HOMEKIT_CHARACTERISTIC_(
                                                              TARGET_TEMPERATURE,                         // Target temperatur for the room
                                                              (float) 0,                                  // temperature set to zero
                                                              .getter = getTemp                           // getter function: Apple Devices request the target temperature with this callback function
                                                             );
                                                             
homekit_characteristic_t unitDisp  = HOMEKIT_CHARACTERISTIC_(
                                                              TEMPERATURE_DISPLAY_UNITS,                  // Display unit which is used in the thermostat 0: Celsius, 1: Fahrenheit
                                                              (uint8_t) 0,                                // Set to Celsius
                                                              .getter = getDispUnit                       // getter function: Apple Devices request the target display unit with this callback funcion
                                                             );

 // create accessory with two different services: ACCESSORY_INFORMATION and THERMOSTAT
homekit_accessory_t *accessories[] = {                                                                         
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_thermostat, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "ESP8266 Thermostat"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Arduino HomeKit"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123456"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266/ESP32"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
        // add characteristics to accessory
        HOMEKIT_SERVICE(THERMOSTAT, .primary=true, .characteristics=(homekit_characteristic_t*[]) {              
            &currState,
            &reqState,
            &currTemp,
            &reqTemp,
            &unitDisp,
            NULL
        }),
        NULL
    }),
    NULL
};

// put all accessories into homekit configuration and add password to config
homekit_server_config_t accessory_config = { 
    .accessories = accessories,
    .password = "111-11-111"
};
