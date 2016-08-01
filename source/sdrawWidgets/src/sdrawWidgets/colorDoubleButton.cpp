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
//  \file      colorDoubleButton.cpp
//
//  \brief     Declaration of a class that contains 2 buttons to receive and
//             send color selections
//
//  \author    Stian Broen
//
//  \date      14.09.2011
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
#include <QPainter>
#include <QPaintEvent>
#include <QColorDialog>

// local includes
#include "colorDoubleButton.h"

using namespace sdraw;

CColorDoubleButton::CColorDoubleButton(QWidget *parent)
   : QWidget(parent)
{
   setFixedSize(default_DOUBLEBUTTON_WIDTH, default_DOUBLEBUTTON_HEIGHT);
   _buffer = QPixmap(default_DOUBLEBUTTON_WIDTH,default_DOUBLEBUTTON_HEIGHT);
   _buffer.fill(Qt::white);

   firstButtonRect  = QRect(QPoint(5,5), 
                            QPoint(default_DOUBLEBUTTON_WIDTH - 6, default_middleHorizontal - 10));
   secondButtonRect = QRect(QPoint(5, default_middleHorizontal -2), 
                            QPoint(default_DOUBLEBUTTON_WIDTH - 6, default_DOUBLEBUTTON_HEIGHT - 15));

   firstButtonColorRect  = QRect(QPoint(15,15), 
                            QPoint(default_DOUBLEBUTTON_WIDTH - 16, default_middleHorizontal - 20));
   secondButtonColorRect = QRect(QPoint(15, default_middleHorizontal + 8), 
                            QPoint(default_DOUBLEBUTTON_WIDTH - 16, default_DOUBLEBUTTON_HEIGHT - 25));

   color1   = Qt::black;
   color2   = Qt::black;
   penColor = Qt::black;
   redSelected  = false;
   blueSelected = false;
   drawButtons();
}

CColorDoubleButton::~CColorDoubleButton()
{

}

void CColorDoubleButton::mouseReleaseEvent( QMouseEvent * event)
{
   switch(event->button())
   {
      case(Qt::LeftButton):
      {
         if(event->pos().y() < default_middleHorizontal)
         {
            drawRed(true, true);
         }
         else
         {
            drawBlue(true, true);
         }
         break;
      }
      case(Qt::RightButton):
      {
         if(event->pos().y() < default_middleHorizontal)
         {
            QColor tmpClr = QColorDialog::getColor();
            if(tmpClr.isValid())
            {
               color1 = tmpClr;
               emit changePen1FromButtonsSig(color1);
               if(redSelected)
               {
                  penColor = tmpClr;
                  emit changePenColorFromButtonsSig(penColor);
               }
               drawRed(true, false);
            }
         }
         else
         {
            QColor tmpClr = QColorDialog::getColor();
            if(tmpClr.isValid())
            {
               color2 = tmpClr;
               emit changePen2FromButtonsSig(color2);
               if(blueSelected)
               {
                  penColor = tmpClr;
                  emit changePenColorFromButtonsSig(penColor);
               }
               drawBlue(true, false);
            }
         }
         break;
      }
      default:
      {
         return;
      }
   }
}

void CColorDoubleButton::paintEvent( QPaintEvent* )
{
   // copy the image from the buffer pixmap to the window
   QPainter painter( this );
   painter.drawPixmap( 0,0, _buffer );
}

void CColorDoubleButton::drawButtons()
{
   QPainter painter(&_buffer);

   // draw outer frame
   /*
   QRect mainFrame(QPoint(1,1), QPoint(default_DOUBLEBUTTON_WIDTH - 3, default_DOUBLEBUTTON_HEIGHT - 12));
   QPen pen(Qt::black , 1);
   pen.setStyle(Qt::SolidLine);
   painter.setPen(pen);
   painter.drawRoundedRect(mainFrame, 5, 5);
   */

   // draw button frames
   QPen redPen(Qt::red, 2);
   painter.setPen(redPen);
   painter.drawRect(firstButtonRect);
   QPen bluePen(Qt::blue, 2);
   painter.setPen(bluePen);
   painter.drawRect(secondButtonRect);
   update();
   painter.end();

   // draw button colors
   drawRed(true, true);   
}

void CColorDoubleButton::drawRed(bool selected, bool changeSelected)
{
   if(changeSelected)
   {
      emit changePenColorFromButtonsSig(color1);
      redSelected  =  selected;
      blueSelected = !selected;
      selectRed (redSelected );
      selectBlue(blueSelected);
   }

   QPainter painter(&_buffer);
   painter.fillRect(firstButtonColorRect,Qt::white);
   painter.fillRect(secondButtonColorRect,Qt::white);

   painter.setPen(QPen(color1));
   painter.fillRect(firstButtonColorRect,color1);

   painter.setPen(QPen(color2));
   painter.fillRect(secondButtonColorRect,color2);

   update();
   painter.end();
}

void CColorDoubleButton::drawBlue(bool selected, bool changeSelected)
{
   if(changeSelected)
   {
      emit changePenColorFromButtonsSig(color2);
      redSelected  = !selected;
      blueSelected =  selected;
      selectRed (redSelected );
      selectBlue(blueSelected);
   }

   QPainter painter(&_buffer);
   painter.fillRect(firstButtonColorRect,Qt::white);
   painter.fillRect(secondButtonColorRect,Qt::white);

   painter.setPen(QPen(color2));
   painter.fillRect(secondButtonColorRect,color2);

   painter.setPen(QPen(color1));
   painter.fillRect(firstButtonColorRect,color1);

   update();
   painter.end();
}

void CColorDoubleButton::selectRed(bool choice)
{
   QPainter painter(&_buffer);
   if(choice == true)
   {
      QBrush brush;
      QLinearGradient grad((QPointF)firstButtonRect.topLeft(), (QPointF)firstButtonRect.bottomRight());
      grad.setColorAt(0, Qt::gray);
      grad.setColorAt(1, Qt::white);
      brush = QBrush (grad);
      painter.setPen(QPen(Qt::white));
      painter.setBrush(brush);
      painter.fillRect(QRect(QPoint(firstButtonRect.topLeft    ().x() + 1, firstButtonRect.topLeft    ().y() + 1) ,
                             QPoint(firstButtonRect.bottomRight().x() - 1, firstButtonRect.bottomRight().y() - 1)), brush);
      
   }
   else
   {
      painter.fillRect(QRect(QPoint(firstButtonRect.topLeft    ().x() + 1, firstButtonRect.topLeft    ().y() + 1) ,
                       QPoint(firstButtonRect.bottomRight().x() - 1, firstButtonRect.bottomRight().y() - 1)), Qt::white);
   }

   
   update();
   painter.end();
}

void CColorDoubleButton::selectBlue(bool choice)
{
   QPainter painter(&_buffer);
   if(choice == true)
   {
      QBrush brush;
      QLinearGradient grad((QPointF)secondButtonRect.topLeft(), (QPointF)secondButtonRect.bottomRight());
      grad.setColorAt(0, Qt::gray);
      grad.setColorAt(1, Qt::white);
      brush = QBrush (grad);
      painter.setPen(QPen(Qt::white));
      painter.setBrush(brush);
      painter.fillRect(QRect(QPoint(secondButtonRect.topLeft    ().x() + 1, secondButtonRect.topLeft    ().y() + 1) ,
                             QPoint(secondButtonRect.bottomRight().x() - 1, secondButtonRect.bottomRight().y() - 1)), brush);
   }
   else
   {
      painter.fillRect(QRect(QPoint(secondButtonRect.topLeft    ().x() + 1, secondButtonRect.topLeft    ().y() + 1) ,
                             QPoint(secondButtonRect.bottomRight().x() - 1, secondButtonRect.bottomRight().y() - 1)), 
                             Qt::white);
   }

   
   update();
   painter.end();
}

void CColorDoubleButton::changePen(QColor &color, unsigned short buttonNo)
{
   if(buttonNo == 1)
   {
      color1 = color;
      if(redSelected)
      {
         penColor = color1;
         emit changePenColorFromButtonsSig(penColor);
      }
      drawRed(true, false); 
   }
   else if(buttonNo == 2)
   {
      color2 = color;
      if(blueSelected)
      {
         penColor = color1;
         emit changePenColorFromButtonsSig(penColor);
      }
      drawBlue(true, false);
   }
   else
      return;
}

void CColorDoubleButton::changeAlpha(int value)
{
   if(redSelected)
   {
      color1.setAlpha(value);
      emit changePen1FromButtonsSig(color1);
      emit changePenColorFromButtonsSig(color1);
      drawRed(true, false); 
   }
   else if(blueSelected)
   {
      color2.setAlpha(value);
      emit changePen2FromButtonsSig(color2);
      emit changePenColorFromButtonsSig(color2);
      drawBlue(true, false);
   }
   else
      return;
}

void CColorDoubleButton::changeGrayLvl(int value)
{
   QColor *pt = 0;
   if(redSelected)
   {
      pt = &color1;
   }
   else if(blueSelected)
   {
      pt = &color2;
   }
   else
      return;

   int mRedJump   = (int)((qreal)(255 - pt->red  ()) / 100.0) * value;
   int mGreenJump = (int)((qreal)(255 - pt->green()) / 100.0) * value;
   int mBlueJump  = (int)((qreal)(255 - pt->blue ()) / 100.0) * value;

   pt->setRed  (mRedJump  );
   pt->setGreen(mGreenJump);
   pt->setBlue (mBlueJump );

   emit changePenColorFromButtonsSig(*pt);

   if(redSelected)
   {
      emit changePen1FromButtonsSig(color1);
      drawRed(true, false);
   }
   else
   {
      emit changePen1FromButtonsSig(color2);
      drawBlue(true, false);
   }
   pt = 0;
}
