/**
* @file oscilloscope.ino
* Brief Description
*
* @version @n 0.1
* @date @n Sat Sep 6 16:52:13 2014
*
* @author @n Kwabena
*/

#include <InterfaceLibrary.h>

ILCore core; // interface library
ILOscilloscope oscope; // graphing window
ILOscilloscopeGraph graph;
float i = 0, j = 0;

void setup() {
    core.init();
    oscope.init(&core, "Amplitude Modulation");
    graph.init(&oscope, "");
    graph.setFillStyle(FS_SOLID_PATTERN);
}

void loop() {
    graph.addValueF(sin(radians(i)) * cos(radians(j)));
    i += 1;
    j += 10;
    delay(100);
}
