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
//  \file      angleSelector.cpp
//
//  \brief     Implementation of a widget to select an angle between 0 and 360
//
//  \author    Stian Broen
//
//  \date      August-2011
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
#include <math.h>
#include <QPoint>
#include <QMouseEvent>
#include <QRectF>
#include <QPainter>
#include <QSpinBox>

// local includes
#include "angleSelector.h"

using namespace sdraw;

CAngleSelector::CAngleSelector(QWidget *parent, int _width, int _height)
   : QWidget(parent)
{
   spinBox = new QSpinBox(this);
   if(_width > 0 && _height)
   {
      mWidth  = _width;
      mHeight = _height;
      centralX = (qreal)mWidth / 2.0;
      centralY = (qreal)mHeight / 2.0;
   }
   else
   {
      mWidth   = default_WIDTH;
      mHeight  = default_LENGTH;
      centralX = default_CENTRAL_X;
      centralY = default_CENTRAL_Y;
   }
   setFixedSize(mWidth + 3,mHeight + 3 + spinBox->height());
   QRect spinRect(0,mHeight + 3,mWidth, spinBox->height() );
   spinBox->setGeometry(spinRect);
   spinBox->setValue(90);
   spinBox->setMaximum(360);
   spinBox->setSingleStep(1);
   connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxChanged(int)));
   centralPoint = QPoint(centralX,centralY);
   //setMouseTracking(true);
   max = 360;
   _angle = 90.0;

   // make button background
   background = QPixmap(mWidth,mHeight);
   QLinearGradient linearGrad(QPointF(0.0,0.0), QPointF(mWidth, mHeight));
   linearGrad.setColorAt(0, Qt::gray);
   linearGrad.setColorAt(1, Qt::white);
   QBrush backgroundBrush(linearGrad);
   QPainter backgroundPainter(&background);
   backgroundPainter.fillRect(QRectF(0,0,mWidth, mHeight), backgroundBrush);

}

void CAngleSelector::defAngle(qreal angle)
{
   if(angle >= 0 && angle <= max)
      _angle = angle;
   spinBox->setValue(_angle);
   drawCake();
}

void CAngleSelector::setMax(qreal _max)
{
   if(_max >= 0 && _max <= 360)
      max = _max;
   if(_angle > max)
   {
      _angle = max;
      spinBox->setValue(_angle);
      drawCake();
   }
}

CAngleSelector::~CAngleSelector()
{

}

qreal CAngleSelector::getAngle()
{
   return _angle;
}


//  Slot invoked when the user changes the angle value using the spinbox.
//  It will redraw the cake and emit the new updated value
void CAngleSelector::spinBoxChanged(int val)
{
   if(val >=0 && val <= max)
   {
      _angle = (qreal)val;
      emit setAngle(_angle);
      drawCake();
   }
}


//  Draws a cake into the buffer, defined by central point and the current angle
void CAngleSelector::drawCake()
{
   QRectF rectangle(0.0, 0.0, (qreal)mWidth, (qreal)mHeight);
   int startAngle = 0;
   int spanAngle = (int)_angle * 16;

   _buffer = background;
   QPainter painter( &_buffer );
   painter.setRenderHint(QPainter::Antialiasing,true);
   QPen pen( Qt::white, 1 );
   pen.setStyle     (Qt::SolidLine );
   pen.setCapStyle  (Qt::RoundCap  );
   pen.setJoinStyle (Qt::MiterJoin );
   painter.setPen( pen );

   QRadialGradient grad(centralPoint, (qreal)(double)mWidth/2.0);
   grad.setColorAt(0, Qt::black);
   grad.setColorAt(1, Qt::white);
   QBrush brush(grad);
   painter.setBrush(brush);
   
   painter.drawPie( rectangle, startAngle, spanAngle );

   QBrush whiteFill(Qt::white, Qt::SolidPattern);
   QPen whitePen( Qt::white, 1 );
   painter.setPen(whitePen);
   painter.setBrush(whiteFill);
   startAngle = spanAngle;
   spanAngle = 360.0 * 16 - startAngle;
   painter.drawPie( rectangle, startAngle, spanAngle );

   update();
}


//  Calculate the angle between the line going from the widget central point to the
//  argument point, and the horizontal line going through the central point
double CAngleSelector::calcAngle(const QPoint &pt)
{
   QPoint currPt = pt;
   int currX = currPt.x();
   int currY = currPt.y();

   // move to origo, the line from origo to pt is hypothenus
   currPt.setX(currX - centralPoint.x());
   currPt.setY(currY - centralPoint.y());
   
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
   return acos(res) * 180.0 / PI;

}


//  This virtual method gets called whenever the widget needs
//  painting, e.g., when it has been obscured and unhidden again,
//  and when repaint() is called.
void CAngleSelector::paintEvent( QPaintEvent* )
{
   // copy the image from the buffer pixmap to the window
   QPainter painter( this );
   painter.drawPixmap( 0,0, _buffer );
}


//  This virtual method gets called whenever the window is resized. We
//  use it to make sure that the off-screen buffer always has at least
//  the same size as the window.
//  In order not to lose the original scribbling, it is first copied
//  to a temporary buffer. After the main buffer has been resized and
//  filled with white, the image is copied from the temporary buffer to
//  the main buffer.
void CAngleSelector::resizeEvent( QResizeEvent* )
{
    if (width() > _buffer.width() ||
        height() > _buffer.height() )
    {
        blank();
    }
}


//  This virtual method gets called whenever mouse is moved while left mouse
//  button is pressed
void CAngleSelector::mouseMoveEvent( QMouseEvent* event )
{
   QPoint currPt = event->pos();
   int currX     = currPt.x();
   int currY     = currPt.y();
   int centralX  = centralPoint.x();
   int centralY  = centralPoint.y();
   double angle  = 0.0;

   if(currX > centralX && currY == centralY)
      angle = 360.0;
   else if(currX < centralX && currY == centralY)
      angle = 180.0;
   else if(currX == centralX && currY < centralY)
      angle = 90.0;
   else if(currX == centralX && currY > centralY)
      angle = 270.0;
   else if(currX > centralX && currY < centralY)
      angle = calcAngle(currPt);
   else if(currX < centralX && currY < centralY)
      angle = 180.0 - calcAngle(currPt);
   else if(currX < centralX && currY > centralY)
      angle = 180.0 + calcAngle(currPt);
   else if(currX > centralX && currY > centralY)
      angle = 360.0 - calcAngle(currPt);
   else
      return;

   if(angle >= 0 && angle <= max)
   {
      _angle = (qreal)angle;
      emit setAngle(_angle);
      spinBox->setValue(_angle);
      drawCake();
   }
}


//  This virtual method gets called whenever mouse left button is released
void CAngleSelector::mouseReleaseEvent(QMouseEvent *)
{
   emit defineAngle(_angle);
}

void CAngleSelector:: blank()
{
   QPixmap tmp( size() );
   tmp.fill( Qt::white );
   QPainter painter( &tmp );
   painter.drawPixmap( 0,0, _buffer );
   _buffer = tmp;
   drawCake();
}