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
//  \file      colorDoubleButton.h
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

#ifndef COLORDOUBLEBUTTON_H
#define COLORDOUBLEBUTTON_H

#define default_DOUBLEBUTTON_WIDTH  60
#define default_DOUBLEBUTTON_HEIGHT 135
#define default_middleHorizontal default_DOUBLEBUTTON_HEIGHT/2 + 1

#include <QWidget>
#include <QPixmap>
#include <QRect>

// forward declarations
class QPaintEvent;
class QMouseEvent;

namespace sdraw {
   class CColorDoubleButton : public QWidget
   {
      Q_OBJECT

   public:
      CColorDoubleButton(QWidget *parent = 0);
      ~CColorDoubleButton();

      public slots:
      void changePen(QColor &color, unsigned short buttonNo);
      void changeAlpha(int value);
      void changeGrayLvl(int value);

   protected:
      virtual void paintEvent(QPaintEvent *);
      virtual void mouseReleaseEvent(QMouseEvent *);

   signals:
      void changePenColorFromButtonsSig(QColor &color);
      void changePen1FromButtonsSig(QColor &color);
      void changePen2FromButtonsSig(QColor &color);

   private:

      bool redSelected;
      bool blueSelected;
      void drawButtons();
      void drawRed(bool selected, bool changeSelected);
      void drawBlue(bool selected, bool changeSelected);
      void selectRed(bool choice);
      void selectBlue(bool choice);
      QPixmap _buffer;
      QRect firstButtonRect;
      QRect secondButtonRect;
      QRect firstButtonColorRect;
      QRect secondButtonColorRect;
      QColor color1;
      QColor color2;
      QColor penColor;
   };
}
#endif