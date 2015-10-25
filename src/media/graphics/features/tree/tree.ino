/**
* @file tree.ino
* Brief Description
*
* @version @n 0.1
* @date @n Sat Sep 6 17:20:49 2014
*
* @author @n Kwabena
*/

#include <InterfaceLibrary.h>

ILCore core; // interface library
ILTree tree; // tree window

const char *frontLeftVolts = "front/left/volts";
const char *frontLeftAmps = "front/left/amps";

const char *frontRightVolts = "front/right/volts";
const char *frontRightAmps = "front/right/amps";

const char *backLeftVolts = "back/left/volts";
const char *backLeftAmps = "back/left/amps";

const char *backRightVolts = "back/right/volts";
const char *backRightAmps = "back/right/amps";

void setup() {
    core.init();
    tree.init(&core, "Motor Stats");

    tree.newItem(frontLeftVolts, "5.05 V");
    tree.newItem(frontLeftAmps, "1.10 A");

    tree.newItem(frontRightVolts, "4.95 V");
    tree.newItem(frontRightAmps, "1.00 A");

    tree.newItem(backLeftVolts, "5.00 V");
    tree.newItem(backLeftAmps, "1.05 A");

    tree.newItem(backRightVolts, "5.10 V");
    tree.newItem(backRightAmps, "1.15 A");
}

void loop() {
}
