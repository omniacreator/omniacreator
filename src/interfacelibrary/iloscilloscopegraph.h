/***************************************************************************//**
* @file
* Interface Library Oscilloscope Graph
*
* @version @n 1.0
* @date @n 3/16/2014
*
* @author @n Kwabena W. Agyeman
* @copyright @n (c) 2014 Kwabena W. Agyeman
* @n All rights reserved - Please see the end of the file for the terms of use
*
* @par Update History:
* @n v1.0 - Original release - 3/16/2014
*******************************************************************************/

#ifndef ILOSCILLOSCOPEGRAPH_H
#define ILOSCILLOSCOPEGRAPH_H

#include "iloscilloscope.h"

typedef struct GraphKeyValue
{
    int32 key, value;
}
GraphKeyValue;

typedef struct GraphKeyValueF
{
    float32 key, value;
}
GraphKeyValueF;

typedef struct GraphRange
{
    int32 from, to;
}
GraphRange;

typedef struct GraphRangeF
{
    float32 from, to;
}
GraphRangeF;

typedef struct GraphKeyValueWithSymmetricalErrors
{
    int32 key, value, keyError, valueError;
}
GraphKeyValueWithSymmetricalErrors;

typedef struct GraphKeyValueWithSymmetricalErrorsF
{
    float32 key, value, keyError, valueError;
}
GraphKeyValueWithSymmetricalErrorsF;

typedef struct GraphValueWithSymmetricalErrors
{
    int32 value, keyError, valueError;
}
GraphValueWithSymmetricalErrors;

typedef struct GraphValueWithSymmetricalErrorsF
{
    float32 value, keyError, valueError;
}
GraphValueWithSymmetricalErrorsF;

typedef struct GraphKeyValueWithErrors
{
    int32 key, value,
    keyErrorMinus, keyErrorPlus,
    valueErrorMinus, valueErrorPlus;
}
GraphKeyValueWithErrors;

typedef struct GraphKeyValueWithErrorsF
{
    float32 key, value,
    keyErrorMinus, keyErrorPlus,
    valueErrorMinus, valueErrorPlus;
}
GraphKeyValueWithErrorsF;

typedef struct GraphValueWithErrors
{
    int32 value,
    keyErrorMinus, keyErrorPlus,
    valueErrorMinus, valueErrorPlus;
}
GraphValueWithErrors;

typedef struct GraphValueWithErrorsF
{
    float32 value,
    keyErrorMinus, keyErrorPlus,
    valueErrorMinus, valueErrorPlus;
}
GraphValueWithErrorsF;

class ILOscilloscopeGraph : public ILOscilloscopePlot
{

public:

    ILOscilloscopeGraph() : ILOscilloscopePlot()
    {
        m_coordinateSystem = CoordinateSystem(); m_angleUnits = AngleUnits();

        m_lineColor = argb(); m_lineStyle = LineStyle();
        m_fillColor = argb(); m_fillStyle = FillStyle();

        m_deltaMode = bool(); m_size = uint32();
        m_sampleRate = int32(); m_sampleRateF = float32();
        m_scaler = int32(); m_scalerF = float32();

        m_fft = GraphFFT(); m_fftwf = GraphFFTWF();

        m_scatterShape = ScatterShape(); m_graphStyle = GraphLineStyle();

        m_channelFillGraph = 0;
        m_cFGFriend = 0;

        m_keyErrors = bool(); m_valueErrors = bool();
    }

    virtual ILNodeType type() const
    {
        return NT_WINDOW_OSCILLOSCOPE_PLOT_GRAPH;
    }

    bool init(ILOscilloscope *parent, const char *name,
              bool waitForResponse = false)
    {
        bool result = (!m_enabled) && parent && parent->enabled() && name;

        if(result)
        {
            construct(parent);

            if(!(result = sendPacket(OSCILLOSCOPE_NEW_GRAPH,
            name, waitForResponse)))
            {
                deconstruct();
            }
        }

        return result;
    }

    bool removeData(bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_GRAPH_DATA,
        waitForResponse);
    }

    // Adders /////////////////////////////////////////////////////////////////

    bool addKeyValue(int32 key, int32 value,
                     bool waitForResponse = false)
    {
        GraphKeyValue keyValue; keyValue.key = key; keyValue.value = value;

        return addKeyValue(keyValue, waitForResponse);
    }

    bool addKeyValue(const GraphKeyValue &keyValue,
                     bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_0,
        reinterpret_cast<const int32 *>(&keyValue),
        sizeof(GraphKeyValue) / sizeof(int32),
        waitForResponse);
    }

    bool addKeyValues(const GraphKeyValue *keyValues, uint16 size,
                      bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_0_A,
        reinterpret_cast<const int32 *>(keyValues), size,
        sizeof(GraphKeyValue) / sizeof(int32),
        waitForResponse);
    }

    bool addKeyValueF(float32 key, float32 value,
                      bool waitForResponse = false)
    {
        GraphKeyValueF keyValue; keyValue.key = key; keyValue.value = value;

        return addKeyValueF(keyValue, waitForResponse);
    }

    bool addKeyValueF(const GraphKeyValueF &keyValue,
                      bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_0_F,
        reinterpret_cast<const float32 *>(&keyValue),
        sizeof(GraphKeyValueF) / sizeof(float32),
        waitForResponse);
    }

    bool addKeyValuesF(const GraphKeyValueF *keyValues, uint16 size,
                       bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_0_F_A,
        reinterpret_cast<const float32 *>(keyValues), size,
        sizeof(GraphKeyValueF) / sizeof(float32),
        waitForResponse);
    }

    bool addValue(int32 value,
                  bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_1,
        value, waitForResponse);
    }

    bool addValues(const int32 *values, uint16 size,
                   bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_1_A,
        values, size, sizeof(int32) / sizeof(int32),
        waitForResponse);
    }

    bool addValueF(float32 value,
                   bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_1_F,
        value, waitForResponse);
    }

    bool addValuesF(const float32 *values, uint16 size,
                    bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_1_F_A,
        values, size, sizeof(float32) / sizeof(float32),
        waitForResponse);
    }

    bool addKeyValueWithSymmetricalErrors(int32 key, int32 value,
    int32 keyError, int32 valueError,
    bool waitForResponse = false)
    {
        GraphKeyValueWithSymmetricalErrors keyValue;

        keyValue.key = key;
        keyValue.value = value;
        keyValue.keyError = keyError;
        keyValue.valueError = valueError;

        return addKeyValueWithSymmetricalErrors(keyValue, waitForResponse);
    }

    bool addKeyValueWithSymmetricalErrors(
    const GraphKeyValueWithSymmetricalErrors &keyValue,
    bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_0,
        reinterpret_cast<const int32 *>(&keyValue),
        sizeof(GraphKeyValueWithSymmetricalErrors) / sizeof(int32),
        waitForResponse);
    }

    bool addKeyValueWithSymmetricalErrors(
    const GraphKeyValueWithSymmetricalErrors *keyValues,
    uint16 size, bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_0_A,
        reinterpret_cast<const int32 *>(keyValues), size,
        sizeof(GraphKeyValueWithSymmetricalErrors) / sizeof(int32),
        waitForResponse);
    }

    bool addKeyValueWithSymmetricalErrorsF(float32 key, float32 value,
    float32 keyError, float32 valueError,
    bool waitForResponse = false)
    {
        GraphKeyValueWithSymmetricalErrorsF keyValue;

        keyValue.key = key;
        keyValue.value = value;
        keyValue.keyError = keyError;
        keyValue.valueError = valueError;

        return addKeyValueWithSymmetricalErrorsF(keyValue, waitForResponse);
    }

    bool addKeyValueWithSymmetricalErrorsF(
    const GraphKeyValueWithSymmetricalErrorsF &keyValue,
    bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_0_F,
        reinterpret_cast<const float32 *>(&keyValue),
        sizeof(GraphKeyValueWithSymmetricalErrorsF) / sizeof(float32),
        waitForResponse);
    }

    bool addKeyValueWithSymmetricalErrorsF(
    const GraphKeyValueWithSymmetricalErrorsF *keyValues,
    uint16 size, bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_0_F_A,
        reinterpret_cast<const float32 *>(keyValues), size,
        sizeof(GraphKeyValueWithSymmetricalErrorsF) / sizeof(float32),
        waitForResponse);
    }

    bool addValueWithSymmetricalErrors(int32 value,
    int32 keyError, int32 valueError,
    bool waitForResponse = false)
    {
        GraphValueWithSymmetricalErrors data;

        data.value = value;
        data.keyError = keyError;
        data.valueError = valueError;

        return addValueWithSymmetricalErrors(data, waitForResponse);
    }

    bool addValueWithSymmetricalErrors(
    const GraphValueWithSymmetricalErrors &value,
    bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_1,
        reinterpret_cast<const int32 *>(&value),
        sizeof(GraphValueWithSymmetricalErrors) / sizeof(int32),
        waitForResponse);
    }

    bool addValueWithSymmetricalErrors(
    const GraphValueWithSymmetricalErrors *values,
    uint16 size, bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_1_A,
        reinterpret_cast<const int32 *>(values), size,
        sizeof(GraphValueWithSymmetricalErrors) / sizeof(int32),
        waitForResponse);
    }

    bool addValueWithSymmetricalErrorsF(float32 value,
    float32 keyError, float32 valueError,
    bool waitForResponse = false)
    {
        GraphValueWithSymmetricalErrorsF data;

        data.value = value;
        data.keyError = keyError;
        data.valueError = valueError;

        return addValueWithSymmetricalErrorsF(data, waitForResponse);
    }

    bool addValueWithSymmetricalErrorsF(
    const GraphValueWithSymmetricalErrorsF &value,
    bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_1_F,
        reinterpret_cast<const float32 *>(&value),
        sizeof(GraphValueWithSymmetricalErrorsF) / sizeof(float32),
        waitForResponse);
    }

    bool addValueWithSymmetricalErrorsF(
    const GraphValueWithSymmetricalErrorsF *values,
    uint16 size, bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_1_F_A,
        reinterpret_cast<const float32 *>(values), size,
        sizeof(GraphValueWithSymmetricalErrorsF) / sizeof(float32),
        waitForResponse);
    }

    bool addKeyValueWithErrors(int32 key, int32 value,
    int32 keyErrorMinus, int32 keyErrorPlus,
    int32 valueErrorMinus, int32 valueErrorPlus,
    bool waitForResponse = false)
    {
        GraphKeyValueWithErrors keyValue;

        keyValue.key = key;
        keyValue.value = value;
        keyValue.keyErrorMinus = keyErrorMinus;
        keyValue.keyErrorPlus = keyErrorPlus;
        keyValue.valueErrorMinus = valueErrorMinus;
        keyValue.valueErrorPlus = valueErrorPlus;

        return addKeyValueWithErrors(keyValue, waitForResponse);
    }

    bool addKeyValueWithErrors(
    const GraphKeyValueWithErrors &keyValue,
    bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_2,
        reinterpret_cast<const int32 *>(&keyValue),
        sizeof(GraphKeyValueWithErrors) / sizeof(int32),
        waitForResponse);
    }

    bool addKeyValueWithErrors(
    const GraphKeyValueWithErrors *keyValues,
    uint16 size, bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_2_A,
        reinterpret_cast<const int32 *>(keyValues), size,
        sizeof(GraphKeyValueWithErrors) / sizeof(int32),
        waitForResponse);
    }

    bool addKeyValueWithErrorsF(float32 key, float32 value,
    float32 keyErrorMinus, float32 keyErrorPlus,
    float32 valueErrorMinus, float32 valueErrorPlus,
    bool waitForResponse = false)
    {
        GraphKeyValueWithErrorsF keyValue;

        keyValue.key = key;
        keyValue.value = value;
        keyValue.keyErrorMinus = keyErrorMinus;
        keyValue.keyErrorPlus = keyErrorPlus;
        keyValue.valueErrorMinus = valueErrorMinus;
        keyValue.valueErrorPlus = valueErrorPlus;

        return addKeyValueWithErrorsF(keyValue, waitForResponse);
    }

    bool addKeyValueWithErrorsF(
    const GraphKeyValueWithErrorsF &keyValue,
    bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_2_F,
        reinterpret_cast<const float32 *>(&keyValue),
        sizeof(GraphKeyValueWithErrorsF) / sizeof(float32),
        waitForResponse);
    }

    bool addKeyValueWithErrorsF(
    const GraphKeyValueWithErrorsF *keyValues,
    uint16 size, bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_2_F_A,
        reinterpret_cast<const float32 *>(keyValues), size,
        sizeof(GraphKeyValueWithErrorsF) / sizeof(float32),
        waitForResponse);
    }

    bool addValueWithErrors(int32 value,
    int32 keyErrorMinus, int32 keyErrorPlus,
    int32 valueErrorMinus, int32 valueErrorPlus,
    bool waitForResponse = false)
    {
        GraphValueWithErrors data;

        data.value = value;
        data.keyErrorMinus = keyErrorMinus;
        data.keyErrorPlus = keyErrorPlus;
        data.valueErrorMinus = valueErrorMinus;
        data.valueErrorPlus = valueErrorPlus;

        return addValueWithErrors(data, waitForResponse);
    }

    bool addValueWithErrors(
    const GraphValueWithErrors &value,
    bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_3,
        reinterpret_cast<const int32 *>(&value),
        sizeof(GraphValueWithErrors) / sizeof(int32),
        waitForResponse);
    }

    bool addValueWithErrors(
    const GraphValueWithErrors *values,
    uint16 size, bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_3_A,
        reinterpret_cast<const int32 *>(values), size,
        sizeof(GraphValueWithErrors) / sizeof(int32),
        waitForResponse);
    }

    bool addValueWithErrorsF(float32 value,
    float32 keyErrorMinus, float32 keyErrorPlus,
    float32 valueErrorMinus, float32 valueErrorPlus,
    bool waitForResponse = false)
    {
        GraphValueWithErrorsF data;

        data.value = value;
        data.keyErrorMinus = keyErrorMinus;
        data.keyErrorPlus = keyErrorPlus;
        data.valueErrorMinus = valueErrorMinus;
        data.valueErrorPlus = valueErrorPlus;

        return addValueWithErrorsF(data, waitForResponse);
    }

    bool addValueWithErrorsF(
    const GraphValueWithErrorsF &value,
    bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_3_F,
        reinterpret_cast<const float32 *>(&value),
        sizeof(GraphValueWithErrorsF) / sizeof(float32),
        waitForResponse);
    }

    bool addValueWithErrorsF(
    const GraphValueWithErrorsF *values,
    uint16 size, bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_GRAPH_DATA_WITH_ERRORS_3_F_A,
        reinterpret_cast<const float32 *>(values), size,
        sizeof(GraphValueWithErrorsF) / sizeof(float32),
        waitForResponse);
    }

    // Removers ///////////////////////////////////////////////////////////////

    bool removeDataBefore(int32 before,
                          bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_GRAPH_DATA_BEFORE,
        before, waitForResponse);
    }

    bool removeDataBeforeF(float32 before,
                           bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_GRAPH_DATA_BEFORE_F,
        before, waitForResponse);
    }

    bool removeDataAfter(int32 after,
                         bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_GRAPH_DATA_AFTER,
        after, waitForResponse);
    }

    bool removeDataAfterF(float32 after,
                          bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_GRAPH_DATA_AFTER_F,
        after, waitForResponse);
    }

    bool removeDataBetween(int32 from, int32 to,
                           bool waitForResponse = false)
    {
        GraphRange range; range.from = from; range.to = to;

        return removeDataBetween(range, waitForResponse);
    }

    bool removeDataBetween(const GraphRange &range,
                           bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_GRAPH_DATA_BETWEEN,
        reinterpret_cast<const int32 *>(&range),
        sizeof(GraphRange) / sizeof(int32),
        waitForResponse);
    }

    bool removeDataBetween(const GraphRange *ranges, uint16 size,
                           bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_GRAPH_DATA_BETWEEN_A,
        reinterpret_cast<const int32 *>(ranges), size,
        sizeof(GraphRange) / sizeof(int32),
        waitForResponse);
    }

    bool removeDataBetweenF(float32 from, float32 to,
                            bool waitForResponse = false)
    {
        GraphRangeF range; range.from = from; range.to = to;

        return removeDataBetweenF(range, waitForResponse);
    }

    bool removeDataBetweenF(const GraphRangeF &range,
                            bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_GRAPH_DATA_BETWEEN_F,
        reinterpret_cast<const float32 *>(&range),
        sizeof(GraphRangeF) / sizeof(float32),
        waitForResponse);
    }

    bool removeDataBetweenF(const GraphRangeF *ranges, uint16 size,
                            bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_GRAPH_DATA_BETWEEN_F_A,
        reinterpret_cast<const float32 *>(ranges), size,
        sizeof(GraphRangeF) / sizeof(float32),
        waitForResponse);
    }

    bool removeDatum(int32 datum,
                     bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_GRAPH_DATUM,
        datum, waitForResponse);
    }

    bool removeData(const int32 *data, uint16 size,
                    bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_GRAPH_DATUM_A,
        data, size, sizeof(int32) / sizeof(int32),
        waitForResponse);
    }

    bool removeDatumF(float32 datum,
                      bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_GRAPH_DATUM_F,
        datum, waitForResponse);
    }

    bool removeDataF(const float32 *data, uint16 size,
                     bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_GRAPH_DATUM_F_A,
        data, size, sizeof(float32) / sizeof(float32),
        waitForResponse);
    }

    // Properties /////////////////////////////////////////////////////////////

    bool setName(const char *name,
                 bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_SET_GRAPH_NAME,
        name, waitForResponse);
    }

    bool getName(char *name,
                 uint16 size)
    {
        return receivePacket(OSCILLOSCOPE_GET_GRAPH_NAME,
        name, size);
    }

    bool setCoordinateSystem(CoordinateSystem coordinateSystem,
                             bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_GRAPH_COORDINATE_SYSTEM,
        int32(coordinateSystem), waitForResponse)))
        {
            m_coordinateSystem = coordinateSystem;
        }

        return result;
    }

    CoordinateSystem getCoordinateSystem(bool bypassCache = false,
                                         bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_GRAPH_COORDINATE_SYSTEM,
            &response)))
            {
               m_coordinateSystem = CoordinateSystem(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_coordinateSystem : CoordinateSystem();
    }

    bool setAngleUnits(AngleUnits angleUnits,
                       bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_GRAPH_ANGLE_UNITS,
        int32(angleUnits), waitForResponse)))
        {
            m_angleUnits = angleUnits;
        }

        return result;
    }

    AngleUnits getAngleUnits(bool bypassCache = false,
                             bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_GRAPH_ANGLE_UNITS,
            &response)))
            {
               m_angleUnits = AngleUnits(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_angleUnits : AngleUnits();
    }

    bool setLineColor(argb lineColor,
                      bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_GRAPH_LINE_COLOR,
        int32(lineColor), waitForResponse)))
        {
            m_lineColor = lineColor;
        }

        return result;
    }

    argb getLineColor(bool bypassCache = false,
                      bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_GRAPH_LINE_COLOR,
            &response)))
            {
               m_lineColor = argb(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_lineColor : argb();
    }

    bool setLineStyle(LineStyle lineStyle,
                      bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_GRAPH_LINE_STYLE,
        int32(lineStyle), waitForResponse)))
        {
            m_lineStyle = lineStyle;
        }

        return result;
    }

    LineStyle getLineStyle(bool bypassCache = false,
                           bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_GRAPH_LINE_STYLE,
            &response)))
            {
               m_lineStyle = LineStyle(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_lineStyle : LineStyle();
    }

    bool setFillColor(argb fillColor,
                      bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_GRAPH_FILL_COLOR,
        int32(fillColor), waitForResponse)))
        {
            m_fillColor = fillColor;
        }

        return result;
    }

    argb getFillColor(bool bypassCache = false,
                      bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_GRAPH_FILL_COLOR,
            &response)))
            {
               m_fillColor = argb(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_fillColor : argb();
    }

    bool setFillStyle(FillStyle fillStyle,
                      bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_GRAPH_FILL_STYLE,
        int32(fillStyle), waitForResponse)))
        {
            m_fillStyle = fillStyle;
        }

        return result;
    }

    FillStyle getFillStyle(bool bypassCache = false,
                           bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_GRAPH_FILL_STYLE,
            &response)))
            {
               m_fillStyle = FillStyle(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_fillStyle : FillStyle();
    }

    bool setDeltaMode(bool deltaMode,
                      bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_GRAPH_DELTA_MODE,
        int32(deltaMode), waitForResponse)))
        {
            m_deltaMode = deltaMode;
        }

        return result;
    }

    bool getDeltaMode(bool bypassCache = false,
                      bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_GRAPH_DELTA_MODE,
            &response)))
            {
               m_deltaMode = bool(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_deltaMode : bool();
    }

    bool setSize(uint32 size,
                 bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_GRAPH_SIZE,
        int32(size), waitForResponse)))
        {
            m_size = size;
        }

        return result;
    }

    uint32 getSize(bool bypassCache = false,
                   bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_GRAPH_SIZE,
            &response)))
            {
               m_size = uint32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_size : uint32();
    }

    bool setSampleRate(int32 sampleRate,
                       bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_GRAPH_SAMPLE_RATE,
        int32(sampleRate), waitForResponse)))
        {
            m_sampleRate = sampleRate;
        }

        return result;
    }

    int32 getSampleRate(bool bypassCache = false,
                        bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_GRAPH_SAMPLE_RATE,
            &response)))
            {
               m_sampleRate = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_sampleRate : int32();
    }

    bool setSampleRateF(float32 sampleRateF,
                        bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_GRAPH_SAMPLE_RATE_F,
        float32(sampleRateF), waitForResponse)))
        {
            m_sampleRateF = sampleRateF;
        }

        return result;
    }

    float32 getSampleRateF(bool bypassCache = false,
                           bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_GRAPH_SAMPLE_RATE_F,
            &response)))
            {
               m_sampleRateF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_sampleRateF : float32();
    }

//    bool setSampleRateF(int32 sampleRateF,
//                        bool waitForResponse = false)
//    {
//        bool result = setSampleRate(sampleRateF, waitForResponse);

//        if(result)
//        {
//            m_sampleRateF = m_sampleRate;
//        }

//        return result;
//    }

//    bool setSampleRate(float32 sampleRate,
//                       bool waitForResponse = false)
//    {
//        bool result = setSampleRateF(sampleRate, waitForResponse);

//        if(result)
//        {
//            m_sampleRate = m_sampleRateF;
//        }

//        return result;
//    }

    bool setScaler(int32 scaler,
                   bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_GRAPH_SCALER,
        int32(scaler), waitForResponse)))
        {
            m_scaler = scaler;
        }

        return result;
    }

    int32 getScaler(bool bypassCache = false,
                    bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_GRAPH_SCALER,
            &response)))
            {
               m_scaler = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_scaler : int32();
    }

    bool setScalerF(float32 scalerF,
                    bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_GRAPH_SCALER_F,
        float32(scalerF), waitForResponse)))
        {
            m_scalerF = scalerF;
        }

        return result;
    }

    float32 getScalerF(bool bypassCache = false,
                       bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_GRAPH_SCALER_F,
            &response)))
            {
               m_scalerF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_scalerF : float32();
    }

//    bool setScalerF(int32 scalerF,
//                    bool waitForResponse = false)
//    {
//        bool result = setScaler(scalerF, waitForResponse);

//        if(result)
//        {
//            m_scalerF = m_scaler;
//        }

//        return result;
//    }

//    bool setScaler(float32 scaler,
//                   bool waitForResponse = false)
//    {
//        bool result = setScalerF(scaler, waitForResponse);

//        if(result)
//        {
//            m_scaler = m_scalerF;
//        }

//        return result;
//    }

    bool setFFT(GraphFFT fft,
                bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_GRAPH_FFT,
        int32(fft), waitForResponse)))
        {
            m_fft = fft;
        }

        return result;
    }

    GraphFFT getFFT(bool bypassCache = false,
                    bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_GRAPH_FFT,
            &response)))
            {
               m_fft = GraphFFT(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_fft : GraphFFT();
    }

    bool setFFTWF(GraphFFTWF fftwf,
                  bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_GRAPH_FFT_WINDOW,
        int32(fftwf), waitForResponse)))
        {
            m_fftwf = fftwf;
        }

        return result;
    }

    GraphFFTWF getFFTWF(bool bypassCache = false,
                        bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_GRAPH_FFT_WINDOW,
            &response)))
            {
               m_fftwf = GraphFFTWF(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_fftwf : GraphFFTWF();
    }

    bool setScatterShape(ScatterShape scatterShape,
                         bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_GRAPH_SCATTER_STYLE,
        int32(scatterShape), waitForResponse)))
        {
            m_scatterShape = scatterShape;
        }

        return result;
    }

    ScatterShape getScatterShape(bool bypassCache = false,
                                 bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_GRAPH_SCATTER_STYLE,
            &response)))
            {
               m_scatterShape = ScatterShape(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_scatterShape : ScatterShape();
    }

    bool setGraphStyle(GraphLineStyle graphStyle,
                       bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_GRAPH_STYLE,
        int32(graphStyle), waitForResponse)))
        {
            m_graphStyle = graphStyle;
        }

        return result;
    }

    GraphLineStyle getGraphStyle(bool bypassCache = false,
                             bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_GRAPH_STYLE,
            &response)))
            {
               m_graphStyle = GraphLineStyle(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_graphStyle : GraphLineStyle();
    }

    bool setChannelFillGraph(ILOscilloscopeGraph *graph,
                             bool waitForResponse = false)
    {
        bool result = graph && graph->enabled() &&
                      (graph->parent() == m_parent);

        if(result)
        {
            if((result = sendPacket(OSCILLOSCOPE_SET_GRAPH_CHANNEL_FILL_GRAPH,
            graph->handle(), waitForResponse)))
            {
                m_channelFillGraph = graph; graph->m_cFGFriend = this;
            }
        }

        return result;
    }

    ILOscilloscopeGraph *getChannelFillGraph(bool bypassCache = false,
                                             bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_GRAPH_CHANNEL_FILL_GRAPH,
            &response)))
            {
               m_channelFillGraph =
               static_cast<ILOscilloscopeGraph *>(findSibling(response));
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_channelFillGraph : 0;
    }

    bool setKeyErrors(bool keyErrors,
                      bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_GRAPH_KEY_ERRORS,
        int32(keyErrors), waitForResponse)))
        {
            m_keyErrors = keyErrors;
        }

        return result;
    }

    bool getKeyErrors(bool bypassCache = false,
                      bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_GRAPH_KEY_ERRORS,
            &response)))
            {
               m_keyErrors = bool(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_keyErrors : bool();
    }

    bool setValueErrors(bool valueErrors,
                        bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_GRAPH_VALUE_ERRORS,
        int32(valueErrors), waitForResponse)))
        {
            m_valueErrors = valueErrors;
        }

        return result;
    }

    bool getValueErrors(bool bypassCache = false,
                        bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_GRAPH_VALUE_ERRORS,
            &response)))
            {
               m_valueErrors = bool(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_valueErrors : bool();
    }

protected:

    virtual void deconstruct()
    {
        if(m_cFGFriend)
        {
            m_cFGFriend->m_channelFillGraph = 0;
        }

        ILOscilloscopePlot::deconstruct();
    }

private:

    ILOscilloscopeGraph(const ILOscilloscopeGraph &);
    ILOscilloscopeGraph &operator=(const ILOscilloscopeGraph &);

    CoordinateSystem m_coordinateSystem; AngleUnits m_angleUnits;

    argb m_lineColor; LineStyle m_lineStyle;
    argb m_fillColor; FillStyle m_fillStyle;

    bool m_deltaMode; uint32 m_size;
    int32 m_sampleRate; float32 m_sampleRateF;
    int32 m_scaler; float32 m_scalerF;

    GraphFFT m_fft; GraphFFTWF m_fftwf;

    ScatterShape m_scatterShape; GraphLineStyle m_graphStyle;

    ILOscilloscopeGraph *m_channelFillGraph;
    ILOscilloscopeGraph *m_cFGFriend;

    bool m_keyErrors; bool m_valueErrors;
};

#endif // ILOSCILLOSCOPEGRAPH_H

/***************************************************************************//**
* @file
* @par MIT License - TERMS OF USE:
* @n Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* @n
* @n The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* @n
* @n THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*******************************************************************************/
