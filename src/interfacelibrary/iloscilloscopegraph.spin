''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' @file
' Interface Library Oscilloscope Graph
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

    #(core#OSCILLOSCOPE_GRAPH_BASE * core#BASE_SIZE)

    OSCILLOSCOPE_NEW_GRAPH
    OSCILLOSCOPE_NEW_GRAPH_SPACER_0 ' spacer
    OSCILLOSCOPE_NEW_GRAPH_SPACER_1 ' spacer
    OSCILLOSCOPE_NEW_GRAPH_SPACER_2 ' spacer
    OSCILLOSCOPE_REMOVE_GRAPH_DATA
    OSCILLOSCOPE_REMOVE_GRAPH_DATA_SPACER_0 ' spacer
    OSCILLOSCOPE_REMOVE_GRAPH_DATA_SPACER_1 ' spacer
    OSCILLOSCOPE_REMOVE_GRAPH_DATA_SPACER_2 ' spacer
    OSCILLOSCOPE_ADD_GRAPH_DATA_0
    OSCILLOSCOPE_ADD_GRAPH_DATA_0_A
    OSCILLOSCOPE_ADD_GRAPH_DATA_0_F
    OSCILLOSCOPE_ADD_GRAPH_DATA_0_F_A
    OSCILLOSCOPE_ADD_GRAPH_DATA_1
    OSCILLOSCOPE_ADD_GRAPH_DATA_1_A
    OSCILLOSCOPE_ADD_GRAPH_DATA_1_F
    OSCILLOSCOPE_ADD_GRAPH_DATA_1_F_A
    OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_0
    OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_0_A
    OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_0_F
    OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_0_F_A
    OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_1
    OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_1_A
    OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_1_F
    OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_1_F_A
    OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_2
    OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_2_A
    OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_2_F
    OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_2_F_A
    OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_3
    OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_3_A
    OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_3_F
    OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_3_F_A
    OSCILLOSCOPE_ADD_GRAPH_DATA_SPACER_0 ' spacer
    OSCILLOSCOPE_ADD_GRAPH_DATA_SPACER_1 ' spacer
    OSCILLOSCOPE_ADD_GRAPH_DATA_SPACER_2 ' spacer
    OSCILLOSCOPE_ADD_GRAPH_DATA_SPACER_3 ' spacer
    OSCILLOSCOPE_ADD_GRAPH_DATA_SPACER_4 ' spacer
    OSCILLOSCOPE_ADD_GRAPH_DATA_SPACER_5 ' spacer
    OSCILLOSCOPE_ADD_GRAPH_DATA_SPACER_6 ' spacer
    OSCILLOSCOPE_ADD_GRAPH_DATA_SPACER_7 ' spacer
    OSCILLOSCOPE_REMOVE_GRAPH_DATA_BEFORE
    OSCILLOSCOPE_REMOVE_GRAPH_DATA_BEFORE_F
    OSCILLOSCOPE_REMOVE_GRAPH_DATA_AFTER
    OSCILLOSCOPE_REMOVE_GRAPH_DATA_AFTER_F
    OSCILLOSCOPE_REMOVE_GRAPH_DATA_BETWEEN
    OSCILLOSCOPE_REMOVE_GRAPH_DATA_BETWEEN_A
    OSCILLOSCOPE_REMOVE_GRAPH_DATA_BETWEEN_F
    OSCILLOSCOPE_REMOVE_GRAPH_DATA_BETWEEN_F_A
    OSCILLOSCOPE_REMOVE_GRAPH_DATUM
    OSCILLOSCOPE_REMOVE_GRAPH_DATUM_A
    OSCILLOSCOPE_REMOVE_GRAPH_DATUM_F
    OSCILLOSCOPE_REMOVE_GRAPH_DATUM_F_A
    OSCILLOSCOPE_REMOVE_GRAPH_DATUM_SPACER_0 ' spacer
    OSCILLOSCOPE_REMOVE_GRAPH_DATUM_SPACER_1 ' spacer
    OSCILLOSCOPE_REMOVE_GRAPH_DATUM_SPACER_2 ' spacer
    OSCILLOSCOPE_REMOVE_GRAPH_DATUM_SPACER_3 ' spacer
    OSCILLOSCOPE_SET_GRAPH_NAME
    OSCILLOSCOPE_GET_GRAPH_NAME
    OSCILLOSCOPE_SET_GRAPH_COORDINATE_SYSTEM
    OSCILLOSCOPE_GET_GRAPH_COORDINATE_SYSTEM
    OSCILLOSCOPE_SET_GRAPH_ANGLE_UNITS
    OSCILLOSCOPE_GET_GRAPH_ANGLE_UNITS
    OSCILLOSCOPE_SET_GRAPH_LINE_COLOR
    OSCILLOSCOPE_GET_GRAPH_LINE_COLOR
    OSCILLOSCOPE_SET_GRAPH_LINE_STYLE
    OSCILLOSCOPE_GET_GRAPH_LINE_STYLE
    OSCILLOSCOPE_SET_GRAPH_FILL_COLOR
    OSCILLOSCOPE_GET_GRAPH_FILL_COLOR
    OSCILLOSCOPE_SET_GRAPH_FILL_STYLE
    OSCILLOSCOPE_GET_GRAPH_FILL_STYLE
    OSCILLOSCOPE_SET_GRAPH_DELTA_MODE
    OSCILLOSCOPE_GET_GRAPH_DELTA_MODE
    OSCILLOSCOPE_SET_GRAPH_SIZE
    OSCILLOSCOPE_GET_GRAPH_SIZE
    OSCILLOSCOPE_SET_GRAPH_SAMPLE_RATE
    OSCILLOSCOPE_GET_GRAPH_SAMPLE_RATE
    OSCILLOSCOPE_SET_GRAPH_SAMPLE_RATE_F
    OSCILLOSCOPE_GET_GRAPH_SAMPLE_RATE_F
    OSCILLOSCOPE_SET_GRAPH_SCALER
    OSCILLOSCOPE_GET_GRAPH_SCALER
    OSCILLOSCOPE_SET_GRAPH_SCALER_F
    OSCILLOSCOPE_GET_GRAPH_SCALER_F
    OSCILLOSCOPE_SET_GRAPH_FFT
    OSCILLOSCOPE_GET_GRAPH_FFT
    OSCILLOSCOPE_SET_GRAPH_FFT_WINDOW
    OSCILLOSCOPE_GET_GRAPH_FFT_WINDOW
    OSCILLOSCOPE_SET_GRAPH_SCATTER_STYLE
    OSCILLOSCOPE_GET_GRAPH_SCATTER_STYLE
    OSCILLOSCOPE_SET_GRAPH_STYLE
    OSCILLOSCOPE_GET_GRAPH_STYLE
    OSCILLOSCOPE_SET_GRAPH_CHANNEL_FILL_GRAPH
    OSCILLOSCOPE_GET_GRAPH_CHANNEL_FILL_GRAPH
    OSCILLOSCOPE_SET_GRAPH_KEY_ERRORS
    OSCILLOSCOPE_GET_GRAPH_KEY_ERRORS
    OSCILLOSCOPE_SET_GRAPH_VALUE_ERRORS
    OSCILLOSCOPE_GET_GRAPH_VALUE_ERRORS

PUB init(name)
    core.send2Handle1String(OSCILLOSCOPE_NEW_GRAPH, {
    } m_parentHandle, m_thisHandle, name)

PUB removeData
    core.send2Handle(OSCILLOSCOPE_REMOVE_GRAPH_DATA, {
    } m_parentHandle, m_thisHandle)

PUB addKeyValue(key, value)
    core.send2Handle2Long(OSCILLOSCOPE_ADD_GRAPH_DATA_0, {
    } m_parentHandle, m_thisHandle, key, value)

PUB addKeyValueF(keyF, valueF)
    core.send2Handle2Long(OSCILLOSCOPE_ADD_GRAPH_DATA_0_F, {
    } m_parentHandle, m_thisHandle, keyF, valueF)

PUB addValue(value)
    core.send2Handle1Long(OSCILLOSCOPE_ADD_GRAPH_DATA_1, {
    } m_parentHandle, m_thisHandle, value)

PUB addValueF(valueF)
    core.send2Handle1Long(OSCILLOSCOPE_ADD_GRAPH_DATA_1_F, {
    } m_parentHandle, m_thisHandle, valueF)

PUB addKeyValueWithSymmetricalErrors(key, value, keyError, valueError)
    core.send2Handle4Long(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_0, {
    } m_parentHandle, m_thisHandle, key, value, keyError, valueError)

PUB addKeyValueWithSymmetricalErrorsF(keyF, valueF, keyErrorF, valueErrorF)
    core.send2Handle4Long(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_0_F, {
    } m_parentHandle, m_thisHandle, keyF, valueF, keyErrorF, valueErrorF)

PUB addValueWithSymmetricalErrors(value, keyError, valueError)
    core.send2Handle3Long(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_1, {
    } m_parentHandle, m_thisHandle, value, keyError, valueError)

PUB addValueWithSymmetricalErrorsF(valueF, keyErrorF, valueErrorF)
    core.send2Handle3Long(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_1_F, {
    } m_parentHandle, m_thisHandle, valueF, keyErrorF, valueErrorF)

PUB addKeyValueWithErrors(key, value, {
} keyErrorMinus, keyErrorPlus, valueErrorMinus, valueErrorPlus)
    core.send2Handle6Long(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_2, {
    } m_parentHandle, m_thisHandle, key, value, {
    } keyErrorMinus, keyErrorPlus, valueErrorMinus, valueErrorPlus)

PUB addKeyValueWithErrorsF(keyF, valueF, {
} keyErrorMinusF, keyErrorPlusF, valueErrorMinusF, valueErrorPlusF)
    core.send2Handle6Long(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_2_F, {
    } m_parentHandle, m_thisHandle, keyF, valueF, {
    } keyErrorMinusF, keyErrorPlusF, valueErrorMinusF, valueErrorPlusF)

PUB addValueWithErrors(value, {
} keyErrorMinus, keyErrorPlus, valueErrorMinus, valueErrorPlus)
    core.send2Handle5Long(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_3, {
    } m_parentHandle, m_thisHandle, value, {
    } keyErrorMinus, keyErrorPlus, valueErrorMinus, valueErrorPlus)

PUB addValueWithErrorsF(valueF, {
} keyErrorMinusF, keyErrorPlusF, valueErrorMinusF, valueErrorPlusF)
    core.send2Handle5Long(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_3_F, {
    } m_parentHandle, m_thisHandle, valueF, {
    } keyErrorMinusF, keyErrorPlusF, valueErrorMinusF, valueErrorPlusF)

PUB removeDataBefore(before)
    core.send2Handle1Long(OSCILLOSCOPE_REMOVE_GRAPH_DATA_BEFORE, {
    } m_parentHandle, m_thisHandle, before)

PUB removeDataBeforeF(beforeF)
    core.send2Handle1Long(OSCILLOSCOPE_REMOVE_GRAPH_DATA_BEFORE_F, {
    } m_parentHandle, m_thisHandle, beforeF)

PUB removeDataAfter(after)
    core.send2Handle1Long(OSCILLOSCOPE_REMOVE_GRAPH_DATA_AFTER, {
    } m_parentHandle, m_thisHandle, after)

PUB removeDataAfterF(afterF)
    core.send2Handle1Long(OSCILLOSCOPE_REMOVE_GRAPH_DATA_AFTER_F, {
    } m_parentHandle, m_thisHandle, afterF)

PUB removeDataBetween(fromKey, toKey)
    core.send2Handle2Long(OSCILLOSCOPE_REMOVE_GRAPH_DATA_BETWEEN, {
    } m_parentHandle, m_thisHandle, fromKey, toKey)

PUB removeDataBetweenF(fromKeyF, toKeyF)
    core.send2Handle2Long(OSCILLOSCOPE_REMOVE_GRAPH_DATA_BETWEEN_F, {
    } m_parentHandle, m_thisHandle, fromKeyF, toKeyF)

PUB removeDatum(datum)
    core.send2Handle1Long(OSCILLOSCOPE_REMOVE_GRAPH_DATUM, {
    } m_parentHandle, m_thisHandle, datum)

PUB removeDatumF(datumF)
    core.send2Handle1Long(OSCILLOSCOPE_REMOVE_GRAPH_DATUM_F, {
    } m_parentHandle, m_thisHandle, datumF)

PUB setName(name)
    core.send2Handle1String(OSCILLOSCOPE_SET_GRAPH_NAME, {
    } m_parentHandle, m_thisHandle, name)

PUB setCoordinateSystem(coordinateSystem)
    core.send2Handle1Long(OSCILLOSCOPE_SET_GRAPH_COORDINATE_SYSTEM, {
    } m_parentHandle, m_thisHandle, coordinateSystem)

PUB setAngleUnits(angleUnits)
    core.send2Handle1Long(OSCILLOSCOPE_SET_GRAPH_ANGLE_UNITS, {
    } m_parentHandle, m_thisHandle, angleUnits)

PUB setLineColor(lineColor)
    core.send2Handle1Long(OSCILLOSCOPE_SET_GRAPH_LINE_COLOR, {
    } m_parentHandle, m_thisHandle, lineColor)

PUB setLineStyle(lineStyle)
    core.send2Handle1Long(OSCILLOSCOPE_SET_GRAPH_LINE_STYLE, {
    } m_parentHandle, m_thisHandle, lineStyle)

PUB setFillColor(fillColor)
    core.send2Handle1Long(OSCILLOSCOPE_SET_GRAPH_FILL_COLOR, {
    } m_parentHandle, m_thisHandle, fillColor)

PUB setFillStyle(fillStyle)
    core.send2Handle1Long(OSCILLOSCOPE_SET_GRAPH_FILL_STYLE, {
    } m_parentHandle, m_thisHandle, fillStyle)

PUB setDeltaMode(deltaMode)
    core.send2Handle1Long(OSCILLOSCOPE_SET_GRAPH_DELTA_MODE, {
    } m_parentHandle, m_thisHandle, deltaMode)

PUB setSize(size)
    core.send2Handle1Long(OSCILLOSCOPE_SET_GRAPH_SIZE, {
    } m_parentHandle, m_thisHandle, size)

PUB setSampleRate(sampleRate)
    core.send2Handle1Long(OSCILLOSCOPE_SET_GRAPH_SAMPLE_RATE, {
    } m_parentHandle, m_thisHandle, sampleRate)

PUB setSampleRateF(sampleRateF)
    core.send2Handle1Long(OSCILLOSCOPE_SET_GRAPH_SAMPLE_RATE_F, {
    } m_parentHandle, m_thisHandle, sampleRateF)

PUB setScaler(scaler)
    core.send2Handle1Long(OSCILLOSCOPE_SET_GRAPH_SCALER, {
    } m_parentHandle, m_thisHandle, scaler)

PUB setScalerF(scalerF)
    core.send2Handle1Long(OSCILLOSCOPE_SET_GRAPH_SCALER_F, {
    } m_parentHandle, m_thisHandle, scalerF)

PUB setFFT(fft)
    core.send2Handle1Long(OSCILLOSCOPE_SET_GRAPH_FFT, {
    } m_parentHandle, m_thisHandle, fft)

PUB setFFTWF(fftwf)
    core.send2Handle1Long(OSCILLOSCOPE_SET_GRAPH_FFT_WINDOW, {
    } m_parentHandle, m_thisHandle, fftwf)

PUB setScatterShape(scatterShape)
    core.send2Handle1Long(OSCILLOSCOPE_SET_GRAPH_SCATTER_STYLE, {
    } m_parentHandle, m_thisHandle, scatterShape)

PUB setGraphStyle(graphStyle)
    core.send2Handle1Long(OSCILLOSCOPE_SET_GRAPH_STYLE, {
    } m_parentHandle, m_thisHandle, graphStyle)

PUB setChannelFillGraph(graphHandle)
    core.send2Handle1Byte(OSCILLOSCOPE_SET_GRAPH_CHANNEL_FILL_GRAPH, {
    } m_parentHandle, m_thisHandle, graphHandle)

PUB setKeyErrors(keyErrors)
    core.send2Handle1Long(OSCILLOSCOPE_SET_GRAPH_KEY_ERRORS, {
    } m_parentHandle, m_thisHandle, keyErrors)

PUB setValueErrors(valueErrors)
    core.send2Handle1Long(OSCILLOSCOPE_SET_GRAPH_VALUE_ERRORS, {
    } m_parentHandle, m_thisHandle, valueErrors)

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
