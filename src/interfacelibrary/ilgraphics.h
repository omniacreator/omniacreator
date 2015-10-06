/***************************************************************************//**
* @file
* Interface Library Graphics
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

#ifndef ILGRAPHICS_H
#define ILGRAPHICS_H

#include "ilcore.h"

typedef struct GraphicsRect
{
    int32 x1, y1, x2, y2;
}
GraphicsRect;

typedef struct GraphicsRectF
{
    float32 x1, y1, x2, y2;
}
GraphicsRectF;

typedef struct GraphicsPoint
{
    int32 x, y;
}
GraphicsPoint;

typedef struct GraphicsPointF
{
    float32 x, y;
}
GraphicsPointF;

typedef struct GraphicsLine
{
    int32 x1, y1, x2, y2;
}
GraphicsLine;

typedef struct GraphicsLineF
{
    float32 x1, y1, x2, y2;
}
GraphicsLineF;

typedef struct GraphicsQuadraticLine
{
    int32 x1, y1, x2, y2, x3, y3;
}
GraphicsQuadraticLine;

typedef struct GraphicsQuadraticLineF
{
    float32 x1, y1, x2, y2, x3, y3;
}
GraphicsQuadraticLineF;

typedef struct GraphicsCubicLine
{
    int32 x1, y1, x2, y2, x3, y3, x4, y4;
}
GraphicsCubicLine;

typedef struct GraphicsCubicLineF
{
    float32 x1, y1, x2, y2, x3, y3, x4, y4;
}
GraphicsCubicLineF;

typedef struct GraphicsArc
{
    int32 x1, y1, x2, y2, startAngle, endAngle;
}
GraphicsArc;

typedef struct GraphicsArcF
{
    float32 x1, y1, x2, y2, startAngle, endAngle;
}
GraphicsArcF;

typedef struct GraphicsChord
{
    int32 x1, y1, x2, y2, startAngle, endAngle;
}
GraphicsChord;

typedef struct GraphicsChordF
{
    float32 x1, y1, x2, y2, startAngle, endAngle;
}
GraphicsChordF;

typedef struct GraphicsTriangle
{
    int32 x1, y1, x2, y2, x3, y3;
}
GraphicsTriangle;

typedef struct GraphicsTriangleF
{
    float32 x1, y1, x2, y2, x3, y3;
}
GraphicsTriangleF;

typedef struct GraphicsQuadrilateral
{
    int32 x1, y1, x2, y2, x3, y3, x4, y4;
}
GraphicsQuadrilateral;

typedef struct GraphicsQuadrilateralF
{
    float32 x1, y1, x2, y2, x3, y3, x4, y4;
}
GraphicsQuadrilateralF;

typedef struct GraphicsRectangle
{
    int32 x1, y1, x2, y2, xRadius, yRadius;
}
GraphicsRectangle;

typedef struct GraphicsRectangleF
{
    float32 x1, y1, x2, y2, xRadius, yRadius;
}
GraphicsRectangleF;

typedef struct GraphicsEllipse
{
    int32 x1, y1, x2, y2, startAngle, endAngle;
}
GraphicsEllipse;

typedef struct GraphicsEllipseF
{
    float32 x1, y1, x2, y2, startAngle, endAngle;
}
GraphicsEllipseF;

class ILGraphics : public ILWindow
{

public:

    ILGraphics() : ILWindow()
    {
        m_backgroundColor = argb();

        m_coordinateSystem = CoordinateSystem(); m_angleUnits = AngleUnits();

        m_lineColor = argb(); m_lineStyle = LineStyle();
        m_fillColor = argb(); m_fillStyle = FillStyle();

        m_rotation = int32(); m_rotationF = float32();
        m_scale = int32(); m_scaleF = float32();
    }

    virtual ILNodeType type() const
    {
        return NT_WINDOW_GRAPHICS;
    }

    bool initUndocked(ILCore *parent, const char *name,
                      bool waitForResponse = false)
    {
        bool result = (!m_enabled) && parent && parent->enabled() && name;

        if(result)
        {
            construct(parent);

            if(!(result = sendPacket(GRAPHICS_NEW_UNDOCKED_WINDOW,
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

//            if(!(result = sendPacket(GRAPHICS_NEW_DOCKED_WINDOW,
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

            if(!(result = sendPacket(GRAPHICS_NEW_MAIN_DOCKED_WINDOW,
            name, waitForResponse)))
            {
                deconstruct();
            }
        }

        return result;
    }

    bool clearAll(int32 x1, int32 y1, int32 x2, int32 y2,
                  bool waitForResponse = false)
    {
        GraphicsRect rect;

        rect.x1 = x1;
        rect.y1 = y1;
        rect.x2 = x2;
        rect.y2 = y2;

        return clearAll(rect, waitForResponse);
    }

    bool clearAll(const GraphicsRect &rect,
                  bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_CLEAR_ALL_0,
        reinterpret_cast<const int32 *>(&rect),
        sizeof(GraphicsRect) / sizeof(int32),
        waitForResponse);
    }

    bool clearAlls(const GraphicsRect *rect, uint16 size,
                   bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_CLEAR_ALL_0_A,
        reinterpret_cast<const int32 *>(rect), size,
        sizeof(GraphicsRect) / sizeof(int32),
        waitForResponse);
    }

    bool clearAllF(float32 x1, float32 y1, float32 x2, float32 y2,
                   bool waitForResponse = false)
    {
        GraphicsRectF rect;

        rect.x1 = x1;
        rect.y1 = y1;
        rect.x2 = x2;
        rect.y2 = y2;

        return clearAllF(rect, waitForResponse);
    }

    bool clearAllF(const GraphicsRectF &rect,
                   bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_CLEAR_ALL_0_F,
        reinterpret_cast<const float32 *>(&rect),
        sizeof(GraphicsRectF) / sizeof(float32),
        waitForResponse);
    }

    bool clearAllsF(const GraphicsRectF *rect, uint16 size,
                    bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_CLEAR_ALL_0_F_A,
        reinterpret_cast<const float32 *>(rect), size,
        sizeof(GraphicsRectF) / sizeof(float32),
        waitForResponse);
    }

    bool clearAll(bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_CLEAR_ALL_1,
        waitForResponse);
    }

    // Draw Text //////////////////////////////////////////////////////////////

    bool drawText(int32 x, int32 y, const char *text,
                  bool waitForResponse = false)
    {
        GraphicsPoint point;

        point.x = x;
        point.y = y;

        return drawText(point, text, waitForResponse);
    }

    bool drawText(const GraphicsPoint &point, const char *text,
                  bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_TEXT,
        reinterpret_cast<const int32 *>(&point),
        sizeof(GraphicsPoint) / sizeof(int32),
        text, waitForResponse);
    }

    bool drawTexts(const GraphicsPoint *point, uint16 size,
                   const char **texts, bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_TEXT_A,
        reinterpret_cast<const int32 *>(point), size,
        sizeof(GraphicsPoint) / sizeof(int32),
        texts, waitForResponse);
    }

    bool drawTextF(float32 x, float32 y, const char *text,
                   bool waitForResponse = false)
    {
        GraphicsPointF point;

        point.x = x;
        point.y = y;

        return drawTextF(point, text, waitForResponse);
    }

    bool drawTextF(const GraphicsPointF &point, const char *text,
                   bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_TEXT_F,
        reinterpret_cast<const float32 *>(&point),
        sizeof(GraphicsPointF) / sizeof(float32),
        text, waitForResponse);
    }

    bool drawTextsF(const GraphicsPointF *point, uint16 size,
                    const char **texts, bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_TEXT_F_A,
        reinterpret_cast<const float32 *>(point), size,
        sizeof(GraphicsPointF) / sizeof(float32),
        texts, waitForResponse);
    }

    // Draw Line //////////////////////////////////////////////////////////////

    bool drawLine(int32 x1, int32 y1, int32 x2, int32 y2,
                  bool waitForResponse = false)
    {
        GraphicsLine line;

        line.x1 = x1;
        line.y1 = y1;
        line.x2 = x2;
        line.y2 = y2;

        return drawLine(line, waitForResponse);
    }

    bool drawLine(const GraphicsLine &line,
                  bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_LINE,
        reinterpret_cast<const int32 *>(&line),
        sizeof(GraphicsLine) / sizeof(int32),
        waitForResponse);
    }

    bool drawLines(const GraphicsLine *line, uint16 size,
                   bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_LINE_A,
        reinterpret_cast<const int32 *>(line), size,
        sizeof(GraphicsLine) / sizeof(int32),
        waitForResponse);
    }

    bool drawLineF(float32 x1, float32 y1, float32 x2, float32 y2,
                   bool waitForResponse = false)
    {
        GraphicsLineF line;

        line.x1 = x1;
        line.y1 = y1;
        line.x2 = x2;
        line.y2 = y2;

        return drawLineF(line, waitForResponse);
    }

    bool drawLineF(const GraphicsLineF &line,
                   bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_LINE_F,
        reinterpret_cast<const float32 *>(&line),
        sizeof(GraphicsLineF) / sizeof(float32),
        waitForResponse);
    }

    bool drawLinesF(const GraphicsLineF *line, uint16 size,
                    bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_LINE_F_A,
        reinterpret_cast<const float32 *>(line), size,
        sizeof(GraphicsLineF) / sizeof(float32),
        waitForResponse);
    }

    // Draw Quadratic Line ////////////////////////////////////////////////////

    bool drawQuadraticLine(int32 x1, int32 y1,
                           int32 x2, int32 y2,
                           int32 x3, int32 y3,
                           bool waitForResponse = false)
    {
        GraphicsQuadraticLine quadraticLine;

        quadraticLine.x1 = x1;
        quadraticLine.y1 = y1;
        quadraticLine.x2 = x2;
        quadraticLine.y2 = y2;
        quadraticLine.x3 = x3;
        quadraticLine.y3 = y3;

        return drawQuadraticLine(quadraticLine, waitForResponse);
    }

    bool drawQuadraticLine(const GraphicsQuadraticLine &quadraticLine,
                           bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_QUADRATIC_LINE,
        reinterpret_cast<const int32 *>(&quadraticLine),
        sizeof(GraphicsQuadraticLine) / sizeof(int32),
        waitForResponse);
    }

    bool drawQuadraticLines(const GraphicsQuadraticLine *quadraticLine,
                            uint16 size, bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_QUADRATIC_LINE_A,
        reinterpret_cast<const int32 *>(quadraticLine), size,
        sizeof(GraphicsQuadraticLine) / sizeof(int32),
        waitForResponse);
    }

    bool drawQuadraticLineF(float32 x1, float32 y1,
                            float32 x2, float32 y2,
                            float32 x3, float32 y3,
                            bool waitForResponse = false)
    {
        GraphicsQuadraticLineF quadraticLine;

        quadraticLine.x1 = x1;
        quadraticLine.y1 = y1;
        quadraticLine.x2 = x2;
        quadraticLine.y2 = y2;
        quadraticLine.x3 = x3;
        quadraticLine.y3 = y3;

        return drawQuadraticLineF(quadraticLine, waitForResponse);
    }

    bool drawQuadraticLineF(const GraphicsQuadraticLineF &quadraticLine,
                            bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_QUADRATIC_LINE_F,
        reinterpret_cast<const float32 *>(&quadraticLine),
        sizeof(GraphicsQuadraticLineF) / sizeof(float32),
        waitForResponse);
    }

    bool drawQuadraticLinesF(const GraphicsQuadraticLineF *quadraticLine,
                             uint16 size, bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_QUADRATIC_LINE_F_A,
        reinterpret_cast<const float32 *>(quadraticLine), size,
        sizeof(GraphicsQuadraticLineF) / sizeof(float32),
        waitForResponse);
    }

    // Draw Cubic Line ////////////////////////////////////////////////////////

    bool drawCubicLine(int32 x1, int32 y1,
                       int32 x2, int32 y2,
                       int32 x3, int32 y3,
                       int32 x4, int32 y4,
                       bool waitForResponse = false)
    {
        GraphicsCubicLine cubicLine;

        cubicLine.x1 = x1;
        cubicLine.y1 = y1;
        cubicLine.x2 = x2;
        cubicLine.y2 = y2;
        cubicLine.x3 = x3;
        cubicLine.y3 = y3;
        cubicLine.x4 = x4;
        cubicLine.y4 = y4;

        return drawCubicLine(cubicLine, waitForResponse);
    }

    bool drawCubicLine(const GraphicsCubicLine &cubicLine,
                       bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_CUBIC_LINE,
        reinterpret_cast<const int32 *>(&cubicLine),
        sizeof(GraphicsCubicLine) / sizeof(int32),
        waitForResponse);
    }

    bool drawCubicLines(const GraphicsCubicLine *cubicLine,
                        uint16 size, bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_CUBIC_LINE_A,
        reinterpret_cast<const int32 *>(cubicLine), size,
        sizeof(GraphicsCubicLine) / sizeof(int32),
        waitForResponse);
    }

    bool drawCubicLineF(float32 x1, float32 y1,
                        float32 x2, float32 y2,
                        float32 x3, float32 y3,
                        float32 x4, float32 y4,
                        bool waitForResponse = false)
    {
        GraphicsCubicLineF cubicLine;

        cubicLine.x1 = x1;
        cubicLine.y1 = y1;
        cubicLine.x2 = x2;
        cubicLine.y2 = y2;
        cubicLine.x3 = x3;
        cubicLine.y3 = y3;
        cubicLine.x4 = x4;
        cubicLine.y4 = y4;

        return drawCubicLineF(cubicLine, waitForResponse);
    }

    bool drawCubicLineF(const GraphicsCubicLineF &cubicLine,
                        bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_CUBIC_LINE_F,
        reinterpret_cast<const float32 *>(&cubicLine),
        sizeof(GraphicsCubicLineF) / sizeof(float32),
        waitForResponse);
    }

    bool drawCubicLinesF(const GraphicsCubicLineF *cubicLine,
                         uint16 size, bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_CUBIC_LINE_F_A,
        reinterpret_cast<const float32 *>(cubicLine), size,
        sizeof(GraphicsCubicLineF) / sizeof(float32),
        waitForResponse);
    }

    // Draw Arc ///////////////////////////////////////////////////////////////

    bool drawArc(int32 x1, int32 y1, int32 x2, int32 y2,
                 int32 startAngle, int32 endAngle,
                 bool waitForResponse = false)
    {
        GraphicsArc arc;

        arc.x1 = x1;
        arc.y1 = y1;
        arc.x2 = x2;
        arc.y2 = y2;
        arc.startAngle = startAngle;
        arc.endAngle = endAngle;

        return drawArc(arc, waitForResponse);
    }

    bool drawArc(const GraphicsArc &arc,
                 bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_ARC,
        reinterpret_cast<const int32 *>(&arc),
        sizeof(GraphicsArc) / sizeof(int32),
        waitForResponse);
    }

    bool drawArcs(const GraphicsArc *arc, uint16 size,
                  bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_ARC_A,
        reinterpret_cast<const int32 *>(arc), size,
        sizeof(GraphicsArc) / sizeof(int32),
        waitForResponse);
    }

    bool drawArcF(float32 x1, float32 y1, float32 x2, float32 y2,
                  float32 startAngle, float32 endAngle,
                  bool waitForResponse = false)
    {
        GraphicsArcF arc;

        arc.x1 = x1;
        arc.y1 = y1;
        arc.x2 = x2;
        arc.y2 = y2;
        arc.startAngle = startAngle;
        arc.endAngle = endAngle;

        return drawArcF(arc, waitForResponse);
    }

    bool drawArcF(const GraphicsArcF &arc,
                  bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_ARC_F,
        reinterpret_cast<const float32 *>(&arc),
        sizeof(GraphicsArcF) / sizeof(float32),
        waitForResponse);
    }

    bool drawArcsF(const GraphicsArcF *arc, uint16 size,
                   bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_ARC_F_A,
        reinterpret_cast<const float32 *>(arc), size,
        sizeof(GraphicsArcF) / sizeof(float32),
        waitForResponse);
    }

    // Draw Chord /////////////////////////////////////////////////////////////

    bool drawChord(int32 x1, int32 y1, int32 x2, int32 y2,
                   int32 startAngle, int32 endAngle,
                   bool waitForResponse = false)
    {
        GraphicsChord chord;

        chord.x1 = x1;
        chord.y1 = y1;
        chord.x2 = x2;
        chord.y2 = y2;
        chord.startAngle = startAngle;
        chord.endAngle = endAngle;

        return drawChord(chord, waitForResponse);
    }

    bool drawChord(const GraphicsChord &chord,
                   bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_CHORD,
        reinterpret_cast<const int32 *>(&chord),
        sizeof(GraphicsChord) / sizeof(int32),
        waitForResponse);
    }

    bool drawChords(const GraphicsChord *chord, uint16 size,
                    bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_CHORD_A,
        reinterpret_cast<const int32 *>(chord), size,
        sizeof(GraphicsChord) / sizeof(int32),
        waitForResponse);
    }

    bool drawChordF(float32 x1, float32 y1, float32 x2, float32 y2,
                    float32 startAngle, float32 endAngle,
                    bool waitForResponse = false)
    {
        GraphicsChordF chord;

        chord.x1 = x1;
        chord.y1 = y1;
        chord.x2 = x2;
        chord.y2 = y2;
        chord.startAngle = startAngle;
        chord.endAngle = endAngle;

        return drawChordF(chord, waitForResponse);
    }

    bool drawChordF(const GraphicsChordF &chord,
                    bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_CHORD_F,
        reinterpret_cast<const float32 *>(&chord),
        sizeof(GraphicsChordF) / sizeof(float32),
        waitForResponse);
    }

    bool drawChordsF(const GraphicsChordF *chord, uint16 size,
                     bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_CHORD_F_A,
        reinterpret_cast<const float32 *>(chord), size,
        sizeof(GraphicsChordF) / sizeof(float32),
        waitForResponse);
    }

    // Draw Triangle //////////////////////////////////////////////////////////

    bool drawTriangle(int32 x1, int32 y1,
                      int32 x2, int32 y2,
                      int32 x3, int32 y3,
                      bool waitForResponse = false)
    {
        GraphicsTriangle triangle;

        triangle.x1 = x1;
        triangle.y1 = y1;
        triangle.x2 = x2;
        triangle.y2 = y2;
        triangle.x3 = x3;
        triangle.y3 = y3;

        return drawTriangle(triangle, waitForResponse);
    }

    bool drawTriangle(const GraphicsTriangle &triangle,
                      bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_TRIANGLE,
        reinterpret_cast<const int32 *>(&triangle),
        sizeof(GraphicsTriangle) / sizeof(int32),
        waitForResponse);
    }

    bool drawTriangles(const GraphicsTriangle *triangle,
                       uint16 size, bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_TRIANGLE_A,
        reinterpret_cast<const int32 *>(triangle), size,
        sizeof(GraphicsTriangle) / sizeof(int32),
        waitForResponse);
    }

    bool drawTriangleF(float32 x1, float32 y1,
                       float32 x2, float32 y2,
                       float32 x3, float32 y3,
                       bool waitForResponse = false)
    {
        GraphicsTriangleF triangle;

        triangle.x1 = x1;
        triangle.y1 = y1;
        triangle.x2 = x2;
        triangle.y2 = y2;
        triangle.x3 = x3;
        triangle.y3 = y3;

        return drawTriangleF(triangle, waitForResponse);
    }

    bool drawTriangleF(const GraphicsTriangleF &triangle,
                       bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_TRIANGLE_F,
        reinterpret_cast<const float32 *>(&triangle),
        sizeof(GraphicsTriangleF) / sizeof(float32),
        waitForResponse);
    }

    bool drawTrianglesF(const GraphicsTriangleF *triangle,
                        uint16 size, bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_TRIANGLE_F_A,
        reinterpret_cast<const float32 *>(triangle), size,
        sizeof(GraphicsTriangleF) / sizeof(float32),
        waitForResponse);
    }

    // Draw Quadrilateral /////////////////////////////////////////////////////

    bool drawQuadrilateral(int32 x1, int32 y1,
                           int32 x2, int32 y2,
                           int32 x3, int32 y3,
                           int32 x4, int32 y4,
                           bool waitForResponse = false)
    {
        GraphicsQuadrilateral quadrilateral;

        quadrilateral.x1 = x1;
        quadrilateral.y1 = y1;
        quadrilateral.x2 = x2;
        quadrilateral.y2 = y2;
        quadrilateral.x3 = x3;
        quadrilateral.y3 = y3;
        quadrilateral.x4 = x4;
        quadrilateral.y4 = y4;

        return drawQuadrilateral(quadrilateral, waitForResponse);
    }

    bool drawQuadrilateral(const GraphicsQuadrilateral &quadrilateral,
                           bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_QUADRILATERAL,
        reinterpret_cast<const int32 *>(&quadrilateral),
        sizeof(GraphicsQuadrilateral) / sizeof(int32),
        waitForResponse);
    }

    bool drawQuadrilaterals(const GraphicsQuadrilateral *quadrilateral,
                            uint16 size, bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_QUADRILATERAL_A,
        reinterpret_cast<const int32 *>(quadrilateral), size,
        sizeof(GraphicsQuadrilateral) / sizeof(int32),
        waitForResponse);
    }

    bool drawQuadrilateralF(float32 x1, float32 y1,
                            float32 x2, float32 y2,
                            float32 x3, float32 y3,
                            float32 x4, float32 y4,
                            bool waitForResponse = false)
    {
        GraphicsQuadrilateralF quadrilateral;

        quadrilateral.x1 = x1;
        quadrilateral.y1 = y1;
        quadrilateral.x2 = x2;
        quadrilateral.y2 = y2;
        quadrilateral.x3 = x3;
        quadrilateral.y3 = y3;
        quadrilateral.x4 = x4;
        quadrilateral.y4 = y4;

        return drawQuadrilateralF(quadrilateral, waitForResponse);
    }

    bool drawQuadrilateralF(const GraphicsQuadrilateralF &quadrilateral,
                            bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_QUADRILATERAL_F,
        reinterpret_cast<const float32 *>(&quadrilateral),
        sizeof(GraphicsQuadrilateralF) / sizeof(float32),
        waitForResponse);
    }

    bool drawQuadrilateralsF(const GraphicsQuadrilateralF *quadrilateral,
                             uint16 size, bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_QUADRILATERAL_F_A,
        reinterpret_cast<const float32 *>(quadrilateral), size,
        sizeof(GraphicsQuadrilateralF) / sizeof(float32),
        waitForResponse);
    }

    // Draw Rectangle /////////////////////////////////////////////////////////

    bool drawRectangle(int32 x1, int32 y1, int32 x2, int32 y2,
                       int32 xRadius = 0, int32 yRadius = 0,
                       bool waitForResponse = false)
    {
        GraphicsRectangle rectangle;

        rectangle.x1 = x1;
        rectangle.y1 = y1;
        rectangle.x2 = x2;
        rectangle.y2 = y2;
        rectangle.xRadius = xRadius;
        rectangle.yRadius = yRadius;

        return drawRectangle(rectangle, waitForResponse);
    }

    bool drawRectangle(const GraphicsRectangle &rectangle,
                       bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_RECTANGLE,
        reinterpret_cast<const int32 *>(&rectangle),
        sizeof(GraphicsRectangle) / sizeof(int32),
        waitForResponse);
    }

    bool drawRectangles(const GraphicsRectangle *rectangle, uint16 size,
                        bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_RECTANGLE_A,
        reinterpret_cast<const int32 *>(rectangle), size,
        sizeof(GraphicsRectangle) / sizeof(int32),
        waitForResponse);
    }

    bool drawRectangleF(float32 x1, float32 y1, float32 x2, float32 y2,
                        float32 xRadius = 0, float32 yRadius = 0,
                        bool waitForResponse = false)
    {
        GraphicsRectangleF rectangle;

        rectangle.x1 = x1;
        rectangle.y1 = y1;
        rectangle.x2 = x2;
        rectangle.y2 = y2;
        rectangle.xRadius = xRadius;
        rectangle.yRadius = yRadius;

        return drawRectangleF(rectangle, waitForResponse);
    }

    bool drawRectangleF(const GraphicsRectangleF &rectangle,
                        bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_RECTANGLE_F,
        reinterpret_cast<const float32 *>(&rectangle),
        sizeof(GraphicsRectangleF) / sizeof(float32),
        waitForResponse);
    }

    bool drawRectanglesF(const GraphicsRectangleF *rectangle, uint16 size,
                         bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_RECTANGLE_F_A,
        reinterpret_cast<const float32 *>(rectangle), size,
        sizeof(GraphicsRectangleF) / sizeof(float32),
        waitForResponse);
    }

    // Draw Ellipse ///////////////////////////////////////////////////////////

    bool drawEllipse(int32 x1, int32 y1, int32 x2, int32 y2,
                     int32 startAngle = 0, int32 endAngle = 0,
                     bool waitForResponse = false)
    {
        GraphicsEllipse ellipse;

        ellipse.x1 = x1;
        ellipse.y1 = y1;
        ellipse.x2 = x2;
        ellipse.y2 = y2;
        ellipse.startAngle = startAngle;
        ellipse.endAngle = endAngle;

        return drawEllipse(ellipse, waitForResponse);
    }

    bool drawEllipse(const GraphicsEllipse &ellipse,
                     bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_ELLIPSE,
        reinterpret_cast<const int32 *>(&ellipse),
        sizeof(GraphicsEllipse) / sizeof(int32),
        waitForResponse);
    }

    bool drawEllipses(const GraphicsEllipse *ellipse, uint16 size,
                      bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_ELLIPSE_A,
        reinterpret_cast<const int32 *>(ellipse), size,
        sizeof(GraphicsEllipse) / sizeof(int32),
        waitForResponse);
    }

    bool drawEllipseF(float32 x1, float32 y1, float32 x2, float32 y2,
                      float32 startAngle = 0, float32 endAngle = 0,
                      bool waitForResponse = false)
    {
        GraphicsEllipseF ellipse;

        ellipse.x1 = x1;
        ellipse.y1 = y1;
        ellipse.x2 = x2;
        ellipse.y2 = y2;
        ellipse.startAngle = startAngle;
        ellipse.endAngle = endAngle;

        return drawEllipseF(ellipse, waitForResponse);
    }

    bool drawEllipseF(const GraphicsEllipseF &ellipse,
                      bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_ELLIPSE_F,
        reinterpret_cast<const float32 *>(&ellipse),
        sizeof(GraphicsEllipseF) / sizeof(float32),
        waitForResponse);
    }

    bool drawEllipsesF(const GraphicsEllipseF *ellipse, uint16 size,
                       bool waitForResponse = false)
    {
        return sendPacket(GRAPHICS_DRAW_ELLIPSE_F_A,
        reinterpret_cast<const float32 *>(ellipse), size,
        sizeof(GraphicsEllipseF) / sizeof(float32),
        waitForResponse);
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
            receivePacket(GRAPHICS_GET_BACKGROUND_COLOR,
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

    bool setCoordinateSystem(CoordinateSystem coordinateSystem,
                             bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(GRAPHICS_SET_COORDINATE_SYSTEM,
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
            receivePacket(GRAPHICS_GET_COORDINATE_SYSTEM,
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
        sendPacket(GRAPHICS_SET_ANGLE_UNITS,
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
            receivePacket(GRAPHICS_GET_ANGLE_UNITS,
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
        sendPacket(GRAPHICS_SET_LINE_COLOR,
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
            receivePacket(GRAPHICS_GET_LINE_COLOR,
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
        sendPacket(GRAPHICS_SET_LINE_STYLE,
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
            receivePacket(GRAPHICS_GET_LINE_STYLE,
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
        sendPacket(GRAPHICS_SET_FILL_COLOR,
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
            receivePacket(GRAPHICS_GET_FILL_COLOR,
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
        sendPacket(GRAPHICS_SET_FILL_STYLE,
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
            receivePacket(GRAPHICS_GET_FILL_STYLE,
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

    bool setRotation(int32 rotation,
                     bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(GRAPHICS_SET_ROTATION,
        int32(rotation), waitForResponse)))
        {
            m_rotation = rotation;
        }

        return result;
    }

    int32 getRotation(bool bypassCache = false,
                      bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(GRAPHICS_GET_ROTATION,
            &response)))
            {
               m_rotation = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_rotation : int32();
    }

    bool setRotationF(float32 rotationF,
                      bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(GRAPHICS_SET_ROTATION_F,
        float32(rotationF), waitForResponse)))
        {
            m_rotationF = rotationF;
        }

        return result;
    }

    float32 getRotationF(bool bypassCache = false,
                         bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(GRAPHICS_GET_ROTATION_F,
            &response)))
            {
               m_rotationF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_rotationF : float32();
    }

//    bool setRotationF(int32 rotationF,
//                      bool waitForResponse = false)
//    {
//        bool result = setRotation(rotationF, waitForResponse);

//        if(result)
//        {
//            m_rotationF = m_rotation;
//        }

//        return result;
//    }

//    bool setRotation(float32 rotation,
//                     bool waitForResponse = false)
//    {
//        bool result = setRotationF(rotation, waitForResponse);

//        if(result)
//        {
//            m_rotation = m_rotationF;
//        }

//        return result;
//    }

    bool setScale(int32 scale,
                  bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(GRAPHICS_SET_SCALE,
        int32(scale), waitForResponse)))
        {
            m_scale = scale;
        }

        return result;
    }

    int32 getScale(bool bypassCache = false,
                   bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            int32 response;

            if((result =
            receivePacket(GRAPHICS_GET_SCALE,
            &response)))
            {
               m_scale = int32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_scale : int32();
    }

    bool setScaleF(float32 scaleF,
                   bool waitForResponse = false)
    {
        bool result;

        if((result =
        sendPacket(GRAPHICS_SET_SCALE_F,
        float32(scaleF), waitForResponse)))
        {
            m_scaleF = scaleF;
        }

        return result;
    }

    float32 getScaleF(bool bypassCache = false,
                      bool *ok = 0)
    {
        bool result = m_enabled;

        if(result && bypassCache)
        {
            float32 response;

            if((result =
            receivePacket(GRAPHICS_GET_SCALE_F,
            &response)))
            {
               m_scaleF = float32(response);
            }
        }

        if(ok)
        {
            *ok = result;
        }

        return result ? m_scaleF : float32();
    }

//    bool setScaleF(int32 scaleF,
//                   bool waitForResponse = false)
//    {
//        bool result = setScale(scaleF, waitForResponse);

//        if(result)
//        {
//            m_scaleF = m_scale;
//        }

//        return result;
//    }

//    bool setScale(float32 scale,
//                  bool waitForResponse = false)
//    {
//        bool result = setScaleF(scale, waitForResponse);

//        if(result)
//        {
//            m_scale = m_scaleF;
//        }

//        return result;
//    }

private:

    ILGraphics(const ILGraphics &);
    ILGraphics &operator=(const ILGraphics &);

    argb m_backgroundColor;

    CoordinateSystem m_coordinateSystem; AngleUnits m_angleUnits;

    argb m_lineColor; LineStyle m_lineStyle;
    argb m_fillColor; FillStyle m_fillStyle;

    int32 m_rotation; float32 m_rotationF;
    int32 m_scale; float32 m_scaleF;
};

#endif // ILGRAPHICS_H

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
