/***************************************************************************//**
* @file
* Interface Library Oscilloscope Curve
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

#ifndef ILOSCILLOSCOPECURVE_H
#define ILOSCILLOSCOPECURVE_H

#include "iloscilloscope.h"

typedef struct CurveIndexKeyValue
{
    int32 index, key, value;
}
CurveIndexKeyValue;

typedef struct CurveIndexKeyValueF
{
    float32 index, key, value;
}
CurveIndexKeyValueF;

typedef struct CurveKeyValue
{
    int32 key, value;
}
CurveKeyValue;

typedef struct CurveKeyValueF
{
    float32 key, value;
}
CurveKeyValueF;

typedef struct CurveRange
{
    int32 from, to;
}
CurveRange;

typedef struct CurveRangeF
{
    float32 from, to;
}
CurveRangeF;

class ILOscilloscopeCurve : public ILOscilloscopePlot
{

public:

    ILOscilloscopeCurve() : ILOscilloscopePlot()
    {
        m_coordinateSystem = CoordinateSystem(); m_angleUnits = AngleUnits();

        m_lineColor = argb(); m_lineStyle = LineStyle();
        m_fillColor = argb(); m_fillStyle = FillStyle();

        m_deltaMode = bool(); m_size = uint32();
        m_sampleRate = int32(); m_sampleRateF = float32();
        m_scaler = int32(); m_scalerF = float32();

        m_fft = CurveFFT(); m_fftwf = CurveFFTWF();

        m_scatterShape = ScatterShape(); m_curveStyle = CurveLineStyle();

        m_channelFillCurve = 0;
        m_cFCFriend = 0;
    }

    virtual ILNodeType type() const
    {
        return NT_WINDOW_OSCILLOSCOPE_PLOT_CURVE;
    }

    bool init(ILOscilloscope *parent, const char *name,
              bool waitForResponse = false)
    {
        bool result = (!m_enabled) && parent && parent->enabled() && name;

        if(result)
        {
            construct(parent);

            if(!(result = sendPacket(OSCILLOSCOPE_NEW_CURVE,
            name, waitForResponse)))
            {
                deconstruct();
            }
        }

        return result;
    }

    bool removeData(bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_CURVE_DATA,
        waitForResponse);
    }

    // Adders /////////////////////////////////////////////////////////////////

    bool addIndexKeyValue(int32 index, int32 key, int32 value,
                          bool waitForResponse = false)
    {
        CurveIndexKeyValue indexKeyValue;

        indexKeyValue.index = index;
        indexKeyValue.key = key;
        indexKeyValue.value = value;

        return addIndexKeyValue(indexKeyValue, waitForResponse);
    }

    bool addIndexKeyValue(const CurveIndexKeyValue &indexKeyValue,
                          bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_CURVE_DATA_0,
        reinterpret_cast<const int32 *>(&indexKeyValue),
        sizeof(CurveIndexKeyValue) / sizeof(int32),
        waitForResponse);
    }

    bool addIndexKeyValues(const CurveIndexKeyValue *indexKeyValue,
                           uint16 size, bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_CURVE_DATA_0_A,
        reinterpret_cast<const int32 *>(indexKeyValue), size,
        sizeof(CurveIndexKeyValue) / sizeof(int32),
        waitForResponse);
    }

    bool addIndexKeyValueF(float32 index, float32 key, float32 value,
                           bool waitForResponse = false)
    {
        CurveIndexKeyValueF indexKeyValue;

        indexKeyValue.index = index;
        indexKeyValue.key = key;
        indexKeyValue.value = value;

        return addIndexKeyValueF(indexKeyValue, waitForResponse);
    }

    bool addIndexKeyValueF(const CurveIndexKeyValueF &indexKeyValue,
                           bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_CURVE_DATA_0_F,
        reinterpret_cast<const float32 *>(&indexKeyValue),
        sizeof(CurveIndexKeyValueF) / sizeof(float32),
        waitForResponse);
    }

    bool addIndexKeyValuesF(const CurveIndexKeyValueF *indexKeyValue,
                            uint16 size, bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_CURVE_DATA_0_F_A,
        reinterpret_cast<const float32 *>(indexKeyValue), size,
        sizeof(CurveIndexKeyValueF) / sizeof(float32),
        waitForResponse);
    }

    bool addKeyValue(int32 key, int32 value,
                     bool waitForResponse = false)
    {
        CurveKeyValue keyValue; keyValue.key = key; keyValue.value = value;

        return addKeyValue(keyValue, waitForResponse);
    }

    bool addKeyValue(const CurveKeyValue &keyValue,
                     bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_CURVE_DATA_1,
        reinterpret_cast<const int32 *>(&keyValue),
        sizeof(CurveKeyValue) / sizeof(int32),
        waitForResponse);
    }

    bool addKeyValues(const CurveKeyValue *keyValues, uint16 size,
                      bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_CURVE_DATA_1_A,
        reinterpret_cast<const int32 *>(keyValues), size,
        sizeof(CurveKeyValue) / sizeof(int32),
        waitForResponse);
    }

    bool addKeyValueF(float32 key, float32 value,
                      bool waitForResponse = false)
    {
        CurveKeyValueF keyValue; keyValue.key = key; keyValue.value = value;

        return addKeyValueF(keyValue, waitForResponse);
    }

    bool addKeyValueF(const CurveKeyValueF &keyValue,
                      bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_CURVE_DATA_1_F,
        reinterpret_cast<const float32 *>(&keyValue),
        sizeof(CurveKeyValueF) / sizeof(float32),
        waitForResponse);
    }

    bool addKeyValuesF(const CurveKeyValueF *keyValues, uint16 size,
                       bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_ADD_CURVE_DATA_1_F_A,
        reinterpret_cast<const float32 *>(keyValues), size,
        sizeof(CurveKeyValueF) / sizeof(float32),
        waitForResponse);
    }

    // Removers ///////////////////////////////////////////////////////////////

    bool removeDataBefore(int32 before,
                          bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_CURVE_DATA_BEFORE,
        before, waitForResponse);
    }

    bool removeDataBeforeF(float32 before,
                           bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_CURVE_DATA_BEFORE_F,
        before, waitForResponse);
    }

    bool removeDataAfter(int32 after,
                         bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_CURVE_DATA_AFTER,
        after, waitForResponse);
    }

    bool removeDataAfterF(float32 after,
                          bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_CURVE_DATA_AFTER_F,
        after, waitForResponse);
    }

    bool removeDataBetween(int32 from, int32 to,
                           bool waitForResponse = false)
    {
        CurveRange range; range.from = from; range.to = to;

        return removeDataBetween(range, waitForResponse);
    }

    bool removeDataBetween(const CurveRange &range,
                           bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_CURVE_DATA_BETWEEN,
        reinterpret_cast<const int32 *>(&range),
        sizeof(CurveRange) / sizeof(int32),
        waitForResponse);
    }

    bool removeDataBetween(const CurveRange *ranges, uint16 size,
                           bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_CURVE_DATA_BETWEEN_A,
        reinterpret_cast<const int32 *>(ranges), size,
        sizeof(CurveRange) / sizeof(int32),
        waitForResponse);
    }

    bool removeDataBetweenF(float32 from, float32 to,
                            bool waitForResponse = false)
    {
        CurveRangeF range; range.from = from; range.to = to;

        return removeDataBetweenF(range, waitForResponse);
    }

    bool removeDataBetweenF(const CurveRangeF &range,
                            bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_CURVE_DATA_BETWEEN_F,
        reinterpret_cast<const float32 *>(&range),
        sizeof(CurveRangeF) / sizeof(float32),
        waitForResponse);
    }

    bool removeDataBetweenF(const CurveRangeF *ranges, uint16 size,
                            bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_CURVE_DATA_BETWEEN_F_A,
        reinterpret_cast<const float32 *>(ranges), size,
        sizeof(CurveRangeF) / sizeof(float32),
        waitForResponse);
    }

    bool removeDatum(int32 datum,
                     bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_CURVE_DATUM,
        datum, waitForResponse);
    }

    bool removeData(const int32 *data, uint16 size,
                    bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_CURVE_DATUM_A,
        data, size, sizeof(int32) / sizeof(int32),
        waitForResponse);
    }

    bool removeDatumF(float32 datum,
                      bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_CURVE_DATUM_F,
        datum, waitForResponse);
    }

    bool removeDataF(const float32 *data, uint16 size,
                     bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_CURVE_DATUM_F_A,
        data, size, sizeof(float32) / sizeof(float32),
        waitForResponse);
    }

    // Properties /////////////////////////////////////////////////////////////

    bool setName(const char *name,
                 bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_SET_CURVE_NAME,
        name, waitForResponse);
    }

    bool getName(char *name,
                 uint16 size)
    {
        return receivePacket(OSCILLOSCOPE_GET_CURVE_NAME,
        name, size);
    }

    bool setCoordinateSystem(CoordinateSystem coordinateSystem,
                             bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_CURVE_COORDINATE_SYSTEM,
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
            receivePacket(OSCILLOSCOPE_GET_CURVE_COORDINATE_SYSTEM,
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
        sendPacket(OSCILLOSCOPE_SET_CURVE_ANGLE_UNITS,
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
            receivePacket(OSCILLOSCOPE_GET_CURVE_ANGLE_UNITS,
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
        sendPacket(OSCILLOSCOPE_SET_CURVE_LINE_COLOR,
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
            receivePacket(OSCILLOSCOPE_GET_CURVE_LINE_COLOR,
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
        sendPacket(OSCILLOSCOPE_SET_CURVE_LINE_STYLE,
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
            receivePacket(OSCILLOSCOPE_GET_CURVE_LINE_STYLE,
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
        sendPacket(OSCILLOSCOPE_SET_CURVE_FILL_COLOR,
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
            receivePacket(OSCILLOSCOPE_GET_CURVE_FILL_COLOR,
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
        sendPacket(OSCILLOSCOPE_SET_CURVE_FILL_STYLE,
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
            receivePacket(OSCILLOSCOPE_GET_CURVE_FILL_STYLE,
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
        sendPacket(OSCILLOSCOPE_SET_CURVE_DELTA_MODE,
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
            receivePacket(OSCILLOSCOPE_GET_CURVE_DELTA_MODE,
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
        sendPacket(OSCILLOSCOPE_SET_CURVE_SIZE,
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
            receivePacket(OSCILLOSCOPE_GET_CURVE_SIZE,
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
        sendPacket(OSCILLOSCOPE_SET_CURVE_SAMPLE_RATE,
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
            receivePacket(OSCILLOSCOPE_GET_CURVE_SAMPLE_RATE,
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
        sendPacket(OSCILLOSCOPE_SET_CURVE_SAMPLE_RATE_F,
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
            receivePacket(OSCILLOSCOPE_GET_CURVE_SAMPLE_RATE_F,
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
        sendPacket(OSCILLOSCOPE_SET_CURVE_SCALER,
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
            receivePacket(OSCILLOSCOPE_GET_CURVE_SCALER,
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
        sendPacket(OSCILLOSCOPE_SET_CURVE_SCALER_F,
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
            receivePacket(OSCILLOSCOPE_GET_CURVE_SCALER_F,
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

    bool setFFT(CurveFFT fft,
                bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_CURVE_FFT,
        int32(fft), waitForResponse)))
        {
            m_fft = fft;
        }

        return result;
    }

    CurveFFT getFFT(bool bypassCache = false,
                    bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_CURVE_FFT,
            &response)))
            {
               m_fft = CurveFFT(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_fft : CurveFFT();
    }

    bool setFFTWF(CurveFFTWF fftwf,
                  bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_CURVE_FFT_WINDOW,
        int32(fftwf), waitForResponse)))
        {
            m_fftwf = fftwf;
        }

        return result;
    }

    CurveFFTWF getFFTWF(bool bypassCache = false,
                        bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_CURVE_FFT_WINDOW,
            &response)))
            {
               m_fftwf = CurveFFTWF(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_fftwf : CurveFFTWF();
    }

    bool setScatterShape(ScatterShape scatterShape,
                         bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_CURVE_SCATTER_STYLE,
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
            receivePacket(OSCILLOSCOPE_GET_CURVE_SCATTER_STYLE,
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

    bool setCurveStyle(CurveLineStyle curveStyle,
                       bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_CURVE_STYLE,
        int32(curveStyle), waitForResponse)))
        {
            m_curveStyle = curveStyle;
        }

        return result;
    }

    CurveLineStyle getCurveStyle(bool bypassCache = false,
                             bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_CURVE_STYLE,
            &response)))
            {
               m_curveStyle = CurveLineStyle(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_curveStyle : CurveLineStyle();
    }

    bool setChannelFillCurve(ILOscilloscopeCurve *curve,
                             bool waitForResponse = false)
    {
        bool result = curve && curve->enabled() &&
                      (curve->parent() == m_parent);

        if(result)
        {
            if((result = sendPacket(OSCILLOSCOPE_SET_CURVE_CHANNEL_FILL_CURVE,
            curve->handle(), waitForResponse)))
            {
                m_channelFillCurve = curve; curve->m_cFCFriend = this;
            }
        }

        return result;
    }

    ILOscilloscopeCurve *getChannelFillCurve(bool bypassCache = false,
                                             bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_CURVE_CHANNEL_FILL_CURVE,
            &response)))
            {
               m_channelFillCurve =
               static_cast<ILOscilloscopeCurve *>(findSibling(response));
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_channelFillCurve : 0;
    }

protected:

    virtual void deconstruct()
    {
        if(m_cFCFriend)
        {
            m_cFCFriend->m_channelFillCurve = 0;
        }

        ILOscilloscopePlot::deconstruct();
    }

private:

    ILOscilloscopeCurve(const ILOscilloscopeCurve &);
    ILOscilloscopeCurve &operator=(const ILOscilloscopeCurve &);

    CoordinateSystem m_coordinateSystem; AngleUnits m_angleUnits;

    argb m_lineColor; LineStyle m_lineStyle;
    argb m_fillColor; FillStyle m_fillStyle;

    bool m_deltaMode; uint32 m_size;
    int32 m_sampleRate; float32 m_sampleRateF;
    int32 m_scaler; float32 m_scalerF;

    CurveFFT m_fft; CurveFFTWF m_fftwf;

    ScatterShape m_scatterShape; CurveLineStyle m_curveStyle;

    ILOscilloscopeCurve *m_channelFillCurve;
    ILOscilloscopeCurve *m_cFCFriend;
};

#endif // ILOSCILLOSCOPECURVE_H

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
