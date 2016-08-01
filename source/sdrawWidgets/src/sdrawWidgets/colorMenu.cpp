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
//  \file      colorMenu.cpp
//
//  \brief     Implementation of a class that contain all necessary widgets to
//             select 2 colors. It basically just contains the different color
//             widgets, make layout for them and routes the color signals to parent
//
//  \author    Stian Broen
//
//  \date      September-2011
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
#include <QDial>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

// local includes
#include "colorMenu.h"

using namespace sdraw;

CColorMenu::CColorMenu(QWidget *parent)
   : QWidget(parent)
{
   setFixedSize(default_COLORMENU_WIDTH, default_COLORMENU_HEIGHT);

   color1   = Qt::black;
   color2   = Qt::black;
   penColor = Qt::black;

   // setup widgets
   alphaDial = new QDial(this);
   alphaDial->setRange(0,255);
   alphaDial->setValue(255);
   alphaDial->setToolTip("Transparency");

   grayLvlDial = new QDial(this);
   grayLvlDial->setRange(0,100);
   grayLvlDial->setValue(0);

   // this doesnt work at the moment, TODO
   grayLvlDial->hide();

   buttons = new CColorDoubleButton(this);
   palette = new CColorPalette(this);

   // dial layout
   QVBoxLayout *dialLayout = new QVBoxLayout;
   dialLayout->addWidget(alphaDial);
   dialLayout->addWidget(grayLvlDial);

   // total layout
   QHBoxLayout *totalLayout = new QHBoxLayout;
   totalLayout->addLayout(dialLayout);
   totalLayout->addWidget(buttons);
   totalLayout->addWidget(palette);

   setLayout(totalLayout);

   // make connections
   connect(alphaDial  , SIGNAL(valueChanged(int)), 
           this       , SLOT  (alphaChanged(int)));

   connect(grayLvlDial, SIGNAL(valueChanged  (int)), 
           this       , SLOT  (grayLvlChanged(int)));

   connect(palette    , SIGNAL(colorSignalFromPalette(QColor &, unsigned short)),
           this       , SLOT  (colorFromPaletteSlot  (QColor &, unsigned short)));

   connect(this       , SIGNAL(routeColorSig         (QColor &, unsigned short)),
           buttons    , SLOT  (changePen           (QColor &, unsigned short)));

   connect(this       , SIGNAL(changeAlphaFromMenuSig(int)),
           buttons    , SLOT  (changeAlpha           (int)));

   connect(this       , SIGNAL(changeGrayLvlFromMenuSig(int)),
           buttons    , SLOT  (changeGrayLvl           (int)));

   connect(buttons, SIGNAL(changePen1FromButtonsSig(QColor &)),
           this   , SLOT  (color1FromButtonsSlot     (QColor &)));

   connect(buttons, SIGNAL(changePen2FromButtonsSig(QColor &)),
           this   , SLOT  (color2FromButtonsSlot     (QColor &)));

   connect(buttons, SIGNAL(changePenColorFromButtonsSig(QColor &)),
           this   , SLOT  (penColorFromButtonsSlot     (QColor &)));

}

CColorMenu::~CColorMenu()
{

}

void CColorMenu::colorFromPaletteSlot(QColor &color, unsigned short noColor)
{
   if(noColor == 1)
   {
      color1 = color;
      emit setColor1Sig(color);
   }
   else if(noColor == 2)
   {
      color2 = color;
      emit setColor2Sig(color);
   }
   else
      return;

   emit routeColorSig(color, noColor);
}

void CColorMenu::alphaChanged(int value)
{
   emit changeAlphaFromMenuSig(value);
}

void CColorMenu::grayLvlChanged(int value)
{
   emit changeGrayLvlFromMenuSig(value);
}

void CColorMenu::color1FromButtonsSlot  (QColor &color)
{
   color1 = color;
   emit setColor1Sig(color);
}

void CColorMenu::color2FromButtonsSlot  (QColor &color)
{
   color2 = color;
   emit setColor2Sig(color);
}

void CColorMenu::penColorFromButtonsSlot(QColor &color)
{
   penColor = color;
   emit setPenColorSig(color);
}

void CColorMenu::respondToColorRequest()
{
   emit setColor1Sig(color1);
   emit setColor2Sig(color2);
   emit setPenColorSig(penColor);
}