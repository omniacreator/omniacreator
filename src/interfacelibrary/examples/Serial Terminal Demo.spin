''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' @file
' Interface Library Terminal Serial Demo
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

CON NUM_TERMINALS = 4
OBJ terminals[NUM_TERMINALS]: "ilterminal.spin"

CON ' Terminal Window Handles

    TERMINAL_0
    TERMINAL_1
    TERMINAL_2
    TERMINAL_3

OBJ n: "Simple_Numbers.spin"

PUB main | i

    core.init(RX_PIN, TX_PIN)
    ' core.clearAll

    terminals[TERMINAL_0].setParentHandle(0)
    terminals[TERMINAL_0].setThisHandle(TERMINAL_0)
    terminals[TERMINAL_0].init(string("Terminal 0"))

    terminals[TERMINAL_1].setParentHandle(0)
    terminals[TERMINAL_1].setThisHandle(TERMINAL_1)
    terminals[TERMINAL_1].init(string("Terminal 1"))

    terminals[TERMINAL_2].setParentHandle(0)
    terminals[TERMINAL_2].setThisHandle(TERMINAL_2)
    terminals[TERMINAL_2].init(string("Terminal 2"))

    terminals[TERMINAL_3].setParentHandle(0)
    terminals[TERMINAL_3].setThisHandle(TERMINAL_3)
    terminals[TERMINAL_3].init(string("Terminal 3"))

    i := 0

    repeat

        terminals[TERMINAL_0].writeString(string("Hello World!", NEW_LINE))

        case (i + 0) // 8
          0: terminals[TERMINAL_0].writeChar(core#CC_SET_FOREGROUND_TO_GRAY)
          1: terminals[TERMINAL_0].writeChar(core#CC_SET_FOREGROUND_TO_BLUE)
          2: terminals[TERMINAL_0].writeChar(core#CC_SET_FOREGROUND_TO_RED)
          3: terminals[TERMINAL_0].writeChar(core#CC_SET_FOREGROUND_TO_GREEN)
          4: terminals[TERMINAL_0].writeChar(core#CC_SET_FOREGROUND_TO_MAGENTA)
          5: terminals[TERMINAL_0].writeChar(core#CC_SET_FOREGROUND_TO_CYAN)
          6: terminals[TERMINAL_0].writeChar(core#CC_SET_FOREGROUND_TO_YELLOW)
          7: terminals[TERMINAL_0].writeChar(core#CC_SET_FOREGROUND_TO_BLACK)

        terminals[TERMINAL_1].writeString(string("Hello World!", NEW_LINE))

        case (i + 2) // 8
          0: terminals[TERMINAL_1].writeChar(core#CC_SET_FOREGROUND_TO_GRAY)
          1: terminals[TERMINAL_1].writeChar(core#CC_SET_FOREGROUND_TO_BLUE)
          2: terminals[TERMINAL_1].writeChar(core#CC_SET_FOREGROUND_TO_RED)
          3: terminals[TERMINAL_1].writeChar(core#CC_SET_FOREGROUND_TO_GREEN)
          4: terminals[TERMINAL_1].writeChar(core#CC_SET_FOREGROUND_TO_MAGENTA)
          5: terminals[TERMINAL_1].writeChar(core#CC_SET_FOREGROUND_TO_CYAN)
          6: terminals[TERMINAL_1].writeChar(core#CC_SET_FOREGROUND_TO_YELLOW)
          7: terminals[TERMINAL_1].writeChar(core#CC_SET_FOREGROUND_TO_BLACK)

        terminals[TERMINAL_2].writeString(string("Hello World!", NEW_LINE))

        case (i + 4) // 8
          0: terminals[TERMINAL_2].writeChar(core#CC_SET_FOREGROUND_TO_GRAY)
          1: terminals[TERMINAL_2].writeChar(core#CC_SET_FOREGROUND_TO_BLUE)
          2: terminals[TERMINAL_2].writeChar(core#CC_SET_FOREGROUND_TO_RED)
          3: terminals[TERMINAL_2].writeChar(core#CC_SET_FOREGROUND_TO_GREEN)
          4: terminals[TERMINAL_2].writeChar(core#CC_SET_FOREGROUND_TO_MAGENTA)
          5: terminals[TERMINAL_2].writeChar(core#CC_SET_FOREGROUND_TO_CYAN)
          6: terminals[TERMINAL_2].writeChar(core#CC_SET_FOREGROUND_TO_YELLOW)
          7: terminals[TERMINAL_2].writeChar(core#CC_SET_FOREGROUND_TO_BLACK)

        terminals[TERMINAL_3].writeString(string("Hello World!", NEW_LINE))

        case (i + 6) // 8
          0: terminals[TERMINAL_3].writeChar(core#CC_SET_FOREGROUND_TO_GRAY)
          1: terminals[TERMINAL_3].writeChar(core#CC_SET_FOREGROUND_TO_BLUE)
          2: terminals[TERMINAL_3].writeChar(core#CC_SET_FOREGROUND_TO_RED)
          3: terminals[TERMINAL_3].writeChar(core#CC_SET_FOREGROUND_TO_GREEN)
          4: terminals[TERMINAL_3].writeChar(core#CC_SET_FOREGROUND_TO_MAGENTA)
          5: terminals[TERMINAL_3].writeChar(core#CC_SET_FOREGROUND_TO_CYAN)
          6: terminals[TERMINAL_3].writeChar(core#CC_SET_FOREGROUND_TO_YELLOW)
          7: terminals[TERMINAL_3].writeChar(core#CC_SET_FOREGROUND_TO_BLACK)

        ' default terminal

        core.writeString(string("i = "))
        core.writeString(n.dec(i))
        core.writeChar(NEW_LINE)

        i += 1

        waitcnt(clkfreq + cnt)

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
