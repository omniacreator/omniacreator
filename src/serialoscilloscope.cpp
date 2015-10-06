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

#include "serialoscilloscope.h"
#include "ui_serialoscilloscope.h"

const QPen SerialOscilloscope::m_pens[] =
{
    QPen(QBrush(QColor(000, 000, 255)), 0, Qt::SolidLine),
    QPen(QBrush(QColor(255, 000, 000)), 0, Qt::SolidLine),
    QPen(QBrush(QColor(000, 255, 000)), 0, Qt::SolidLine),
    QPen(QBrush(QColor(255, 000, 255)), 0, Qt::SolidLine),
    QPen(QBrush(QColor(000, 255, 255)), 0, Qt::SolidLine),
    QPen(QBrush(QColor(255, 255, 000)), 0, Qt::SolidLine),

    QPen(QBrush(QColor(000, 000, 255)), 0, Qt::DashLine),
    QPen(QBrush(QColor(255, 000, 000)), 0, Qt::DashLine),
    QPen(QBrush(QColor(000, 255, 000)), 0, Qt::DashLine),
    QPen(QBrush(QColor(255, 000, 255)), 0, Qt::DashLine),
    QPen(QBrush(QColor(000, 255, 255)), 0, Qt::DashLine),
    QPen(QBrush(QColor(255, 255, 000)), 0, Qt::DashLine),

    QPen(QBrush(QColor(000, 000, 255)), 0, Qt::DotLine),
    QPen(QBrush(QColor(255, 000, 000)), 0, Qt::DotLine),
    QPen(QBrush(QColor(000, 255, 000)), 0, Qt::DotLine),
    QPen(QBrush(QColor(255, 000, 255)), 0, Qt::DotLine),
    QPen(QBrush(QColor(000, 255, 255)), 0, Qt::DotLine),
    QPen(QBrush(QColor(255, 255, 000)), 0, Qt::DotLine),

    QPen(QBrush(QColor(000, 000, 255)), 0, Qt::DashDotLine),
    QPen(QBrush(QColor(255, 000, 000)), 0, Qt::DashDotLine),
    QPen(QBrush(QColor(000, 255, 000)), 0, Qt::DashDotLine),
    QPen(QBrush(QColor(255, 000, 255)), 0, Qt::DashDotLine),
    QPen(QBrush(QColor(000, 255, 255)), 0, Qt::DashDotLine),
    QPen(QBrush(QColor(255, 255, 000)), 0, Qt::DashDotLine),

    QPen(QBrush(QColor(000, 000, 255)), 0, Qt::DashDotDotLine),
    QPen(QBrush(QColor(255, 000, 000)), 0, Qt::DashDotDotLine),
    QPen(QBrush(QColor(000, 255, 000)), 0, Qt::DashDotDotLine),
    QPen(QBrush(QColor(255, 000, 255)), 0, Qt::DashDotDotLine),
    QPen(QBrush(QColor(000, 255, 255)), 0, Qt::DashDotDotLine),
    QPen(QBrush(QColor(255, 255, 000)), 0, Qt::DashDotDotLine)
};

const int SerialOscilloscope::m_pensSize =
sizeof(SerialOscilloscope::m_pens) / sizeof(const QPen);

const QPen SerialOscilloscope::m_spens[] =
{
    QPen(QBrush(QColor(000, 000, 255)), 3, Qt::SolidLine),
    QPen(QBrush(QColor(255, 000, 000)), 3, Qt::SolidLine),
    QPen(QBrush(QColor(000, 255, 000)), 3, Qt::SolidLine),
    QPen(QBrush(QColor(255, 000, 255)), 3, Qt::SolidLine),
    QPen(QBrush(QColor(000, 255, 255)), 3, Qt::SolidLine),
    QPen(QBrush(QColor(255, 255, 000)), 3, Qt::SolidLine),

    QPen(QBrush(QColor(000, 000, 255)), 3, Qt::DashLine),
    QPen(QBrush(QColor(255, 000, 000)), 3, Qt::DashLine),
    QPen(QBrush(QColor(000, 255, 000)), 3, Qt::DashLine),
    QPen(QBrush(QColor(255, 000, 255)), 3, Qt::DashLine),
    QPen(QBrush(QColor(000, 255, 255)), 3, Qt::DashLine),
    QPen(QBrush(QColor(255, 255, 000)), 3, Qt::DashLine),

    QPen(QBrush(QColor(000, 000, 255)), 3, Qt::DotLine),
    QPen(QBrush(QColor(255, 000, 000)), 3, Qt::DotLine),
    QPen(QBrush(QColor(000, 255, 000)), 3, Qt::DotLine),
    QPen(QBrush(QColor(255, 000, 255)), 3, Qt::DotLine),
    QPen(QBrush(QColor(000, 255, 255)), 3, Qt::DotLine),
    QPen(QBrush(QColor(255, 255, 000)), 3, Qt::DotLine),

    QPen(QBrush(QColor(000, 000, 255)), 3, Qt::DashDotLine),
    QPen(QBrush(QColor(255, 000, 000)), 3, Qt::DashDotLine),
    QPen(QBrush(QColor(000, 255, 000)), 3, Qt::DashDotLine),
    QPen(QBrush(QColor(255, 000, 255)), 3, Qt::DashDotLine),
    QPen(QBrush(QColor(000, 255, 255)), 3, Qt::DashDotLine),
    QPen(QBrush(QColor(255, 255, 000)), 3, Qt::DashDotLine),

    QPen(QBrush(QColor(000, 000, 255)), 3, Qt::DashDotDotLine),
    QPen(QBrush(QColor(255, 000, 000)), 3, Qt::DashDotDotLine),
    QPen(QBrush(QColor(000, 255, 000)), 3, Qt::DashDotDotLine),
    QPen(QBrush(QColor(255, 000, 255)), 3, Qt::DashDotDotLine),
    QPen(QBrush(QColor(000, 255, 255)), 3, Qt::DashDotDotLine),
    QPen(QBrush(QColor(255, 255, 000)), 3, Qt::DashDotDotLine)
};

const int SerialOscilloscope::m_spensSize =
sizeof(SerialOscilloscope::m_spens) / sizeof(const QPen);

const QBrush SerialOscilloscope::m_brushes[] =
{
    QBrush(QColor(200, 200, 255), Qt::NoBrush),
    QBrush(QColor(255, 200, 200), Qt::NoBrush),
    QBrush(QColor(200, 255, 200), Qt::NoBrush),
    QBrush(QColor(255, 200, 255), Qt::NoBrush),
    QBrush(QColor(200, 255, 255), Qt::NoBrush),
    QBrush(QColor(255, 255, 200), Qt::NoBrush),

    QBrush(QColor(200, 200, 255), Qt::NoBrush),
    QBrush(QColor(255, 200, 200), Qt::NoBrush),
    QBrush(QColor(200, 255, 200), Qt::NoBrush),
    QBrush(QColor(255, 200, 255), Qt::NoBrush),
    QBrush(QColor(200, 255, 255), Qt::NoBrush),
    QBrush(QColor(255, 255, 200), Qt::NoBrush),

    QBrush(QColor(200, 200, 255), Qt::NoBrush),
    QBrush(QColor(255, 200, 200), Qt::NoBrush),
    QBrush(QColor(200, 255, 200), Qt::NoBrush),
    QBrush(QColor(255, 200, 255), Qt::NoBrush),
    QBrush(QColor(200, 255, 255), Qt::NoBrush),
    QBrush(QColor(255, 255, 200), Qt::NoBrush),

    QBrush(QColor(200, 200, 255), Qt::NoBrush),
    QBrush(QColor(255, 200, 200), Qt::NoBrush),
    QBrush(QColor(200, 255, 200), Qt::NoBrush),
    QBrush(QColor(255, 200, 255), Qt::NoBrush),
    QBrush(QColor(200, 255, 255), Qt::NoBrush),
    QBrush(QColor(255, 255, 200), Qt::NoBrush),

    QBrush(QColor(200, 200, 255), Qt::NoBrush),
    QBrush(QColor(255, 200, 200), Qt::NoBrush),
    QBrush(QColor(200, 255, 200), Qt::NoBrush),
    QBrush(QColor(255, 200, 255), Qt::NoBrush),
    QBrush(QColor(200, 255, 255), Qt::NoBrush),
    QBrush(QColor(255, 255, 200), Qt::NoBrush)
};

const int SerialOscilloscope::m_brushesSize =
sizeof(SerialOscilloscope::m_brushes) / sizeof(const QBrush);

const QBrush SerialOscilloscope::m_sbrushes[] =
{
    QBrush(QColor(200, 200, 255), Qt::NoBrush),
    QBrush(QColor(255, 200, 200), Qt::NoBrush),
    QBrush(QColor(200, 255, 200), Qt::NoBrush),
    QBrush(QColor(255, 200, 255), Qt::NoBrush),
    QBrush(QColor(200, 255, 255), Qt::NoBrush),
    QBrush(QColor(255, 255, 200), Qt::NoBrush),

    QBrush(QColor(200, 200, 255), Qt::NoBrush),
    QBrush(QColor(255, 200, 200), Qt::NoBrush),
    QBrush(QColor(200, 255, 200), Qt::NoBrush),
    QBrush(QColor(255, 200, 255), Qt::NoBrush),
    QBrush(QColor(200, 255, 255), Qt::NoBrush),
    QBrush(QColor(255, 255, 200), Qt::NoBrush),

    QBrush(QColor(200, 200, 255), Qt::NoBrush),
    QBrush(QColor(255, 200, 200), Qt::NoBrush),
    QBrush(QColor(200, 255, 200), Qt::NoBrush),
    QBrush(QColor(255, 200, 255), Qt::NoBrush),
    QBrush(QColor(200, 255, 255), Qt::NoBrush),
    QBrush(QColor(255, 255, 200), Qt::NoBrush),

    QBrush(QColor(200, 200, 255), Qt::NoBrush),
    QBrush(QColor(255, 200, 200), Qt::NoBrush),
    QBrush(QColor(200, 255, 200), Qt::NoBrush),
    QBrush(QColor(255, 200, 255), Qt::NoBrush),
    QBrush(QColor(200, 255, 255), Qt::NoBrush),
    QBrush(QColor(255, 255, 200), Qt::NoBrush),

    QBrush(QColor(200, 200, 255), Qt::NoBrush),
    QBrush(QColor(255, 200, 200), Qt::NoBrush),
    QBrush(QColor(200, 255, 200), Qt::NoBrush),
    QBrush(QColor(255, 200, 255), Qt::NoBrush),
    QBrush(QColor(200, 255, 255), Qt::NoBrush),
    QBrush(QColor(255, 255, 200), Qt::NoBrush)
};

const int SerialOscilloscope::m_sbrushesSize =
sizeof(SerialOscilloscope::m_sbrushes) / sizeof(const QBrush);

const int SerialOscilloscope::m_fftw_sizes[] =
{
    2,      4,      8,      16,     32,     64,     128,    256,
    512,    1024,   2048,   4096,   8192,   16384,  32768,  65536
};

const int SerialOscilloscope::m_fftw_sizesSize =
sizeof(SerialOscilloscope::m_fftw_sizes) / sizeof(const int);

QMap<int, fftw_plan> SerialOscilloscope::m_fftw_plans =
QMap<int, fftw_plan>();

QMap<int, fftw_plan> SerialOscilloscope::m_fftw_f_plans =
QMap<int, fftw_plan>();

QMap<int, fftw_plan> SerialOscilloscope::m_fftw_b_plans =
QMap<int, fftw_plan>();

bool SerialOscilloscope::m_fftw_enabled = false;
bool SerialOscilloscope::m_fftw_multi_threaded = false;
bool SerialOscilloscope::m_fftw_wisdom = false;

QMutex SerialOscilloscope::m_fftw_lock;

void SerialOscilloscope::initFftw(QSettings *settings, QWidget *parent)
{
    finiFftw();

    if((m_fftw_multi_threaded=fftw_init_threads()))
    {
        fftw_plan_with_nthreads(qMax(QThread::idealThreadCount(), 1));
    }

    QSettings mySettings(settings ? settings->fileName() : QString(),
                         settings ? settings->format() : QSettings::Format());

    mySettings.beginGroup(SERIAL_OSCILLOSCOPE_FFTW_KEY_GROUP);

    if(mySettings.contains(SERIAL_OSCILLOSCOPE_FFTW_KEY_WISDOM))
    {
        QByteArray wisdom = mySettings.
        value(SERIAL_OSCILLOSCOPE_FFTW_KEY_WISDOM).toString().toUtf8();

        if(!(m_fftw_wisdom=fftw_import_wisdom_from_string(wisdom.constData())))
        {
            QString name = QApplication::applicationName();

            QMessageBox::critical(parent, tr("FFT Benchmark"),
            tr("Benchmark data corrupted. %L1 must rebenchmark.").arg(name));
        }
    }
    else
    {
        QString name = QApplication::applicationName();

        QMessageBox::information(parent, tr("FFT Benchmark"),
        tr("%L1 has to benchmark your computer processor to enable "
           "Oscilloscope FFT support. %L1 only needs to do this once. "
           "However, it may take a little while.").arg(name));
    }

    QProgressDialog dialog(parent, Qt::MSWindowsFixedSizeDialogHint |
    Qt::WindowTitleHint | Qt::WindowSystemMenuHint |
    Qt::WindowCloseButtonHint);

    dialog.setWindowTitle(tr("FFT Benchmark"));
    dialog.setWindowModality(Qt::ApplicationModal);
    dialog.setLabelText(tr("Benchmarking Processor..."));
    dialog.setRange(0, m_fftw_sizesSize * 3); dialog.setValue(0);

    if(!m_fftw_wisdom)
    {
        dialog.show();
    }

    QEventLoop loop;
    QFuture<fftw_plan> future;
    QFutureWatcher<fftw_plan> watcher;

    connect(&watcher, SIGNAL(finished()), &loop, SLOT(quit()));

    for(int i = 0; i < m_fftw_sizesSize; i++)
    {
        future = QtConcurrent::run(initFftwHelper, m_fftw_sizes[i]);
        watcher.setFuture(future); loop.exec();
        fftw_plan plan = future.result();

        if(dialog.wasCanceled())
        {
            break;
        }

        if(plan)
        {
            m_fftw_plans.insert(m_fftw_sizes[i], plan);
        }

        dialog.setValue((i * 3) + 1);

        ///////////////////////////////////////////////////////////////////////

        future = QtConcurrent::run(initFftwHelperF, m_fftw_sizes[i]);
        watcher.setFuture(future); loop.exec();
        plan = future.result();

        if(dialog.wasCanceled())
        {
            break;
        }

        if(plan)
        {
            m_fftw_f_plans.insert(m_fftw_sizes[i], plan);
        }

        dialog.setValue((i * 3) + 2);

        ///////////////////////////////////////////////////////////////////////

        future = QtConcurrent::run(initFftwHelperB, m_fftw_sizes[i]);
        watcher.setFuture(future); loop.exec();
        plan = future.result();

        if(dialog.wasCanceled())
        {
            break;
        }

        if(plan)
        {
            m_fftw_b_plans.insert(m_fftw_sizes[i], plan);
        }

        dialog.setValue((i * 3) + 3);
    }

    m_fftw_enabled = true;

    if((m_fftw_plans.size() == m_fftw_sizesSize)
    && (m_fftw_f_plans.size() == m_fftw_sizesSize)
    && (m_fftw_b_plans.size() == m_fftw_sizesSize))
    {
        char *wisdom = fftw_export_wisdom_to_string();
        mySettings.setValue(SERIAL_OSCILLOSCOPE_FFTW_KEY_WISDOM, wisdom);
        free(wisdom);

        if(!m_fftw_wisdom)
        {
            QMessageBox::information(parent, tr("FFT Benchmark"),
            tr("Benchmarking succeeded. Enabling Oscilloscope FFT support."));
        }
    }
    else
    {
        finiFftw();

        QMessageBox::critical(parent, tr("FFT Benchmark"),
        tr("Benchmarking failed. Disabling Oscilloscope FFT support."));
    }
}

fftw_plan SerialOscilloscope::initFftwHelper(int size)
{
    fftw_plan plan = NULL;

    if(size > 1)
    {
        int inSize = size;
        int outSize = ((size / 2) + 1);

        double *in = q_check_ptr(fftw_alloc_real(inSize));
        fftw_complex *out = q_check_ptr(fftw_alloc_complex(outSize));

        qMemSet(in, 0, sizeof(double) * inSize);
        qMemSet(out, 0, sizeof(fftw_complex) * outSize);

        m_fftw_lock.lock();
        plan = fftw_plan_dft_r2c_1d(size, in, out,
        (m_fftw_wisdom ? FFTW_ESTIMATE : FFTW_MEASURE) | FFTW_DESTROY_INPUT);
        m_fftw_lock.unlock();

        fftw_free(in);
        fftw_free(out);
    }

    return plan;
}

fftw_plan SerialOscilloscope::initFftwHelperF(int size)
{
    fftw_plan plan = NULL;

    if(size > 1)
    {
        int inSize = size;
        int outSize = size;

        fftw_complex *in = q_check_ptr(fftw_alloc_complex(inSize));
        fftw_complex *out = q_check_ptr(fftw_alloc_complex(outSize));

        qMemSet(in, 0, sizeof(fftw_complex) * inSize);
        qMemSet(out, 0, sizeof(fftw_complex) * outSize);

        m_fftw_lock.lock();
        plan = fftw_plan_dft_1d(size, in, out, FFTW_FORWARD,
        (m_fftw_wisdom ? FFTW_ESTIMATE : FFTW_MEASURE) | FFTW_DESTROY_INPUT);
        m_fftw_lock.unlock();

        fftw_free(in);
        fftw_free(out);
    }

    return plan;
}

fftw_plan SerialOscilloscope::initFftwHelperB(int size)
{
    fftw_plan plan = NULL;

    if(size > 1)
    {
        int inSize = size;
        int outSize = size;

        fftw_complex *in = q_check_ptr(fftw_alloc_complex(inSize));
        fftw_complex *out = q_check_ptr(fftw_alloc_complex(outSize));

        qMemSet(in, 0, sizeof(fftw_complex) * inSize);
        qMemSet(out, 0, sizeof(fftw_complex) * outSize);

        m_fftw_lock.lock();
        plan = fftw_plan_dft_1d(size, in, out, FFTW_BACKWARD,
        (m_fftw_wisdom ? FFTW_ESTIMATE : FFTW_MEASURE) | FFTW_DESTROY_INPUT);
        m_fftw_lock.unlock();

        fftw_free(in);
        fftw_free(out);
    }

    return plan;
}

void SerialOscilloscope::finiFftw()
{
    if(m_fftw_enabled)
    {
        foreach(fftw_plan plan, m_fftw_plans.values())
        {
            fftw_destroy_plan(plan);
        }

        m_fftw_plans.clear();

        foreach(fftw_plan plan, m_fftw_f_plans.values())
        {
            fftw_destroy_plan(plan);
        }

        m_fftw_f_plans.clear();

        foreach(fftw_plan plan, m_fftw_b_plans.values())
        {
            fftw_destroy_plan(plan);
        }

        m_fftw_b_plans.clear();

        if(m_fftw_multi_threaded)
        {
            fftw_cleanup_threads();
        }
        else
        {
            fftw_cleanup();
        }

        fftw_forget_wisdom();

        m_fftw_enabled = false;
        m_fftw_multi_threaded = false;
        m_fftw_wisdom = false;
    }
}

SerialOscilloscope::SerialOscilloscope(const QString &title,
QSettings *settings, QWidget *parent) :
SerialWindow(title, settings, parent), m_ui(new Ui::SerialOscilloscope)
{
    m_ui->setupUi(this); m_ui->statusBar->hide();

    QGLWidget *plotGLWidget = new QGLWidget(QGLFormat(QGL::SampleBuffers));
    QGLWidget *mathGLWidget = new QGLWidget(QGLFormat(QGL::SampleBuffers));

    QGridLayout *plotLayout = new QGridLayout(plotGLWidget);
    QGridLayout *mathLayout = new QGridLayout(mathGLWidget);

    plotLayout->setMargin(0); plotLayout->setSpacing(0);
    mathLayout->setMargin(0); mathLayout->setSpacing(0);

    m_ui->gridLayout->addWidget(plotGLWidget, 0, 0);
    m_ui->gridLayout->addWidget(mathGLWidget, 0, 1);

    plotLayout->addWidget(m_plot = new MyCustomPlot()); m_plotResize = true;
    mathLayout->addWidget(m_math = new MyCustomPlot()); m_mathResize = true;

    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    m_math->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    m_plot->setAutoAddPlottableToLegend(false);
    m_math->setAutoAddPlottableToLegend(false);

    m_plot->axisRect()->setupFullAxesBox(true);
    m_math->axisRect()->setupFullAxesBox(true);

    m_plot->installEventFilter(this);
    m_math->installEventFilter(this);

    m_plot->xAxis->setNumberFormat("gb"); m_plot->xAxis->setPadding(2);
    m_math->xAxis->setNumberFormat("gb"); m_math->xAxis->setPadding(2);

    m_plot->yAxis->setNumberFormat("gb"); m_plot->yAxis->setPadding(2);
    m_math->yAxis->setNumberFormat("gb"); m_math->yAxis->setPadding(2);

    m_plot->xAxis2->setNumberFormat("gb"); m_plot->xAxis2->setPadding(2);
    m_math->xAxis2->setNumberFormat("gb"); m_math->xAxis2->setPadding(2);

    m_plot->yAxis2->setNumberFormat("gb"); m_plot->yAxis2->setPadding(2);
    m_math->yAxis2->setNumberFormat("gb"); m_math->yAxis2->setPadding(2);

    m_plot->xAxis2->setTicks(false); m_plot->yAxis2->setTicks(false);
    m_math->xAxis2->setTicks(false); m_math->yAxis2->setTicks(false);

    m_plot->plotLayout()->insertRow(0); m_plot->plotLayout()->addElement(0, 0,
    m_plotTitle = new QCPPlotTitle(m_plot, tr("Time Domain")));
    m_plotTitle->setVisible(false);

    m_math->plotLayout()->insertRow(0); m_math->plotLayout()->addElement(0, 0,
    m_mathTitle = new QCPPlotTitle(m_math, tr("Frequency Domain")));
    m_mathTitle->setVisible(false);

    m_math->parentWidget()->hide();

    m_background = Qt::white; setBackgroundColor(getBackgroundColor());

    m_plottables = QMap<int, QCPAbstractPlottable*>();

    m_plotXAutoScale = true; m_plotYAutoScale = true;
    m_plotXAxisRangeBackup = m_plot->xAxis->range();
    m_plotYAxisRangeBackup = m_plot->yAxis->range();

    m_mathXAutoScale = true; m_mathYAutoScale = true;
    m_mathXAxisRangeBackup = m_math->xAxis->range();
    m_mathYAxisRangeBackup = m_math->yAxis->range();

    m_rasterWidth = 640; m_rasterHeight = 480; m_rasterSaveViewport = false;
    m_vectorWidth = 640; m_vectorHeight = 480; m_vectorSaveViewport = false;

    m_vectorTitle = windowTitle();
    m_vectorDescription = tr("By: ") + qgetenv(BY_NAME);

    connect(m_ui->actionZoom_In, SIGNAL(triggered()),
            this, SLOT(zoomIn()));

    connect(m_ui->actionZoom_Out, SIGNAL(triggered()),
            this, SLOT(zoomOut()));

    connect(m_ui->actionZoom_Fit, SIGNAL(triggered()),
            this, SLOT(zoomFit()));

    connect(m_ui->actionSave_Raster_Image, SIGNAL(triggered()),
            this, SLOT(saveRasterImage()));

    connect(m_ui->actionSave_Vector_Image, SIGNAL(triggered()),
            this, SLOT(saveVectorImage()));

    connect(m_ui->actionImport_State, SIGNAL(triggered()),
            this, SLOT(importState()));

    connect(m_ui->actionExport_State, SIGNAL(triggered()),
            this, SLOT(exportState()));

    connect(m_ui->actionClose_Window, SIGNAL(triggered()),
            this, SLOT(close()));

    connect(m_ui->actionGeneral_Help, SIGNAL(triggered()),
            this, SLOT(generalHelp()));

    connect(m_ui->actionOscilloscope_Help, SIGNAL(triggered()),
            this, SLOT(oscilloscopeHelp()));

    restoreState();

    updatePlots();
}

SerialOscilloscope::~SerialOscilloscope()
{
    m_replotLock.tryLock();
    m_replotLock.unlock();

    delete m_ui;
}

bool SerialOscilloscope::importExportEnabled() const
{
    return true;
}

QString SerialOscilloscope::keyGroup() const
{
    return SERIAL_OSCILLOSCOPE_KEY_GROUP;
}

QString SerialOscilloscope::keyState() const
{
    return SERIAL_OSCILLOSCOPE_KEY_STATE;
}

QString SerialOscilloscope::keyGeometry() const
{
    return SERIAL_OSCILLOSCOPE_KEY_GEOMETRY;
}

void SerialOscilloscope::setBackgroundColor(QRgb rgba)
{
    m_background = QColor::fromRgb(rgba); // NO APLHA!

    // Setup QCustomplot "Plot" Colors...
    {
        m_plot->setBackground(m_background);

        // Axis Base Pen
        {
            QPen x1Pen = m_plot->xAxis->basePen();
            QPen y1Pen = m_plot->yAxis->basePen();
            QPen x2Pen = m_plot->xAxis2->basePen();
            QPen y2Pen = m_plot->yAxis2->basePen();

            QPen sx1Pen = m_plot->xAxis->selectedBasePen();
            QPen sy1Pen = m_plot->yAxis->selectedBasePen();
            QPen sx2Pen = m_plot->xAxis2->selectedBasePen();
            QPen sy2Pen = m_plot->yAxis2->selectedBasePen();

            if(m_background.color().lightness() > 150)
            {
                x1Pen.setColor(Qt::black);
                y1Pen.setColor(Qt::black);
                x2Pen.setColor(Qt::black);
                y2Pen.setColor(Qt::black);

                sx1Pen.setColor(QColor(000, 000, 255));
                sy1Pen.setColor(QColor(000, 000, 255));
                sx2Pen.setColor(QColor(000, 000, 255));
                sy2Pen.setColor(QColor(000, 000, 255));
            }
            else
            {
                x1Pen.setColor(Qt::white);
                y1Pen.setColor(Qt::white);
                x2Pen.setColor(Qt::white);
                y2Pen.setColor(Qt::white);

                sx1Pen.setColor(QColor(200, 200, 255));
                sy1Pen.setColor(QColor(200, 200, 255));
                sx2Pen.setColor(QColor(200, 200, 255));
                sy2Pen.setColor(QColor(200, 200, 255));
            }

            m_plot->xAxis->setBasePen(x1Pen);
            m_plot->yAxis->setBasePen(y1Pen);
            m_plot->xAxis2->setBasePen(x2Pen);
            m_plot->yAxis2->setBasePen(y2Pen);

            m_plot->xAxis->setSelectedBasePen(sx1Pen);
            m_plot->yAxis->setSelectedBasePen(sy1Pen);
            m_plot->xAxis2->setSelectedBasePen(sx2Pen);
            m_plot->yAxis2->setSelectedBasePen(sy2Pen);
        }

        // Axis Tick Pen
        {
            QPen x1Pen = m_plot->xAxis->tickPen();
            QPen y1Pen = m_plot->yAxis->tickPen();
            QPen x2Pen = m_plot->xAxis2->tickPen();
            QPen y2Pen = m_plot->yAxis2->tickPen();

            QPen sx1Pen = m_plot->xAxis->selectedTickPen();
            QPen sy1Pen = m_plot->yAxis->selectedTickPen();
            QPen sx2Pen = m_plot->xAxis2->selectedTickPen();
            QPen sy2Pen = m_plot->yAxis2->selectedTickPen();

            if(m_background.color().lightness() > 150)
            {
                x1Pen.setColor(Qt::black);
                y1Pen.setColor(Qt::black);
                x2Pen.setColor(Qt::black);
                y2Pen.setColor(Qt::black);

                sx1Pen.setColor(QColor(000, 000, 255));
                sy1Pen.setColor(QColor(000, 000, 255));
                sx2Pen.setColor(QColor(000, 000, 255));
                sy2Pen.setColor(QColor(000, 000, 255));
            }
            else
            {
                x1Pen.setColor(Qt::white);
                y1Pen.setColor(Qt::white);
                x2Pen.setColor(Qt::white);
                y2Pen.setColor(Qt::white);

                sx1Pen.setColor(QColor(200, 200, 255));
                sy1Pen.setColor(QColor(200, 200, 255));
                sx2Pen.setColor(QColor(200, 200, 255));
                sy2Pen.setColor(QColor(200, 200, 255));
            }

            m_plot->xAxis->setTickPen(x1Pen);
            m_plot->yAxis->setTickPen(y1Pen);
            m_plot->xAxis2->setTickPen(x2Pen);
            m_plot->yAxis2->setTickPen(y2Pen);

            m_plot->xAxis->setSelectedTickPen(sx1Pen);
            m_plot->yAxis->setSelectedTickPen(sy1Pen);
            m_plot->xAxis2->setSelectedTickPen(sx2Pen);
            m_plot->yAxis2->setSelectedTickPen(sy2Pen);
        }

        // Axis Sub Tick Pen
        {
            QPen x1Pen = m_plot->xAxis->subTickPen();
            QPen y1Pen = m_plot->yAxis->subTickPen();
            QPen x2Pen = m_plot->xAxis2->subTickPen();
            QPen y2Pen = m_plot->yAxis2->subTickPen();

            QPen sx1Pen = m_plot->xAxis->selectedSubTickPen();
            QPen sy1Pen = m_plot->yAxis->selectedSubTickPen();
            QPen sx2Pen = m_plot->xAxis2->selectedSubTickPen();
            QPen sy2Pen = m_plot->yAxis2->selectedSubTickPen();

            if(m_background.color().lightness() > 150)
            {
                x1Pen.setColor(Qt::black);
                y1Pen.setColor(Qt::black);
                x2Pen.setColor(Qt::black);
                y2Pen.setColor(Qt::black);

                sx1Pen.setColor(QColor(000, 000, 255));
                sy1Pen.setColor(QColor(000, 000, 255));
                sx2Pen.setColor(QColor(000, 000, 255));
                sy2Pen.setColor(QColor(000, 000, 255));
            }
            else
            {
                x1Pen.setColor(Qt::white);
                y1Pen.setColor(Qt::white);
                x2Pen.setColor(Qt::white);
                y2Pen.setColor(Qt::white);

                sx1Pen.setColor(QColor(200, 200, 255));
                sy1Pen.setColor(QColor(200, 200, 255));
                sx2Pen.setColor(QColor(200, 200, 255));
                sy2Pen.setColor(QColor(200, 200, 255));
            }

            m_plot->xAxis->setSubTickPen(x1Pen);
            m_plot->yAxis->setSubTickPen(y1Pen);
            m_plot->xAxis2->setSubTickPen(x2Pen);
            m_plot->yAxis2->setSubTickPen(y2Pen);

            m_plot->xAxis->setSelectedSubTickPen(sx1Pen);
            m_plot->yAxis->setSelectedSubTickPen(sy1Pen);
            m_plot->xAxis2->setSelectedSubTickPen(sx2Pen);
            m_plot->yAxis2->setSelectedSubTickPen(sy2Pen);
        }

        // Axis Grid Pen
        {
            QPen x1Pen = m_plot->xAxis->grid()->pen();
            QPen y1Pen = m_plot->yAxis->grid()->pen();
            QPen x2Pen = m_plot->xAxis2->grid()->pen();
            QPen y2Pen = m_plot->yAxis2->grid()->pen();

            if(m_background.color().lightness() > 150)
            {
                x1Pen.setColor(QColor(Qt::black).lighter());
                y1Pen.setColor(QColor(Qt::black).lighter());
                x2Pen.setColor(QColor(Qt::black).lighter());
                y2Pen.setColor(QColor(Qt::black).lighter());
            }
            else
            {
                x1Pen.setColor(QColor(Qt::white).darker());
                y1Pen.setColor(QColor(Qt::white).darker());
                x2Pen.setColor(QColor(Qt::white).darker());
                y2Pen.setColor(QColor(Qt::white).darker());
            }

            m_plot->xAxis->grid()->setPen(x1Pen);
            m_plot->yAxis->grid()->setPen(y1Pen);
            m_plot->xAxis2->grid()->setPen(x2Pen);
            m_plot->yAxis2->grid()->setPen(y2Pen);
        }

        // Axis Sub Grid Pen
        {
            QPen x1Pen = m_plot->xAxis->grid()->subGridPen();
            QPen y1Pen = m_plot->yAxis->grid()->subGridPen();
            QPen x2Pen = m_plot->xAxis2->grid()->subGridPen();
            QPen y2Pen = m_plot->yAxis2->grid()->subGridPen();

            if(m_background.color().lightness() > 150)
            {
                x1Pen.setColor(QColor(Qt::black).lighter());
                y1Pen.setColor(QColor(Qt::black).lighter());
                x2Pen.setColor(QColor(Qt::black).lighter());
                y2Pen.setColor(QColor(Qt::black).lighter());
            }
            else
            {
                x1Pen.setColor(QColor(Qt::white).darker());
                y1Pen.setColor(QColor(Qt::white).darker());
                x2Pen.setColor(QColor(Qt::white).darker());
                y2Pen.setColor(QColor(Qt::white).darker());
            }

            m_plot->xAxis->grid()->setSubGridPen(x1Pen);
            m_plot->yAxis->grid()->setSubGridPen(y1Pen);
            m_plot->xAxis2->grid()->setSubGridPen(x2Pen);
            m_plot->yAxis2->grid()->setSubGridPen(y2Pen);
        }

        // Axis Zero Line Pen
        {
            QPen x1Pen = m_plot->xAxis->grid()->zeroLinePen();
            QPen y1Pen = m_plot->yAxis->grid()->zeroLinePen();
            QPen x2Pen = m_plot->xAxis2->grid()->zeroLinePen();
            QPen y2Pen = m_plot->yAxis2->grid()->zeroLinePen();

            if(m_background.color().lightness() > 150)
            {
                x1Pen.setColor(QColor(Qt::black).lighter());
                y1Pen.setColor(QColor(Qt::black).lighter());
                x2Pen.setColor(QColor(Qt::black).lighter());
                y2Pen.setColor(QColor(Qt::black).lighter());
            }
            else
            {
                x1Pen.setColor(QColor(Qt::white).darker());
                y1Pen.setColor(QColor(Qt::white).darker());
                x2Pen.setColor(QColor(Qt::white).darker());
                y2Pen.setColor(QColor(Qt::white).darker());
            }

            m_plot->xAxis->grid()->setZeroLinePen(x1Pen);
            m_plot->yAxis->grid()->setZeroLinePen(y1Pen);
            m_plot->xAxis2->grid()->setZeroLinePen(x2Pen);
            m_plot->yAxis2->grid()->setZeroLinePen(y2Pen);
        }

        // Axis Label Color

        if(m_background.color().lightness() > 150)
        {
            m_plot->xAxis->setLabelColor(Qt::black);
            m_plot->yAxis->setLabelColor(Qt::black);
            m_plot->xAxis2->setLabelColor(Qt::black);
            m_plot->yAxis2->setLabelColor(Qt::black);

            m_plot->xAxis->setTickLabelColor(Qt::black);
            m_plot->yAxis->setTickLabelColor(Qt::black);
            m_plot->xAxis2->setTickLabelColor(Qt::black);
            m_plot->yAxis2->setTickLabelColor(Qt::black);

            m_plot->xAxis->setSelectedLabelColor(QColor(000, 000, 255));
            m_plot->yAxis->setSelectedLabelColor(QColor(000, 000, 255));
            m_plot->xAxis2->setSelectedLabelColor(QColor(000, 000, 255));
            m_plot->yAxis2->setSelectedLabelColor(QColor(000, 000, 255));

            m_plot->xAxis->setSelectedTickLabelColor(QColor(000, 000, 255));
            m_plot->yAxis->setSelectedTickLabelColor(QColor(000, 000, 255));
            m_plot->xAxis2->setSelectedTickLabelColor(QColor(000, 000, 255));
            m_plot->yAxis2->setSelectedTickLabelColor(QColor(000, 000, 255));
        }
        else
        {
            m_plot->xAxis->setLabelColor(Qt::white);
            m_plot->yAxis->setLabelColor(Qt::white);
            m_plot->xAxis2->setLabelColor(Qt::white);
            m_plot->yAxis2->setLabelColor(Qt::white);

            m_plot->xAxis->setTickLabelColor(Qt::white);
            m_plot->yAxis->setTickLabelColor(Qt::white);
            m_plot->xAxis2->setTickLabelColor(Qt::white);
            m_plot->yAxis2->setTickLabelColor(Qt::white);

            m_plot->xAxis->setSelectedLabelColor(QColor(200, 200, 255));
            m_plot->yAxis->setSelectedLabelColor(QColor(200, 200, 255));
            m_plot->xAxis2->setSelectedLabelColor(QColor(200, 200, 255));
            m_plot->yAxis2->setSelectedLabelColor(QColor(200, 200, 255));

            m_plot->xAxis->setSelectedTickLabelColor(QColor(200, 200, 255));
            m_plot->yAxis->setSelectedTickLabelColor(QColor(200, 200, 255));
            m_plot->xAxis2->setSelectedTickLabelColor(QColor(200, 200, 255));
            m_plot->yAxis2->setSelectedTickLabelColor(QColor(200, 200, 255));
        }

        // Legend Brush
        {
            QBrush brush = m_plot->legend->brush();
            QBrush sbrush = m_plot->legend->selectedBrush();

            brush.setColor(m_background.color());
            sbrush.setColor(m_background.color());

            m_plot->legend->setBrush(brush);
            m_plot->legend->setSelectedBrush(sbrush);
        }

        // Legend Border Pen
        {
            QPen pen = m_plot->legend->borderPen();
            QPen spen = m_plot->legend->selectedBorderPen();

            if(m_background.color().lightness() > 150)
            {
                pen.setColor(Qt::black);
                spen.setColor(QColor(000, 000, 255));
            }
            else
            {
                pen.setColor(Qt::white);
                spen.setColor(QColor(200, 200, 255));
            }

            m_plot->legend->setBorderPen(pen);
            m_plot->legend->setSelectedBorderPen(spen);
        }

        // Legend Icon Border Pen
        {
            QPen pen = m_plot->legend->iconBorderPen();
            QPen spen = m_plot->legend->selectedIconBorderPen();

            pen.setColor(m_background.color());
            spen.setColor(m_background.color());

            m_plot->legend->setIconBorderPen(pen);
            m_plot->legend->setSelectedIconBorderPen(spen);
        }

        // Legend Text Color

        if(m_background.color().lightness() > 150)
        {
            m_plot->legend->setTextColor(Qt::black);
            m_plot->legend->setSelectedTextColor(QColor(000, 000, 255));
        }
        else
        {
            m_plot->legend->setTextColor(Qt::white);
            m_plot->legend->setSelectedTextColor(QColor(200, 200, 255));
        }

        // Title Text Color

        if(m_background.color().lightness() > 150)
        {
            m_plotTitle->setTextColor(Qt::black);
            m_plotTitle->setSelectedTextColor(QColor(000, 000, 255));
        }
        else
        {
            m_plotTitle->setTextColor(Qt::white);
            m_plotTitle->setSelectedTextColor(QColor(200, 200, 255));
        }
    }

    // Setup QCustomplot "Math" Colors...
    {
        m_math->setBackground(m_background);

        // Axis Base Pen
        {
            QPen x1Pen = m_math->xAxis->basePen();
            QPen y1Pen = m_math->yAxis->basePen();
            QPen x2Pen = m_math->xAxis2->basePen();
            QPen y2Pen = m_math->yAxis2->basePen();

            QPen sx1Pen = m_math->xAxis->selectedBasePen();
            QPen sy1Pen = m_math->yAxis->selectedBasePen();
            QPen sx2Pen = m_math->xAxis2->selectedBasePen();
            QPen sy2Pen = m_math->yAxis2->selectedBasePen();

            if(m_background.color().lightness() > 150)
            {
                x1Pen.setColor(Qt::black);
                y1Pen.setColor(Qt::black);
                x2Pen.setColor(Qt::black);
                y2Pen.setColor(Qt::black);

                sx1Pen.setColor(QColor(000, 000, 255));
                sy1Pen.setColor(QColor(000, 000, 255));
                sx2Pen.setColor(QColor(000, 000, 255));
                sy2Pen.setColor(QColor(000, 000, 255));
            }
            else
            {
                x1Pen.setColor(Qt::white);
                y1Pen.setColor(Qt::white);
                x2Pen.setColor(Qt::white);
                y2Pen.setColor(Qt::white);

                sx1Pen.setColor(QColor(200, 200, 255));
                sy1Pen.setColor(QColor(200, 200, 255));
                sx2Pen.setColor(QColor(200, 200, 255));
                sy2Pen.setColor(QColor(200, 200, 255));
            }

            m_math->xAxis->setBasePen(x1Pen);
            m_math->yAxis->setBasePen(y1Pen);
            m_math->xAxis2->setBasePen(x2Pen);
            m_math->yAxis2->setBasePen(y2Pen);

            m_math->xAxis->setSelectedBasePen(sx1Pen);
            m_math->yAxis->setSelectedBasePen(sy1Pen);
            m_math->xAxis2->setSelectedBasePen(sx2Pen);
            m_math->yAxis2->setSelectedBasePen(sy2Pen);
        }

        // Axis Tick Pen
        {
            QPen x1Pen = m_math->xAxis->tickPen();
            QPen y1Pen = m_math->yAxis->tickPen();
            QPen x2Pen = m_math->xAxis2->tickPen();
            QPen y2Pen = m_math->yAxis2->tickPen();

            QPen sx1Pen = m_math->xAxis->selectedTickPen();
            QPen sy1Pen = m_math->yAxis->selectedTickPen();
            QPen sx2Pen = m_math->xAxis2->selectedTickPen();
            QPen sy2Pen = m_math->yAxis2->selectedTickPen();

            if(m_background.color().lightness() > 150)
            {
                x1Pen.setColor(Qt::black);
                y1Pen.setColor(Qt::black);
                x2Pen.setColor(Qt::black);
                y2Pen.setColor(Qt::black);

                sx1Pen.setColor(QColor(000, 000, 255));
                sy1Pen.setColor(QColor(000, 000, 255));
                sx2Pen.setColor(QColor(000, 000, 255));
                sy2Pen.setColor(QColor(000, 000, 255));
            }
            else
            {
                x1Pen.setColor(Qt::white);
                y1Pen.setColor(Qt::white);
                x2Pen.setColor(Qt::white);
                y2Pen.setColor(Qt::white);

                sx1Pen.setColor(QColor(200, 200, 255));
                sy1Pen.setColor(QColor(200, 200, 255));
                sx2Pen.setColor(QColor(200, 200, 255));
                sy2Pen.setColor(QColor(200, 200, 255));
            }

            m_math->xAxis->setTickPen(x1Pen);
            m_math->yAxis->setTickPen(y1Pen);
            m_math->xAxis2->setTickPen(x2Pen);
            m_math->yAxis2->setTickPen(y2Pen);

            m_math->xAxis->setSelectedTickPen(sx1Pen);
            m_math->yAxis->setSelectedTickPen(sy1Pen);
            m_math->xAxis2->setSelectedTickPen(sx2Pen);
            m_math->yAxis2->setSelectedTickPen(sy2Pen);
        }

        // Axis Sub Tick Pen
        {
            QPen x1Pen = m_math->xAxis->subTickPen();
            QPen y1Pen = m_math->yAxis->subTickPen();
            QPen x2Pen = m_math->xAxis2->subTickPen();
            QPen y2Pen = m_math->yAxis2->subTickPen();

            QPen sx1Pen = m_math->xAxis->selectedSubTickPen();
            QPen sy1Pen = m_math->yAxis->selectedSubTickPen();
            QPen sx2Pen = m_math->xAxis2->selectedSubTickPen();
            QPen sy2Pen = m_math->yAxis2->selectedSubTickPen();

            if(m_background.color().lightness() > 150)
            {
                x1Pen.setColor(Qt::black);
                y1Pen.setColor(Qt::black);
                x2Pen.setColor(Qt::black);
                y2Pen.setColor(Qt::black);

                sx1Pen.setColor(QColor(000, 000, 255));
                sy1Pen.setColor(QColor(000, 000, 255));
                sx2Pen.setColor(QColor(000, 000, 255));
                sy2Pen.setColor(QColor(000, 000, 255));
            }
            else
            {
                x1Pen.setColor(Qt::white);
                y1Pen.setColor(Qt::white);
                x2Pen.setColor(Qt::white);
                y2Pen.setColor(Qt::white);

                sx1Pen.setColor(QColor(200, 200, 255));
                sy1Pen.setColor(QColor(200, 200, 255));
                sx2Pen.setColor(QColor(200, 200, 255));
                sy2Pen.setColor(QColor(200, 200, 255));
            }

            m_math->xAxis->setSubTickPen(x1Pen);
            m_math->yAxis->setSubTickPen(y1Pen);
            m_math->xAxis2->setSubTickPen(x2Pen);
            m_math->yAxis2->setSubTickPen(y2Pen);

            m_math->xAxis->setSelectedSubTickPen(sx1Pen);
            m_math->yAxis->setSelectedSubTickPen(sy1Pen);
            m_math->xAxis2->setSelectedSubTickPen(sx2Pen);
            m_math->yAxis2->setSelectedSubTickPen(sy2Pen);
        }

        // Axis Grid Pen
        {
            QPen x1Pen = m_math->xAxis->grid()->pen();
            QPen y1Pen = m_math->yAxis->grid()->pen();
            QPen x2Pen = m_math->xAxis2->grid()->pen();
            QPen y2Pen = m_math->yAxis2->grid()->pen();

            if(m_background.color().lightness() > 150)
            {
                x1Pen.setColor(QColor(Qt::black).lighter());
                y1Pen.setColor(QColor(Qt::black).lighter());
                x2Pen.setColor(QColor(Qt::black).lighter());
                y2Pen.setColor(QColor(Qt::black).lighter());
            }
            else
            {
                x1Pen.setColor(QColor(Qt::white).darker());
                y1Pen.setColor(QColor(Qt::white).darker());
                x2Pen.setColor(QColor(Qt::white).darker());
                y2Pen.setColor(QColor(Qt::white).darker());
            }

            m_math->xAxis->grid()->setPen(x1Pen);
            m_math->yAxis->grid()->setPen(y1Pen);
            m_math->xAxis2->grid()->setPen(x2Pen);
            m_math->yAxis2->grid()->setPen(y2Pen);
        }

        // Axis Sub Grid Pen
        {
            QPen x1Pen = m_math->xAxis->grid()->subGridPen();
            QPen y1Pen = m_math->yAxis->grid()->subGridPen();
            QPen x2Pen = m_math->xAxis2->grid()->subGridPen();
            QPen y2Pen = m_math->yAxis2->grid()->subGridPen();

            if(m_background.color().lightness() > 150)
            {
                x1Pen.setColor(QColor(Qt::black).lighter());
                y1Pen.setColor(QColor(Qt::black).lighter());
                x2Pen.setColor(QColor(Qt::black).lighter());
                y2Pen.setColor(QColor(Qt::black).lighter());
            }
            else
            {
                x1Pen.setColor(QColor(Qt::white).darker());
                y1Pen.setColor(QColor(Qt::white).darker());
                x2Pen.setColor(QColor(Qt::white).darker());
                y2Pen.setColor(QColor(Qt::white).darker());
            }

            m_math->xAxis->grid()->setSubGridPen(x1Pen);
            m_math->yAxis->grid()->setSubGridPen(y1Pen);
            m_math->xAxis2->grid()->setSubGridPen(x2Pen);
            m_math->yAxis2->grid()->setSubGridPen(y2Pen);
        }

        // Axis Zero Line Pen
        {
            QPen x1Pen = m_math->xAxis->grid()->zeroLinePen();
            QPen y1Pen = m_math->yAxis->grid()->zeroLinePen();
            QPen x2Pen = m_math->xAxis2->grid()->zeroLinePen();
            QPen y2Pen = m_math->yAxis2->grid()->zeroLinePen();

            if(m_background.color().lightness() > 150)
            {
                x1Pen.setColor(QColor(Qt::black).lighter());
                y1Pen.setColor(QColor(Qt::black).lighter());
                x2Pen.setColor(QColor(Qt::black).lighter());
                y2Pen.setColor(QColor(Qt::black).lighter());
            }
            else
            {
                x1Pen.setColor(QColor(Qt::white).darker());
                y1Pen.setColor(QColor(Qt::white).darker());
                x2Pen.setColor(QColor(Qt::white).darker());
                y2Pen.setColor(QColor(Qt::white).darker());
            }

            m_math->xAxis->grid()->setZeroLinePen(x1Pen);
            m_math->yAxis->grid()->setZeroLinePen(y1Pen);
            m_math->xAxis2->grid()->setZeroLinePen(x2Pen);
            m_math->yAxis2->grid()->setZeroLinePen(y2Pen);
        }

        // Axis Label Color

        if(m_background.color().lightness() > 150)
        {
            m_math->xAxis->setLabelColor(Qt::black);
            m_math->yAxis->setLabelColor(Qt::black);
            m_math->xAxis2->setLabelColor(Qt::black);
            m_math->yAxis2->setLabelColor(Qt::black);

            m_math->xAxis->setTickLabelColor(Qt::black);
            m_math->yAxis->setTickLabelColor(Qt::black);
            m_math->xAxis2->setTickLabelColor(Qt::black);
            m_math->yAxis2->setTickLabelColor(Qt::black);

            m_math->xAxis->setSelectedLabelColor(QColor(000, 000, 255));
            m_math->yAxis->setSelectedLabelColor(QColor(000, 000, 255));
            m_math->xAxis2->setSelectedLabelColor(QColor(000, 000, 255));
            m_math->yAxis2->setSelectedLabelColor(QColor(000, 000, 255));

            m_math->xAxis->setSelectedTickLabelColor(QColor(000, 000, 255));
            m_math->yAxis->setSelectedTickLabelColor(QColor(000, 000, 255));
            m_math->xAxis2->setSelectedTickLabelColor(QColor(000, 000, 255));
            m_math->yAxis2->setSelectedTickLabelColor(QColor(000, 000, 255));
        }
        else
        {
            m_math->xAxis->setLabelColor(Qt::white);
            m_math->yAxis->setLabelColor(Qt::white);
            m_math->xAxis2->setLabelColor(Qt::white);
            m_math->yAxis2->setLabelColor(Qt::white);

            m_math->xAxis->setTickLabelColor(Qt::white);
            m_math->yAxis->setTickLabelColor(Qt::white);
            m_math->xAxis2->setTickLabelColor(Qt::white);
            m_math->yAxis2->setTickLabelColor(Qt::white);

            m_math->xAxis->setSelectedLabelColor(QColor(200, 200, 255));
            m_math->yAxis->setSelectedLabelColor(QColor(200, 200, 255));
            m_math->xAxis2->setSelectedLabelColor(QColor(200, 200, 255));
            m_math->yAxis2->setSelectedLabelColor(QColor(200, 200, 255));

            m_math->xAxis->setSelectedTickLabelColor(QColor(200, 200, 255));
            m_math->yAxis->setSelectedTickLabelColor(QColor(200, 200, 255));
            m_math->xAxis2->setSelectedTickLabelColor(QColor(200, 200, 255));
            m_math->yAxis2->setSelectedTickLabelColor(QColor(200, 200, 255));
        }

        // Legend Brush
        {
            QBrush brush = m_math->legend->brush();
            QBrush sbrush = m_math->legend->selectedBrush();

            brush.setColor(m_background.color());
            sbrush.setColor(m_background.color());

            m_math->legend->setBrush(brush);
            m_math->legend->setSelectedBrush(sbrush);
        }

        // Legend Border Pen
        {
            QPen pen = m_math->legend->borderPen();
            QPen spen = m_math->legend->selectedBorderPen();

            if(m_background.color().lightness() > 150)
            {
                pen.setColor(Qt::black);
                spen.setColor(QColor(000, 000, 255));
            }
            else
            {
                pen.setColor(Qt::white);
                spen.setColor(QColor(200, 200, 255));
            }

            m_math->legend->setBorderPen(pen);
            m_math->legend->setSelectedBorderPen(spen);
        }

        // Legend Icon Border Pen
        {
            QPen pen = m_math->legend->iconBorderPen();
            QPen spen = m_math->legend->selectedIconBorderPen();

            pen.setColor(m_background.color());
            spen.setColor(m_background.color());

            m_math->legend->setIconBorderPen(pen);
            m_math->legend->setSelectedIconBorderPen(spen);
        }

        // Legend Text Color

        if(m_background.color().lightness() > 150)
        {
            m_math->legend->setTextColor(Qt::black);
            m_math->legend->setSelectedTextColor(QColor(000, 000, 255));
        }
        else
        {
            m_math->legend->setTextColor(Qt::white);
            m_math->legend->setSelectedTextColor(QColor(200, 200, 255));
        }

        // Title Text Color

        if(m_background.color().lightness() > 150)
        {
            m_mathTitle->setTextColor(Qt::black);
            m_mathTitle->setSelectedTextColor(QColor(000, 000, 255));
        }
        else
        {
            m_mathTitle->setTextColor(Qt::white);
            m_mathTitle->setSelectedTextColor(QColor(200, 200, 255));
        }
    }

    updatePlots();
}

QRgb SerialOscilloscope::getBackgroundColor() const
{
    return m_background.color().rgb(); // NO APLHA!
}

bool SerialOscilloscope::setXAxisScaleType(QCPAxis::ScaleType scaleType)
{
    bool ok = notInvalid(__FUNCTION__, "scaleType", scaleType,
                         QCPAxis::stLinear, QCPAxis::stLogarithmic);

    if(ok)
    {
        if((m_plot->xAxis->scaleType() == QCPAxis::stLinear)
        && (scaleType != QCPAxis::stLinear))
        {
            m_plot->xAxis->setNumberPrecision(0);
            m_plot->xAxis2->setNumberPrecision(0);
        }

        if((m_plot->xAxis->scaleType() != QCPAxis::stLinear)
        && (scaleType == QCPAxis::stLinear))
        {
            m_plot->xAxis->setNumberPrecision(6);
            m_plot->xAxis2->setNumberPrecision(6);
        }

        m_plot->xAxis->setScaleType(scaleType);
        m_plot->xAxis2->setScaleType(scaleType);

        updatePlots();
    }

    return ok;
}

QCPAxis::ScaleType SerialOscilloscope::getXAxisScaleType() const
{
    return m_plot->xAxis->scaleType();
}

bool SerialOscilloscope::setXAxisScaleLogBase(double base)
{
    bool ok = (notInfinite(__FUNCTION__, "base", base)
            && notInvalid(__FUNCTION__, "base", base, 1.0,
                          std::numeric_limits<double>::max()));

    if(ok)
    {
        m_plot->xAxis->setScaleLogBase(base);
        m_plot->xAxis2->setScaleLogBase(base);

        updatePlots();
    }

    return ok;
}

double SerialOscilloscope::getXAxisScaleLogBase() const
{
    return m_plot->xAxis->scaleLogBase();
}

void SerialOscilloscope::setXAxisRangeAutoScale(bool on)
{
    m_plotXAutoScale = on;

    updatePlots();
}

bool SerialOscilloscope::getXAxisRangeAutoScale() const
{
    return m_plotXAutoScale;
}

bool SerialOscilloscope::setXAxisRangeLower(double lower)
{
    bool ok = notInvalid(__FUNCTION__, "lower", lower,
                         -m_plot->xAxis->range().maxRange,
                         +m_plot->xAxis->range().maxRange);

    if(ok)
    {
        m_plot->xAxis->setRangeLower(lower);
        m_plot->xAxis2->setRangeLower(lower);

        m_plotXAxisRangeBackup = m_plot->xAxis->range();

        updatePlots();
    }

    return ok;
}

double SerialOscilloscope::getXAxisRangeLower() const
{
    return m_plot->xAxis->range().lower;
}

bool SerialOscilloscope::setXAxisRangeUpper(double upper)
{
    bool ok = notInvalid(__FUNCTION__, "upper", upper,
                         -m_plot->xAxis->range().maxRange,
                         +m_plot->xAxis->range().maxRange);

    if(ok)
    {
        m_plot->xAxis->setRangeUpper(upper);
        m_plot->xAxis2->setRangeUpper(upper);

        m_plotXAxisRangeBackup = m_plot->xAxis->range();

        updatePlots();
    }

    return ok;
}

double SerialOscilloscope::getXAxisRangeUpper() const
{
    return m_plot->xAxis->range().upper;
}

void SerialOscilloscope::setXAxisRangeReversed(bool reversed)
{
    m_plot->xAxis->setRangeReversed(reversed);
    m_plot->xAxis2->setRangeReversed(reversed);

    updatePlots();
}

bool SerialOscilloscope::getXAxisRangeReversed() const
{
    return m_plot->xAxis->rangeReversed();
}

void SerialOscilloscope::setXAxisLabel(const QString &name)
{
    m_plot->xAxis->setLabel(name);

    updatePlots();
}

QString SerialOscilloscope::getXAxisLabel() const
{
    return m_plot->xAxis->label();
}

bool SerialOscilloscope::setYAxisScaleType(QCPAxis::ScaleType scaleType)
{
    bool ok = notInvalid(__FUNCTION__, "scaleType", scaleType,
                         QCPAxis::stLinear, QCPAxis::stLogarithmic);

    if(ok)
    {
        if((m_plot->yAxis->scaleType() == QCPAxis::stLinear)
        && (scaleType != QCPAxis::stLinear))
        {
            m_plot->yAxis->setNumberPrecision(0);
            m_plot->yAxis2->setNumberPrecision(0);
        }

        if((m_plot->yAxis->scaleType() != QCPAxis::stLinear)
        && (scaleType == QCPAxis::stLinear))
        {
            m_plot->yAxis->setNumberPrecision(6);
            m_plot->yAxis2->setNumberPrecision(6);
        }

        m_plot->yAxis->setScaleType(scaleType);
        m_plot->yAxis2->setScaleType(scaleType);

        updatePlots();
    }

    return ok;
}

QCPAxis::ScaleType SerialOscilloscope::getYAxisScaleType() const
{
    return m_plot->yAxis->scaleType();
}

bool SerialOscilloscope::setYAxisScaleLogBase(double base)
{
    bool ok = (notInfinite(__FUNCTION__, "base", base)
            && notInvalid(__FUNCTION__, "base", base, 1.0,
                          std::numeric_limits<double>::max()));

    if(ok)
    {
        m_plot->yAxis->setScaleLogBase(base);
        m_plot->yAxis2->setScaleLogBase(base);

        updatePlots();
    }

    return ok;
}

double SerialOscilloscope::getYAxisScaleLogBase() const
{
    return m_plot->yAxis->scaleLogBase();
}

void SerialOscilloscope::setYAxisRangeAutoScale(bool on)
{
    m_plotYAutoScale = on;

    updatePlots();
}

bool SerialOscilloscope::getYAxisRangeAutoScale() const
{
    return m_plotYAutoScale;
}

bool SerialOscilloscope::setYAxisRangeLower(double lower)
{
    bool ok = notInvalid(__FUNCTION__, "lower", lower,
                         -m_plot->yAxis->range().maxRange,
                         +m_plot->yAxis->range().maxRange);

    if(ok)
    {
        m_plot->yAxis->setRangeLower(lower);
        m_plot->yAxis2->setRangeLower(lower);

        m_plotYAxisRangeBackup = m_plot->yAxis->range();

        updatePlots();
    }

    return ok;
}

double SerialOscilloscope::getYAxisRangeLower() const
{
    return m_plot->yAxis->range().lower;
}

bool SerialOscilloscope::setYAxisRangeUpper(double upper)
{
    bool ok = notInvalid(__FUNCTION__, "upper", upper,
                         -m_plot->yAxis->range().maxRange,
                         +m_plot->yAxis->range().maxRange);

    if(ok)
    {
        m_plot->yAxis->setRangeUpper(upper);
        m_plot->yAxis2->setRangeUpper(upper);

        m_plotYAxisRangeBackup = m_plot->yAxis->range();

        updatePlots();
    }

    return ok;
}

double SerialOscilloscope::getYAxisRangeUpper() const
{
    return m_plot->yAxis->range().upper;
}

void SerialOscilloscope::setYAxisRangeReversed(bool reversed)
{
    m_plot->yAxis->setRangeReversed(reversed);
    m_plot->yAxis2->setRangeReversed(reversed);

    updatePlots();
}

bool SerialOscilloscope::getYAxisRangeReversed() const
{
    return m_plot->yAxis->rangeReversed();
}

void SerialOscilloscope::setYAxisLabel(const QString &name)
{
    m_plot->yAxis->setLabel(name);

    updatePlots();
}

QString SerialOscilloscope::getYAxisLabel() const
{
    return m_plot->yAxis->label();
}

bool SerialOscilloscope::setFFTXAxisScaleType(QCPAxis::ScaleType scaleType)
{
    bool ok = (FFTWEnabled(__FUNCTION__)
            && notInvalid(__FUNCTION__, "scaleType", scaleType,
                          QCPAxis::stLinear, QCPAxis::stLogarithmic));

    if(ok)
    {
        if((m_math->xAxis->scaleType() == QCPAxis::stLinear)
        && (scaleType != QCPAxis::stLinear))
        {
            m_math->xAxis->setNumberPrecision(0);
            m_math->xAxis2->setNumberPrecision(0);
        }

        if((m_math->xAxis->scaleType() != QCPAxis::stLinear)
        && (scaleType == QCPAxis::stLinear))
        {
            m_math->xAxis->setNumberPrecision(6);
            m_math->xAxis2->setNumberPrecision(6);
        }

        m_math->xAxis->setScaleType(scaleType);
        m_math->xAxis2->setScaleType(scaleType);

        updatePlots();
    }

    return ok;
}

QCPAxis::ScaleType SerialOscilloscope::getFFTXAxisScaleType() const
{
    return m_math->xAxis->scaleType();
}

bool SerialOscilloscope::setFFTXAxisScaleLogBase(double base)
{
    bool ok = (FFTWEnabled(__FUNCTION__)
            && notInfinite(__FUNCTION__, "base", base)
            && notInvalid(__FUNCTION__, "base", base, 1.0,
                          std::numeric_limits<double>::max()));

    if(ok)
    {
        m_math->xAxis->setScaleLogBase(base);
        m_math->xAxis2->setScaleLogBase(base);

        updatePlots();
    }

    return ok;
}

double SerialOscilloscope::getFFTXAxisScaleLogBase() const
{
    return m_math->xAxis->scaleLogBase();
}

bool SerialOscilloscope::setFFTXAxisRangeAutoScale(bool on)
{
    bool ok = FFTWEnabled(__FUNCTION__);

    if(ok)
    {
        m_mathXAutoScale = on;

        updatePlots();
    }

    return ok;
}

bool SerialOscilloscope::getFFTXAxisRangeAutoScale() const
{
    return m_mathXAutoScale;
}

bool SerialOscilloscope::setFFTXAxisRangeLower(double lower)
{
    bool ok = (FFTWEnabled(__FUNCTION__)
            && notInvalid(__FUNCTION__, "lower", lower,
                         -m_math->xAxis->range().maxRange,
                         +m_math->xAxis->range().maxRange));

    if(ok)
    {
        m_math->xAxis->setRangeLower(lower);
        m_math->xAxis2->setRangeLower(lower);

        m_mathXAxisRangeBackup = m_math->xAxis->range();

        updatePlots();
    }

    return ok;
}

double SerialOscilloscope::getFFTXAxisRangeLower() const
{
    return m_math->xAxis->range().lower;
}

bool SerialOscilloscope::setFFTXAxisRangeUpper(double upper)
{
    bool ok = (FFTWEnabled(__FUNCTION__)
            && notInvalid(__FUNCTION__, "upper", upper,
                         -m_math->xAxis->range().maxRange,
                         +m_math->xAxis->range().maxRange));

    if(ok)
    {
        m_math->xAxis->setRangeUpper(upper);
        m_math->xAxis2->setRangeUpper(upper);

        m_mathXAxisRangeBackup = m_math->xAxis->range();

        updatePlots();
    }

    return ok;
}

double SerialOscilloscope::getFFTXAxisRangeUpper() const
{
    return m_math->xAxis->range().upper;
}

bool SerialOscilloscope::setFFTXAxisRangeReversed(bool reversed)
{
    bool ok = FFTWEnabled(__FUNCTION__);

    if(ok)
    {
        m_math->xAxis->setRangeReversed(reversed);
        m_math->xAxis2->setRangeReversed(reversed);

        updatePlots();
    }

    return ok;
}

bool SerialOscilloscope::getFFTXAxisRangeReversed() const
{
    return m_math->xAxis->rangeReversed();
}

bool SerialOscilloscope::setFFTXAxisLabel(const QString &name)
{
    bool ok = FFTWEnabled(__FUNCTION__);

    if(ok)
    {
        m_math->xAxis->setLabel(name);

        updatePlots();
    }

    return ok;
}

QString SerialOscilloscope::getFFTXAxisLabel() const
{
    return m_math->xAxis->label();
}

bool SerialOscilloscope::setFFTYAxisScaleType(QCPAxis::ScaleType scaleType)
{
    bool ok = (FFTWEnabled(__FUNCTION__)
            && notInvalid(__FUNCTION__, "scaleType", scaleType,
                          QCPAxis::stLinear, QCPAxis::stLogarithmic));

    if(ok)
    {
        if((m_math->yAxis->scaleType() == QCPAxis::stLinear)
        && (scaleType != QCPAxis::stLinear))
        {
            m_math->yAxis->setNumberPrecision(0);
            m_math->yAxis2->setNumberPrecision(0);
        }

        if((m_math->yAxis->scaleType() != QCPAxis::stLinear)
        && (scaleType == QCPAxis::stLinear))
        {
            m_math->yAxis->setNumberPrecision(6);
            m_math->yAxis2->setNumberPrecision(6);
        }

        m_math->yAxis->setScaleType(scaleType);
        m_math->yAxis2->setScaleType(scaleType);

        updatePlots();
    }

    return ok;
}

QCPAxis::ScaleType SerialOscilloscope::getFFTYAxisScaleType() const
{
    return m_math->yAxis->scaleType();
}

bool SerialOscilloscope::setFFTYAxisScaleLogBase(double base)
{
    bool ok = (FFTWEnabled(__FUNCTION__)
            && notInfinite(__FUNCTION__, "base", base)
            && notInvalid(__FUNCTION__, "base", base, 1.0,
                          std::numeric_limits<double>::max()));

    if(ok)
    {
        m_math->yAxis->setScaleLogBase(base);
        m_math->yAxis2->setScaleLogBase(base);

        updatePlots();
    }

    return ok;
}

double SerialOscilloscope::getFFTYAxisScaleLogBase() const
{
    return m_math->yAxis->scaleLogBase();
}

bool SerialOscilloscope::setFFTYAxisRangeAutoScale(bool on)
{
    bool ok = FFTWEnabled(__FUNCTION__);

    if(ok)
    {
        m_mathYAutoScale = on;

        updatePlots();
    }

    return ok;
}

bool SerialOscilloscope::getFFTYAxisRangeAutoScale() const
{
    return m_mathYAutoScale;
}

bool SerialOscilloscope::setFFTYAxisRangeLower(double lower)
{
    bool ok = (FFTWEnabled(__FUNCTION__)
            && notInvalid(__FUNCTION__, "lower", lower,
                         -m_math->yAxis->range().maxRange,
                         +m_math->yAxis->range().maxRange));

    if(ok)
    {
        m_math->yAxis->setRangeLower(lower);
        m_math->yAxis2->setRangeLower(lower);

        m_mathYAxisRangeBackup = m_math->yAxis->range();

        updatePlots();
    }

    return ok;
}

double SerialOscilloscope::getFFTYAxisRangeLower() const
{
    return m_math->yAxis->range().lower;
}

bool SerialOscilloscope::setFFTYAxisRangeUpper(double upper)
{
    bool ok = (FFTWEnabled(__FUNCTION__)
            && notInvalid(__FUNCTION__, "upper", upper,
                         -m_math->yAxis->range().maxRange,
                         +m_math->yAxis->range().maxRange));

    if(ok)
    {
        m_math->yAxis->setRangeUpper(upper);
        m_math->yAxis2->setRangeUpper(upper);

        m_mathYAxisRangeBackup = m_math->yAxis->range();

        updatePlots();
    }

    return ok;
}

double SerialOscilloscope::getFFTYAxisRangeUpper() const
{
    return m_math->yAxis->range().upper;
}

bool SerialOscilloscope::setFFTYAxisRangeReversed(bool reversed)
{
    bool ok = FFTWEnabled(__FUNCTION__);

    if(ok)
    {
        m_math->yAxis->setRangeReversed(reversed);
        m_math->yAxis2->setRangeReversed(reversed);

        updatePlots();
    }

    return ok;
}

bool SerialOscilloscope::getFFTYAxisRangeReversed() const
{
    return m_math->yAxis->rangeReversed();
}

bool SerialOscilloscope::setFFTYAxisLabel(const QString &name)
{
    bool ok = FFTWEnabled(__FUNCTION__);

    if(ok)
    {
        m_math->yAxis->setLabel(name);

        updatePlots();
    }

    return ok;
}

QString SerialOscilloscope::getFFTYAxisLabel() const
{
    return m_math->yAxis->label();
}

bool SerialOscilloscope::newBar(int plottable, const QString &name)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    bool ok = plottableDoesNotExist(__FUNCTION__, plottable);

    if(!ok)
    {
        // Silently handle error...

        QCPAbstractPlottable *tempPlottable = m_plottables.value(plottable);

        if(qobject_cast<MyBars *>(tempPlottable))
        {
            ok = setBarName(plottable, name); updatePlots(); return ok;
        }
        else // Replace object instead of error...
        {
            deletePlot(plottable);
        }
    }

    MyBars *myPlottable =
    new MyBars(m_plot->xAxis, m_plot->yAxis);

    {
        int temp = (plottable % m_pensSize);

        if(temp < 0)
        {
            temp += m_pensSize;
        }

        myPlottable->setPen(m_pens[temp]);
    }

    {
        int temp = (plottable % m_spensSize);

        if(temp < 0)
        {
            temp += m_spensSize;
        }

        myPlottable->setSelectedPen(m_spens[temp]);
    }

    {
        int temp = (plottable % m_brushesSize);

        if(temp < 0)
        {
            temp += m_brushesSize;
        }

        myPlottable->setBrush(m_brushes[temp]);
    }

    {
        int temp = (plottable % m_sbrushesSize);

        if(temp < 0)
        {
            temp += m_sbrushesSize;
        }

        myPlottable->setSelectedBrush(m_sbrushes[temp]);
    }

    QMap<int, QCPAbstractPlottable *>::iterator
    i = m_plottables.insert(plottable, myPlottable),
    j = m_plottables.begin(); int index = std::distance(j, i);

    m_plot->insertPlottable(index, myPlottable);

    m_plot->legend->insertRow(index); m_plot->legend->addElement(index, 0,
    new QCPPlottableLegendItem(m_plot->legend, myPlottable));

    ///////////////////////////////////////////////////////////////////////

    QBrush brush = myPlottable->brush();
    QBrush sbrush = myPlottable->selectedBrush();

    brush.setStyle(Qt::SolidPattern);
    sbrush.setStyle(Qt::SolidPattern);

    myPlottable->setBrush(brush);
    myPlottable->setSelectedBrush(sbrush);

    ///////////////////////////////////////////////////////////////////////

    ok = setBarName(plottable, name);

    updatePlots();

    return ok;
}

bool SerialOscilloscope::newCurve(int plottable, const QString &name)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    bool ok = plottableDoesNotExist(__FUNCTION__, plottable);

    if(!ok)
    {
        // Silently handle error...

        QCPAbstractPlottable *tempPlottable = m_plottables.value(plottable);

        if(qobject_cast<MyCurve *>(tempPlottable))
        {
            ok = setCurveName(plottable, name); updatePlots(); return ok;
        }
        else // Replace object instead of error...
        {
            deletePlot(plottable);
        }
    }

    MyCurve *myPlottable =
    new MyCurve(m_plot->xAxis, m_plot->yAxis);

    {
        int temp = (plottable % m_pensSize);

        if(temp < 0)
        {
            temp += m_pensSize;
        }

        myPlottable->setPen(m_pens[temp]);
    }

    {
        int temp = (plottable % m_spensSize);

        if(temp < 0)
        {
            temp += m_spensSize;
        }

        myPlottable->setSelectedPen(m_spens[temp]);
    }

    {
        int temp = (plottable % m_brushesSize);

        if(temp < 0)
        {
            temp += m_brushesSize;
        }

        myPlottable->setBrush(m_brushes[temp]);
    }

    {
        int temp = (plottable % m_sbrushesSize);

        if(temp < 0)
        {
            temp += m_sbrushesSize;
        }

        myPlottable->setSelectedBrush(m_sbrushes[temp]);
    }

    QMap<int, QCPAbstractPlottable *>::iterator
    i = m_plottables.insert(plottable, myPlottable),
    j = m_plottables.begin(); int index = std::distance(j, i);

    m_plot->insertPlottable(index, myPlottable);

    m_plot->legend->insertRow(index); m_plot->legend->addElement(index, 0,
    new QCPPlottableLegendItem(m_plot->legend, myPlottable));

    ///////////////////////////////////////////////////////////////////////

    myPlottable->m_FFTWPlot =
    new QCPCurve(m_math->xAxis, m_math->yAxis);

    myPlottable->m_FFTWPlot->setPen(myPlottable->pen());
    myPlottable->m_FFTWPlot->setSelectedPen(myPlottable->selectedPen());
    myPlottable->m_FFTWPlot->setBrush(myPlottable->brush());
    myPlottable->m_FFTWPlot->setSelectedBrush(myPlottable->selectedBrush());

    index = 0; for(QMap<int, QCPAbstractPlottable *>::const_iterator
    k = m_plottables.constBegin(), l = m_plottables.constEnd();
    (k != l) && (k.value() != myPlottable); k++)
    {
        if(qobject_cast<MyCurve *>(k.value())
        || qobject_cast<MyGraph *>(k.value()))
        {
            index += 1;
        }
    }

    m_math->insertPlottable(index, myPlottable->m_FFTWPlot);

    m_math->legend->insertRow(index); m_math->legend->addElement(index, 0,
    new QCPPlottableLegendItem(m_math->legend, myPlottable->m_FFTWPlot));

    ///////////////////////////////////////////////////////////////////////

    ok = setCurveName(plottable, name);

    updatePlots();

    return ok;
}

bool SerialOscilloscope::newGraph(int plottable, const QString &name)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    bool ok = plottableDoesNotExist(__FUNCTION__, plottable);

    if(!ok)
    {
        // Silently handle error...

        QCPAbstractPlottable *tempPlottable = m_plottables.value(plottable);

        if(qobject_cast<MyGraph *>(tempPlottable))
        {
            ok = setGraphName(plottable, name); updatePlots(); return ok;
        }
        else // Replace object instead of error...
        {
            deletePlot(plottable);
        }
    }

    MyGraph *myPlottable =
    new MyGraph(m_plot->xAxis, m_plot->yAxis);

    {
        int temp = (plottable % m_pensSize);

        if(temp < 0)
        {
            temp += m_pensSize;
        }

        myPlottable->setPen(m_pens[temp]);
    }

    {
        int temp = (plottable % m_spensSize);

        if(temp < 0)
        {
            temp += m_spensSize;
        }

        myPlottable->setSelectedPen(m_spens[temp]);
    }

    {
        int temp = (plottable % m_brushesSize);

        if(temp < 0)
        {
            temp += m_brushesSize;
        }

        myPlottable->setBrush(m_brushes[temp]);
    }

    {
        int temp = (plottable % m_sbrushesSize);

        if(temp < 0)
        {
            temp += m_sbrushesSize;
        }

        myPlottable->setSelectedBrush(m_sbrushes[temp]);
    }

    QMap<int, QCPAbstractPlottable *>::iterator
    i = m_plottables.insert(plottable, myPlottable),
    j = m_plottables.begin(); int index = std::distance(j, i);

    m_plot->insertPlottable(index, myPlottable);

    m_plot->legend->insertRow(index); m_plot->legend->addElement(index, 0,
    new QCPPlottableLegendItem(m_plot->legend, myPlottable));

    ///////////////////////////////////////////////////////////////////////

    myPlottable->m_FFTWPlot =
    new QCPGraph(m_math->xAxis, m_math->yAxis);

    myPlottable->m_FFTWPlot->setPen(myPlottable->pen());
    myPlottable->m_FFTWPlot->setSelectedPen(myPlottable->selectedPen());
    myPlottable->m_FFTWPlot->setBrush(myPlottable->brush());
    myPlottable->m_FFTWPlot->setSelectedBrush(myPlottable->selectedBrush());

    index = 0; for(QMap<int, QCPAbstractPlottable *>::const_iterator
    k = m_plottables.constBegin(), l = m_plottables.constEnd();
    (k != l) && (k.value() != myPlottable); k++)
    {
        if(qobject_cast<MyCurve *>(k.value())
        || qobject_cast<MyGraph *>(k.value()))
        {
            index += 1;
        }
    }

    m_math->insertPlottable(index, myPlottable->m_FFTWPlot);

    m_math->legend->insertRow(index); m_math->legend->addElement(index, 0,
    new QCPPlottableLegendItem(m_math->legend, myPlottable->m_FFTWPlot));

    ///////////////////////////////////////////////////////////////////////

    ok = setGraphName(plottable, name);

    updatePlots();

    return ok;
}

bool SerialOscilloscope::newBox(int plottable, const QString &name)
{
    // This code was optimal once-upon-a-time... It has now been "tweaked" to
    // not error out on any problem and tries to solve it as best it can...

    bool ok = plottableDoesNotExist(__FUNCTION__, plottable);

    if(!ok)
    {
        // Silently handle error...

        QCPAbstractPlottable *tempPlottable = m_plottables.value(plottable);

        if(qobject_cast<MyStatisticalBox *>(tempPlottable))
        {
            ok = setBoxName(plottable, name); updatePlots(); return ok;
        }
        else // Replace object instead of error...
        {
            deletePlot(plottable);
        }
    }

    MyStatisticalBox *myPlottable =
    new MyStatisticalBox(m_plot->xAxis, m_plot->yAxis);

    {
        int temp = (plottable % m_pensSize);

        if(temp < 0)
        {
            temp += m_pensSize;
        }

        myPlottable->setPen(m_pens[temp]);
    }

    {
        int temp = (plottable % m_spensSize);

        if(temp < 0)
        {
            temp += m_spensSize;
        }

        myPlottable->setSelectedPen(m_spens[temp]);
    }

    {
        int temp = (plottable % m_brushesSize);

        if(temp < 0)
        {
            temp += m_brushesSize;
        }

        myPlottable->setBrush(m_brushes[temp]);
    }

    {
        int temp = (plottable % m_sbrushesSize);

        if(temp < 0)
        {
            temp += m_sbrushesSize;
        }

        myPlottable->setSelectedBrush(m_sbrushes[temp]);
    }

    QMap<int, QCPAbstractPlottable *>::iterator
    i = m_plottables.insert(plottable, myPlottable),
    j = m_plottables.begin(); int index = std::distance(j, i);

    m_plot->insertPlottable(index, myPlottable);

    m_plot->legend->insertRow(index); m_plot->legend->addElement(index, 0,
    new QCPPlottableLegendItem(m_plot->legend, myPlottable));

    ///////////////////////////////////////////////////////////////////////

    QBrush brush = myPlottable->brush();
    QBrush sbrush = myPlottable->selectedBrush();

    brush.setStyle(Qt::SolidPattern);
    sbrush.setStyle(Qt::SolidPattern);

    myPlottable->setBrush(brush);
    myPlottable->setSelectedBrush(sbrush);

    ///////////////////////////////////////////////////////////////////////

    ok = setBoxName(plottable, name);

    updatePlots();

    return ok;
}

bool SerialOscilloscope::setBarName(int plottable,
                                    const QString &name)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        myPlottable->setName(name);

        m_plot->legend->itemWithPlottable(myPlottable)->setVisible(
        !myPlottable->name().isEmpty());

        updatePlots();
    }

    return myPlottable;
}

QString SerialOscilloscope::getBarName(int plottable,
                                       bool *ok)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->name();
    }

    return QString();
}

bool SerialOscilloscope::setBarCoordinateSystem(int plottable,
                                                CoordinateSystem
                                                coordinateSystem)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
        "coordinateSystem", coordinateSystem, CS_CARTESIAN, CS_POLAR);

        if(ok)
        {
            myPlottable->m_coordinateSystem = coordinateSystem;

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

CoordinateSystem SerialOscilloscope::getBarCoordinateSystem(int plottable,
                                                            bool *ok)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_coordinateSystem;
    }

    return CoordinateSystem();
}

bool SerialOscilloscope::setBarAngleUnits(int plottable,
                                          AngleUnits
                                          angleUnits)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
                             "angleUnits", angleUnits, AU_DEGREES, AU_RADIANS);

        if(ok)
        {
            myPlottable->m_angleUnits = angleUnits;

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

AngleUnits SerialOscilloscope::getBarAngleUnits(int plottable,
                                                bool *ok)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_angleUnits;
    }

    return AngleUnits();
}

bool SerialOscilloscope::setBarLineColor(int plottable,
                                         QRgb rgba)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        QColor color = QColor::fromRgba(rgba);

        QPen pen = myPlottable->pen();
        QPen spen = myPlottable->selectedPen();

        pen.setColor(color);
        spen.setColor(color);

        myPlottable->setPen(pen);
        myPlottable->setSelectedPen(spen);

        updatePlots();
    }

    return myPlottable;
}

QRgb SerialOscilloscope::getBarLineColor(int plottable,
                                         bool *ok)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->pen().color().rgba();
    }

    return QRgb();
}

bool SerialOscilloscope::setBarLineStyle(int plottable,
                                         Qt::PenStyle lineStyle)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
        "lineStyle", lineStyle, Qt::NoPen, Qt::DashDotDotLine);

        if(ok)
        {
            QPen pen = myPlottable->pen();
            QPen spen = myPlottable->selectedPen();

            pen.setStyle(lineStyle);
            spen.setStyle(lineStyle);

            myPlottable->setPen(pen);
            myPlottable->setSelectedPen(spen);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

Qt::PenStyle SerialOscilloscope::getBarLineStyle(int plottable,
                                                 bool *ok)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->pen().style();
    }

    return Qt::PenStyle();
}

bool SerialOscilloscope::setBarFillColor(int plottable,
                                         QRgb rgba)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        QColor color = QColor::fromRgba(rgba);

        QBrush brush = myPlottable->brush();
        QBrush sbrush = myPlottable->selectedBrush();

        brush.setColor(color);
        sbrush.setColor(color);

        myPlottable->setBrush(brush);
        myPlottable->setSelectedBrush(sbrush);

        updatePlots();
    }

    return myPlottable;
}

QRgb SerialOscilloscope::getBarFillColor(int plottable,
                                         bool *ok)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->brush().color().rgba();
    }

    return QRgb();
}

bool SerialOscilloscope::setBarFillStyle(int plottable,
                                         Qt::BrushStyle fillStyle)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        int min0 = Qt::NoBrush;
        int max0 = Qt::SolidPattern;

        int min1 = Qt::HorPattern;
        int max1 = Qt::DiagCrossPattern;

        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
                             "fillStyle", fillStyle, min0, max0, min1, max1);

        if(ok)
        {
            QBrush brush = myPlottable->brush();
            QBrush sbrush = myPlottable->selectedBrush();

            brush.setStyle(fillStyle);
            sbrush.setStyle(fillStyle);

            myPlottable->setBrush(brush);
            myPlottable->setSelectedBrush(sbrush);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

Qt::BrushStyle SerialOscilloscope::getBarFillStyle(int plottable,
                                                   bool *ok)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->brush().style();
    }

    return Qt::BrushStyle();
}

bool SerialOscilloscope::setBarDeltaMode(int plottable,
                                         bool deltaMode)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        myPlottable->m_deltaMode = deltaMode;

        updatePlots();
    }

    return myPlottable;
}

bool SerialOscilloscope::getBarDeltaMode(int plottable,
                                         bool *ok)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_deltaMode;
    }

    return bool();
}

bool SerialOscilloscope::setBarSize(int plottable,
                                    int size)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
                             "size", size, 0, OSCILLOSCOPE_MAX_DATA_SIZE);

        if(ok)
        {
            myPlottable->m_size = size;

            while(myPlottable->data()->size() > myPlottable->m_size)
            {
                myPlottable->removeData(myPlottable->data()->firstKey());
            }

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

int SerialOscilloscope::getBarSize(int plottable,
                                   bool *ok)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_size;
    }

    return int();
}

bool SerialOscilloscope::setBarSampleRate(int plottable,
                                          double sampleRate)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "sampleRate", sampleRate)
                && notZero(__FUNCTION__, myPlottable->name(),
                           "sampleRate", sampleRate));

        if(ok)
        {
            myPlottable->m_sampleRate = (1.0 / sampleRate);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

double SerialOscilloscope::getBarSampleRate(int plottable,
                                            bool *ok)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return (1.0 / myPlottable->m_sampleRate);
    }

    return double();
}

bool SerialOscilloscope::setBarScaler(int plottable,
                                      double scaler)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "scaler", scaler)
                && notZero(__FUNCTION__, myPlottable->name(),
                           "scaler", scaler));

        if(ok)
        {
            myPlottable->m_scaler = (1.0 / scaler);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

double SerialOscilloscope::getBarScaler(int plottable,
                                        bool *ok)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return (1.0 / myPlottable->m_scaler);
    }

    return double();
}

bool SerialOscilloscope::setBarWidth(int plottable,
                                     double width)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, "width", width,
                             std::numeric_limits<double>::denorm_min(),
                             std::numeric_limits<double>::max());

        if(ok)
        {
            myPlottable->setWidth(width);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

double SerialOscilloscope::getBarWidth(int plottable,
                                       bool *ok)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->width();
    }

    return double();
}

bool SerialOscilloscope::setBarScatterStyle(int plottable,
                                            QCPScatterStyle::ScatterShape
                                            scatterShape)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
                             "scatterShape", scatterShape,
                             QCPScatterStyle::ssNone,
                             QCPScatterStyle::ssPeace);

        if(ok)
        {
            // myPlottable->setScatterStyle(scatterShape);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

QCPScatterStyle::ScatterShape SerialOscilloscope::getBarScatterStyle(
                              int plottable,
                              bool *ok)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        // return myPlottable->scatterStyle().shape();
    }

    return QCPScatterStyle::ScatterShape();
}

bool SerialOscilloscope::setBarBelow(int plottable,
                                     int targetPlottable)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        MyBars *myTargetPlottable = plottableCast
        <MyBars>(__FUNCTION__, targetPlottable);

        if(myTargetPlottable)
        {
            if(myPlottable == myTargetPlottable)
            {
                myPlottable->moveBelow(NULL);
            }
            else
            {
                myPlottable->moveBelow(myTargetPlottable);
            }

            updatePlots();
        }

        return myTargetPlottable;
    }

    return myPlottable;
}

int SerialOscilloscope::getBarBelow(int plottable,
                                    bool *ok)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return m_plottables.key(myPlottable->barBelow(), plottable);
    }

    return int();
}

bool SerialOscilloscope::setBarAbove(int plottable,
                                     int targetPlottable)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        MyBars *myTargetPlottable = plottableCast
        <MyBars>(__FUNCTION__, targetPlottable);

        if(myTargetPlottable)
        {
            if(myPlottable == myTargetPlottable)
            {
                myPlottable->moveAbove(NULL);
            }
            else
            {
                myPlottable->moveAbove(myTargetPlottable);
            }

            updatePlots();
        }

        return myTargetPlottable;
    }

    return myPlottable;
}

int SerialOscilloscope::getBarAbove(int plottable,
                                    bool *ok)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return m_plottables.key(myPlottable->barAbove(), plottable);
    }

    return int();
}

bool SerialOscilloscope::setCurveName(int plottable,
                                      const QString &name)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        myPlottable->setName(name); myPlottable->m_FFTWPlot->setName(name);

        m_plot->legend->itemWithPlottable(myPlottable)->setVisible(
        !myPlottable->name().isEmpty());

        m_math->legend->itemWithPlottable(
        myPlottable->m_FFTWPlot)->setVisible(
        (myPlottable->m_FFTWOption != CFFT_NONE) &&
        (!myPlottable->m_FFTWPlot->name().isEmpty()));

        updatePlots();
    }

    return myPlottable;
}

QString SerialOscilloscope::getCurveName(int plottable,
                                         bool *ok)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->name();
    }

    return QString();
}

bool SerialOscilloscope::setCurveCoordinateSystem(int plottable,
                                                  CoordinateSystem
                                                  coordinateSystem)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
        "coordinateSystem", coordinateSystem, CS_CARTESIAN, CS_POLAR);

        if(ok)
        {
            myPlottable->m_coordinateSystem = coordinateSystem;

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

CoordinateSystem SerialOscilloscope::getCurveCoordinateSystem(int plottable,
                                                              bool *ok)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_coordinateSystem;
    }

    return CoordinateSystem();
}

bool SerialOscilloscope::setCurveAngleUnits(int plottable,
                                            AngleUnits
                                            angleUnits)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
                             "angleUnits", angleUnits, AU_DEGREES, AU_RADIANS);

        if(ok)
        {
            myPlottable->m_angleUnits = angleUnits;

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

AngleUnits SerialOscilloscope::getCurveAngleUnits(int plottable,
                                                  bool *ok)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_angleUnits;
    }

    return AngleUnits();
}

bool SerialOscilloscope::setCurveLineColor(int plottable,
                                           QRgb rgba)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        QColor color = QColor::fromRgba(rgba);

        QPen pen = myPlottable->pen();
        QPen spen = myPlottable->selectedPen();

        pen.setColor(color);
        spen.setColor(color);

        myPlottable->setPen(pen);
        myPlottable->setSelectedPen(spen);

        myPlottable->m_FFTWPlot->setPen(pen);
        myPlottable->m_FFTWPlot->setSelectedPen(spen);

        updatePlots();
    }

    return myPlottable;
}

QRgb SerialOscilloscope::getCurveLineColor(int plottable,
                                           bool *ok)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->pen().color().rgba();
    }

    return QRgb();
}

bool SerialOscilloscope::setCurveLineStyle(int plottable,
                                           Qt::PenStyle lineStyle)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
        "lineStyle", lineStyle, Qt::NoPen, Qt::DashDotDotLine);

        if(ok)
        {
            QPen pen = myPlottable->pen();
            QPen spen = myPlottable->selectedPen();

            pen.setStyle(lineStyle);
            spen.setStyle(lineStyle);

            myPlottable->setPen(pen);
            myPlottable->setSelectedPen(spen);

            myPlottable->m_FFTWPlot->setPen(pen);
            myPlottable->m_FFTWPlot->setSelectedPen(spen);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

Qt::PenStyle SerialOscilloscope::getCurveLineStyle(int plottable,
                                                   bool *ok)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->pen().style();
    }

    return Qt::PenStyle();
}

bool SerialOscilloscope::setCurveFillColor(int plottable,
                                           QRgb rgba)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        QColor color = QColor::fromRgba(rgba);

        QBrush brush = myPlottable->brush();
        QBrush sbrush = myPlottable->selectedBrush();

        brush.setColor(color);
        sbrush.setColor(color);

        myPlottable->setBrush(brush);
        myPlottable->setSelectedBrush(sbrush);

        myPlottable->m_FFTWPlot->setBrush(brush);
        myPlottable->m_FFTWPlot->setSelectedBrush(sbrush);

        updatePlots();
    }

    return myPlottable;
}

QRgb SerialOscilloscope::getCurveFillColor(int plottable,
                                           bool *ok)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->brush().color().rgba();
    }

    return QRgb();
}

bool SerialOscilloscope::setCurveFillStyle(int plottable,
                                           Qt::BrushStyle fillStyle)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        int min0 = Qt::NoBrush;
        int max0 = Qt::SolidPattern;

        int min1 = Qt::HorPattern;
        int max1 = Qt::DiagCrossPattern;

        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
                             "fillStyle", fillStyle, min0, max0, min1, max1);

        if(ok)
        {
            QBrush brush = myPlottable->brush();
            QBrush sbrush = myPlottable->selectedBrush();

            brush.setStyle(fillStyle);
            sbrush.setStyle(fillStyle);

            myPlottable->setBrush(brush);
            myPlottable->setSelectedBrush(sbrush);

            myPlottable->m_FFTWPlot->setBrush(brush);
            myPlottable->m_FFTWPlot->setSelectedBrush(sbrush);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

Qt::BrushStyle SerialOscilloscope::getCurveFillStyle(int plottable,
                                                     bool *ok)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->brush().style();
    }

    return Qt::BrushStyle();
}

bool SerialOscilloscope::setCurveDeltaMode(int plottable,
                                           bool deltaMode)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        myPlottable->m_deltaMode = deltaMode;

        updatePlots();
    }

    return myPlottable;
}

bool SerialOscilloscope::getCurveDeltaMode(int plottable,
                                           bool *ok)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_deltaMode;
    }

    return bool();
}

bool SerialOscilloscope::setCurveSize(int plottable,
                                      int size)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
                             "size", size, 0, OSCILLOSCOPE_MAX_DATA_SIZE);

        if(ok)
        {
            myPlottable->m_size = size;

            while(myPlottable->data()->size() > myPlottable->m_size)
            {
                myPlottable->removeData(myPlottable->data()->firstKey());
            }

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

int SerialOscilloscope::getCurveSize(int plottable,
                                     bool *ok)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_size;
    }

    return int();
}

bool SerialOscilloscope::setCurveSampleRate(int plottable,
                                            double sampleRate)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "sampleRate", sampleRate)
                && notZero(__FUNCTION__, myPlottable->name(),
                           "sampleRate", sampleRate));

        if(ok)
        {
            myPlottable->m_sampleRate = (1.0 / sampleRate);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

double SerialOscilloscope::getCurveSampleRate(int plottable,
                                              bool *ok)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return (1.0 / myPlottable->m_sampleRate);
    }

    return double();
}

bool SerialOscilloscope::setCurveScaler(int plottable,
                                        double scaler)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "scaler", scaler)
                && notZero(__FUNCTION__, myPlottable->name(),
                           "scaler", scaler));

        if(ok)
        {
            myPlottable->m_scaler = (1.0 / scaler);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

double SerialOscilloscope::getCurveScaler(int plottable,
                                          bool *ok)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return (1.0 / myPlottable->m_scaler);
    }

    return double();
}

bool SerialOscilloscope::setCurveFFT(int plottable,
                                     CurveFFT option)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (FFTWEnabled(__FUNCTION__, myPlottable->name())
                && notInvalid(__FUNCTION__, myPlottable->name(),
                "option", option, CFFT_NONE, CFFT_BACKWARD));

        if(ok)
        {
            if((myPlottable->m_FFTWOption != CFFT_NONE)
            && (option == CFFT_NONE))
            {
                myPlottable->m_FFTWPlot->clearData();
            }

            if((myPlottable->m_FFTWOption == CFFT_NONE)
            && (option != CFFT_NONE))
            {
                myPlottable->m_FFTWPlot->clearData();
            }

            myPlottable->m_FFTWOption = option;

            m_math->legend->itemWithPlottable(
            myPlottable->m_FFTWPlot)->setVisible(
            (myPlottable->m_FFTWOption != CFFT_NONE) &&
            (!myPlottable->m_FFTWPlot->name().isEmpty()));

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

CurveFFT SerialOscilloscope::getCurveFFT(int plottable,
                                         bool *ok)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_FFTWOption;
    }

    return CurveFFT();
}

bool SerialOscilloscope::setCurveFFTWindowFunction(int plottable,
                                                   CurveFFTWF function)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (FFTWEnabled(__FUNCTION__, myPlottable->name())
                && notInvalid(__FUNCTION__, myPlottable->name(),
                "function",function,CFFTWF_RECTANGLE,CFFTWF_BLACKMAN_NUTTAL));

        if(ok)
        {
            myPlottable->m_FFTWFunction = function;

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

CurveFFTWF SerialOscilloscope::getCurveFFTWindowFunction(int plottable,
                                                         bool *ok)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_FFTWFunction;
    }

    return CurveFFTWF();
}

bool SerialOscilloscope::setCurveScatterStyle(int plottable,
                                              QCPScatterStyle::ScatterShape
                                              scatterShape)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
                             "scatterShape", scatterShape,
                             QCPScatterStyle::ssNone,
                             QCPScatterStyle::ssPeace);

        if(ok)
        {
            myPlottable->setScatterStyle(scatterShape);
            myPlottable->m_FFTWPlot->setScatterStyle(scatterShape);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

QCPScatterStyle::ScatterShape SerialOscilloscope::getCurveScatterStyle(
                              int plottable,
                              bool *ok)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->scatterStyle().shape();
    }

    return QCPScatterStyle::ScatterShape();
}

bool SerialOscilloscope::setCurveStyle(int plottable,
                                       QCPCurve::LineStyle style)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
        "style", style, QCPCurve::lsNone, QCPCurve::lsLine);

        if(ok)
        {
            myPlottable->setLineStyle(style);
            myPlottable->m_FFTWPlot->setLineStyle(style);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

QCPCurve::LineStyle SerialOscilloscope::getCurveStyle(int plottable,
                                                      bool *ok)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->lineStyle();
    }

    return QCPCurve::LineStyle();
}

bool SerialOscilloscope::setCurveChannelFillCurve(int plottable,
                                                  int targetPlottable)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        MyCurve *myTargetPlottable = plottableCast
        <MyCurve>(__FUNCTION__, targetPlottable);

        if(myTargetPlottable)
        {
            if(myPlottable == myTargetPlottable)
            {
                // myPlottable->setChannelFillCurve(NULL);
                // myPlottable->FFTWPlot->setChannelFillCurve(NULL);
            }
            else
            {
                // myPlottable->setChannelFillCurve(myTargetPlottable);
                // myPlottable->FFTWPlot->setChannelFillCurve(
                // myTargetPlottable->FFTWPlot);
            }

            updatePlots();
        }

        return myTargetPlottable;
    }

    return myPlottable;
}

int SerialOscilloscope::getCurveChannelFillCurve(int plottable,
                                                 bool *ok)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        // return m_plottables.key(myPlottable->channelFillCurve(), plottable);
    }

    return int();
}

bool SerialOscilloscope::setGraphName(int plottable,
                                      const QString &name)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        myPlottable->setName(name); myPlottable->m_FFTWPlot->setName(name);

        m_plot->legend->itemWithPlottable(myPlottable)->setVisible(
        !myPlottable->name().isEmpty());

        m_math->legend->itemWithPlottable(
        myPlottable->m_FFTWPlot)->setVisible(
        (myPlottable->m_FFTWOption != GFFT_NONE) &&
        (!myPlottable->m_FFTWPlot->name().isEmpty()));

        updatePlots();
    }

    return myPlottable;
}

QString SerialOscilloscope::getGraphName(int plottable,
                                         bool *ok)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->name();
    }

    return QString();
}

bool SerialOscilloscope::setGraphCoordinateSystem(int plottable,
                                                  CoordinateSystem
                                                  coordinateSystem)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
        "coordinateSystem", coordinateSystem, CS_CARTESIAN, CS_POLAR);

        if(ok)
        {
            myPlottable->m_coordinateSystem = coordinateSystem;

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

CoordinateSystem SerialOscilloscope::getGraphCoordinateSystem(int plottable,
                                                              bool *ok)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_coordinateSystem;
    }

    return CoordinateSystem();
}

bool SerialOscilloscope::setGraphAngleUnits(int plottable,
                                            AngleUnits
                                            angleUnits)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
                             "angleUnits", angleUnits, AU_DEGREES, AU_RADIANS);

        if(ok)
        {
            myPlottable->m_angleUnits = angleUnits;

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

AngleUnits SerialOscilloscope::getGraphAngleUnits(int plottable,
                                                  bool *ok)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_angleUnits;
    }

    return AngleUnits();
}

bool SerialOscilloscope::setGraphLineColor(int plottable,
                                           QRgb rgba)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        QColor color = QColor::fromRgba(rgba);

        QPen pen = myPlottable->pen();
        QPen spen = myPlottable->selectedPen();

        pen.setColor(color);
        spen.setColor(color);

        myPlottable->setPen(pen);
        myPlottable->setSelectedPen(spen);

        myPlottable->m_FFTWPlot->setPen(pen);
        myPlottable->m_FFTWPlot->setSelectedPen(spen);

        updatePlots();
    }

    return myPlottable;
}

QRgb SerialOscilloscope::getGraphLineColor(int plottable,
                                           bool *ok)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->pen().color().rgba();
    }

    return QRgb();
}

bool SerialOscilloscope::setGraphLineStyle(int plottable,
                                           Qt::PenStyle lineStyle)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
        "lineStyle", lineStyle, Qt::NoPen, Qt::DashDotDotLine);

        if(ok)
        {
            QPen pen = myPlottable->pen();
            QPen spen = myPlottable->selectedPen();

            pen.setStyle(lineStyle);
            spen.setStyle(lineStyle);

            myPlottable->setPen(pen);
            myPlottable->setSelectedPen(spen);

            myPlottable->m_FFTWPlot->setPen(pen);
            myPlottable->m_FFTWPlot->setSelectedPen(spen);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

Qt::PenStyle SerialOscilloscope::getGraphLineStyle(int plottable,
                                                   bool *ok)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->pen().style();
    }

    return Qt::PenStyle();
}

bool SerialOscilloscope::setGraphFillColor(int plottable,
                                           QRgb rgba)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        QColor color = QColor::fromRgba(rgba);

        QBrush brush = myPlottable->brush();
        QBrush sbrush = myPlottable->selectedBrush();

        brush.setColor(color);
        sbrush.setColor(color);

        myPlottable->setBrush(brush);
        myPlottable->setSelectedBrush(sbrush);

        myPlottable->m_FFTWPlot->setBrush(brush);
        myPlottable->m_FFTWPlot->setSelectedBrush(sbrush);

        updatePlots();
    }

    return myPlottable;
}

QRgb SerialOscilloscope::getGraphFillColor(int plottable,
                                           bool *ok)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->brush().color().rgba();
    }

    return QRgb();
}

bool SerialOscilloscope::setGraphFillStyle(int plottable,
                                           Qt::BrushStyle fillStyle)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        int min0 = Qt::NoBrush;
        int max0 = Qt::SolidPattern;

        int min1 = Qt::HorPattern;
        int max1 = Qt::DiagCrossPattern;

        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
                             "fillStyle", fillStyle, min0, max0, min1, max1);

        if(ok)
        {
            QBrush brush = myPlottable->brush();
            QBrush sbrush = myPlottable->selectedBrush();

            brush.setStyle(fillStyle);
            sbrush.setStyle(fillStyle);

            myPlottable->setBrush(brush);
            myPlottable->setSelectedBrush(sbrush);

            myPlottable->m_FFTWPlot->setBrush(brush);
            myPlottable->m_FFTWPlot->setSelectedBrush(sbrush);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

Qt::BrushStyle SerialOscilloscope::getGraphFillStyle(int plottable,
                                                     bool *ok)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->brush().style();
    }

    return Qt::BrushStyle();
}

bool SerialOscilloscope::setGraphDeltaMode(int plottable,
                                           bool deltaMode)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        myPlottable->m_deltaMode = deltaMode;

        updatePlots();
    }

    return myPlottable;
}

bool SerialOscilloscope::getGraphDeltaMode(int plottable,
                                           bool *ok)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_deltaMode;
    }

    return bool();
}

bool SerialOscilloscope::setGraphSize(int plottable,
                                      int size)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
                             "size", size, 0, OSCILLOSCOPE_MAX_DATA_SIZE);

        if(ok)
        {
            myPlottable->m_size = size;

            while(myPlottable->data()->size() > myPlottable->m_size)
            {
                myPlottable->removeData(myPlottable->data()->firstKey());
            }

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

int SerialOscilloscope::getGraphSize(int plottable,
                                     bool *ok)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_size;
    }

    return int();
}

bool SerialOscilloscope::setGraphSampleRate(int plottable,
                                            double sampleRate)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "sampleRate", sampleRate)
                && notZero(__FUNCTION__, myPlottable->name(),
                           "sampleRate", sampleRate));

        if(ok)
        {
            myPlottable->m_sampleRate = (1.0 / sampleRate);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

double SerialOscilloscope::getGraphSampleRate(int plottable,
                                              bool *ok)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return (1.0 / myPlottable->m_sampleRate);
    }

    return double();
}

bool SerialOscilloscope::setGraphScaler(int plottable,
                                        double scaler)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "scaler", scaler)
                && notZero(__FUNCTION__, myPlottable->name(),
                           "scaler", scaler));

        if(ok)
        {
            myPlottable->m_scaler = (1.0 / scaler);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

double SerialOscilloscope::getGraphScaler(int plottable,
                                          bool *ok)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return (1.0 / myPlottable->m_scaler);
    }

    return double();
}

bool SerialOscilloscope::setGraphFFT(int plottable,
                                     GraphFFT option)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (FFTWEnabled(__FUNCTION__, myPlottable->name())
                && notInvalid(__FUNCTION__, myPlottable->name(),
                "option", option, GFFT_NONE, GFFT_PHASE_DEGREES));

        if(ok)
        {
            if((myPlottable->m_FFTWOption != GFFT_NONE)
            && (option == GFFT_NONE))
            {
                myPlottable->m_FFTWPlot->clearData();
            }

            if((myPlottable->m_FFTWOption == GFFT_NONE)
            && (option != GFFT_NONE))
            {
                myPlottable->m_FFTWPlot->clearData();
            }

            myPlottable->m_FFTWOption = option;

            m_math->legend->itemWithPlottable(
            myPlottable->m_FFTWPlot)->setVisible(
            (myPlottable->m_FFTWOption != GFFT_NONE) &&
            (!myPlottable->m_FFTWPlot->name().isEmpty()));

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

GraphFFT SerialOscilloscope::getGraphFFT(int plottable,
                                         bool *ok)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_FFTWOption;
    }

    return GraphFFT();
}

bool SerialOscilloscope::setGraphFFTWindowFunction(int plottable,
                                                   GraphFFTWF function)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (FFTWEnabled(__FUNCTION__, myPlottable->name())
                && notInvalid(__FUNCTION__, myPlottable->name(),
                "function",function,GFFTWF_RECTANGLE,GFFTWF_BLACKMAN_NUTTAL));

        if(ok)
        {
            myPlottable->m_FFTWFunction = function;

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

GraphFFTWF SerialOscilloscope::getGraphFFTWindowFunction(int plottable,
                                                         bool *ok)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_FFTWFunction;
    }

    return GraphFFTWF();
}

bool SerialOscilloscope::setGraphScatterStyle(int plottable,
                                              QCPScatterStyle::ScatterShape
                                              scatterShape)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
                             "scatterShape", scatterShape,
                             QCPScatterStyle::ssNone,
                             QCPScatterStyle::ssPeace);

        if(ok)
        {
            myPlottable->setScatterStyle(scatterShape);
            myPlottable->m_FFTWPlot->setScatterStyle(scatterShape);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

QCPScatterStyle::ScatterShape SerialOscilloscope::getGraphScatterStyle(
                              int plottable,
                              bool *ok)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->scatterStyle().shape();
    }

    return QCPScatterStyle::ScatterShape();
}

bool SerialOscilloscope::setGraphStyle(int plottable,
                                       QCPGraph::LineStyle style)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
        "style", style, QCPGraph::lsNone, QCPGraph::lsImpulse);

        if(ok)
        {
            myPlottable->setLineStyle(style);
            myPlottable->m_FFTWPlot->setLineStyle(style);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

QCPGraph::LineStyle SerialOscilloscope::getGraphStyle(int plottable,
                                                      bool *ok)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->lineStyle();
    }

    return QCPGraph::LineStyle();
}

bool SerialOscilloscope::setGraphChannelFillGraph(int plottable,
                                                  int targetPlottable)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        MyGraph *myTargetPlottable = plottableCast
        <MyGraph>(__FUNCTION__, targetPlottable);

        if(myTargetPlottable)
        {
            if(myPlottable == myTargetPlottable)
            {
                myPlottable->setChannelFillGraph(NULL);
                myPlottable->m_FFTWPlot->setChannelFillGraph(NULL);
            }
            else
            {
                myPlottable->setChannelFillGraph(myTargetPlottable);
                myPlottable->m_FFTWPlot->setChannelFillGraph(
                myTargetPlottable->m_FFTWPlot);
            }

            updatePlots();
        }

        return myTargetPlottable;
    }

    return myPlottable;
}

int SerialOscilloscope::getGraphChannelFillGraph(int plottable,
                                                 bool *ok)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return m_plottables.key(myPlottable->channelFillGraph(), plottable);
    }

    return int();
}

bool SerialOscilloscope::setGraphKeyErrors(int plottable,
                                           bool keyErrors)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        myPlottable->setErrorType(static_cast<QCPGraph::ErrorType>(
        (myPlottable->errorType() & (~QCPGraph::etKey)) |
        (keyErrors ? QCPGraph::etKey : QCPGraph::etNone)));

        myPlottable->m_FFTWPlot->setErrorType(myPlottable->errorType());

        updatePlots();
    }

    return myPlottable;
}

bool SerialOscilloscope::getGraphKeyErrors(int plottable,
                                           bool *ok)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->errorType() & QCPGraph::etKey;
    }

    return bool();
}

bool SerialOscilloscope::setGraphValueErrors(int plottable,
                                             bool valueErrors)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        myPlottable->setErrorType(static_cast<QCPGraph::ErrorType>(
        (myPlottable->errorType() & (~QCPGraph::etValue)) |
        (valueErrors ? QCPGraph::etValue : QCPGraph::etNone)));

        myPlottable->m_FFTWPlot->setErrorType(myPlottable->errorType());

        updatePlots();
    }

    return myPlottable;
}

bool SerialOscilloscope::getGraphValueErrors(int plottable,
                                             bool *ok)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->errorType() & QCPGraph::etValue;
    }

    return bool();
}

bool SerialOscilloscope::setBoxName(int plottable,
                                    const QString &name)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        myPlottable->setName(name);

        m_plot->legend->itemWithPlottable(myPlottable)->setVisible(
        !myPlottable->name().isEmpty());

        updatePlots();
    }

    return myPlottable;
}

QString SerialOscilloscope::getBoxName(int plottable,
                                       bool *ok)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->name();
    }

    return QString();
}

bool SerialOscilloscope::setBoxCoordinateSystem(int plottable,
                                                CoordinateSystem
                                                coordinateSystem)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
        "coordinateSystem", coordinateSystem, CS_CARTESIAN, CS_POLAR);

        if(ok)
        {
            myPlottable->m_coordinateSystem = coordinateSystem;

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

CoordinateSystem SerialOscilloscope::getBoxCoordinateSystem(int plottable,
                                                            bool *ok)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_coordinateSystem;
    }

    return CoordinateSystem();
}

bool SerialOscilloscope::setBoxAngleUnits(int plottable,
                                          AngleUnits
                                          angleUnits)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
                             "angleUnits", angleUnits, AU_DEGREES, AU_RADIANS);

        if(ok)
        {
            myPlottable->m_angleUnits = angleUnits;

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

AngleUnits SerialOscilloscope::getBoxAngleUnits(int plottable,
                                                bool *ok)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_angleUnits;
    }

    return AngleUnits();
}

bool SerialOscilloscope::setBoxLineColor(int plottable,
                                         QRgb rgba)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        QColor color = QColor::fromRgba(rgba);

        QPen pen = myPlottable->pen();
        QPen spen = myPlottable->selectedPen();

        pen.setColor(color);
        spen.setColor(color);

        myPlottable->setPen(pen);
        myPlottable->setSelectedPen(spen);

        updatePlots();
    }

    return myPlottable;
}

QRgb SerialOscilloscope::getBoxLineColor(int plottable,
                                         bool *ok)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->pen().color().rgba();
    }

    return QRgb();
}

bool SerialOscilloscope::setBoxLineStyle(int plottable,
                                         Qt::PenStyle lineStyle)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
        "lineStyle", lineStyle, Qt::NoPen, Qt::DashDotDotLine);

        if(ok)
        {
            QPen pen = myPlottable->pen();
            QPen spen = myPlottable->selectedPen();

            pen.setStyle(lineStyle);
            spen.setStyle(lineStyle);

            myPlottable->setPen(pen);
            myPlottable->setSelectedPen(spen);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

Qt::PenStyle SerialOscilloscope::getBoxLineStyle(int plottable,
                                                 bool *ok)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->pen().style();
    }

    return Qt::PenStyle();
}

bool SerialOscilloscope::setBoxFillColor(int plottable,
                                         QRgb rgba)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        QColor color = QColor::fromRgba(rgba);

        QBrush brush = myPlottable->brush();
        QBrush sbrush = myPlottable->selectedBrush();

        brush.setColor(color);
        sbrush.setColor(color);

        myPlottable->setBrush(brush);
        myPlottable->setSelectedBrush(sbrush);

        updatePlots();
    }

    return myPlottable;
}

QRgb SerialOscilloscope::getBoxFillColor(int plottable,
                                         bool *ok)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->brush().color().rgba();
    }

    return QRgb();
}

bool SerialOscilloscope::setBoxFillStyle(int plottable,
                                         Qt::BrushStyle fillStyle)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        int min0 = Qt::NoBrush;
        int max0 = Qt::SolidPattern;

        int min1 = Qt::HorPattern;
        int max1 = Qt::DiagCrossPattern;

        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
                             "fillStyle", fillStyle, min0, max0, min1, max1);

        if(ok)
        {
            QBrush brush = myPlottable->brush();
            QBrush sbrush = myPlottable->selectedBrush();

            brush.setStyle(fillStyle);
            sbrush.setStyle(fillStyle);

            myPlottable->setBrush(brush);
            myPlottable->setSelectedBrush(sbrush);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

Qt::BrushStyle SerialOscilloscope::getBoxFillStyle(int plottable,
                                                   bool *ok)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->brush().style();
    }

    return Qt::BrushStyle();
}

bool SerialOscilloscope::setBoxDeltaMode(int plottable,
                                         bool deltaMode)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        myPlottable->m_deltaMode = deltaMode;

        updatePlots();
    }

    return myPlottable;
}

bool SerialOscilloscope::getBoxDeltaMode(int plottable,
                                         bool *ok)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_deltaMode;
    }

    return bool();
}

bool SerialOscilloscope::setBoxSize(int plottable,
                                    int size)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
                             "size", size, 0, OSCILLOSCOPE_MAX_DATA_SIZE);

        if(ok)
        {
            myPlottable->m_size = size;

            while(myPlottable->m_outs.size() > myPlottable->m_size)
            {
                myPlottable->m_outs.remove(myPlottable->m_outs.firstKey());
            }

            myPlottable->update(); updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

int SerialOscilloscope::getBoxSize(int plottable,
                                   bool *ok)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->m_size;
    }

    return int();
}

bool SerialOscilloscope::setBoxSampleRate(int plottable,
                                          double sampleRate)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "sampleRate", sampleRate)
                && notZero(__FUNCTION__, myPlottable->name(),
                           "sampleRate", sampleRate));

        if(ok)
        {
            myPlottable->m_sampleRate = (1.0 / sampleRate);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

double SerialOscilloscope::getBoxSampleRate(int plottable,
                                            bool *ok)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return (1.0 / myPlottable->m_sampleRate);
    }

    return double();
}

bool SerialOscilloscope::setBoxScaler(int plottable,
                                      double scaler)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "scaler", scaler)
                && notZero(__FUNCTION__, myPlottable->name(),
                           "scaler", scaler));

        if(ok)
        {
            double minimum = myPlottable->minimum() /
                             myPlottable->m_scaler;

            double lowerQuartile = myPlottable->lowerQuartile() /
                                   myPlottable->m_scaler;

            double median = myPlottable->median() /
                            myPlottable->m_scaler;

            double upperQuartile = myPlottable->upperQuartile() /
                                   myPlottable->m_scaler;

            double maximum = myPlottable->maximum() /
                             myPlottable->m_scaler;

            myPlottable->m_scaler = (1.0 / scaler);

            myPlottable->setMinimum(minimum *
                                    myPlottable->m_scaler);

            myPlottable->setLowerQuartile(lowerQuartile *
                                          myPlottable->m_scaler);

            myPlottable->setMedian(median *
                                   myPlottable->m_scaler);

            myPlottable->setUpperQuartile(upperQuartile *
                                          myPlottable->m_scaler);

            myPlottable->setMaximum(maximum *
                                    myPlottable->m_scaler);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

double SerialOscilloscope::getBoxScaler(int plottable,
                                        bool *ok)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return (1.0 / myPlottable->m_scaler);
    }

    return double();
}

bool SerialOscilloscope::setBoxWidth(int plottable,
                                     double width)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, "width", width,
                             std::numeric_limits<double>::denorm_min(),
                             std::numeric_limits<double>::max());

        if(ok)
        {
            myPlottable->setWhiskerWidth(width *
            (myPlottable->whiskerWidth() / myPlottable->width()));
            myPlottable->setWidth(width);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

double SerialOscilloscope::getBoxWidth(int plottable,
                                       bool *ok)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->width();
    }

    return double();
}

bool SerialOscilloscope::setBoxScatterStyle(int plottable,
                                            QCPScatterStyle::ScatterShape
                                            scatterShape)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInvalid(__FUNCTION__, myPlottable->name(),
                             "scatterShape", scatterShape,
                             QCPScatterStyle::ssNone,
                             QCPScatterStyle::ssPeace);

        if(ok)
        {
            myPlottable->setOutlierStyle(scatterShape);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

QCPScatterStyle::ScatterShape SerialOscilloscope::getBoxScatterStyle(
                              int plottable,
                              bool *ok)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->outlierStyle().shape();
    }

    return QCPScatterStyle::ScatterShape();
}

bool SerialOscilloscope::setBoxKey(int plottable,
                                   double key)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInfinite(__FUNCTION__, myPlottable->name(),
                              "key", key);

        if(ok)
        {
            myPlottable->setKey(key);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

double SerialOscilloscope::getBoxKey(int plottable,
                                     bool *ok)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->key();
    }

    return double();
}

bool SerialOscilloscope::setBoxMinimum(int plottable,
                                       double minimum)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInfinite(__FUNCTION__, myPlottable->name(),
                              "minimum", minimum);

        if(ok)
        {
            myPlottable->setMinimum(minimum*myPlottable->m_scaler);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

double SerialOscilloscope::getBoxMinimum(int plottable,
                                         bool *ok)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->maximum()/myPlottable->m_scaler;
    }

    return double();
}

bool SerialOscilloscope::setBoxLowerQuartile(int plottable,
                                             double lowerQuartile)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInfinite(__FUNCTION__, myPlottable->name(),
                              "lowerQuartile", lowerQuartile);

        if(ok)
        {
            myPlottable->setLowerQuartile(lowerQuartile*myPlottable->m_scaler);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

double SerialOscilloscope::getBoxLowerQuartile(int plottable,
                                               bool *ok)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->lowerQuartile()/myPlottable->m_scaler;
    }

    return double();
}

bool SerialOscilloscope::setBoxMedian(int plottable,
                                      double median)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInfinite(__FUNCTION__, myPlottable->name(),
                              "median", median);

        if(ok)
        {
            myPlottable->setMedian(median*myPlottable->m_scaler);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

double SerialOscilloscope::getBoxMedian(int plottable,
                                        bool *ok)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->median()/myPlottable->m_scaler;
    }

    return double();
}

bool SerialOscilloscope::setBoxUpperQuartile(int plottable,
                                             double upperQuartile)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInfinite(__FUNCTION__, myPlottable->name(),
                              "upperQuartile", upperQuartile);

        if(ok)
        {
            myPlottable->setUpperQuartile(upperQuartile*myPlottable->m_scaler);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

double SerialOscilloscope::getBoxUpperQuartile(int plottable,
                                               bool *ok)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->upperQuartile()/myPlottable->m_scaler;
    }

    return double();
}

bool SerialOscilloscope::setBoxMaximum(int plottable,
                                       double maximum)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInfinite(__FUNCTION__, myPlottable->name(),
                              "maximum", maximum);

        if(ok)
        {
            myPlottable->setMaximum(maximum*myPlottable->m_scaler);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

double SerialOscilloscope::getBoxMaximum(int plottable,
                                         bool *ok)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if((*ok = myPlottable))
    {
        return myPlottable->maximum()/myPlottable->m_scaler;
    }

    return double();
}

bool SerialOscilloscope::deletePlot(int plottable)
{
    QCPAbstractPlottable *aPlottable =
    plottableExists(__FUNCTION__, plottable);

    if(aPlottable)
    {
        if(qobject_cast<MyCurve *>(aPlottable))
        {
            m_math->removePlottable(static_cast
            <MyCurve *>(aPlottable)->m_FFTWPlot);
        }
        else if(qobject_cast<MyGraph *>(aPlottable))
        {
            m_math->removePlottable(static_cast
            <MyGraph *>(aPlottable)->m_FFTWPlot);
        }

        m_plottables.remove(plottable);
        m_plot->removePlottable(aPlottable);

        updatePlots();
    }

    return aPlottable;
}

void SerialOscilloscope::deletePlots()
{
    foreach(int plottable, m_plottables.keys())
    {
        deletePlot(plottable);
    }

    updatePlots();
}

bool SerialOscilloscope::addBarData(int plottable,
                                    double key,
                                    double value)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "key", key)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "value", value));

        if(ok)
        {
            if(myPlottable->m_coordinateSystem == CS_POLAR)
            {
                double temp = key;

                if(myPlottable->m_angleUnits == AU_DEGREES)
                {
                    temp = qDegreesToRadians(temp);
                }

                value = value * qSin(temp) * myPlottable->m_scaler;
            }
            else
            {
                value *= myPlottable->m_scaler;
            }

            const_cast<QCPBarDataMap *>(myPlottable->data())->insert(
            key, QCPBarData(key, value));

            while(myPlottable->data()->size() > myPlottable->m_size)
            {
                myPlottable->removeData(myPlottable->data()->firstKey());
            }

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::addBarData(int plottable,
                                    double value)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInfinite(__FUNCTION__, myPlottable->name(),
                              "value", value);

        if(ok)
        {
            double key = 0.0;

            if(!myPlottable->data()->isEmpty())
            {
                key = myPlottable->data()->lastKey() +
                      myPlottable->m_sampleRate;
            }
            else if(myPlottable->m_deltaMode)
            {
                key = -(myPlottable->m_size * myPlottable->m_sampleRate);
            }

            if(myPlottable->m_coordinateSystem == CS_POLAR)
            {
                double temp = key;

                if(myPlottable->m_angleUnits == AU_DEGREES)
                {
                    temp = qDegreesToRadians(temp);
                }

                value = value * qSin(temp) * myPlottable->m_scaler;
            }
            else
            {
                value *= myPlottable->m_scaler;
            }

            if(myPlottable->m_deltaMode
            && (!myPlottable->data()->isEmpty())
            && (myPlottable->data()->size() == myPlottable->m_size))
            {
                QCPBarDataMap::iterator i =
                const_cast<QCPBarDataMap *>(myPlottable->data())->begin()+1;

                QCPBarDataMap::iterator j =
                const_cast<QCPBarDataMap *>(myPlottable->data())->end();

                for(QCPBarDataMap::iterator k = (j + 1); i != k; i++)
                {
                    QCPBarDataMap::iterator l = (i - 1);

                    l.value().value =
                    (i==j) ? value : i.value().value;
                }
            }
            else
            {
                const_cast<QCPBarDataMap *>(myPlottable->data())->insert(
                key, QCPBarData(key, value));

                while(myPlottable->data()->size() > myPlottable->m_size)
                {
                    myPlottable->removeData(myPlottable->data()->firstKey());
                }
            }

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::addCurveData(int plottable,
                                      double index,
                                      double key,
                                      double value)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "index", index)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "key", key)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "value", value));

        if(ok)
        {
            if(myPlottable->m_coordinateSystem == CS_POLAR)
            {
                double temp = key;

                if(myPlottable->m_angleUnits == AU_DEGREES)
                {
                    temp = qDegreesToRadians(temp);
                }

                key = value * qCos(temp) * myPlottable->m_scaler;
                value = value * qSin(temp) * myPlottable->m_scaler;
            }
            else
            {
                value *= myPlottable->m_scaler;
            }

            const_cast<QCPCurveDataMap *>(myPlottable->data())->insert(
            index, QCPCurveData(index, key, value));

            while(myPlottable->data()->size() > myPlottable->m_size)
            {
                myPlottable->removeData(myPlottable->data()->firstKey());
            }

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::addCurveData(int plottable,
                                      double key,
                                      double value)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "key", key)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "value", value));

        if(ok)
        {
            double index = 0.0;

            if(!myPlottable->data()->isEmpty())
            {
                index = myPlottable->data()->lastKey() +
                        myPlottable->m_sampleRate;
            }
            else if(myPlottable->m_deltaMode)
            {
                index = -(myPlottable->m_size * myPlottable->m_sampleRate);
            }

            if(myPlottable->m_coordinateSystem == CS_POLAR)
            {
                double temp = key;

                if(myPlottable->m_angleUnits == AU_DEGREES)
                {
                    temp = qDegreesToRadians(temp);
                }

                key = value * qCos(temp) * myPlottable->m_scaler;
                value = value * qSin(temp) * myPlottable->m_scaler;
            }
            else
            {
                value *= myPlottable->m_scaler;
            }

            if(myPlottable->m_deltaMode
            && (!myPlottable->data()->isEmpty())
            && (myPlottable->data()->size() == myPlottable->m_size))
            {
                QCPCurveDataMap::iterator i =
                const_cast<QCPCurveDataMap *>(myPlottable->data())->begin()+1;

                QCPCurveDataMap::iterator j =
                const_cast<QCPCurveDataMap *>(myPlottable->data())->end();

                for(QCPCurveDataMap::iterator k = (j + 1); i != k; i++)
                {
                    QCPCurveDataMap::iterator l = (i - 1);

                    l.value().key =
                    (i==j) ? key : i.value().key;

                    l.value().value =
                    (i==j) ? value : i.value().value;
                }
            }
            else
            {
                const_cast<QCPCurveDataMap *>(myPlottable->data())->insert(
                index, QCPCurveData(index, key, value));

                while(myPlottable->data()->size() > myPlottable->m_size)
                {
                    myPlottable->removeData(myPlottable->data()->firstKey());
                }
            }

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::addGraphData(int plottable,
                                      double key,
                                      double value)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "key", key)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "value", value));

        if(ok)
        {
            if(myPlottable->m_coordinateSystem == CS_POLAR)
            {
                double temp = key;

                if(myPlottable->m_angleUnits == AU_DEGREES)
                {
                    temp = qDegreesToRadians(temp);
                }

                value = value * qSin(temp) * myPlottable->m_scaler;
            }
            else
            {
                value *= myPlottable->m_scaler;
            }

            const_cast<QCPDataMap *>(myPlottable->data())->insert(
            key, QCPData(key, value));

            while(myPlottable->data()->size() > myPlottable->m_size)
            {
                myPlottable->removeData(myPlottable->data()->firstKey());
            }

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::addGraphData(int plottable,
                                      double value)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInfinite(__FUNCTION__, myPlottable->name(),
                              "value", value);

        if(ok)
        {
            double key = 0.0;

            if(!myPlottable->data()->isEmpty())
            {
                key = myPlottable->data()->lastKey() +
                      myPlottable->m_sampleRate;
            }
            else if(myPlottable->m_deltaMode)
            {
                key = -(myPlottable->m_size * myPlottable->m_sampleRate);
            }

            if(myPlottable->m_coordinateSystem == CS_POLAR)
            {
                double temp = key;

                if(myPlottable->m_angleUnits == AU_DEGREES)
                {
                    temp = qDegreesToRadians(temp);
                }

                value = value * qSin(temp) * myPlottable->m_scaler;
            }
            else
            {
                value *= myPlottable->m_scaler;
            }

            if(myPlottable->m_deltaMode
            && (!myPlottable->data()->isEmpty())
            && (myPlottable->data()->size() == myPlottable->m_size))
            {
                QCPDataMap::iterator i =
                const_cast<QCPDataMap *>(myPlottable->data())->begin()+1;

                QCPDataMap::iterator j =
                const_cast<QCPDataMap *>(myPlottable->data())->end();

                for(QCPDataMap::iterator k = (j + 1); i != k; i++)
                {
                    QCPDataMap::iterator l = (i - 1);

                    l.value().value =
                    (i==j) ? value : i.value().value;

                    l.value().keyErrorPlus =
                    (i==j) ? 0.0 : i.value().keyErrorPlus;

                    l.value().keyErrorMinus =
                    (i==j) ? 0.0 : i.value().keyErrorMinus;

                    l.value().valueErrorPlus =
                    (i==j) ? 0.0 : i.value().valueErrorPlus;

                    l.value().valueErrorMinus =
                    (i==j) ? 0.0 : i.value().valueErrorMinus;
                }
            }
            else
            {
                const_cast<QCPDataMap *>(myPlottable->data())->insert(
                key, QCPData(key, value));

                while(myPlottable->data()->size() > myPlottable->m_size)
                {
                    myPlottable->removeData(myPlottable->data()->firstKey());
                }
            }

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::addGraphDataWithErrors(int plottable,
                                                double key,
                                                double value,
                                                double keyError,
                                                double valueError)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "key", key)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "value", value)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "keyError", keyError)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "valueError", valueError));

        if(ok)
        {
            double keyErrorPlus = keyError / 2.0;
            double keyErrorMinus = keyError / 2.0;
            double valueErrorPlus = valueError / 2.0;
            double valueErrorMinus = valueError / 2.0;

            if(myPlottable->m_coordinateSystem == CS_POLAR)
            {
                double temp = key;

                if(myPlottable->m_angleUnits == AU_DEGREES)
                {
                    temp = qDegreesToRadians(temp);
                }

                value = value * qSin(temp) * myPlottable->m_scaler;

                valueErrorMinus = valueErrorMinus * qSin(temp) *
                                  myPlottable->m_scaler;

                valueErrorPlus = valueErrorPlus * qSin(temp) *
                                 myPlottable->m_scaler;
            }
            else
            {
                value *= myPlottable->m_scaler;

                valueErrorMinus *= myPlottable->m_scaler;
                valueErrorPlus *= myPlottable->m_scaler;
            }

            QCPDataMap::iterator iterator =
            const_cast<QCPDataMap *>(myPlottable->data())->insert(
            key, QCPData(key, value));

            iterator.value().keyErrorPlus = qAbs(keyErrorPlus);
            iterator.value().keyErrorMinus = qAbs(keyErrorMinus);
            iterator.value().valueErrorPlus = qAbs(valueErrorPlus);
            iterator.value().valueErrorMinus = qAbs(valueErrorMinus);

            while(myPlottable->data()->size() > myPlottable->m_size)
            {
                myPlottable->removeData(myPlottable->data()->firstKey());
            }

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::addGraphDataWithErrors(int plottable,
                                                double value,
                                                double keyError,
                                                double valueError)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "value", value)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "keyError", keyError)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "valueError", valueError));

        if(ok)
        {
            double key = 0.0;

            double keyErrorPlus = keyError / 2.0;
            double keyErrorMinus = keyError / 2.0;
            double valueErrorPlus = valueError / 2.0;
            double valueErrorMinus = valueError / 2.0;

            if(!myPlottable->data()->isEmpty())
            {
                key = myPlottable->data()->lastKey() +
                      myPlottable->m_sampleRate;
            }
            else if(myPlottable->m_deltaMode)
            {
                key = -(myPlottable->m_size * myPlottable->m_sampleRate);
            }

            if(myPlottable->m_coordinateSystem == CS_POLAR)
            {
                double temp = key;

                if(myPlottable->m_angleUnits == AU_DEGREES)
                {
                    temp = qDegreesToRadians(temp);
                }

                value = value * qSin(temp) * myPlottable->m_scaler;

                valueErrorMinus = valueErrorMinus * qSin(temp) *
                                  myPlottable->m_scaler;

                valueErrorPlus = valueErrorPlus * qSin(temp) *
                                 myPlottable->m_scaler;
            }
            else
            {
                value *= myPlottable->m_scaler;

                valueErrorMinus *= myPlottable->m_scaler;
                valueErrorPlus *= myPlottable->m_scaler;
            }

            if(myPlottable->m_deltaMode
            && (!myPlottable->data()->isEmpty())
            && (myPlottable->data()->size() == myPlottable->m_size))
            {
                QCPDataMap::iterator i =
                const_cast<QCPDataMap *>(myPlottable->data())->begin()+1;

                QCPDataMap::iterator j =
                const_cast<QCPDataMap *>(myPlottable->data())->end();

                for(QCPDataMap::iterator k = (j + 1); i != k; i++)
                {
                    QCPDataMap::iterator l = (i - 1);

                    l.value().value =
                    (i==j) ? value : i.value().value;

                    l.value().keyErrorPlus =
                    (i==j) ? qAbs(keyErrorPlus) : i.value().keyErrorPlus;

                    l.value().keyErrorMinus =
                    (i==j) ? qAbs(keyErrorMinus): i.value().keyErrorMinus;

                    l.value().valueErrorPlus =
                    (i==j) ? qAbs(valueErrorPlus) : i.value().valueErrorPlus;

                    l.value().valueErrorMinus =
                    (i==j) ? qAbs(valueErrorMinus) : i.value().valueErrorMinus;
                }
            }
            else
            {
                QCPDataMap::iterator iterator =
                const_cast<QCPDataMap *>(myPlottable->data())->insert(
                key, QCPData(key, value));

                iterator.value().keyErrorPlus = qAbs(keyErrorPlus);
                iterator.value().keyErrorMinus = qAbs(keyErrorMinus);
                iterator.value().valueErrorPlus = qAbs(valueErrorPlus);
                iterator.value().valueErrorMinus = qAbs(valueErrorMinus);

                while(myPlottable->data()->size() > myPlottable->m_size)
                {
                    myPlottable->removeData(myPlottable->data()->firstKey());
                }
            }

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::addGraphDataWithErrors(int plottable,
                                                double key,
                                                double value,
                                                double keyErrorMinus,
                                                double keyErrorPlus,
                                                double valueErrorMinus,
                                                double valueErrorPlus)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "key", key)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "value", value)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "keyErrorMinus", keyErrorMinus)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "keyErrorPlus", keyErrorPlus)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "valueErrorMinus", valueErrorMinus)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "valueErrorPlus", valueErrorPlus));

        if(ok)
        {
            if(myPlottable->m_coordinateSystem == CS_POLAR)
            {
                double temp = key;

                if(myPlottable->m_angleUnits == AU_DEGREES)
                {
                    temp = qDegreesToRadians(temp);
                }

                value = value * qSin(temp) * myPlottable->m_scaler;

                valueErrorMinus = valueErrorMinus * qSin(temp) *
                                  myPlottable->m_scaler;

                valueErrorPlus = valueErrorPlus * qSin(temp) *
                                 myPlottable->m_scaler;
            }
            else
            {
                value *= myPlottable->m_scaler;

                valueErrorMinus *= myPlottable->m_scaler;
                valueErrorPlus *= myPlottable->m_scaler;
            }

            QCPDataMap::iterator iterator =
            const_cast<QCPDataMap *>(myPlottable->data())->insert(
            key, QCPData(key, value));

            iterator.value().keyErrorPlus = qAbs(keyErrorPlus);
            iterator.value().keyErrorMinus = qAbs(keyErrorMinus);
            iterator.value().valueErrorPlus = qAbs(valueErrorPlus);
            iterator.value().valueErrorMinus = qAbs(valueErrorMinus);

            while(myPlottable->data()->size() > myPlottable->m_size)
            {
                myPlottable->removeData(myPlottable->data()->firstKey());
            }

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::addGraphDataWithErrors(int plottable,
                                                double value,
                                                double keyErrorMinus,
                                                double keyErrorPlus,
                                                double valueErrorMinus,
                                                double valueErrorPlus)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "value", value)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "keyErrorMinus", keyErrorMinus)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "keyErrorPlus", keyErrorPlus)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "valueErrorMinus", valueErrorMinus)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "valueErrorPlus", valueErrorPlus));

        if(ok)
        {
            double key = 0.0;

            if(!myPlottable->data()->isEmpty())
            {
                key = myPlottable->data()->lastKey() +
                      myPlottable->m_sampleRate;
            }
            else if(myPlottable->m_deltaMode)
            {
                key = -(myPlottable->m_size * myPlottable->m_sampleRate);
            }

            if(myPlottable->m_coordinateSystem == CS_POLAR)
            {
                double temp = key;

                if(myPlottable->m_angleUnits == AU_DEGREES)
                {
                    temp = qDegreesToRadians(temp);
                }

                value = value * qSin(temp) * myPlottable->m_scaler;

                valueErrorMinus = valueErrorMinus * qSin(temp) *
                                  myPlottable->m_scaler;

                valueErrorPlus = valueErrorPlus * qSin(temp) *
                                 myPlottable->m_scaler;
            }
            else
            {
                value *= myPlottable->m_scaler;

                valueErrorMinus *= myPlottable->m_scaler;
                valueErrorPlus *= myPlottable->m_scaler;
            }

            if(myPlottable->m_deltaMode
            && (!myPlottable->data()->isEmpty())
            && (myPlottable->data()->size() == myPlottable->m_size))
            {
                QCPDataMap::iterator i =
                const_cast<QCPDataMap *>(myPlottable->data())->begin()+1;

                QCPDataMap::iterator j =
                const_cast<QCPDataMap *>(myPlottable->data())->end();

                for(QCPDataMap::iterator k = (j + 1); i != k; i++)
                {
                    QCPDataMap::iterator l = (i - 1);

                    l.value().value =
                    (i==j) ? value : i.value().value;

                    l.value().keyErrorPlus =
                    (i==j) ? qAbs(keyErrorPlus) : i.value().keyErrorPlus;

                    l.value().keyErrorMinus =
                    (i==j) ? qAbs(keyErrorMinus): i.value().keyErrorMinus;

                    l.value().valueErrorPlus =
                    (i==j) ? qAbs(valueErrorPlus) : i.value().valueErrorPlus;

                    l.value().valueErrorMinus =
                    (i==j) ? qAbs(valueErrorMinus) : i.value().valueErrorMinus;
                }
            }
            else
            {
                QCPDataMap::iterator iterator =
                const_cast<QCPDataMap *>(myPlottable->data())->insert(
                key, QCPData(key, value));

                iterator.value().keyErrorPlus = qAbs(keyErrorPlus);
                iterator.value().keyErrorMinus = qAbs(keyErrorMinus);
                iterator.value().valueErrorPlus = qAbs(valueErrorPlus);
                iterator.value().valueErrorMinus = qAbs(valueErrorMinus);

                while(myPlottable->data()->size() > myPlottable->m_size)
                {
                    myPlottable->removeData(myPlottable->data()->firstKey());
                }
            }

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::addBoxData(int plottable,
                                    double index,
                                    double value)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "index", index)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "value", value));

        if(ok)
        {
            value *= myPlottable->m_scaler;

            myPlottable->m_outs.insert(index, value);

            while(myPlottable->m_outs.size() > myPlottable->m_size)
            {
                myPlottable->m_outs.remove(myPlottable->m_outs.firstKey());
            }

            myPlottable->update(); updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::addBoxData(int plottable,
                                    double value)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInfinite(__FUNCTION__, myPlottable->name(),
                              "value", value);

        if(ok)
        {
            double index = 0.0;

            if(!myPlottable->m_outs.isEmpty())
            {
                index = myPlottable->m_outs.lastKey() +
                        myPlottable->m_sampleRate;
            }
            else if(myPlottable->m_deltaMode)
            {
                index = -(myPlottable->m_size * myPlottable->m_sampleRate);
            }

            value *= myPlottable->m_scaler;

            if(myPlottable->m_deltaMode
            && (!myPlottable->m_outs.isEmpty())
            && (myPlottable->m_outs.size() == myPlottable->m_size))
            {
                QMap<double, double>::iterator i =
                myPlottable->m_outs.begin()+1;

                QMap<double, double>::iterator j =
                myPlottable->m_outs.end();

                for(QMap<double, double>::iterator k = (j + 1); i != k; i++)
                {
                    QMap<double, double>::iterator l = (i - 1);

                    l.value() =
                    (i==j) ? value : i.value();
                }
            }
            else
            {
                myPlottable->m_outs.insert(index, value);

                while(myPlottable->m_outs.size() > myPlottable->m_size)
                {
                    myPlottable->m_outs.remove(myPlottable->m_outs.firstKey());
                }
            }

            myPlottable->update(); updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::removeBarDataBefore(int plottable,
                                             double key)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInfinite(__FUNCTION__, myPlottable->name(),
                              "key", key);

        if(ok)
        {
            myPlottable->removeDataBefore(key);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::removeBarDateAfter(int plottable,
                                            double key)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInfinite(__FUNCTION__, myPlottable->name(),
                              "key", key);

        if(ok)
        {
            myPlottable->removeDataAfter(key);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::removeBarDataBetween(int plottable,
                                              double fromKey,
                                              double toKey)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "fromKey", fromKey)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "toKey", toKey));

        if(ok)
        {
            if(fromKey == toKey)
            {
                myPlottable->removeData(fromKey);
            }
            else
            {
                myPlottable->removeData(qMin(fromKey, toKey),
                                        qMax(fromKey, toKey));
            }

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::removeBarData(int plottable)
{
    MyBars *myPlottable = plottableCast
    <MyBars>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        myPlottable->clearData();

        updatePlots();
    }

    return myPlottable;
}

bool SerialOscilloscope::removeCurveDataBefore(int plottable,
                                               double index)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInfinite(__FUNCTION__, myPlottable->name(),
                              "index", index);

        if(ok)
        {
            myPlottable->removeDataBefore(index);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::removeCurveDateAfter(int plottable,
                                              double index)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInfinite(__FUNCTION__, myPlottable->name(),
                              "index", index);

        if(ok)
        {
            myPlottable->removeDataAfter(index);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::removeCurveDataBetween(int plottable,
                                                double fromIndex,
                                                double toIndex)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "fromIndex", fromIndex)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "toIndex", toIndex));

        if(ok)
        {
            if(fromIndex == toIndex)
            {
                myPlottable->removeData(fromIndex);
            }
            else
            {
                myPlottable->removeData(qMin(fromIndex, toIndex),
                                        qMax(fromIndex, toIndex));
            }

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::removeCurveData(int plottable)
{
    MyCurve *myPlottable = plottableCast
    <MyCurve>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        myPlottable->clearData();

        updatePlots();
    }

    return myPlottable;
}

bool SerialOscilloscope::removeGraphDataBefore(int plottable,
                                               double key)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInfinite(__FUNCTION__, myPlottable->name(),
                              "key", key);

        if(ok)
        {
            myPlottable->removeDataBefore(key);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::removeGraphDateAfter(int plottable,
                                              double key)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInfinite(__FUNCTION__, myPlottable->name(),
                              "key", key);

        if(ok)
        {
            myPlottable->removeDataAfter(key);

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::removeGraphDataBetween(int plottable,
                                                double fromKey,
                                                double toKey)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "fromKey", fromKey)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "toKey", toKey));

        if(ok)
        {
            if(fromKey == toKey)
            {
                myPlottable->removeData(fromKey);
            }
            else
            {
                myPlottable->removeData(qMin(fromKey, toKey),
                                        qMax(fromKey, toKey));
            }

            updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::removeGraphData(int plottable)
{
    MyGraph *myPlottable = plottableCast
    <MyGraph>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        myPlottable->clearData();

        updatePlots();
    }

    return myPlottable;
}

bool SerialOscilloscope::removeBoxDataBefore(int plottable,
                                             double index)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInfinite(__FUNCTION__, myPlottable->name(),
                              "index", index);

        if(ok)
        {
            // See Also
            //
            // void QCPBars::removeDataBefore(double key)

            QMap<double, double>::iterator i =
            myPlottable->m_outs.begin();

            QMap<double, double>::iterator j =
            myPlottable->m_outs.end();

            for(; (i != j) && (i.key() < index);
                i = myPlottable->m_outs.erase(i));

            myPlottable->update(); updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::removeBoxDateAfter(int plottable,
                                            double index)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = notInfinite(__FUNCTION__, myPlottable->name(),
                              "index", index);

        if(ok)
        {
            // See Also
            //
            // void QCPBars::removeDataAfter(double key)

            QMap<double, double>::iterator i =
            myPlottable->m_outs.upperBound(index);

            QMap<double, double>::iterator j =
            myPlottable->m_outs.end();

            for(; i != j; i = myPlottable->m_outs.erase(i));

            myPlottable->update(); updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::removeBoxDataBetween(int plottable,
                                              double fromIndex,
                                              double toIndex)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        bool ok = (notInfinite(__FUNCTION__, myPlottable->name(),
                               "fromIndex", fromIndex)
                && notInfinite(__FUNCTION__, myPlottable->name(),
                               "toIndex", toIndex));

        if(ok)
        {
            // See Also
            //
            // QCPBars::removeData(double fromKey, double toKey)

            if(fromIndex == toIndex)
            {
                myPlottable->m_outs.remove(fromIndex);
            }
            else
            {
                QMap<double, double>::iterator i =
                myPlottable->m_outs.upperBound(qMin(fromIndex, toIndex));

                QMap<double, double>::iterator j =
                myPlottable->m_outs.upperBound(qMax(fromIndex, toIndex));

                for(; i != j; i = myPlottable->m_outs.erase(i));
            }

            myPlottable->update(); updatePlots();
        }

        return ok;
    }

    return myPlottable;
}

bool SerialOscilloscope::removeBoxData(int plottable)
{
    MyStatisticalBox *myPlottable = plottableCast
    <MyStatisticalBox>(__FUNCTION__, plottable);

    if(myPlottable)
    {
        // See Also
        //
        // void QCPBars::clearData()

        myPlottable->m_outs.clear();

        myPlottable->update(); updatePlots();
    }

    return myPlottable;
}

bool SerialOscilloscope::removeData(int plottable)
{
    QCPAbstractPlottable *aPlottable =
    plottableExists(__FUNCTION__, plottable);

    if(aPlottable)
    {
        if(qobject_cast<MyBars *>(aPlottable))
        {
            removeBarData(plottable);
        }
        else if(qobject_cast<MyCurve *>(aPlottable))
        {
            removeCurveData(plottable);
        }
        else if(qobject_cast<MyGraph *>(aPlottable))
        {
            removeGraphData(plottable);
        }
        else if(qobject_cast<MyStatisticalBox *>(aPlottable))
        {
            removeBoxData(plottable);
        }

        updatePlots();
    }

    return aPlottable;
}

void SerialOscilloscope::removeDatas()
{
    foreach(int plottable, m_plottables.keys())
    {
        removeData(plottable);
    }

    updatePlots();
}

void SerialOscilloscope::zoomIn()
{
    if(!m_math->underMouse())
    {
        QRect geometry = m_plot->geometry();

        QPointF localPos(geometry.width()/2.0, geometry.height()/2.0);
        QPointF globalPos(geometry.x()+localPos.x(),geometry.y()+localPos.y());

        QWheelEvent wheelEvent(localPos, globalPos, QPoint(0, +1),
        QPoint(0, +120), +120, Qt::Vertical, Qt::NoButton, Qt::NoModifier);

        QApplication::sendEvent(m_plot, &wheelEvent);
    }

    if(!m_plot->underMouse())
    {
        QRect geometry = m_math->geometry();

        QPointF localPos(geometry.width()/2.0, geometry.height()/2.0);
        QPointF globalPos(geometry.x()+localPos.x(),geometry.y()+localPos.y());

        QWheelEvent wheelEvent(localPos, globalPos, QPoint(0, +1),
        QPoint(0, +120), +120, Qt::Vertical, Qt::NoButton, Qt::NoModifier);

        QApplication::sendEvent(m_math, &wheelEvent);
    }
}

void SerialOscilloscope::zoomOut()
{
    if(!m_math->underMouse())
    {
        QRect geometry = m_plot->geometry();

        QPointF localPos(geometry.width()/2.0, geometry.height()/2.0);
        QPointF globalPos(geometry.x()+localPos.x(),geometry.y()+localPos.y());

        QWheelEvent wheelEvent(localPos, globalPos, QPoint(0, -1),
        QPoint(0, -120), -120, Qt::Vertical, Qt::NoButton, Qt::NoModifier);

        QApplication::sendEvent(m_plot, &wheelEvent);
    }

    if(!m_plot->underMouse())
    {
        QRect geometry = m_math->geometry();

        QPointF localPos(geometry.width()/2.0, geometry.height()/2.0);
        QPointF globalPos(geometry.x()+localPos.x(),geometry.y()+localPos.y());

        QWheelEvent wheelEvent(localPos, globalPos, QPoint(0, -1),
        QPoint(0, -120), -120, Qt::Vertical, Qt::NoButton, Qt::NoModifier);

        QApplication::sendEvent(m_math, &wheelEvent);
    }
}

void SerialOscilloscope::zoomFit()
{
    if(!m_math->underMouse())
    {
        m_plot->deselectAll();
        m_plot->rescaleAxes(true);

        if(!m_plotXAutoScale)
        {
            m_plot->xAxis->setRange(m_plotXAxisRangeBackup);
            m_plot->xAxis2->setRange(m_plotXAxisRangeBackup);
        }

        if(!m_plotYAutoScale)
        {
            m_plot->yAxis->setRange(m_plotYAxisRangeBackup);
            m_plot->yAxis2->setRange(m_plotYAxisRangeBackup);
        }

        m_plotXAxisRangeBackup = m_plot->xAxis->range();
        m_plotYAxisRangeBackup = m_plot->yAxis->range();

        m_plot->replot();
        m_plotResize = true;
    }

    if(!m_plot->underMouse())
    {
        m_math->deselectAll();
        m_math->rescaleAxes(true);

        if(!m_mathXAutoScale)
        {
            m_math->xAxis->setRange(m_mathXAxisRangeBackup);
            m_math->xAxis2->setRange(m_mathXAxisRangeBackup);
        }

        if(!m_mathYAutoScale)
        {
            m_math->yAxis->setRange(m_mathYAxisRangeBackup);
            m_math->yAxis2->setRange(m_mathYAxisRangeBackup);
        }

        m_mathXAxisRangeBackup = m_math->xAxis->range();
        m_mathYAxisRangeBackup = m_math->yAxis->range();

        m_math->replot();
        m_mathResize = true;
    }
}

void SerialOscilloscope::saveRasterImage()
{
    QSettings settings(settingsFileName(), settingsFormat());

    settings.beginGroup(keyGroup());
    settings.beginGroup(windowTitle());

    QString saveFile = settings.value(SERIAL_OSCILLOSCOPE_KEY_SAVE_R_FILE,
                                      QDir::homePath()).toString();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"),
    saveFile, tr("Image Files (*.bmp *.jpg *.jpeg *.png)"));

    if(!fileName.isEmpty())
    {
        if(fileName.endsWith(".bmp", Qt::CaseInsensitive)
        || fileName.endsWith(".jpg", Qt::CaseInsensitive)
        || fileName.endsWith(".jpeg", Qt::CaseInsensitive)
        || fileName.endsWith(".png", Qt::CaseInsensitive))
        {
            UtilRasterDialog dialog(tr("Save Options"), this);

            dialog.setWidth(m_rasterWidth);
            dialog.setHeight(m_rasterHeight);
            dialog.setSaveCurrentView(m_rasterSaveViewport);

            if(dialog.exec() == QDialog::Accepted)
            {
                QPixmap pixmap(dialog.getWidth(), dialog.getHeight());

                QCPPainter painter;

                if(!painter.begin(&pixmap))
                {
                    QMessageBox::critical(this, tr("Save Image Error"),
                    tr("Painting initialization failed")); return;
                }

                QCPRange xRp = m_plot->xAxis->range();
                QCPRange yRp = m_plot->yAxis->range();
                QCPRange x2Rp = m_plot->xAxis2->range();
                QCPRange y2Rp = m_plot->yAxis2->range();

                QCPRange xRm = m_math->xAxis->range();
                QCPRange yRm = m_math->yAxis->range();
                QCPRange x2Rm = m_math->xAxis2->range();
                QCPRange y2Rm = m_math->yAxis2->range();

                if(!dialog.getSaveCurrentView())
                {
                    m_plot->rescaleAxes(true); m_math->rescaleAxes(true);

                    if(!m_plotXAutoScale)
                    {
                        m_plot->xAxis->setRange(m_plotXAxisRangeBackup);
                        m_plot->xAxis2->setRange(m_plotXAxisRangeBackup);
                    }

                    if(!m_plotYAutoScale)
                    {
                        m_plot->yAxis->setRange(m_plotYAxisRangeBackup);
                        m_plot->yAxis2->setRange(m_plotYAxisRangeBackup);
                    }

                    if(!m_mathXAutoScale)
                    {
                        m_math->xAxis->setRange(m_mathXAxisRangeBackup);
                        m_math->xAxis2->setRange(m_mathXAxisRangeBackup);
                    }

                    if(!m_mathYAutoScale)
                    {
                        m_math->yAxis->setRange(m_mathYAxisRangeBackup);
                        m_math->yAxis2->setRange(m_mathYAxisRangeBackup);
                    }
                }

                if(m_math->parentWidget()->isHidden())
                {
                    m_plot->toPainter(&painter,
                                      painter.device()->width(),
                                      painter.device()->height());
                }
                else
                {
                    m_plot->toPainter(&painter,
                                      painter.device()->width() / 2,
                                      painter.device()->height());

                    painter.translate(painter.device()->width() / 2, 0);

                    m_math->toPainter(&painter,
                                      painter.device()->width() / 2,
                                      painter.device()->height());
                }

                m_plot->xAxis->setRange(xRp);
                m_plot->yAxis->setRange(yRp);
                m_plot->xAxis2->setRange(x2Rp);
                m_plot->yAxis2->setRange(y2Rp);

                m_math->xAxis->setRange(xRm);
                m_math->yAxis->setRange(yRm);
                m_math->xAxis2->setRange(x2Rm);
                m_math->yAxis2->setRange(y2Rm);

                if((!painter.end()) || (!pixmap.save(fileName)))
                {
                    QMessageBox::critical(this, tr("Save Image Error"),
                    tr("Painting finalization failed")); return;
                }

                QFileInfo fileInfo(fileName);

                settings.setValue(SERIAL_OSCILLOSCOPE_KEY_SAVE_R_FILE,
                fileInfo.canonicalFilePath());

                m_rasterWidth = dialog.getWidth();
                m_rasterHeight = dialog.getHeight();
                m_rasterSaveViewport = dialog.getSaveCurrentView();
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Save Image Error"),
            tr("Unsupported file type"));
        }
    }
}

void SerialOscilloscope::saveVectorImage()
{
    QSettings settings(settingsFileName(), settingsFormat());

    settings.beginGroup(keyGroup());
    settings.beginGroup(windowTitle());

    QString saveFile = settings.value(SERIAL_OSCILLOSCOPE_KEY_SAVE_V_FILE,
                                      QDir::homePath()).toString();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"),
    saveFile, tr("Image Files (*.svg *.pdf)"));

    if(!fileName.isEmpty())
    {
        if(fileName.endsWith(".svg", Qt::CaseInsensitive))
        {
            UtilVectorDialog dialog(tr("Save Options"), this);

            dialog.setWidth(m_vectorWidth);
            dialog.setHeight(m_vectorHeight);
            dialog.setSaveCurrentView(m_vectorSaveViewport);

            dialog.setFileTitle(m_vectorTitle);
            dialog.setFileDescription(m_vectorDescription);

            if(dialog.exec() == QDialog::Accepted)
            {
                QSvgGenerator generator; generator.setFileName(fileName);

                generator.setSize(QSize(dialog.getWidth(),
                                        dialog.getHeight()));

                generator.setViewBox(QRect(0, 0, dialog.getWidth() - 1,
                                                 dialog.getHeight() - 1));

                // QT-BUG: QSvgGenerator should escape strings for me...

                generator.setTitle(QString(dialog.getFileTitle()).
                replace("&", "&amp;").replace("\"", "&apos;").
                replace("'", "&quot;").replace("<", "&lt;").
                replace(">", "&gt;"));

                generator.setDescription(QString(dialog.getFileDescription()).
                replace("&", "&amp;").replace("\"", "&apos;").
                replace("'", "&quot;").replace("<", "&lt;").
                replace(">", "&gt;"));

                QCPPainter painter;

                if(!painter.begin(&generator))
                {
                    QMessageBox::critical(this, tr("Save Image Error"),
                    tr("Painting initialization failed")); return;
                }

                QCPRange xRp = m_plot->xAxis->range();
                QCPRange yRp = m_plot->yAxis->range();
                QCPRange x2Rp = m_plot->xAxis2->range();
                QCPRange y2Rp = m_plot->yAxis2->range();

                QCPRange xRm = m_math->xAxis->range();
                QCPRange yRm = m_math->yAxis->range();
                QCPRange x2Rm = m_math->xAxis2->range();
                QCPRange y2Rm = m_math->yAxis2->range();

                if(!dialog.getSaveCurrentView())
                {
                    m_plot->rescaleAxes(true); m_math->rescaleAxes(true);

                    if(!m_plotXAutoScale)
                    {
                        m_plot->xAxis->setRange(m_plotXAxisRangeBackup);
                        m_plot->xAxis2->setRange(m_plotXAxisRangeBackup);
                    }

                    if(!m_plotYAutoScale)
                    {
                        m_plot->yAxis->setRange(m_plotYAxisRangeBackup);
                        m_plot->yAxis2->setRange(m_plotYAxisRangeBackup);
                    }

                    if(!m_mathXAutoScale)
                    {
                        m_math->xAxis->setRange(m_mathXAxisRangeBackup);
                        m_math->xAxis2->setRange(m_mathXAxisRangeBackup);
                    }

                    if(!m_mathYAutoScale)
                    {
                        m_math->yAxis->setRange(m_mathYAxisRangeBackup);
                        m_math->yAxis2->setRange(m_mathYAxisRangeBackup);
                    }
                }

                if(m_math->parentWidget()->isHidden())
                {
                    m_plot->toPainter(&painter,
                                      painter.device()->width(),
                                      painter.device()->height());
                }
                else
                {
                    m_plot->toPainter(&painter,
                                      painter.device()->width() / 2,
                                      painter.device()->height());

                    painter.translate(painter.device()->width() / 2, 0);

                    m_math->toPainter(&painter,
                                      painter.device()->width() / 2,
                                      painter.device()->height());
                }

                m_plot->xAxis->setRange(xRp);
                m_plot->yAxis->setRange(yRp);
                m_plot->xAxis2->setRange(x2Rp);
                m_plot->yAxis2->setRange(y2Rp);

                m_math->xAxis->setRange(xRm);
                m_math->yAxis->setRange(yRm);
                m_math->xAxis2->setRange(x2Rm);
                m_math->yAxis2->setRange(y2Rm);

                if(!painter.end())
                {
                    QMessageBox::critical(this, tr("Save Image Error"),
                    tr("Painting finalization failed")); return;
                }

                QFileInfo fileInfo(fileName);

                settings.setValue(SERIAL_OSCILLOSCOPE_KEY_SAVE_V_FILE,
                fileInfo.canonicalFilePath());

                m_vectorWidth = dialog.getWidth();
                m_vectorHeight = dialog.getHeight();
                m_vectorSaveViewport = dialog.getSaveCurrentView();

                m_vectorTitle = dialog.getFileTitle();
                m_vectorDescription = dialog.getFileDescription();
            }
        }
        else if(fileName.endsWith(".pdf", Qt::CaseInsensitive))
        {
            QPrinter printer;

            printer.setOutputFormat(QPrinter::NativeFormat);

            QPageSetupDialog dialog(&printer, this);

            if(dialog.exec() == QDialog::Accepted)
            {
                QPageLayout layout = printer.pageLayout();
                printer.setOutputFormat(QPrinter::PdfFormat);
                printer.setOutputFileName(fileName);
                printer.setPageLayout(layout);

                UtilVectorDialogWOWH dialog(tr("Save Options"), this);

                dialog.setSaveCurrentView(m_vectorSaveViewport);

                dialog.setFileTitle(m_vectorTitle);
                dialog.setFileDescription(m_vectorDescription);

                if(dialog.exec() == QDialog::Accepted)
                {
                    printer.setCreator(dialog.getFileDescription());
                    printer.setDocName(dialog.getFileTitle());

                    QCPPainter painter;

                    if(!painter.begin(&printer))
                    {
                        QMessageBox::critical(this, tr("Save Image Error"),
                        tr("Painting initialization failed")); return;
                    }

                    QCPRange xRp = m_plot->xAxis->range();
                    QCPRange yRp = m_plot->yAxis->range();
                    QCPRange x2Rp = m_plot->xAxis2->range();
                    QCPRange y2Rp = m_plot->yAxis2->range();

                    QCPRange xRm = m_math->xAxis->range();
                    QCPRange yRm = m_math->yAxis->range();
                    QCPRange x2Rm = m_math->xAxis2->range();
                    QCPRange y2Rm = m_math->yAxis2->range();

                    if(!dialog.getSaveCurrentView())
                    {
                        m_plot->rescaleAxes(true); m_math->rescaleAxes(true);

                        if(!m_plotXAutoScale)
                        {
                            m_plot->xAxis->setRange(m_plotXAxisRangeBackup);
                            m_plot->xAxis2->setRange(m_plotXAxisRangeBackup);
                        }

                        if(!m_plotYAutoScale)
                        {
                            m_plot->yAxis->setRange(m_plotYAxisRangeBackup);
                            m_plot->yAxis2->setRange(m_plotYAxisRangeBackup);
                        }

                        if(!m_mathXAutoScale)
                        {
                            m_math->xAxis->setRange(m_mathXAxisRangeBackup);
                            m_math->xAxis2->setRange(m_mathXAxisRangeBackup);
                        }

                        if(!m_mathYAutoScale)
                        {
                            m_math->yAxis->setRange(m_mathYAxisRangeBackup);
                            m_math->yAxis2->setRange(m_mathYAxisRangeBackup);
                        }
                    }

                    if(m_math->parentWidget()->isHidden())
                    {
                        m_plot->toPainter(&painter,
                                          painter.device()->width(),
                                          painter.device()->height());
                    }
                    else
                    {
                        m_plot->toPainter(&painter,
                                          painter.device()->width() / 2,
                                          painter.device()->height());

                        painter.translate(painter.device()->width() / 2, 0);

                        m_math->toPainter(&painter,
                                          painter.device()->width() / 2,
                                          painter.device()->height());
                    }

                    m_plot->xAxis->setRange(xRp);
                    m_plot->yAxis->setRange(yRp);
                    m_plot->xAxis2->setRange(x2Rp);
                    m_plot->yAxis2->setRange(y2Rp);

                    m_math->xAxis->setRange(xRm);
                    m_math->yAxis->setRange(yRm);
                    m_math->xAxis2->setRange(x2Rm);
                    m_math->yAxis2->setRange(y2Rm);

                    if(!painter.end())
                    {
                        QMessageBox::critical(this, tr("Save Image Error"),
                        tr("Painting finalization failed")); return;
                    }

                    QFileInfo fileInfo(fileName);

                    settings.setValue(SERIAL_OSCILLOSCOPE_KEY_SAVE_V_FILE,
                    fileInfo.canonicalFilePath());

                    m_vectorSaveViewport = dialog.getSaveCurrentView();

                    m_vectorTitle = dialog.getFileTitle();
                    m_vectorDescription = dialog.getFileDescription();
                }
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Save Image Error"),
            tr("Unsupported file type"));
        }
    }
}

void SerialOscilloscope::importState(const QString &fileName)
{
    QSettings settings(settingsFileName(), settingsFormat());

    settings.beginGroup(keyGroup());
    settings.beginGroup(windowTitle());

    QString openFile = settings.value(SERIAL_OSCILLOSCOPE_KEY_IMPORT_FILE,
                                      QDir::homePath()).toString();

    QString temp = fileName.isEmpty() ? QFileDialog::getOpenFileName(this,
    tr("Import State"), openFile, tr("JSON Files (*.json)")) : fileName;

    if(!temp.isEmpty())
    {
        QFile file(temp);

        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QJsonParseError parseError; QJsonDocument json =
            QJsonDocument::fromJson(file.readAll(), &parseError);

            if(parseError.error == QJsonParseError::NoError)
            {
                QJsonObject object = json.object();

                if(object.value("type").toString() == "oscilloscope")
                {
                    deletePlots();

                    setWindowTitle(object.value("title").toString());

                    QString background = object.value("background").toString();

                    if(QColor::isValidColor(background))
                    {
                        setBackgroundColor(QColor(background).rgba());
                    }

                    setXAxisScaleType(static_cast<QCPAxis::ScaleType>(
                    object.value("XAxisScaleType").
                    toInt(getXAxisScaleType())));

                    setXAxisScaleLogBase(
                    object.value("XAxisScaleLogBase").
                    toDouble(getXAxisScaleLogBase()));

                    setXAxisRangeAutoScale(
                    object.value("XAxisRangeAutoScale").
                    toBool(getXAxisRangeAutoScale()));

                    setXAxisRangeLower(
                    object.value("XAxisRangeLower").
                    toDouble(getXAxisRangeLower()));

                    setXAxisRangeUpper(
                    object.value("XAxisRangeUpper").
                    toDouble(getXAxisRangeUpper()));

                    setXAxisRangeReversed(
                    object.value("XAxisRangeReversed").
                    toBool(getXAxisRangeReversed()));

                    setXAxisLabel(
                    object.value("XAxisLabel").
                    toString(getXAxisLabel()));

                    setYAxisScaleType(static_cast<QCPAxis::ScaleType>(
                    object.value("YAxisScaleType").
                    toInt(getYAxisScaleType())));

                    setYAxisScaleLogBase(
                    object.value("YAxisScaleLogBase").
                    toDouble(getYAxisScaleLogBase()));

                    setYAxisRangeAutoScale(
                    object.value("YAxisRangeAutoScale").
                    toBool(getYAxisRangeAutoScale()));

                    setYAxisRangeLower(
                    object.value("YAxisRangeLower").
                    toDouble(getYAxisRangeLower()));

                    setYAxisRangeUpper(
                    object.value("YAxisRangeUpper").
                    toDouble(getYAxisRangeUpper()));

                    setYAxisRangeReversed(
                    object.value("YAxisRangeReversed").
                    toBool(getYAxisRangeReversed()));

                    setYAxisLabel(
                    object.value("YAxisLabel").
                    toString(getYAxisLabel()));

                    setFFTXAxisScaleType(static_cast<QCPAxis::ScaleType>(
                    object.value("FFTXAxisScaleType").
                    toInt(getFFTXAxisScaleType())));

                    setFFTXAxisScaleLogBase(
                    object.value("FFTXAxisScaleLogBase").
                    toDouble(getFFTXAxisScaleLogBase()));

                    setFFTXAxisRangeAutoScale(
                    object.value("FFTXAxisRangeAutoScale").
                    toBool(getFFTXAxisRangeAutoScale()));

                    setFFTXAxisRangeLower(
                    object.value("FFTXAxisRangeLower").
                    toDouble(getFFTXAxisRangeLower()));

                    setFFTXAxisRangeUpper(
                    object.value("FFTXAxisRangeUpper").
                    toDouble(getFFTXAxisRangeUpper()));

                    setFFTXAxisRangeReversed(
                    object.value("FFTXAxisRangeReversed").
                    toBool(getFFTXAxisRangeReversed()));

                    setFFTXAxisLabel(
                    object.value("FFTXAxisLabel").
                    toString(getFFTXAxisLabel()));

                    setFFTYAxisScaleType(static_cast<QCPAxis::ScaleType>(
                    object.value("FFTYAxisScaleType").
                    toInt(getFFTYAxisScaleType())));

                    setFFTYAxisScaleLogBase(
                    object.value("FFTYAxisScaleLogBase").
                    toDouble(getFFTYAxisScaleLogBase()));

                    setFFTYAxisRangeAutoScale(
                    object.value("FFTYAxisRangeAutoScale").
                    toBool(getFFTYAxisRangeAutoScale()));

                    setFFTYAxisRangeLower(
                    object.value("FFTYAxisRangeLower").
                    toDouble(getFFTYAxisRangeLower()));

                    setFFTYAxisRangeUpper(
                    object.value("FFTYAxisRangeUpper").
                    toDouble(getFFTYAxisRangeUpper()));

                    setFFTYAxisRangeReversed(
                    object.value("FFTYAxisRangeReversed").
                    toBool(getFFTYAxisRangeReversed()));

                    setFFTYAxisLabel(
                    object.value("FFTYAxisLabel").
                    toString(getFFTYAxisLabel()));

                    QJsonArray plottables=object.value("plottables").toArray();

                    for(int i = 0, j = plottables.size(); i < j; i++)
                    {
                        QJsonObject plottable = plottables.at(i).toObject();

                        if(plottable.value("type").toString() == "bar")
                        {
                            int handle = plottable.value("plottable").toInt();

                            newBar(handle,
                            plottable.value("name").toString());

                            QString lineColor =
                            plottable.value("lineColor").toString();

                            if(QColor::isValidColor(lineColor))
                            {
                                setBarLineColor(handle,
                                QColor(lineColor).rgba());
                            }

                            setBarLineStyle(handle,
                            static_cast<Qt::PenStyle>(
                            plottable.value("lineStyle").toInt()));

                            QString fillColor =
                            plottable.value("fillColor").toString();

                            if(QColor::isValidColor(fillColor))
                            {
                                setBarFillColor(handle,
                                QColor(fillColor).rgba());
                            }

                            setBarFillStyle(handle,
                            static_cast<Qt::BrushStyle>(
                            plottable.value("fillStyle").toInt()));

                            setBarDeltaMode(handle,
                            plottable.value("deltaMode").toBool());

                            setBarSize(handle,
                            plottable.value("size").toInt());

                            setBarSampleRate(handle,
                            plottable.value("sampleRate").toDouble());

                            setBarWidth(handle,
                            plottable.value("width").toDouble());

                            setBarScatterStyle(handle,
                            static_cast<QCPScatterStyle::ScatterShape>(
                            plottable.value("scatterStyle").toInt()));

                            QJsonArray data=plottable.value("data").toArray();

                            for(int k = 0, l = data.size(); k < l; k++)
                            {
                                QJsonObject keyValue = data.at(k).toObject();

                                addBarData(handle,
                                keyValue.value("key").toDouble(),
                                keyValue.value("value").toDouble());
                            }

                            setBarCoordinateSystem(handle,
                            static_cast<CoordinateSystem>(
                            plottable.value("coordinateSystem").toInt()));

                            setBarAngleUnits(handle,
                            static_cast<AngleUnits>(
                            plottable.value("angleUnits").toInt()));

                            setBarScaler(handle,
                            plottable.value("scaler").toDouble());
                        }
                        else if(plottable.value("type").toString() == "curve")
                        {
                            int handle = plottable.value("plottable").toInt();

                            newCurve(handle,
                            plottable.value("name").toString());

                            QString lineColor =
                            plottable.value("lineColor").toString();

                            if(QColor::isValidColor(lineColor))
                            {
                                setCurveLineColor(handle,
                                QColor(lineColor).rgba());
                            }

                            setCurveLineStyle(handle,
                            static_cast<Qt::PenStyle>(
                            plottable.value("lineStyle").toInt()));

                            QString fillColor =
                            plottable.value("fillColor").toString();

                            if(QColor::isValidColor(fillColor))
                            {
                                setCurveFillColor(handle,
                                QColor(fillColor).rgba());
                            }

                            setCurveFillStyle(handle,
                            static_cast<Qt::BrushStyle>(
                            plottable.value("fillStyle").toInt()));

                            setCurveDeltaMode(handle,
                            plottable.value("deltaMode").toBool());

                            setCurveSize(handle,
                            plottable.value("size").toInt());

                            setCurveSampleRate(handle,
                            plottable.value("sampleRate").toDouble());

                            setCurveFFT(handle,
                            static_cast<CurveFFT>(
                            plottable.value("FFT").toInt()));

                            setCurveFFTWindowFunction(handle,
                            static_cast<CurveFFTWF>(
                            plottable.value("FFTWF").toInt()));

                            setCurveScatterStyle(handle,
                            static_cast<QCPScatterStyle::ScatterShape>(
                            plottable.value("scatterStyle").toInt()));

                            setCurveStyle(handle,
                            static_cast<QCPCurve::LineStyle>(
                            plottable.value("style").toInt()));

                            QJsonArray data=plottable.value("data").toArray();

                            for(int k = 0, l = data.size(); k < l; k++)
                            {
                                QJsonObject indexValue = data.at(k).toObject();

                                addCurveData(handle,
                                indexValue.value("index").toDouble(),
                                indexValue.value("key").toDouble(),
                                indexValue.value("value").toDouble());
                            }

                            setCurveCoordinateSystem(handle,
                            static_cast<CoordinateSystem>(
                            plottable.value("coordinateSystem").toInt()));

                            setCurveAngleUnits(handle,
                            static_cast<AngleUnits>(
                            plottable.value("angleUnits").toInt()));

                            setCurveScaler(handle,
                            plottable.value("scaler").toDouble());
                        }
                        else if(plottable.value("type").toString() == "graph")
                        {
                            int handle = plottable.value("plottable").toInt();

                            newGraph(handle,
                            plottable.value("name").toString());

                            QString lineColor =
                            plottable.value("lineColor").toString();

                            if(QColor::isValidColor(lineColor))
                            {
                                setGraphLineColor(handle,
                                QColor(lineColor).rgba());
                            }

                            setGraphLineStyle(handle,
                            static_cast<Qt::PenStyle>(
                            plottable.value("lineStyle").toInt()));

                            QString fillColor =
                            plottable.value("fillColor").toString();

                            if(QColor::isValidColor(fillColor))
                            {
                                setGraphFillColor(handle,
                                QColor(fillColor).rgba());
                            }

                            setGraphFillStyle(handle,
                            static_cast<Qt::BrushStyle>(
                            plottable.value("fillStyle").toInt()));

                            setGraphDeltaMode(handle,
                            plottable.value("deltaMode").toBool());

                            setGraphSize(handle,
                            plottable.value("size").toInt());

                            setGraphSampleRate(handle,
                            plottable.value("sampleRate").toDouble());

                            setGraphFFT(handle,
                            static_cast<GraphFFT>(
                            plottable.value("FFT").toInt()));

                            setGraphFFTWindowFunction(handle,
                            static_cast<GraphFFTWF>(
                            plottable.value("FFTWF").toInt()));

                            setGraphScatterStyle(handle,
                            static_cast<QCPScatterStyle::ScatterShape>(
                            plottable.value("scatterStyle").toInt()));

                            setGraphStyle(handle,
                            static_cast<QCPGraph::LineStyle>(
                            plottable.value("style").toInt()));

                            setGraphKeyErrors(handle,
                            plottable.value("keyErrors").toBool());

                            setGraphValueErrors(handle,
                            plottable.value("valueErrors").toBool());

                            QJsonArray data=plottable.value("data").toArray();

                            for(int k = 0, l = data.size(); k < l; k++)
                            {
                                QJsonObject keyValue = data.at(k).toObject();

                                addGraphDataWithErrors(handle,
                                keyValue.value("key").toDouble(),
                                keyValue.value("value").toDouble(),
                                keyValue.value("keyErrorMinus").toDouble(),
                                keyValue.value("keyErrorPlus").toDouble(),
                                keyValue.value("valueErrorMinus").toDouble(),
                                keyValue.value("valueErrorPlus").toDouble());
                            }

                            setGraphCoordinateSystem(handle,
                            static_cast<CoordinateSystem>(
                            plottable.value("coordinateSystem").toInt()));

                            setGraphAngleUnits(handle,
                            static_cast<AngleUnits>(
                            plottable.value("angleUnits").toInt()));

                            setGraphScaler(handle,
                            plottable.value("scaler").toDouble());
                        }
                        else if(plottable.value("type").toString() == "box")
                        {
                            int handle = plottable.value("plottable").toInt();

                            newBox(handle,
                            plottable.value("name").toString());

                            QString lineColor =
                            plottable.value("lineColor").toString();

                            if(QColor::isValidColor(lineColor))
                            {
                                setBoxLineColor(handle,
                                QColor(lineColor).rgba());
                            }

                            setBoxLineStyle(handle,
                            static_cast<Qt::PenStyle>(
                            plottable.value("lineStyle").toInt()));

                            QString fillColor =
                            plottable.value("fillColor").toString();

                            if(QColor::isValidColor(fillColor))
                            {
                                setBoxFillColor(handle,
                                QColor(fillColor).rgba());
                            }

                            setBoxFillStyle(handle,
                            static_cast<Qt::BrushStyle>(
                            plottable.value("fillStyle").toInt()));

                            setBoxDeltaMode(handle,
                            plottable.value("deltaMode").toBool());

                            setBoxSize(handle,
                            plottable.value("size").toInt());

                            setBoxSampleRate(handle,
                            plottable.value("sampleRate").toDouble());

                            setBoxWidth(handle,
                            plottable.value("width").toDouble());

                            setBoxScatterStyle(handle,
                            static_cast<QCPScatterStyle::ScatterShape>(
                            plottable.value("scatterStyle").toInt()));

                            QJsonArray data=plottable.value("data").toArray();

                            for(int k = 0, l = data.size(); k < l; k++)
                            {
                                QJsonObject indexValue = data.at(k).toObject();

                                addBoxData(handle,
                                indexValue.value("index").toDouble(),
                                indexValue.value("value").toDouble());
                            }

                            setBoxCoordinateSystem(handle,
                            static_cast<CoordinateSystem>(
                            plottable.value("coordinateSystem").toInt()));

                            setBoxAngleUnits(handle,
                            static_cast<AngleUnits>(
                            plottable.value("angleUnits").toInt()));

                            setBoxScaler(handle,
                            plottable.value("scaler").toDouble());

                            setBoxKey(handle,
                            plottable.value("key").toDouble());

                            setBoxMinimum(handle,
                            plottable.value("minimum").toDouble());

                            setBoxLowerQuartile(handle,
                            plottable.value("lowerQuartile").toDouble());

                            setBoxMedian(handle,
                            plottable.value("median").toDouble());

                            setBoxUpperQuartile(handle,
                            plottable.value("upperQuartile").toDouble());

                            setBoxMaximum(handle,
                            plottable.value("maximum").toDouble());
                        }
                    }

                    for(int i = 0, j = plottables.size(); i < j; i++)
                    {
                        QJsonObject plottable = plottables.at(i).toObject();

                        if(plottable.value("type").toString() == "bar")
                        {
                            int handle = plottable.value("plottable").toInt();

                            setBarBelow(handle,
                            plottable.value("below").toInt());

                            setBarAbove(handle,
                            plottable.value("above").toInt());
                        }
                        else if(plottable.value("type").toString() == "curve")
                        {
                            int handle = plottable.value("plottable").toInt();

                            setCurveChannelFillCurve(handle,
                            plottable.value("channelFillCurve").toInt());
                        }
                        else if(plottable.value("type").toString() == "graph")
                        {
                            int handle = plottable.value("plottable").toInt();

                            setGraphChannelFillGraph(handle,
                            plottable.value("channelFillGraph").toInt());
                        }
                        else if(plottable.value("type").toString() == "box")
                        {
                            int handle = plottable.value("plottable").toInt();

                            Q_UNUSED(handle);
                        }
                    }

                    QFileInfo fileInfo(temp);

                    settings.setValue(SERIAL_OSCILLOSCOPE_KEY_IMPORT_FILE,
                    fileInfo.canonicalFilePath());
                }
                else
                {
                    QMessageBox::critical(this, tr("Import State Error"),
                    tr("Incompatible JSON file"));
                }
            }
            else
            {
                QMessageBox::critical(this, tr("Import State Error"),
                parseError.errorString());
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Import State Error"),
            file.errorString());
        }
    }
}

void SerialOscilloscope::exportState(const QString &fileName)
{
    QSettings settings(settingsFileName(), settingsFormat());

    settings.beginGroup(keyGroup());
    settings.beginGroup(windowTitle());

    QString saveFile = settings.value(SERIAL_OSCILLOSCOPE_KEY_EXPORT_FILE,
                                      QDir::homePath()).toString();

    QString temp = fileName.isEmpty() ? QFileDialog::getSaveFileName(this,
    tr("Export State"), saveFile, tr("JSON Files (*.json)")) : fileName;

    if(!temp.isEmpty())
    {
        QFile file(temp);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QJsonObject object;

            object.insert("type", QString("oscilloscope"));
            object.insert("title", windowTitle());

            object.insert("background",
            QColor::fromRgba(getBackgroundColor()).name(QColor::HexArgb));

            object.insert("XAxisScaleType",
                          getXAxisScaleType());
            object.insert("XAxisScaleLogBase",
                          getXAxisScaleLogBase());
            object.insert("XAxisRangeAutoScale",
                          getXAxisRangeAutoScale());
            object.insert("XAxisRangeLower",
                          getXAxisRangeLower());
            object.insert("XAxisRangeUpper",
                          getXAxisRangeUpper());
            object.insert("XAxisRangeReversed",
                          getXAxisRangeReversed());
            object.insert("XAxisLabel",
                          getXAxisLabel());

            object.insert("YAxisScaleType",
                          getYAxisScaleType());
            object.insert("YAxisScaleLogBase",
                          getYAxisScaleLogBase());
            object.insert("YAxisRangeAutoScale",
                          getYAxisRangeAutoScale());
            object.insert("YAxisRangeLower",
                          getYAxisRangeLower());
            object.insert("YAxisRangeUpper",
                          getYAxisRangeUpper());
            object.insert("YAxisRangeReversed",
                          getYAxisRangeReversed());
            object.insert("YAxisLabel",
                          getYAxisLabel());

            object.insert("FFTXAxisScaleType",
                          getFFTXAxisScaleType());
            object.insert("FFTXAxisScaleLogBase",
                          getFFTXAxisScaleLogBase());
            object.insert("FFTXAxisRangeAutoScale",
                          getFFTXAxisRangeAutoScale());
            object.insert("FFTXAxisRangeLower",
                          getFFTXAxisRangeLower());
            object.insert("FFTXAxisRangeUpper",
                          getFFTXAxisRangeUpper());
            object.insert("FFTXAxisRangeReversed",
                          getFFTXAxisRangeReversed());
            object.insert("FFTXAxisLabel",
                          getFFTXAxisLabel());

            object.insert("FFTYAxisScaleType",
                          getFFTYAxisScaleType());
            object.insert("FFTYAxisScaleLogBase",
                          getFFTYAxisScaleLogBase());
            object.insert("FFTYAxisRangeAutoScale",
                          getFFTYAxisRangeAutoScale());
            object.insert("FFTYAxisRangeLower",
                          getFFTYAxisRangeLower());
            object.insert("FFTYAxisRangeUpper",
                          getFFTYAxisRangeUpper());
            object.insert("FFTYAxisRangeReversed",
                          getFFTYAxisRangeReversed());
            object.insert("FFTYAxisLabel",
                          getFFTYAxisLabel());

            QJsonArray plottables;

            QMap<int, QCPAbstractPlottable *>::const_iterator i =
            m_plottables.constBegin();

            QMap<int, QCPAbstractPlottable *>::const_iterator j =
            m_plottables.constEnd();

            for(; i != j; i++)
            {
                if(qobject_cast<MyBars *>(i.value()))
                {
                    QJsonObject plottable; bool ok;

                    plottable.insert("type", QString("bar"));
                    plottable.insert("plottable", i.key());

                    plottable.insert("name",
                    getBarName(i.key(), &ok));

                    plottable.insert("coordinateSystem",
                    getBarCoordinateSystem(i.key(), &ok));

                    plottable.insert("angleUnits",
                    getBarAngleUnits(i.key(), &ok));

                    plottable.insert("lineColor", QColor::fromRgba(
                    getBarLineColor(i.key(), &ok)).name(QColor::HexArgb));

                    plottable.insert("lineStyle",
                    getBarLineStyle(i.key(), &ok));

                    plottable.insert("fillColor", QColor::fromRgba(
                    getBarFillColor(i.key(), &ok)).name(QColor::HexArgb));

                    plottable.insert("fillStyle",
                    getBarFillStyle(i.key(), &ok));

                    plottable.insert("deltaMode",
                    getBarDeltaMode(i.key(), &ok));

                    plottable.insert("size",
                    getBarSize(i.key(), &ok));

                    plottable.insert("sampleRate",
                    getBarSampleRate(i.key(), &ok));

                    plottable.insert("scaler",
                    getBarScaler(i.key(), &ok));

                    plottable.insert("width",
                    getBarWidth(i.key(), &ok));

                    plottable.insert("scatterStyle",
                    getBarScatterStyle(i.key(), &ok));

                    plottable.insert("below",
                    getBarBelow(i.key(), &ok));

                    plottable.insert("above",
                    getBarAbove(i.key(), &ok));

                    QJsonArray data;

                    QCPBarDataMap::const_iterator k =
                    static_cast<MyBars *>
                    (i.value())->data()->constBegin();

                    QCPBarDataMap::const_iterator l =
                    static_cast<MyBars *>
                    (i.value())->data()->constEnd();

                    for(; k != l; k++)
                    {
                        QJsonObject keyValue;

                        keyValue.insert("key",
                        k.value().key);

                        keyValue.insert("value",
                        k.value().value);

                        data.append(keyValue);
                    }

                    plottable.insert("data", data);
                    plottables.append(plottable);
                }
                else if(qobject_cast<MyCurve *>(i.value()))
                {
                    QJsonObject plottable; bool ok;

                    plottable.insert("type", QString("curve"));
                    plottable.insert("plottable", i.key());

                    plottable.insert("name",
                    getCurveName(i.key(), &ok));

                    plottable.insert("coordinateSystem",
                    getCurveCoordinateSystem(i.key(), &ok));

                    plottable.insert("angleUnits",
                    getCurveAngleUnits(i.key(), &ok));

                    plottable.insert("lineColor", QColor::fromRgba(
                    getCurveLineColor(i.key(), &ok)).name(QColor::HexArgb));

                    plottable.insert("lineStyle",
                    getCurveLineStyle(i.key(), &ok));

                    plottable.insert("fillColor", QColor::fromRgba(
                    getCurveFillColor(i.key(), &ok)).name(QColor::HexArgb));

                    plottable.insert("fillStyle",
                    getCurveFillStyle(i.key(), &ok));

                    plottable.insert("deltaMode",
                    getCurveDeltaMode(i.key(), &ok));

                    plottable.insert("size",
                    getCurveSize(i.key(), &ok));

                    plottable.insert("sampleRate",
                    getCurveSampleRate(i.key(), &ok));

                    plottable.insert("scaler",
                    getCurveScaler(i.key(), &ok));

                    plottable.insert("FFT",
                    getCurveFFT(i.key(), &ok));

                    plottable.insert("FFTWF",
                    getCurveFFTWindowFunction(i.key(), &ok));

                    plottable.insert("scatterStyle",
                    getCurveScatterStyle(i.key(), &ok));

                    plottable.insert("style",
                    getCurveStyle(i.key(), &ok));

                    plottable.insert("channelFillCurve",
                    getCurveChannelFillCurve(i.key(), &ok));

                    QJsonArray data;

                    QCPCurveDataMap::const_iterator k =
                    static_cast<MyCurve *>
                    (i.value())->data()->constBegin();

                    QCPCurveDataMap::const_iterator l =
                    static_cast<MyCurve *>
                    (i.value())->data()->constEnd();

                    for(; k != l; k++)
                    {
                        QJsonObject indexValue;

                        indexValue.insert("index",
                        k.value().t);

                        indexValue.insert("key",
                        k.value().key);

                        indexValue.insert("value",
                        k.value().value);

                        data.append(indexValue);
                    }

                    plottable.insert("data", data);
                    plottables.append(plottable);
                }
                else if(qobject_cast<MyGraph *>(i.value()))
                {
                    QJsonObject plottable; bool ok;

                    plottable.insert("type", QString("graph"));
                    plottable.insert("plottable", i.key());

                    plottable.insert("name",
                    getGraphName(i.key(), &ok));

                    plottable.insert("coordinateSystem",
                    getGraphCoordinateSystem(i.key(), &ok));

                    plottable.insert("angleUnits",
                    getGraphAngleUnits(i.key(), &ok));

                    plottable.insert("lineColor", QColor::fromRgba(
                    getGraphLineColor(i.key(), &ok)).name(QColor::HexArgb));

                    plottable.insert("lineStyle",
                    getGraphLineStyle(i.key(), &ok));

                    plottable.insert("fillColor", QColor::fromRgba(
                    getGraphFillColor(i.key(), &ok)).name(QColor::HexArgb));

                    plottable.insert("fillStyle",
                    getGraphFillStyle(i.key(), &ok));

                    plottable.insert("deltaMode",
                    getGraphDeltaMode(i.key(), &ok));

                    plottable.insert("size",
                    getGraphSize(i.key(), &ok));

                    plottable.insert("sampleRate",
                    getGraphSampleRate(i.key(), &ok));

                    plottable.insert("scaler",
                    getGraphScaler(i.key(), &ok));

                    plottable.insert("FFT",
                    getGraphFFT(i.key(), &ok));

                    plottable.insert("FFTWF",
                    getGraphFFTWindowFunction(i.key(), &ok));

                    plottable.insert("scatterStyle",
                    getGraphScatterStyle(i.key(), &ok));

                    plottable.insert("style",
                    getGraphStyle(i.key(), &ok));

                    plottable.insert("channelFillGraph",
                    getGraphChannelFillGraph(i.key(), &ok));

                    plottable.insert("keyErrors",
                    getGraphKeyErrors(i.key(), &ok));

                    plottable.insert("valueErrors",
                    getGraphValueErrors(i.key(), &ok));

                    QJsonArray data;

                    QCPDataMap::const_iterator k =
                    static_cast<MyGraph *>
                    (i.value())->data()->constBegin();

                    QCPDataMap::const_iterator l =
                    static_cast<MyGraph *>
                    (i.value())->data()->constEnd();

                    for(; k != l; k++)
                    {
                        QJsonObject keyValue;

                        keyValue.insert("key",
                        k.value().key);

                        keyValue.insert("value",
                        k.value().value);

                        keyValue.insert("keyErrorMinus",
                        k.value().keyErrorMinus);

                        keyValue.insert("keyErrorPlus",
                        k.value().keyErrorPlus);

                        keyValue.insert("valueErrorMinus",
                        k.value().valueErrorMinus);

                        keyValue.insert("valueErrorPlus",
                        k.value().valueErrorPlus);

                        data.append(keyValue);
                    }

                    plottable.insert("data", data);
                    plottables.append(plottable);
                }
                else if(qobject_cast<MyStatisticalBox *>(i.value()))
                {
                    QJsonObject plottable; bool ok;

                    plottable.insert("type", QString("box"));
                    plottable.insert("plottable", i.key());

                    plottable.insert("name",
                    getBoxName(i.key(), &ok));

                    plottable.insert("coordinateSystem",
                    getBoxCoordinateSystem(i.key(), &ok));

                    plottable.insert("angleUnits",
                    getBoxAngleUnits(i.key(), &ok));

                    plottable.insert("lineColor", QColor::fromRgba(
                    getBoxLineColor(i.key(), &ok)).name(QColor::HexArgb));

                    plottable.insert("lineStyle",
                    getBoxLineStyle(i.key(), &ok));

                    plottable.insert("fillColor", QColor::fromRgba(
                    getBoxFillColor(i.key(), &ok)).name(QColor::HexArgb));

                    plottable.insert("fillStyle",
                    getBoxFillStyle(i.key(), &ok));

                    plottable.insert("deltaMode",
                    getBoxDeltaMode(i.key(), &ok));

                    plottable.insert("size",
                    getBoxSize(i.key(), &ok));

                    plottable.insert("sampleRate",
                    getBoxSampleRate(i.key(), &ok));

                    plottable.insert("scaler",
                    getBoxScaler(i.key(), &ok));

                    plottable.insert("width",
                    getBoxWidth(i.key(), &ok));

                    plottable.insert("scatterStyle",
                    getBoxScatterStyle(i.key(), &ok));

                    plottable.insert("key",
                    getBoxKey(i.key(), &ok));

                    plottable.insert("minimum",
                    getBoxMinimum(i.key(), &ok));

                    plottable.insert("lowerQuartile",
                    getBoxLowerQuartile(i.key(), &ok));

                    plottable.insert("median",
                    getBoxMedian(i.key(), &ok));

                    plottable.insert("upperQuartile",
                    getBoxUpperQuartile(i.key(), &ok));

                    plottable.insert("maximum",
                    getBoxMaximum(i.key(), &ok));

                    QJsonArray data;

                    QMap<double, double>::const_iterator k =
                    static_cast<MyStatisticalBox *>
                    (i.value())->m_outs.constBegin();

                    QMap<double, double>::const_iterator l =
                    static_cast<MyStatisticalBox *>
                    (i.value())->m_outs.constEnd();

                    for(; k != l; k++)
                    {
                        QJsonObject indexValue;

                        indexValue.insert("index",
                        k.key());

                        indexValue.insert("value",
                        k.value());

                        data.append(indexValue);
                    }

                    plottable.insert("data", data);
                    plottables.append(plottable);
                }
            }

            object.insert("plottables", plottables);

            QByteArray json = QJsonDocument(object).toJson();

            if(file.write(json) == json.size())
            {
                QFileInfo fileInfo(temp);

                settings.setValue(SERIAL_OSCILLOSCOPE_KEY_EXPORT_FILE,
                fileInfo.canonicalFilePath());
            }
            else
            {
                QMessageBox::critical(this, tr("Export State Error"),
                file.errorString());
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Export State Error"),
            file.errorString());
        }
    }
}

void SerialOscilloscope::generalHelp()
{
    if(!QDesktopServices::openUrl(QUrl("http://" PROJECT_DOMAIN_NAME_STR "/"
    "help/general/")))
    {
        QMessageBox::critical(this, tr("Open General Help Error"),
        tr("Unable to open the URL to the General Help page"));
    }
}

void SerialOscilloscope::oscilloscopeHelp()
{
    if(!QDesktopServices::openUrl(QUrl("http://" PROJECT_DOMAIN_NAME_STR "/"
    "help/widgets/oscilloscope/")))
    {
        QMessageBox::critical(this, tr("Open Oscilloscope Help Error"),
        tr("Unable to open the URL to the Oscilloscope Help page"));
    }
}

void SerialOscilloscope::updatePlots()
{
    if(m_replotLock.tryLock())
    {
        QTimer::singleShot(UPDATE_WAIT, this, SLOT(repaintPlots()));
    }
}

void SerialOscilloscope::repaintPlots()
{
    if(m_fftw_enabled)
    {
        QFutureSynchronizer<void> sync;

        foreach(QCPAbstractPlottable *aPlottable, m_plottables.values())
        {
            if(qobject_cast<MyCurve *>(aPlottable))
            {
                MyCurve *curve = static_cast<MyCurve *>(aPlottable);

                if(curve->m_FFTWOption != CFFT_NONE)
                {
                    sync.addFuture(QtConcurrent::run(
                    SerialOscilloscope::calculateCurveFFT, curve));
                }
            }
            else if(qobject_cast<MyGraph *>(aPlottable))
            {
                MyGraph *graph = static_cast<MyGraph *>(aPlottable);

                if(graph->m_FFTWOption != GFFT_NONE)
                {
                    sync.addFuture(QtConcurrent::run(
                    SerialOscilloscope::calculateGraphFFT, graph));
                }
            }
        }

        sync.waitForFinished();

        bool ok = !sync.futures().isEmpty();

        m_plotTitle->setVisible(ok);
        m_mathTitle->setVisible(ok);
        m_math->parentWidget()->setVisible(ok);
    }

    ///////////////////////////////////////////////////////////////////////////

    foreach(QCPAbstractPlottable *aPlottable, m_plottables.values())
    {
        if(qobject_cast<MyGraph *>(aPlottable))
        {
            MyGraph *graph =
            static_cast<MyGraph *>(aPlottable);

            QPen errorPen = graph->errorPen();
            QPen errorPenFFT = graph->m_FFTWPlot->errorPen();

            if(m_background.color().lightness() > 150)
            {
                errorPen.setColor(Qt::black);
                errorPenFFT.setColor(Qt::black);
            }
            else
            {
                errorPen.setColor(Qt::white);
                errorPenFFT.setColor(Qt::white);
            }

            graph->setErrorPen(errorPen);
            graph->m_FFTWPlot->setErrorPen(errorPenFFT);
        }
        else if(qobject_cast<MyStatisticalBox *>(aPlottable))
        {
            MyStatisticalBox *box =
            static_cast<MyStatisticalBox *>(aPlottable);

            QPen whiskerPen = box->whiskerPen();
            QPen whiskerBarPen = box->whiskerBarPen();
            // QPen medianPen = box->medianPen();

            if(m_background.color().lightness() > 150)
            {
                whiskerPen.setColor(Qt::black);
                whiskerBarPen.setColor(Qt::black);
                // medianPen.setColor(Qt::black);
            }
            else
            {
                whiskerPen.setColor(Qt::white);
                whiskerBarPen.setColor(Qt::white);
                // medianPen.setColor(Qt::white);
            }

            box->setWhiskerPen(whiskerPen);
            box->setWhiskerBarPen(whiskerBarPen);
            // box->setMedianPen(medianPen);
        }
    }

    ///////////////////////////////////////////////////////////////////////////

    // Plot Replotting
    {
        bool visible = false;

        for(int i = 0, j = m_plot->legend->itemCount(); i < j; i++)
        {
            visible = visible || m_plot->legend->item(i)->visible();
        }

        m_plot->legend->setVisible(visible);

        ///////////////////////////////////////////////////////////////////////

        if(m_plotResize)
        {
            m_plot->rescaleAxes(true);

            if(!m_plotXAutoScale)
            {
                m_plot->xAxis->setRange(m_plotXAxisRangeBackup);
                m_plot->xAxis2->setRange(m_plotXAxisRangeBackup);
            }

            if(!m_plotYAutoScale)
            {
                m_plot->yAxis->setRange(m_plotYAxisRangeBackup);
                m_plot->yAxis2->setRange(m_plotYAxisRangeBackup);
            }

            m_plotXAxisRangeBackup = m_plot->xAxis->range();
            m_plotYAxisRangeBackup = m_plot->yAxis->range();
        }

        // QCP-BUG: Spacing is not removed for hidden items...

        m_plot->replot();
    }

    // Math Replotting
    {
        bool visible = false;

        for(int i = 0, j = m_math->legend->itemCount(); i < j; i++)
        {
            visible = visible || m_math->legend->item(i)->visible();
        }

        m_math->legend->setVisible(visible);

        ///////////////////////////////////////////////////////////////////////

        if(m_mathResize)
        {
            m_math->rescaleAxes(true);

            if(!m_mathXAutoScale)
            {
                m_math->xAxis->setRange(m_mathXAxisRangeBackup);
                m_math->xAxis2->setRange(m_mathXAxisRangeBackup);
            }

            if(!m_mathYAutoScale)
            {
                m_math->yAxis->setRange(m_mathYAxisRangeBackup);
                m_math->yAxis2->setRange(m_mathYAxisRangeBackup);
            }

            m_mathXAxisRangeBackup = m_math->xAxis->range();
            m_mathYAxisRangeBackup = m_math->yAxis->range();
        }

        // QCP-BUG: Spacing is not removed for hidden items...

        m_math->replot();
    }

    m_replotLock.unlock();
}

bool SerialOscilloscope::eventFilter(QObject *object, QEvent *event)
{
    if((object == m_plot) || (object == m_math))
    {
        if(event->type() == QEvent::MouseButtonDblClick)
        {
            if(object == m_plot)
            {
                m_plot->deselectAll();
                m_plot->rescaleAxes(true);

                if(!m_plotXAutoScale)
                {
                    m_plot->xAxis->setRange(m_plotXAxisRangeBackup);
                    m_plot->xAxis2->setRange(m_plotXAxisRangeBackup);
                }

                if(!m_plotYAutoScale)
                {
                    m_plot->yAxis->setRange(m_plotYAxisRangeBackup);
                    m_plot->yAxis2->setRange(m_plotYAxisRangeBackup);
                }

                m_plotXAxisRangeBackup = m_plot->xAxis->range();
                m_plotYAxisRangeBackup = m_plot->yAxis->range();

                m_plot->replot();
                m_plotResize = true;
            }
            else if(object == m_math)
            {
                m_math->deselectAll();
                m_math->rescaleAxes(true);

                if(!m_mathXAutoScale)
                {
                    m_math->xAxis->setRange(m_mathXAxisRangeBackup);
                    m_math->xAxis2->setRange(m_mathXAxisRangeBackup);
                }

                if(!m_mathYAutoScale)
                {
                    m_math->yAxis->setRange(m_mathYAxisRangeBackup);
                    m_math->yAxis2->setRange(m_mathYAxisRangeBackup);
                }

                m_mathXAxisRangeBackup = m_math->xAxis->range();
                m_mathYAxisRangeBackup = m_math->yAxis->range();

                m_math->replot();
                m_mathResize = true;
            }
        }

        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

            if(mouseEvent->buttons() & Qt::LeftButton)
            {
                if(object == m_plot)
                {
                    m_plotResize = false;
                }
                else if(object == m_math)
                {
                    m_mathResize = false;
                }
            }
        }

        if(event->type() == QEvent::Wheel)
        {
            if(object == m_plot)
            {
                m_plotResize = false;
            }
            else if(object == m_math)
            {
                m_mathResize = false;
            }
        }

        if(event->type() == QEvent::ToolTip)
        {
            QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);

            QCPAbstractPlottable *aPlottable=NULL; double x=0.0; double y=0.0;

            if(object == m_plot)
            {
                aPlottable = m_plot->plottableAt(helpEvent->pos());

                x = m_plot->xAxis->pixelToCoord(helpEvent->pos().x());
                y = m_plot->yAxis->pixelToCoord(helpEvent->pos().y());
            }
            else if(object == m_math)
            {
                aPlottable = m_math->plottableAt(helpEvent->pos());

                x = m_math->xAxis->pixelToCoord(helpEvent->pos().x());
                y = m_math->yAxis->pixelToCoord(helpEvent->pos().y());
            }

            if(aPlottable)
            {
                QToolTip::showText(helpEvent->globalPos(),
                                   tr("<table>"
                                        "<tr>"
                                          "<th colspan=\"2\">Point</th>"
                                        "</tr>"
                                        "<tr>"
                                          "<td>X:</td>" "<td>%L1</td>"
                                        "</tr>"
                                        "<tr>"
                                          "<td>Y:</td>" "<td>%L2</td>"
                                        "</tr>"
                                      "</table>").
                                      arg(x).
                                      arg(y));
            }
            else
            {
                QToolTip::hideText(); event->ignore();
            }

            return true;
        }

        if(event->type() == QEvent::WhatsThis)
        {
            QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);

            QCPAbstractPlottable *aPlottable=NULL; double x=0.0; double y=0.0;

            if(object == m_plot)
            {
                aPlottable = m_plot->plottableAt(helpEvent->pos());

                x = m_plot->xAxis->pixelToCoord(helpEvent->pos().x());
                y = m_plot->yAxis->pixelToCoord(helpEvent->pos().y());
            }
            else if(object == m_math)
            {
                aPlottable = m_math->plottableAt(helpEvent->pos());

                x = m_math->xAxis->pixelToCoord(helpEvent->pos().x());
                y = m_math->yAxis->pixelToCoord(helpEvent->pos().y());
            }

            if(aPlottable)
            {
                QWhatsThis::showText(helpEvent->globalPos(),
                                     tr("<table>"
                                          "<tr>"
                                            "<th colspan=\"2\">Point</th>"
                                          "</tr>"
                                          "<tr>"
                                            "<td>X:</td>" "<td>%L1</td>"
                                          "</tr>"
                                          "<tr>"
                                            "<td>Y:</td>" "<td>%L2</td>"
                                          "</tr>"
                                        "</table>").
                                        arg(x).
                                        arg(y));
            }
            else
            {
                QWhatsThis::hideText(); event->ignore();
            }

            return true;
        }
    }

    return SerialWindow::eventFilter(object, event);
}

void SerialOscilloscope::resizeEvent(QResizeEvent *event)
{
    SerialWindow::resizeEvent(event);
}

template <class T>
T *SerialOscilloscope::plottableCast(const QString &FName,
                                     int plottable)
{
    QCPAbstractPlottable *aPlottable = plottableExists(FName, plottable);

    if(aPlottable)
    {
        T *myPlottable = qobject_cast<T *>(aPlottable);

        if(!myPlottable)
        {
            QString instanceClassName = aPlottable->metaObject()->className();
            QString templateClassName = T::staticMetaObject.className();

            emit errorMessage(QString(metaObject()->className()) +
            "::" + FName + "[" + windowTitle() + "] -> " +
            tr("Plottable %L1 type cast error (%L2 != %L3)").
            arg(plottable).
            arg(instanceClassName.mid(2)).
            arg(templateClassName.mid(2)));
        }

        return myPlottable;
    }

    return NULL;
}

bool SerialOscilloscope::notInfinite(const QString &FName,
                                     const QString &PName,
                                     const QString &VName,
                                     double value)
{
    bool ok = qIsFinite(value);

    if(!ok)
    {
        emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "][" + PName + "] -> " +
        tr("Argument (%L1 == %L2) is an infinite value").
        arg(VName).arg(value));
    }

    return ok;
}

bool SerialOscilloscope::notInfinite(const QString &FName,
                                     const QString &VName,
                                     double value)
{
    bool ok = qIsFinite(value);

    if(!ok)
    {
        emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "] -> " +
        tr("Argument (%L1 == %L2) is an infinite value").
        arg(VName).arg(value));
    }

    return ok;
}

bool SerialOscilloscope::notZero(const QString &FName,
                                 const QString &PName,
                                 const QString &VName,
                                 double value)
{
    bool ok = !qFuzzyCompare(1.0 + value, 1.0);

    if(!ok)
    {
        emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "][" + PName + "] -> " +
        tr("Argument (%L1 == %L2) is a zero value").
        arg(VName).arg(value));
    }

    return ok;
}

bool SerialOscilloscope::notZero(const QString &FName,
                                 const QString &VName,
                                 double value)
{
    bool ok = !qFuzzyCompare(1.0 + value, 1.0);

    if(!ok)
    {
        emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "] -> " +
        tr("Argument (%L1 == %L2) is a zero value").
        arg(VName).arg(value));
    }

    return ok;
}

bool SerialOscilloscope::notInvalid(const QString &FName,
                                    const QString &PName,
                                    const QString &VName,
                                    double value,
                                    double minValue0,
                                    double maxValue0,
                                    double minValue1,
                                    double maxValue1)
{
    bool ok = (((minValue0 <= value) && (value <= maxValue0))
            || ((minValue1 <= value) && (value <= maxValue1)));

    if(!ok)
    {
        emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "][" + PName + "] -> " +
        tr("Argument (%L1 == %L2) is out of the valid range between "
        "[%L3 : %L4] + [%L5 : %L6]").arg(VName).arg(value).
        arg(minValue0).arg(maxValue0).
        arg(minValue1).arg(maxValue1));
    }

    return ok;
}

bool SerialOscilloscope::notInvalid(const QString &FName,
                                    const QString &PName,
                                    const QString &VName,
                                    double value,
                                    double minValue,
                                    double maxValue)
{
    bool ok = ((minValue <= value) && (value <= maxValue));

    if(!ok)
    {
        emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "][" + PName + "] -> " +
        tr("Argument (%L1 == %L2) is out of the valid range between "
        "[%L3 : %L4]").arg(VName).arg(value).arg(minValue).arg(maxValue));
    }

    return ok;
}

bool SerialOscilloscope::notInvalid(const QString &FName,
                                    const QString &VName,
                                    double value,
                                    double minValue,
                                    double maxValue)
{
    bool ok = ((minValue <= value) && (value <= maxValue));

    if(!ok)
    {
        emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "] -> " +
        tr("Argument (%L1 == %L2) is out of the valid range between "
        "[%L3 : %L4]").arg(VName).arg(value).arg(minValue).arg(maxValue));
    }

    return ok;
}

QCPAbstractPlottable *SerialOscilloscope::plottableExists(const QString &FName,
                                                          int plottable)
{
    QCPAbstractPlottable *aPlottable = m_plottables.value(plottable);

    if(!aPlottable)
    {
        emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "] -> " +
        tr("Plottable %L1 does not exist").
        arg(plottable));
    }

    return aPlottable;
}

bool SerialOscilloscope::plottableDoesNotExist(const QString &FName,
                                               int plottable)
{
    Q_UNUSED(FName); // Silently handle error...

    bool ok = !m_plottables.contains(plottable);

    if(!ok)
    {
        /* emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "] -> " +
        tr("Plottable %L1 already exists").
        arg(plottable)); */
    }

    return ok;
}

bool SerialOscilloscope::FFTWEnabled(const QString &FName,
                                     const QString &PName)
{
    if(!m_fftw_enabled)
    {
        emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "][" + PName + "] -> " +
        tr("FFT support is disabled"));
    }

    return m_fftw_enabled;
}

bool SerialOscilloscope::FFTWEnabled(const QString &FName)
{
    if(!m_fftw_enabled)
    {
        emit errorMessage(QString(metaObject()->className()) +
        "::" + FName + "[" + windowTitle() + "] -> " +
        tr("FFT support is disabled"));
    }

    return m_fftw_enabled;
}

void SerialOscilloscope::calculateCurveFFT(MyCurve *curve)
{
    if(!curve)
    {
        return;
    }

    curve->m_FFTWPlot->clearData();

    QMap<int, fftw_plan>::const_iterator iterator;

    switch(curve->m_FFTWOption)
    {
        case CFFT_NONE: return;

        case CFFT_FORWARD:
        iterator = m_fftw_f_plans.upperBound(curve->data()->size()); break;

        case CFFT_BACKWARD:
        iterator = m_fftw_b_plans.upperBound(curve->data()->size()); break;

        default: return;
    }

    int inSize = iterator.key();
    int outSize = iterator.key();

    fftw_complex *in = q_check_ptr(fftw_alloc_complex(inSize));
    fftw_complex *out = q_check_ptr(fftw_alloc_complex(outSize));

    qMemSet(in, 0, sizeof(fftw_complex) * inSize);
    qMemSet(out, 0, sizeof(fftw_complex) * outSize);

    QCPCurveDataMap::const_iterator i = curve->data()->begin();
    QCPCurveDataMap::const_iterator j = curve->data()->end();

    for(int k = 0, l = curve->data()->size() - 1; i != j; i++, k++)
    {
        double w = 1.0;

        if(l > 0)
        {
            switch(curve->m_FFTWFunction)
            {
                case CFFTWF_RECTANGLE: break;

                case CFFTWF_TRIANGLE:
                {
                    w = 1.0 - qAbs((k - (l / 2.0)) / (l / 2.0));

                    break;
                }

                case CFFTWF_WELCH:
                {
                    w = 1.0 - qPow((k - (l / 2.0)) / (l / 2.0), 2.0);

                    break;
                }

                case CFFTWF_HANNING:
                {
                    double a = (1.0 / 2.0), b = (1.0 / 2.0);

                    w = a - (b * qCos((2.0 * M_PI * k) / l));

                    break;
                }

                case CFFTWF_HAMMING:
                {
                    double a = (25.0 / 46.0), b = (21.0 / 46.0);

                    w = a - (b * qCos((2.0 * M_PI * k) / l));

                    break;
                }

                case CFFTWF_BLACKMAN:
                {
                    double a0 = (7938.0 / 18608.0),
                           a1 = (9240.0 / 18608.0),
                           a2 = (1430.0 / 18608.0);

                    w = a0 - (a1 * qCos((2.0 * M_PI * k) / l)) +
                             (a2 * qCos((4.0 * M_PI * k) / l));

                    break;
                }

                case CFFTWF_BLACKMAN_HARRIS:
                {
                    double a0 = 0.35875,
                           a1 = 0.48829,
                           a2 = 0.14128,
                           a3 = 0.01168;

                    w = a0 - (a1 * qCos((2.0 * M_PI * k) / l))
                           + (a2 * qCos((4.0 * M_PI * k) / l))
                           - (a3 * qCos((6.0 * M_PI * k) / l));

                    break;
                }

                case CFFTWF_BLACKMAN_NUTTAL:
                {
                    double a0 = 0.3635819,
                           a1 = 0.4891775,
                           a2 = 0.1365995,
                           a3 = 0.0106411;

                    w = a0 - (a1 * qCos((2.0 * M_PI * k) / l))
                           + (a2 * qCos((4.0 * M_PI * k) / l))
                           - (a3 * qCos((6.0 * M_PI * k) / l));

                    break;
                }

                default: break;
            }
        }

        in[k][0] = i.value().key * w;
        in[k][1] = i.value().value * w;
    }

    fftw_execute_dft(iterator.value(), in, out);

    double frequency = 0.0;
    double deltaFrequency = ((1.0 / curve->m_sampleRate) / inSize);

    for(int k = 0; k < outSize; k++, frequency += deltaFrequency)
    {
        const_cast<QCPCurveDataMap *>(curve->data())->insert(
        frequency, QCPCurveData(frequency, out[k][0], out[k][1]));
    }

    fftw_free(in);
    fftw_free(out);
}

void SerialOscilloscope::calculateGraphFFT(MyGraph *graph)
{
    if(!graph)
    {
        return;
    }

    graph->m_FFTWPlot->clearData();

    QMap<int, fftw_plan>::const_iterator iterator =
    m_fftw_plans.upperBound(graph->data()->size());

    int inSize = iterator.key();
    int outSize = ((iterator.key() / 2) + 1);

    double *in = q_check_ptr(fftw_alloc_real(inSize));
    fftw_complex *out = q_check_ptr(fftw_alloc_complex(outSize));

    double *inM = q_check_ptr(fftw_alloc_real(inSize));
    fftw_complex *outM = q_check_ptr(fftw_alloc_complex(outSize));

    double *inP = q_check_ptr(fftw_alloc_real(inSize));
    fftw_complex *outP = q_check_ptr(fftw_alloc_complex(outSize));

    qMemSet(in, 0, sizeof(double) * inSize);
    qMemSet(out, 0, sizeof(fftw_complex) * outSize);

    qMemSet(inM, 0, sizeof(double) * inSize);
    qMemSet(outM, 0, sizeof(fftw_complex) * outSize);

    qMemSet(inP, 0, sizeof(double) * inSize);
    qMemSet(outP, 0, sizeof(fftw_complex) * outSize);

    QCPDataMap::const_iterator i = graph->data()->begin();
    QCPDataMap::const_iterator j = graph->data()->end();

    for(int k = 0, l = graph->data()->size() - 1; i != j; i++, k++)
    {
        double w = 1.0;

        if(l > 0)
        {
            switch(graph->m_FFTWFunction)
            {
                case GFFTWF_RECTANGLE: break;

                case GFFTWF_TRIANGLE:
                {
                    w = 1.0 - qAbs((k - (l / 2.0)) / (l / 2.0));

                    break;
                }

                case GFFTWF_WELCH:
                {
                    w = 1.0 - qPow((k - (l / 2.0)) / (l / 2.0), 2.0);

                    break;
                }

                case GFFTWF_HANNING:
                {
                    double a = (1.0 / 2.0), b = (1.0 / 2.0);

                    w = a - (b * qCos((2.0 * M_PI * k) / l));

                    break;
                }

                case GFFTWF_HAMMING:
                {
                    double a = (25.0 / 46.0), b = (21.0 / 46.0);

                    w = a - (b * qCos((2.0 * M_PI * k) / l));

                    break;
                }

                case GFFTWF_BLACKMAN:
                {
                    double a0 = (7938.0 / 18608.0),
                           a1 = (9240.0 / 18608.0),
                           a2 = (1430.0 / 18608.0);

                    w = a0 - (a1 * qCos((2.0 * M_PI * k) / l)) +
                             (a2 * qCos((4.0 * M_PI * k) / l));

                    break;
                }

                case GFFTWF_BLACKMAN_HARRIS:
                {
                    double a0 = 0.35875,
                           a1 = 0.48829,
                           a2 = 0.14128,
                           a3 = 0.01168;

                    w = a0 - (a1 * qCos((2.0 * M_PI * k) / l))
                           + (a2 * qCos((4.0 * M_PI * k) / l))
                           - (a3 * qCos((6.0 * M_PI * k) / l));

                    break;
                }

                case GFFTWF_BLACKMAN_NUTTAL:
                {
                    double a0 = 0.3635819,
                           a1 = 0.4891775,
                           a2 = 0.1365995,
                           a3 = 0.0106411;

                    w = a0 - (a1 * qCos((2.0 * M_PI * k) / l))
                           + (a2 * qCos((4.0 * M_PI * k) / l))
                           - (a3 * qCos((6.0 * M_PI * k) / l));

                    break;
                }

                default: break;
            }
        }

        in[k] = i.value().value * w;
        inM[k] = i.value().valueErrorMinus * w;
        inP[k] = i.value().valueErrorPlus * w;
    }

    fftw_execute_dft_r2c(iterator.value(), in, out);
    fftw_execute_dft_r2c(iterator.value(), inM, outM);
    fftw_execute_dft_r2c(iterator.value(), inP, outP);

    double pOld = 0.0, pOldM = 0.0, pOldP = 0.0;
    double pUnwrapper = 0.0, pUnwrapperM = 0.0, pUnwrapperP = 0.0;

    double frequency = 0.0;
    double deltaFrequency = ((1.0 / graph->m_sampleRate) / inSize);

    for(int k = 0; k < outSize; k++, frequency += deltaFrequency)
    {
        switch(graph->m_FFTWOption)
        {
            case GFFT_NONE: break;

            case GFFT_MAGNITUDE:
            {
                double m = qSqrt(qPow(out[k][0], 2.0) + qPow(out[k][1], 2.0));
                m = m * ((k != 0) && (k != (outSize-1)) ? 2.0 : 1.0) / inSize;

                QCPDataMap::iterator l =
                const_cast<QCPDataMap *>(graph->m_FFTWPlot->data())->insert(
                frequency, QCPData(frequency, m));

                l.value().keyErrorPlus = 0.0;
                l.value().keyErrorMinus = 0.0;

                ///////////////////////////////////////////////////////////////

                m = qSqrt(qPow(outP[k][0], 2.0) + qPow(outP[k][1], 2.0));
                m = m * ((k != 0) && (k != (outSize-1)) ? 2.0 : 1.0) / inSize;
                l.value().valueErrorPlus = m;

                ///////////////////////////////////////////////////////////////

                m = qSqrt(qPow(outM[k][0], 2.0) + qPow(outM[k][1], 2.0));
                m = m * ((k != 0) && (k != (outSize-1)) ? 2.0 : 1.0) / inSize;
                l.value().valueErrorMinus = m;

                break;
            }

            case GFFT_POWER:
            {
                double m = qSqrt(qPow(out[k][0], 2.0) + qPow(out[k][1], 2.0));
                m = m * ((k != 0) && (k != (outSize-1)) ? 2.0 : 1.0) / inSize;

                QCPDataMap::iterator l =
                const_cast<QCPDataMap *>(graph->m_FFTWPlot->data())->insert(
                frequency, QCPData(frequency, m * m));

                l.value().keyErrorPlus = 0.0;
                l.value().keyErrorMinus = 0.0;

                ///////////////////////////////////////////////////////////////

                m = qSqrt(qPow(outP[k][0], 2.0) + qPow(outP[k][1], 2.0));
                m = m * ((k != 0) && (k != (outSize-1)) ? 2.0 : 1.0) / inSize;
                l.value().valueErrorPlus = m * m;

                ///////////////////////////////////////////////////////////////

                m = qSqrt(qPow(outM[k][0], 2.0) + qPow(outM[k][1], 2.0));
                m = m * ((k != 0) && (k != (outSize-1)) ? 2.0 : 1.0) / inSize;
                l.value().valueErrorMinus = m * m;

                break;
            }

            case GFFT_MAGNITUDE_DB:
            {
                double m = qSqrt(qPow(out[k][0], 2.0) + qPow(out[k][1], 2.0));
                m = m * ((k != 0) && (k != (outSize-1)) ? 2.0 : 1.0) / inSize;

                QCPDataMap::iterator l =
                const_cast<QCPDataMap *>(graph->m_FFTWPlot->data())->insert(
                frequency, QCPData(frequency, 10.0 * log10(m)));

                l.value().keyErrorPlus = 0.0;
                l.value().keyErrorMinus = 0.0;

                ///////////////////////////////////////////////////////////////

                m = qSqrt(qPow(outP[k][0], 2.0) + qPow(outP[k][1], 2.0));
                m = m * ((k != 0) && (k != (outSize-1)) ? 2.0 : 1.0) / inSize;
                l.value().valueErrorPlus = 10.0 * log10(m);

                ///////////////////////////////////////////////////////////////

                m = qSqrt(qPow(outM[k][0], 2.0) + qPow(outM[k][1], 2.0));
                m = m * ((k != 0) && (k != (outSize-1)) ? 2.0 : 1.0) / inSize;
                l.value().valueErrorMinus = 10.0 * log10(m);

                break;
            }

            case GFFT_POWER_DB:
            {
                double m = qSqrt(qPow(out[k][0], 2.0) + qPow(out[k][1], 2.0));
                m = m * ((k != 0) && (k != (outSize-1)) ? 2.0 : 1.0) / inSize;

                QCPDataMap::iterator l =
                const_cast<QCPDataMap *>(graph->m_FFTWPlot->data())->insert(
                frequency, QCPData(frequency, 10.0 * log10(m * m)));

                l.value().keyErrorPlus = 0.0;
                l.value().keyErrorMinus = 0.0;

                ///////////////////////////////////////////////////////////////

                m = qSqrt(qPow(outP[k][0], 2.0) + qPow(outP[k][1], 2.0));
                m = m * ((k != 0) && (k != (outSize-1)) ? 2.0 : 1.0) / inSize;
                l.value().valueErrorPlus = 10.0 * log10(m * m);

                ///////////////////////////////////////////////////////////////

                m = qSqrt(qPow(outM[k][0], 2.0) + qPow(outM[k][1], 2.0));
                m = m * ((k != 0) && (k != (outSize-1)) ? 2.0 : 1.0) / inSize;
                l.value().valueErrorMinus = 10.0 * log10(m * m);

                break;
            }

            case GFFT_PHASE_RADIANS:
            {
                double p = qAtan2(out[k][1], out[k][0]);
                double d = p - pOld; pOld = p;

                if(qAbs(d) >= M_PI)
                {
                    pUnwrapper += (d < 0.0) ? (+2.0 * M_PI) : (-2.0 * M_PI);
                }

                QCPDataMap::iterator l = const_cast<QCPDataMap *>
                (graph->m_FFTWPlot->data())->insert(frequency,
                QCPData(frequency, p + pUnwrapper));

                l.value().keyErrorPlus = 0.0;
                l.value().keyErrorMinus = 0.0;

                ///////////////////////////////////////////////////////////////

                double pM = qAtan2(outM[k][1], outM[k][0]);
                double dM = pM - pOldM; pOldM = pM;

                if(qAbs(dM) >= M_PI)
                {
                    pUnwrapperM += (dM < 0.0) ? (+2.0 * M_PI) : (-2.0 * M_PI);
                }

                l.value().valueErrorPlus = pM + pUnwrapperM;

                ///////////////////////////////////////////////////////////////

                double pP = qAtan2(outP[k][1], outP[k][0]);
                double dP = pP - pOldP; pOldP = pP;

                if(qAbs(dP) >= M_PI)
                {
                    pUnwrapperP += (dP < 0.0) ? (+2.0 * M_PI) : (-2.0 * M_PI);
                }

                l.value().valueErrorPlus = pP + pUnwrapperP;

                break;
            }

            case GFFT_PHASE_DEGREES:
            {
                double p = qAtan2(out[k][1], out[k][0]);
                double d = p - pOld; pOld = p;

                if(qAbs(d) >= M_PI)
                {
                    pUnwrapper += (d < 0.0) ? (+2.0 * M_PI) : (-2.0 * M_PI);
                }

                QCPDataMap::iterator l = const_cast<QCPDataMap *>
                (graph->m_FFTWPlot->data())->insert(frequency,
                QCPData(frequency, qRadiansToDegrees(p + pUnwrapper)));

                l.value().keyErrorPlus = 0.0;
                l.value().keyErrorMinus = 0.0;

                ///////////////////////////////////////////////////////////////

                double pM = qAtan2(outM[k][1], outM[k][0]);
                double dM = pM - pOldM; pOldM = pM;

                if(qAbs(dM) >= M_PI)
                {
                    pUnwrapperM += (dM < 0.0) ? (+2.0 * M_PI) : (-2.0 * M_PI);
                }

                l.value().valueErrorPlus = qRadiansToDegrees(pM + pUnwrapperM);

                ///////////////////////////////////////////////////////////////

                double pP = qAtan2(outP[k][1], outP[k][0]);
                double dP = pP - pOldP; pOldP = pP;

                if(qAbs(dP) >= M_PI)
                {
                    pUnwrapperP += (dP < 0.0) ? (+2.0 * M_PI) : (-2.0 * M_PI);
                }

                l.value().valueErrorPlus = qRadiansToDegrees(pP + pUnwrapperP);

                break;
            }

            default: break;
        }
    }

    fftw_free(in);
    fftw_free(out);

    fftw_free(inM);
    fftw_free(outM);

    fftw_free(inP);
    fftw_free(outP);
}

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
