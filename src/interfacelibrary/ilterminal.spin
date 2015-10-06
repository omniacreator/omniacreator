''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' @file
' Interface Library Terminal
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

OBJ core: "ilcore.spin"

VAR long m_parentHandle, m_thisHandle

PUB setParentHandle(parentHandle)
    m_parentHandle := parentHandle

PUB setThisHandle(thisHandle)
    m_thisHandle := thisHandle

CON ' SerialFunction

    #(core#WINDOW_BASE * core#BASE_SIZE)

    WINDOW_CLOSE_WINDOW
    WINDOW_CLOSE_WINDOWS
    WINDOW_CLOSE_WINDOW_SPACER_0 ' spacer
    WINDOW_CLOSE_WINDOW_SPACER_1 ' spacer
    WINDOW_CLOSE_WINDOW_SPACER_2 ' spacer
    WINDOW_CLOSE_WINDOW_SPACER_3 ' spacer
    WINDOW_CLOSE_WINDOW_SPACER_4 ' spacer
    WINDOW_CLOSE_WINDOW_SPACER_5 ' spacer
    WINDOW_DELETE_WINDOW
    WINDOW_DELETE_WINDOWS
    WINDOW_DELETE_WINDOW_SPACER_0 ' spacer
    WINDOW_DELETE_WINDOW_SPACER_1 ' spacer
    WINDOW_DELETE_WINDOW_SPACER_2 ' spacer
    WINDOW_DELETE_WINDOW_SPACER_3 ' spacer
    WINDOW_DELETE_WINDOW_SPACER_4 ' spacer
    WINDOW_DELETE_WINDOW_SPACER_5 ' spacer
    WINDOW_SET_WINDOW_TITLE
    WINDOW_GET_WINDOW_TITLE
    WINDOW_SET_WINDOW_MESSAGE
    WINDOW_GET_WINDOW_MESSAGE
    WINDOW_SET_WINDOW_VISIBLE
    WINDOW_GET_WINDOW_VISIBLE
    WINDOW_SET_WINDOW_MINIMIZED
    WINDOW_GET_WINDOW_MINIMIZED
    WINDOW_SET_WINDOW_MAXIMIZED
    WINDOW_GET_WINDOW_MAXIMIZED
    WINDOW_SET_WINDOW_FULL_SCREEN
    WINDOW_GET_WINDOW_FULL_SCREEN

    #(core#TERMINAL_BASE * core#BASE_SIZE)

    TERMINAL_NEW_UNDOCKED_WINDOW
    TERMINAL_NEW_UNDOCKED_WINDOW_SPACER_0 ' spacer
    TERMINAL_NEW_UNDOCKED_WINDOW_SPACER_1 ' spacer
    TERMINAL_NEW_UNDOCKED_WINDOW_SPACER_2 ' spacer
    TERMINAL_NEW_DOCKED_WINDOW
    TERMINAL_NEW_DOCKED_WINDOW_SPACER_0 ' spacer
    TERMINAL_NEW_DOCKED_WINDOW_SPACER_1 ' spacer
    TERMINAL_NEW_DOCKED_WINDOW_SPACER_2 ' spacer
    TERMINAL_NEW_MAIN_DOCKED_WINDOW
    TERMINAL_NEW_MAIN_DOCKED_WINDOW_SPACER_0 ' spacer
    TERMINAL_NEW_MAIN_DOCKED_WINDOW_SPACER_1 ' spacer
    TERMINAL_NEW_MAIN_DOCKED_WINDOW_SPACER_2 ' spacer
    TERMINAL_PRINT
    TERMINAL_PRINTLN

PUB initUndocked(name)
    core.send1Handle1String(TERMINAL_NEW_UNDOCKED_WINDOW, {
    } m_thisHandle, name)

PUB initDocked(name)
    init(name)

PUB init(name)
    core.send1Handle1String(TERMINAL_NEW_MAIN_DOCKED_WINDOW, {
    } m_thisHandle, name)

PUB setTitle(title)
    core.send1Handle1String(WINDOW_SET_WINDOW_TITLE, {
    } m_thisHandle, title)

PUB setMessage(message)
    core.send1Handle1String(WINDOW_SET_WINDOW_MESSAGE, {
    } m_thisHandle, message)

PUB setVisibility(visibility)
    core.send1Handle1Long(WINDOW_SET_WINDOW_VISIBLE, {
    } m_thisHandle, visibility)

PUB setMinimized(minimized)
    core.send1Handle1Long(WINDOW_SET_WINDOW_MINIMIZED, {
    } m_thisHandle, minimized)

PUB setMaximized(maximized)
    core.send1Handle1Long(WINDOW_SET_WINDOW_MAXIMIZED, {
    } m_thisHandle, maximized)

PUB setFullscreen(fullscreen)
    core.send1Handle1Long(WINDOW_SET_WINDOW_FULL_SCREEN, {
    } m_thisHandle, fullscreen)

PUB writeChar(c)
    core.send1Handle1Byte(TERMINAL_PRINT, {
    } m_thisHandle, c)

PUB writeString(s)
    core.send1Handle1String(TERMINAL_PRINT, {
    } m_thisHandle, s)

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
