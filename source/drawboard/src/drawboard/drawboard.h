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
//  \file      drawboard.h
//
//  \brief     Declaration of the draw board base widget
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
//             02 28.03.2012 - Reorganizing code
//
//  \par       01 26.09.2011 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

#ifndef DRAWBOARD_H
#define DRAWBOARD_H

// Qt includes
#include <QWidget>
#include <QPixmap>
#include <QRect>
#include <QPen>
#include <QtCore/qmath.h>

// solution includes
#include <drawLib/draw_2D.h>
#include <drawLib/draw_Segments.h>
#include <drawCommands/drawboard_pixmapSegment.h>
#include <common/definitions.h>
#include <zooming/zoomer.h>
#include <drawSettings/DrawSettings_2D.h>

// Qt forward declarations
class QResizeEvent;
class QPaintEvent;
class QMouseEvent;
class QKeyEvent;
class QCursor;
class QUndoStack;
class QPrinter;

namespace sdraw {
   class CDrawBoard : public QWidget
   {
      Q_OBJECT

   public:
      explicit CDrawBoard(QWidget* parent = 0,
         const QString &name = QString(tr("Untitled")),
         const QSize &size = QSize(500, 500));
      ~CDrawBoard();
      void initBuffers();
      void leaveEvent(QEvent *);
      virtual void keyPressEvent(QKeyEvent *);
      virtual void keyReleaseEvent(QKeyEvent *);
      QString getMyName();
      QSize getOrigSize();
      void deletePixmapSegment(int index);
      QUndoStack *undoStack() const;

   signals:
      void newSizeSig(const QSize &size);
      void mouseCoordSig(const QPoint &pt);
      void info(const QString &txt);
      void adjustCanvas();
      void selectionActionFromToolbarSig(TSelectionAction selectionAction);
      void selectionOptionFromToolbarSig(TSelectionOption selectionOption);
      void selectionModeFromToolbarSig(TSelectionMode   selection);
      void selectionCompModeFromToolbarSig(QPainter::CompositionMode mode);
      void selectionCompAlphaFromToolbarSig(int value);
      void selectionTransformationFromToolbarSig(TTransformationMode mode);
      void selectionRotationAxisFromToolbarSig(Qt::Axis axis, bool enable);
      void selectionTransformationSquareFromToolbarSig(bool enable);

   public slots:
      void updateSelectionMode(TSelectionMode selection);
      void performSelectionAction(TSelectionAction action);
      void setMouseActive(bool onOrOff);
      void updateOperation(TOperation operation);
      //void updateShape(shapeType shape);
      void updateAngle(qreal val);
      //void updateRoundedRect_X(int radius);
      //void updateRoundedRect_Y(int radius);
      void doEnterInterpSelection();
      void addPixmapSegment(C2DPixmapSegment *segment);
      void receivePixmapSegment(C2DPixmapSegment *segment);
      void updateSlot();
      void updateCursorSlot(Qt::CursorShape shape);
      void slotLoad(const QString &fileName = QString(""), QImage *img = 0);
      void slotSave(const QString &fileName);
      void slotPrintPreview();
      void slotPrint();
      void paintOnPrinter(QPrinter*);
      void updateSettings(CSettings &settings);
      void copyActionTriggered(bool);
      void cutActionTriggered(bool);
      void pasteActionTriggered(bool);
      void deleteActionTriggered(bool);
      void cancelActionTriggered(bool);
      void zoomInActionTriggered(bool);
      void zoomOutActionTriggered(bool);

   private:
      void arrangeKeySequenceActions();
      void drawPixmapSegments(QPaintDevice *device, Zoomer *mZoomer = 0);
      bool event(QEvent *);
      void mousePressEvent(QMouseEvent *);
      void mouseReleaseEvent(QMouseEvent *);
      void mouseMoveEvent(QMouseEvent *);
      void mouseDoubleClickEvent(QMouseEvent *);
      void wheelEvent(QWheelEvent *);
      void paintEvent(QPaintEvent *);

   private:
      bool activeMouse;
      QPixmap _buffer;
      QPixmap _aidBuffer;
      TOperation mOperation;
      CSettings _settings;
      QMap<int, C2DPixmapSegment*> m_segmentHash;
      QUndoStack *m_undoStack;
      QCursor *zoomCursor;
      QCursor *arrowCursor;
      QCursor *openHandCursor;
      Zoomer *mainzoomer;
      Zoomer *aidzoomer;
      bool aidOnly;
      bool aidOn;
      QColor backgroundColor;
      CDraw2D *m_draw;
      CSegmenter *m_segmenter;
      QString myName;
      QSize origSize;
      QPoint dragStart;
      QAction *copyAction;
      QAction *cutAction;
      QAction *pasteAction;
      QAction *deleteAction;
      QAction *cancelAction;
      QAction *zoomInAction;
      QAction *zoomOutAction;
      QPen redDebuggingPen; // TODO : Delete
      QPen blueDebuggingPen; // TODO : Delete
   };
}

#endif