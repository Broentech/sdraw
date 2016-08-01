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
//  \file      colorPalette.h
//
//  \brief     Declaration of a class containing a color palette
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

#ifndef COLORPALETTE_H
#define COLORPALETTE_H

#define default_COLORPALETTE_WIDTH  602
#define default_COLORPALETTE_HEIGHT 122

#define default_COLORBUTTON_WIDTH  30
#define default_COLORBUTTON_HEIGHT 30

// Qt includes
#include <QWidget>
#include <QPixmap>

namespace sdraw {
   class CColorPalette : public QWidget
   {
      Q_OBJECT

   public:
      explicit CColorPalette(QWidget *parent = 0, int width = 0, int height = 0);
      ~CColorPalette();

   protected:
      virtual void paintEvent(QPaintEvent *);
      virtual void mouseMoveEvent(QMouseEvent *);
      virtual void mouseReleaseEvent(QMouseEvent *);
      virtual void leaveEvent(QEvent      *);

   signals :
      void colorSignalFromPalette(QColor &color, unsigned short noColor);

   private:
      void makeColors();
      void makeButtons();

   private:
      QRect chooseRect1;
      QRect choiceRect1;
      QRect choiceRect2;
      int widgetWidth;
      int widgetHeight;
      int columnCount;
      int rowCount;
      QPixmap background;
      QList<QColor> colorList;
   };
}

#endif