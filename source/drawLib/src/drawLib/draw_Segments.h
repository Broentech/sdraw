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
//  \file      draw_Segments.h
//
//  \brief     Declaration of the draw board selection functionality
//
//  \author    Stian Broen
//
//  \date      26.09.2011
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 26.09.2011 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

#ifndef TWOD_DRAWBOARD_SELECTION
#define TWOD_DRAWBOARD_SELECTION

// Qt includes
#include <QHash>
#include <QTime>
#include <QObject>
#include <QPainter>

// solution includes
#include <common/definitions.h>
#include <selectionLib/selectionKeeper.h>
#include <zooming/zoomer.h>

// local includes
#include "draw_Base.h"

// Qt forward declarations
class QMouseEvent;
class QPixmap;
class QPaintDevice;

namespace sdraw {
   class CSegmenter : public CDrawBase
   {
      Q_OBJECT

   public:
      explicit CSegmenter(QObject* parent = 0);
      ~CSegmenter();
      void registerBuffer(QPixmap *buffer);
      void releaseBuffer();
      bool hasRegisteredBuffer();
      void paintSelections(QPaintDevice *buffer = 0);
      virtual void copyActionTriggered(const QPoint &pt, bool);
      virtual void cutActionTriggered(const QPoint &pt, bool);
      virtual void pasteActionTriggered(const QPoint &pt, bool);
      virtual void deleteActionTriggered(const QPoint &pt, bool);
      virtual void cancelActionTriggered(const QPoint &pt, bool);
      virtual void selecterPressEvent(QMouseEvent*);
      virtual void selecterMoveEvent(QMouseEvent*);
      virtual void selecterDoubleclickEvent(QMouseEvent*);
      virtual void selecterReleaseEvent(QMouseEvent*);
      //virtual void paintEvent(QPaintEvent*);

   signals:
      void updateCursorSig(Qt::CursorShape shape);

   protected:
      QPixmap *_copyBuffer;
      TSelectionMode currentSelectionMode;
      TTransformationMode currentTransformationMode;
      TDragArea currentDragArea;
      QPoint dragStart;
      bool transparentSelection;
      bool inverseSelection;
      bool isBorrow;
      bool isStationary;
      bool isDraggingShape;
      bool isTransforming;
      QPolygon polygonSelection;
      QPoint polygonPressPoint;

   public slots:
      void updateSelectionMode(TSelectionMode selection);
      void updateSelectionOption(TSelectionOption selectionOption);
      void updateSelectionCompMode(QPainter::CompositionMode mode);
      void updateSelectionCompAlpha(int value);
      void updateSelectionTransformation(TTransformationMode mode);
      void updateSelectionRotationAxis(Qt::Axis axis, bool enable);
      void updateSelectionTransformationSquare(bool enable);
      //void updateOperation(TOperation operation);
      void performSelectionAction(const QPoint &pt, TSelectionAction selectionAction);
      void performSelectionAction_noPt(TSelectionAction selectionAction);
      void printPixmap(QRect &target, const QPixmap &content, QRect &source, const QString &text, bool doUpdate = true); // debugging

   private:
      void rectangularPress(const QPoint &pressPoint, bool doCopy = true);
      void rectangularMove(const QPoint &currPoint);
      void rectangularRelease(const QPoint &releasePoint);
      void rectangularAction(const QPoint &currPos, TSelectionAction selectionAction);
      void polygonPress(const QPoint &pressPoint);
      void polygonMove(const QPoint &currPoint);
      void polygonDoubleClick(const QPoint &doubleClickPoint);
      void polygonRelease(const QPoint &releasePoint);
      void polygonAction(const QPoint &currPos, TSelectionAction selectionAction);
      void doInitSelectionMovement();
      void doInitSelectionTransformation(QPoint pt);

   private:
      CSelectionKeeper *selectionKeeper;
      QTime mousePressTime;
      QTime mouseReleaseTime;
      int compositionAlpha;
      int dx;
      int dy;
      QPoint pressPoint;
      QPainter::CompositionMode compositionMode;
      bool useComposition;
      bool isDefiningRect;
      bool hasClearedSelections;
      bool squareTrans;
      bool rotate_x_enabled;
      bool rotate_y_enabled;
      bool rotate_z_enabled;
   };
}

#endif