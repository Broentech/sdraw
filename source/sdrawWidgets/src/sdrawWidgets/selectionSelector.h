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
//  \file      selectionSelector.h
//
//  \brief     Declaration of a class specify selection mode
//
//  \author    Stian Broen
//
//  \date      21.09.2011
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

#ifndef SELECTIONSELECTOR_H
#define SELECTIONSELECTOR_H

// Qt includes
#include <QWidget>
#include <QPainter>
#include <QMenu>

// solution includes
#include <common/definitions.h>

// forward declarations
class QPushButton;
class QCheckBox;
class QAction;
class QToolBar;
class QGroupBox;

namespace sdraw {
   class CSelectionSelector : public QWidget
   {
      Q_OBJECT

   public:
      CSelectionSelector(QWidget *parent = 0);
      ~CSelectionSelector(){}
      inline void setParentRect(QRect rect) { parentRect = rect; }

   public slots:
      void rectangularSelectionButtonClicked(bool checked);
      void polygonSelectionButtonClicked(bool checked);
      void freeSelectionButtonClicked(bool checked);
      void moveButtonClicked(bool checked);
      void scaleButtonClicked(bool checked);
      void rotateButtonClicked(bool checked);
      void shearButtonClicked(bool checked);
      void ignoreWhiteButtonClicked(bool checked);
      void inverseButtonClicked(bool checked);
      void cloneButtonClicked(bool checked);
      void compositionModeButtonClicked(bool checked);
      void compositionMenuButtonClicked(bool checked);
      void XButtonClicked(bool checked);
      void YButtonClicked(bool checked);
      void ZButtonClicked(bool checked);
      void squareTransButtonClicked(bool checked);
      void resondToRequestAll();
      void receiveCompositionMode(QPainter::CompositionMode mode);
      void receiveCompositionAlpha(int value);
      void closeCompositionMenu();

   signals:
      void updateSelectionModeFromWidgetSig(TSelectionMode selection);
      void updateSelectionOptionFromWidgetSig(TSelectionOption selectionOption);
      void updateCompositionMode(QPainter::CompositionMode mode);
      void updateCompositionAlpha(int value);
      void updateTransformationMode(TTransformationMode mode);
      void updateRotationAxis(Qt::Axis axis, bool enable);
      void updateTransformationSquare(bool enable);
      void requestCompModeSig();

   private:
      void arrangeWidgets();
      void arrangeSelectionWidgets();
      void arrangeTransformationWidgets();
      void arrangeOptionsWidgets();
      void arrangeAxisWidgets();
      void arrangeTransOptionsWidgets();

   private:
      QPixmap background;
      TSelectionMode currentSelectionMode;
      TTransformationMode currentTransformation;
      bool squareTrans_enabled;
      bool rotate_x_enabled;
      bool rotate_y_enabled;
      bool rotate_z_enabled;
      QRect parentRect;
      bool ignoreWhite;
      bool inverseSelection;
      bool cloneSelection;
      bool combine;
      QToolBar  *selectionBar;
      QGroupBox *shapeBox;
      QAction   *rectangularSelectionButton;
      QAction   *polygonSelectionButton;
      QAction   *freeSelectionButton;
      QToolBar  *transformationBar;
      QGroupBox *transBox;
      QAction   *transMoveButton;
      QAction   *transScaleButton;
      QAction   *transRotateButton;
      QAction   *transShearButton;
      QToolBar  *optionsBar;
      QGroupBox *optionsBox;
      QAction   *ignoreWhiteButton;
      QAction   *inverseButton;
      QAction   *cloneButton;
      QAction   *compositionModeButton;
      QAction   *compositionMenuButton;
      QMenu     *compositionMenu;
      QToolBar  *axisBar;
      QGroupBox *axisBox;
      QAction   *X_axisButton;
      QAction   *Y_axisButton;
      QAction   *Z_axisButton;
      QToolBar  *transOptionsBar;
      QGroupBox *transOptionsBox;
      QAction   *squareTransButton;
   };
}
#endif