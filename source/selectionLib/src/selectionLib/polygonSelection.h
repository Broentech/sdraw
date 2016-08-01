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
//  \file      polygonSelection.h
//
//  \brief     Definition of a class that defines a polygon selection
//
//  \author    Stian Broen
//
//  \date      08.12.2011
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 08.12.2011 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

#ifndef POLYGON_SELECTION_HEADERFILE
#define POLYGON_SELECTION_HEADERFILE

// Qt includes
#include <QObject>
#include <QPolygon>
#include <QHash>
#include <QRect>
#include <QPen>

// solution includes
#include <common/definitions.h>

// local includes
#include "selectionBase.h"

// Qt forward declarations
class QPixmap;
class QPaintDevice;

namespace sdraw {
   class CPolygonSelection : public CSelectionBase
   {
      Q_OBJECT

   public:
      explicit CPolygonSelection(const QPolygon &polygon, const QPixmap *_content = 0);
      ~CPolygonSelection();
      TPolygonDragArea getAreaAtPoint(const QPoint &pt);
      void updateContent(const QPixmap *newContent, const QRect *newContentRect = 0);
      void storeThisPixmap(const QPixmap *worldPixmap, bool enforce = false);
      void drawContentToWorld(QPaintDevice *targetPixmap);
      void drawSelectionToWorld(QPaintDevice *targetPixmap);
      void dragSelection(int dx, int dy);
      void makeWorldShadow(const QPixmap *worldPixmap);
      void collapseContents();
      bool setMoving(bool onOrOff);
      void toggle();
      inline QPolygon getWorldPolygon(){ return m_world_polygon; }
      inline QPolygon getLocalPolygon(){ return m_local_polygon; }
      void changePen(const QPen *pen, bool redrawContent = false);
      QPen getCurrentPen();
      void moveDragBox(int dx, int dy, const QPixmap *worldPixmap);
      void setDragboxIndex(int index);
      void clearContent();
      void drawIntersectionContent(const QPixmap *outsideContent,
         const QRect &world_outsideContentRect,
         const QRect &local_outsideContentRect,
         QPolygon &interpoly);

   signals:
      void printSig(QRect &target, const QPixmap &content, QRect &source, const QString &text);

   private:
      void makeWorldGeo(const QPolygon &polygon);
      void makeLocalGeo(const QPolygon &polygon);
      QPolygon translateToLocal(const QPolygon &polygon);
      void defineDragboxes();
      void drawSelection(const QPen *pen = 0, bool keepPen = false);
      void drawContent(const QPixmap *source);

   private:
      QRect worldSelectionRect;
      QRect worldContentRect;
      QRect localSelectionRect;
      QRect localContentRect;
      QPolygon m_world_polygon;
      QPolygon m_local_polygon;
      QPen m_pen;
      TPolygonDragArea currentClickArea;
      QHash<int, QRect > world_polygonDragBoxes;
      QHash<int, QRect > local_polygonDragBoxes;
      int activeDragboxIndex;

   
   };
}

#endif