/**
* @file table.ino
* Brief Description
*
* @version @n 0.1
* @date @n Sat Sep 6 18:09:53 2014
*
* @author @n Kwabena
*/

#include <InterfaceLibrary.h>

ILCore core; // interface library
ILTable table; // tree window

void setup() {
    core.init();
    table.init(&core, "Sensor Gradient");

    for(int i = 0; i < 10; i++) {
        table.newRow(i, i);
    }

    for(int j = 0; j < 10; j++) {
        table.newColumn(j, j);
    }

    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            float k = radians(map(i-5, -5, 4, -90, 90));
            float l = radians(map(j-5, -5, 4, -90, 90));
            table.newItem(i, j, 10*10*((cos(k)*cos(k))+(cos(l)*cos(l)))*((cos(k)*cos(k))+(cos(l)*cos(l))));
        }
    }
}

void loop() {
}
