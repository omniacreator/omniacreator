''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' @file
' Interface Library Core Serial Demo
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

CON NEW_LINE = 10

OBJ core: "ilcore.spin"

CON BUFFER_SIZE = 256

PUB main | i, c, buffer[(BUFFER_SIZE+3)/4]

    core.init(RX_PIN, TX_PIN)
    ' core.clearAll

    repeat

        i := 0
        c := 0

        bytefill(@buffer, 0, BUFFER_SIZE)

        repeat

            if (i => constant(BUFFER_SIZE - 1)) or (c == NEW_LINE)
                quit

            core.idleTask

            if core.available
                buffer.byte[i++] := c := core.read

        core.writeString(string("echo -> "))
        core.writeString(@buffer)

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
