/**
* @file interface.ino
* Brief Description
*
* @version @n 0.1
* @date @n Sun Sep 7 11:32:53 2014
*
* @author @n Kwabena
*/

#include <InterfaceLibrary.h>

#define STR_BUF 32
#define DO_GROUPS 2
#define DO_PINS 14
#define AI_PINS 6

ILCore core; // interface library
ILInterface interface; // interface window
ILInterfaceGroupBox groups[DO_GROUPS]; // group boxes
ILInterfaceCheckBox pinControl[DO_PINS]; // check boxes

ILInterfaceGroupBox analogIn;
ILInterfaceLabel analogInputs[AI_PINS];

void setup() {
    core.init();
    interface.init(&core, "Pin Control");
    groups[0].init(&interface, "Pins 00-06");
    groups[1].init(&interface, "Pins 07-13");

    for(int i = 0; i < DO_PINS; i++) {
        char s[STR_BUF] = {};
        snprintf(s, sizeof(s), "Pin %d", i);
        pinControl[i].init(i < (DO_PINS / DO_GROUPS) ? &groups[0] : &groups[1], s);
        pinMode(i, OUTPUT);
    }

    analogIn.init(&interface, "Analog In");

    for(int i = 0; i < AI_PINS; i++) {
        char s[STR_BUF] = {};
        snprintf(s, sizeof(s), "A%d", i);
        analogInputs[i].init(&analogIn, s);
    }
}

void loop() {
    core.idleTask();

    for(int i = 0; i < DO_PINS; i++) {
        digitalWrite(i, pinControl[i].getChecked());
    }

    for(int i = 0; i < AI_PINS; i++) {
        char s[STR_BUF] = {};
        snprintf(s, sizeof(s), "A%d = %d volts", i, ((analogRead(i) + 100) * 5) / 1024);
        analogInputs[i].setText(s);
    }
}
