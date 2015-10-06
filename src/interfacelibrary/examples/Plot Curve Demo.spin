''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' @file
' Interface Library Curve Plot Demo
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

CON NUM_OSCOPES = 6
OBJ oscopes[NUM_OSCOPES]: "iloscilloscope.spin"

CON ' Oscilloscope Window Handles

    BUTTERFLY
    LISSAJOUS
    EPICYCLOID
    FERMATS
    NCURVE
    CIRCLE

CON NUM_CURVES = 9
OBJ curves[NUM_CURVES]: "iloscilloscopecurve.spin"

CON ' Oscilloscope Curve Handles

    BUTTERFLY_0
    LISSAJOUS_0
    LISSAJOUS_1
    EPICYCLOID_0
    FERMATS_0
    FERMATS_1
    NCURVE_0
    CIRCLE_0
    CIRCLE_1

OBJ f: "Float32Full.spin"

PUB main | i

    core.init(RX_PIN, TX_PIN)
    ' core.clearAll

    oscopes[BUTTERFLY].setParentHandle(0)
    oscopes[BUTTERFLY].setThisHandle(BUTTERFLY)
    oscopes[BUTTERFLY].init(string("Butterfly Curve"))
    oscopes[BUTTERFLY].removeDatas

    curves[BUTTERFLY_0].setParentHandle(BUTTERFLY)
    curves[BUTTERFLY_0].setThisHandle(BUTTERFLY_0)
    curves[BUTTERFLY_0].init(string("Butterfly Function"))
    curves[BUTTERFLY_0].setLineColor($FF7F007F)
    curves[BUTTERFLY_0].setLineStyle(core#LS_SOLID_LINE)
    curves[BUTTERFLY_0].setFillColor($7F7F007F)
    curves[BUTTERFLY_0].setFillStyle(core#FS_SOLID_PATTERN)
    curves[BUTTERFLY_0].setSize(4096)

    oscopes[LISSAJOUS].setParentHandle(0)
    oscopes[LISSAJOUS].setThisHandle(LISSAJOUS)
    oscopes[LISSAJOUS].init(string("Lissajous Curves"))
    oscopes[LISSAJOUS].removeDatas

    curves[LISSAJOUS_0].setParentHandle(LISSAJOUS)
    curves[LISSAJOUS_0].setThisHandle(LISSAJOUS_0)
    curves[LISSAJOUS_0].init(string("Lissajous Curve 1"))
    curves[LISSAJOUS_0].setLineColor($FFFF7F00)
    curves[LISSAJOUS_0].setLineStyle(core#LS_SOLID_LINE)
    curves[LISSAJOUS_0].setFillColor($7FFF7FC8)
    curves[LISSAJOUS_0].setFillStyle(core#FS_SOLID_PATTERN)
    curves[LISSAJOUS_0].setSize(512)

    curves[LISSAJOUS_1].setParentHandle(LISSAJOUS)
    curves[LISSAJOUS_1].setThisHandle(LISSAJOUS_1)
    curves[LISSAJOUS_1].init(string("Lissajous Curve 2"))
    curves[LISSAJOUS_1].setLineColor($FFFFFF00)
    curves[LISSAJOUS_1].setLineStyle(core#LS_SOLID_LINE)
    curves[LISSAJOUS_1].setFillColor($7FFFFFC8)
    curves[LISSAJOUS_1].setFillStyle(core#FS_SOLID_PATTERN)
    curves[LISSAJOUS_1].setSize(512)

    oscopes[EPICYCLOID].setParentHandle(0)
    oscopes[EPICYCLOID].setThisHandle(EPICYCLOID)
    oscopes[EPICYCLOID].init(string("Epicycloid Curve"))
    oscopes[EPICYCLOID].removeDatas

    curves[EPICYCLOID_0].setParentHandle(EPICYCLOID)
    curves[EPICYCLOID_0].setThisHandle(EPICYCLOID_0)
    curves[EPICYCLOID_0].init(string("Epicycloid Function"))
    curves[EPICYCLOID_0].setLineColor($FF007F7F)
    curves[EPICYCLOID_0].setLineStyle(core#LS_SOLID_LINE)
    curves[EPICYCLOID_0].setFillColor($7F007F7F)
    curves[EPICYCLOID_0].setFillStyle(core#FS_SOLID_PATTERN)
    curves[EPICYCLOID_0].setSize(4096)

    oscopes[FERMATS].setParentHandle(0)
    oscopes[FERMATS].setThisHandle(FERMATS)
    oscopes[FERMATS].init(string("Fermat's Spiral"))
    oscopes[FERMATS].removeDatas

    curves[FERMATS_0].setParentHandle(FERMATS)
    curves[FERMATS_0].setThisHandle(FERMATS_0)
    curves[FERMATS_0].init(string("Fermat's Spiral 1"))
    curves[FERMATS_0].setLineColor($FF0000FF)
    curves[FERMATS_0].setLineStyle(core#LS_SOLID_LINE)
    curves[FERMATS_0].setFillColor($7FC8C8FF)
    curves[FERMATS_0].setFillStyle(core#FS_SOLID_PATTERN)
    curves[FERMATS_0].setSize(4096)

    curves[FERMATS_1].setParentHandle(FERMATS)
    curves[FERMATS_1].setThisHandle(FERMATS_1)
    curves[FERMATS_1].init(string("Fermat's Spiral 2"))
    curves[FERMATS_1].setLineColor($FFFF0000)
    curves[FERMATS_1].setLineStyle(core#LS_SOLID_LINE)
    curves[FERMATS_1].setFillColor($7FFFC8C8)
    curves[FERMATS_1].setFillStyle(core#FS_SOLID_PATTERN)
    curves[FERMATS_1].setSize(4096)

    oscopes[NCURVE].setParentHandle(0)
    oscopes[NCURVE].setThisHandle(NCURVE)
    oscopes[NCURVE].init(string("N-Curve Curve"))
    oscopes[NCURVE].removeDatas

    curves[NCURVE_0].setParentHandle(NCURVE)
    curves[NCURVE_0].setThisHandle(NCURVE_0)
    curves[NCURVE_0].init(string("N-Curve Function"))
    curves[NCURVE_0].setLineColor($FF007F00)
    curves[NCURVE_0].setLineStyle(core#LS_SOLID_LINE)
    curves[NCURVE_0].setFillColor($7F007F00)
    curves[NCURVE_0].setFillStyle(core#FS_SOLID_PATTERN)
    curves[NCURVE_0].setSize(4096)

    oscopes[CIRCLE].setParentHandle(0)
    oscopes[CIRCLE].setThisHandle(CIRCLE)
    oscopes[CIRCLE].init(string("Circles Curve"))
    oscopes[CIRCLE].removeDatas

    curves[CIRCLE_0].setParentHandle(CIRCLE)
    curves[CIRCLE_0].setThisHandle(CIRCLE_0)
    curves[CIRCLE_0].init(string("Circle Curve 1"))
    curves[CIRCLE_0].setLineColor($FFFFA500)
    curves[CIRCLE_0].setLineStyle(core#LS_SOLID_LINE)
    curves[CIRCLE_0].setFillColor($00000000)
    curves[CIRCLE_0].setFillStyle(core#FS_NO_BRUSH)
    curves[CIRCLE_0].setSize(128)

    curves[CIRCLE_1].setParentHandle(CIRCLE)
    curves[CIRCLE_1].setThisHandle(CIRCLE_1)
    curves[CIRCLE_1].init(string("Circle Curve 2"))
    curves[CIRCLE_1].setLineColor($FFFFA500)
    curves[CIRCLE_1].setLineStyle(core#LS_SOLID_LINE)
    curves[CIRCLE_1].setFillColor($00000000)
    curves[CIRCLE_1].setFillStyle(core#FS_NO_BRUSH)
    curves[CIRCLE_1].setSize(128)

    f.start
    i := 0.0

    repeat

        curves[BUTTERFLY_0].addKeyValueF({
        } f.fmul(f.sin(f.radians(i)), f.fsub(f.fsub({
        } f.exp(f.cos(f.radians(i))), {
        } f.fmul(2.0, f.cos(f.radians(f.fmul(4.0, i))))), {
        } f.pow(f.sin(f.radians(f.fdiv(i, 12.0))), 5.0))), {
        } f.fmul(f.cos(f.radians(i)), f.fsub(f.fsub({
        } f.exp(f.cos(f.radians(i))), {
        } f.fmul(2.0, f.cos(f.radians(f.fmul(4.0, i))))), {
        } f.pow(f.sin(f.radians(f.fdiv(i, 12.0))), 5.0))))

        curves[LISSAJOUS_0].addKeyValueF({
        } f.sin(f.radians(f.fmul(5.0, i))), {
        } f.sin(f.radians(f.fmul(6.0, i))))

        curves[LISSAJOUS_1].addKeyValueF({
        } f.sin(f.radians(f.fmul(6.0, i))), {
        } f.sin(f.radians(f.fmul(5.0, i))))

        curves[EPICYCLOID_0].addKeyValueF({
        } f.fsub(f.fmul(f.fmul(3.0, 3.1), f.cos(f.radians(i))), {
        } f.fmul(3.0, f.cos(f.radians(f.fmul(3.1, i))))), {
        } f.fsub(f.fmul(f.fmul(3.0, 3.1), f.sin(f.radians(i))), {
        } f.fmul(3.0, f.sin(f.radians(f.fmul(3.1, i))))))

        curves[FERMATS_0].addKeyValueF({
        } f.fmul(f.fsqr(i), f.cos(f.radians(i))), {
        } f.fmul(f.fsqr(i), f.sin(f.radians(i))))

        curves[FERMATS_1].addKeyValueF({
        } f.fneg(f.fmul(f.fsqr(i), f.cos(f.radians(i)))), {
        } f.fneg(f.fmul(f.fsqr(i), f.sin(f.radians(i)))))

        curves[NCURVE_0].addKeyValueF({
        } f.fmul(f.fmul(2.0, pi), f.fadd(f.fsub(f.fdiv(i, 4096.0), 1.0), {
        } f.fmul(f.cos(f.fmul(f.fmul(76.0, pi), f.fdiv(i, 4096.0))), {
        } f.cos(f.fmul(f.fmul(38.0, pi), f.fdiv(i, 4096.0)))))), {
        } f.fadd(f.cos(f.fmul(f.fmul(2.0, pi), f.fdiv(i, 4096.0))), {
        } f.fmul(f.fmul(2.0, pi), {
        } f.fmul(f.cos(f.fmul(f.fmul(76.0, pi), f.fdiv(i, 4096.0))), {
        } f.sin(f.fmul(f.fmul(38.0, pi), f.fdiv(i, 4096.0)))))))

        curves[CIRCLE_0].addKeyValueF({
        } f.fsub(f.fmul(f.fmul(3.0, f.sin(i)), f.cos(i)), 1.5), {
        } f.fsub(f.fmul(f.fmul(3.0, f.cos(i)), f.cos(i)), 0.0))

        curves[CIRCLE_1].addKeyValueF({
        } f.fadd(f.fmul(f.fmul(3.0, f.sin(i)), f.cos(i)), 1.5), {
        } f.fadd(f.fmul(f.fmul(3.0, f.cos(i)), f.cos(i)), 0.0))

        i := f.fadd(i, 1.0)

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
