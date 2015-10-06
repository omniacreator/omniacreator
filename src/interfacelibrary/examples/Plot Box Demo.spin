''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' @file
' Interface Library Box Plot Demo
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

CON NUM_BOXES = 4
OBJ boxes[NUM_BOXES]: "iloscilloscopebox.spin"

CON ' Oscilloscope Box Handles

    BOX_0
    BOX_1
    BOX_2
    BOX_3

PUB main

    core.init(RX_PIN, TX_PIN)
    ' core.clearAll

    oscopes[WINDOW].setParentHandle(0)
    oscopes[WINDOW].setThisHandle(WINDOW)
    oscopes[WINDOW].init(string("Box Window"))
    oscopes[WINDOW].setYAxisRangeLower(-10)
    oscopes[WINDOW].setYAxisRangeUpper(+70)

    boxes[BOX_0].setParentHandle(WINDOW)
    boxes[BOX_0].setThisHandle(BOX_0)
    boxes[BOX_0].init(string("Stat Box 0"))

    boxes[BOX_1].setParentHandle(WINDOW)
    boxes[BOX_1].setThisHandle(BOX_1)
    boxes[BOX_1].init(string("Stat Box 1"))

    boxes[BOX_2].setParentHandle(WINDOW)
    boxes[BOX_2].setThisHandle(BOX_2)
    boxes[BOX_2].init(string("Stat Box 2"))

    boxes[BOX_3].setParentHandle(WINDOW)
    boxes[BOX_3].setThisHandle(BOX_3)
    boxes[BOX_3].init(string("Stat Box 3"))

    boxes[BOX_0].setKey(0) ' x position
    boxes[BOX_1].setKey(1) ' x position
    boxes[BOX_2].setKey(2) ' x position
    boxes[BOX_3].setKey(3) ' x position

    repeat

        boxes[BOX_0].setMinimum(((||(cnt?)) // 10) + 0)
        boxes[BOX_0].setLowerQuartile(((||(cnt?)) // 10) + 10)
        boxes[BOX_0].setMedian(((||(cnt?)) // 10) + 30)
        boxes[BOX_0].setUpperQuartile(((||(cnt?)) // 10) + 50)
        boxes[BOX_0].setMaximum(((||(cnt?)) // 10) + 60)

        boxes[BOX_1].setMinimum(((||(cnt?)) // 10) + 0)
        boxes[BOX_1].setLowerQuartile(((||(cnt?)) // 10) + 10)
        boxes[BOX_1].setMedian(((||(cnt?)) // 10) + 30)
        boxes[BOX_1].setUpperQuartile(((||(cnt?)) // 10) + 50)
        boxes[BOX_1].setMaximum(((||(cnt?)) // 10) + 60)

        boxes[BOX_2].setMinimum(((||(cnt?)) // 10) + 0)
        boxes[BOX_2].setLowerQuartile(((||(cnt?)) // 10) + 10)
        boxes[BOX_2].setMedian(((||(cnt?)) // 10) + 30)
        boxes[BOX_2].setUpperQuartile(((||(cnt?)) // 10) + 50)
        boxes[BOX_2].setMaximum(((||(cnt?)) // 10) + 60)

        boxes[BOX_3].setMinimum(((||(cnt?)) // 10) + 0)
        boxes[BOX_3].setLowerQuartile(((||(cnt?)) // 10) + 10)
        boxes[BOX_3].setMedian(((||(cnt?)) // 10) + 30)
        boxes[BOX_3].setUpperQuartile(((||(cnt?)) // 10) + 50)
        boxes[BOX_3].setMaximum(((||(cnt?)) // 10) + 60)

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
