''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' @file
' Interface Library Graph Plot Demo
'
' @version @n 1.0
' @date @n 11/30/2014
'
' @author @n Kwabena W. Agyeman
' @copyright @n (c) 2014 Kwabena W. Agyeman
' @n All rights reserved - Please see the end of the file for the terms of use
'
' @par Update History:
' @n v1.0 - Original release - 11/30/2014
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

CON _CLKFREQ = 80_000_000
CON _CLKMODE = xtal1 + pll16x

CON RX_PIN = 31
CON TX_PIN = 30

CON ANALOG_VIN_MAX_VOLTAGE = 5 ' 0V-5V
CON ANALOG_VIN_MAX_DEC = 1023 ' 0-1023

CON SAMPLE_RATE = 100 ' hz (for fft)

OBJ core: "ilcore.spin"

CON NUM_OSCOPES = 1
OBJ oscopes[NUM_OSCOPES]: "iloscilloscope.spin"

CON ' Oscilloscope Window Handles

    WINDOW

CON NUM_GRAPHS = 4
OBJ graphs[NUM_GRAPHS]: "iloscilloscopegraph.spin"

CON ' Oscilloscope Graph Handles

    ANALOG_0
    ANALOG_1
    ANALOG_2
    ANALOG_3

OBJ f: "Float32Full.spin"

PUB main

    core.init(RX_PIN, TX_PIN)
    ' core.clearAll

    oscopes[WINDOW].setParentHandle(0)
    oscopes[WINDOW].setThisHandle(WINDOW)
    oscopes[WINDOW].init(string("Analog Window"))
    oscopes[WINDOW].setBackgroundColor($FF444444)

    oscopes[WINDOW].setXAxisLabel(string($CE, $94, " Time (s)")) ' utf-8 delta
    oscopes[WINDOW].setYAxisLabel(string("Voltage (v)"))
    oscopes[WINDOW].setYAxisRangeAutoScale(false)
    oscopes[WINDOW].setYAxisRangeLower(0)
    oscopes[WINDOW].setYAxisRangeUpper(ANALOG_VIN_MAX_VOLTAGE)

    oscopes[WINDOW].setFFTXAxisLabel(string("Frequency (hz)"))
    oscopes[WINDOW].setFFTYAxisLabel(string("Voltage (v)"))
    oscopes[WINDOW].setFFTYAxisRangeAutoScale(false)
    oscopes[WINDOW].setFFTYAxisRangeLower(0)
    oscopes[WINDOW].setFFTYAxisRangeUpper(1)

    graphs[ANALOG_0].setParentHandle(WINDOW)
    graphs[ANALOG_0].setThisHandle(ANALOG_0)
    graphs[ANALOG_0].init(string("Analog 0"))
    graphs[ANALOG_0].setDeltaMode(true)
    graphs[ANALOG_0].setSampleRate(SAMPLE_RATE)
    graphs[ANALOG_0].setScaler(ANALOG_VIN_MAX_DEC/ANALOG_VIN_MAX_VOLTAGE)
    graphs[ANALOG_0].setFFT(core#GFFT_MAGNITUDE)

    graphs[ANALOG_1].setParentHandle(WINDOW)
    graphs[ANALOG_1].setThisHandle(ANALOG_1)
    graphs[ANALOG_1].init(string("Analog 1"))
    graphs[ANALOG_1].setDeltaMode(true)
    graphs[ANALOG_1].setSampleRate(SAMPLE_RATE)
    graphs[ANALOG_1].setScaler(ANALOG_VIN_MAX_DEC/ANALOG_VIN_MAX_VOLTAGE)
    graphs[ANALOG_1].setFFT(core#GFFT_MAGNITUDE)

    graphs[ANALOG_2].setParentHandle(WINDOW)
    graphs[ANALOG_2].setThisHandle(ANALOG_2)
    graphs[ANALOG_2].init(string("Analog 2"))
    graphs[ANALOG_2].setDeltaMode(true)
    graphs[ANALOG_2].setSampleRate(SAMPLE_RATE)
    graphs[ANALOG_2].setScaler(ANALOG_VIN_MAX_DEC/ANALOG_VIN_MAX_VOLTAGE)
    graphs[ANALOG_2].setFFT(core#GFFT_MAGNITUDE)

    graphs[ANALOG_3].setParentHandle(WINDOW)
    graphs[ANALOG_3].setThisHandle(ANALOG_3)
    graphs[ANALOG_3].init(string("Analog 3"))
    graphs[ANALOG_3].setDeltaMode(true)
    graphs[ANALOG_3].setSampleRate(SAMPLE_RATE)
    graphs[ANALOG_3].setScaler(ANALOG_VIN_MAX_DEC/ANALOG_VIN_MAX_VOLTAGE)
    graphs[ANALOG_3].setFFT(core#GFFT_MAGNITUDE)

    f.start

    repeat

        graphs[ANALOG_0].addValueF(readAnalog0)
        graphs[ANALOG_1].addValueF(readAnalog1)
        graphs[ANALOG_2].addValueF(readAnalog2)
        graphs[ANALOG_3].addValueF(readAnalog3)

DAT readAnalog0i long 0.0
PUB readAnalog0

    ' Replace the code here with code to return analog data from some source...

    result := f.sin(f.radians(readAnalog0i))

    ' Scale Output
    result := f.fmul(result, f.ffloat(ANALOG_VIN_MAX_DEC/2))
    result := f.fadd(result, f.ffloat(ANALOG_VIN_MAX_DEC/2))

    readAnalog0i := f.fadd(readAnalog0i, 1.0) ' play with this number

DAT readAnalog1i long 0.0
DAT readAnalog1j long 0.0
PUB readAnalog1

    ' Replace the code here with code to return analog data from some source...

    result := f.fmul(f.sin(f.radians(readAnalog1i)), {
            } f.sin(f.radians(readAnalog1j)))

    ' Scale Output
    result := f.fmul(result, f.ffloat(ANALOG_VIN_MAX_DEC/2))
    result := f.fadd(result, f.ffloat(ANALOG_VIN_MAX_DEC/2))

    readAnalog1i := f.fadd(readAnalog1i, 1.0) ' play with this number
    readAnalog1j := f.fadd(readAnalog1j, 2.0) ' play with this number

DAT readAnalog2i long 0.0
DAT readAnalog2j long 0.0
DAT readAnalog2k long 0.0
PUB readAnalog2

    ' Replace the code here with code to return analog data from some source...

    result := f.fmul(f.sin(f.radians(readAnalog2i)), {
            } f.fmul(f.sin(f.radians(readAnalog2j)), {
            } f.sin(f.radians(readAnalog2k))))

    ' Scale Output
    result := f.fmul(result, f.ffloat(ANALOG_VIN_MAX_DEC/2))
    result := f.fadd(result, f.ffloat(ANALOG_VIN_MAX_DEC/2))

    readAnalog2i := f.fadd(readAnalog2i, 1.0) ' play with this number
    readAnalog2j := f.fadd(readAnalog2j, 2.0) ' play with this number
    readAnalog2k := f.fadd(readAnalog2k, 3.0) ' play with this number

DAT readAnalog3i long 0.0
DAT readAnalog3j long 0.0
DAT readAnalog3k long 0.0
DAT readAnalog3l long 0.0
PUB readAnalog3

    ' Replace the code here with code to return analog data from some source...

    result := f.fmul(f.sin(f.radians(readAnalog3i)), {
            } f.fmul(f.sin(f.radians(readAnalog3j)), {
            } f.fmul(f.sin(f.radians(readAnalog3k)), {
            } f.sin(f.radians(readAnalog3l)))))

    ' Scale Output
    result := f.fmul(result, f.ffloat(ANALOG_VIN_MAX_DEC/2))
    result := f.fadd(result, f.ffloat(ANALOG_VIN_MAX_DEC/2))

    readAnalog3i := f.fadd(readAnalog3i, 1.0) ' play with this number
    readAnalog3j := f.fadd(readAnalog3j, 2.0) ' play with this number
    readAnalog3k := f.fadd(readAnalog3k, 3.0) ' play with this number
    readAnalog3l := f.fadd(readAnalog3l, 4.0) ' play with this number

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' @file
' @par MIT License - TERMS OF USE:
' @n Permission is hereby granted, free of charge, to any person obtaining a
' copy of this software and associated documentation files (the "Software"), to
' deal in the Software without restriction, including without limitation the
' rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
' sell copies of the Software, and to permit persons to whom the Software is
' furnished to do so, subject to the following conditions:
' @n
' @n The above copyright notice and this permission notice shall be included in
' all copies or substantial portions of the Software.
' @n
' @n THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
' IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
' FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
' AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
' LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
' OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
' SOFTWARE.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
