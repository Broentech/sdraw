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
//  \file      draw_2D.cpp
//
//  \brief     Implementation of the draw functionality of the drawboard
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
//             02 01.04.2012 - Introduced the concept of "parent targets". This class
//                             no longer has any internal pixmap targets to write content to, instead
//                             targets must be registered before writing, and released when writing is done
//
//  \par       01 26.09.2011 Original version
//
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QImage>
#include <QFile>
#include <QBitmap>
#include <math.h>
#include <QtDebug>
#include <QtMath>

// solution includes
#include <_debugStuff/imageDialog.h>
#include <drawSettings/DrawSettings_2D.h>

// local includes
#include "draw_2D.h"

using namespace sdraw;

CDraw2D::CDraw2D(QObject* parent)
   : CDrawBase(parent),
   algorithm(new PixelAlgo(this)),
   m_first(true),
   m_lastMouseEvent(_NONE)
{
   
}

CDraw2D::~CDraw2D()
{
   delete algorithm;
   algorithm = 0;
}

void CDraw2D::drawRectangle(QPoint bottomRight)
{
   if(!bufferRegistered)
   {
      return;
   }

   QPainter painter(m_buffer);
   painter.setPen(CDrawSettings_2D::getInstance()->penSettings().getPen());
   painter.setRenderHint(QPainter::Antialiasing, _settings.getAntialiasing());
   painter.drawRect( QRect(_last, bottomRight) );

   emit requestUpdate();
}

void CDraw2D::drawEllipse(QPoint bottomRight)
{
   if(!bufferRegistered)
   {
      return;
   }

   QPainter painter(m_buffer);
   painter.setPen(CDrawSettings_2D::getInstance()->penSettings().getPen());
   painter.setRenderHint(QPainter::Antialiasing,_settings.getAntialiasing());
   painter.drawEllipse( QRect(_last, bottomRight) );

   emit requestUpdate();
}

void CDraw2D::drawRoundedRectangle(QPoint bottomRight)
{
   if (!bufferRegistered)
   {
      return;
   }

   QPoint tLeft;
   QPoint bRight;

   if (_last.x() <= bottomRight.x() && _last.y() <= bottomRight.y())
   {
      tLeft = _last;
      bRight = bottomRight;
   }
   else if (_last.x() > bottomRight.x() && _last.y() < bottomRight.y())
   {
      tLeft.setX(bottomRight.x());
      tLeft.setY(_last.y());
      bRight.setX(_last.x());
      bRight.setY(bottomRight.y());
   }
   else if (_last.x() > bottomRight.x() && _last.y() > bottomRight.y())
   {
      tLeft = bottomRight;
      bRight = _last;
   }
   else if (_last.x() < bottomRight.x() && _last.y() > bottomRight.y())
   {
      tLeft.setX(_last.x());
      tLeft.setY(bottomRight.y());
      bRight.setX(bottomRight.x());
      bRight.setY(_last.y());
   }

   QPainter painter(m_buffer);
   painter.setPen(CDrawSettings_2D::getInstance()->penSettings().getPen());
   painter.setRenderHint(QPainter::Antialiasing, _settings.getAntialiasing());
   painter.drawRoundedRect(
      QRect(tLeft, bRight),
      CDrawSettings_2D::getInstance()->getRoundedRectRadius_X(),
      CDrawSettings_2D::getInstance()->getRoundedRectRadius_Y()
      );

   emit requestUpdate();
}

void CDraw2D::drawPoint(QPoint pt)
{
   if(!bufferRegistered)
   {
      return;
   }

   QPoint drawPoint = pt;
   QPainter painter(m_buffer);
   painter.setPen(CDrawSettings_2D::getInstance()->penSettings().getPen());
   painter.setRenderHint(QPainter::Antialiasing,_settings.getAntialiasing());
   painter.drawPoint(drawPoint);
   painter.end();

   emit requestUpdate();
}

void CDraw2D::drawLine(QPoint pt)
{
   if(!bufferRegistered)
   {
      return;
   }

   QPainter painter(m_buffer);
   painter.setPen(CDrawSettings_2D::getInstance()->penSettings().getPen());
   painter.setRenderHint(QPainter::Antialiasing,_settings.getAntialiasing());
   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
   painter.drawLine( _last, pt );
   painter.end();

   emit requestUpdate();
}

void CDraw2D::drawArc()
{
   if(!bufferRegistered)
   {
      return;
   }

   QPainter painter(m_buffer);
   painter.setPen(CDrawSettings_2D::getInstance()->penSettings().getPen());
   painter.setRenderHint(QPainter::Antialiasing,_settings.getAntialiasing());
   painter.drawArc( QRect(topLeftForCircular, bottomRightForCircular), alphaDegrees*16, betaDegrees*16 );

   emit requestUpdate();
}

void CDraw2D::drawPie()
{
   if(!bufferRegistered)
   {
      return;
   }

   QPainter painter(m_buffer);
   painter.setRenderHint(QPainter::Antialiasing,_settings.getAntialiasing());

   painter.setPen(CDrawSettings_2D::getInstance()->penSettings().getPen());
   painter.drawPie( QRectF((QPointF)topLeftForCircular, 
                           (QPointF)bottomRightForCircular), 
                            alphaDegrees*16, betaDegrees*16 );

   emit requestUpdate();
}

void CDraw2D::drawChord()
{
   if(!bufferRegistered)
   {
      return;
   }

   QPainter painter(m_buffer);
   painter.setPen(CDrawSettings_2D::getInstance()->penSettings().getPen());
   painter.setRenderHint(QPainter::Antialiasing,_settings.getAntialiasing());
   painter.drawChord( QRectF(topLeftForCircular, bottomRightForCircular), alphaDegrees*16, betaDegrees*16 );

   emit requestUpdate();
}

void CDraw2D::drawPolyLine()
{
   if(!bufferRegistered)
   {
      return;
   }

   QPainter painter(m_buffer);
   painter.setPen(CDrawSettings_2D::getInstance()->penSettings().getPen());
   painter.setRenderHint(QPainter::Antialiasing,_settings.getAntialiasing());
   painter.drawPolyline(polyLine);

   emit requestUpdate();
}

void CDraw2D::drawPolygon()
{
   if(!bufferRegistered)
   {
      return;
   }

   QPainter painter(m_buffer);
   painter.setPen(CDrawSettings_2D::getInstance()->penSettings().getPen());
   painter.setRenderHint(QPainter::Antialiasing,_settings.getAntialiasing());
   painter.drawPolygon(polygon);

   emit requestUpdate();
}

//void CDraw2D::updateShape(shapeType shape)
//{
//   currShape = shape;
//}

void CDraw2D::updateAngle(qreal val)
{
   _angle = val;
}

//void CDraw2D::updateRoundedRect_X(int radius)
//{
//   roundedRectRadius_X = radius;
//}
//
//void CDraw2D::updateRoundedRect_Y(int radius)
//{
//   roundedRectRadius_Y = radius;
//}

void CDraw2D::updateOperation(TOperation operation)
{
   //C2D_DrawBoard_Base::updateOperation(operation);
   mOperation = operation;
   switch(mOperation)
   {
      case(DRAW):
      {
         emit activateMouseMovement(false);
         //activeMouse = false;
         aidOn = false;
         break;
      }
      case(POLYGONIZER):
      {
         break;
      }
      case(FILLER):
      {
         break;
      }
      case(SELECTER):
      {
         break;
      }
      case(TEXT):
      {
         break;
      }
      case(INSPECTER):
      {
         break;
      }
      default:
      {
         break;
      }
   }
}

bool CDraw2D::arrangeAngles(QPoint pt)
{
   Quadrant quadr;
   qreal tempAng = calcAngleBetweenLines(origo,pt, &quadr);

   // get current difference in degrees, and compare with the last calculated diff
   qreal tmpAngleDiff = alphaDegrees - tempAng;
   if(_lastAngleDiff != 0)
   {
      if( ((_lastAngleDiff < 0 && tmpAngleDiff > 0)  || 
           (_lastAngleDiff > 0 && tmpAngleDiff < 0)) && (quadr == alphaQuadrant))
      {
         // the current degree diff is different than the last one, meaning it has taken
         // a direction around the clock, different than the direction it started, the clock must be reset
         // but there are some exceptions :
         if(!(alphaQuadrant == fourth && clockCount == -3) &&
            !(alphaQuadrant == fourth && clockCount ==  1) &&
            !(alphaQuadrant == first  && clockCount == -1) &&
            !(alphaQuadrant == first  && clockCount ==  3)  )
         {
            // reset the clock counter
            clockCount = 0;
         }
      }
   }
   _lastAngleDiff = tmpAngleDiff;

   betaDegrees = (tempAng - alphaDegrees);
   bool clockwiseDirection;

   // calculate the clock-direction, based on quadrant comparisen
   if(0 < isClockwise(lastQuadrant, quadr, &clockwiseDirection))
   {
      // reaching here means that a new direction was found
      if(quadr != lastQuadrant)
      {
         // the cursor is not inside the quadrant where it started
         lastQuadrant = quadr;
         if(clockwiseDirection)
         {
            // direction is with the clock
            clockCount--;
            if(clockCount == -5)
            {
               // the cursor has moved on full 360 degree rotation, stop counting and return false
               clockCount++;
               return false;
            }
         }
         else
         {
            // direction is against the clock
            clockCount++;
            if(clockCount == 5)
            {
               // the cursor has moved on full 360 degree rotation, stop counting and return false
               clockCount--;
               return false;
            }
         }
      }
   }

   if(alphaQuadrant == first)
   {
      // cursor start is in the first quadrant TODO
      if(clockCount < 0)
      {
         betaDegrees = -(360.0 - betaDegrees);
      }
      else if(clockCount == 4)
      {
         betaDegrees = 360.0 + betaDegrees;
      }
   }
   
   else if(alphaQuadrant == second)
   {
      // cursor start is in the second quadrant TODO
      if(clockCount <= -2)
      {
         betaDegrees = -(alphaDegrees + (360.0 - tempAng));
      }
      else if(clockCount >= 3)
      {
         betaDegrees = (360.0 - alphaDegrees) + tempAng;
      }
   }

   else if(alphaQuadrant == third)
   {
      // cursor start is in the third quadrant
      if(clockCount >= 2)
      {
         betaDegrees = tempAng + (360 - alphaDegrees);
      }
      else if(clockCount < -2)
      {
         betaDegrees = -(360.0 - betaDegrees);
      }
   }

   else if(alphaQuadrant == fourth)
   {
      // cursor start is in the fourth quadrant
      if(clockCount > 0)
      {
         betaDegrees = tempAng + (360 - alphaDegrees);
      }
      else if(clockCount <= 0)
      {
         if(alphaDegrees > tempAng)
         {
            betaDegrees = -(alphaDegrees - tempAng);
         }
         else if(clockCount != 0)
         {
            betaDegrees =   -(alphaDegrees + (360 - tempAng));
         }
      }
   }

   // return successfull degree calculation
   return true;
}

int CDraw2D::isClockwise(Quadrant alpha, Quadrant beta, bool *clockW)
{
   if(beta == alpha)
   {
      // not able to determine direction
      return -1;
   }

   if(alpha == first)
   {
      if(beta == second)
      {
         *clockW = false;
         return 1;
      }
      else if(beta == fourth)
      {
         *clockW = true;
         return 1;
      }
      else
      {
         return -1;
      }
   }
   else if(alpha == second)
   {
      if(beta == third)
      {
         *clockW = false;
         return 1;
      }
      else if(beta == first)
      {
         *clockW = true;
         return 1;
      }
      else
      {
         return -1;
      }
   }
   else if(alpha == third)
   {
      if(beta == fourth)
      {
         *clockW = false;
         return 1;
      }
      else if(beta == second)
      {
         *clockW = true;
         return 1;
      }
      else
      {
         return -1;
      }
   }
   else if(alpha == fourth)
   {
      if(beta == first)
      {
         *clockW = false;
         return 1;
      }
      else if(beta == third)
      {
         *clockW = true;
         return 1;
      }
      else
      {
         return -1;
      }
   }
   else
   {
      return -1;
   }
}

int CDraw2D::calcVectorLength(QPoint pt, QPoint endPt)
{
   QPoint currPt = pt;
   int currX = currPt.x();
   int currY = currPt.y();

   // move to origo
   currPt.setX(currX - endPt.x());
   currPt.setY(currY - endPt.y());

   double lengthCurr = sqrt(pow((double)currPt  .x(), 2.0) + pow((double)currPt  .y(), 2.0));
   double tmpFloor = floor(lengthCurr);
   if((lengthCurr - tmpFloor) < 0.5)
      return (int)tmpFloor;
   else
      return (int)ceil(lengthCurr);
}

//  Calculates angle. The first point defines the origo of a circle and the second
//  point is on a horizontal line between origo and the point. The angle calculated is 
//  the angle between the horizontal line going from origo and to the right, and the 
//  line from origo to the second point
qreal CDraw2D::calcAngleBetweenLines(QPoint pt, QPoint endPt, Quadrant *quadrantBuff)
{
   QPoint currPt = pt;
   int currX = endPt.x();
   int currY = endPt.y();
   int centralX  = currPt.x();
   int centralY  = currPt.y();
   double angle  = 0.0;

   if (currX > centralX && currY == centralY)
   {
      return 360.0;
   }
   else if (currX < centralX && currY == centralY)
   {
      return 180.0;
   }
   else if (currX == centralX && currY < centralY)
   {
      return 90.0;
   }
   else if (currX == centralX && currY > centralY)
   {
      return 270.0;
   }

   // move to origo, the line from origo to pt is hypothenus
   currPt.setX(currX - centralX);
   currPt.setY(currY - centralY);
   
   // get adjacent line
   QPoint adjacent(currPt.x(), 0);

   // calculate hypothenus * adjacent (vector scalar)
   int scalar     = currPt.x() * adjacent.x() + currPt.y() * adjacent.y();

   // calculate |hypothenus| and |adjacent|
   double lengthCurr = sqrt(pow((double)currPt  .x(), 2.0) + pow((double)currPt  .y(), 2.0));
   double lengthAdj  = sqrt(pow((double)adjacent.x(), 2.0) + pow((double)adjacent.y(), 2.0));
   
   // calculate (hypothenus * adjacent) / (|hypothenus| * |adjacent|)
   double res = (double)scalar / (lengthCurr * lengthAdj);

   // return cos^-1(res) converted from radians to degrees
   angle = acos(res) * 180.0 / PI;

   if(currX > centralX && currY < centralY)
   {
      *quadrantBuff = first;
      return (qreal)angle;
   }
   else if(currX < centralX && currY < centralY)
   {
      *quadrantBuff = second;
      return (qreal)(180.0 - angle);
   }
   else if(currX < centralX && currY > centralY)
   {
      *quadrantBuff = third;
      return (qreal)(180.0 + angle);
   }
   else if(currX > centralX && currY > centralY)
   {
      *quadrantBuff = fourth;
      return (qreal)(360 - angle);
   }
   else
   {
      return - 1;
   }
}

void CDraw2D::drawPressEvent(QMouseEvent* event)
{
   if (m_lastMouseEvent == _PRESS)
   {
      return; 
   }
   m_lastMouseEvent = _PRESS;

   if(event->button() == Qt::LeftButton)
   {
      mouseButtonBuff = Qt::LeftButton;
   }
   else if(event->button() == Qt::RightButton)
   {
      mouseButtonBuff = Qt::RightButton;
   }

   QPoint eventPt = event->pos();

   

   // this class does not perform sdraw, but if sdraw has been performed on the target pixmap
   // we need to use the registered zoomer object to retrieve the correct point
   if(hasRegisteredZoomer() && m_zoomer->hasSource())
   {
      _last = m_zoomer->getBufferPoint(eventPt, true, true);
   }
   else
   {
      _last = eventPt;
   }

   if (!m_buffer->rect().contains(_last))
   {
      return;
   }

   drawPress(_last);
}

void CDraw2D::drawPress(const QPoint &p)
{
   switch (CDrawSettings_2D::getInstance()->getShape())
   {
   case(SNoShape) :
   {
      noShapePolygon.clear();
      noShapePolygon << p;
      emit activateMouseMovement(true);
      //activeMouse = true;
      aidOn = true;
      break;
   }
   case(SLine) :
   {
      emit activateMouseMovement(true);
      //activeMouse = true;
      aidOn = true;
      break;
   }
   case(SRectangle) :
   {
      emit activateMouseMovement(true);
      //activeMouse = true;
      aidOn = true;
      break;
   }
   case(SRoundedRectangle) :
   {
      emit activateMouseMovement(true);
      //activeMouse = true;
      aidOn = true;
      break;
   }
   case(SEllipse) :
   {
      emit activateMouseMovement(true);
      //activeMouse = true;
      aidOn = true;
      break;
   }
   case(SArc) :
   {
      emit activateMouseMovement(true);
      //activeMouse = true;
      if (radius == 0)
      {
         origo = p;
         aidOn = true;
      }
      break;
   }
   case(SPie) :
   {
      emit activateMouseMovement(true);
      //activeMouse = true;
      if (radius == 0)
      {
         origo = p;
         aidOn = true;
      }
      break;
   }
   case(SChord) :
   {
      emit activateMouseMovement(true);
      //activeMouse = true;
      if (radius == 0)
      {
         origo = p;
         aidOn = true;
      }
      break;
   }
   case(SPolyline) :
   {
      if (polyLine.size() == 0)
      {
         emit activateMouseMovement(true);
         //activeMouse = true;
      }
      polyLine << p;
      aidOn = true;
      break;
   }
   case(SPolygon) :
   {
      if (polygon.size() == 0)
      {
         emit activateMouseMovement(true);
         //activeMouse = true;
      }
      polygon << p;
      aidOn = true;
      break;
   }
   default:
   {
      break;
   }
   }
}

void CDraw2D::drawMoveEvent(QMouseEvent* event)
{
   if (m_lastMouseEvent == _RELEASE)
   {
      return; 
   }
   m_lastMouseEvent = _MOVE;

   if (event->button() == Qt::LeftButton)
   {
      mouseButtonBuff = Qt::LeftButton;
   }
   else if (event->button() == Qt::RightButton)
   {
      mouseButtonBuff = Qt::RightButton;
   }

   QPoint drawPoint;
   QPoint eventPt = event->pos();

   // this class does not perform sdraw, but if sdraw has been performed on the target pixmap
   // we need to use the registered zoomer object to retrieve the correct point
   if(hasRegisteredZoomer() && m_zoomer->hasSource())
   {
      drawPoint = m_zoomer->getBufferPoint(eventPt, true, true);
   }
   else
   {
      drawPoint = eventPt;
   }

   QRect bufrect = m_buffer->rect();
   if (!bufrect.contains(drawPoint))
   {
      if (drawPoint.y() > bufrect.height())
      {
         drawPoint.setY(bufrect.height());
      }
      if (drawPoint.y() < 0)
      {
         drawPoint.setY(0);
      }
      if (drawPoint.x() > bufrect.width())
      {
         drawPoint.setX(bufrect.width());
      }
      if (drawPoint.x() < 0)
      {
         drawPoint.setX(0);
      }
      m_lastMouseEvent = _RELEASE;
      return drawRelease(drawPoint);
   }

   drawMove(drawPoint);
}

void CDraw2D::drawMove(const QPoint &p)
{
   shapeType _shape = CDrawSettings_2D::getInstance()->getShape();
   switch (_shape)
   {
   case(SNoShape) :
   {
      noShapePolygon << p;
      QPen pen = CDrawSettings_2D::getInstance()->penSettings().getPen();
      drawAidLine(_last, p, &pen);
      break;
   }
   case(SLine) :
   {
      blankAidBuffer();
      drawAidLine(_last, p);
      break;
   }
   case(SRectangle) :
   {
      blankAidBuffer();
      drawAidRectangle(p);
      break;
   }
   case(SRoundedRectangle) :
   {
      blankAidBuffer();
      drawAidRoundedRectangle(p);
      break;
   }
   case(SEllipse) :
   {
      blankAidBuffer();
      drawAidEllipse(p);
      break;
   }
   case(SArc) :
   {
      blankAidBuffer();
      if (radius == 0)
      {
         drawAidLine(origo, p);
      }
      else
      {
         if (arrangeAngles(p))
         {
            drawAidArc(p);
         }
      }
      break;
   }
   case(SPie) :
   {
      blankAidBuffer();
      if (radius == 0)
      {
         drawAidLine(origo, p);
      }
      else
      {
         if (arrangeAngles(p))
         {
            drawAidPie(p);
         }
      }
      break;
   }
   case(SChord) :
   {
      blankAidBuffer();
      if (radius == 0)
      {
         blankAidBuffer();
         drawAidLine(origo, p);
      }
      else
      {
         if (arrangeAngles(p))
         {
            drawAidChord(p);
         }
      }
      break;
   }
   case(SPolyline) :
   {
      blankAidBuffer();
      if (polyLine.size() > 0)
      {
         drawAidLine(polyLine.at(polyLine.size() - 1), p);
      }
      if (polyLine.size() >= 2)
      {
         drawAidPolyline(polyLine);
      }
      break;
   }
   case(SPolygon) :
   {
      blankAidBuffer();
      if (polygon.size() == 1)
      {
         drawAidLine(polygon.at(polygon.size() - 1), p);
      }
      else if (polygon.size() > 1)
      {
         polygon << p;
         drawAidPolygon(polygon);
         polygon.pop_back();
      }
      break;
   }
   default:
   {
      break;
   }
   }

   if (
      _shape != SLine &&
      _shape != SRectangle &&
      _shape != SRoundedRectangle &&
      _shape != SEllipse
      )
   {
      // remember the current mouse position
      _last = p;
   }
}

void CDraw2D::drawDoubleclickEvent(QMouseEvent* event)
{
   m_lastMouseEvent = _DOUBLECLICK;
   if(event->button() == Qt::LeftButton)
   {
      mouseButtonBuff = Qt::LeftButton;
   }
   else if(event->button() == Qt::RightButton)
   {
      mouseButtonBuff = Qt::RightButton;
   }

   switch (CDrawSettings_2D::getInstance()->getShape())
   {
      case(SNoShape):
      {
         break;
      }
      case(SLine):
      {
         break;
      }
      case(SRectangle):
      {
         break;
      }
      case(SRoundedRectangle):
      {
         break;
      }
      case(SEllipse):
      {
         break;
      }
      case(SArc):
      {
         break;
      }
      case(SPie):
      {
         break;
      }
      case(SChord):
      {
         break;
      }
      case(SPolyline):
      {
         blankAidBuffer();
         aidOn = false;
         emit activateMouseMovement(false);
         //activeMouse = false;
         if(polyLine.size() > 1)
         {
            emit appendPixmapSegment(createSubpixmapFromPolygon(polyLine));
         }
         drawPolyLine();
         polyLine.clear();
         break;
      }
      case(SPolygon):
      {
         blankAidBuffer();
         aidOn = false;
         emit activateMouseMovement(false);
         //activeMouse = false;
         if(polygon.size() > 1)
         {
            polygon << polygon.at(0);
            emit appendPixmapSegment(createSubpixmapFromPolygon(polygon));
         }
         drawPolygon();
         polygon.clear();
         break;
      }
      default:
      {
         break;
      }
   }
}

void CDraw2D::drawReleaseEvent(QMouseEvent* event)
{
   if (m_lastMouseEvent == _RELEASE)
   {
      return;
   }
   m_lastMouseEvent = _RELEASE;
   QPoint releasePoint;
   QPoint eventPt = event->pos();

   // this class does not perform sdraw, but if sdraw has been performed on the target pixmap
   // we need to use the registered zoomer object to retrieve the correct point
   if(hasRegisteredZoomer() && m_zoomer->hasSource())
   {
      releasePoint = m_zoomer->getBufferPoint(eventPt, true, true);
   }
   else
   {
      releasePoint = eventPt;
   }

   if (!m_buffer->rect().contains(releasePoint))
   {
      return;
   }

   if(event->button() == Qt::LeftButton)
   {
      mouseButtonBuff = Qt::LeftButton;
   }
   else if(event->button() == Qt::RightButton)
   {
      mouseButtonBuff = Qt::RightButton;
   }

   drawRelease(releasePoint);
}

void CDraw2D::drawRelease(const QPoint &p)
{
   switch (CDrawSettings_2D::getInstance()->getShape())
   {
   case(SNoShape) :
   {
      emit activateMouseMovement(false);
      //activeMouse = false;
      noShapePolygon << p;
      //if(releasePoint == _last)
      //{
      //   drawPoint(tmpClr, releasePoint);
      //}
      C2DPixmapSegment *segment = createSubpixmapFromPolygon(noShapePolygon, TO_LINES);
      emit appendPixmapSegment(segment);
      polyLine = noShapePolygon;
      //drawPolyLine();
      polyLine.clear();
      blankAidBuffer();
      break;
   }
   case(SLine) :
   {
      emit activateMouseMovement(false);
      //activeMouse = false;
      aidOn = false;
      blankAidBuffer();
      QPolygon linePoly;
      linePoly << _last << p;
      emit appendPixmapSegment(createSubpixmapFromPolygon(linePoly, TO_LINES));
      //drawLine(p);
      break;
   }
   case(SRectangle) :
   {
      emit activateMouseMovement(false);
      //activeMouse = false;
      aidOn = false;
      blankAidBuffer();
      QPolygon rectPoly;
      QRect drawRect;
      drawRect.setTopLeft(_last);
      drawRect.setBottomRight(p);
      rectPoly << drawRect.topLeft()
         << drawRect.topRight()
         << drawRect.bottomRight()
         << drawRect.bottomLeft()
         << drawRect.topLeft();

      emit appendPixmapSegment(createSubpixmapFromPolygon(rectPoly, TO_RECT)); // draw the polygon out as a rectangle
      //drawRectangle(p);
      break;
   }
   case(SRoundedRectangle) :
   {
      emit activateMouseMovement(false);
      //activeMouse = false;
      aidOn = false;
      blankAidBuffer();
      QPolygon rectPoly;
      QRect drawRect;
      drawRect.setTopLeft(_last);
      drawRect.setBottomRight(p);
      rectPoly << drawRect.topLeft()
         << drawRect.topRight()
         << drawRect.bottomRight()
         << drawRect.bottomLeft()
         << drawRect.topLeft();

      emit appendPixmapSegment(createSubpixmapFromPolygon(rectPoly, TO_ROUNDED_RECT));
      //drawRoundedRectangle(p);
      break;
   }
   case(SEllipse) :
   {
      emit activateMouseMovement(false);
      //activeMouse = false;
      aidOn = false;
      blankAidBuffer();
      QPolygon ellipsePoly;
      ellipsePoly << _last << p;
      emit appendPixmapSegment(createSubpixmapFromPolygon(ellipsePoly, TO_ELLIPSE));
      //drawEllipse(p);
      break;
   }
   case(SArc) :
   {
      if (radius == 0)
      {
         Quadrant quadr;
         radius = calcVectorLength(origo, p);
         alphaDegrees = calcAngleBetweenLines(origo, p, &quadr);
         alphaQuadrant = quadr;
         lastQuadrant = quadr;
         topLeftForCircular = QPoint(origo.x() - radius, origo.y() - radius);
         bottomRightForCircular = QPoint(origo.x() + radius, origo.y() + radius);

         emit activateMouseMovement(true);
         //activeMouse = true;
      }
      else
      {
         emit activateMouseMovement(false);
         //activeMouse = false;
         blankAidBuffer();
         QPolygon arcPoly;
         arcPoly << topLeftForCircular << bottomRightForCircular;
         emit appendPixmapSegment(createSubpixmapFromPolygon(arcPoly, TO_ARC));
         //drawArc();
         radius = 0;
         clockCount = 0;
         aidOn = false;
      }
      break;
   }
   case(SPie) :
   {
      if (radius == 0)
      {
         Quadrant quadr;
         radius = calcVectorLength(origo, p);
         alphaDegrees = calcAngleBetweenLines(origo, p, &quadr);
         alphaQuadrant = quadr;
         lastQuadrant = quadr;
         topLeftForCircular = QPoint(origo.x() - radius, origo.y() - radius);
         bottomRightForCircular = QPoint(origo.x() + radius, origo.y() + radius);

         emit activateMouseMovement(true);
         //activeMouse = true;
      }
      else
      {
         emit activateMouseMovement(false);
         //activeMouse = false;
         blankAidBuffer();
         QPolygon piePoly;
         piePoly << topLeftForCircular << bottomRightForCircular;
         emit appendPixmapSegment(createSubpixmapFromPolygon(piePoly, TO_PIE));
         //drawPie();
         radius = 0;
         clockCount = 0;
         aidOn = false;
      }
      break;
   }
   case(SChord) :
   {
      if (radius == 0)
      {
         Quadrant quadr;
         radius = calcVectorLength(origo, p);
         alphaDegrees = calcAngleBetweenLines(origo, p, &quadr);
         alphaQuadrant = quadr;
         lastQuadrant = quadr;
         topLeftForCircular = QPoint(origo.x() - radius, origo.y() - radius);
         bottomRightForCircular = QPoint(origo.x() + radius, origo.y() + radius);
         emit activateMouseMovement(true);
         //activeMouse = true;
      }
      else
      {
         emit activateMouseMovement(false);
         //activeMouse = false;
         blankAidBuffer();
         QPolygon chordPoly;
         chordPoly << topLeftForCircular << bottomRightForCircular;
         emit appendPixmapSegment(createSubpixmapFromPolygon(chordPoly, TO_CHORD));
         //drawChord();
         clockCount = 0;
         radius = 0;
         aidOn = false;
      }
      break;
   }
   case(SPolyline) :
   {
      break;
   }
   case(SPolygon) :
   {
      break;
   }
   default:
   {
      break;
   }
   }

   // remember the current mouse position
   _last = p;
   if (m_first)
   {
      emit adjustCanvas();
      m_first = false;
   }
}

C2DPixmapSegment* CDraw2D::createSubpixmapFromPolygon(QPolygon &polygon, EPolygonConversion conv)
{
   if(polygon.size() < 2)
   {
      return 0;
   }

   

   int sizeOffset = qFloor(static_cast<qreal>(CDrawSettings_2D::getInstance()->penSettings().getWidth())/2.);

   // get the polygons relative to its bounding rectangle
   QPolygon normalizedPolygon = normalizePolygon(polygon, sizeOffset);
   QRect boundingRect = polygon.boundingRect();
   QRect normalizedBoundingRect = normalizedPolygon.boundingRect();
   QSize segmentSize(
      boundingRect.size().width() +  2 * sizeOffset + 1, 
      boundingRect.size().height() + 2 * sizeOffset + 1
      );

   // create segment
   QPixmap segmentContent(segmentSize);
   segmentContent.fill(Qt::transparent);
   QPainter segmentPainter(&segmentContent);
   segmentPainter.setPen(CDrawSettings_2D::getInstance()->penSettings().getPen());
   segmentPainter.setRenderHint(QPainter::Antialiasing,_settings.getAntialiasing());
   
   QPoint firstPt;
   QPoint secondPt;
   switch(conv)
   {
   case(NO_CONVERSION):
      segmentPainter.drawPolyline(normalizedPolygon);
      break;

   case(TO_LINES):
      firstPt = normalizedPolygon.at(0);
      for (int i = 1; i<normalizedPolygon.size(); i++)
      {
         secondPt = normalizedPolygon.at(i);
         if(firstPt == secondPt)
         {
            continue;
         }
         segmentPainter.drawLine(firstPt, secondPt);
         firstPt = secondPt;
      }
      break;

   case(TO_RECT):
      segmentPainter.drawRect(normalizedBoundingRect);
      break;

   case(TO_ROUNDED_RECT):
      segmentPainter.drawRoundedRect(
      normalizedBoundingRect,
      CDrawSettings_2D::getInstance()->getRoundedRectRadius_X(),
      CDrawSettings_2D::getInstance()->getRoundedRectRadius_Y()
      );
      break;

   case(TO_ELLIPSE):
      segmentPainter.drawEllipse(normalizedBoundingRect);
      break;

   case(TO_PIE):
      segmentPainter.drawPie(normalizedBoundingRect, alphaDegrees * 16, betaDegrees * 16);
      break;

   case(TO_ARC):
      segmentPainter.drawArc(normalizedBoundingRect, alphaDegrees * 16, betaDegrees * 16);
      break;

   case(TO_CHORD):
      segmentPainter.drawChord(normalizedBoundingRect, alphaDegrees * 16, betaDegrees * 16);
      break;

   default:
      segmentPainter.drawPolyline(normalizedPolygon);
      break;
   }

   segmentPainter.end();

   //DebugImageDialog DBG(segmentContent);
   //DBG.exec();

   C2DPixmapSegment *segment = new C2DPixmapSegment(&segmentContent, &boundingRect, 1, sizeOffset);
   return segment;
}

C2DPixmapSegment* CDraw2D::doFill(QPoint pt)
{
   if(!hasRegisteredBuffer())
   {
      return 0;
   }

   QImage img = m_buffer->toImage();
   blankAidBuffer();

   if (!checkGradientPoints())
   {
      return 0;
   }

   

   QPainterPath paintPath;
   QPolygon polygon = algorithm->polygonizeBorder(pt, *m_buffer).toPolygon();
   QRect boundingRect = polygon.boundingRect();

   QPointF normalizedCenter = CDrawSettings_2D::getInstance()->brushSettings().gradientCenterPoint();
   normalizedCenter.setX(normalizedCenter.x() - boundingRect.topLeft().x());
   normalizedCenter.setY(normalizedCenter.y() - boundingRect.topLeft().y());
   if (normalizedCenter.y() < 0)
   {
      normalizedCenter.setY(0);
   }
   if (normalizedCenter.y() > boundingRect.height())
   {
      normalizedCenter.setY(boundingRect.height());
   }
   if (normalizedCenter.x() < 0)
   {
      normalizedCenter.setX(0);
   }
   if (normalizedCenter.x() > boundingRect.width())
   {
      normalizedCenter.setX(boundingRect.width());
   }

   CBrushSettings brs = CDrawSettings_2D::getInstance()->brushSettings();
   brs.setGradientCenter(normalizedCenter);

   QPointF normalizedFocal = CDrawSettings_2D::getInstance()->brushSettings().gradientFocalPoint();
   normalizedFocal.setX(normalizedFocal.x() - boundingRect.topLeft().x());
   normalizedFocal.setY(normalizedFocal.y() - boundingRect.topLeft().y());
   if (normalizedFocal.y() < 0)
   {
      normalizedFocal.setY(0);
   }
   if (normalizedFocal.y() > boundingRect.height())
   {
      normalizedFocal.setY(boundingRect.height());
   }
   if (normalizedFocal.x() < 0)
   {
      normalizedFocal.setX(0);
   }
   if (normalizedFocal.x() > boundingRect.width())
   {
      normalizedFocal.setX(boundingRect.width());
   }

   brs.setGradientFocal(normalizedFocal);
   CDrawSettings_2D::getInstance()->setBrushSettings(brs);

   QPolygon normalizedPolygon = normalizePolygon(polygon, 0);
   if (normalizedPolygon.size() > 1)
   {
      paintPath.addPolygon(normalizedPolygon);
   }
   else
   {
      paintPath.addRect(0, 0, m_buffer->width(), m_buffer->height());
   }
   paintPath.closeSubpath();

   QSize segmentSize(boundingRect.size().width(), boundingRect.size().height());
   QPixmap segmentContent(segmentSize);
   segmentContent.fill(Qt::transparent);
   QPainter segmentPainter(&segmentContent);
   segmentPainter.setRenderHint(QPainter::Antialiasing, _settings.getAntialiasing());

   QBrush brush = CDrawSettings_2D::getInstance()->brushSettings().getBrush();
   QString summ = explainBrush(brush);
   //segmentPainter.fillRect(paintPath.boundingRect(), brush);
   segmentPainter.setPen(CDrawSettings_2D::getInstance()->penSettings().getPen());
   segmentPainter.setBrush(brush);

   segmentPainter.drawPath(paintPath);
   segmentPainter.end();

   //DebugImageDialog DBG(segmentContent);
   //DBG.exec();

   C2DPixmapSegment *segment = new C2DPixmapSegment(&segmentContent, &boundingRect, 1, 0);
   return segment;
}

bool CDraw2D::checkGradientPoints()
{
   Qt::BrushStyle _brushStyle = CDrawSettings_2D::getInstance()->brushSettings().getBrushStyle();
   if (!doUsePenTexture)
   {
      if (_brushStyle == Qt::LinearGradientPattern)
      {
         if (!newInterpolPts)
         {
            newInterpolPts = true;
         }
         else
         {
            doEnterInterpSelection();
            return false;
         }
      }
      else if (_brushStyle == Qt::RadialGradientPattern)
      {
         if (!newInterpolPts)
         {
            newInterpolPts = true;
         }
         else
         {
            doEnterInterpSelection();
            return false;
         }
      }
      else if (_brushStyle == Qt::ConicalGradientPattern)
      {

         if (!newInterpolPts)
         {
            newInterpolPts = true;
         }
         else
         {
            doEnterInterpSelection();
            return false;
         }
      }
   }

   return true;
}

//  Draw a line from current position of the cursor, to an edge in argument
//  direction (LEFT)
void CDraw2D::drawStraightAidLine(QPoint pt)
{
   if(!hasRegisteredAidBuffer() || !hasRegisteredBuffer())
   {
      return;
   }

   QPoint endPt;
   QImage img = m_buffer->toImage();
   QRgb compareColor = img.pixel(pt);
   backgroundColor = compareColor;

   blankAidBuffer();

   // draw new aid line
   if(img.pixel(pt) == compareColor)
   {
      QPainter painter(m_aidBuffer);
      QPen pen(Qt::lightGray , 1);
      pen.setStyle(Qt::DashLine);
      painter.setPen(pen);

      //DebugImageDialog _DBG(*m_buffer);
      //_DBG.exec();

      algorithm->click2edge(pt, endPt, img);
      int tmpDistance = abs(endPt.x() - pt.x());
      if(tmpDistance > 5)
      {
         painter.drawLine(pt, endPt);
      }
   }

   // update screen
   emit requestUpdate();
}

void CDraw2D::doEnterInterpSelection()
{
   stateSelectInterpolationPoints = true;
   emit info("Select first interpolation point");
}

void CDraw2D::fillerMoveEvent(QMouseEvent* event)
{
   QPoint l_pt;
   QPoint eventPt = event->pos();

   // this class does not perform sdraw, but if sdraw has been performed on the target pixmap
   // we need to use the registered zoomer object to retrieve the correct point
   if(hasRegisteredZoomer() && m_zoomer->hasSource())
   {
      l_pt = m_zoomer->getBufferPoint(eventPt, true, true);
   }
   else
   {
      l_pt = eventPt;
   }

   if(stateSelectInterpolationPoints && firstInterpPointSelected)
   {
      drawAidLine(CDrawSettings_2D::getInstance()->brushSettings().gradientCenterPoint().toPoint(), l_pt);
   }
   else if(/*aidOn &&*/ !stateSelectInterpolationPoints)
   {
      drawStraightAidLine(l_pt);
   }
}

void CDraw2D::fillerPressEvent(QMouseEvent *event)
{
   // TODO ???
}

void CDraw2D::fillerDoubleclickEvent(QMouseEvent *event)
{
   // TODO ???
}

void CDraw2D::fillerReleaseEvent(QMouseEvent* event)
{
   QPoint l_pt;
   QPoint eventPt = event->pos();

   // this class does not perform sdraw, but if sdraw has been performed on the target pixmap
   // we need to use the registered zoomer object to retrieve the correct point
   if(hasRegisteredZoomer() && m_zoomer->hasSource())
   {
      l_pt = m_zoomer->getBufferPoint(eventPt, true, true);
   }
   else
   {
      l_pt = eventPt;
   }

   C2DPixmapSegment *segment = 0;
   if(stateSelectInterpolationPoints)
   {
      if(!firstInterpPointSelected)
      {
         // the first interpolation point selected
         CBrushSettings _brush = CDrawSettings_2D::getInstance()->brushSettings();
         _brush.setGradientCenter(l_pt);
         CDrawSettings_2D::getInstance()->setBrushSettings(_brush);
         if (CDrawSettings_2D::getInstance()->brushSettings().getBrushStyle() == Qt::ConicalGradientPattern)
         {
            newInterpolPts = false;
            stateSelectInterpolationPoints = false;
            segment = doFill(fillPt);
         }
         else
         {
            //setMouseTracking(true);
            aidOn = true;
            emit info("Select second interpolation point");
            firstInterpPointSelected = true;
         }
      }
      else
      {
         // the second interpolation point selected
         CBrushSettings _brush = CDrawSettings_2D::getInstance()->brushSettings();
         _brush.setGradientFocal(l_pt);
         CDrawSettings_2D::getInstance()->setBrushSettings(_brush);
         emit info("");
         firstInterpPointSelected = false;
         stateSelectInterpolationPoints = false;

         newInterpolPts = false;
         segment = doFill(fillPt);

         if(mOperation == DRAW)
         {
            aidOn = false;
            //setMouseTracking(false);
         }
      }
   }
   else
   {
      fillPt = l_pt;
      segment = doFill(fillPt);
   }

   if (0 != segment)
   {
      emit appendPixmapSegment(segment);
      emit requestUpdate();
   }
}

QPolygon sdraw::normalizePolygon(const QPolygon &polygon, int offset)
{
   QPolygon ret = polygon;
   QRect boundingRect = ret.boundingRect();
   QPoint transPt(-boundingRect.topLeft().x() + offset,
      -boundingRect.topLeft().y() + offset);
   ret.translate(transPt);
   return ret;
}