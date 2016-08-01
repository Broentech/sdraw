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
//  \file      gradientColors.h
//
//  \brief     Declaration of a class to define gradient colors
//
//  \author    Stian Broen
//
//  \date      20.05.2012
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

#ifndef GRADIENT_COLORS_H
#define GRADIENT_COLORS_H

// Qt includes
#include <QWidget>
#include <QHash>
#include <QMap>
#include <QColor>
#include <QGroupBox>
#include <QCheckBox>

// solution includes
#include "chooseColor.h"

// Qt forward declarations
class QPaintEvent;
class QPushButton;
class QPolygon;
class QVBoxLayout;
class QPixmap;
class QDial;
class QSpinBox;
class QMouseEvent;

namespace sdraw {
   // gradient color arrow class definition
   class GradientArrow
   {

   public:
      GradientArrow();

      bool     m_hasFocus;
      int      m_index;
      QPolygon m_area;
      QColor   m_color;
      qreal    m_position;
   };

   // drawing widget definition
   class CGradientColorsView : public QWidget
   {
      Q_OBJECT

   public:
      CGradientColorsView(QWidget *parent = 0);
      ~CGradientColorsView();
      void toggleBackground(bool onOrOff);
      void createBackground(int vertLineSpace = 5, int horLineSpace = 5);
      void setFocusColor(const QColor &col);
      void clearArrows();
      inline QHash<int, GradientArrow> arrows() const { return m_arrows; }
      QPixmap *getBackGround();

   protected:
      void paintEvent(QPaintEvent *);
      void mousePressEvent(QMouseEvent *);
      void mouseMoveEvent(QMouseEvent *);
      void mouseReleaseEvent(QMouseEvent *);

   public slots:
      void changeArrow(GradientArrow arrow);
      void addArrow(GradientArrow arrow);
      void removeArrow(int index);

   signals:
      void arrowMove(qreal lastPos, GradientArrow arrow);
      void newFocusColor(const QColor &col, int index);

   private:
      int backgroundVerSpace;
      int backgroundHorSpace;
      QRect viewRect;
      QSize viewSize;
      QPixmap *background;
      QPoint dragStart;
      QHash<int, GradientArrow> m_arrows;
      bool m_isMoving;
   };

   // main widget definition
   class CGradientColors : public QWidget
   {
      Q_OBJECT

   public:
      CGradientColors(QWidget *parent = 0);
      ~CGradientColors();
      bool getAllColors(QMap<qreal, QColor> &colors);
      QPixmap *getBackGround();
      void setSingleColorMode();
      void setMultiColorMode();

   private slots:
      void arrowMove(qreal lastPos, GradientArrow arrow);
      void addColorButtonClicked();
      void colorFromPicker(const QColor &col);
      void alphaChanged(int val);
      void deleteFocusArrow();
      void resetToDefault();
      void newFocusColor(const QColor &col, int index);
      void toggleBackgroundGrid(int state);
      void changeBackgroundGridSpacing(int spacing);
      void addAlphaAndSendSingleColor(const QColor &color);

   signals:
      void setSingleColor(const QColor &color);
      void addGradientColor(qreal pos, const QColor &color);
      void setGradientColor(qreal lastPos, qreal pos, const QColor &color);
      void removeGradientColor(qreal pos);
      void gradientReset();
      void backgroundChanged(bool onOrOff);
      void changeArrow(GradientArrow arrow);
      void addArrow(GradientArrow arrow);
      void removeArrow(int index);

   private:
      void arrangeLayout();
      void addArrow(const QColor &color, qreal position = 0.5, bool hasFocus = true);

   private:
      QVBoxLayout *layout;
      QCheckBox *showBackgroundGrid;
      QSpinBox *backGroundSpacingSpinbox;
      QDial *alphaDial;
      QSpinBox *alphaSpinbox;
      QPushButton *addColorButton;
      QPushButton *removeColorButton;
      QPushButton *resetButton;
      CGradientColorsView *colorView;
      ChooseColor *colorPicker;
      int m_focusArrow;
      //QHash<int, GradientArrow> m_arrows;
   };
}

#endif