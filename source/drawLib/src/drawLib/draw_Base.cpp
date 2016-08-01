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
/*!
//  \file      draw_Base.cpp
//
//  \brief     Implementation of the draw base functionality of the drawboard
//
//  \author    Stian Broen
//
//  \date      01.04.2012
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 01.04.2012 Original version
//
//
//             
//
*/
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QPixmap>
#include <QPainter>
#include <QFile>
#include <QtDebug>
#include <QtCore/qmath.h>

// solution includes
#include <DrawCommands/drawboard_pixmapSegment.h>
#include <zooming/zoomer.h>
#include <_debugStuff/imageDialog.h>

// local includes
#include "draw_Base.h"

using namespace sdraw;

CDrawBase::CDrawBase(QObject *parent)
   : QObject(parent),
   m_buffer(0),
   m_aidBuffer(0),
   m_zoomer(0),
   bufferRegistered(false),
   aidBufferRegistered(false),
   hasZoomer(false),
   stateSelectInterpolationPoints(false),
   firstInterpPointSelected(false),
   newInterpolPts(true),
   //currShape(SNoShape),
   doUsePenTexture(false),
   aidOn(false),
   aidOnly(false),
   _angle(90),
   _lastAngleDiff(0),
   radius(0),
   alphaDegrees(0),
   betaDegrees(0),
   clockCount(0),
   //roundedRectRadius_X(10),
   //roundedRectRadius_Y(10),
   lastQuadrant(undefined)
{
   
}

CDrawBase::~CDrawBase()
{
   m_buffer = 0;
   m_aidBuffer = 0;
   m_zoomer = 0;
}

void CDrawBase::registerBuffer(QPixmap *buffer)
{
   if(0 == buffer)
   {
      releaseBuffer();
   }
   else
   {
      m_buffer = buffer;
      bufferRegistered = true;
   }
}

void CDrawBase::releaseBuffer()
{
   m_buffer = 0;
   bufferRegistered = false;
}

bool CDrawBase::hasRegisteredBuffer()
{
   if(0 != m_buffer)
   {
      bufferRegistered = true;
   }
   else
   {
      bufferRegistered = false;
   }
   return bufferRegistered;
}

void CDrawBase::registerAidBuffer(QPixmap *buffer)
{
   if(0 == buffer)
   {
      releaseAidBuffer();
   }
   else
   {
      m_aidBuffer = buffer;
      aidBufferRegistered = true;
   }
}

void CDrawBase::releaseAidBuffer()
{
   m_aidBuffer = 0;
   aidBufferRegistered = false;
}

bool CDrawBase::hasRegisteredAidBuffer()
{
   if(0 != m_aidBuffer)
   {
      aidBufferRegistered = true;
   }
   else
   {
      aidBufferRegistered = false;
   }
   return aidBufferRegistered;
}

void CDrawBase::registerZoomer(Zoomer *zoomer, const QString &bufferName, const QString &aidBufferName)
{
   if(0 == zoomer)
   {
      releaseZoomer();
   }
   else
   {
      m_bufferName = bufferName;
      m_aidBufferName = aidBufferName;
      m_zoomer = zoomer;
      hasZoomer = true;
   }
}

void CDrawBase::releaseZoomer()
{
   m_zoomer = 0;
   hasZoomer = false;
}

bool CDrawBase::hasRegisteredZoomer()
{
   if(0 != m_zoomer)
   {
      hasZoomer = true;
   }
   else
   {
      hasZoomer = false;
   }
   return hasZoomer;
}

void CDrawBase::blankAidBuffer()
{
   if(!aidBufferRegistered)
   {
      return;
   }

   m_aidBuffer->fill(Qt::white);
   emit requestUpdate();
}

void CDrawBase::drawAidLine(QPoint start, QPoint stop, QPen *pen)
{
   if(!aidBufferRegistered)
   {
      return;
   }

   QPen usePen;
   if (0 != pen)
   {
      usePen = *pen;
   }
   else
   {
      m_aidBuffer->fill(Qt::white);
      usePen = QPen(Qt::lightGray, 1);
      usePen.setStyle(Qt::DashLine);
   }
   
   QPainter painter(m_aidBuffer);
   painter.setRenderHint(QPainter::Antialiasing,_settings.getAntialiasing());
   painter.setPen(usePen);
   painter.drawLine(start, stop);
   painter.end();
   emit requestUpdate();
}

void CDrawBase::drawAidEllipse(QPoint bottomRight, QPen *pen)
{
   if(!aidBufferRegistered)
   {
      return;
   }

   // draw new rectangle
   QRect newRect(_last, bottomRight);
   QPainter painter(m_aidBuffer);

   QPen usePen;
   if (0 != pen)
   {
      usePen = *pen;
   }
   else
   {
      usePen = QPen(Qt::lightGray, 1);
      usePen.setStyle(Qt::DashLine);
   }
   painter.setPen(usePen);
   painter.drawEllipse(newRect);

   emit requestUpdate();
}

void CDrawBase::drawAidRectangle(QPoint bottomRight, QPen *pen)
{
   if(!aidBufferRegistered)
   {
      return;
   }

   // draw new rectangle
   QRect newRect(_last, bottomRight);
   QPainter painter(m_aidBuffer);

   QPen usePen;
   if (0 != pen)
   {
      usePen = *pen;
   }
   else
   {
      usePen = QPen(Qt::lightGray, 1);
      usePen.setStyle(Qt::DashLine);
   }
   painter.setPen(usePen);
   painter.drawRect(newRect);
   //qInfo() << "W=" << newRect.width() << ", H=" << newRect.height();

   //DebugImageDialog DBG(*m_aidBuffer);
   //DBG.exec();

   emit requestUpdate();
}

void CDrawBase::drawAidRoundedRectangle(QPoint bottomRight, QPen *pen)
{
   if(!aidBufferRegistered)
   {
      return;
   }

   QPoint tLeft;
   QPoint bRight;

   if(_last.x() <= bottomRight.x() && _last.y() <= bottomRight.y())
   {
      tLeft  = _last;
      bRight = bottomRight;
   }
   else if(_last.x() > bottomRight.x() && _last.y() < bottomRight.y())
   {
      tLeft .setX(bottomRight.x());
      tLeft .setY(_last      .y());
      bRight.setX(_last      .x());
      bRight.setY(bottomRight.y());
   }
   else if(_last.x() > bottomRight.x() && _last.y() > bottomRight.y())
   {
      tLeft = bottomRight;
      bRight = _last;
   }
   else if(_last.x() < bottomRight.x() && _last.y() > bottomRight.y())
   {
      tLeft.setX(_last       .x());
      tLeft.setY(bottomRight .y());
      bRight.setX(bottomRight.x());
      bRight.setY(_last      .y());
   }

   // draw new rounded aid rectangle
   QRect newRect(tLeft, bRight);
   QPainter painter(m_aidBuffer);

   QPen usePen;
   if (0 != pen)
   {
      usePen = *pen;
   }
   else
   {
      usePen = QPen(Qt::lightGray, 1);
      usePen.setStyle(Qt::DashLine);
   }
   painter.setPen(usePen);
   painter.drawRoundedRect(
      newRect, 
      CDrawSettings_2D::getInstance()->getRoundedRectRadius_X(),
      CDrawSettings_2D::getInstance()->getRoundedRectRadius_Y()
      );

   emit requestUpdate();
}

void CDrawBase::drawAidArc(QPoint pt, QPen *pen)
{
   if(!aidBufferRegistered)
   {
      return;
   }

   QRect newRect(topLeftForCircular, bottomRightForCircular);
   QPainter painter(m_aidBuffer);

   QPen usePen;
   if (0 != pen)
   {
      usePen = *pen;
   }
   else
   {
      usePen = QPen(Qt::lightGray, 1);
      usePen.setStyle(Qt::DashLine);
   }
   painter.setPen(usePen);
   painter.drawArc(newRect, alphaDegrees*16, betaDegrees*16);

   emit requestUpdate();
}

void CDrawBase::drawAidPie(QPoint pt, QPen *pen)
{
   if(!aidBufferRegistered)
   {
      return;
   }

   QRect newRect(topLeftForCircular, bottomRightForCircular);
   QPainter painter(m_aidBuffer);

   QPen usePen;
   if (0 != pen)
   {
      usePen = *pen;
   }
   else
   {
      usePen = QPen(Qt::lightGray, 1);
      usePen.setStyle(Qt::DashLine);
   }
   painter.setPen(usePen);
   painter.drawPie(newRect, alphaDegrees*16, betaDegrees*16);

   emit requestUpdate();
}

void CDrawBase::drawAidChord(QPoint pt, QPen *pen)
{
   if(!aidBufferRegistered)
   {
      return;
   }

   QRect newRect(topLeftForCircular, bottomRightForCircular);
   QPainter painter(m_aidBuffer);

   QPen usePen;
   if (0 != pen)
   {
      usePen = *pen;
   }
   else
   {
      usePen = QPen(Qt::lightGray, 1);
      usePen.setStyle(Qt::DashLine);
   }
   painter.setPen(usePen);
   painter.drawChord(newRect, alphaDegrees*16, betaDegrees*16);

   emit requestUpdate();
}

void CDrawBase::drawAidPolyline(QPolygon _polyLine, QPen *pen)
{
   if(!aidBufferRegistered)
   {
      return;
   }

   if(_polyLine.size() >= 2)
   {
      // draw new aid polyline
      QPainter painter(m_aidBuffer);

      QPen usePen;
      if (0 != pen)
      {
         usePen = *pen;
      }
      else
      {
         usePen = QPen(Qt::lightGray, 1);
         usePen.setStyle(Qt::DashLine);
      }
      painter.setPen(usePen);
      painter.drawPolyline(_polyLine);
   }

   emit requestUpdate();
}

void CDrawBase::drawAidPolygon(QPolygon _polygon, QPen *pen)
{
   if(!aidBufferRegistered)
   {
      return;
   }

   if(_polygon.size() >= 2)
   {
      // draw new aid polyline
      QPainter painter(m_aidBuffer);

      QPen usePen;
      if (0 != pen)
      {
         usePen = *pen;
      }
      else
      {
         usePen = QPen(Qt::lightGray, 1);
         usePen.setStyle(Qt::DashLine);
      }
      painter.setPen(usePen);
      painter.drawPolygon(_polygon);
   }

   emit requestUpdate();
}


