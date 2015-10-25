/**
* @file terminal.ino
* Brief Description
*
* @version @n 0.1
* @date @n Sun Sep 7 16:59:20 2014
*
* @author @n Kwabena
*/

#include <InterfaceLibrary.h>

ILCore core;
ILTerminal term;

void setup() {
    core.init();
    term.init(&core, "Demo Text");
    term.write(CC_SET_FOREGROUND_TO_RED);
    term.print(" _______                  __               \n"
               "|       |.--------.-----.|__|.---.-.       \n"
               "|   -   ||        |     ||  ||  _  |       \n"
               "|_______||__|__|__|__|__||__||___._|       \n"
               "                                           \n");
    term.write(CC_SET_FOREGROUND_TO_GREEN);
    term.print(" ______                    __              \n"
               "|      |.----.-----.---.-.|  |_.-----.----.\n"
               "|   ---||   _|  -__|  _  ||   _|  _  |   _|\n"
               "|______||__| |_____|___._||____|_____|__|  \n"
               "                                           \n");
    term.write(CC_SET_FOREGROUND_TO_BLUE);
    term.print(" _______                       __               __ \n"
               "|_     _|.-----.----.--------.|__|.-----.---.-.|  |\n"
               "  |   |  |  -__|   _|        ||  ||     |  _  ||  |\n"
               "  |___|  |_____|__| |__|__|__||__||__|__|___._||__|\n"
               "                                                   \n");
}

void loop() {
}
