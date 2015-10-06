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

#ifndef SERIALGRAPHICS_H
#define SERIALGRAPHICS_H

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

namespace Ui
{
    class SerialGraphics;
}

class SerialGraphics : public SerialWindow
{
    Q_OBJECT

public:

    explicit SerialGraphics(const QString &title,
                            QSettings *settings = NULL,
                            QWidget *parent = NULL);

    virtual ~SerialGraphics();

    virtual bool importExportEnabled() const;

    virtual QString keyGroup() const;
    virtual QString keyState() const;
    virtual QString keyGeometry() const;

    void setBackgroundColor(QRgb rgba);
    QRgb getBackgroundColor() const;

    bool setCoordinateSystem(CoordinateSystem coordinateSystem);
    CoordinateSystem getCoordinateSystem() const;

    bool setAngleUnits(AngleUnits angleUnits);
    AngleUnits getAngleUnits() const;

    void setLineColor(QRgb rgba);
    QRgb getLineColor() const;

    bool setLineStyle(Qt::PenStyle lineStyle);
    Qt::PenStyle getLineStyle() const;

    void setFillColor(QRgb rgba);
    QRgb getFillColor() const;

    bool setFillStyle(Qt::BrushStyle fillStyle);
    Qt::BrushStyle getFillStyle() const;

    bool setRotation(double rotation);
    double getRotation() const;

    bool setScale(double scale);
    double getScale() const;

    ///////////////////////////////////////////////////////////////////////////

    bool drawText(double x, double y,
                  const QString &text);

    bool drawLine(double x1, double y1,
                  double x2, double y2);

    bool drawQuadraticLine(double x1, double y1,
                           double x2, double y2,
                           double x3, double y3);

    bool drawCubicLine(double x1, double y1,
                       double x2, double y2,
                       double x3, double y3,
                       double x4, double y4);

    bool drawArc(double x1, double y1,
                 double x2, double y2,
                 double startAngle,
                 double endAngle);

    bool drawChord(double x1, double y1,
                   double x2, double y2,
                   double startAngle,
                   double endAngle);

    bool drawTriangle(double x1, double y1,
                      double x2, double y2,
                      double x3, double y3);

    bool drawQuadrilateral(double x1, double y1,
                           double x2, double y2,
                           double x3, double y3,
                           double x4, double y4);

    bool drawRectangle(double x1, double y1,
                       double x2, double y2,
                       double xRadius = 0.0,
                       double yRadius = 0.0);

    bool drawEllipse(double x1, double y1,
                     double x2, double y2,
                     double startAngle = 0.0,
                     double endAngle = 0.0);

    ///////////////////////////////////////////////////////////////////////////

    bool clearAll(double x1, double y1,
                  double x2, double y2);

    void clearAll();

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
    void graphicsHelp();

protected:

    virtual bool eventFilter(QObject *object, QEvent *event);

    virtual void resizeEvent(QResizeEvent *event);

private:

    Q_DISABLE_COPY(SerialGraphics)

    QJsonObject toJson(const QString &function,
                       const QVariantList &args);

    bool notInfinite(const QString &FName,
                     const QString &VName,
                     double value);

    bool notZero(const QString &FName,
                 const QString &VName,
                 double value);

    bool notInvalid(const QString &FName,
                    const QString &VName,
                    double value,
                    double minValue,
                    double maxValue);

    bool m_resize; QWidget *m_widget; QGraphicsScene *m_scene;

    CoordinateSystem m_coordinateSystem; AngleUnits m_angleUnits;
    QPen m_pen; QBrush m_brush; double m_rotation, m_scale;

    int m_rasterWidth, m_rasterHeight, m_vectorWidth, m_vectorHeight;
    bool m_rasterSaveViewport, m_vectorSaveViewport;
    QString m_vectorTitle, m_vectorDescription;

    Ui::SerialGraphics *m_ui;
};

#endif // SERIALGRAPHICS_H

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
