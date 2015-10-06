''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' @file
' Interface Library Bar Plot Demo
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

OBJ core: "ilcore.spin"

CON NUM_OSCOPES = 1
OBJ oscopes[NUM_OSCOPES]: "iloscilloscope.spin"

CON ' Oscilloscope Window Handles

    WINDOW

CON NUM_BARS = 4
OBJ bars[NUM_BARS]: "iloscilloscopebar.spin"

CON ' Oscilloscope Bar Handles

    BAR_0
    BAR_1
    BAR_2
    BAR_3

OBJ f: "Float32Full.spin"

PUB main | i, j, k, l

    core.init(RX_PIN, TX_PIN)
    ' core.clearAll

    oscopes[WINDOW].setParentHandle(0)
    oscopes[WINDOW].setThisHandle(WINDOW)
    oscopes[WINDOW].init(string("Bar Window"))
    oscopes[WINDOW].setYAxisRangeLower(-1)
    oscopes[WINDOW].setYAxisRangeUpper(+1)

    bars[BAR_0].setParentHandle(WINDOW)
    bars[BAR_0].setThisHandle(BAR_0)
    bars[BAR_0].init(string("Graph 0"))
    bars[BAR_0].setSize(100)

    bars[BAR_1].setParentHandle(WINDOW)
    bars[BAR_1].setThisHandle(BAR_1)
    bars[BAR_1].init(string("Graph 1"))
    bars[BAR_1].setSize(100)

    bars[BAR_2].setParentHandle(WINDOW)
    bars[BAR_2].setThisHandle(BAR_2)
    bars[BAR_2].init(string("Graph 2"))
    bars[BAR_2].setSize(100)

    bars[BAR_3].setParentHandle(WINDOW)
    bars[BAR_3].setThisHandle(BAR_3)
    bars[BAR_3].init(string("Graph 3"))
    bars[BAR_3].setSize(100)

    f.start

    i := 0.0
    j := 0.0
    k := 0.0
    l := 0.0

    repeat

        bars[BAR_0].addValueF(f.sin(f.radians(i)))
        bars[BAR_1].addValueF(f.sin(f.radians(j)))
        bars[BAR_2].addValueF(f.sin(f.radians(k)))
        bars[BAR_3].addValueF(f.sin(f.radians(l)))

        i := f.fadd(i, 1.0)
        j := f.fadd(j, 2.0)
        k := f.fadd(k, 4.0)
        l := f.fadd(l, 8.0)

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
