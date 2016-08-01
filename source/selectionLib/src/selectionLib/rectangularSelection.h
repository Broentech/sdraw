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
//  \file      rectangularSelection.h
//
//  \brief     Definition of a class that defines a rectangular selection with dragboxes
//             on each corner, and on each side. The selection may have a QPixmap content
//             drawn inside it.
//
//  \author    Stian Broen
//
//  \date      24.10.2011
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 24.10.2011 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

#ifndef RECTANGULAR_SELECTION_H
#define RECTANGULAR_SELECTION_H

// Qt includes
#include <QRect>
#include <QObject>
#include <QPixmap>
#include <QCursor>
#include <QPen>

// solution includes
#include <common/definitions.h>
#include <zooming/zoomer.h>

// Qt forward declarations
class QTransform;

namespace sdraw {
   class CRectangularSelection : public QObject
   {
      Q_OBJECT

   public:
      explicit CRectangularSelection(const QPoint &upperLeft, const QPoint &bottomRight, QPixmap *_content = 0);
      ~CRectangularSelection();
      void registerBuffer(QPixmap *buffer, const QPoint &upperLeft = QPoint(0, 0), const QPoint &bottomRight = QPoint(0, 0));
      void releaseBuffer();
      bool getHasBuffer();
      bool confirmGeometries();
      Qt::CursorShape getHoverShape(const QPoint &pt);
      void dragSelection(int dx, int dy);
      void dragSelectionBox(int dx, int dy, QPixmap *worldContent = 0);
      QPixmap *getShadowPixmap();
      QPixmap *getFullPixmap();
      QPixmap *getContentPixmap();
      QRect getWorldFullRect();
      QRect getWorldContentRect();
      QRect getLocalFullRect();
      QRect getLocalContentRect();
      QPen  getCurrentPen();
      void storeThisPixmap(const QPixmap *worldPixmap, bool enforce = false);
      QPixmap* getExtraPixmap(){ return extraPixmap; }
      void makeWorldShadow(const QPixmap *worldPixmap);
      void updateContent(const QPixmap *newContent, const QRect *newContentRect = 0);
      void clearContent();
      void collapseContents();
      void drawContentToWorld(
         QPaintDevice *targetPixmap,
         Zoomer *zoomer = 0,
         const QString &zoomBufferName = "");
      void drawSelectionToWorld(
         QPaintDevice *targetPixmap,
         Zoomer *zoomer = 0,
         const QString &zoomBufferName = "");
      void drawIntersectionContent(
         const QPixmap *outsideContent,
         const QRect &world_outsideContentRect,
         const QRect &local_outsideContentRect);
      void changePen(const QPen *pen, bool redrawContent = false);
      void toggle();
      bool getToggle();
      bool getHasMoved();
      bool getHasExtraContent();
      TDragArea getCurrentDragArea();
      void deleteTransforms();
      inline void setTransform(QTransform trans){ transform = trans; }
      QTransform getTransform();
      QTransform getLastTransform();
      void setLastTransform(QTransform trans);
      inline qreal getAccumRotation_x(){ return accumRotation_x; }
      inline qreal getAccumRotation_y(){ return accumRotation_y; }
      inline qreal getAccumRotation_z(){ return accumRotation_z; }
      inline void setAccumRotation_x(qreal num){ accumRotation_x = num; }
      inline void setAccumRotation_y(qreal num){ accumRotation_y = num; }
      inline void setAccumRotation_z(qreal num){ accumRotation_z = num; }

   signals:
      void printSig(QRect &target, const QPixmap &content, QRect &source, const QString &text);

   private:
      void drawSelection(const QPen *pen = 0, bool keepPen = false);
      void defineWorldRects();
      void defineLocalRects();

   private:
      TSelection bufferSel;
      TSelection localSel;
      QPoint outsideUpperLeft;
      QPoint outsideBottomRight;
      QRect rectangularSelectionErase;
      QRect rectangularSelection;
      QRect contentRect;
      QRect upperLeftCornerDragArea;
      QRect upperMidDragArea;
      QRect upperRightDragArea;
      QRect midLeftDragArea;
      QRect midRightDragArea;
      QRect bottomLeftDragArea;
      QRect bottomMidDragArea;
      QRect bottomRightDragArea;
      QPoint mUpperLeft;
      QPoint mBottomRight;
      QRect local_rectangularSelectionErase;
      QRect local_rectangularSelection;
      QRect local_contentRect;
      QRect local_upperLeftCornerDragArea;
      QRect local_upperMidDragArea;
      QRect local_upperRightDragArea;
      QRect local_midLeftDragArea;
      QRect local_midRightDragArea;
      QRect local_bottomLeftDragArea;
      QRect local_bottomMidDragArea;
      QRect local_bottomRightDragArea;
      qreal accumRotation_x;
      qreal accumRotation_y;
      qreal accumRotation_z;
      bool hasBuffer;
      QPixmap *m_buffer;
      bool isToggled;
      bool hasShadow;
      bool writeExtraInstead;
      TDragArea currentDragArea;
      QPixmap *bottomPixmap;
      QPixmap *content;
      QPixmap *topContent;
      QPixmap *worldShadow;
      QPixmap *extraPixmap;
      QPen m_pen;
      QTransform transform;
      QTransform lastTransform;
      QSize contentSize;
      QSize totalSize;

   };
}

#endif