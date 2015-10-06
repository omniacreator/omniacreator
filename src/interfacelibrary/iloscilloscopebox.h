/***************************************************************************//**
* @file
* Interface Library Oscilloscope Box
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

#ifndef ILOSCILLOSCOPEBOX_H
#define ILOSCILLOSCOPEBOX_H

#include "iloscilloscope.h"

typedef struct BoxIndexValue
{
    int32 index, value;
}
BoxIndexValue;

typedef struct BoxIndexValueF
{
    float32 index, value;
}
BoxIndexValueF;

typedef struct BoxRange
{
    int32 from, to;
}
BoxRange;

typedef struct BoxRangeF
{
    float32 from, to;
}
BoxRangeF;

class ILOscilloscopeBox : public ILOscilloscopePlot
{

public:

    ILOscilloscopeBox() : ILOscilloscopePlot()
    {
        m_coordinateSystem = CoordinateSystem(); m_angleUnits = AngleUnits();

        m_lineColor = argb(); m_lineStyle = LineStyle();
        m_fillColor = argb(); m_fillStyle = FillStyle();

        m_deltaMode = bool(); m_size = uint32();
        m_sampleRate = int32(); m_sampleRateF = float32();
        m_scaler = int32(); m_scalerF = float32();

        m_width = int32(); m_widthF = float32();

        m_scatterShape = ScatterShape();

        m_key = int32(); m_keyF = float32();
        m_minimum = int32(); m_minimumF = float32();
        m_lowerQuartile = int32(); m_lowerQuartileF = float32();
        m_median = int32(); m_medianF = float32();
        m_upperQuartile = int32(); m_upperQuartileF = float32();
        m_maximum = int32(); m_maximumF = float32();
    }

    virtual ILNodeType type() const
    {
        return NT_WINDOW_OSCILLOSCOPE_PLOT_BOX;
    }

    bool init(ILOscilloscope *parent, const char *name,
              bool waitForResponse = false)
    {
        bool result = (!m_enabled) && parent && parent->enabled() && name;

        if(result)
        {
            construct(parent);

            if(!(result = sendPacket(OSCILLOSCOPE_NEW_BOX,
            name, waitForResponse)))
            {
                deconstruct();
            }
        }

        return result;
    }

    bool removeData(bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BOX_DATA,
        waitForResponse);
    }

    // Adders /////////////////////////////////////////////////////////////////

    bool addIndexValue(int32 index, int32 value,
                       bool waitForResponse = false)
    {
        BoxIndexValue indexValue;

        indexValue.index = index;
        indexValue.value = value;

        return addIndexValue(indexValue, waitForResponse);
    }

    bool addIndexValue(const BoxIndexValue &indexValue,
                       bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_BOX_DATA_0,
        reinterpret_cast<const int32 *>(&indexValue),
        sizeof(BoxIndexValue) / sizeof(int32),
        waitForResponse);
    }

    bool addIndexValues(const BoxIndexValue *indexValues, uint16 size,
                        bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_BOX_DATA_0_A,
        reinterpret_cast<const int32 *>(indexValues), size,
        sizeof(BoxIndexValue) / sizeof(int32),
        waitForResponse);
    }

    bool addIndexValueF(float32 index, float32 value,
                        bool waitForResponse = false)
    {
        BoxIndexValueF indexValue;

        indexValue.index = index;
        indexValue.value = value;

        return addIndexValueF(indexValue, waitForResponse);
    }

    bool addIndexValueF(const BoxIndexValueF &indexValue,
                        bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_BOX_DATA_0_F,
        reinterpret_cast<const float32 *>(&indexValue),
        sizeof(BoxIndexValueF) / sizeof(float32),
        waitForResponse);
    }

    bool addIndexValuesF(const BoxIndexValueF *indexValues, uint16 size,
                         bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_BOX_DATA_0_F_A,
        reinterpret_cast<const float32 *>(indexValues), size,
        sizeof(BoxIndexValueF) / sizeof(float32),
        waitForResponse);
    }

    bool addValue(int32 value,
                  bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_BOX_DATA_1,
        value, waitForResponse);
    }

    bool addValues(const int32 *values, uint16 size,
                   bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_BOX_DATA_1_A,
        values, size, sizeof(int32) / sizeof(int32),
        waitForResponse);
    }

    bool addValueF(float32 value,
                   bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_BOX_DATA_1_F,
        value, waitForResponse);
    }

    bool addValuesF(const float32 *values, uint16 size,
                    bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_BOX_DATA_1_F_A,
        values, size, sizeof(float32) / sizeof(float32),
        waitForResponse);
    }

    // Removers ///////////////////////////////////////////////////////////////

    bool removeDataBefore(int32 before,
                          bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BOX_DATA_BEFORE,
        before, waitForResponse);
    }

    bool removeDataBeforeF(float32 before,
                           bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BOX_DATA_BEFORE_F,
        before, waitForResponse);
    }

    bool removeDataAfter(int32 after,
                         bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BOX_DATA_AFTER,
        after, waitForResponse);
    }

    bool removeDataAfterF(float32 after,
                          bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BOX_DATA_AFTER_F,
        after, waitForResponse);
    }

    bool removeDataBetween(int32 from, int32 to,
                           bool waitForResponse = false)
    {
        BoxRange range; range.from = from; range.to = to;

        return removeDataBetween(range, waitForResponse);
    }

    bool removeDataBetween(const BoxRange &range,
                           bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BOX_DATA_BETWEEN,
        reinterpret_cast<const int32 *>(&range),
        sizeof(BoxRange) / sizeof(int32),
        waitForResponse);
    }

    bool removeDataBetween(const BoxRange *ranges, uint16 size,
                           bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BOX_DATA_BETWEEN_A,
        reinterpret_cast<const int32 *>(ranges), size,
        sizeof(BoxRange) / sizeof(int32),
        waitForResponse);
    }

    bool removeDataBetweenF(float32 from, float32 to,
                            bool waitForResponse = false)
    {
        BoxRangeF range; range.from = from; range.to = to;

        return removeDataBetweenF(range, waitForResponse);
    }

    bool removeDataBetweenF(const BoxRangeF &range,
                            bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BOX_DATA_BETWEEN_F,
        reinterpret_cast<const float32 *>(&range),
        sizeof(BoxRangeF) / sizeof(float32),
        waitForResponse);
    }

    bool removeDataBetweenF(const BoxRangeF *ranges, uint16 size,
                            bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BOX_DATA_BETWEEN_F_A,
        reinterpret_cast<const float32 *>(ranges), size,
        sizeof(BoxRangeF) / sizeof(float32),
        waitForResponse);
    }

    bool removeDatum(int32 datum,
                     bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BOX_DATUM,
        datum, waitForResponse);
    }

    bool removeData(const int32 *data, uint16 size,
                    bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BOX_DATUM_A,
        data, size, sizeof(int32) / sizeof(int32),
        waitForResponse);
    }

    bool removeDatumF(float32 datum,
                      bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BOX_DATUM_F,
        datum, waitForResponse);
    }

    bool removeDataF(const float32 *data, uint16 size,
                     bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BOX_DATUM_F_A,
        data, size, sizeof(float32) / sizeof(float32),
        waitForResponse);
    }

    // Properties /////////////////////////////////////////////////////////////

    bool setName(const char *name,
                 bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_SET_BOX_NAME,
        name, waitForResponse);
    }

    bool getName(char *name,
                 uint16 size)
    {
        return receivePacket(OSCILLOSCOPE_GET_BOX_NAME,
        name, size);
    }

    bool setCoordinateSystem(CoordinateSystem coordinateSystem,
                             bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_BOX_COORDINATE_SYSTEM,
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
            receivePacket(OSCILLOSCOPE_GET_BOX_COORDINATE_SYSTEM,
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
        sendPacket(OSCILLOSCOPE_SET_BOX_ANGLE_UNITS,
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
            receivePacket(OSCILLOSCOPE_GET_BOX_ANGLE_UNITS,
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
        sendPacket(OSCILLOSCOPE_SET_BOX_LINE_COLOR,
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
            receivePacket(OSCILLOSCOPE_GET_BOX_LINE_COLOR,
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
        sendPacket(OSCILLOSCOPE_SET_BOX_LINE_STYLE,
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
            receivePacket(OSCILLOSCOPE_GET_BOX_LINE_STYLE,
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
        sendPacket(OSCILLOSCOPE_SET_BOX_FILL_COLOR,
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
            receivePacket(OSCILLOSCOPE_GET_BOX_FILL_COLOR,
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
        sendPacket(OSCILLOSCOPE_SET_BOX_FILL_STYLE,
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
            receivePacket(OSCILLOSCOPE_GET_BOX_FILL_STYLE,
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
        sendPacket(OSCILLOSCOPE_SET_BOX_DELTA_MODE,
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
            receivePacket(OSCILLOSCOPE_GET_BOX_DELTA_MODE,
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
        sendPacket(OSCILLOSCOPE_SET_BOX_SIZE,
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
            receivePacket(OSCILLOSCOPE_GET_BOX_SIZE,
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
        sendPacket(OSCILLOSCOPE_SET_BOX_SAMPLE_RATE,
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
            receivePacket(OSCILLOSCOPE_GET_BOX_SAMPLE_RATE,
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
        sendPacket(OSCILLOSCOPE_SET_BOX_SAMPLE_RATE_F,
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
            receivePacket(OSCILLOSCOPE_GET_BOX_SAMPLE_RATE_F,
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
        sendPacket(OSCILLOSCOPE_SET_BOX_SCALER,
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
            receivePacket(OSCILLOSCOPE_GET_BOX_SCALER,
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
        sendPacket(OSCILLOSCOPE_SET_BOX_SCALER_F,
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
            receivePacket(OSCILLOSCOPE_GET_BOX_SCALER_F,
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

    bool setWidth(int32 width,
                  bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_BOX_WIDTH,
        int32(width), waitForResponse)))
        {
            m_width = width;
        }

        return result;
    }

    int32 getWidth(bool bypassCache = false,
                   bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_BOX_WIDTH,
            &response)))
            {
               m_width = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_width : int32();
    }

    bool setWidthF(float32 widthF,
                   bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_BOX_WIDTH_F,
        float32(widthF), waitForResponse)))
        {
            m_widthF = widthF;
        }

        return result;
    }

    float32 getWidthF(bool bypassCache = false,
                      bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_BOX_WIDTH_F,
            &response)))
            {
               m_widthF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_widthF : float32();
    }

//    bool setWidthF(int32 widthF,
//                   bool waitForResponse = false)
//    {
//        bool result = setWidth(widthF, waitForResponse);

//        if(result)
//        {
//            m_widthF = m_width;
//        }

//        return result;
//    }

//    bool setWidth(float32 width,
//                  bool waitForResponse = false)
//    {
//        bool result = setWidthF(width, waitForResponse);

//        if(result)
//        {
//            m_width = m_widthF;
//        }

//        return result;
//    }

    bool setScatterShape(ScatterShape scatterShape,
                         bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_BOX_SCATTER_STYLE,
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
            receivePacket(OSCILLOSCOPE_GET_BOX_SCATTER_STYLE,
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

    bool setKey(int32 key,
                bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_BOX_KEY,
        int32(key), waitForResponse)))
        {
            m_key = key;
        }

        return result;
    }

    int32 getKey(bool bypassCache = false,
                 bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_BOX_KEY,
            &response)))
            {
               m_key = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_key : int32();
    }

    bool setKeyF(float32 keyF,
                 bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_BOX_KEY_F,
        float32(keyF), waitForResponse)))
        {
            m_keyF = keyF;
        }

        return result;
    }

    float32 getKeyF(bool bypassCache = false,
                    bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_BOX_KEY_F,
            &response)))
            {
               m_keyF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_keyF : float32();
    }

//    bool setKeyF(int32 keyF,
//                 bool waitForResponse = false)
//    {
//        bool result = setKey(keyF, waitForResponse);

//        if(result)
//        {
//            m_keyF = m_key;
//        }

//        return result;
//    }

//    bool setKey(float32 key,
//                bool waitForResponse = false)
//    {
//        bool result = setKeyF(key, waitForResponse);

//        if(result)
//        {
//            m_key = m_keyF;
//        }

//        return result;
//    }

    bool setMinimum(int32 minimum,
                    bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_BOX_MINIMUM,
        int32(minimum), waitForResponse)))
        {
            m_minimum = minimum;
        }

        return result;
    }

    int32 getMinimum(bool bypassCache = false,
                     bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_BOX_MINIMUM,
            &response)))
            {
               m_minimum = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_minimum : int32();
    }

    bool setMinimumF(float32 minimumF,
                     bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_BOX_MINIMUM_F,
        float32(minimumF), waitForResponse)))
        {
            m_minimumF = minimumF;
        }

        return result;
    }

    float32 getMinimumF(bool bypassCache = false,
                        bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_BOX_MINIMUM_F,
            &response)))
            {
               m_minimumF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_minimumF : float32();
    }

//    bool setMinimumF(int32 minimumF,
//                     bool waitForResponse = false)
//    {
//        bool result = setMinimum(minimumF, waitForResponse);

//        if(result)
//        {
//            m_minimumF = m_minimum;
//        }

//        return result;
//    }

//    bool setMinimum(float32 minimum,
//                    bool waitForResponse = false)
//    {
//        bool result = setMinimumF(minimum, waitForResponse);

//        if(result)
//        {
//            m_minimum = m_minimumF;
//        }

//        return result;
//    }

    bool setLowerQuartile(int32 lowerQuartile,
                          bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_BOX_LOWER_QUARTILE,
        int32(lowerQuartile), waitForResponse)))
        {
            m_lowerQuartile = lowerQuartile;
        }

        return result;
    }

    int32 getLowerQuartile(bool bypassCache = false,
                           bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_BOX_LOWER_QUARTILE,
            &response)))
            {
               m_lowerQuartile = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_lowerQuartile : int32();
    }

    bool setLowerQuartileF(float32 lowerQuartileF,
                           bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_BOX_LOWER_QUARTILE_F,
        float32(lowerQuartileF), waitForResponse)))
        {
            m_lowerQuartileF = lowerQuartileF;
        }

        return result;
    }

    float32 getLowerQuartileF(bool bypassCache = false,
                              bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_BOX_LOWER_QUARTILE_F,
            &response)))
            {
               m_lowerQuartileF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_lowerQuartileF : float32();
    }

//    bool setLowerQuartileF(int32 lowerQuartileF,
//                           bool waitForResponse = false)
//    {
//        bool result = setLowerQuartile(lowerQuartileF, waitForResponse);

//        if(result)
//        {
//            m_lowerQuartileF = m_lowerQuartile;
//        }

//        return result;
//    }

//    bool setLowerQuartile(float32 lowerQuartile,
//                          bool waitForResponse = false)
//    {
//        bool result = setLowerQuartileF(lowerQuartile, waitForResponse);

//        if(result)
//        {
//            m_lowerQuartile = m_lowerQuartileF;
//        }

//        return result;
//    }

    bool setMedian(int32 median,
                   bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_BOX_MEDIAN,
        int32(median), waitForResponse)))
        {
            m_median = median;
        }

        return result;
    }

    int32 getMedian(bool bypassCache = false,
                    bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_BOX_MEDIAN,
            &response)))
            {
               m_median = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_median : int32();
    }

    bool setMedianF(float32 medianF,
                    bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_BOX_MEDIAN_F,
        float32(medianF), waitForResponse)))
        {
            m_medianF = medianF;
        }

        return result;
    }

    float32 getMedianF(bool bypassCache = false,
                       bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_BOX_MEDIAN_F,
            &response)))
            {
               m_medianF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_medianF : float32();
    }

//    bool setMedianF(int32 medianF,
//                    bool waitForResponse = false)
//    {
//        bool result = setMedian(medianF, waitForResponse);

//        if(result)
//        {
//            m_medianF = m_median;
//        }

//        return result;
//    }

//    bool setMedian(float32 median,
//                   bool waitForResponse = false)
//    {
//        bool result = setMedianF(median, waitForResponse);

//        if(result)
//        {
//            m_median = m_medianF;
//        }

//        return result;
//    }

    bool setUpperQuartile(int32 upperQuartile,
                          bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_BOX_UPPER_QUARTILE,
        int32(upperQuartile), waitForResponse)))
        {
            m_upperQuartile = upperQuartile;
        }

        return result;
    }

    int32 getUpperQuartile(bool bypassCache = false,
                           bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_BOX_UPPER_QUARTILE,
            &response)))
            {
               m_upperQuartile = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_upperQuartile : int32();
    }

    bool setUpperQuartileF(float32 upperQuartileF,
                           bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_BOX_UPPER_QUARTILE_F,
        float32(upperQuartileF), waitForResponse)))
        {
            m_upperQuartileF = upperQuartileF;
        }

        return result;
    }

    float32 getUpperQuartileF(bool bypassCache = false,
                              bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_BOX_UPPER_QUARTILE_F,
            &response)))
            {
               m_upperQuartileF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_upperQuartileF : float32();
    }

//    bool setUpperQuartileF(int32 upperQuartileF,
//                           bool waitForResponse = false)
//    {
//        bool result = setUpperQuartile(upperQuartileF, waitForResponse);

//        if(result)
//        {
//            m_upperQuartileF = m_upperQuartile;
//        }

//        return result;
//    }

//    bool setUpperQuartile(float32 upperQuartile,
//                          bool waitForResponse = false)
//    {
//        bool result = setUpperQuartileF(upperQuartile, waitForResponse);

//        if(result)
//        {
//            m_upperQuartile = m_upperQuartileF;
//        }

//        return result;
//    }

    bool setMaximum(int32 maximum,
                    bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_BOX_MAXIMUM,
        int32(maximum), waitForResponse)))
        {
            m_maximum = maximum;
        }

        return result;
    }

    int32 getMaximum(bool bypassCache = false,
                     bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_BOX_MAXIMUM,
            &response)))
            {
               m_maximum = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_maximum : int32();
    }

    bool setMaximumF(float32 maximumF,
                     bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_BOX_MAXIMUM_F,
        float32(maximumF), waitForResponse)))
        {
            m_maximumF = maximumF;
        }

        return result;
    }

    float32 getMaximumF(bool bypassCache = false,
                        bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_BOX_MAXIMUM_F,
            &response)))
            {
               m_maximumF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_maximumF : float32();
    }

//    bool setMaximumF(int32 maximumF,
//                     bool waitForResponse = false)
//    {
//        bool result = setMaximum(maximumF, waitForResponse);

//        if(result)
//        {
//            m_maximumF = m_maximum;
//        }

//        return result;
//    }

//    bool setMaximum(float32 maximum,
//                    bool waitForResponse = false)
//    {
//        bool result = setMaximumF(maximum, waitForResponse);

//        if(result)
//        {
//            m_maximum = m_maximumF;
//        }

//        return result;
//    }

protected:

    virtual void deconstruct()
    {
        ILOscilloscopePlot::deconstruct();
    }

private:

    ILOscilloscopeBox(const ILOscilloscopeBox &);
    ILOscilloscopeBox &operator=(const ILOscilloscopeBox &);

    CoordinateSystem m_coordinateSystem; AngleUnits m_angleUnits;

    argb m_lineColor; LineStyle m_lineStyle;
    argb m_fillColor; FillStyle m_fillStyle;

    bool m_deltaMode; uint32 m_size;
    int32 m_sampleRate; float32 m_sampleRateF;
    int32 m_scaler; float32 m_scalerF;

    int32 m_width; float32 m_widthF;

    ScatterShape m_scatterShape;

    int32 m_key; float32 m_keyF;
    int32 m_minimum; float32 m_minimumF;
    int32 m_lowerQuartile; float32 m_lowerQuartileF;
    int32 m_median; float32 m_medianF;
    int32 m_upperQuartile; float32 m_upperQuartileF;
    int32 m_maximum; float32 m_maximumF;
};

#endif // ILOSCILLOSCOPEBOX_H

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
