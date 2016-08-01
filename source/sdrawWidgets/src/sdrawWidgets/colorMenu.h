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
//  \file      colorMenu.h
//
//  \brief     Declaration of a class that contain all necessary widgets to
//             select 2 colors. It basically just contains the different color
//             widgets, make layout for them and routes the color signals to parent
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

#ifndef COLORMENU_H
#define COLORMENU_H

#define default_COLORMENU_WIDTH  770
#define default_COLORMENU_HEIGHT 135

// Qt includes
#include <QWidget>

// local includes
#include "colorPalette.h"
#include "colorDoubleButton.h"

// forward declarations
class QDial;

namespace sdraw {
   class CColorMenu : public QWidget
   {
      Q_OBJECT

   public:
      CColorMenu(QWidget *parent = 0);
      ~CColorMenu();

   public slots:
      void colorFromPaletteSlot(QColor &color, unsigned short noColor);
      void color1FromButtonsSlot(QColor &color);
      void color2FromButtonsSlot(QColor &color);
      void penColorFromButtonsSlot(QColor &color);
      void alphaChanged(int value);
      void grayLvlChanged(int value);
      void respondToColorRequest();

   signals:
      void routeColorSig(QColor &color, unsigned short noColor);
      void setColor1Sig(QColor &color);
      void setColor2Sig(QColor &color);
      void setPenColorSig(QColor &color);
      void changeAlphaFromMenuSig(int value);
      void changeGrayLvlFromMenuSig(int value);

   private:
      QDial *alphaDial;
      QDial *grayLvlDial;
      CColorPalette *palette;
      CColorDoubleButton *buttons;
      QColor color1;
      QColor color2;
      QColor penColor;

   };
}
#endif