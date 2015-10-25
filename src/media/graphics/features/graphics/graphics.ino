/**
* @file graphics.ino
* Brief Description
*
* @version @n 0.1
* @date @n Sun Sep 28 21:05:54 2014
*
* @author @n Kwabena
*/

#include <InterfaceLibrary.h>

ILCore core; // interface library
ILGraphics graphics; // graphics window

void setup() {
    core.init();
    core.clearAll();
    graphics.init(&core, "Simple Shapes");
    graphics.clearAll();

    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            graphics.drawRectangleF(i, j, i + 0.5, j + 0.5);
        }
    }
}

void loop() {
}
