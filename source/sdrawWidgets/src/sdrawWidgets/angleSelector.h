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
//  \file      angleSelector.h
//
//  \brief     Declaration of a class used to select an angle between 0 and 360
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

#ifndef ANGLESELECTOR_H
#define ANGLESELECTOR_H

#define PI 3.14159265
#define default_WIDTH     201
#define default_LENGTH    201
#define default_CENTRAL_X 100
#define default_CENTRAL_Y 100

#include <QWidget>
#include <QPixmap>

// forward declarations
class QSpinBox;

namespace sdraw {
   class CAngleSelector : public QWidget
   {
      Q_OBJECT

   public:
      CAngleSelector(QWidget *parent = 0, int _width = -1, int _height = -1);
      ~CAngleSelector();
      qreal getAngle();
      void defAngle(qreal angle);
      void setMax(qreal _max);

   protected:
      virtual void mouseMoveEvent(QMouseEvent * event);
      virtual void mouseReleaseEvent(QMouseEvent *);
      virtual void paintEvent(QPaintEvent *);
      virtual void resizeEvent(QResizeEvent*);

      public slots:

      void spinBoxChanged(int val);

   signals:

      void setAngle(qreal _angle);
      void defineAngle(qreal _angle);

   private:
      int mWidth;
      int mHeight;
      qreal max;
      qreal centralX;
      qreal centralY;
      QPixmap background;
      QPixmap _buffer;
      qreal _angle;
      QPoint centralPoint;

      QSpinBox *spinBox;
      void drawCake();
      void blank();
      double calcAngle(const QPoint &pt);

   };
}

#endif