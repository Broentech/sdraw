//h+//////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2016 Broentech Solutions AS
// Contact: https://broentech.no/#!/contact
//
//
// GNU Lesser General Public License Usage
// This file may be used under the terms of the GNU Lesser
// General Public License version 3 as published by the Free Software
// Foundation and appearing in the file LICENSE.LGPL3 included in the
// packaging of this file. Please review the following information to
// ensure the GNU Lesser General Public License version 3 requirements
// will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
//
//
//h+//////////////////////////////////////////////////////////////////////////
//
//  \file      draw_2D.h
//
//  \brief     Declaration of the draw board drawing functionality
//
//  \author    Stian Broen
//
//  \date      26.09.2011
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//             02 29.03.2012 - Reorganizing code
//
//  \par       01 26.09.2011 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

#ifndef TWOD_DRAWBOARD_DRAW_H
#define TWOD_DRAWBOARD_DRAW_H

// Qt includes
#include <QColor>
#include <QPoint>
#include <QPainter>
#include <QObject>

// solution includes
#include <common/definitions.h>
#include <sdrawWidgets/shapeSelector.h>
#include <sdrawWidgets/settingsMenu.h>
#include <algorithms/pixelAlgorithms.h>
#include <drawCommands/drawboard_pixmapSegment.h>
#include <zooming/zoomer.h>

// local includes
#include "draw_Base.h"

// Qt forward declarations
class QEvent;
class QMouseEvent;
class QWheelEvent;
class QPaintEvent;

namespace sdraw {

   enum LastMouseEventType {
      _NONE,
      _PRESS,
      _MOVE,
      _RELEASE,
      _DOUBLECLICK
   };

   class CDraw2D : public CDrawBase
   {
      Q_OBJECT

   public:
      CDraw2D(QObject* parent = 0);
      ~CDraw2D();
      void drawPressEvent(QMouseEvent*);
      void drawPress(const QPoint &p);
      void drawMoveEvent(QMouseEvent*);
      void drawMove(const QPoint &p);
      void drawDoubleclickEvent(QMouseEvent*);
      void drawReleaseEvent(QMouseEvent*);
      void drawRelease(const QPoint &p);
      void fillerPressEvent(QMouseEvent*);
      void fillerMoveEvent(QMouseEvent*);
      void fillerDoubleclickEvent(QMouseEvent*);
      void fillerReleaseEvent(QMouseEvent*);
      virtual void updateOperation(TOperation operation);
      //void updateShape(shapeType shape);
      void updateAngle(qreal val);
      //void updateRoundedRect_X(int radius);
      //void updateRoundedRect_Y(int radius);
      void doEnterInterpSelection();

   signals:
      void appendPixmapSegment(C2DPixmapSegment *segment);
      void adjustCanvas();

   protected:
      void drawRectangle(QPoint bottomRight);
      void drawRoundedRectangle(QPoint bottomRight);
      void drawEllipse(QPoint bottomRight);
      void drawPoint(QPoint pt);
      void drawLine(QPoint pt);
      void drawArc();
      void drawPie();
      void drawChord();
      void drawPolyLine();
      void drawPolygon();
      int   isClockwise(Quadrant alpha, Quadrant beta, bool *clockW);
      bool  arrangeAngles(QPoint pt);
      int   calcVectorLength(QPoint pt, QPoint endPt);
      qreal calcAngleBetweenLines(QPoint pt, QPoint endPt, Quadrant *quadrantBuff);

   private:
      void drawStraightAidLine(QPoint pt);
      bool checkGradientPoints();
      C2DPixmapSegment* doFill(QPoint pt);
      C2DPixmapSegment* createSubpixmapFromPolygon(QPolygon &polygon, EPolygonConversion conv = NO_CONVERSION);

   private:
      PixelAlgo *algorithm;
      QPolygon noShapePolygon;
      bool m_first;
      LastMouseEventType m_lastMouseEvent;
   };

   QPolygon normalizePolygon(const QPolygon &polygon, int offset = 0);
}

#endif