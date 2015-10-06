/***************************************************************************//**
* @file
* Serial Oscilloscope
*
* @version @n 1.0
* @date @n 11/18/2013
*
* @author @n Kwabena W. Agyeman
* @copyright @n (c) 2013 Kwabena W. Agyeman
* @n All rights reserved - Please see the end of the file for the terms of use
*
* @par Update History:
* @n v1.0 - Original release - 11/18/2013
*******************************************************************************/

#ifndef SERIALOSCILLOSCOPE_H
#define SERIALOSCILLOSCOPE_H

#include <QtConcurrent>
#include <QtCore>
#include <QtGui>
#include <QtOpenGL>
#include <QtPrintSupport>
#include <QtSvg>
#include <QtWidgets>

#include "serialwindow.h"

#include "utilrasterdialog.h"
#include "utilvectordialog.h"
#include "utilvectordialogwowh.h"

#include "fftw3.h"
#include "qcustomplot.h"

#define OSCILLOSCOPE_DEF_DATA_SIZE 256
#define OSCILLOSCOPE_MAX_DATA_SIZE 65536

namespace Ui
{
    class SerialOscilloscope;
}

class MyCustomPlot : public QCustomPlot ///////////////////////////////////////
{
    Q_OBJECT

public:

    MyCustomPlot(QWidget *widget = NULL) : QCustomPlot(widget) { }

    // See Also
    //
    // bool QCustomPlot::addPlottable(QCPAbstractPlottable *plottable)

    bool insertPlottable(int index, QCPAbstractPlottable *plottable)
    {
        if(mPlottables.contains(plottable))
        {
            qDebug() <<
            Q_FUNC_INFO <<
            "plottable already added to this QCustomPlot:" <<
            reinterpret_cast<quintptr>(plottable);

            return false;
        }

        if(plottable->parentPlot() != this)
        {
            qDebug() <<
            Q_FUNC_INFO <<
            "plottable not created with this QCustomPlot as parent:" <<
            reinterpret_cast<quintptr>(plottable);

            return false;
        }

        mPlottables.insert(index, plottable);

        if(mAutoAddPlottableToLegend)
        {
            plottable->addToLegend();
        }

        if(qobject_cast<QCPGraph *>(plottable))
        {
            mGraphs.insert(index, static_cast<QCPGraph *>(plottable));
        }

        if(!plottable->layer())
        {
            plottable->setLayer(currentLayer());
        }

        return true;
    }

private:

    Q_DISABLE_COPY(MyCustomPlot) //////////////////////////////////////////////
};

class MyBars : public QCPBars /////////////////////////////////////////////////
{
    Q_OBJECT

public:

    MyBars(QCPAxis *key, QCPAxis *value) :
    QCPBars(key, value)
    {
        m_coordinateSystem = CS_CARTESIAN;
        m_angleUnits = AU_DEGREES;

        m_deltaMode = false;
        m_size = OSCILLOSCOPE_DEF_DATA_SIZE;

        m_sampleRate = 1.0;
        m_scaler = 1.0;
    }

    CoordinateSystem m_coordinateSystem;
    AngleUnits m_angleUnits;

    bool m_deltaMode;
    int m_size;

    double m_sampleRate;
    double m_scaler;

private:

    Q_DISABLE_COPY(MyBars) ////////////////////////////////////////////////////
};

class MyCurve : public QCPCurve ///////////////////////////////////////////////
{
    Q_OBJECT

public:

    MyCurve(QCPAxis *key, QCPAxis *value) :
    QCPCurve(key, value)
    {
        m_coordinateSystem = CS_CARTESIAN;
        m_angleUnits = AU_DEGREES;

        m_deltaMode = false;
        m_size = OSCILLOSCOPE_DEF_DATA_SIZE;

        m_sampleRate = 1.0;
        m_scaler = 1.0;

        m_FFTWOption = CFFT_NONE;
        m_FFTWFunction = CFFTWF_RECTANGLE;

        m_FFTWPlot = NULL;
    }

    CoordinateSystem m_coordinateSystem;
    AngleUnits m_angleUnits;

    bool m_deltaMode;
    int m_size;

    double m_sampleRate;
    double m_scaler;

    CurveFFT m_FFTWOption;
    CurveFFTWF m_FFTWFunction;

    QCPCurve *m_FFTWPlot;

private:

    Q_DISABLE_COPY(MyCurve) ///////////////////////////////////////////////////
};

class MyGraph : public QCPGraph ///////////////////////////////////////////////
{
    Q_OBJECT

public:

    MyGraph(QCPAxis *key, QCPAxis *value) :
    QCPGraph(key, value)
    {
        m_coordinateSystem = CS_CARTESIAN;
        m_angleUnits = AU_DEGREES;

        m_deltaMode = false;
        m_size = OSCILLOSCOPE_DEF_DATA_SIZE;

        m_sampleRate = 1.0;
        m_scaler = 1.0;

        m_FFTWOption = GFFT_NONE;
        m_FFTWFunction = GFFTWF_RECTANGLE;

        m_FFTWPlot = NULL;
    }

    CoordinateSystem m_coordinateSystem;
    AngleUnits m_angleUnits;

    bool m_deltaMode;
    int m_size;

    double m_sampleRate;
    double m_scaler;

    GraphFFT m_FFTWOption;
    GraphFFTWF m_FFTWFunction;

    QCPGraph *m_FFTWPlot;

private:

    Q_DISABLE_COPY(MyGraph) ///////////////////////////////////////////////////
};

class MyStatisticalBox : public QCPStatisticalBox /////////////////////////////
{
    Q_OBJECT

public:

    MyStatisticalBox(QCPAxis *key, QCPAxis *value) :
    QCPStatisticalBox(key, value)
    {
        m_coordinateSystem = CS_CARTESIAN;
        m_angleUnits = AU_DEGREES;

        m_deltaMode = false;
        m_size = OSCILLOSCOPE_DEF_DATA_SIZE;

        m_sampleRate = 1.0;
        m_scaler = 1.0;

        m_outs = QMap<double, double>();
    }

    void update()
    {
        mOutliers.clear();

        QMap<double, double>::const_iterator i = m_outs.constBegin();
        QMap<double, double>::const_iterator j = m_outs.constEnd();

        for(; i != j; i++)
        {
            mOutliers.append(i.value());
        }
    }

    CoordinateSystem m_coordinateSystem;
    AngleUnits m_angleUnits;

    bool m_deltaMode;
    int m_size;

    double m_sampleRate;
    double m_scaler;

    QMap<double, double> m_outs;

private:

    Q_DISABLE_COPY(MyStatisticalBox) //////////////////////////////////////////
};

class SerialOscilloscope : public SerialWindow
{
    Q_OBJECT

public:

    static void initFftw(QSettings *settings = NULL, QWidget *parent = NULL);
    static void finiFftw();

    explicit SerialOscilloscope(const QString &title,
                                QSettings *settings = NULL,
                                QWidget *parent = NULL);

    virtual ~SerialOscilloscope();

    virtual bool importExportEnabled() const;

    virtual QString keyGroup() const;
    virtual QString keyState() const;
    virtual QString keyGeometry() const;

    void setBackgroundColor(QRgb rgba);
    QRgb getBackgroundColor() const;

    ///////////////////////////////////////////////////////////////////////////

    bool setXAxisScaleType(QCPAxis::ScaleType scaleType);
    QCPAxis::ScaleType getXAxisScaleType() const;

    bool setXAxisScaleLogBase(double base);
    double getXAxisScaleLogBase() const;

    void setXAxisRangeAutoScale(bool on);
    bool getXAxisRangeAutoScale() const;

    bool setXAxisRangeLower(double lower);
    double getXAxisRangeLower() const;

    bool setXAxisRangeUpper(double upper);
    double getXAxisRangeUpper() const;

    void setXAxisRangeReversed(bool reversed);
    bool getXAxisRangeReversed() const;

    void setXAxisLabel(const QString &name);
    QString getXAxisLabel() const;

    ///////////////////////////////////////////////////////////////////////////

    bool setYAxisScaleType(QCPAxis::ScaleType scaleType);
    QCPAxis::ScaleType getYAxisScaleType() const;

    bool setYAxisScaleLogBase(double base);
    double getYAxisScaleLogBase() const;

    void setYAxisRangeAutoScale(bool on);
    bool getYAxisRangeAutoScale() const;

    bool setYAxisRangeLower(double lower);
    double getYAxisRangeLower() const;

    bool setYAxisRangeUpper(double upper);
    double getYAxisRangeUpper() const;

    void setYAxisRangeReversed(bool reversed);
    bool getYAxisRangeReversed() const;

    void setYAxisLabel(const QString &name);
    QString getYAxisLabel() const;

    ///////////////////////////////////////////////////////////////////////////

    bool setFFTXAxisScaleType(QCPAxis::ScaleType scaleType);
    QCPAxis::ScaleType getFFTXAxisScaleType() const;

    bool setFFTXAxisScaleLogBase(double base);
    double getFFTXAxisScaleLogBase() const;

    bool setFFTXAxisRangeAutoScale(bool on);
    bool getFFTXAxisRangeAutoScale() const;

    bool setFFTXAxisRangeLower(double lower);
    double getFFTXAxisRangeLower() const;

    bool setFFTXAxisRangeUpper(double upper);
    double getFFTXAxisRangeUpper() const;

    bool setFFTXAxisRangeReversed(bool reversed);
    bool getFFTXAxisRangeReversed() const;

    bool setFFTXAxisLabel(const QString &name);
    QString getFFTXAxisLabel() const;

    ///////////////////////////////////////////////////////////////////////////

    bool setFFTYAxisScaleType(QCPAxis::ScaleType scaleType);
    QCPAxis::ScaleType getFFTYAxisScaleType() const;

    bool setFFTYAxisScaleLogBase(double base);
    double getFFTYAxisScaleLogBase() const;

    bool setFFTYAxisRangeAutoScale(bool on);
    bool getFFTYAxisRangeAutoScale() const;

    bool setFFTYAxisRangeLower(double lower);
    double getFFTYAxisRangeLower() const;

    bool setFFTYAxisRangeUpper(double upper);
    double getFFTYAxisRangeUpper() const;

    bool setFFTYAxisRangeReversed(bool reversed);
    bool getFFTYAxisRangeReversed() const;

    bool setFFTYAxisLabel(const QString &name);
    QString getFFTYAxisLabel() const;

    ///////////////////////////////////////////////////////////////////////////

    bool newBar(int plottable, const QString &name);
    bool newCurve(int plottable, const QString &name);
    bool newGraph(int plottable, const QString &name);
    bool newBox(int plottable, const QString &name);

    ///////////////////////////////////////////////////////////////////////////

    bool setBarName(int plottable,
                    const QString &name);

    QString getBarName(int plottable,
                       bool *ok);

    bool setBarCoordinateSystem(int plottable,
                                CoordinateSystem coordinateSystem);

    CoordinateSystem getBarCoordinateSystem(int plottable,
                                            bool *ok);

    bool setBarAngleUnits(int plottable,
                          AngleUnits angleUnits);

    AngleUnits getBarAngleUnits(int plottable,
                                bool *ok);

    bool setBarLineColor(int plottable,
                         QRgb rgba);

    QRgb getBarLineColor(int plottable,
                         bool *ok);

    bool setBarLineStyle(int plottable,
                         Qt::PenStyle lineStyle);

    Qt::PenStyle getBarLineStyle(int plottable,
                                 bool *ok);

    bool setBarFillColor(int plottable,
                         QRgb rgba);

    QRgb getBarFillColor(int plottable,
                         bool *ok);

    bool setBarFillStyle(int plottable,
                         Qt::BrushStyle fillStyle);

    Qt::BrushStyle getBarFillStyle(int plottable,
                                   bool *ok);

    bool setBarDeltaMode(int plottable,
                         bool deltaMode);

    bool getBarDeltaMode(int plottable,
                         bool *ok);

    bool setBarSize(int plottable,
                    int size);

    int getBarSize(int plottable,
                   bool *ok);

    bool setBarSampleRate(int plottable,
                          double sampleRate);

    double getBarSampleRate(int plottable,
                            bool *ok);

    bool setBarScaler(int plottable,
                      double scaler);

    double getBarScaler(int plottable,
                        bool *ok);

    bool setBarWidth(int plottable,
                     double width);

    double getBarWidth(int plottable,
                       bool *ok);

    bool setBarScatterStyle(int plottable,
                            QCPScatterStyle::ScatterShape scatterShape);

    QCPScatterStyle::ScatterShape getBarScatterStyle(int plottable,
                                                     bool *ok);

    bool setBarBelow(int plottable,
                     int targetPlottable);

    int getBarBelow(int plottable,
                    bool *ok);

    bool setBarAbove(int plottable,
                     int targetPlottable);

    int getBarAbove(int plottable,
                    bool *ok);

    ///////////////////////////////////////////////////////////////////////////

    bool setCurveName(int plottable,
                      const QString &name);

    QString getCurveName(int plottable,
                         bool *ok);

    bool setCurveCoordinateSystem(int plottable,
                                  CoordinateSystem coordinateSystem);

    CoordinateSystem getCurveCoordinateSystem(int plottable,
                                              bool *ok);

    bool setCurveAngleUnits(int plottable,
                            AngleUnits angleUnits);

    AngleUnits getCurveAngleUnits(int plottable,
                                  bool *ok);

    bool setCurveLineColor(int plottable,
                           QRgb rgba);

    QRgb getCurveLineColor(int plottable,
                           bool *ok);

    bool setCurveLineStyle(int plottable,
                           Qt::PenStyle lineStyle);

    Qt::PenStyle getCurveLineStyle(int plottable,
                                   bool *ok);

    bool setCurveFillColor(int plottable,
                           QRgb rgba);

    QRgb getCurveFillColor(int plottable,
                           bool *ok);

    bool setCurveFillStyle(int plottable,
                           Qt::BrushStyle fillStyle);

    Qt::BrushStyle getCurveFillStyle(int plottable,
                                     bool *ok);

    bool setCurveDeltaMode(int plottable,
                           bool deltaMode);

    bool getCurveDeltaMode(int plottable,
                           bool *ok);

    bool setCurveSize(int plottable,
                      int size);

    int getCurveSize(int plottable,
                     bool *ok);

    bool setCurveSampleRate(int plottable,
                            double sampleRate);

    double getCurveSampleRate(int plottable,
                              bool *ok);

    bool setCurveScaler(int plottable,
                        double scaler);

    double getCurveScaler(int plottable,
                          bool *ok);

    bool setCurveFFT(int plottable,
                     CurveFFT option);

    CurveFFT getCurveFFT(int plottable,
                         bool *ok);

    bool setCurveFFTWindowFunction(int plottable,
                                   CurveFFTWF function);

    CurveFFTWF getCurveFFTWindowFunction(int plottable,
                                         bool *ok);

    bool setCurveScatterStyle(int plottable,
                              QCPScatterStyle::ScatterShape scatterShape);

    QCPScatterStyle::ScatterShape getCurveScatterStyle(int plottable,
                                                       bool *ok);

    bool setCurveStyle(int plottable,
                       QCPCurve::LineStyle style);

    QCPCurve::LineStyle getCurveStyle(int plottable,
                                      bool *ok);

    bool setCurveChannelFillCurve(int plottable,
                                  int targetPlottable);

    int getCurveChannelFillCurve(int plottable,
                                 bool *ok);

    ///////////////////////////////////////////////////////////////////////////

    bool setGraphName(int plottable,
                      const QString &name);

    QString getGraphName(int plottable,
                         bool *ok);

    bool setGraphCoordinateSystem(int plottable,
                                  CoordinateSystem coordinateSystem);

    CoordinateSystem getGraphCoordinateSystem(int plottable,
                                              bool *ok);

    bool setGraphAngleUnits(int plottable,
                            AngleUnits angleUnits);

    AngleUnits getGraphAngleUnits(int plottable,
                                  bool *ok);

    bool setGraphLineColor(int plottable,
                           QRgb rgba);

    QRgb getGraphLineColor(int plottable,
                           bool *ok);

    bool setGraphLineStyle(int plottable,
                           Qt::PenStyle lineStyle);

    Qt::PenStyle getGraphLineStyle(int plottable,
                                   bool *ok);

    bool setGraphFillColor(int plottable,
                           QRgb rgba);

    QRgb getGraphFillColor(int plottable,
                           bool *ok);

    bool setGraphFillStyle(int plottable,
                           Qt::BrushStyle fillStyle);

    Qt::BrushStyle getGraphFillStyle(int plottable,
                                     bool *ok);

    bool setGraphDeltaMode(int plottable,
                           bool deltaMode);

    bool getGraphDeltaMode(int plottable,
                           bool *ok);

    bool setGraphSize(int plottable,
                      int size);

    int getGraphSize(int plottable,
                     bool *ok);

    bool setGraphSampleRate(int plottable,
                            double sampleRate);

    double getGraphSampleRate(int plottable,
                              bool *ok);

    bool setGraphScaler(int plottable,
                        double scaler);

    double getGraphScaler(int plottable,
                          bool *ok);

    bool setGraphFFT(int plottable,
                     GraphFFT option);

    GraphFFT getGraphFFT(int plottable,
                         bool *ok);

    bool setGraphFFTWindowFunction(int plottable,
                                   GraphFFTWF function);

    GraphFFTWF getGraphFFTWindowFunction(int plottable,
                                         bool *ok);

    bool setGraphScatterStyle(int plottable,
                              QCPScatterStyle::ScatterShape scatterShape);

    QCPScatterStyle::ScatterShape getGraphScatterStyle(int plottable,
                                                       bool *ok);

    bool setGraphStyle(int plottable,
                       QCPGraph::LineStyle style);

    QCPGraph::LineStyle getGraphStyle(int plottable,
                                      bool *ok);

    bool setGraphChannelFillGraph(int plottable,
                                  int targetPlottable);

    int getGraphChannelFillGraph(int plottable,
                                 bool *ok);

    bool setGraphKeyErrors(int plottable,
                           bool keyErrors);

    bool getGraphKeyErrors(int plottable,
                           bool *ok);

    bool setGraphValueErrors(int plottable,
                             bool valueErrors);

    bool getGraphValueErrors(int plottable,
                             bool *ok);

    ///////////////////////////////////////////////////////////////////////////

    bool setBoxName(int plottable,
                    const QString &name);

    QString getBoxName(int plottable,
                       bool *ok);

    bool setBoxCoordinateSystem(int plottable,
                                CoordinateSystem coordinateSystem);

    CoordinateSystem getBoxCoordinateSystem(int plottable,
                                            bool *ok);

    bool setBoxAngleUnits(int plottable,
                          AngleUnits angleUnits);

    AngleUnits getBoxAngleUnits(int plottable,
                                bool *ok);

    bool setBoxLineColor(int plottable,
                         QRgb rgba);

    QRgb getBoxLineColor(int plottable,
                         bool *ok);

    bool setBoxLineStyle(int plottable,
                         Qt::PenStyle lineStyle);

    Qt::PenStyle getBoxLineStyle(int plottable,
                                 bool *ok);

    bool setBoxFillColor(int plottable,
                         QRgb rgba);

    QRgb getBoxFillColor(int plottable,
                         bool *ok);

    bool setBoxFillStyle(int plottable,
                         Qt::BrushStyle fillStyle);

    Qt::BrushStyle getBoxFillStyle(int plottable,
                                   bool *ok);

    bool setBoxDeltaMode(int plottable,
                         bool deltaMode);

    bool getBoxDeltaMode(int plottable,
                         bool *ok);

    bool setBoxSize(int plottable,
                    int size);

    int getBoxSize(int plottable,
                   bool *ok);

    bool setBoxSampleRate(int plottable,
                          double sampleRate);

    double getBoxSampleRate(int plottable,
                            bool *ok);

    bool setBoxScaler(int plottable,
                      double scaler);

    double getBoxScaler(int plottable,
                        bool *ok);

    bool setBoxWidth(int plottable,
                     double width);

    double getBoxWidth(int plottable,
                       bool *ok);

    bool setBoxScatterStyle(int plottable,
                            QCPScatterStyle::ScatterShape scatterShape);

    QCPScatterStyle::ScatterShape getBoxScatterStyle(int plottable,
                                                     bool *ok);

    bool setBoxKey(int plottable,
                   double key);

    double getBoxKey(int plottable,
                     bool *ok);

    bool setBoxMinimum(int plottable,
                       double minimum);

    double getBoxMinimum(int plottable,
                         bool *ok);

    bool setBoxLowerQuartile(int plottable,
                             double lowerQuartile);

    double getBoxLowerQuartile(int plottable,
                               bool *ok);

    bool setBoxMedian(int plottable,
                      double median);

    double getBoxMedian(int plottable,
                        bool *ok);

    bool setBoxUpperQuartile(int plottable,
                             double upperQuartile);

    double getBoxUpperQuartile(int plottable,
                               bool *ok);

    bool setBoxMaximum(int plottable,
                       double maximum);

    double getBoxMaximum(int plottable,
                         bool *ok);

    ///////////////////////////////////////////////////////////////////////////

    bool deletePlot(int plottable);
    void deletePlots();

    ///////////////////////////////////////////////////////////////////////////

    bool addBarData(int plottable,
                    double key,
                    double value);

    bool addBarData(int plottable,
                    double value);

    ///////////////////////////////////////////////////////////////////////////

    bool addCurveData(int plottable,
                      double index,
                      double key,
                      double value);

    bool addCurveData(int plottable,
                      double key,
                      double value);

    ///////////////////////////////////////////////////////////////////////////

    bool addGraphData(int plottable,
                      double key,
                      double value);

    bool addGraphData(int plottable,
                      double value);

    ///////////////////////////////////////////////////////////////////////////

    bool addGraphDataWithErrors(int plottable,
                                double key,
                                double value,
                                double keyError,
                                double valueError);

    bool addGraphDataWithErrors(int plottable,
                                double value,
                                double keyError,
                                double valueError);

    bool addGraphDataWithErrors(int plottable,
                                double key,
                                double value,
                                double keyErrorMinus,
                                double keyErrorPlus,
                                double valueErrorMinus,
                                double valueErrorPlus);

    bool addGraphDataWithErrors(int plottable,
                                double value,
                                double keyErrorMinus,
                                double keyErrorPlus,
                                double valueErrorMinus,
                                double valueErrorPlus);

    ///////////////////////////////////////////////////////////////////////////

    bool addBoxData(int plottable,
                    double index,
                    double value);

    bool addBoxData(int plottable,
                    double value);

    ///////////////////////////////////////////////////////////////////////////

    bool removeBarDataBefore(int plottable,
                             double key);

    bool removeBarDateAfter(int plottable,
                            double key);

    bool removeBarDataBetween(int plottable,
                              double fromKey,
                              double toKey);

    bool removeBarData(int plottable);

    ///////////////////////////////////////////////////////////////////////////

    bool removeCurveDataBefore(int plottable,
                               double index);

    bool removeCurveDateAfter(int plottable,
                              double index);

    bool removeCurveDataBetween(int plottable,
                                double fromIndex,
                                double toIndex);

    bool removeCurveData(int plottable);

    ///////////////////////////////////////////////////////////////////////////

    bool removeGraphDataBefore(int plottable,
                               double key);

    bool removeGraphDateAfter(int plottable,
                              double key);

    bool removeGraphDataBetween(int plottable,
                                double fromKey,
                                double toKey);

    bool removeGraphData(int plottable);

    ///////////////////////////////////////////////////////////////////////////

    bool removeBoxDataBefore(int plottable,
                             double index);

    bool removeBoxDateAfter(int plottable,
                            double index);

    bool removeBoxDataBetween(int plottable,
                              double fromIndex,
                              double toIndex);

    bool removeBoxData(int plottable);

    ///////////////////////////////////////////////////////////////////////////

    bool removeData(int plottable);
    void removeDatas();

public slots:

    virtual void importState(const QString &fileName = QString());
    virtual void exportState(const QString &fileName = QString());

private slots:

    void zoomIn();
    void zoomOut();
    void zoomFit();

    void saveRasterImage();
    void saveVectorImage();

    void generalHelp();
    void oscilloscopeHelp();

    void updatePlots();
    void repaintPlots();

protected:

    virtual bool eventFilter(QObject *object, QEvent *event);

    virtual void resizeEvent(QResizeEvent *event);

private:

    Q_DISABLE_COPY(SerialOscilloscope)

    template <class T>
    T *plottableCast(const QString &FName,
                     int plottable);

    bool notInfinite(const QString &FName,
                     const QString &PName,
                     const QString &VName,
                     double value);

    bool notInfinite(const QString &FName,
                     const QString &VName,
                     double value);

    bool notZero(const QString &FName,
                 const QString &PName,
                 const QString &VName,
                 double value);

    bool notZero(const QString &FName,
                 const QString &VName,
                 double value);

    bool notInvalid(const QString &FName,
                    const QString &PName,
                    const QString &VName,
                    double value,
                    double minValue0,
                    double maxValue0,
                    double minValue1,
                    double maxValue1);

    bool notInvalid(const QString &FName,
                    const QString &PName,
                    const QString &VName,
                    double value,
                    double minValue,
                    double maxValue);

    bool notInvalid(const QString &FName,
                    const QString &VName,
                    double value,
                    double minValue,
                    double maxValue);

    QCPAbstractPlottable *plottableExists(const QString &FName,
                                          int plottable);

    bool plottableDoesNotExist(const QString &FName,
                               int plottable);

    bool FFTWEnabled(const QString &FName,
                     const QString &PName);

    bool FFTWEnabled(const QString &FName);

    static fftw_plan initFftwHelper(int size);
    static fftw_plan initFftwHelperF(int size);
    static fftw_plan initFftwHelperB(int size);

    static void calculateCurveFFT(MyCurve *curve);
    static void calculateGraphFFT(MyGraph *graph);

    static const QPen m_pens[]; static const int m_pensSize;
    static const QPen m_spens[]; static const int m_spensSize;
    static const QBrush m_brushes[]; static const int m_brushesSize;
    static const QBrush m_sbrushes[]; static const int m_sbrushesSize;

    static const int m_fftw_sizes[]; static const int m_fftw_sizesSize;
    static QMap<int, fftw_plan> m_fftw_plans, m_fftw_f_plans, m_fftw_b_plans;
    static bool m_fftw_enabled, m_fftw_multi_threaded, m_fftw_wisdom;
    static QMutex m_fftw_lock;

    MyCustomPlot *m_plot, *m_math; bool m_plotResize, m_mathResize;
    QCPPlotTitle *m_plotTitle, *m_mathTitle;

    QMutex m_replotLock;

    QBrush m_background;

    QMap<int, QCPAbstractPlottable *> m_plottables;

    bool m_plotXAutoScale, m_plotYAutoScale;
    QCPRange m_plotXAxisRangeBackup, m_plotYAxisRangeBackup;

    bool m_mathXAutoScale, m_mathYAutoScale;
    QCPRange m_mathXAxisRangeBackup, m_mathYAxisRangeBackup;

    int m_rasterWidth, m_rasterHeight, m_vectorWidth, m_vectorHeight;
    bool m_rasterSaveViewport, m_vectorSaveViewport;
    QString m_vectorTitle, m_vectorDescription;

    Ui::SerialOscilloscope *m_ui;
};

#endif // SERIALOSCILLOSCOPE_H

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
