/***************************************************************************//**
* @file
* Interface Library Oscilloscope
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

#ifndef ILOSCILLOSCOPE_H
#define ILOSCILLOSCOPE_H

#include "ilcore.h"

class ILOscilloscopePlot : public ILNode
{

public:

    ILOscilloscopePlot() : ILNode()
    {
        // UNUSED //
    }

protected:

    virtual uint16 handlesSize() const
    {
        return 2;
    }

    virtual void construct(ILNode *parent)
    {
        if(parent && parent->parent())
        {
            ILNode::construct(parent);

            m_handles[0] = parent->handle();
            m_handles[1] = m_handle;
        }
    }

private:

    ILOscilloscopePlot(const ILOscilloscopePlot &);
    ILOscilloscopePlot &operator=(const ILOscilloscopePlot &);

    friend class ILOscilloscope;
};

class ILOscilloscope : public ILWindow
{

public:

    ILOscilloscope() : ILWindow()
    {
        m_backgroundColor = argb();

        m_XAxisScaleType = ScaleType();
        m_XAxisScaleLogBase = int32(); m_XAxisScaleLogBaseF = float32();
        m_XAxisRangeAutoScale = true;
        m_XAxisRangeLower = int32(); m_XAxisRangeLowerF = float32();
        m_XAxisRangeUpper = int32(); m_XAxisRangeUpperF = float32();
        m_XAxisRangeReversed = bool();

        m_YAxisScaleType = ScaleType();
        m_YAxisScaleLogBase = int32(); m_YAxisScaleLogBaseF = float32();
        m_YAxisRangeAutoScale = true;
        m_YAxisRangeLower = int32(); m_YAxisRangeLowerF = float32();
        m_YAxisRangeUpper = int32(); m_YAxisRangeUpperF = float32();
        m_YAxisRangeReversed = bool();

        m_FFTXAxisScaleType = ScaleType();
        m_FFTXAxisScaleLogBase = int32(); m_FFTXAxisScaleLogBaseF = float32();
        m_FFTXAxisRangeAutoScale = true;
        m_FFTXAxisRangeLower = int32(); m_FFTXAxisRangeLowerF = float32();
        m_FFTXAxisRangeUpper = int32(); m_FFTXAxisRangeUpperF = float32();
        m_FFTXAxisRangeReversed = bool();

        m_FFTYAxisScaleType = ScaleType();
        m_FFTYAxisScaleLogBase = int32(); m_FFTYAxisScaleLogBaseF = float32();
        m_FFTYAxisRangeAutoScale = true;
        m_FFTYAxisRangeLower = int32(); m_FFTYAxisRangeLowerF = float32();
        m_FFTYAxisRangeUpper = int32(); m_FFTYAxisRangeUpperF = float32();
        m_FFTYAxisRangeReversed = bool();
    }

    virtual ILNodeType type() const
    {
        return NT_WINDOW_OSCILLOSCOPE;
    }

    bool initUndocked(ILCore *parent, const char *name,
                      bool waitForResponse = false)
    {
        bool result = (!m_enabled) && parent && parent->enabled() && name;

        if(result)
        {
            construct(parent);

            if(!(result = sendPacket(OSCILLOSCOPE_NEW_UNDOCKED_WINDOW,
            name, waitForResponse)))
            {
                deconstruct();
            }
        }

        return result;
    }

    bool initDocked(ILCore *parent, const char *name,
                    bool waitForResponse = false)
    {
        return init(parent, name, waitForResponse);
    }

//    bool initDocked(ILWindow *parent, const char *name,
//                    bool waitForResponse = false)
//    {
//        bool result = (!m_enabled) && parent && parent->enabled() && name;

//        if(result)
//        {
//            construct(parent->parent());

//            if(!(result = sendPacket(OSCILLOSCOPE_NEW_DOCKED_WINDOW,
//            parent->handle(), name, waitForResponse)))
//            {
//                deconstruct();
//            }
//        }

//        return result;
//    }

    bool init(ILCore *parent, const char *name,
              bool waitForResponse = false)
    {
        bool result = (!m_enabled) && parent && parent->enabled() && name;

        if(result)
        {
            construct(parent);

            if(!(result = sendPacket(OSCILLOSCOPE_NEW_MAIN_DOCKED_WINDOW,
            name, waitForResponse)))
            {
                deconstruct();
            }
        }

        return result;
    }

    bool removeData(ILOscilloscopePlot *plot,
                    bool waitForResponse = false)
    {
        bool result = plot && plot->enabled() && (plot->parent() == this);

        if(result)
        {
            result = sendPacket(OSCILLOSCOPE_REMOVE_DATA,
            plot->handle(), waitForResponse);
        }

        return result;
    }

    bool removeDatas(bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_REMOVE_DATAS, waitForResponse);
    }

    bool deinitPlot(ILOscilloscopePlot *plot,
                    bool waitForResponse = false)
    {
        bool result = plot && plot->enabled() && (plot->parent() == this);

        if(result)
        {
            if((result = sendPacket(OSCILLOSCOPE_DELETE_PLOT,
            plot->handle(), waitForResponse)))
            {
                plot->deconstruct();
            }
        }

        return result;
    }

    bool deinitPlots(bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_DELETE_PLOTS,
        waitForResponse)))
        {
            deconstructChildren();
        }

        return result;
    }

    // Properties /////////////////////////////////////////////////////////////

    bool setBackgroundColor(argb backgroundColor,
                            bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_BACKGROUND_COLOR,
        int32(backgroundColor), waitForResponse)))
        {
            m_backgroundColor = backgroundColor;
        }

        return result;
    }

    argb getBackgroundColor(bool bypassCache = false,
                            bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_BACKGROUND_COLOR,
            &response)))
            {
               m_backgroundColor = argb(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_backgroundColor : argb();
    }

    // X Axis /////////////////////////////////////////////////////////////////

    bool setXAxisScaleType(ScaleType scaleType,
                           bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_XAXIS_SCALE_TYPE,
        int32(scaleType), waitForResponse)))
        {
            m_XAxisScaleType = scaleType;
        }

        return result;
    }

    ScaleType getXAxisScaleType(bool bypassCache = false,
                                bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_XAXIS_SCALE_TYPE,
            &response)))
            {
               m_XAxisScaleType = ScaleType(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_XAxisScaleType : ScaleType();
    }

    bool setXAxisScaleLogBase(int32 scaleLogBase,
                              bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_XAXIS_SCALE_LOG_BASE,
        int32(scaleLogBase), waitForResponse)))
        {
            m_XAxisScaleLogBase = scaleLogBase;
        }

        return result;
    }

    int32 getXAxisScaleLogBase(bool bypassCache = false,
                               bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_XAXIS_SCALE_LOG_BASE,
            &response)))
            {
               m_XAxisScaleLogBase = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_XAxisScaleLogBase : int32();
    }

    bool setXAxisScaleLogBaseF(float32 scaleLogBase,
                               bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_XAXIS_SCALE_LOG_BASE_F,
        float32(scaleLogBase), waitForResponse)))
        {
            m_XAxisScaleLogBaseF = scaleLogBase;
        }

        return result;
    }

    float32 getXAxisScaleLogBaseF(bool bypassCache = false,
                                  bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_XAXIS_SCALE_LOG_BASE_F,
            &response)))
            {
               m_XAxisScaleLogBaseF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_XAxisScaleLogBaseF : float32();
    }

//    bool setXAxisScaleLogBaseF(int32 scaleLogBase,
//                               bool waitForResponse = false)
//    {
//        bool result = setXAxisScaleLogBase(scaleLogBase, waitForResponse);

//        if(result)
//        {
//            m_XAxisScaleLogBaseF = m_XAxisScaleLogBase;
//        }

//        return result;
//    }

//    bool setXAxisScaleLogBase(float32 scaleLogBase,
//                              bool waitForResponse = false)
//    {
//        bool result = setXAxisScaleLogBaseF(scaleLogBase, waitForResponse);

//        if(result)
//        {
//            m_XAxisScaleLogBase = m_XAxisScaleLogBaseF;
//        }

//        return result;
//    }

    bool setXAxisRangeAutoScale(bool on,
                                bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_XAXIS_RANGE_AUTO_SCALE,
        int32(on), waitForResponse)))
        {
            m_XAxisRangeAutoScale = on;
        }

        return result;
    }

    bool getXAxisRangeAutoScale(bool bypassCache = false,
                                bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_XAXIS_RANGE_AUTO_SCALE,
            &response)))
            {
               m_XAxisRangeAutoScale = bool(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_XAxisRangeAutoScale : bool();
    }

    bool setXAxisRangeLower(int32 rangeLower,
                            bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_XAXIS_RANGE_LOWER,
        int32(rangeLower), waitForResponse)))
        {
            m_XAxisRangeLower = rangeLower;
        }

        return result;
    }

    int32 getXAxisRangeLower(bool bypassCache = false,
                             bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_XAXIS_RANGE_LOWER,
            &response)))
            {
               m_XAxisRangeLower = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_XAxisRangeLower : int32();
    }

    bool setXAxisRangeLowerF(float32 rangeLower,
                             bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_XAXIS_RANGE_LOWER_F,
        float32(rangeLower), waitForResponse)))
        {
            m_XAxisRangeLowerF = rangeLower;
        }

        return result;
    }

    float32 getXAxisRangeLowerF(bool bypassCache = false,
                                bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_XAXIS_RANGE_LOWER_F,
            &response)))
            {
               m_XAxisRangeLowerF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_XAxisRangeLowerF : float32();
    }

//    bool setXAxisRangeLowerF(int32 rangeLower,
//                             bool waitForResponse = false)
//    {
//        bool result = setXAxisRangeLower(rangeLower, waitForResponse);

//        if(result)
//        {
//            m_XAxisRangeLowerF = m_XAxisRangeLower;
//        }

//        return result;
//    }

//    bool setXAxisRangeLower(float32 rangeLower,
//                            bool waitForResponse = false)
//    {
//        bool result = setXAxisRangeLowerF(rangeLower, waitForResponse);

//        if(result)
//        {
//            m_XAxisRangeLower = m_XAxisRangeLowerF;
//        }

//        return result;
//    }

    bool setXAxisRangeUpper(int32 rangeUpper,
                            bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_XAXIS_RANGE_UPPER,
        int32(rangeUpper), waitForResponse)))
        {
            m_XAxisRangeUpper = rangeUpper;
        }

        return result;
    }

    int32 getXAxisRangeUpper(bool bypassCache = false,
                             bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_XAXIS_RANGE_UPPER,
            &response)))
            {
               m_XAxisRangeUpper = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_XAxisRangeUpper : int32();
    }

    bool setXAxisRangeUpperF(float32 rangeUpper,
                             bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_XAXIS_RANGE_UPPER_F,
        float32(rangeUpper), waitForResponse)))
        {
            m_XAxisRangeUpperF = rangeUpper;
        }

        return result;
    }

    float32 getXAxisRangeUpperF(bool bypassCache = false,
                                bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_XAXIS_RANGE_UPPER_F,
            &response)))
            {
               m_XAxisRangeUpperF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_XAxisRangeUpperF : float32();
    }

//    bool setXAxisRangeUpperF(int32 rangeUpper,
//                             bool waitForResponse = false)
//    {
//        bool result = setXAxisRangeUpper(rangeUpper, waitForResponse);

//        if(result)
//        {
//            m_XAxisRangeUpperF = m_XAxisRangeUpper;
//        }

//        return result;
//    }

//    bool setXAxisRangeUpper(float32 rangeUpper,
//                            bool waitForResponse = false)
//    {
//        bool result = setXAxisRangeUpperF(rangeUpper, waitForResponse);

//        if(result)
//        {
//            m_XAxisRangeUpper = m_XAxisRangeUpperF;
//        }

//        return result;
//    }

    bool setXAxisRangeReversed(bool rangeReversed,
                               bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_XAXIS_RANGE_REVERSED,
        int32(rangeReversed), waitForResponse)))
        {
            m_XAxisRangeReversed = rangeReversed;
        }

        return result;
    }

    bool getXAxisRangeReversed(bool bypassCache = false,
                               bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_XAXIS_RANGE_REVERSED,
            &response)))
            {
               m_XAxisRangeReversed = bool(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_XAxisRangeReversed : bool();
    }

    bool setXAxisLabel(const char *label,
                       bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_SET_XAXIS_LABEL,
        label, waitForResponse);
    }

    bool getXAxisLabel(char *label,
                       uint16 size)
    {
        return receivePacket(OSCILLOSCOPE_GET_XAXIS_LABEL,
        label, size);
    }

    // Y Axis /////////////////////////////////////////////////////////////////

    bool setYAxisScaleType(ScaleType scaleType,
                           bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_YAXIS_SCALE_TYPE,
        int32(scaleType), waitForResponse)))
        {
            m_YAxisScaleType = scaleType;
        }

        return result;
    }

    ScaleType getYAxisScaleType(bool bypassCache = false,
                                bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_YAXIS_SCALE_TYPE,
            &response)))
            {
               m_YAxisScaleType = ScaleType(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_YAxisScaleType : ScaleType();
    }

    bool setYAxisScaleLogBase(int32 scaleLogBase,
                              bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_YAXIS_SCALE_LOG_BASE,
        int32(scaleLogBase), waitForResponse)))
        {
            m_YAxisScaleLogBase = scaleLogBase;
        }

        return result;
    }

    int32 getYAxisScaleLogBase(bool bypassCache = false,
                               bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_YAXIS_SCALE_LOG_BASE,
            &response)))
            {
               m_YAxisScaleLogBase = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_YAxisScaleLogBase : int32();
    }

    bool setYAxisScaleLogBaseF(float32 scaleLogBase,
                               bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_YAXIS_SCALE_LOG_BASE_F,
        float32(scaleLogBase), waitForResponse)))
        {
            m_YAxisScaleLogBaseF = scaleLogBase;
        }

        return result;
    }

    float32 getYAxisScaleLogBaseF(bool bypassCache = false,
                                  bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_YAXIS_SCALE_LOG_BASE_F,
            &response)))
            {
               m_YAxisScaleLogBaseF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_YAxisScaleLogBaseF : float32();
    }

//    bool setYAxisScaleLogBaseF(int32 scaleLogBase,
//                               bool waitForResponse = false)
//    {
//        bool result = setYAxisScaleLogBase(scaleLogBase, waitForResponse);

//        if(result)
//        {
//            m_YAxisScaleLogBaseF = m_YAxisScaleLogBase;
//        }

//        return result;
//    }

//    bool setYAxisScaleLogBase(float32 scaleLogBase,
//                              bool waitForResponse = false)
//    {
//        bool result = setYAxisScaleLogBaseF(scaleLogBase, waitForResponse);

//        if(result)
//        {
//            m_YAxisScaleLogBase = m_YAxisScaleLogBaseF;
//        }

//        return result;
//    }

    bool setYAxisRangeAutoScale(bool on,
                                bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_YAXIS_RANGE_AUTO_SCALE,
        int32(on), waitForResponse)))
        {
            m_YAxisRangeAutoScale = on;
        }

        return result;
    }

    bool getYAxisRangeAutoScale(bool bypassCache = false,
                                bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_YAXIS_RANGE_AUTO_SCALE,
            &response)))
            {
               m_YAxisRangeAutoScale = bool(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_YAxisRangeAutoScale : bool();
    }

    bool setYAxisRangeLower(int32 rangeLower,
                            bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_YAXIS_RANGE_LOWER,
        int32(rangeLower), waitForResponse)))
        {
            m_YAxisRangeLower = rangeLower;
        }

        return result;
    }

    int32 getYAxisRangeLower(bool bypassCache = false,
                             bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_YAXIS_RANGE_LOWER,
            &response)))
            {
               m_YAxisRangeLower = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_YAxisRangeLower : int32();
    }

    bool setYAxisRangeLowerF(float32 rangeLower,
                             bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_YAXIS_RANGE_LOWER_F,
        float32(rangeLower), waitForResponse)))
        {
            m_YAxisRangeLowerF = rangeLower;
        }

        return result;
    }

    float32 getYAxisRangeLowerF(bool bypassCache = false,
                                bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_YAXIS_RANGE_LOWER_F,
            &response)))
            {
               m_YAxisRangeLowerF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_YAxisRangeLowerF : float32();
    }

//    bool setYAxisRangeLowerF(int32 rangeLower,
//                             bool waitForResponse = false)
//    {
//        bool result = setYAxisRangeLower(rangeLower, waitForResponse);

//        if(result)
//        {
//            m_YAxisRangeLowerF = m_YAxisRangeLower;
//        }

//        return result;
//    }

//    bool setYAxisRangeLower(float32 rangeLower,
//                            bool waitForResponse = false)
//    {
//        bool result = setYAxisRangeLowerF(rangeLower, waitForResponse);

//        if(result)
//        {
//            m_YAxisRangeLower = m_YAxisRangeLowerF;
//        }

//        return result;
//    }

    bool setYAxisRangeUpper(int32 rangeUpper,
                            bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_YAXIS_RANGE_UPPER,
        int32(rangeUpper), waitForResponse)))
        {
            m_YAxisRangeUpper = rangeUpper;
        }

        return result;
    }

    int32 getYAxisRangeUpper(bool bypassCache = false,
                             bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_YAXIS_RANGE_UPPER,
            &response)))
            {
               m_YAxisRangeUpper = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_YAxisRangeUpper : int32();
    }

    bool setYAxisRangeUpperF(float32 rangeUpper,
                             bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_YAXIS_RANGE_UPPER_F,
        float32(rangeUpper), waitForResponse)))
        {
            m_YAxisRangeUpperF = rangeUpper;
        }

        return result;
    }

    float32 getYAxisRangeUpperF(bool bypassCache = false,
                                bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_YAXIS_RANGE_UPPER_F,
            &response)))
            {
               m_YAxisRangeUpperF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_YAxisRangeUpperF : float32();
    }

//    bool setYAxisRangeUpperF(int32 rangeUpper,
//                             bool waitForResponse = false)
//    {
//        bool result = setYAxisRangeUpper(rangeUpper, waitForResponse);

//        if(result)
//        {
//            m_YAxisRangeUpperF = m_YAxisRangeUpper;
//        }

//        return result;
//    }

//    bool setYAxisRangeUpper(float32 rangeUpper,
//                            bool waitForResponse = false)
//    {
//        bool result = setYAxisRangeUpperF(rangeUpper, waitForResponse);

//        if(result)
//        {
//            m_YAxisRangeUpper = m_YAxisRangeUpperF;
//        }

//        return result;
//    }

    bool setYAxisRangeReversed(bool rangeReversed,
                               bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_YAXIS_RANGE_REVERSED,
        int32(rangeReversed), waitForResponse)))
        {
            m_YAxisRangeReversed = rangeReversed;
        }

        return result;
    }

    bool getYAxisRangeReversed(bool bypassCache = false,
                               bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_YAXIS_RANGE_REVERSED,
            &response)))
            {
               m_YAxisRangeReversed = bool(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_YAxisRangeReversed : bool();
    }

    bool setYAxisLabel(const char *label,
                       bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_SET_YAXIS_LABEL,
        label, waitForResponse);
    }

    bool getYAxisLabel(char *label,
                       uint16 size)
    {
        return receivePacket(OSCILLOSCOPE_GET_YAXIS_LABEL,
        label, size);
    }

    // FFT X Axis /////////////////////////////////////////////////////////////

    bool setFFTXAxisScaleType(ScaleType scaleType,
                              bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_FFTXAXIS_SCALE_TYPE,
        int32(scaleType), waitForResponse)))
        {
            m_FFTXAxisScaleType = scaleType;
        }

        return result;
    }

    ScaleType getFFTXAxisScaleType(bool bypassCache = false,
                                   bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_FFTXAXIS_SCALE_TYPE,
            &response)))
            {
               m_FFTXAxisScaleType = ScaleType(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_FFTXAxisScaleType : ScaleType();
    }

    bool setFFTXAxisScaleLogBase(int32 scaleLogBase,
                                 bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_FFTXAXIS_SCALE_LOG_BASE,
        int32(scaleLogBase), waitForResponse)))
        {
            m_FFTXAxisScaleLogBase = scaleLogBase;
        }

        return result;
    }

    int32 getFFTXAxisScaleLogBase(bool bypassCache = false,
                                  bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_FFTXAXIS_SCALE_LOG_BASE,
            &response)))
            {
               m_FFTXAxisScaleLogBase = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_FFTXAxisScaleLogBase : int32();
    }

    bool setFFTXAxisScaleLogBaseF(float32 scaleLogBase,
                                  bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_FFTXAXIS_SCALE_LOG_BASE_F,
        float32(scaleLogBase), waitForResponse)))
        {
            m_FFTXAxisScaleLogBaseF = scaleLogBase;
        }

        return result;
    }

    float32 getFFTXAxisScaleLogBaseF(bool bypassCache = false,
                                     bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_FFTXAXIS_SCALE_LOG_BASE_F,
            &response)))
            {
               m_FFTXAxisScaleLogBaseF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_FFTXAxisScaleLogBaseF : float32();
    }

//    bool setFFTXAxisScaleLogBaseF(int32 scaleLogBase,
//                                  bool waitForResponse = false)
//    {
//        bool result = setFFTXAxisScaleLogBase(scaleLogBase, waitForResponse);

//        if(result)
//        {
//            m_FFTXAxisScaleLogBaseF = m_FFTXAxisScaleLogBase;
//        }

//        return result;
//    }

//    bool setFFTXAxisScaleLogBase(float32 scaleLogBase,
//                                 bool waitForResponse = false)
//    {
//        bool result = setFFTXAxisScaleLogBaseF(scaleLogBase, waitForResponse);

//        if(result)
//        {
//            m_FFTXAxisScaleLogBase = m_FFTXAxisScaleLogBaseF;
//        }

//        return result;
//    }

    bool setFFTXAxisRangeAutoScale(bool on,
                                   bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_FFTXAXIS_RANGE_AUTO_SCALE,
        int32(on), waitForResponse)))
        {
            m_FFTXAxisRangeAutoScale = on;
        }

        return result;
    }

    bool getFFTXAxisRangeAutoScale(bool bypassCache = false,
                                   bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_FFTXAXIS_RANGE_AUTO_SCALE,
            &response)))
            {
               m_FFTXAxisRangeAutoScale = bool(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_FFTXAxisRangeAutoScale : bool();
    }

    bool setFFTXAxisRangeLower(int32 rangeLower,
                            bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_FFTXAXIS_RANGE_LOWER,
        int32(rangeLower), waitForResponse)))
        {
            m_FFTXAxisRangeLower = rangeLower;
        }

        return result;
    }

    int32 getFFTXAxisRangeLower(bool bypassCache = false,
                             bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_FFTXAXIS_RANGE_LOWER,
            &response)))
            {
               m_FFTXAxisRangeLower = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_FFTXAxisRangeLower : int32();
    }

    bool setFFTXAxisRangeLowerF(float32 rangeLower,
                             bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_FFTXAXIS_RANGE_LOWER_F,
        float32(rangeLower), waitForResponse)))
        {
            m_FFTXAxisRangeLowerF = rangeLower;
        }

        return result;
    }

    float32 getFFTXAxisRangeLowerF(bool bypassCache = false,
                                bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_FFTXAXIS_RANGE_LOWER_F,
            &response)))
            {
               m_FFTXAxisRangeLowerF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_FFTXAxisRangeLowerF : float32();
    }

//    bool setFFTXAxisRangeLowerF(int32 rangeLower,
//                             bool waitForResponse = false)
//    {
//        bool result = setFFTXAxisRangeLower(rangeLower, waitForResponse);

//        if(result)
//        {
//            m_FFTXAxisRangeLowerF = m_FFTXAxisRangeLower;
//        }

//        return result;
//    }

//    bool setFFTXAxisRangeLower(float32 rangeLower,
//                            bool waitForResponse = false)
//    {
//        bool result = setFFTXAxisRangeLowerF(rangeLower, waitForResponse);

//        if(result)
//        {
//            m_FFTXAxisRangeLower = m_FFTXAxisRangeLowerF;
//        }

//        return result;
//    }

    bool setFFTXAxisRangeUpper(int32 rangeUpper,
                            bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_FFTXAXIS_RANGE_UPPER,
        int32(rangeUpper), waitForResponse)))
        {
            m_FFTXAxisRangeUpper = rangeUpper;
        }

        return result;
    }

    int32 getFFTXAxisRangeUpper(bool bypassCache = false,
                             bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_FFTXAXIS_RANGE_UPPER,
            &response)))
            {
               m_FFTXAxisRangeUpper = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_FFTXAxisRangeUpper : int32();
    }

    bool setFFTXAxisRangeUpperF(float32 rangeUpper,
                             bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_FFTXAXIS_RANGE_UPPER_F,
        float32(rangeUpper), waitForResponse)))
        {
            m_FFTXAxisRangeUpperF = rangeUpper;
        }

        return result;
    }

    float32 getFFTXAxisRangeUpperF(bool bypassCache = false,
                                bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_FFTXAXIS_RANGE_UPPER_F,
            &response)))
            {
               m_FFTXAxisRangeUpperF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_FFTXAxisRangeUpperF : float32();
    }

//    bool setFFTXAxisRangeUpperF(int32 rangeUpper,
//                             bool waitForResponse = false)
//    {
//        bool result = setFFTXAxisRangeUpper(rangeUpper, waitForResponse);

//        if(result)
//        {
//            m_FFTXAxisRangeUpperF = m_FFTXAxisRangeUpper;
//        }

//        return result;
//    }

//    bool setFFTXAxisRangeUpper(float32 rangeUpper,
//                            bool waitForResponse = false)
//    {
//        bool result = setFFTXAxisRangeUpperF(rangeUpper, waitForResponse);

//        if(result)
//        {
//            m_FFTXAxisRangeUpper = m_FFTXAxisRangeUpperF;
//        }

//        return result;
//    }

    bool setFFTXAxisRangeReversed(bool rangeReversed,
                                  bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_FFTXAXIS_RANGE_REVERSED,
        int32(rangeReversed), waitForResponse)))
        {
            m_FFTXAxisRangeReversed = rangeReversed;
        }

        return result;
    }

    bool getFFTXAxisRangeReversed(bool bypassCache = false,
                                  bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_FFTXAXIS_RANGE_REVERSED,
            &response)))
            {
               m_FFTXAxisRangeReversed = bool(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_FFTXAxisRangeReversed : bool();
    }

    bool setFFTXAxisLabel(const char *label,
                          bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_SET_FFTXAXIS_LABEL,
        label, waitForResponse);
    }

    bool getFFTXAxisLabel(char *label,
                          uint16 size)
    {
        return receivePacket(OSCILLOSCOPE_GET_FFTXAXIS_LABEL,
        label, size);
    }

    // FFT Y Axis /////////////////////////////////////////////////////////////

    bool setFFTYAxisScaleType(ScaleType scaleType,
                              bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_FFTYAXIS_SCALE_TYPE,
        int32(scaleType), waitForResponse)))
        {
            m_FFTYAxisScaleType = scaleType;
        }

        return result;
    }

    ScaleType getFFTYAxisScaleType(bool bypassCache = false,
                                   bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_FFTYAXIS_SCALE_TYPE,
            &response)))
            {
               m_FFTYAxisScaleType = ScaleType(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_FFTYAxisScaleType : ScaleType();
    }

    bool setFFTYAxisScaleLogBase(int32 scaleLogBase,
                                 bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_FFTYAXIS_SCALE_LOG_BASE,
        int32(scaleLogBase), waitForResponse)))
        {
            m_FFTYAxisScaleLogBase = scaleLogBase;
        }

        return result;
    }

    int32 getFFTYAxisScaleLogBase(bool bypassCache = false,
                                  bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_FFTYAXIS_SCALE_LOG_BASE,
            &response)))
            {
               m_FFTYAxisScaleLogBase = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_FFTYAxisScaleLogBase : int32();
    }

    bool setFFTYAxisScaleLogBaseF(float32 scaleLogBase,
                                  bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_FFTYAXIS_SCALE_LOG_BASE_F,
        float32(scaleLogBase), waitForResponse)))
        {
            m_FFTYAxisScaleLogBaseF = scaleLogBase;
        }

        return result;
    }

    float32 getFFTYAxisScaleLogBaseF(bool bypassCache = false,
                                     bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_FFTYAXIS_SCALE_LOG_BASE_F,
            &response)))
            {
               m_FFTYAxisScaleLogBaseF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_FFTYAxisScaleLogBaseF : float32();
    }

//    bool setFFTYAxisScaleLogBaseF(int32 scaleLogBase,
//                                  bool waitForResponse = false)
//    {
//        bool result = setFFTYAxisScaleLogBase(scaleLogBase, waitForResponse);

//        if(result)
//        {
//            m_FFTYAxisScaleLogBaseF = m_FFTYAxisScaleLogBase;
//        }

//        return result;
//    }

//    bool setFFTYAxisScaleLogBase(float32 scaleLogBase,
//                                 bool waitForResponse = false)
//    {
//        bool result = setFFTYAxisScaleLogBaseF(scaleLogBase, waitForResponse);

//        if(result)
//        {
//            m_FFTYAxisScaleLogBase = m_FFTYAxisScaleLogBaseF;
//        }

//        return result;
//    }

    bool setFFTYAxisRangeAutoScale(bool on,
                                   bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_FFTYAXIS_RANGE_AUTO_SCALE,
        int32(on), waitForResponse)))
        {
            m_FFTYAxisRangeAutoScale = on;
        }

        return result;
    }

    bool getFFTYAxisRangeAutoScale(bool bypassCache = false,
                                   bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_FFTYAXIS_RANGE_AUTO_SCALE,
            &response)))
            {
               m_FFTYAxisRangeAutoScale = bool(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_FFTYAxisRangeAutoScale : bool();
    }

    bool setFFTYAxisRangeLower(int32 rangeLower,
                            bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_FFTYAXIS_RANGE_LOWER,
        int32(rangeLower), waitForResponse)))
        {
            m_FFTYAxisRangeLower = rangeLower;
        }

        return result;
    }

    int32 getFFTYAxisRangeLower(bool bypassCache = false,
                             bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_FFTYAXIS_RANGE_LOWER,
            &response)))
            {
               m_FFTYAxisRangeLower = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_FFTYAxisRangeLower : int32();
    }

    bool setFFTYAxisRangeLowerF(float32 rangeLower,
                             bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_FFTYAXIS_RANGE_LOWER_F,
        float32(rangeLower), waitForResponse)))
        {
            m_FFTYAxisRangeLowerF = rangeLower;
        }

        return result;
    }

    float32 getFFTYAxisRangeLowerF(bool bypassCache = false,
                                bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_FFTYAXIS_RANGE_LOWER_F,
            &response)))
            {
               m_FFTYAxisRangeLowerF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_FFTYAxisRangeLowerF : float32();
    }

//    bool setFFTYAxisRangeLowerF(int32 rangeLower,
//                             bool waitForResponse = false)
//    {
//        bool result = setFFTYAxisRangeLower(rangeLower, waitForResponse);

//        if(result)
//        {
//            m_FFTYAxisRangeLowerF = m_FFTYAxisRangeLower;
//        }

//        return result;
//    }

//    bool setFFTYAxisRangeLower(float32 rangeLower,
//                            bool waitForResponse = false)
//    {
//        bool result = setFFTYAxisRangeLowerF(rangeLower, waitForResponse);

//        if(result)
//        {
//            m_FFTYAxisRangeLower = m_FFTYAxisRangeLowerF;
//        }

//        return result;
//    }

    bool setFFTYAxisRangeUpper(int32 rangeUpper,
                            bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_FFTYAXIS_RANGE_UPPER,
        int32(rangeUpper), waitForResponse)))
        {
            m_FFTYAxisRangeUpper = rangeUpper;
        }

        return result;
    }

    int32 getFFTYAxisRangeUpper(bool bypassCache = false,
                             bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_FFTYAXIS_RANGE_UPPER,
            &response)))
            {
               m_FFTYAxisRangeUpper = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_FFTYAxisRangeUpper : int32();
    }

    bool setFFTYAxisRangeUpperF(float32 rangeUpper,
                             bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_FFTYAXIS_RANGE_UPPER_F,
        float32(rangeUpper), waitForResponse)))
        {
            m_FFTYAxisRangeUpperF = rangeUpper;
        }

        return result;
    }

    float32 getFFTYAxisRangeUpperF(bool bypassCache = false,
                                bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_FFTYAXIS_RANGE_UPPER_F,
            &response)))
            {
               m_FFTYAxisRangeUpperF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_FFTYAxisRangeUpperF : float32();
    }

//    bool setFFTYAxisRangeUpperF(int32 rangeUpper,
//                             bool waitForResponse = false)
//    {
//        bool result = setFFTYAxisRangeUpper(rangeUpper, waitForResponse);

//        if(result)
//        {
//            m_FFTYAxisRangeUpperF = m_FFTYAxisRangeUpper;
//        }

//        return result;
//    }

//    bool setFFTYAxisRangeUpper(float32 rangeUpper,
//                            bool waitForResponse = false)
//    {
//        bool result = setFFTYAxisRangeUpperF(rangeUpper, waitForResponse);

//        if(result)
//        {
//            m_FFTYAxisRangeUpper = m_FFTYAxisRangeUpperF;
//        }

//        return result;
//    }

    bool setFFTYAxisRangeReversed(bool rangeReversed,
                                  bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(OSCILLOSCOPE_SET_FFTYAXIS_RANGE_REVERSED,
        int32(rangeReversed), waitForResponse)))
        {
            m_FFTYAxisRangeReversed = rangeReversed;
        }

        return result;
    }

    bool getFFTYAxisRangeReversed(bool bypassCache = false,
                                  bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(OSCILLOSCOPE_GET_FFTYAXIS_RANGE_REVERSED,
            &response)))
            {
               m_FFTYAxisRangeReversed = bool(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_FFTYAxisRangeReversed : bool();
    }

    bool setFFTYAxisLabel(const char *label,
                          bool waitForResponse = false)
    {
        return sendPacket(OSCILLOSCOPE_SET_FFTYAXIS_LABEL,
        label, waitForResponse);
    }

    bool getFFTYAxisLabel(char *label,
                          uint16 size)
    {
        return receivePacket(OSCILLOSCOPE_GET_FFTYAXIS_LABEL,
        label, size);
    }

private:

    ILOscilloscope(const ILOscilloscope &);
    ILOscilloscope &operator=(const ILOscilloscope &);

    argb m_backgroundColor;

    ScaleType m_XAxisScaleType;
    int32 m_XAxisScaleLogBase; float32 m_XAxisScaleLogBaseF;
    bool m_XAxisRangeAutoScale;
    int32 m_XAxisRangeLower; float32 m_XAxisRangeLowerF;
    int32 m_XAxisRangeUpper; float32 m_XAxisRangeUpperF;
    bool m_XAxisRangeReversed;

    ScaleType m_YAxisScaleType;
    int32 m_YAxisScaleLogBase; float32 m_YAxisScaleLogBaseF;
    bool m_YAxisRangeAutoScale;
    int32 m_YAxisRangeLower; float32 m_YAxisRangeLowerF;
    int32 m_YAxisRangeUpper; float32 m_YAxisRangeUpperF;
    bool m_YAxisRangeReversed;

    ScaleType m_FFTXAxisScaleType;
    int32 m_FFTXAxisScaleLogBase; float32 m_FFTXAxisScaleLogBaseF;
    bool m_FFTXAxisRangeAutoScale;
    int32 m_FFTXAxisRangeLower; float32 m_FFTXAxisRangeLowerF;
    int32 m_FFTXAxisRangeUpper; float32 m_FFTXAxisRangeUpperF;
    bool m_FFTXAxisRangeReversed;

    ScaleType m_FFTYAxisScaleType;
    int32 m_FFTYAxisScaleLogBase; float32 m_FFTYAxisScaleLogBaseF;
    bool m_FFTYAxisRangeAutoScale;
    int32 m_FFTYAxisRangeLower; float32 m_FFTYAxisRangeLowerF;
    int32 m_FFTYAxisRangeUpper; float32 m_FFTYAxisRangeUpperF;
    bool m_FFTYAxisRangeReversed;
};

#endif // ILOSCILLOSCOPE_H

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
