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
//  \file      colorPalette.cpp
//
//  \brief     Implementation of a class containing a color palette
//
//  \author    Stian Broen
//
//  \date      September 2011
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
#include <QPainter>
#include <QMouseEvent>

// local includes
#include "colorPalette.h"

using namespace sdraw;

CColorPalette::CColorPalette(QWidget *parent, int width, int height)
   : QWidget(parent)
{
   widgetWidth  = 0;
   widgetHeight = 0;
   columnCount  = 0;
   rowCount     = 0;
   if(width > 0 && height > 0)
   {
      widgetWidth  = width;
      widgetHeight = height;
   }
   else
   {
      widgetWidth  = default_COLORPALETTE_WIDTH;
      widgetHeight = default_COLORPALETTE_HEIGHT;
   }

   setMouseTracking(true);

   // define all colors
   makeColors();

   // draw buttons to widget
   makeButtons();
}

CColorPalette::~CColorPalette()
{

}

void CColorPalette::paintEvent( QPaintEvent* )
{
   // copy the image from the buffer pixmap to the window
   QPainter painter( this );
   painter.drawPixmap( 0,0, background );
}

void CColorPalette::leaveEvent(QEvent *)
{
   QPainter painter( &background );
   if(chooseRect1 != choiceRect1 && chooseRect1 != choiceRect2)
   {
      QPen erasePen(Qt::white);
      painter.setPen(erasePen);
      painter.drawRect(chooseRect1);
      update();
   }
   // redraw the chosen colors
   if(!choiceRect1.isEmpty())
   {
      QPen pen(Qt::SolidLine);
      pen.setWidth(1);
      pen.setColor(Qt::red);
      painter.setPen(pen);
      painter.drawRect(choiceRect1);
   }
   if(!choiceRect2.isEmpty())
   {
      QPen pen(Qt::SolidLine);
      pen.setWidth(1);
      pen.setColor(Qt::blue);
      painter.setPen(pen);
      painter.drawRect(choiceRect2);
   }
}

void CColorPalette::mouseMoveEvent( QMouseEvent* event )
{
   QPainter painter( &background );

   QPen erasePen(Qt::white);
   painter.setPen(erasePen);
   painter.drawRect(chooseRect1);

   QPoint pt = event->pos();
   int remX = (pt.x() %  default_COLORBUTTON_WIDTH  );
   int remY = (pt.y() %  default_COLORBUTTON_HEIGHT );
   int x1 = pt.x() - remX;
   int y1 = pt.y() - remY;
   int x2 = x1 + default_COLORBUTTON_WIDTH;
   int y2 = y1 + default_COLORBUTTON_HEIGHT;
   QPoint upperLeft (x1 , y1);
   QPoint lowerRight(x2 , y2);
   chooseRect1 = QRect(upperLeft, lowerRight);

   QPen pen(Qt::DashLine);
   pen.setWidth(1);
   pen.setColor(Qt::red);
   painter.setPen(pen);
   painter.drawRect(chooseRect1);

   // redraw the chosen colors
   if(!choiceRect1.isEmpty())
   {
      QPen pen(Qt::SolidLine);
      pen.setWidth(1);
      pen.setColor(Qt::red);
      painter.setPen(pen);
      painter.drawRect(choiceRect1);
   }
   if(!choiceRect2.isEmpty())
   {
      QPen pen(Qt::SolidLine);
      pen.setWidth(1);
      pen.setColor(Qt::blue);
      painter.setPen(pen);
      painter.drawRect(choiceRect2);
   }
   update();
}

void CColorPalette::mouseReleaseEvent( QMouseEvent * event)
{
   // calculate draw rectangle coordinates
   QPoint pt = event->pos();
   int remX = (pt.x() %  default_COLORBUTTON_WIDTH  );
   int remY = (pt.y() %  default_COLORBUTTON_HEIGHT );
   int x1 = pt.x() - remX;
   int y1 = pt.y() - remY;
   int x2 = x1 + default_COLORBUTTON_WIDTH;
   int y2 = y1 + default_COLORBUTTON_HEIGHT;
   QPoint upperLeft (x1 , y1);
   QPoint lowerRight(x2 , y2);
   chooseRect1 = QRect(upperLeft, lowerRight);

   // get new color
   QPoint centralPoint(upperLeft.x() + 10, upperLeft.y() + 10);
   QImage img = background.toImage();
   QColor newColor(img.pixel(centralPoint));

   QRect *choice = 0;
   QColor color;
   switch(event->button())
   {
      case(Qt::LeftButton):
      {
         if(QRect(upperLeft, lowerRight) == choiceRect2)
         {
            upperLeft.setX(upperLeft.x() + 2);
            upperLeft.setY(upperLeft.y() + 2);
            lowerRight.setX(lowerRight.x() - 2);
            lowerRight.setY(lowerRight.y() - 2);
         }
         emit colorSignalFromPalette(newColor, 1);
         color = QColor(Qt::red);
         choice = &choiceRect1;
         break;
      }
      case(Qt::RightButton):
      {
         if(QRect(upperLeft, lowerRight) == choiceRect1)
         {
            upperLeft.setX(upperLeft.x() + 2);
            upperLeft.setY(upperLeft.y() + 2);
            lowerRight.setX(lowerRight.x() - 2);
            lowerRight.setY(lowerRight.y() - 2);
         }
         emit colorSignalFromPalette(newColor, 2);
         color = QColor(Qt::blue);
         choice = &choiceRect2;
         break;
      }
      default:
      {
         return;
      }
   }

   QPainter painter( &background );
   if(!choice->isEmpty())
   {
      QPen erasePen(Qt::white);
      painter.setPen(erasePen);
      painter.drawRect(*choice);
   }
   
   *choice = QRect(upperLeft, lowerRight);
   QPen pen(Qt::SolidLine);
   pen.setWidth(1);
   pen.setColor(color);
   painter.setPen(pen);
   painter.drawRect(*choice);
   choice = 0;
   update();
   return;
}

void CColorPalette::makeColors()
{
   columnCount   = (int)floor((double)widgetWidth  / ((double)default_COLORBUTTON_WIDTH ));
   rowCount      = (int)floor((double)widgetHeight / ((double)default_COLORBUTTON_HEIGHT));

   int buttonCount   = columnCount * (rowCount-1);
   qreal colorSplit  = (1.0 / ((qreal)buttonCount)) * 6.0;
   int currInterv     = 0;
   qreal RGBinterval = qreal(buttonCount) / 6.0;
   qreal mRed        = 1;
   qreal mGreen      = 1;
   qreal mBlue       = 0;
   int currInterval  = 0;
   
   // make colors
   for(int i=0; i<buttonCount; i++, currInterv++)
   {
      if(currInterv >= (int) ceil((double)RGBinterval))
      {
         currInterval++;
         currInterv = 0;
      }

      switch(currInterval)
      {
         case(0):
         {
            // color moving from yellow to green
            mRed -= colorSplit;
            break;
         }
         case(1):
         {
            // color moving from green to cyan
            mRed   = 0;
            mBlue += colorSplit;
            break;
         }
         case(2):
         {
            // color moving from cyan to blue
            mBlue   = 1;
            mGreen -= colorSplit;
            break;
         }
         case(3):
         {
            // color moving from blue to pink
            mGreen = 0; 
            mRed  += colorSplit;
            break;
         }
         case(4):
         {
            // color moving from pink to red
            mRed = 1;
            mBlue -= colorSplit;
            break;
         }
         case(5):
         {
            // color moving from red to yellow
            mBlue   = 0;
            mGreen += colorSplit;
            break;
         }

         default:
         {
            continue;
         }
      }

      // create color and add to list
      QColor tmpColor;
      tmpColor.setRedF(mRed);
      tmpColor.setGreenF(mGreen);
      tmpColor.setBlueF(mBlue);
      colorList << tmpColor;
   }

   // make greyscale
   mRed        = 1;
   mGreen      = 1;
   mBlue       = 1;
   qreal greyScaleSplit = 1.0 / columnCount;
   for(int i=0; i<columnCount; i++)
   {
      mRed   -= greyScaleSplit;
      mGreen -= greyScaleSplit;
      mBlue  -= greyScaleSplit;
      QColor tmpColor;
      tmpColor.setRedF(mRed);
      tmpColor.setGreenF(mGreen);
      tmpColor.setBlueF(mBlue);
      colorList << tmpColor;
   }
}

void CColorPalette::makeButtons()
{
   //widgetWidth = widgetWidth + columnCount + 1;
   setFixedSize(widgetWidth, widgetHeight);

   // make background
   background = QPixmap(widgetWidth,widgetHeight);
   background.fill(Qt::white);

   // create and distribute buttons
   for(int i=0; i<rowCount; i++)
   {
      for(int j=0; j<columnCount; j++)
      {
         int colorIndex = j+(i*columnCount); 
         if(colorIndex < colorList.size())
         {
            // construct rectangle
            int x1 = /*1*(j+1) +*/ (default_COLORBUTTON_WIDTH  ) * j;
            int y1 = /*1*(i+1) +*/ (default_COLORBUTTON_HEIGHT ) * i;
            int x2 = /*1*(j+1) +*/ (default_COLORBUTTON_WIDTH  ) * (j + 1) /*- 2*/;
            int y2 = /*1*(i+1) +*/ (default_COLORBUTTON_HEIGHT ) * (i + 1) /*- 2*/;

            QPoint upperLeft  (x1 + 4, y1 + 4);
            QPoint bottomRight(x2 - 4, y2 - 4);
            QRect buttonRect(upperLeft, bottomRight);

            // get color and define pen and brush
            QColor color = colorList.at(colorIndex);
            QPen pen(color, 1);
            QBrush brush(color);   

            // paint rectangle to background pixmap
            QPainter painter( &background );
            painter.setPen(pen);
            painter.setBrush(brush);
            painter.drawRect(buttonRect);
            painter.fillRect(buttonRect, brush);
         }
      }
   }

   update();
}