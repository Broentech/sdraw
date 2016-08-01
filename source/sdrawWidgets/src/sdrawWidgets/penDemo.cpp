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
//  \file      penDemo.h
//
//  \brief     Implementation of a class that demonstrates the current pen settings
//
//  \author    Stian Broen
//
//  \date      13.05.2012
//
//  \par       Copyright:
//
//             Copyright by Stian Broen
//
//  \par       Revision History
//
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QPixmap>
#include <QPainter>
#include <QTimer>
#include <QResizeEvent>
#include <QtCore/qmath.h>

// local includes
#include "penDemo.h"

using namespace sdraw;

CPenDemoFigures::CPenDemoFigures(QObject *parent)
   :QObject(parent)
{

}

CPenDemoFigures::~CPenDemoFigures()
{

}

QLineF CPenDemoFigures::nextToDraw(int index)
{
   if(index + 1 < m_circle.size())
   {
      QLineF line(m_circle.at(index), m_circle.at(index + 1));
      qreal dx = qAbs(line.x1() - line.x2());
      qreal dy = qAbs(line.y1() - line.y2());
      if(dx > 3.0 || dy > 3.0)
      {
         return QLineF(0,0,0,0);
      }
      return line;
   }
   else if(index + 2 <  m_circle.size() + m_retangle.size())
   {
      return QLineF(m_retangle.at(index - m_circle.size() + 1), m_retangle.at(index - m_circle.size() + 2));
   }
   else if(index + 3 < m_circle.size() + m_retangle.size() + m_lines.size())
   {
      //return QLineF(0,0,0,0);
      return QLineF(m_lines.at(index - m_circle.size() - m_retangle.size() + 2), m_lines.at(index - m_circle.size() - m_retangle.size() + 3));
   }
   else
   {
      emit clear();
      return QLineF(0,0,0,0);
   }
}

void CPenDemoFigures::redrawFigures(QSize size)
{
   m_baseSize = size;

   m_circle.clear();
   m_retangle.clear();
   m_lines.clear();

   createCircles();
   createRectangle();
   createLines();
}

void CPenDemoFigures::createCircles()
{
   if(m_baseSize.isNull() || m_baseSize.isEmpty())
   {
      return;
   }

   // make head
   qreal m_radius = (qMin(m_baseSize.width(), m_baseSize.height()))/3.0;
   QPointF m_origo(m_baseSize.width()/2, m_radius + 10);
   for(int i=0; i<360; i++)
   {
      QPointF calcPt = findCirclePoint(m_origo, i, m_radius); 
      if(calcPt.x() >= 0 && calcPt.y() >= 0)
      {
         m_circle << calcPt;
      }
   }

   // make left eye
   QPointF m_leftEyeOrigo;
   m_leftEyeOrigo.setX(m_origo.x() - (m_radius/2.3));
   m_leftEyeOrigo.setY(m_origo.y() - (m_radius/2.3));
   qreal m_smallRadius = m_radius / 5.0;
   QPolygonF m_leftEye;
   for(int i=0; i<360; i+=4)
   {
      QPointF calcPt = findCirclePoint(m_leftEyeOrigo, i, m_smallRadius); 
      if(calcPt.x() >= 0 && calcPt.y() >= 0)
      {
         m_leftEye << calcPt;
      }
   }

   // make right eye
   QPointF m_rightEyeOrigo;
   m_rightEyeOrigo.setX(m_origo.x() + (m_radius/2.3));
   m_rightEyeOrigo.setY(m_origo.y() - (m_radius/2.3));
   QPolygonF m_rightEye;
   for(int i=0; i<360; i+=4)
   {
      QPointF calcPt = findCirclePoint(m_rightEyeOrigo, i, m_smallRadius); 
      if(calcPt.x() >= 0 && calcPt.y() >= 0)
      {
         m_rightEye << calcPt;
      }
   }

   // make nose
   QPointF m_noseOrigo;
   m_noseOrigo.setX(m_origo.x());
   m_noseOrigo.setY(m_origo.y() + (m_radius/15));
   qreal m_smallestRadius = m_radius / 9.0;
   QPolygonF m_nose;
   for(int i=0; i<360; i+=5)
   {
      QPointF calcPt = findCirclePoint(m_noseOrigo, i, m_smallestRadius); 
      if(calcPt.x() >= 0 && calcPt.y() >= 0)
      {
         m_nose << calcPt;
      }
   }

   // make mouth
   QPointF m_mouthOrigo;
   m_mouthOrigo.setX(m_origo.x());
   m_mouthOrigo.setY(0);
   qreal m_bigRadius = m_radius * 1.7;
   QPolygonF m_mouth;
   for(int i=250; i<290; i++)
   {
      QPointF calcPt = findCirclePoint(m_mouthOrigo, i, m_bigRadius); 
      if(calcPt.x() >= 0 && calcPt.y() >= 0)
      {
         m_mouth << calcPt;
      }
   }

   m_circle << m_leftEye;
   m_circle << m_rightEye;
   m_circle << m_nose;
   m_circle << m_mouth;
}

QPointF CPenDemoFigures::findCirclePoint(const QPointF &origo, int degrees, qreal radius)
{
   QPointF first = QPointF(origo.x() + radius, origo.y()); 
   int m_degrees = qAbs(degrees) % 360;
   if(m_degrees == 0 || m_degrees == 360)
   {
      return first;
   }
   else if(m_degrees == 180)
   {
      return QPointF(origo.x() - radius, origo.y());
   }

   int m_useDeg = m_degrees;
   if(m_degrees > 180)
   {
      m_useDeg = 180 - (m_degrees % 180);
   }

   qreal m_rad = radius;
   qreal m_deg = static_cast<qreal>(m_useDeg);
   qreal m_pi  = static_cast<qreal>(PI);

   qreal m_degInRad = m_deg * m_pi / 180.0;
   qreal m_sin      = qSin(m_degInRad);
   qreal dy         = m_rad * m_sin;
   qreal m_bet      = (180.0 - m_deg) / 2.0;
   qreal m_betInRad = m_bet * m_pi / 180.0;
   qreal m_tan      = qTan(m_betInRad); 
   if(m_tan == 0)
   {
      return QPointF(-1,-1);
   }
   qreal dx         = dy / m_tan;

   if(m_degrees < 180)
   {
      return QPointF(first.x() - dx, first.y() - dy);
   }
   else
   {
      return QPointF(first.x() - dx, first.y() + dy);
   }
}

void CPenDemoFigures::createRectangle()
{
   qreal m_radius = (qMin(m_baseSize.width(), m_baseSize.height()))/3.0;
   QPointF startPt(m_baseSize.width()/2 - m_radius + 3, 2*(m_radius) + 12);

   qreal m_width = 2*m_radius;
   qreal m_stepSize = m_width / 50;
   for(int i=0; i<50; i++)
   {
      m_retangle << QPointF(startPt.x() + i*m_stepSize, startPt.y());
   }

   startPt = m_retangle.at(m_retangle.size()-1);
   m_stepSize /= 2;
   for(int i=0; i<50; i++)
   {
      m_retangle << QPointF(startPt.x(), startPt.y() + i*m_stepSize);
   }

   m_stepSize *= 2;
   startPt = m_retangle.at(m_retangle.size()-1);
   for(int i=0; i<50; i++)
   {
      m_retangle << QPointF(startPt.x() - i*m_stepSize, startPt.y());
   }

   startPt = m_retangle.at(m_retangle.size()-1);
   m_stepSize /= 2;
   for(int i=0; i<50; i++)
   {
      m_retangle << QPointF(startPt.x(), startPt.y() - i*m_stepSize);
   }
}

void CPenDemoFigures::createLines()
{

}

CPenDemo::CPenDemo(QWidget *parent)
   :QWidget(parent)
{
   figures = 0;
   this->resize(parent->size());
   m_demoMap  = 0;
   m_timer    = 0;
   m_counter  = 0;
   initDemoMap();
}

CPenDemo::~CPenDemo()
{
   if(0 != m_demoMap)
   {
      delete m_demoMap;
      m_demoMap = 0;
   }
   if(0 != m_timer)
   {
      delete m_timer;
      m_timer = 0;
   }
}

void CPenDemo::initDemoMap()
{
   if(0 != m_demoMap)
   {
      delete m_demoMap;
      m_demoMap = 0;
   }
   m_demoMap = new QPixmap(this->width(), this->height());
   m_demoMap->fill(Qt::white);

   if(0 != figures)
   {
      delete figures;
      figures = 0;
   }
   figures = new CPenDemoFigures(this);

   connect(figures, SIGNAL(clear()), 
      this, SLOT(resetCounter()));

   figures->redrawFigures(m_demoMap->size());

   if(0 != m_timer)
   {
      if(m_timer->isActive())
      {
         m_timer->stop();
      }

      disconnect(m_timer, SIGNAL(timeout()), 
         this, SLOT(updateDemoMap()));

      delete m_timer;
      m_timer = 0;
   }
   m_timer = new QTimer(this);

   connect(m_timer, SIGNAL(timeout()), 
      this, SLOT(updateDemoMap()));

   m_timer->start(10);
}

void CPenDemo::updateDemoMap()
{
   if(!this->isVisible() || 0 == figures || 0 == m_demoMap)
   {
      return;
   }

   QLineF line = figures->nextToDraw(m_counter);
   m_counter++;

   if(line.length() <= 0 || line.x1() < 0 || line.x2() < 0 || line.y1() < 0 || line.y2() < 0)
   {
      return;
   }
   QPainter painter(m_demoMap);
   painter.setRenderHint(QPainter::Antialiasing);
   painter.setPen(CDrawSettings_2D::getInstance()->penSettings().getPen());
   painter.drawLine(line);
   painter.end();

   update();
}

void CPenDemo::paintEvent(QPaintEvent *)
{
   QPainter painter(this);
   painter.drawPixmap(0,0, m_demoMap->width(), m_demoMap->height(), *m_demoMap);
   painter.end();
}

void CPenDemo::resizeEvent(QResizeEvent * event)
{
   this->setMaximumSize(event->size());
   this->setMinimumSize(event->size());
   initDemoMap();
}

void CPenDemo::resetCounter()
{
   m_timer->stop();
   m_counter = 0;
   QTimer::singleShot(3000, this, SLOT(initDemoMap()));
}

