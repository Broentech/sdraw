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
//  \file      penSelector.h
//
//  \brief     Declaration of a class to define pen
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
*/
//h-//////////////////////////////////////////////////////////////////////////

#ifndef PENSELECTOR_H
#define PENSELECTOR_H

// Qt includes
#include <QPen>

// solution includes
#include <Common/definitions.h>
#include "chooseColor.h"

// local includes
#include "_selectionBase.h"
#include "penDemo.h"
#include "penWidthSelector.h"

// forward Qt declarations
class QPushButton;
class QToolButton;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QString;
class QSpinBox;
class QSlider;
class QPen;
class QButtonGroup;
class QPaintEvent;
class QVBoxLayout;

namespace sdraw {
   class CPenSelector : public SdrawCustomWidget
   {
      Q_OBJECT

   public:
      explicit CPenSelector(QWidget *parent = 0);
      ~CPenSelector();

   public slots:
      void someStyleButtonClicked(int id);
      void someCapButtonClicked(int id);
      void someJoinButtonClicked(int id);
      void widthChanging(int val);
      void widthChanged(int val);
      void colorChanged(const QColor &col);

   private:
      void mouseReleaseEvent(QMouseEvent *);
      void arrangeColorSelection();
      ChooseColor *colorPicker;
      void arrangeDemo();
      void arrangeStyleButtons();
      void arrangeWidthSelection();
      void arrangeCapButtons();
      void arrangeJoinButtons();

   private:
      CPenDemo *demo;
      QVBoxLayout *totalLayout;
      QGridLayout *allButtonsLayout;
      QButtonGroup *styleButtons;
      QPushButton *solidButton;
      QPushButton *dashButton;
      QPushButton *dotButton;
      QPushButton *dashDotButton;
      QPushButton *dashDotDotButton;
      CPenWidthSelector *m_widthSelector;
      QButtonGroup *capButtons;
      QPushButton *squareButton;
      QPushButton *flatButton;
      QPushButton *roundButton;
      QButtonGroup *joinButtons;
      QPushButton *bevelJoinButton;
      QPushButton *miterJoinButton;
      QPushButton *roundJoinButton;
   };
}

#endif