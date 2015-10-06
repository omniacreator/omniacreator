''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' @file
' Interface Library Oscilloscope
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

    #(core#OSCILLOSCOPE_BASE * core#BASE_SIZE)

    OSCILLOSCOPE_NEW_UNDOCKED_WINDOW
    OSCILLOSCOPE_NEW_UNDOCKED_WINDOW_SPACER_0 ' spacer
    OSCILLOSCOPE_NEW_UNDOCKED_WINDOW_SPACER_1 ' spacer
    OSCILLOSCOPE_NEW_UNDOCKED_WINDOW_SPACER_2 ' spacer
    OSCILLOSCOPE_NEW_DOCKED_WINDOW
    OSCILLOSCOPE_NEW_DOCKED_WINDOW_SPACER_0 ' spacer
    OSCILLOSCOPE_NEW_DOCKED_WINDOW_SPACER_1 ' spacer
    OSCILLOSCOPE_NEW_DOCKED_WINDOW_SPACER_2 ' spacer
    OSCILLOSCOPE_NEW_MAIN_DOCKED_WINDOW
    OSCILLOSCOPE_NEW_MAIN_DOCKED_WINDOW_SPACER_0 ' spacer
    OSCILLOSCOPE_NEW_MAIN_DOCKED_WINDOW_SPACER_1 ' spacer
    OSCILLOSCOPE_NEW_MAIN_DOCKED_WINDOW_SPACER_2 ' spacer
    OSCILLOSCOPE_REMOVE_DATA
    OSCILLOSCOPE_REMOVE_DATAS
    OSCILLOSCOPE_REMOVE_DATA_SPACER_0 ' spacer
    OSCILLOSCOPE_REMOVE_DATA_SPACER_1 ' spacer
    OSCILLOSCOPE_REMOVE_DATA_SPACER_2 ' spacer
    OSCILLOSCOPE_REMOVE_DATA_SPACER_3 ' spacer
    OSCILLOSCOPE_REMOVE_DATA_SPACER_4 ' spacer
    OSCILLOSCOPE_REMOVE_DATA_SPACER_5 ' spacer
    OSCILLOSCOPE_DELETE_PLOT
    OSCILLOSCOPE_DELETE_PLOTS
    OSCILLOSCOPE_DELETE_PLOT_SPACER_0 ' spacer
    OSCILLOSCOPE_DELETE_PLOT_SPACER_1 ' spacer
    OSCILLOSCOPE_DELETE_PLOT_SPACER_2 ' spacer
    OSCILLOSCOPE_DELETE_PLOT_SPACER_3 ' spacer
    OSCILLOSCOPE_DELETE_PLOT_SPACER_4 ' spacer
    OSCILLOSCOPE_DELETE_PLOT_SPACER_5 ' spacer
    OSCILLOSCOPE_SET_BACKGROUND_COLOR
    OSCILLOSCOPE_GET_BACKGROUND_COLOR

    #(core#OSCILLOSCOPE_XAXIS_BASE * core#BASE_SIZE)

    OSCILLOSCOPE_SET_XAXIS_SCALE_TYPE
    OSCILLOSCOPE_GET_XAXIS_SCALE_TYPE
    OSCILLOSCOPE_SET_XAXIS_SCALE_LOG_BASE
    OSCILLOSCOPE_GET_XAXIS_SCALE_LOG_BASE
    OSCILLOSCOPE_SET_XAXIS_SCALE_LOG_BASE_F
    OSCILLOSCOPE_GET_XAXIS_SCALE_LOG_BASE_F
    OSCILLOSCOPE_SET_XAXIS_RANGE_AUTO_SCALE
    OSCILLOSCOPE_GET_XAXIS_RANGE_AUTO_SCALE
    OSCILLOSCOPE_SET_XAXIS_RANGE_LOWER
    OSCILLOSCOPE_GET_XAXIS_RANGE_LOWER
    OSCILLOSCOPE_SET_XAXIS_RANGE_LOWER_F
    OSCILLOSCOPE_GET_XAXIS_RANGE_LOWER_F
    OSCILLOSCOPE_SET_XAXIS_RANGE_UPPER
    OSCILLOSCOPE_GET_XAXIS_RANGE_UPPER
    OSCILLOSCOPE_SET_XAXIS_RANGE_UPPER_F
    OSCILLOSCOPE_GET_XAXIS_RANGE_UPPER_F
    OSCILLOSCOPE_SET_XAXIS_RANGE_REVERSED
    OSCILLOSCOPE_GET_XAXIS_RANGE_REVERSED
    OSCILLOSCOPE_SET_XAXIS_LABEL
    OSCILLOSCOPE_GET_XAXIS_LABEL

    #(core#OSCILLOSCOPE_YAXIS_BASE * core#BASE_SIZE)

    OSCILLOSCOPE_SET_YAXIS_SCALE_TYPE
    OSCILLOSCOPE_GET_YAXIS_SCALE_TYPE
    OSCILLOSCOPE_SET_YAXIS_SCALE_LOG_BASE
    OSCILLOSCOPE_GET_YAXIS_SCALE_LOG_BASE
    OSCILLOSCOPE_SET_YAXIS_SCALE_LOG_BASE_F
    OSCILLOSCOPE_GET_YAXIS_SCALE_LOG_BASE_F
    OSCILLOSCOPE_SET_YAXIS_RANGE_AUTO_SCALE
    OSCILLOSCOPE_GET_YAXIS_RANGE_AUTO_SCALE
    OSCILLOSCOPE_SET_YAXIS_RANGE_LOWER
    OSCILLOSCOPE_GET_YAXIS_RANGE_LOWER
    OSCILLOSCOPE_SET_YAXIS_RANGE_LOWER_F
    OSCILLOSCOPE_GET_YAXIS_RANGE_LOWER_F
    OSCILLOSCOPE_SET_YAXIS_RANGE_UPPER
    OSCILLOSCOPE_GET_YAXIS_RANGE_UPPER
    OSCILLOSCOPE_SET_YAXIS_RANGE_UPPER_F
    OSCILLOSCOPE_GET_YAXIS_RANGE_UPPER_F
    OSCILLOSCOPE_SET_YAXIS_RANGE_REVERSED
    OSCILLOSCOPE_GET_YAXIS_RANGE_REVERSED
    OSCILLOSCOPE_SET_YAXIS_LABEL
    OSCILLOSCOPE_GET_YAXIS_LABEL

    #(core#OSCILLOSCOPE_FFTXAXIS_BASE * core#BASE_SIZE)

    OSCILLOSCOPE_SET_FFTXAXIS_SCALE_TYPE
    OSCILLOSCOPE_GET_FFTXAXIS_SCALE_TYPE
    OSCILLOSCOPE_SET_FFTXAXIS_SCALE_LOG_BASE
    OSCILLOSCOPE_GET_FFTXAXIS_SCALE_LOG_BASE
    OSCILLOSCOPE_SET_FFTXAXIS_SCALE_LOG_BASE_F
    OSCILLOSCOPE_GET_FFTXAXIS_SCALE_LOG_BASE_F
    OSCILLOSCOPE_SET_FFTXAXIS_RANGE_AUTO_SCALE
    OSCILLOSCOPE_GET_FFTXAXIS_RANGE_AUTO_SCALE
    OSCILLOSCOPE_SET_FFTXAXIS_RANGE_LOWER
    OSCILLOSCOPE_GET_FFTXAXIS_RANGE_LOWER
    OSCILLOSCOPE_SET_FFTXAXIS_RANGE_LOWER_F
    OSCILLOSCOPE_GET_FFTXAXIS_RANGE_LOWER_F
    OSCILLOSCOPE_SET_FFTXAXIS_RANGE_UPPER
    OSCILLOSCOPE_GET_FFTXAXIS_RANGE_UPPER
    OSCILLOSCOPE_SET_FFTXAXIS_RANGE_UPPER_F
    OSCILLOSCOPE_GET_FFTXAXIS_RANGE_UPPER_F
    OSCILLOSCOPE_SET_FFTXAXIS_RANGE_REVERSED
    OSCILLOSCOPE_GET_FFTXAXIS_RANGE_REVERSED
    OSCILLOSCOPE_SET_FFTXAXIS_LABEL
    OSCILLOSCOPE_GET_FFTXAXIS_LABEL

    #(core#OSCILLOSCOPE_FFTYAXIS_BASE * core#BASE_SIZE)

    OSCILLOSCOPE_SET_FFTYAXIS_SCALE_TYPE
    OSCILLOSCOPE_GET_FFTYAXIS_SCALE_TYPE
    OSCILLOSCOPE_SET_FFTYAXIS_SCALE_LOG_BASE
    OSCILLOSCOPE_GET_FFTYAXIS_SCALE_LOG_BASE
    OSCILLOSCOPE_SET_FFTYAXIS_SCALE_LOG_BASE_F
    OSCILLOSCOPE_GET_FFTYAXIS_SCALE_LOG_BASE_F
    OSCILLOSCOPE_SET_FFTYAXIS_RANGE_AUTO_SCALE
    OSCILLOSCOPE_GET_FFTYAXIS_RANGE_AUTO_SCALE
    OSCILLOSCOPE_SET_FFTYAXIS_RANGE_LOWER
    OSCILLOSCOPE_GET_FFTYAXIS_RANGE_LOWER
    OSCILLOSCOPE_SET_FFTYAXIS_RANGE_LOWER_F
    OSCILLOSCOPE_GET_FFTYAXIS_RANGE_LOWER_F
    OSCILLOSCOPE_SET_FFTYAXIS_RANGE_UPPER
    OSCILLOSCOPE_GET_FFTYAXIS_RANGE_UPPER
    OSCILLOSCOPE_SET_FFTYAXIS_RANGE_UPPER_F
    OSCILLOSCOPE_GET_FFTYAXIS_RANGE_UPPER_F
    OSCILLOSCOPE_SET_FFTYAXIS_RANGE_REVERSED
    OSCILLOSCOPE_GET_FFTYAXIS_RANGE_REVERSED
    OSCILLOSCOPE_SET_FFTYAXIS_LABEL
    OSCILLOSCOPE_GET_FFTYAXIS_LABEL

PUB initUndocked(name)
    core.send1Handle1String(OSCILLOSCOPE_NEW_UNDOCKED_WINDOW, {
    } m_thisHandle, name)

PUB initDocked(name)
    init(name)

PUB init(name)
    core.send1Handle1String(OSCILLOSCOPE_NEW_MAIN_DOCKED_WINDOW, {
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

PUB removeData(plotHandle)
    core.send1Handle1Byte(OSCILLOSCOPE_REMOVE_DATA, {
    } m_thisHandle, plotHandle)

PUB removeDatas
    core.send1Handle(OSCILLOSCOPE_REMOVE_DATAS, {
    } m_thisHandle)

PUB deinitPlot(plotHandle)
    core.send1Handle1Byte(OSCILLOSCOPE_DELETE_PLOT, {
    } m_thisHandle, plotHandle)

PUB deinitPlots
    core.send1Handle(OSCILLOSCOPE_DELETE_PLOTS, {
    } m_thisHandle)

PUB setBackgroundColor(backgroundColor)
    core.send1Handle1Long(OSCILLOSCOPE_SET_BACKGROUND_COLOR, {
    } m_thisHandle, backgroundColor)

PUB setXAxisScaleType(scaleType)
    core.send1Handle1Long(OSCILLOSCOPE_SET_XAXIS_SCALE_TYPE, {
    } m_thisHandle, scaleType)

PUB setXAxisScaleLogBase(scaleLogBase)
    core.send1Handle1Long(OSCILLOSCOPE_SET_XAXIS_SCALE_LOG_BASE, {
    } m_thisHandle, scaleLogBase)

PUB setXAxisScaleLogBaseF(scaleLogBaseF)
    core.send1Handle1Long(OSCILLOSCOPE_SET_XAXIS_SCALE_LOG_BASE_F, {
    } m_thisHandle, scaleLogBaseF)

PUB setXAxisRangeAutoScale(on)
    core.send1Handle1Long(OSCILLOSCOPE_SET_XAXIS_RANGE_AUTO_SCALE, {
    } m_thisHandle, on)

PUB setXAxisRangeLower(rangeLower)
    core.send1Handle1Long(OSCILLOSCOPE_SET_XAXIS_RANGE_LOWER, {
    } m_thisHandle, rangeLower)

PUB setXAxisRangeLowerF(rangeLowerF)
    core.send1Handle1Long(OSCILLOSCOPE_SET_XAXIS_RANGE_LOWER_F, {
    } m_thisHandle, rangeLowerF)

PUB setXAxisRangeUpper(rangeUpper)
    core.send1Handle1Long(OSCILLOSCOPE_SET_XAXIS_RANGE_UPPER, {
    } m_thisHandle, rangeUpper)

PUB setXAxisRangeUpperF(rangeUpperF)
    core.send1Handle1Long(OSCILLOSCOPE_SET_XAXIS_RANGE_UPPER_F, {
    } m_thisHandle, rangeUpperF)

PUB setXAxisRangeReversed(rangeReversed)
    core.send1Handle1Long(OSCILLOSCOPE_SET_XAXIS_RANGE_REVERSED, {
    } m_thisHandle, rangeReversed)

PUB setXAxisLabel(label)
    core.send1Handle1String(OSCILLOSCOPE_SET_XAXIS_LABEL, {
    } m_thisHandle, label)

PUB setYAxisScaleType(scaleType)
    core.send1Handle1Long(OSCILLOSCOPE_SET_YAXIS_SCALE_TYPE, {
    } m_thisHandle, scaleType)

PUB setYAxisScaleLogBase(scaleLogBase)
    core.send1Handle1Long(OSCILLOSCOPE_SET_YAXIS_SCALE_LOG_BASE, {
    } m_thisHandle, scaleLogBase)

PUB setYAxisScaleLogBaseF(scaleLogBaseF)
    core.send1Handle1Long(OSCILLOSCOPE_SET_YAXIS_SCALE_LOG_BASE_F, {
    } m_thisHandle, scaleLogBaseF)

PUB setYAxisRangeAutoScale(on)
    core.send1Handle1Long(OSCILLOSCOPE_SET_YAXIS_RANGE_AUTO_SCALE, {
    } m_thisHandle, on)

PUB setYAxisRangeLower(rangeLower)
    core.send1Handle1Long(OSCILLOSCOPE_SET_YAXIS_RANGE_LOWER, {
    } m_thisHandle, rangeLower)

PUB setYAxisRangeLowerF(rangeLowerF)
    core.send1Handle1Long(OSCILLOSCOPE_SET_YAXIS_RANGE_LOWER_F, {
    } m_thisHandle, rangeLowerF)

PUB setYAxisRangeUpper(rangeUpper)
    core.send1Handle1Long(OSCILLOSCOPE_SET_YAXIS_RANGE_UPPER, {
    } m_thisHandle, rangeUpper)

PUB setYAxisRangeUpperF(rangeUpperF)
    core.send1Handle1Long(OSCILLOSCOPE_SET_YAXIS_RANGE_UPPER_F, {
    } m_thisHandle, rangeUpperF)

PUB setYAxisRangeReversed(rangeReversed)
    core.send1Handle1Long(OSCILLOSCOPE_SET_YAXIS_RANGE_REVERSED, {
    } m_thisHandle, rangeReversed)

PUB setYAxisLabel(label)
    core.send1Handle1String(OSCILLOSCOPE_SET_YAXIS_LABEL, {
    } m_thisHandle, label)

PUB setFFTXAxisScaleType(scaleType)
    core.send1Handle1Long(OSCILLOSCOPE_SET_FFTXAXIS_SCALE_TYPE, {
    } m_thisHandle, scaleType)

PUB setFFTXAxisScaleLogBase(scaleLogBase)
    core.send1Handle1Long(OSCILLOSCOPE_SET_FFTXAXIS_SCALE_LOG_BASE, {
    } m_thisHandle, scaleLogBase)

PUB setFFTXAxisScaleLogBaseF(scaleLogBaseF)
    core.send1Handle1Long(OSCILLOSCOPE_SET_FFTXAXIS_SCALE_LOG_BASE_F, {
    } m_thisHandle, scaleLogBaseF)

PUB setFFTXAxisRangeAutoScale(on)
    core.send1Handle1Long(OSCILLOSCOPE_SET_FFTXAXIS_RANGE_AUTO_SCALE, {
    } m_thisHandle, on)

PUB setFFTXAxisRangeLower(rangeLower)
    core.send1Handle1Long(OSCILLOSCOPE_SET_FFTXAXIS_RANGE_LOWER, {
    } m_thisHandle, rangeLower)

PUB setFFTXAxisRangeLowerF(rangeLowerF)
    core.send1Handle1Long(OSCILLOSCOPE_SET_FFTXAXIS_RANGE_LOWER_F, {
    } m_thisHandle, rangeLowerF)

PUB setFFTXAxisRangeUpper(rangeUpper)
    core.send1Handle1Long(OSCILLOSCOPE_SET_FFTXAXIS_RANGE_UPPER, {
    } m_thisHandle, rangeUpper)

PUB setFFTXAxisRangeUpperF(rangeUpperF)
    core.send1Handle1Long(OSCILLOSCOPE_SET_FFTXAXIS_RANGE_UPPER_F, {
    } m_thisHandle, rangeUpperF)

PUB setFFTXAxisRangeReversed(rangeReversed)
    core.send1Handle1Long(OSCILLOSCOPE_SET_FFTXAXIS_RANGE_REVERSED, {
    } m_thisHandle, rangeReversed)

PUB setFFTXAxisLabel(label)
    core.send1Handle1String(OSCILLOSCOPE_SET_FFTXAXIS_LABEL, {
    } m_thisHandle, label)

PUB setFFTYAxisScaleType(scaleType)
    core.send1Handle1Long(OSCILLOSCOPE_SET_FFTYAXIS_SCALE_TYPE, {
    } m_thisHandle, scaleType)

PUB setFFTYAxisScaleLogBase(scaleLogBase)
    core.send1Handle1Long(OSCILLOSCOPE_SET_FFTYAXIS_SCALE_LOG_BASE, {
    } m_thisHandle, scaleLogBase)

PUB setFFTYAxisScaleLogBaseF(scaleLogBaseF)
    core.send1Handle1Long(OSCILLOSCOPE_SET_FFTYAXIS_SCALE_LOG_BASE_F, {
    } m_thisHandle, scaleLogBaseF)

PUB setFFTYAxisRangeAutoScale(on)
    core.send1Handle1Long(OSCILLOSCOPE_SET_FFTYAXIS_RANGE_AUTO_SCALE, {
    } m_thisHandle, on)

PUB setFFTYAxisRangeLower(rangeLower)
    core.send1Handle1Long(OSCILLOSCOPE_SET_FFTYAXIS_RANGE_LOWER, {
    } m_thisHandle, rangeLower)

PUB setFFTYAxisRangeLowerF(rangeLowerF)
    core.send1Handle1Long(OSCILLOSCOPE_SET_FFTYAXIS_RANGE_LOWER_F, {
    } m_thisHandle, rangeLowerF)

PUB setFFTYAxisRangeUpper(rangeUpper)
    core.send1Handle1Long(OSCILLOSCOPE_SET_FFTYAXIS_RANGE_UPPER, {
    } m_thisHandle, rangeUpper)

PUB setFFTYAxisRangeUpperF(rangeUpperF)
    core.send1Handle1Long(OSCILLOSCOPE_SET_FFTYAXIS_RANGE_UPPER_F, {
    } m_thisHandle, rangeUpperF)

PUB setFFTYAxisRangeReversed(rangeReversed)
    core.send1Handle1Long(OSCILLOSCOPE_SET_FFTYAXIS_RANGE_REVERSED, {
    } m_thisHandle, rangeReversed)

PUB setFFTYAxisLabel(label)
    core.send1Handle1String(OSCILLOSCOPE_SET_FFTYAXIS_LABEL, {
    } m_thisHandle, label)

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
