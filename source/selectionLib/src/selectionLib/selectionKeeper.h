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
//  \file      selectionKeeper.h
//
//  \brief     Definition of a class that defines a cluster of selections
//
//  \author    Stian Broen
//
//  \date      30.10.2011
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 30.10.2011 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

#ifndef SELECTION_KEEPER
#define SELECTION_KEEPER

// Qt includes
#include <QList>
#include <QCursor>

// solution includes
#include <common/definitions.h>
#include <zooming/zoomer.h>

// local includes
#include "rectangularSelection.h"
#include "polygonSelection.h"

// Qt forward declarations
class QPixmap;
class QPoint;
class QPaintDevice;

namespace sdraw {
   class CSelectionKeeper : public QObject
   {
      Q_OBJECT

   public:
      explicit CSelectionKeeper(QObject* parent = 0);
      ~CSelectionKeeper();

      // convenience functions
      bool hasRects();        //< see if there are any rectangular selections
      bool hasPolygons();     //< see if there are any polygon selections
      bool hasFocus();        //< see if the mouse is hovering over a rectangular selection
      bool hasPolygonFocus(); //< see if the mouse is hovering over a polygon selection

      // functions performing on both rectangular and polygon selections
      void updateAllSelections();                           //< use the world pixmap to update the content of the selections
      void doTransform(const QPoint &pt);                   //< transform all selections
      void finalizeTransformation();                        //< finalize all selections
      void drawIntersectionsToWorld(QPixmap *targetPixmap); //< draw the selection-intersections back to world pixmap
      void collapseSelections();                            //< collapse all pixmaps in all intersected, inactive selections
      void endTransformation();                             //< save current transformation to pixmap for all selections

      // functions performing on only rectangular selections
      void doTransformRect(const QPoint &pt); //< use the currently set transformation parameters to transform rectangular selections

      // functions performing on only polygon selections
      void doTransformPolygon(const QPoint &pt); //< use the currently set transformation parameters to transform polygon selections

      void setComposition(bool doUse = false, int compositionMode = 0, int alpha = 255); //< set composition parameters
      void drawAllSelections(QPaintDevice *targetPixmap, bool includeRects = true, Zoomer *zoomer = 0, const QString &zoomBufferName = "");
      void drawFocusToArg(QPaintDevice *targetPixmap);
      void addRectangularSelection(const QPoint *start, const QPoint *stop, bool includeWorldContent);
      Qt::CursorShape mouseMovementRect(const QPoint *currPoint);
      void toggleFocusRect();
      Qt::CursorShape getCurrShape();
      TDragArea getFocusDragArea();
      void clearWorldAll(QPaintDevice *targetPixmap, bool force = false);
      void clearWorldAt(QPaintDevice *targetPixmap, bool focus = true, int rectIndex = -1, int polyIndex = -1);
      void clearContentAt(int index, bool clearFocus = false);
      void deleteFocusRect();
      CRectangularSelection *getFocusRect();
      QList <CRectangularSelection*> getAllRects();
      void castShadows(QPaintDevice *targetPixmap);
      void makeWorldShadows();
      void dragBox(int dx, int dy, QPixmap *targetPixmap = 0);
      void dragToggledRectangleSelections(int dx, int dy);
      void considerDrawingPixmap(const QPixmap *pic, QRect &worldRect);
      int getFocusIndex();
      void drawCompositionToContent();
      void setPressPoint(QPoint &pt);
      void setTransformation(TTransformationMode mode, bool square, bool rotate_x, bool rotate_y, bool rotate_z);

      // polygon selection related public functions
      TPolygonDragArea getPressPointArea(const QPoint &pt, bool doUpdateLocal = true);
      TPolygonDragArea getPressPointArea();
      void deleteFocusPolygon();
      void deletePolygon(int index);
      void clearPolygonContentAt(int index, bool clearFocus = false);

      CPolygonSelection *getFocusPolygon();
      inline bool getIsDefiningPolygon() { return isDefiningPolygon; }
      inline bool getIsDraggingPolygonSelection() { return isDraggingPolygonSelection; }
      inline bool getIsDraggingPolygonBox() { return isDraggingPolygonBox; }
      inline bool getIsStatic() { return isStatic; }
      void setIsDefiningPolygon(bool val = true);
      void setIsDraggingPolygonSelection(bool val = true);
      void setIsDraggingPolygonBox(bool val = true);
      void setIsStatic(bool val = true);
      void mouseMovementPolygon(const QPoint *currPoint);
      void setReleasePoint(QPoint pt){ releasePoint = pt; }

      // polygon manipulating public functions
      void addPolygonSelection(const QPolygon &polygon);
      void dragToggledPolygonSelection(int dx, int dy);
      void dragPolygonDragbox(int dx, int dy);
      void toggleFocusPolygon();
      void stopMovingRects();

      void registerBuffer(QPixmap *buffer);
      void releaseBuffer();
      bool getHasBuffer();

   signals:
      void printSig(QRect &target, const QPixmap &content, QRect &source, const QString &text);

   private:


      QList<int> hasIntersectionIndexes_rectangles;
      QList<int> hasIntetsectionIndexes_polygons;

      bool handleTransform(QPoint &start,
         const QPoint &stop,
         QRect &rect,
         QTransform &resTrans,
         QTransform foundation,
         qreal &rotate_x,
         qreal &rotate_y,
         qreal &rotate_z);

      // private polygon related functions
      void handleCompositions_poly(bool drawToContent = false);

      // private rectangular related functions
      void handleCompositions_rect(bool drawToContent = false);
      void deleteRect(int index);

      // private functions relevant to both rectangular and polygon selections
      bool makePixmapComposition(int compositionMode,
         const QRect &worldSelectionRect,
         int compositionAlpha,
         const QPixmap *source,
         QPixmap *destination,
         QPixmap *result);

   private:
      QPixmap *m_buffer;
      bool hasBuffer;
      QList <CPolygonSelection*> allPolygons;
      TPolygonDragArea pressPointArea;
      bool isDefiningPolygon;
      bool isDraggingPolygonSelection;
      bool isDraggingPolygonBox;
      bool isStatic;
      int polygonFocusIndex;
      int dragboxFocusPolygon;
      bool m_doUseComposition;
      int  m_compositionMode;
      int  m_compositionAlpha;
      Qt::CursorShape currShape;
      int rectFocusIndex;
      QList <CRectangularSelection*> allRects;
      QPoint releasePoint;
      QPoint staticPressPoint;
      QPoint pressPoint;
      bool squareTrans;
      bool rotate_x_enabled;
      bool rotate_y_enabled;
      bool rotate_z_enabled;
      qreal rememberLastRotation_x;
      qreal rememberLastRotation_y;
      qreal rememberLastRotation_z;
      TTransformationMode currentTransformationMode;
   };
}

#endif
