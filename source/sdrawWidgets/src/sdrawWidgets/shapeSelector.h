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
//  \file      shapeSelector.h
//
//  \brief     Declaration of a class to draw shape
//             Declaration of enum to define shape type
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

#ifndef SHAPESELECTOR_H
#define SHAPESELECTOR_H
#define MAX_X_RADIUS 90
#define MAX_Y_RADIUS 90

// Qt includes
#include <QWidget>
#include <QPixmap>

// solution includes
#include <common/definitions.h>

// local includes
#include "angleSelector.h"

// forward declarations
class QVBoxLayout;
class QPushButton;
class QToolButton;

namespace sdraw {
   class CShapeSelector : public QWidget
   {
      Q_OBJECT

   public:
      CShapeSelector(QWidget *parent = 0);
      ~CShapeSelector();
      void setShape(shapeType newType);
      shapeType getShape();
      int getXRad();
      int getYRad();
      void setXRad(int radius);
      void setYRad(int radius);

   public slots:
      void noShapeButtonClicked(bool);
      void lineShapeButtonClicked(bool);
      void rectShapeButtonButtonClicked(bool);
      void roundedRectShapeButtonClicked(bool);
      void ellipseShapeButtonClicked(bool);
      void arcShapeButtonClicked(bool);
      void pieShapeButtonClicked(bool);
      void chordShapeButtonClicked(bool);
      void polylineShapeButtonClicked(bool);
      void polygonShapeButtonClicked(bool);
      void xRadChange(qreal angle);
      void yRadChange(qreal angle);
      void closeMenu(bool);

   signals:
      //void xRadChangeSig(int radius);
      //void yRadChangeSig(int radius);
      //void shapeChangeSig(shapeType _type);
      void doCloseShapeMenuSig();

   private:
      void arrangeShapeButtons();

   private:
      QVBoxLayout *totalLayout;
      //shapeType currShape;
      QPixmap background;
      CAngleSelector *xRadSelector;
      CAngleSelector *yRadSelector;
      //int xRad;
      //int yRad;
      QToolButton *closeButton;
      QVBoxLayout *allShapesLayouts;
      QPushButton *noShapeButton;
      QPushButton *lineShapeButton;
      QPushButton *rectShapeButton;
      QPushButton *roundedRectShapeButton;
      QPushButton *ellipseShapeButton;
      QPushButton *arcShapeButton;
      QPushButton *pieShapeButton;
      QPushButton *chordShapeButton;
      QPushButton *polylineShapeButton;
      QPushButton *polygonShapeButton;

   };
}
#endif