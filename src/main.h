// by: Kwabena W. Agyeman

#ifndef MAIN_H
#define MAIN_H

#define __TOSTRING__(x) #x
#define TOSTRING(x) __TOSTRING__(x)
#define PROJECT_FULL_NAME_STR TOSTRING(PROJECT_FULL_NAME)
#define PROJECT_FULL_NAME_WO_SPACES_STR TOSTRING(PROJECT_FULL_NAME_WO_SPACES)
#define PROJECT_SHORT_NAME_STR TOSTRING(PROJECT_SHORT_NAME)
#define PROJECT_VERSION_STR TOSTRING(PROJECT_VERSION)
#define PROJECT_VENDOR_STR TOSTRING(PROJECT_VENDOR)
#define PROJECT_COPYRIGHT_STR TOSTRING(PROJECT_COPYRIGHT)
#define PROJECT_DOMAIN_NAME_STR TOSTRING(PROJECT_DOMAIN_NAME)
#define PROJECT_URL_STR TOSTRING(PROJECT_URL) // unusable
#define PROJECT_URL_WO_SLASH_STR TOSTRING(PROJECT_URL_WO_SLASH) // unusable
#define PROJECT_EMAIL_STR TOSTRING(PROJECT_EMAIL)

// Main Window /////////////////////////////////////////////////////////////////

#define MAINWINDOW "MainWindow"

#define MAINWINDOW_KEY_GROUP \
(PROJECT_FULL_NAME_WO_SPACES_STR "/" MAINWINDOW)

#define MAINWINDOW_LAST_PORT "lastPort"
#define MAINWINDOW_LAST_TCP_PORT "lastTCPPort"
#define MAINWINDOW_LAST_UDP_PORT "lastUDPPort"

// Serial Escape //////////////////////////////////////////////////////////////

#define SERIAL_ESCAPE "SerialEscape"

#define SERIAL_ESCAPE_KEY_GROUP \
(PROJECT_FULL_NAME_WO_SPACES_STR "/" SERIAL_ESCAPE)

#define SERIAL_ESCAPE_KEY_OPEN_PROCESS_EN "openProcessEn"
#define SERIAL_ESCAPE_KEY_OPEN_URL_EN "openUrlEn"

#define SERIAL_ESCAPE_JSON "SerialEscapeJSON"

#define SERIAL_ESCAPE_JSON_KEY_GROUP \
(PROJECT_FULL_NAME_WO_SPACES_STR "/" SERIAL_ESCAPE_JSON)

#define SERIAL_ESCAPE_JSON_KEY_FILE "file"

// Serial Terminal ////////////////////////////////////////////////////////////

#define SERIAL_TERMINAL "SerialTerminal"

#define SERIAL_TERMINAL_KEY_GROUP \
(PROJECT_FULL_NAME_WO_SPACES_STR "/" SERIAL_TERMINAL)

#define SERIAL_TERMINAL_KEY_STATE "state"
#define SERIAL_TERMINAL_KEY_GEOMETRY "geometry"
#define SERIAL_TERMINAL_KEY_OPEN_FILE "openFile"
#define SERIAL_TERMINAL_KEY_SAVE_FILE "saveFile"
#define SERIAL_TERMINAL_KEY_TAB_WIDTH "tabWidth"

// Serial Oscilloscope ////////////////////////////////////////////////////////

#define SERIAL_OSCILLOSCOPE "SerialOscilloscope"

#define SERIAL_OSCILLOSCOPE_KEY_GROUP \
(PROJECT_FULL_NAME_WO_SPACES_STR "/" SERIAL_OSCILLOSCOPE)

#define SERIAL_OSCILLOSCOPE_KEY_STATE "state"
#define SERIAL_OSCILLOSCOPE_KEY_GEOMETRY "geometry"
#define SERIAL_OSCILLOSCOPE_KEY_SAVE_R_FILE "saveRasterFile"
#define SERIAL_OSCILLOSCOPE_KEY_SAVE_V_FILE "saveVectorFile"
#define SERIAL_OSCILLOSCOPE_KEY_IMPORT_FILE "importFile"
#define SERIAL_OSCILLOSCOPE_KEY_EXPORT_FILE "exportFile"

#define SERIAL_OSCILLOSCOPE_FFTW "SerialOscilloscopeFFTW"

#define SERIAL_OSCILLOSCOPE_FFTW_KEY_GROUP \
(PROJECT_FULL_NAME_WO_SPACES_STR "/" SERIAL_OSCILLOSCOPE_FFTW)

#define SERIAL_OSCILLOSCOPE_FFTW_KEY_WISDOM "wisdom"

// Serial Tree ////////////////////////////////////////////////////////////////

#define SERIAL_TREE "SerialTree"

#define SERIAL_TREE_KEY_GROUP \
(PROJECT_FULL_NAME_WO_SPACES_STR "/" SERIAL_TREE)

#define SERIAL_TREE_KEY_STATE "state"
#define SERIAL_TREE_KEY_GEOMETRY "geometry"
#define SERIAL_TREE_KEY_OPEN_FILE "openFile"
#define SERIAL_TREE_KEY_SAVE_FILE "saveFile"
#define SERIAL_TREE_KEY_SAVE_FILE_CSV "saveFileCSV"

// Serial Table ///////////////////////////////////////////////////////////////

#define SERIAL_TABLE "SerialTable"

#define SERIAL_TABLE_KEY_GROUP \
(PROJECT_FULL_NAME_WO_SPACES_STR "/" SERIAL_TABLE)

#define SERIAL_TABLE_KEY_STATE "state"
#define SERIAL_TABLE_KEY_GEOMETRY "geometry"
#define SERIAL_TABLE_KEY_OPEN_FILE "openFile"
#define SERIAL_TABLE_KEY_SAVE_FILE "saveFile"
#define SERIAL_TABLE_KEY_SAVE_FILE_CSV "saveFileCSV"

// Serial Interface ///////////////////////////////////////////////////////////

#define SERIAL_INTERFACE "SerialInterface"

#define SERIAL_INTERFACE_KEY_GROUP \
(PROJECT_FULL_NAME_WO_SPACES_STR "/" SERIAL_INTERFACE)

#define SERIAL_INTERFACE_KEY_STATE "state"
#define SERIAL_INTERFACE_KEY_GEOMETRY "geometry"

// Serial Graphics ////////////////////////////////////////////////////////////

#define SERIAL_GRAPHICS "SerialGraphics"

#define SERIAL_GRAPHICS_KEY_GROUP \
(PROJECT_FULL_NAME_WO_SPACES_STR "/" SERIAL_GRAPHICS)

#define SERIAL_GRAPHICS_KEY_STATE "state"
#define SERIAL_GRAPHICS_KEY_GEOMETRY "geometry"
#define SERIAL_GRAPHICS_KEY_SAVE_R_FILE "saveRasterFile"
#define SERIAL_GRAPHICS_KEY_SAVE_V_FILE "saveVectorFile"
#define SERIAL_GRAPHICS_KEY_IMPORT_FILE "importFile"
#define SERIAL_GRAPHICS_KEY_EXPORT_FILE "exportFile"

#endif // MAIN_H
