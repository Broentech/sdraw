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
//  \file      penDemo.h
//
//  \brief     Declaration of a class that demonstrates the current pen settings
//
//  \author    Stian Broen
//
//  \date      13.05.2012
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

#ifndef PEN_DEMO
#define PEN_DEMO

#define PI 3.14159265

// Qt includes
#include <QWidget>
#include <QPolygonF>

// solution includes
#include <DrawSettings/DrawSettings_2D.h>

// Qt forward declarations
class QTimer;
class QPixmap;
class QPaintEvent;
class QResizeEvent;

namespace sdraw {
   class CPenDemoFigures : public QObject
   {
      Q_OBJECT

   public:
      CPenDemoFigures(QObject *parent = 0);
      ~CPenDemoFigures();
      QLineF nextToDraw(int index);
      void redrawFigures(QSize size);

   signals:
      void clear();

   private: // functions
      void createCircles();
      void createRectangle();
      void createLines();

      QPointF findCirclePoint(const QPointF &origo, int degrees, qreal radius);

   private: // members
      QSize     m_baseSize;
      int       m_index;
      int       m_figureIndex;
      QPolygonF m_circle;
      QPolygonF m_retangle;
      QPolygonF m_lines;
   };

   // class to contain the demo widget
   class CPenDemo : public QWidget
   {
      Q_OBJECT

   public:
      CPenDemo(QWidget *parent = 0);
      ~CPenDemo();

   private: // members
      CPenDemoFigures    *figures;
      QPixmap            *m_demoMap;
      QTimer             *m_timer;
      int                 m_counter;

   protected:
      void resizeEvent(QResizeEvent *);
      void paintEvent(QPaintEvent  *);

      private slots:
      void updateDemoMap();
      void resetCounter();
      void initDemoMap();

   };
}
#endif