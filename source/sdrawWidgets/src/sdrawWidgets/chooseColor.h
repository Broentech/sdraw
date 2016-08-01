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
//  \file      chooseColor.h
//
//  \brief     
//
//  \author    Stian Broen
//
//  \date      22.06.2011
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 22.06.2011 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

#ifndef _CHOOSE_COLOR_H_
#define _CHOOSE_COLOR_H_

// Qt includes
#include <QWidget>
#include <QGridLayout>
#include <QMouseEvent>
#include <QPushButton>
#include <QPen>
#include <QColorDialog>
#include <QMenu>
#include <QWidgetAction>
#include <QPainter>
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>

// local includes
#include "qtcolortriangle.h"

class QMouseEvent;

namespace sdraw {
   class ViewColor : public QPushButton
   {
      Q_OBJECT

   public:
      explicit ViewColor(QWidget *parent = 0);
      ~ViewColor();
      void closeMenu();
      void setPen(const QPen &pen);
      QPen pen() const;
      void setColor(const QColor &color);
      QColor color() const;

   signals:
      void colorChanged(const QColor &color);

   public slots:
      void changeColor(const QColor &color);
      void catchCancel(int);

   private:
      void paintEvent(QPaintEvent *);
      void mousePressEvent(QMouseEvent *);

   private:
      QColorDialog *m_colorDialog;
      QPen m__pen;
      QBrush m__brush;
      QColor m__color;
   };

   class ChooseColorScreen : public QWidget
   {
      Q_OBJECT

   public:
      explicit ChooseColorScreen(QWidget *parent = 0);
      ~ChooseColorScreen();

   private:
      QLabel *m_label;

   private:
      void mousePressEvent(QMouseEvent *event);

   signals:
      void colorSelected(QColor color);
   };

   class ChooseColor : public QWidget
   {
      Q_OBJECT

   public:
      explicit ChooseColor(QWidget *parent = 0);
      ~ChooseColor();
      void closeColorDialog();
      QColor getColor();

   protected:
      void resizeEvent(QResizeEvent *event);

   signals:
      void colorChanged(const QColor &col);

   public slots:
      void setColor(const QColor &col);

   private slots:
      void pickMode();
      void colorChgd();

   private:
      QtColorTriangle *m_colorTriangle;
      ViewColor *m_colorView;
      QGridLayout *m_layout;
      ChooseColorScreen *m_screen;
   };
}

#endif 
