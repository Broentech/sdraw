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
//  \file      brushSelector.h
//
//  \brief     Declaration of a class to define brush
//
//  \author    Stian Broen
//
//  \date      August 2011
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

#ifndef BRUSHSELECTOR_H
#define BRUSHSELECTOR_H

// Qt includes
#include <QPen>
#include <QBrush>

// solution includes
#include <DrawSettings/DrawSettings_2D.h>

// local includes
#include "_selectionBase.h"
#include "angleSelector.h"
#include "gradientColors.h"

// forward Qt declarations
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QSpinBox;
class QSlider;
class QLabel;
class QButtonGroup;

namespace sdraw {
   class CBrushSelector : public SdrawCustomWidget
   {
      Q_OBJECT

   public:
      explicit CBrushSelector(QWidget *parent = 0);
      ~CBrushSelector();

   public slots:
      void angleChange(qreal angle);
      void receiveDefinedAngle(qreal angle);
      void somePatternButtonClicked(int id);
      void someSpreadButtonClicked(int id);
      void addGradientColor(qreal pos, const QColor &color);
      void setGradientColor(qreal lastPos, qreal pos, const QColor &color);
      void removeGradientColor(qreal pos);
      void gradientReset();
      void backgroundChanged(bool onOrOff);
      void setSingleColor(const QColor &color);

   private:
      void updateSettings() const;
      void mouseReleaseEvent(QMouseEvent *);
      void hideEvent(QHideEvent *);
      void redrawStandardPatterns();
      void redrawGradientButtons();
      void redrawGradientSpreadButtons(bool force = false);
      void redrawPadSpreadButton(bool force = false);
      void redrawRepeatSpreadButton(bool force = false);
      void redrawReflectSpreadButton(bool force = false);
      void arrangeSpreadButtons();
      void arrangePatternButtons();
      void arrangeGradientColors();


   private:
      QVBoxLayout *gradientColorsLayout;
      CGradientColors *m_gradientColors;
      QLabel *gradientColorsLabel;
      bool drawBackground;
      QVBoxLayout *totalLayout;
      QButtonGroup *patternButtons;
      QVBoxLayout *patternButtonsLayout;
      QPushButton *solidPatternButton;
      QPushButton *dense1PatternButton;
      QPushButton *dense2PatternButton;
      QPushButton *dense3PatternButton;
      QPushButton *dense4PatternButton;
      QPushButton *dense5PatternButton;
      QPushButton *dense6PatternButton;
      QPushButton *dense7PatternButton;
      QPushButton *noBrushButton;
      QPushButton *horPatternButton;
      QPushButton *verPatternButton;
      QPushButton *crossPatternButton;
      QPushButton *bDiagPatternButton;
      QPushButton *fDiagPatternButton;
      QPushButton *diagCrossPatternButton;
      QPushButton *linearGradientPatternButton;
      QPushButton *radialGradientPatternButton;
      QPushButton *conicalGradientPatternButton;
      CAngleSelector *angleSelector;
      QVBoxLayout *spreadButtonsLayout;
      QButtonGroup *spreadButtons;
      QPushButton *padSpreadButton;
      QPushButton *repeatSpreadButton;
      QPushButton *reflectSpreadButton;
   };
}

#endif