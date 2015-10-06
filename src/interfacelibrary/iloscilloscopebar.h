/***************************************************************************//**
* @file
* Interface Library Oscilloscope Bar
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

#ifndef ILOSCILLOSCOPEBAR_H
#define ILOSCILLOSCOPEBAR_H

#include "iloscilloscope.h"

typedef struct BarKeyValue
{
    int32 key, value;
}
BarKeyValue;

typedef struct BarKeyValueF
{
    float32 key, value;
}
BarKeyValueF;

typedef struct BarRange
{
    int32 from, to;
}
BarRange;

typedef struct BarRangeF
{
    float32 from, to;
}
BarRangeF;

class ILOscilloscopeBar : public ILOscilloscopePlot
{

public:

    ILOscilloscopeBar() : ILOscilloscopePlot()
    {
        m_coordinateSystem = CoordinateSystem(); m_angleUnits = AngleUnits();

        m_lineColor = argb(); m_lineStyle = LineStyle();
        m_fillColor = argb(); m_fillStyle = FillStyle();

        m_deltaMode = bool(); m_size = uint32();
        m_sampleRate = int32(); m_sampleRateF = float32();
        m_scaler = int32(); m_scalerF = float32();

        m_width = int32(); m_widthF = float32();

        m_scatterShape = ScatterShape();

        m_barBelow = 0; m_barAbove = 0;
        m_bBFriend = 0; m_bAFriend = 0;
    }

    virtual ILNodeType type() const
    {
        return NT_WINDOW_OSCILLOSCOPE_PLOT_BAR;
    }

    bool init(ILOscilloscope *parent, const char *name,
              bool waitForResponse = false)
    {
        bool result = (!m_enabled) && parent && parent->enabled() && name;

        if(result)
        {
            construct(parent);

            if(!(result = sendPacket(OSCILLOSCOPE_NEW_BAR,
            name, waitForResponse)))
            {
                deconstruct();
            }
        }

        return result;
    }

    bool removeData(bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BAR_DATA,
        waitForResponse);
    }

    // Adders /////////////////////////////////////////////////////////////////

    bool addKeyValue(int32 key, int32 value,
                     bool waitForResponse = false)
    {
        BarKeyValue keyValue; keyValue.key = key; keyValue.value = value;

        return addKeyValue(keyValue, waitForResponse);
    }

    bool addKeyValue(const BarKeyValue &keyValue,
                     bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_BAR_DATA_0,
        reinterpret_cast<const int32 *>(&keyValue),
        sizeof(BarKeyValue) / sizeof(int32),
        waitForResponse);
    }

    bool addKeyValues(const BarKeyValue *keyValues, uint16 size,
                      bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_BAR_DATA_0_A,
        reinterpret_cast<const int32 *>(keyValues), size,
        sizeof(BarKeyValue) / sizeof(int32),
        waitForResponse);
    }

    bool addKeyValueF(float32 key, float32 value,
                      bool waitForResponse = false)
    {
        BarKeyValueF keyValue; keyValue.key = key; keyValue.value = value;

        return addKeyValueF(keyValue, waitForResponse);
    }

    bool addKeyValueF(const BarKeyValueF &keyValue,
                      bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_BAR_DATA_0_F,
        reinterpret_cast<const float32 *>(&keyValue),
        sizeof(BarKeyValueF) / sizeof(float32),
        waitForResponse);
    }

    bool addKeyValuesF(const BarKeyValueF *keyValues, uint16 size,
                       bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_BAR_DATA_0_F_A,
        reinterpret_cast<const float32 *>(keyValues), size,
        sizeof(BarKeyValueF) / sizeof(float32),
        waitForResponse);
    }

    bool addValue(int32 value,
                  bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_BAR_DATA_1,
        value, waitForResponse);
    }

    bool addValues(const int32 *values, uint16 size,
                   bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_BAR_DATA_1_A,
        values, size, sizeof(int32) / sizeof(int32),
        waitForResponse);
    }

    bool addValueF(float32 value,
                   bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_BAR_DATA_1_F,
        value, waitForResponse);
    }

    bool addValuesF(const float32 *values, uint16 size,
                    bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_BAR_DATA_1_F_A,
        values, size, sizeof(float32) / sizeof(float32),
        waitForResponse);
    }

    // Removers ///////////////////////////////////////////////////////////////

    bool removeDataBefore(int32 before,
                          bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BAR_DATA_BEFORE,
        before, waitForResponse);
    }

    bool removeDataBeforeF(float32 before,
                           bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BAR_DATA_BEFORE_F,
        before, waitForResponse);
    }

    bool removeDataAfter(int32 after,
                         bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BAR_DATA_AFTER,
        after, waitForResponse);
    }

    bool removeDataAfterF(float32 after,
                          bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BAR_DATA_AFTER_F,
        after, waitForResponse);
    }

    bool removeDataBetween(int32 from, int32 to,
                           bool waitForResponse = false)
    {
        BarRange range; range.from = from; range.to = to;

        return removeDataBetween(range, waitForResponse);
    }

    bool removeDataBetween(const BarRange &range,
                           bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BAR_DATA_BETWEEN,
        reinterpret_cast<const int32 *>(&range),
        sizeof(BarRange) / sizeof(int32),
        waitForResponse);
    }

    bool removeDataBetween(const BarRange *ranges, uint16 size,
                           bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BAR_DATA_BETWEEN_A,
        reinterpret_cast<const int32 *>(ranges), size,
        sizeof(BarRange) / sizeof(int32),
        waitForResponse);
    }

    bool removeDataBetweenF(float32 from, float32 to,
                            bool waitForResponse = false)
    {
        BarRangeF range; range.from = from; range.to = to;

        return removeDataBetweenF(range, waitForResponse);
    }

    bool removeDataBetweenF(const BarRangeF &range,
                            bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BAR_DATA_BETWEEN_F,
        reinterpret_cast<const float32 *>(&range),
        sizeof(BarRangeF) / sizeof(float32),
        waitForResponse);
    }

    bool removeDataBetweenF(const BarRangeF *ranges, uint16 size,
                            bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BAR_DATA_BETWEEN_F_A,
        reinterpret_cast<const float32 *>(ranges), size,
        sizeof(BarRangeF) / sizeof(float32),
        waitForResponse);
    }

    bool removeDatum(int32 datum,
                     bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BAR_DATUM,
        datum, waitForResponse);
    }

    bool removeData(const int32 *data, uint16 size,
                    bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BAR_DATUM_A,
        data, size, sizeof(int32) / sizeof(int32),
        waitForResponse);
    }

    bool removeDatumF(float32 datum,
                      bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BAR_DATUM_F,
        datum, waitForResponse);
    }

    bool removeDataF(const float32 *data, uint16 size,
                     bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_BAR_DATUM_F_A,
        data, size, sizeof(float32) / sizeof(float32),
        waitForResponse);
    }

    // Properties /////////////////////////////////////////////////////////////

    bool setName(const char *name,
                 bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_SET_BAR_NAME,
        name, waitForResponse);
    }

    bool getName(char *name,
                 uint16 size)
    {
        return receivePacket(OSCILLOSCOPE_GET_BAR_NAME,
        name, size);
    }

    bool setCoordinateSystem(CoordinateSystem coordinateSystem,
                             bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_BAR_COORDINATE_SYSTEM,
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
            receivePacket(OSCILLOSCOPE_GET_BAR_COORDINATE_SYSTEM,
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
        sendPacket(OSCILLOSCOPE_SET_BAR_ANGLE_UNITS,
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
            receivePacket(OSCILLOSCOPE_GET_BAR_ANGLE_UNITS,
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
        sendPacket(OSCILLOSCOPE_SET_BAR_LINE_COLOR,
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
            receivePacket(OSCILLOSCOPE_GET_BAR_LINE_COLOR,
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
        sendPacket(OSCILLOSCOPE_SET_BAR_LINE_STYLE,
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
            receivePacket(OSCILLOSCOPE_GET_BAR_LINE_STYLE,
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
        sendPacket(OSCILLOSCOPE_SET_BAR_FILL_COLOR,
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
            receivePacket(OSCILLOSCOPE_GET_BAR_FILL_COLOR,
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
        sendPacket(OSCILLOSCOPE_SET_BAR_FILL_STYLE,
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
            receivePacket(OSCILLOSCOPE_GET_BAR_FILL_STYLE,
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
        sendPacket(OSCILLOSCOPE_SET_BAR_DELTA_MODE,
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
            receivePacket(OSCILLOSCOPE_GET_BAR_DELTA_MODE,
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
        sendPacket(OSCILLOSCOPE_SET_BAR_SIZE,
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
            receivePacket(OSCILLOSCOPE_GET_BAR_SIZE,
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
        sendPacket(OSCILLOSCOPE_SET_BAR_SAMPLE_RATE,
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
            receivePacket(OSCILLOSCOPE_GET_BAR_SAMPLE_RATE,
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
        sendPacket(OSCILLOSCOPE_SET_BAR_SAMPLE_RATE_F,
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
            receivePacket(OSCILLOSCOPE_GET_BAR_SAMPLE_RATE_F,
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
        sendPacket(OSCILLOSCOPE_SET_BAR_SCALER,
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
            receivePacket(OSCILLOSCOPE_GET_BAR_SCALER,
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
        sendPacket(OSCILLOSCOPE_SET_BAR_SCALER_F,
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
            receivePacket(OSCILLOSCOPE_GET_BAR_SCALER_F,
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
        sendPacket(OSCILLOSCOPE_SET_BAR_WIDTH,
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
            receivePacket(OSCILLOSCOPE_GET_BAR_WIDTH,
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
        sendPacket(OSCILLOSCOPE_SET_BAR_WIDTH_F,
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
            receivePacket(OSCILLOSCOPE_GET_BAR_WIDTH_F,
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
        sendPacket(OSCILLOSCOPE_SET_BAR_SCATTER_STYLE,
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
            receivePacket(OSCILLOSCOPE_GET_BAR_SCATTER_STYLE,
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

    bool setBarBelow(ILOscilloscopeBar *barBelow,
                     bool waitForResponse = false)
    {
        bool result = barBelow && barBelow->enabled() &&
                      (barBelow->parent() == m_parent);

        if(result)
        {
            if((result = sendPacket(OSCILLOSCOPE_SET_BAR_BELOW,
            barBelow->handle(), waitForResponse)))
            {
                m_barBelow = barBelow; barBelow->m_bBFriend = this;
            }
        }

        return result;
    }

    ILOscilloscopeBar *getBarBelow(bool bypassCache = false,
                                   bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_BAR_BELOW,
            &response)))
            {
               m_barBelow =
               static_cast<ILOscilloscopeBar *>(findSibling(response));
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_barBelow : 0;
    }

    bool setBarAbove(ILOscilloscopeBar *barAbove,
                     bool waitForResponse = false)
    {
        bool result = barAbove && barAbove->enabled() &&
                      (barAbove->parent() == m_parent);

        if(result)
        {
            if((result = sendPacket(OSCILLOSCOPE_SET_BAR_ABOVE,
            barAbove->handle(), waitForResponse)))
            {
                m_barAbove = barAbove; barAbove->m_bAFriend = this;
            }
        }

        return result;
    }

    ILOscilloscopeBar *getBarAbove(bool bypassCache = false,
                                   bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_BAR_ABOVE,
            &response)))
            {
               m_barAbove =
               static_cast<ILOscilloscopeBar *>(findSibling(response));
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_barAbove : 0;
    }

protected:

    virtual void deconstruct()
    {
        if(m_bBFriend)
        {
            m_bBFriend->m_barBelow = 0;
        }

        if(m_bAFriend)
        {
            m_bAFriend->m_barAbove = 0;
        }

        ILOscilloscopePlot::deconstruct();
    }

private:

    ILOscilloscopeBar(const ILOscilloscopeBar &);
    ILOscilloscopeBar &operator=(const ILOscilloscopeBar &);

    CoordinateSystem m_coordinateSystem; AngleUnits m_angleUnits;

    argb m_lineColor; LineStyle m_lineStyle;
    argb m_fillColor; FillStyle m_fillStyle;

    bool m_deltaMode; uint32 m_size;
    int32 m_sampleRate; float32 m_sampleRateF;
    int32 m_scaler; float32 m_scalerF;

    int32 m_width; float32 m_widthF;

    ScatterShape m_scatterShape;

    ILOscilloscopeBar *m_barBelow; ILOscilloscopeBar *m_barAbove;
    ILOscilloscopeBar *m_bBFriend; ILOscilloscopeBar *m_bAFriend;
};

#endif // ILOSCILLOSCOPEBAR_H

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
