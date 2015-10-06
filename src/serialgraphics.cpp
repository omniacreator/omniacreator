/***************************************************************************//**
* @file
* Serial Graphics
*
* @version @n 1.0
* @date @n 12/06/2013
*
* @author @n Kwabena W. Agyeman
* @copyright @n (c) 2013 Kwabena W. Agyeman
* @n All rights reserved - Please see the end of the file for the terms of use
*
* @par Update History:
* @n v1.0 - Original release - 12/06/2013
*******************************************************************************/

#include "serialgraphics.h"
#include "ui_serialgraphics.h"

SerialGraphics::SerialGraphics(const QString &title,
QSettings *settings, QWidget *parent) :
SerialWindow(title, settings, parent), m_ui(new Ui::SerialGraphics)
{
    m_ui->setupUi(this); m_ui->statusBar->hide();

    m_resize = true;
    m_widget = new QWidget;
    m_scene = new QGraphicsScene(-1000000000,
                                 -1000000000,
                                 +2000000001,
                                 +2000000001,
                                 this);

    m_widget->installEventFilter(this);
    m_scene->installEventFilter(this);
    m_ui->graphicsView->installEventFilter(this);
    m_ui->graphicsView->setViewport(m_widget);
    m_ui->graphicsView->setScene(m_scene);

    m_coordinateSystem = CS_CARTESIAN; m_angleUnits = AU_DEGREES;
    m_pen = QPen(QColor(000, 000, 255), 0.0); m_brush = QColor(200, 200, 255);
    m_rotation = 0.0; m_scale = 1.0;

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

    connect(m_ui->actionGraphics_Help, SIGNAL(triggered()),
            this, SLOT(graphicsHelp()));

    restoreState();
}

SerialGraphics::~SerialGraphics()
{
    delete m_ui;
}

bool SerialGraphics::importExportEnabled() const
{
    return true;
}

QString SerialGraphics::keyGroup() const
{
    return SERIAL_GRAPHICS_KEY_GROUP;
}

QString SerialGraphics::keyState() const
{
    return SERIAL_GRAPHICS_KEY_STATE;
}

QString SerialGraphics::keyGeometry() const
{
    return SERIAL_GRAPHICS_KEY_GEOMETRY;
}

void SerialGraphics::setBackgroundColor(QRgb rgba)
{
    m_ui->graphicsView->setBackgroundBrush(QColor::fromRgba(rgba));
}

QRgb SerialGraphics::getBackgroundColor() const
{
    return m_ui->graphicsView->backgroundBrush().color().rgba();
}

bool SerialGraphics::setCoordinateSystem(CoordinateSystem coordinateSystem)
{
    bool ok = notInvalid(__FUNCTION__, "coordinateSystem", coordinateSystem,
                         CS_CARTESIAN, CS_POLAR);

    if(ok)
    {
        m_coordinateSystem = coordinateSystem;
    }

    return ok;
}

CoordinateSystem SerialGraphics::getCoordinateSystem() const
{
    return m_coordinateSystem;
}

bool SerialGraphics::setAngleUnits(AngleUnits angleUnits)
{
    bool ok = notInvalid(__FUNCTION__, "angleUnits", angleUnits,
                         AU_DEGREES, AU_RADIANS);

    if(ok)
    {
        m_angleUnits = angleUnits;
    }

    return ok;
}

AngleUnits SerialGraphics::getAngleUnits() const
{
    return m_angleUnits;
}

void SerialGraphics::setLineColor(QRgb rgba)
{
    m_pen.setColor(QColor::fromRgba(rgba));
}

QRgb SerialGraphics::getLineColor() const
{
    return m_pen.color().rgba();
}

bool SerialGraphics::setLineStyle(Qt::PenStyle lineStyle)
{
    bool ok = notInvalid(__FUNCTION__, "lineStyle", lineStyle,
                         Qt::NoPen, Qt::DashDotDotLine);

    if(ok)
    {
        m_pen.setStyle(lineStyle);
    }

    return ok;
}

Qt::PenStyle SerialGraphics::getLineStyle() const
{
    return m_pen.style();
}

void SerialGraphics::setFillColor(QRgb rgba)
{
    m_brush.setColor(QColor::fromRgba(rgba));
}

QRgb SerialGraphics::getFillColor() const
{
    return m_brush.color().rgba();
}

bool SerialGraphics::setFillStyle(Qt::BrushStyle fillStyle)
{
    int min0 = Qt::NoBrush;
    int max0 = Qt::SolidPattern;
    int min1 = Qt::HorPattern;
    int max1 = Qt::DiagCrossPattern;

    bool ok = (((min0 <= fillStyle) && (fillStyle <= max0))
            || ((min1 <= fillStyle) && (fillStyle <= max1)));

    if(ok)
    {
        m_brush.setStyle(fillStyle);
    }
    else
    {
        emit errorMessage(QString(metaObject()->className()) +
        "::" + __FUNCTION__ + "[" + windowTitle() + "] -> " +
        tr("Argument (%L1 == %L2) is out of the valid range between "
        "[%L3 : %L4] + [%L5 : %L6]").arg("fillStyle").arg(fillStyle).
        arg(min0).arg(max0).arg(min1).arg(max1));
    }

    return ok;
}

Qt::BrushStyle SerialGraphics::getFillStyle() const
{
    return m_brush.style();
}

bool SerialGraphics::setRotation(double rotation)
{
    bool ok = notInfinite(__FUNCTION__, "rotation", rotation);

    if(ok)
    {
        if(m_angleUnits == AU_DEGREES)
        {
            m_rotation = rotation;
        }
        else
        {
            m_rotation = qRadiansToDegrees(rotation);
        }
    }

    return ok;
}

double SerialGraphics::getRotation() const
{
    if(m_angleUnits == AU_DEGREES)
    {
        return m_rotation;
    }
    else
    {
        return qDegreesToRadians(m_rotation);
    }
}

bool SerialGraphics::setScale(double scale)
{
    bool ok = notInfinite(__FUNCTION__, "scale", scale);

    if(ok)
    {
        m_scale = scale;
    }

    return ok;
}

double SerialGraphics::getScale() const
{
    return m_scale;
}

bool SerialGraphics::drawText(double x, double y,
                              const QString &text)
{
    bool ok = (notInfinite(__FUNCTION__, "x", x)
            && notInfinite(__FUNCTION__, "y", y)
            && (!text.isEmpty()));

    if(ok)
    {
        QVariantList list; QJsonObject object = toJson("drawText",
        list << x << y << text);

        if(m_coordinateSystem == CS_POLAR)
        {
            if(m_angleUnits == AU_DEGREES)
            {
                x = qDegreesToRadians(x);
            }

            x = y * qCos(x);
            y = y * qSin(x);
        }

        ///////////////////////////////////////////////////////////////////////

        QGraphicsSimpleTextItem *item = m_scene->addSimpleText(text);

        item->setPos(QPointF(x, y));
        item->setPen(m_pen);
        item->setBrush(m_brush);

        ///////////////////////////////////////////////////////////////////////

        item->setRotation(m_rotation); item->setScale(m_scale);

        item->setData(0, object);

        if(m_resize)
        {
            m_ui->graphicsView->fitInView(m_scene->itemsBoundingRect(),
                                          Qt::KeepAspectRatio);
        }
    }

    return ok;
}

bool SerialGraphics::drawLine(double x1, double y1,
                              double x2, double y2)
{
    bool ok = (notInfinite(__FUNCTION__, "x1", x1)
            && notInfinite(__FUNCTION__, "y1", y1)
            && notInfinite(__FUNCTION__, "x2", x2)
            && notInfinite(__FUNCTION__, "y2", y2));

    if(ok)
    {
        QVariantList list; QJsonObject object = toJson("drawLine",
        list << x1 << y1 << x2 << y2);

        if(m_coordinateSystem == CS_POLAR)
        {
            if(m_angleUnits == AU_DEGREES)
            {
                x1 = qDegreesToRadians(x1);
                x2 = qDegreesToRadians(x2);
            }

            x1 = y1 * qCos(x1);
            y1 = y1 * qSin(x1);

            x2 = y2 * qCos(x2);
            y2 = y2 * qSin(x2);
        }

        ///////////////////////////////////////////////////////////////////////

        QLineF line(QPointF(x1, y1), QPointF(x2, y2));

        QGraphicsLineItem *item = m_scene->addLine(line, m_pen);

        ///////////////////////////////////////////////////////////////////////

        item->setRotation(m_rotation); item->setScale(m_scale);

        item->setData(0, object);

        if(m_resize)
        {
            m_ui->graphicsView->fitInView(m_scene->itemsBoundingRect(),
                                          Qt::KeepAspectRatio);
        }
    }

    return ok;
}

bool SerialGraphics::drawQuadraticLine(double x1, double y1,
                                       double x2, double y2,
                                       double x3, double y3)
{
    bool ok = (notInfinite(__FUNCTION__, "x1", x1)
            && notInfinite(__FUNCTION__, "y1", y1)
            && notInfinite(__FUNCTION__, "x2", x2)
            && notInfinite(__FUNCTION__, "y2", y2)
            && notInfinite(__FUNCTION__, "x3", x3)
            && notInfinite(__FUNCTION__, "y3", y3));

    if(ok)
    {
        QVariantList list; QJsonObject object = toJson("drawQuadraticLine",
        list << x1 << y1 << x2 << y2 << x3 << y3);

        if(m_coordinateSystem == CS_POLAR)
        {
            if(m_angleUnits == AU_DEGREES)
            {
                x1 = qDegreesToRadians(x1);
                x2 = qDegreesToRadians(x2);
                x3 = qDegreesToRadians(x3);
            }

            x1 = y1 * qCos(x1);
            y1 = y1 * qSin(x1);

            x2 = y2 * qCos(x2);
            y2 = y2 * qSin(x2);

            x3 = y3 * qCos(x3);
            y3 = y3 * qSin(x3);
        }

        ///////////////////////////////////////////////////////////////////////

        QPainterPath path(QPointF(x1, y1)); path.quadTo(QPointF(x2, y2),
                                                        QPointF(x3, y3));

        QGraphicsPathItem *item = m_scene->addPath(path, m_pen);

        ///////////////////////////////////////////////////////////////////////

        item->setRotation(m_rotation); item->setScale(m_scale);

        item->setData(0, object);

        if(m_resize)
        {
            m_ui->graphicsView->fitInView(m_scene->itemsBoundingRect(),
                                          Qt::KeepAspectRatio);
        }
    }

    return ok;
}

bool SerialGraphics::drawCubicLine(double x1, double y1,
                                   double x2, double y2,
                                   double x3, double y3,
                                   double x4, double y4)
{
    bool ok = (notInfinite(__FUNCTION__, "x1", x1)
            && notInfinite(__FUNCTION__, "y1", y1)
            && notInfinite(__FUNCTION__, "x2", x2)
            && notInfinite(__FUNCTION__, "y2", y2)
            && notInfinite(__FUNCTION__, "x3", x3)
            && notInfinite(__FUNCTION__, "y3", y3)
            && notInfinite(__FUNCTION__, "x4", x4)
            && notInfinite(__FUNCTION__, "y4", y4));

    if(ok)
    {
        QVariantList list; QJsonObject object = toJson("drawCubicLine",
        list << x1 << y1 << x2 << y2 << x3 << y3 << x4 << y4);

        if(m_coordinateSystem == CS_POLAR)
        {
            if(m_angleUnits == AU_DEGREES)
            {
                x1 = qDegreesToRadians(x1);
                x2 = qDegreesToRadians(x2);
                x3 = qDegreesToRadians(x3);
                x4 = qDegreesToRadians(x4);
            }

            x1 = y1 * qCos(x1);
            y1 = y1 * qSin(x1);

            x2 = y2 * qCos(x2);
            y2 = y2 * qSin(x2);

            x3 = y3 * qCos(x3);
            y3 = y3 * qSin(x3);

            x4 = y4 * qCos(x4);
            y4 = y4 * qSin(x4);
        }

        ///////////////////////////////////////////////////////////////////////

        QPainterPath path(QPointF(x1, y1)); path.cubicTo(QPointF(x2, y2),
                                                         QPointF(x3, y3),
                                                         QPointF(x4, y4));

        QGraphicsPathItem *item = m_scene->addPath(path, m_pen);

        ///////////////////////////////////////////////////////////////////////

        item->setRotation(m_rotation); item->setScale(m_scale);

        item->setData(0, object);

        if(m_resize)
        {
            m_ui->graphicsView->fitInView(m_scene->itemsBoundingRect(),
                                          Qt::KeepAspectRatio);
        }
    }

    return ok;
}

bool SerialGraphics::drawArc(double x1, double y1,
                             double x2, double y2,
                             double startAngle,
                             double endAngle)
{
    bool ok = (notInfinite(__FUNCTION__, "x1", x1)
            && notInfinite(__FUNCTION__, "y1", y1)
            && notInfinite(__FUNCTION__, "x2", x2)
            && notInfinite(__FUNCTION__, "y2", y2)
            && notInfinite(__FUNCTION__, "startAngle", startAngle)
            && notInfinite(__FUNCTION__, "endAngle", endAngle));

    if(ok)
    {
        QVariantList list; QJsonObject object = toJson("drawArc",
        list << x1 << y1 << x2 << y2 << startAngle << endAngle);

        if(m_coordinateSystem == CS_POLAR)
        {
            if(m_angleUnits == AU_DEGREES)
            {
                x1 = qDegreesToRadians(x1);
                x2 = qDegreesToRadians(x2);
            }

            x1 = y1 * qCos(x1);
            y1 = y1 * qSin(x1);

            x2 = y2 * qCos(x2);
            y2 = y2 * qSin(x2);
        }

        ///////////////////////////////////////////////////////////////////////

        if(m_angleUnits == AU_RADIANS)
        {
            startAngle = qRadiansToDegrees(startAngle);
            endAngle = qRadiansToDegrees(endAngle);
        }

        QRectF rect(QPointF(x1, y1), QPointF(x2, y2)); QPainterPath path;

        path.arcMoveTo(rect, startAngle);
        path.arcTo(rect, startAngle, endAngle-startAngle);

        QGraphicsPathItem *item = m_scene->addPath(path, m_pen);

        ///////////////////////////////////////////////////////////////////////

        item->setRotation(m_rotation); item->setScale(m_scale);

        item->setData(0, object);

        if(m_resize)
        {
            m_ui->graphicsView->fitInView(m_scene->itemsBoundingRect(),
                                          Qt::KeepAspectRatio);
        }
    }

    return ok;
}

bool SerialGraphics::drawChord(double x1, double y1,
                               double x2, double y2,
                               double startAngle,
                               double endAngle)
{
    bool ok = (notInfinite(__FUNCTION__, "x1", x1)
            && notInfinite(__FUNCTION__, "y1", y1)
            && notInfinite(__FUNCTION__, "x2", x2)
            && notInfinite(__FUNCTION__, "y2", y2)
            && notInfinite(__FUNCTION__, "startAngle", startAngle)
            && notInfinite(__FUNCTION__, "endAngle", endAngle));

    if(ok)
    {
        QVariantList list; QJsonObject object = toJson("drawChord",
        list << x1 << y1 << x2 << y2 << startAngle << endAngle);

        if(m_coordinateSystem == CS_POLAR)
        {
            if(m_angleUnits == AU_DEGREES)
            {
                x1 = qDegreesToRadians(x1);
                x2 = qDegreesToRadians(x2);
            }

            x1 = y1 * qCos(x1);
            y1 = y1 * qSin(x1);

            x2 = y2 * qCos(x2);
            y2 = y2 * qSin(x2);
        }

        ///////////////////////////////////////////////////////////////////////

        if(m_angleUnits == AU_RADIANS)
        {
            startAngle = qRadiansToDegrees(startAngle);
            endAngle = qRadiansToDegrees(endAngle);
        }

        QRectF rect(QPointF(x1, y1), QPointF(x2, y2)); QPainterPath path;

        path.arcMoveTo(rect, startAngle);
        path.arcTo(rect, startAngle, endAngle-startAngle); path.closeSubpath();

        QGraphicsPathItem *item = m_scene->addPath(path, m_pen, m_brush);

        ///////////////////////////////////////////////////////////////////////

        item->setRotation(m_rotation); item->setScale(m_scale);

        item->setData(0, object);

        if(m_resize)
        {
            m_ui->graphicsView->fitInView(m_scene->itemsBoundingRect(),
                                          Qt::KeepAspectRatio);
        }
    }

    return ok;
}

bool SerialGraphics::drawTriangle(double x1, double y1,
                                  double x2, double y2,
                                  double x3, double y3)
{
    bool ok = (notInfinite(__FUNCTION__, "x1", x1)
            && notInfinite(__FUNCTION__, "y1", y1)
            && notInfinite(__FUNCTION__, "x2", x2)
            && notInfinite(__FUNCTION__, "y2", y2)
            && notInfinite(__FUNCTION__, "x3", x3)
            && notInfinite(__FUNCTION__, "y3", y3));

    if(ok)
    {
        QVariantList list; QJsonObject object = toJson("drawTriangle",
        list << x1 << y1 << x2 << y2 << x3 << y3);

        if(m_coordinateSystem == CS_POLAR)
        {
            if(m_angleUnits == AU_DEGREES)
            {
                x1 = qDegreesToRadians(x1);
                x2 = qDegreesToRadians(x2);
                x3 = qDegreesToRadians(x3);
            }

            x1 = y1 * qCos(x1);
            y1 = y1 * qSin(x1);

            x2 = y2 * qCos(x2);
            y2 = y2 * qSin(x2);

            x3 = y3 * qCos(x3);
            y3 = y3 * qSin(x3);
        }

        ///////////////////////////////////////////////////////////////////////

        QPolygonF poly;

        poly.append(QPointF(x1, y1));
        poly.append(QPointF(x2, y2));
        poly.append(QPointF(x3, y3));

        QGraphicsPolygonItem *item = m_scene->addPolygon(poly, m_pen, m_brush);

        ///////////////////////////////////////////////////////////////////////

        item->setRotation(m_rotation); item->setScale(m_scale);

        item->setData(0, object);

        if(m_resize)
        {
            m_ui->graphicsView->fitInView(m_scene->itemsBoundingRect(),
                                          Qt::KeepAspectRatio);
        }
    }

    return ok;
}

bool SerialGraphics::drawQuadrilateral(double x1, double y1,
                                       double x2, double y2,
                                       double x3, double y3,
                                       double x4, double y4)
{
    bool ok = (notInfinite(__FUNCTION__, "x1", x1)
            && notInfinite(__FUNCTION__, "y1", y1)
            && notInfinite(__FUNCTION__, "x2", x2)
            && notInfinite(__FUNCTION__, "y2", y2)
            && notInfinite(__FUNCTION__, "x3", x3)
            && notInfinite(__FUNCTION__, "y3", y3)
            && notInfinite(__FUNCTION__, "x4", x4)
            && notInfinite(__FUNCTION__, "y4", y4));

    if(ok)
    {
        QVariantList list; QJsonObject object = toJson("drawQuadrilateral",
        list << x1 << y1 << x2 << y2 << x3 << y3 << x4 << y4);

        if(m_coordinateSystem == CS_POLAR)
        {
            if(m_angleUnits == AU_DEGREES)
            {
                x1 = qDegreesToRadians(x1);
                x2 = qDegreesToRadians(x2);
                x3 = qDegreesToRadians(x3);
                x4 = qDegreesToRadians(x4);
            }

            x1 = y1 * qCos(x1);
            y1 = y1 * qSin(x1);

            x2 = y2 * qCos(x2);
            y2 = y2 * qSin(x2);

            x3 = y3 * qCos(x3);
            y3 = y3 * qSin(x3);

            x4 = y4 * qCos(x4);
            y4 = y4 * qSin(x4);
        }

        ///////////////////////////////////////////////////////////////////////

        QPolygonF poly;

        poly.append(QPointF(x1, y1));
        poly.append(QPointF(x2, y2));
        poly.append(QPointF(x3, y3));
        poly.append(QPointF(x4, y4));

        QGraphicsPolygonItem *item = m_scene->addPolygon(poly, m_pen, m_brush);

        ///////////////////////////////////////////////////////////////////////

        item->setRotation(m_rotation); item->setScale(m_scale);

        item->setData(0, object);

        if(m_resize)
        {
            m_ui->graphicsView->fitInView(m_scene->itemsBoundingRect(),
                                          Qt::KeepAspectRatio);
        }
    }

    return ok;
}

bool SerialGraphics::drawRectangle(double x1, double y1,
                                   double x2, double y2,
                                   double xRadius,
                                   double yRadius)
{
    bool ok = (notInfinite(__FUNCTION__, "x1", x1)
            && notInfinite(__FUNCTION__, "y1", y1)
            && notInfinite(__FUNCTION__, "x2", x2)
            && notInfinite(__FUNCTION__, "y2", y2)
            && notInvalid(__FUNCTION__, "xRadius", xRadius, 0.0, 100.0)
            && notInvalid(__FUNCTION__, "yRadius", yRadius, 0.0, 100.0));

    if(ok)
    {
        QVariantList list; QJsonObject object = toJson("drawRectangle",
        list << x1 << y1 << x2 << y2 << xRadius << yRadius);

        if(m_coordinateSystem == CS_POLAR)
        {
            if(m_angleUnits == AU_DEGREES)
            {
                x1 = qDegreesToRadians(x1);
                x2 = qDegreesToRadians(x2);
            }

            x1 = y1 * qCos(x1);
            y1 = y1 * qSin(x1);

            x2 = y2 * qCos(x2);
            y2 = y2 * qSin(x2);
        }

        ///////////////////////////////////////////////////////////////////////

        QRectF rect(QRectF(QPointF(x1,y1), QPointF(x2,y2))); QPainterPath path;

        path.addRoundedRect(rect, xRadius, yRadius, Qt::RelativeSize);

        QGraphicsPathItem *item = m_scene->addPath(path, m_pen, m_brush);

        ///////////////////////////////////////////////////////////////////////

        item->setRotation(m_rotation); item->setScale(m_scale);

        item->setData(0, object);

        if(m_resize)
        {
            m_ui->graphicsView->fitInView(m_scene->itemsBoundingRect(),
                                          Qt::KeepAspectRatio);
        }
    }

    return ok;
}

bool SerialGraphics::drawEllipse(double x1, double y1,
                                 double x2, double y2,
                                 double startAngle,
                                 double endAngle)
{
    bool ok = (notInfinite(__FUNCTION__, "x1", x1)
            && notInfinite(__FUNCTION__, "y1", y1)
            && notInfinite(__FUNCTION__, "x2", x2)
            && notInfinite(__FUNCTION__, "y2", y2)
            && notInfinite(__FUNCTION__, "startAngle", startAngle)
            && notInfinite(__FUNCTION__, "endAngle", endAngle));

    if(ok)
    {
        QVariantList list; QJsonObject object = toJson("drawEllipse",
        list << x1 << y1 << x2 << y2 << startAngle << endAngle);

        if(m_coordinateSystem == CS_POLAR)
        {
            if(m_angleUnits == AU_DEGREES)
            {
                x1 = qDegreesToRadians(x1);
                x2 = qDegreesToRadians(x2);
            }

            x1 = y1 * qCos(x1);
            y1 = y1 * qSin(x1);

            x2 = y2 * qCos(x2);
            y2 = y2 * qSin(x2);
        }

        ///////////////////////////////////////////////////////////////////////

        QRectF rect(QPointF(x1,y1), QPointF(x2,y2));

        QGraphicsEllipseItem *item = m_scene->addEllipse(rect, m_pen, m_brush);

        if(m_angleUnits == AU_RADIANS)
        {
            startAngle = qRadiansToDegrees(startAngle);
            endAngle = qRadiansToDegrees(endAngle);
        }

        item->setStartAngle(startAngle);
        item->setSpanAngle(endAngle - startAngle);

        ///////////////////////////////////////////////////////////////////////

        item->setRotation(m_rotation); item->setScale(m_scale);

        item->setData(0, object);

        if(m_resize)
        {
            m_ui->graphicsView->fitInView(m_scene->itemsBoundingRect(),
                                          Qt::KeepAspectRatio);
        }
    }

    return ok;
}

bool SerialGraphics::clearAll(double x1, double y1,
                              double x2, double y2)
{
    bool ok = (notInfinite(__FUNCTION__, "x1", x1)
            && notInfinite(__FUNCTION__, "y1", y1)
            && notInfinite(__FUNCTION__, "x2", x2)
            && notInfinite(__FUNCTION__, "y2", y2));

    if(ok)
    {
        if(m_coordinateSystem == CS_POLAR)
        {
            if(m_angleUnits == AU_DEGREES)
            {
                x1 = qDegreesToRadians(x1);
                x2 = qDegreesToRadians(x2);
            }

            x1 = y1 * qCos(x1);
            y1 = y1 * qSin(x1);

            x2 = y2 * qCos(x2);
            y2 = y2 * qSin(x2);
        }

        QRectF rect(QPointF(x1, y1), QPointF(x2, y2));

        foreach(QGraphicsItem *item, m_scene->items(rect))
        {
            m_scene->removeItem(item); delete item;
        }
    }

    return ok;
}

void SerialGraphics::clearAll()
{
    m_scene->clear();
}

void SerialGraphics::zoomIn()
{
    QRect geometry = m_ui->graphicsView->geometry();

    QPointF localPos(geometry.width()/2.0, geometry.height()/2.0);
    QPointF globalPos(geometry.x()+localPos.x(), geometry.y()+localPos.y());

    QWheelEvent wheelEvent(localPos, globalPos, QPoint(0, +1), QPoint(0, +120),
    +120, Qt::Vertical, Qt::NoButton, Qt::NoModifier);

    QApplication::sendEvent(m_ui->graphicsView, &wheelEvent);
}

void SerialGraphics::zoomOut()
{
    QRect geometry = m_ui->graphicsView->geometry();

    QPointF localPos(geometry.width()/2.0, geometry.height()/2.0);
    QPointF globalPos(geometry.x()+localPos.x(), geometry.y()+localPos.y());

    QWheelEvent wheelEvent(localPos, globalPos, QPoint(0, -1), QPoint(0, -120),
    -120, Qt::Vertical, Qt::NoButton, Qt::NoModifier);

    QApplication::sendEvent(m_ui->graphicsView, &wheelEvent);
}

void SerialGraphics::zoomFit()
{
    m_ui->graphicsView->fitInView(m_scene->itemsBoundingRect(),
                                  Qt::KeepAspectRatio);

    m_resize = true;
}

void SerialGraphics::saveRasterImage()
{
    QSettings settings(settingsFileName(), settingsFormat());

    settings.beginGroup(keyGroup());
    settings.beginGroup(windowTitle());

    QString saveFile = settings.value(SERIAL_GRAPHICS_KEY_SAVE_R_FILE,
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

                pixmap.fill(m_ui->graphicsView->backgroundBrush().color());

                QPainter painter;

                if(!painter.begin(&pixmap))
                {
                    QMessageBox::critical(this, tr("Save Image Error"),
                    tr("Painting initialization failed")); return;
                }

                if(dialog.getSaveCurrentView())
                {
                    m_ui->graphicsView->render(&painter, QRectF(), QRect(),
                    Qt::IgnoreAspectRatio);
                }
                else
                {
                    QRectF rect = m_scene->itemsBoundingRect();

                    QRectF newRect(QPointF(), rect.size() * 1.2);
                    newRect.moveCenter(rect.center());

                    m_scene->render(&painter, QRectF(), newRect,
                    Qt::IgnoreAspectRatio);
                }

                if((!painter.end()) || (!pixmap.save(fileName)))
                {
                    QMessageBox::critical(this, tr("Save Image Error"),
                    tr("Painting finalization failed")); return;
                }

                QFileInfo fileInfo(fileName);

                settings.setValue(SERIAL_GRAPHICS_KEY_SAVE_R_FILE,
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

void SerialGraphics::saveVectorImage()
{
    QSettings settings(settingsFileName(), settingsFormat());

    settings.beginGroup(keyGroup());
    settings.beginGroup(windowTitle());

    QString saveFile = settings.value(SERIAL_GRAPHICS_KEY_SAVE_V_FILE,
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

                QPainter painter;

                if(!painter.begin(&generator))
                {
                    QMessageBox::critical(this, tr("Save Image Error"),
                    tr("Painting initialization failed")); return;
                }

                // TODO: RENDER IS BUGGED!!!

                if(dialog.getSaveCurrentView())
                {
                    m_ui->graphicsView->render(&painter, QRectF(), QRect(),
                    Qt::IgnoreAspectRatio);
                }
                else
                {
                    QRectF rect = m_scene->itemsBoundingRect();

                    QRectF newRect(QPointF(), rect.size() * 1.2);
                    newRect.moveCenter(rect.center());

                    m_scene->render(&painter, QRectF(), newRect,
                    Qt::IgnoreAspectRatio);
                }

                if(!painter.end())
                {
                    QMessageBox::critical(this, tr("Save Image Error"),
                    tr("Painting finalization failed")); return;
                }

                QFileInfo fileInfo(fileName);

                settings.setValue(SERIAL_GRAPHICS_KEY_SAVE_V_FILE,
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

                    QPainter painter;

                    if(!painter.begin(&printer))
                    {
                        QMessageBox::critical(this, tr("Save Image Error"),
                        tr("Painting initialization failed")); return;
                    }

                    if(dialog.getSaveCurrentView())
                    {
                        m_ui->graphicsView->render(&painter, QRectF(), QRect(),
                        Qt::IgnoreAspectRatio);
                    }
                    else
                    {
                        QRectF rect = m_scene->itemsBoundingRect();

                        QRectF newRect(QPointF(), rect.size() * 1.2);
                        newRect.moveCenter(rect.center());

                        m_scene->render(&painter, QRectF(), newRect,
                        Qt::IgnoreAspectRatio);
                    }

                    if(!painter.end())
                    {
                        QMessageBox::critical(this, tr("Save Image Error"),
                        tr("Painting finalization failed")); return;
                    }

                    QFileInfo fileInfo(fileName);

                    settings.setValue(SERIAL_GRAPHICS_KEY_SAVE_V_FILE,
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

void SerialGraphics::importState(const QString &fileName)
{
    QSettings settings(settingsFileName(), settingsFormat());

    settings.beginGroup(keyGroup());
    settings.beginGroup(windowTitle());

    QString openFile = settings.value(SERIAL_GRAPHICS_KEY_IMPORT_FILE,
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

                if(object.value("type").toString() == "graphics")
                {
                    clearAll();

                    setWindowTitle(object.value("title").toString());

                    QString background = object.value("background").toString();

                    if(QColor::isValidColor(background))
                    {
                        setBackgroundColor(QColor(background).rgba());
                    }

                    QJsonArray items = object.value("items").toArray();

                    for(int i = 0, j = items.size(); i < j; i++)
                    {
                        QJsonObject item = items.at(i).toObject();

                        setCoordinateSystem(static_cast<CoordinateSystem>(item.
                        value("coorinateSystem").toInt(m_coordinateSystem)));

                        setAngleUnits(static_cast<AngleUnits>(item.
                        value("angleUnits").toInt(m_angleUnits)));

                        QString lineColor = item.value("lineColor").toString();

                        if(QColor::isValidColor(lineColor))
                        {
                            setLineColor(QColor(lineColor).rgba());
                        }

                        setLineStyle(static_cast<Qt::PenStyle>(item.
                        value("lineStyle").toInt(m_pen.style())));

                        QString fillColor = item.value("fillColor").toString();

                        if(QColor::isValidColor(fillColor))
                        {
                            setFillColor(QColor(fillColor).rgba());
                        }

                        setFillStyle(static_cast<Qt::BrushStyle>(item.
                        value("fillStyle").toInt(m_brush.style())));

                        setRotation(item.
                        value("rotation").toDouble(m_rotation));

                        setScale(item.
                        value("scale").toDouble(m_scale));

                        QString function =
                        item.value("function").toString();

                        QVariantList args =
                        item.value("args").toArray().toVariantList();

                        if((function == "drawText")
                        && (args.size() == 3))
                        {
                            double x = args.takeFirst().toDouble();
                            double y = args.takeFirst().toDouble();

                            QString text = args.takeFirst().toString();

                            drawText(x, y, text);
                        }
                        else if((function == "drawLine")
                        && (args.size() == 4))
                        {
                            double x1 = args.takeFirst().toDouble();
                            double y1 = args.takeFirst().toDouble();
                            double x2 = args.takeFirst().toDouble();
                            double y2 = args.takeFirst().toDouble();

                            drawLine(x1, y1, x2, y2);
                        }
                        else if((function == "drawQuadraticLine")
                        && (args.size() == 6))
                        {
                            double x1 = args.takeFirst().toDouble();
                            double y1 = args.takeFirst().toDouble();
                            double x2 = args.takeFirst().toDouble();
                            double y2 = args.takeFirst().toDouble();
                            double x3 = args.takeFirst().toDouble();
                            double y3 = args.takeFirst().toDouble();

                            drawQuadraticLine(x1, y1, x2, y2, x3, y3);
                        }
                        else if((function == "drawCubicLine")
                        && (args.size() == 8))
                        {
                            double x1 = args.takeFirst().toDouble();
                            double y1 = args.takeFirst().toDouble();
                            double x2 = args.takeFirst().toDouble();
                            double y2 = args.takeFirst().toDouble();
                            double x3 = args.takeFirst().toDouble();
                            double y3 = args.takeFirst().toDouble();
                            double x4 = args.takeFirst().toDouble();
                            double y4 = args.takeFirst().toDouble();

                            drawCubicLine(x1, y1, x2, y2, x3, y3, x4, y4);
                        }
                        else if((function == "drawArc")
                        && (args.size() == 6))
                        {
                            double x1 = args.takeFirst().toDouble();
                            double y1 = args.takeFirst().toDouble();
                            double x2 = args.takeFirst().toDouble();
                            double y2 = args.takeFirst().toDouble();
                            double startAngle = args.takeFirst().toDouble();
                            double endAngle = args.takeFirst().toDouble();

                            drawArc(x1, y1, x2, y2, startAngle, endAngle);
                        }
                        else if((function == "drawChord")
                        && (args.size() == 6))
                        {
                            double x1 = args.takeFirst().toDouble();
                            double y1 = args.takeFirst().toDouble();
                            double x2 = args.takeFirst().toDouble();
                            double y2 = args.takeFirst().toDouble();
                            double startAngle = args.takeFirst().toDouble();
                            double endAngle = args.takeFirst().toDouble();

                            drawChord(x1, y1, x2, y2, startAngle, endAngle);
                        }
                        else if((function == "drawTriangle")
                        && (args.size() == 6))
                        {
                            double x1 = args.takeFirst().toDouble();
                            double y1 = args.takeFirst().toDouble();
                            double x2 = args.takeFirst().toDouble();
                            double y2 = args.takeFirst().toDouble();
                            double x3 = args.takeFirst().toDouble();
                            double y3 = args.takeFirst().toDouble();

                            drawTriangle(x1, y1, x2, y2, x3, y3);
                        }
                        else if((function == "drawQuadrilateral")
                        && (args.size() == 8))
                        {
                            double x1 = args.takeFirst().toDouble();
                            double y1 = args.takeFirst().toDouble();
                            double x2 = args.takeFirst().toDouble();
                            double y2 = args.takeFirst().toDouble();
                            double x3 = args.takeFirst().toDouble();
                            double y3 = args.takeFirst().toDouble();
                            double x4 = args.takeFirst().toDouble();
                            double y4 = args.takeFirst().toDouble();

                            drawQuadrilateral(x1, y1, x2, y2, x3, y3, x4, y4);
                        }
                        else if((function == "drawRectangle")
                        && (args.size() == 6))
                        {
                            double x1 = args.takeFirst().toDouble();
                            double y1 = args.takeFirst().toDouble();
                            double x2 = args.takeFirst().toDouble();
                            double y2 = args.takeFirst().toDouble();
                            double xRadius = args.takeFirst().toDouble();
                            double yRadius = args.takeFirst().toDouble();

                            drawRectangle(x1, y1, x2, y2, xRadius, yRadius);
                        }
                        else if((function == "drawEllipse")
                        && (args.size() == 6))
                        {
                            double x1 = args.takeFirst().toDouble();
                            double y1 = args.takeFirst().toDouble();
                            double x2 = args.takeFirst().toDouble();
                            double y2 = args.takeFirst().toDouble();
                            double startAngle = args.takeFirst().toDouble();
                            double endAngle = args.takeFirst().toDouble();

                            drawEllipse(x1, y1, x2, y2, startAngle, endAngle);
                        }
                    }

                    QFileInfo fileInfo(temp);

                    settings.setValue(SERIAL_GRAPHICS_KEY_IMPORT_FILE,
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

void SerialGraphics::exportState(const QString &fileName)
{
    QSettings settings(settingsFileName(), settingsFormat());

    settings.beginGroup(keyGroup());
    settings.beginGroup(windowTitle());

    QString saveFile = settings.value(SERIAL_GRAPHICS_KEY_EXPORT_FILE,
                                      QDir::homePath()).toString();

    QString temp = fileName.isEmpty() ? QFileDialog::getSaveFileName(this,
    tr("Export State"), saveFile, tr("JSON Files (*.json)")) : fileName;

    if(!temp.isEmpty())
    {
        QFile file(temp);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QJsonObject object;

            object.insert("type", QString("graphics"));
            object.insert("title", windowTitle());

            object.insert("background",
            QColor::fromRgba(getBackgroundColor()).name(QColor::HexArgb));

            QJsonArray items;

            foreach(QGraphicsItem *item, m_scene->items(Qt::AscendingOrder))
            {
                items.append(item->data(0).toJsonObject());
            }

            object.insert("items", items);

            QByteArray json = QJsonDocument(object).toJson();

            if(file.write(json) == json.size())
            {
                QFileInfo fileInfo(temp);

                settings.setValue(SERIAL_GRAPHICS_KEY_EXPORT_FILE,
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

void SerialGraphics::generalHelp()
{
    if(!QDesktopServices::openUrl(QUrl("http://" PROJECT_DOMAIN_NAME_STR "/"
    "help/general/")))
    {
        QMessageBox::critical(this, tr("Open General Help Error"),
        tr("Unable to open the URL to the General Help page"));
    }
}

void SerialGraphics::graphicsHelp()
{
    if(!QDesktopServices::openUrl(QUrl("http://" PROJECT_DOMAIN_NAME_STR "/"
    "help/widgets/graphics/")))
    {
        QMessageBox::critical(this, tr("Open Graphics Help Error"),
        tr("Unable to open the URL to the Graphics Help page"));
    }
}

bool SerialGraphics::eventFilter(QObject *object, QEvent *event)
{
    if((object == m_ui->graphicsView)
    || (object == m_widget)
    || (object == m_scene))
    {
        if(event->type() == QEvent::MouseButtonDblClick)
        {
            m_ui->graphicsView->fitInView(m_scene->itemsBoundingRect(),
                                          Qt::KeepAspectRatio);

            m_resize = true;
        }

        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

            if(mouseEvent->buttons() & Qt::LeftButton)
            {
                m_resize = false;
            }
        }

        if(event->type() == QEvent::Wheel)
        {
            QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);

            double pixelDelta = qPow(1.2, wheelEvent->pixelDelta().y()/001.0);
            double angleDelta = qPow(1.2, wheelEvent->angleDelta().y()/120.0);

            Q_UNUSED(pixelDelta);

            m_ui->graphicsView->scale(angleDelta, angleDelta);

            m_resize = false;
        }

//        if(event->type() == QEvent::GraphicsSceneWheel)
//        {
//            QGraphicsSceneWheelEvent *wheelEvent =
//            static_cast<QGraphicsSceneWheelEvent *>(event);

//            double delta = qPow(1.2, wheelEvent->delta() / 120.0);

//            m_ui->graphicsView->scale(delta, delta);

//            m_resize = false;

//            return true;
//        }

        if(event->type() == QEvent::ToolTip)
        {
            QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);

            if(m_ui->graphicsView->itemAt(helpEvent->pos()))
            {
                QPointF pos = m_ui->graphicsView->mapToScene(helpEvent->pos());

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
                                      arg(pos.x()).
                                      arg(pos.y()));
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

            if(m_ui->graphicsView->itemAt(helpEvent->pos()))
            {
                QPointF pos = m_ui->graphicsView->mapToScene(helpEvent->pos());

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
                                        arg(pos.x()).
                                        arg(pos.y()));
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

void SerialGraphics::resizeEvent(QResizeEvent *event)
{
    if(m_resize)
    {
        m_ui->graphicsView->fitInView(m_scene->itemsBoundingRect(),
                                      Qt::KeepAspectRatio);
    }

    SerialWindow::resizeEvent(event);
}

QJsonObject SerialGraphics::toJson(const QString &function,
                                   const QVariantList &args)
{
    QJsonObject object;

    object.insert("coorinateSystem", m_coordinateSystem);
    object.insert("angleUnits", m_angleUnits);
    object.insert("lineColor", m_pen.color().name(QColor::HexArgb));
    object.insert("lineStyle", m_pen.style());
    object.insert("fillColor", m_brush.color().name(QColor::HexArgb));
    object.insert("fillStyle", m_brush.style());
    object.insert("rotation", m_rotation);
    object.insert("scale", m_scale);
    object.insert("function", function);
    object.insert("args", QJsonArray::fromVariantList(args));

    return object;
}

bool SerialGraphics::notInfinite(const QString &FName,
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

bool SerialGraphics::notZero(const QString &FName,
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

bool SerialGraphics::notInvalid(const QString &FName,
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
