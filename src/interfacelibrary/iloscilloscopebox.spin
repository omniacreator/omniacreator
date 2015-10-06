''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' @file
' Interface Library Oscilloscope Box
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

    #(core#OSCILLOSCOPE_BOX_BASE * core#BASE_SIZE)

    OSCILLOSCOPE_NEW_BOX
    OSCILLOSCOPE_NEW_BOX_SPACER_0 ' spacer
    OSCILLOSCOPE_NEW_BOX_SPACER_1 ' spacer
    OSCILLOSCOPE_NEW_BOX_SPACER_2 ' spacer
    OSCILLOSCOPE_REMOVE_BOX_DATA
    OSCILLOSCOPE_REMOVE_BOX_DATA_SPACER_0 ' spacer
    OSCILLOSCOPE_REMOVE_BOX_DATA_SPACER_1 ' spacer
    OSCILLOSCOPE_REMOVE_BOX_DATA_SPACER_2 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_0
    OSCILLOSCOPE_ADD_BOX_DATA_0_A
    OSCILLOSCOPE_ADD_BOX_DATA_0_F
    OSCILLOSCOPE_ADD_BOX_DATA_0_F_A
    OSCILLOSCOPE_ADD_BOX_DATA_1
    OSCILLOSCOPE_ADD_BOX_DATA_1_A
    OSCILLOSCOPE_ADD_BOX_DATA_1_F
    OSCILLOSCOPE_ADD_BOX_DATA_1_F_A
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_00 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_01 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_02 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_03 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_04 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_05 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_06 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_07 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_08 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_09 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_10 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_11 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_12 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_13 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_14 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_15 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_16 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_17 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_18 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_19 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_20 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_21 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_22 ' spacer
    OSCILLOSCOPE_ADD_BOX_DATA_SPACER_23 ' spacer
    OSCILLOSCOPE_REMOVE_BOX_DATA_BEFORE
    OSCILLOSCOPE_REMOVE_BOX_DATA_BEFORE_F
    OSCILLOSCOPE_REMOVE_BOX_DATA_AFTER
    OSCILLOSCOPE_REMOVE_BOX_DATA_AFTER_F
    OSCILLOSCOPE_REMOVE_BOX_DATA_BETWEEN
    OSCILLOSCOPE_REMOVE_BOX_DATA_BETWEEN_A
    OSCILLOSCOPE_REMOVE_BOX_DATA_BETWEEN_F
    OSCILLOSCOPE_REMOVE_BOX_DATA_BETWEEN_F_A
    OSCILLOSCOPE_REMOVE_BOX_DATUM
    OSCILLOSCOPE_REMOVE_BOX_DATUM_A
    OSCILLOSCOPE_REMOVE_BOX_DATUM_F
    OSCILLOSCOPE_REMOVE_BOX_DATUM_F_A
    OSCILLOSCOPE_REMOVE_BOX_DATUM_SPACER_0 ' spacer
    OSCILLOSCOPE_REMOVE_BOX_DATUM_SPACER_1 ' spacer
    OSCILLOSCOPE_REMOVE_BOX_DATUM_SPACER_2 ' spacer
    OSCILLOSCOPE_REMOVE_BOX_DATUM_SPACER_3 ' spacer
    OSCILLOSCOPE_SET_BOX_NAME
    OSCILLOSCOPE_GET_BOX_NAME
    OSCILLOSCOPE_SET_BOX_COORDINATE_SYSTEM
    OSCILLOSCOPE_GET_BOX_COORDINATE_SYSTEM
    OSCILLOSCOPE_SET_BOX_ANGLE_UNITS
    OSCILLOSCOPE_GET_BOX_ANGLE_UNITS
    OSCILLOSCOPE_SET_BOX_LINE_COLOR
    OSCILLOSCOPE_GET_BOX_LINE_COLOR
    OSCILLOSCOPE_SET_BOX_LINE_STYLE
    OSCILLOSCOPE_GET_BOX_LINE_STYLE
    OSCILLOSCOPE_SET_BOX_FILL_COLOR
    OSCILLOSCOPE_GET_BOX_FILL_COLOR
    OSCILLOSCOPE_SET_BOX_FILL_STYLE
    OSCILLOSCOPE_GET_BOX_FILL_STYLE
    OSCILLOSCOPE_SET_BOX_DELTA_MODE
    OSCILLOSCOPE_GET_BOX_DELTA_MODE
    OSCILLOSCOPE_SET_BOX_SIZE
    OSCILLOSCOPE_GET_BOX_SIZE
    OSCILLOSCOPE_SET_BOX_SAMPLE_RATE
    OSCILLOSCOPE_GET_BOX_SAMPLE_RATE
    OSCILLOSCOPE_SET_BOX_SAMPLE_RATE_F
    OSCILLOSCOPE_GET_BOX_SAMPLE_RATE_F
    OSCILLOSCOPE_SET_BOX_SCALER
    OSCILLOSCOPE_GET_BOX_SCALER
    OSCILLOSCOPE_SET_BOX_SCALER_F
    OSCILLOSCOPE_GET_BOX_SCALER_F
    OSCILLOSCOPE_SET_BOX_WIDTH
    OSCILLOSCOPE_GET_BOX_WIDTH
    OSCILLOSCOPE_SET_BOX_WIDTH_F
    OSCILLOSCOPE_GET_BOX_WIDTH_F
    OSCILLOSCOPE_SET_BOX_SCATTER_STYLE
    OSCILLOSCOPE_GET_BOX_SCATTER_STYLE
    OSCILLOSCOPE_SET_BOX_KEY
    OSCILLOSCOPE_GET_BOX_KEY
    OSCILLOSCOPE_SET_BOX_KEY_F
    OSCILLOSCOPE_GET_BOX_KEY_F
    OSCILLOSCOPE_SET_BOX_MINIMUM
    OSCILLOSCOPE_GET_BOX_MINIMUM
    OSCILLOSCOPE_SET_BOX_MINIMUM_F
    OSCILLOSCOPE_GET_BOX_MINIMUM_F
    OSCILLOSCOPE_SET_BOX_LOWER_QUARTILE
    OSCILLOSCOPE_GET_BOX_LOWER_QUARTILE
    OSCILLOSCOPE_SET_BOX_LOWER_QUARTILE_F
    OSCILLOSCOPE_GET_BOX_LOWER_QUARTILE_F
    OSCILLOSCOPE_SET_BOX_MEDIAN
    OSCILLOSCOPE_GET_BOX_MEDIAN
    OSCILLOSCOPE_SET_BOX_MEDIAN_F
    OSCILLOSCOPE_GET_BOX_MEDIAN_F
    OSCILLOSCOPE_SET_BOX_UPPER_QUARTILE
    OSCILLOSCOPE_GET_BOX_UPPER_QUARTILE
    OSCILLOSCOPE_SET_BOX_UPPER_QUARTILE_F
    OSCILLOSCOPE_GET_BOX_UPPER_QUARTILE_F
    OSCILLOSCOPE_SET_BOX_MAXIMUM
    OSCILLOSCOPE_GET_BOX_MAXIMUM
    OSCILLOSCOPE_SET_BOX_MAXIMUM_F
    OSCILLOSCOPE_GET_BOX_MAXIMUM_F

PUB init(name)
    core.send2Handle1String(OSCILLOSCOPE_NEW_BOX, {
    } m_parentHandle, m_thisHandle, name)

PUB removeData
    core.send2Handle(OSCILLOSCOPE_REMOVE_BOX_DATA, {
    } m_parentHandle, m_thisHandle)

PUB addIndexValue(index, value)
    core.send2Handle2Long(OSCILLOSCOPE_ADD_BOX_DATA_0, {
    } m_parentHandle, m_thisHandle, index, value)

PUB addIndexValueF(indexF, valueF)
    core.send2Handle2Long(OSCILLOSCOPE_ADD_BOX_DATA_0_F, {
    } m_parentHandle, m_thisHandle, indexF, valueF)

PUB addValue(value)
    core.send2Handle1Long(OSCILLOSCOPE_ADD_BOX_DATA_1, {
    } m_parentHandle, m_thisHandle, value)

PUB addValueF(keyF, valueF)
    core.send2Handle1Long(OSCILLOSCOPE_ADD_BOX_DATA_1_F, {
    } m_parentHandle, m_thisHandle, valueF)

PUB removeDataBefore(before)
    core.send2Handle1Long(OSCILLOSCOPE_REMOVE_BOX_DATA_BEFORE, {
    } m_parentHandle, m_thisHandle, before)

PUB removeDataBeforeF(beforeF)
    core.send2Handle1Long(OSCILLOSCOPE_REMOVE_BOX_DATA_BEFORE_F, {
    } m_parentHandle, m_thisHandle, beforeF)

PUB removeDataAfter(after)
    core.send2Handle1Long(OSCILLOSCOPE_REMOVE_BOX_DATA_AFTER, {
    } m_parentHandle, m_thisHandle, after)

PUB removeDataAfterF(afterF)
    core.send2Handle1Long(OSCILLOSCOPE_REMOVE_BOX_DATA_AFTER_F, {
    } m_parentHandle, m_thisHandle, afterF)

PUB removeDataBetween(fromIndex, toIndex)
    core.send2Handle2Long(OSCILLOSCOPE_REMOVE_BOX_DATA_BETWEEN, {
    } m_parentHandle, m_thisHandle, fromIndex, toIndex)

PUB removeDataBetweenF(fromIndexF, toIndexF)
    core.send2Handle2Long(OSCILLOSCOPE_REMOVE_BOX_DATA_BETWEEN_F, {
    } m_parentHandle, m_thisHandle, fromIndexF, toIndexF)

PUB removeDatum(datum)
    core.send2Handle1Long(OSCILLOSCOPE_REMOVE_BOX_DATUM, {
    } m_parentHandle, m_thisHandle, datum)

PUB removeDatumF(datumF)
    core.send2Handle1Long(OSCILLOSCOPE_REMOVE_BOX_DATUM_F, {
    } m_parentHandle, m_thisHandle, datumF)

PUB setName(name)
    core.send2Handle1String(OSCILLOSCOPE_SET_BOX_NAME, {
    } m_parentHandle, m_thisHandle, name)

PUB setCoordinateSystem(coordinateSystem)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_COORDINATE_SYSTEM, {
    } m_parentHandle, m_thisHandle, coordinateSystem)

PUB setAngleUnits(angleUnits)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_ANGLE_UNITS, {
    } m_parentHandle, m_thisHandle, angleUnits)

PUB setLineColor(lineColor)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_LINE_COLOR, {
    } m_parentHandle, m_thisHandle, lineColor)

PUB setLineStyle(lineStyle)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_LINE_STYLE, {
    } m_parentHandle, m_thisHandle, lineStyle)

PUB setFillColor(fillColor)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_FILL_COLOR, {
    } m_parentHandle, m_thisHandle, fillColor)

PUB setFillStyle(fillStyle)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_FILL_STYLE, {
    } m_parentHandle, m_thisHandle, fillStyle)

PUB setDeltaMode(deltaMode)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_DELTA_MODE, {
    } m_parentHandle, m_thisHandle, deltaMode)

PUB setSize(size)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_SIZE, {
    } m_parentHandle, m_thisHandle, size)

PUB setSampleRate(sampleRate)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_SAMPLE_RATE, {
    } m_parentHandle, m_thisHandle, sampleRate)

PUB setSampleRateF(sampleRateF)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_SAMPLE_RATE_F, {
    } m_parentHandle, m_thisHandle, sampleRateF)

PUB setScaler(scaler)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_SCALER, {
    } m_parentHandle, m_thisHandle, scaler)

PUB setScalerF(scalerF)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_SCALER_F, {
    } m_parentHandle, m_thisHandle, scalerF)

PUB setWidth(width)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_WIDTH, {
    } m_parentHandle, m_thisHandle, width)

PUB setWidthF(widthF)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_WIDTH_F, {
    } m_parentHandle, m_thisHandle, widthF)

PUB setScatterShape(scatterShape)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_SCATTER_STYLE, {
    } m_parentHandle, m_thisHandle, scatterShape)

PUB setKey(key)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_KEY, {
    } m_parentHandle, m_thisHandle, key)

PUB setKeyF(keyF)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_KEY_F, {
    } m_parentHandle, m_thisHandle, keyF)

PUB setMinimum(minimum)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_MINIMUM, {
    } m_parentHandle, m_thisHandle, minimum)

PUB setMinimumF(minimumF)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_MINIMUM_F, {
    } m_parentHandle, m_thisHandle, minimumF)

PUB setLowerQuartile(lowerQuartile)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_LOWER_QUARTILE, {
    } m_parentHandle, m_thisHandle, lowerQuartile)

PUB setLowerQuartileF(lowerQuartileF)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_LOWER_QUARTILE_F, {
    } m_parentHandle, m_thisHandle, lowerQuartileF)

PUB setMedian(median)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_MEDIAN, {
    } m_parentHandle, m_thisHandle, median)

PUB setMedianF(medianF)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_MEDIAN_F, {
    } m_parentHandle, m_thisHandle, medianF)

PUB setUpperQuartile(upperQuartile)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_UPPER_QUARTILE, {
    } m_parentHandle, m_thisHandle, upperQuartile)

PUB setUpperQuartileF(upperQuartileF)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_UPPER_QUARTILE_F, {
    } m_parentHandle, m_thisHandle, upperQuartileF)

PUB setMaximum(maximum)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_MAXIMUM, {
    } m_parentHandle, m_thisHandle, maximum)

PUB setMaximumF(maximumF)
    core.send2Handle1Long(OSCILLOSCOPE_SET_BOX_MAXIMUM_F, {
    } m_parentHandle, m_thisHandle, maximumF)

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
