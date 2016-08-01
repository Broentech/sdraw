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
//  \file      selectionKeeper.cpp
//
//  \brief     Implementation of selection keeper class
//
//  \author    Stian Broen
//
//  \date      30.10.2011
//
//  \par       Copyright:
//
//             Copyright by Stian Broen
//
//  \par       Revision History
//
//  \par       01 30.10.2011 Original version       
//
//
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QPixmap>
#include <QPoint>
#include <QPainter>
#include <QBitmap>

// solution includes
#include <zooming/zoomer.h>

// local includes
#include "selectionKeeper.h"

using namespace sdraw;

CSelectionKeeper::CSelectionKeeper(QObject* parent)
   :QObject(parent),
   hasBuffer(false),
   isDefiningPolygon(false),
   isDraggingPolygonSelection(false),
   isDraggingPolygonBox(false),
   isStatic(true),
   pressPointArea(POLY_NOT_DEFINED),
   dragboxFocusPolygon(-1),
   polygonFocusIndex(-1),
   rectFocusIndex(-1),
   rememberLastRotation_x(0),
   rememberLastRotation_y(0),
   rememberLastRotation_z(0),
   m_buffer(0),
   currShape(Qt::ArrowCursor)
{

}

CSelectionKeeper::~CSelectionKeeper()
{
   for (int i = 0; i < allRects.size(); i++)
   {
      CRectangularSelection *deleteMe = allRects.at(i);
      delete deleteMe;
      deleteMe = 0;
   }
   allRects.clear();
}

void CSelectionKeeper::drawAllSelections(QPaintDevice *targetPixmap,
   bool includeRects,
   Zoomer *zoomer,
   const QString &zoomBufferName)
{
   if (0 == targetPixmap || (allRects.size() == 0 && allPolygons.size() == 0))
   {
      return;
   }

   // draw all rectangular selections
   for (int i = 0; i < allRects.size(); i++)
   {
      if (i == rectFocusIndex && allRects.at(rectFocusIndex)->getToggle())
      {
         continue;
      }

      CRectangularSelection *currRect = allRects.at(i);
      if (0 == currRect)
      {
         continue;
      }

      if (includeRects)
      {
         currRect->drawSelectionToWorld(targetPixmap, zoomer, zoomBufferName);
      }
      else
      {
         currRect->drawContentToWorld(targetPixmap, zoomer, zoomBufferName);
      }
   }

   if (hasFocus())
   {
      CRectangularSelection *currRect = allRects.at(rectFocusIndex);
      if (currRect->getToggle())
      {
         if (includeRects)
         {
            currRect->drawSelectionToWorld(targetPixmap, zoomer, zoomBufferName);
         }
         else
         {
            currRect->drawContentToWorld(targetPixmap, zoomer, zoomBufferName);
         }
      }
   }

   // draw all polygons
   CPolygonSelection *currPoly = 0;
   for (int i = 0; i < allPolygons.size(); i++)
   {
      currPoly = allPolygons.at(i);
      if (0 == currPoly)
      {
         continue;
      }
      if (includeRects)
      {
         currPoly->drawSelectionToWorld(targetPixmap);
      }
      else
      {
         currPoly->drawContentToWorld(targetPixmap);
      }
   }
   currPoly = 0;
}

void CSelectionKeeper::drawFocusToArg(QPaintDevice *targetPixmap)
{
   if (0 == targetPixmap)
   {
      return;
   }

   if (hasFocus())
   {
      // draw focus rectangle content to world
      CRectangularSelection *focus = allRects.at(rectFocusIndex);
      if (0 != focus)
      {
         focus->drawContentToWorld(targetPixmap);
      }
      focus = 0;
   }

   if (hasPolygonFocus())
   {
      // draw focus polygon content to world
      CPolygonSelection *focusPoly = allPolygons.at(polygonFocusIndex);
      if (0 != focusPoly)
      {
         focusPoly->drawContentToWorld(targetPixmap);
      }
      focusPoly = 0;
   }
}

void CSelectionKeeper::clearWorldAll(QPaintDevice *targetPixmap, bool force)
{
   if (0 == targetPixmap)
   {
      return;
   }

   CRectangularSelection *currRect = 0;
   QPainter painter(targetPixmap);
   for (int i = 0; i < allRects.size(); i++)
   {
      currRect = allRects.at(i);
      if (0 == currRect)
      {
         continue;
      }

      if (currRect->getHasExtraContent() && !force)
      {
         // do not clear out the world content IF the selection has "extra" content
         continue;
      }
      painter.fillRect(currRect->getWorldContentRect(), Qt::white);
   }
   currRect = 0;

   CPolygonSelection *currPoly = 0;
   for (int i = 0; i < allPolygons.size(); i++)
   {
      currPoly = allPolygons.at(i);
      if (0 == currPoly)
      {
         continue;
      }
      if (currPoly->getHasExtraContent() && !force)
      {
         // do not clear out the world content IF the selection has "extra" content
         continue;
      }

      // whiteout
      QPainterPath path;
      path.addPolygon(currPoly->getWorldPolygon());
      painter.setClipPath(path);
      painter.fillRect(currPoly->getWorldPolygon().boundingRect(), Qt::white);
   }
   currPoly = 0;

   painter.end();
}

void CSelectionKeeper::clearWorldAt(QPaintDevice *targetPixmap, bool focus, int rectIndex, int polyIndex)
{
   if (0 == targetPixmap)
   {
      return;
   }
   if (focus)
   {
      QPainter painter(targetPixmap);
      if (hasFocus())
      {
         CRectangularSelection *focusRect = allRects.at(rectFocusIndex);
         if (0 != focusRect)
         {
            painter.fillRect(focusRect->getWorldContentRect(), Qt::white);
         }
         focusRect = 0;
      }
      if (hasPolygonFocus())
      {
         CPolygonSelection *focusPoly = allPolygons.at(polygonFocusIndex);
         if (0 != focusPoly)
         {
            QPainterPath path;
            path.addPolygon(focusPoly->getWorldPolygon());
            painter.setClipPath(path);
            painter.fillRect(focusPoly->getWorldPolygon().boundingRect(), Qt::white);
         }
         focusPoly = 0;
      }
      painter.end();
   }
   else
   {
      if (!(rectIndex < 0 || rectIndex >= allRects.size()))
      {
         CRectangularSelection *indexRect = allRects.at(rectIndex);
         if (0 != indexRect)
         {
            QPainter painter(targetPixmap);
            painter.fillRect(indexRect->getWorldContentRect(), Qt::white);
            painter.end();
         }
      }
      if (!(polyIndex < 0 || polyIndex >= allPolygons.size()))
      {
         CPolygonSelection *indexPoly = allPolygons.at(polyIndex);
         if (0 != indexPoly)
         {
            QPainter painter(targetPixmap);
            QPainterPath path;
            path.addPolygon(indexPoly->getWorldPolygon());
            painter.setClipPath(path);
            painter.fillRect(indexPoly->getWorldPolygon().boundingRect(), Qt::white);
            painter.end();
         }
      }
   }
}

void CSelectionKeeper::clearContentAt(int index, bool clearFocus)
{
   int clearIndex = -1;
   if (clearFocus)
   {
      if (hasFocus())
      {
         clearIndex = rectFocusIndex;
      }
      else
      {
         return;
      }
   }
   else
   {
      clearIndex = index;
   }
   if (clearIndex >= 0 && clearIndex < allRects.size())
   {
      CRectangularSelection *clearThis = allRects.at(clearIndex);
      clearThis->clearContent();
      clearThis = 0;
   }
}

void CSelectionKeeper::clearPolygonContentAt(int index, bool clearFocus)
{
   int clearIndex = -1;
   if (clearFocus)
   {
      if (hasPolygonFocus())
      {
         clearIndex = polygonFocusIndex;
      }
      else
      {
         return;
      }
   }
   else
   {
      clearIndex = index;
   }
   if (clearIndex >= 0 && clearIndex < allPolygons.size())
   {
      CPolygonSelection *clearThis = allPolygons.at(clearIndex);
      clearThis->clearContent();
      clearThis = 0;
   }
}

void CSelectionKeeper::deleteFocusRect()
{
   deleteRect(rectFocusIndex);
   rectFocusIndex = -1;
}

void CSelectionKeeper::deleteRect(int index)
{
   if (index < 0 || index >= allRects.size())
   {
      return;
   }
   delete allRects.at(index);
   allRects.removeAt(index);
}

void CSelectionKeeper::deleteFocusPolygon()
{
   if(hasPolygonFocus())
   {
      deletePolygon(polygonFocusIndex);
      polygonFocusIndex = -1;
   }
}

void CSelectionKeeper::deletePolygon(int index)
{
   if(index < 0 || index >= allPolygons.size() || 0 == allPolygons.at(index))
   {
      return;
   }
   delete allPolygons.at(index);
   allPolygons.removeAt(index);
}

QList <CRectangularSelection*> CSelectionKeeper::getAllRects()
{
   return allRects;
}

void CSelectionKeeper::addRectangularSelection(const QPoint *start, const QPoint *stop, bool includeWorldContent)
{
   if (0 == start || 0 == stop)
   {
      return;
   }

   CRectangularSelection *addRectSel = 0;
   if (includeWorldContent && getHasBuffer())
   {
      addRectSel = new CRectangularSelection(*start, *stop, m_buffer);
   }
   else
   {
      addRectSel = new CRectangularSelection(*start, *stop, 0);
   }

   if (0 != addRectSel)
   {
      connect(addRectSel, SIGNAL(printSig(QRect &, const QPixmap &, QRect &, const QString &)),
         this, SIGNAL(printSig(QRect &, const QPixmap &, QRect &, const QString &)));

      allRects << addRectSel;
   }
}

//  Call this function on mouse movement events to set focus selection and determine
//  the shape of the mouse cursor at the argument position
Qt::CursorShape CSelectionKeeper::mouseMovementRect(const QPoint *currPoint)
{
   rectFocusIndex = -1;
   if (0 == currPoint)
   {
      return Qt::ArrowCursor;
   }

   // find rects that are in focus
   QList<int> rectsInFocus;
   int noAllRects = allRects.size();
   for (int i = 0; i < noAllRects; i++)
   {
      CRectangularSelection *checkThisFocus = allRects.at(i);
      QRect checkRect = checkThisFocus->getWorldFullRect();
      if (checkRect.contains(*currPoint))
      {
         rectsInFocus << i;
      }
      checkThisFocus = 0;
   }

   // determine focus priority
   QList<int> excludeRect;
   int noRectsInFocus = rectsInFocus.size();
   for (int j = 0; j < noRectsInFocus; j++)
   {
      int currentIndex = rectsInFocus.at(j);
      CRectangularSelection *checkThisPrio = allRects.at(currentIndex);
      QRect checkRect = checkThisPrio->getWorldFullRect();
      for (int k = 0; k < noAllRects; k++)
      {
         int currentCompareIndex = k;
         if (currentIndex == currentCompareIndex)
         {
            continue;
         }
         CRectangularSelection *compareThisPrio = allRects.at(currentCompareIndex);
         QRect compareRect = compareThisPrio->getWorldFullRect();
         if (compareRect.contains(checkRect) || !rectsInFocus.contains(currentCompareIndex))
         {
            // if the check rectangle is inside the compare rect, then exclude compare rect
            excludeRect << currentCompareIndex;
         }
         compareThisPrio = 0;
      }
   }

   // get prioritized selection
   CRectangularSelection *tmpSel = 0;
   for (int l = 0; l < rectsInFocus.size(); l++)
   {
      int currIndex = rectsInFocus.at(l);
      if (excludeRect.contains(currIndex))
         continue;

      rectFocusIndex = currIndex;
      tmpSel = allRects.at(currIndex);
      currShape = tmpSel->getHoverShape(*currPoint);
   }

   // change color of the selection based on focus
   if (0 != tmpSel)
   {
      QPen pen;
      pen.setWidth(2);
      pen.setStyle(Qt::DashLine);
      pen.setColor(tmpSel->getCurrentPen().color());

      // change color of the selection in focus
      tmpSel->changePen(&pen, true);
   }

   // change color of the selections without focus
   for (int x = 0; x < allRects.size(); x++)
   {
      if (x == rectFocusIndex)
         continue;

      CRectangularSelection *tmp = allRects.at(x);
      if (0 != tmp)
      {
         QPen pen;
         pen.setWidth(1);
         pen.setStyle(Qt::DashLine);
         if (!tmp->getToggle())
         {
            pen.setColor(Qt::lightGray);
            tmp->changePen(&pen, true);
         }
         else
         {
            pen.setColor(Qt::blue);
            tmp->changePen(&pen, true);
         }
      }
      tmp = 0;
   }
   tmpSel = 0;
   if (rectsInFocus.size() == 0)
   {
      return Qt::ArrowCursor;
   }
   return currShape;
}

void CSelectionKeeper::toggleFocusRect()
{
   if (!(rectFocusIndex < allRects.size() && rectFocusIndex >= 0))
   {
      // invalid focus rect, nothing to do
      return;
   }

   CRectangularSelection *focusRect = allRects.at(rectFocusIndex);
   if (0 != focusRect)
   {
      focusRect->toggle();
   }
   focusRect = 0;
}

bool CSelectionKeeper::hasFocus()
{
   if (allRects.size() == 0 ||
      rectFocusIndex < 0 ||
      rectFocusIndex >= allRects.size())
   {
      return false;
   }
   else
   {
      return true;
   }
}

bool CSelectionKeeper::hasPolygonFocus()
{
   if (allPolygons.size() == 0 ||
      polygonFocusIndex < 0 ||
      polygonFocusIndex >= allPolygons.size())
   {
      return false;
   }
   return true;
}

Qt::CursorShape CSelectionKeeper::getCurrShape()
{
   return currShape;
}

TDragArea CSelectionKeeper::getFocusDragArea()
{
   if (hasFocus())
   {
      return allRects.at(rectFocusIndex)->getCurrentDragArea();
   }
   else
   {
      return NO_DRAG;
   }
}

void CSelectionKeeper::dragToggledRectangleSelections(int dx, int dy)
{
   CRectangularSelection *tmpSel = 0;

   // first drag all toggled selections
   for (int i = 0; i < allRects.size(); i++)
   {
      tmpSel = allRects.at(i);

      if (0 != tmpSel && tmpSel->getToggle())
      {
         tmpSel->dragSelection(dx, dy);
      }
   }
   tmpSel = 0;

   // make any compositions with the world pixmap
   handleCompositions_rect();

   // then draw any intersections
   for (int i = 0; i < allRects.size(); i++)
   {
      tmpSel = allRects.at(i);
      if (0 != tmpSel)
      {
         for (int j = 0; j < allRects.size(); j++)
         {
            if (i == j)
            {
               continue;
            }

            CRectangularSelection *compareSel = allRects.at(j);
            if (0 != compareSel)
            {
               if (!tmpSel->getWorldContentRect().intersects(compareSel->getWorldContentRect()))
               {
                  continue;
               }

               if (!hasIntersectionIndexes_rectangles.contains(j))
               {
                  hasIntersectionIndexes_rectangles << j;
               }
               compareSel->drawIntersectionContent(tmpSel->getContentPixmap(),
                  tmpSel->getWorldContentRect(),
                  tmpSel->getLocalContentRect());
            }
            compareSel = 0;
         }
      }
      tmpSel = 0;
   }
}

void CSelectionKeeper::dragToggledPolygonSelection(int dx, int dy)
{
   CPolygonSelection *currPoly = 0;
   for (int i = 0; i < allPolygons.size(); i++)
   {
      currPoly = allPolygons.at(i);
      if (0 == currPoly)
      {
         continue;
      }
      currPoly->dragSelection(dx, dy);

   }
   currPoly = 0;

   // make any compositions with the world pixmap
   handleCompositions_poly();

   // then draw any intersections
   for (int i = 0; i < allPolygons.size(); i++)
   {
      currPoly = allPolygons.at(i);
      if (0 == currPoly)
      {
         continue;
      }

      for (int j = 0; j < allPolygons.size(); j++)
      {
         if (i == j)
         {
            continue;
         }

         CPolygonSelection *compareSel = allPolygons.at(j);
         if (0 == compareSel)
         {
            continue;
         }
         QPolygon intersection = currPoly->getWorldPolygon().intersected(compareSel->getWorldPolygon());
         if (intersection.size() < 3)
         {
            continue;
         }

         if (!hasIntetsectionIndexes_polygons.contains(j))
         {
            hasIntetsectionIndexes_polygons << j;
         }
         compareSel->drawIntersectionContent(currPoly->getContentPixmap(),
            currPoly->getWorldPolygon().boundingRect(),
            currPoly->getLocalPolygon().boundingRect(),
            intersection);

         compareSel = 0;
      }

      currPoly = 0;
   }
}

void CSelectionKeeper::handleCompositions_rect(bool drawToContent)
{
   CRectangularSelection *tmpSel = 0;

   if (m_doUseComposition)
   {
      // make composition selection pixmaps
      for (int i = 0; i < allRects.size(); i++)
      {
         tmpSel = allRects.at(i);

         if (0 != tmpSel && tmpSel->getToggle())
         {
            // make a temporary result pixmap
            QPixmap *destinationPt = tmpSel->getContentPixmap();
            QPixmap *compositionResult = new QPixmap(tmpSel->getContentPixmap()->rect().size());
            QRect worldSel = tmpSel->getWorldContentRect();

            // perform merging
            if (makePixmapComposition(m_compositionMode,  // merge using this composition mode
               worldSel,  // take the source content defined by this rectangle
               m_compositionAlpha,  // transparency
               m_buffer,  // source/background pixmap
               destinationPt,  // destination/top pixmap
               compositionResult)) // pixmap to write results to
            {
               if (0 != compositionResult)
               {
                  // successfull merging
                  if (drawToContent)
                  {
                     tmpSel->updateContent(compositionResult);
                  }
                  else
                  {
                     tmpSel->storeThisPixmap(compositionResult, true);
                  }
               }
            }

            // remove temporary result pixmap
            delete compositionResult;
            compositionResult = 0;
            destinationPt = 0;
         }
      }
   }
   tmpSel = 0;
}

void CSelectionKeeper::handleCompositions_poly(bool drawToContent)
{
   CPolygonSelection *tmpSel = 0;
   QPixmap *destinationPt = 0;
   QPixmap *compositionResult = 0;

   if (m_doUseComposition)
   {
      // make composition selection pixmaps
      for (int i = 0; i < allPolygons.size(); i++)
      {
         tmpSel = allPolygons.at(i);
         if (0 == tmpSel)
         {
            continue;
         }
         ESelectionStates selState = tmpSel->getCurrentState();
         if (selState == SEL_STATE_PASSIVE)
         {
            continue;
         }

         // make a temporary result pixmap
         destinationPt = tmpSel->getContentPixmap();
         compositionResult = new QPixmap(tmpSel->getContentPixmap()->rect().size());
         if (0 == compositionResult)
         {
            continue;
         }
         QRect worldSel = tmpSel->getWorldPolygon().boundingRect();

         // perform merging
         if (makePixmapComposition(m_compositionMode,  // merge using this composition mode
            worldSel,  // take the source content defined by this rectangle
            m_compositionAlpha,  // transparency
            m_buffer,  // source/background pixmap
            destinationPt,  // destination/top pixmap
            compositionResult)) // pixmap to write results to
         {
            if (0 != compositionResult)
            {
               // successfull merging
               if (drawToContent)
               {
                  // note : this functions will do clipping based on the polygon
                  tmpSel->updateContent(compositionResult);
               }
               else
               {
                  // note : this function doesnt do clipping, so do it here instead
                  QPixmap *compositionResult_clipped = new QPixmap(compositionResult->size());
                  if (0 == compositionResult_clipped)
                  {
                     continue;
                  }
                  QPainter scissor(compositionResult_clipped);
                  QPainterPath path;
                  path.addPolygon(tmpSel->getLocalPolygon());
                  scissor.setClipPath(path);
                  scissor.drawPixmap(compositionResult_clipped->rect(), *compositionResult, compositionResult->rect());
                  scissor.end();

                  // now it has been clipped, so store it in the selection
                  tmpSel->storeThisPixmap(compositionResult_clipped, true);
                  delete compositionResult_clipped;
               }
            }
         }

         // remove temporary result pixmap
         if (0 != compositionResult)
         {
            delete compositionResult;
         }
         compositionResult = 0;
         destinationPt = 0;

      }
   }
   tmpSel = 0;
}

void CSelectionKeeper::drawCompositionToContent()
{
   handleCompositions_rect(true);
}

void CSelectionKeeper::drawIntersectionsToWorld(QPixmap *targetPixmap)
{
   if(0 == targetPixmap)
   {
      return;
   }
   
   QPainter painter(targetPixmap);

   // draw rectangular intersections to world
   CRectangularSelection *tmpRect = 0;
   int noIndexes = hasIntersectionIndexes_rectangles.size();
   for(int i=0; i<noIndexes; i++)
   {
      int currIndex = hasIntersectionIndexes_rectangles.at(i);
      if(currIndex >= allRects.size())
      {
         continue;
      }
      tmpRect = allRects.at(currIndex);
      if(0 == tmpRect)
      {
         continue;
      }
      QPixmap *drawThis = tmpRect->getContentPixmap();
      if(0 == drawThis)
      {
         continue;
      }
      painter.drawPixmap(tmpRect->getWorldContentRect(), *drawThis, drawThis->rect());
   }
   tmpRect = 0;

   // draw polygon intersections to world
   CPolygonSelection *tmpPoly = 0;
   noIndexes = hasIntersectionIndexes_rectangles.size();
   for(int i=0; i<noIndexes; i++)
   {
      int currIndex = hasIntersectionIndexes_rectangles.at(i);
      if(currIndex >= allPolygons.size())
      {
         continue;
      }
      tmpPoly = allPolygons.at(currIndex);
      if(0 == tmpPoly)
      {
         continue;
      }
      QPixmap *drawThis = tmpPoly->getContentPixmap();
      if(0 == drawThis)
      {
         continue;
      }
      QPainterPath path;
      path.addPolygon(tmpPoly->getWorldPolygon());
      painter.setClipPath(path);
      painter.drawPixmap(tmpPoly->getWorldPolygon().boundingRect(), *drawThis, drawThis->rect());
   }
   tmpPoly = 0;
   
   painter.end();
   
}

void CSelectionKeeper::collapseSelections()
{
   // collapse rectangles
   CRectangularSelection *toggledSel = 0;
   CRectangularSelection *stillSel   = 0;
   for(int i=0; i<allRects.size(); i++)
   {
      toggledSel = allRects.at(i);
      if(0 != toggledSel && toggledSel->getToggle())
      {
         for(int j=0; j<allRects.size(); j++)
         {
            if(i==j)
            {
               continue;
            }
            stillSel = allRects.at(j);
            if(0 == stillSel)
            {
               continue;
            }
            QRect toggledRect = toggledSel->getWorldContentRect();
            QRect stillRect   = stillSel->getWorldContentRect();
            if(toggledRect.intersects(stillRect))
            {
               stillSel->collapseContents();
            }
         }
      }
   }
   toggledSel = 0;
   stillSel   = 0;

   // collapse polygons
   CPolygonSelection *toggledPoly = 0;
   CPolygonSelection *stillPoly   = 0;
   for(int i=0; i<allPolygons.size(); i++)
   {
      toggledPoly = allPolygons.at(i);
      if(0 != toggledPoly && toggledPoly->getCurrentState() != SEL_STATE_PASSIVE)
      {
         for(int j=0; j<allPolygons.size(); j++)
         {
            if(i==j)
            {
               continue;
            }
            stillPoly = allPolygons.at(j);
            if(0 == stillPoly || stillPoly->getCurrentState() != SEL_STATE_PASSIVE)
            {
               continue;
            }
            QPolygon toggledPolygon = toggledPoly->getWorldPolygon();
            QPolygon stillPolygon   = stillPoly->getWorldPolygon();
            QPolygon intersectedPol = toggledPolygon.intersected(stillPolygon);
            if(intersectedPol.size() > 0)
            {
               stillPoly->collapseContents();
            }
         }
      }
   }
   toggledPoly = 0;
   stillPoly   = 0;
}

void CSelectionKeeper::makeWorldShadows()
{
   CRectangularSelection *tmpRectSel = 0;
   for (int i = 0; i < allRects.size(); i++)
   {
      tmpRectSel = allRects.at(i);
      if (0 != tmpRectSel && getHasBuffer())
      {
         tmpRectSel->makeWorldShadow(m_buffer);
      }
   }
   tmpRectSel = 0;

   CPolygonSelection *tmpPolySel = 0;
   for (int i = 0; i < allPolygons.size(); i++)
   {
      tmpPolySel = allPolygons.at(i);
      if (0 != tmpPolySel && getHasBuffer())
      {
         tmpPolySel->makeWorldShadow(m_buffer);
      }
   }

   tmpPolySel = 0;
}

void CSelectionKeeper::castShadows(QPaintDevice *targetPixmap)
{
   if (0 == targetPixmap)
   {
      return;
   }

   CRectangularSelection *currSel = 0;
   QPainter shadowPainter(targetPixmap);
   for (int i = 0; i < allRects.size(); i++)
   {
      if (hasIntersectionIndexes_rectangles.contains(i))
      {
         continue;
      }
      currSel = allRects.at(i);
      if (0 != currSel)
      {
         QPixmap *shadow = currSel->getShadowPixmap();
         if (0 != shadow)
         {
            shadowPainter.drawPixmap(currSel->getWorldContentRect(), *shadow, shadow->rect());
         }
         shadow = 0;
      }
   }
   hasIntersectionIndexes_rectangles.clear();
   currSel = 0;

   CPolygonSelection *currPoly = 0;
   for (int i = 0; i < allPolygons.size(); i++)
   {
      if (hasIntetsectionIndexes_polygons.contains(i))
      {
         continue;
      }
      currPoly = allPolygons.at(i);
      if (0 == currPoly)
      {
         continue;
      }
      QPixmap *shadow = currPoly->getShadowPixmap();
      if (0 == shadow)
      {
         continue;
      }
      QPainterPath path;
      path.addPolygon(currPoly->getWorldPolygon());
      shadowPainter.setClipPath(path);
      shadowPainter.drawPixmap(currPoly->getWorldPolygon().boundingRect(), *shadow, shadow->rect());
      shadow = 0;

   }
   currPoly = 0;

   shadowPainter.end();
}

void CSelectionKeeper::dragBox(int dx, int dy, QPixmap *targetPixmap)
{
   if (hasFocus())
   {
      CRectangularSelection *focusRect = allRects.at(rectFocusIndex);
      if (0 != focusRect)
      {
         focusRect->dragSelectionBox(dx, dy, targetPixmap);
      }
   }
}

CRectangularSelection *CSelectionKeeper::getFocusRect()
{
   CRectangularSelection *returnRect = 0;
   if (hasFocus())
   {
      returnRect = allRects.at(rectFocusIndex);
   }
   return returnRect;
}

//  Given the argument rectangle, see if the argument pixmap, or any part of the
//  pixap should be written to any of the selections
void CSelectionKeeper::considerDrawingPixmap(const QPixmap *pic, QRect &worldRect)
{
   if (0 == pic || worldRect.isNull() || worldRect.isEmpty())
   {
      return;
   }

   CRectangularSelection *tmpSel = 0;
   for (int i = 0; i < allRects.size(); i++)
   {
      tmpSel = allRects.at(i);
      if (0 == tmpSel)
      {
         continue;
      }
      QRect checkRect = tmpSel->getWorldContentRect();
      if (worldRect.intersects(checkRect))
      {
         tmpSel->drawIntersectionContent(pic, worldRect, pic->rect());
         tmpSel->collapseContents();
      }
   }
   tmpSel = 0;
}

void CSelectionKeeper::setComposition(bool doUse, int compositionMode, int alpha)
{
   // TODO : check the arguments before updating class members
   m_doUseComposition = doUse;
   m_compositionMode = compositionMode;
   m_compositionAlpha = alpha;
}

//  given the argument composition mode and alpha, make a combination of the argument 
//  source at the world selection rectangle and the destination. Read "source" as "what
//  was there before" and "destination" as "what was brought on top of the source".
bool CSelectionKeeper::makePixmapComposition(int compositionMode,
   const QRect &worldSelectionRect,
   int compositionAlpha,
   const QPixmap *source,
   QPixmap *destination,
   QPixmap *result)
{
   // check that coomposition mode is valid
   if ((int)compositionMode < 0 || (int)compositionMode > 32)
   {
      // undefined composition mode
      return false;
   }

   // check parameter values
   if (0 == source ||
      0 == destination ||
      0 == result ||
      worldSelectionRect.isNull() ||
      worldSelectionRect.isEmpty())
   {
      // invalid pixmap parameters
      return false;
   }

   // check for valid gemoetry : the rectangular size of the result pixmap and destination pixmap must be the same as the size of the
   // world selection rect
   if (worldSelectionRect.size() != destination->rect().size() ||
      worldSelectionRect.size() != result->rect().size())
   {
      // the geometry of the arguments doesnt match, proceeding would have led to unwanted results
      return false;
   }

   QSize worldSelectionSize = worldSelectionRect.size();

   // write source content at argument world coordinates to temp buffer
   QPixmap *m_source = new QPixmap(worldSelectionSize);
   QRect m_sourceRect = m_source->rect();
   QSize m_sourceSize = m_sourceRect.size();
   QPainter sourcePainter(m_source);
   sourcePainter.setCompositionMode(QPainter::CompositionMode_Source);
   sourcePainter.fillRect(m_sourceRect, Qt::transparent);
   sourcePainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
   sourcePainter.drawPixmap(m_sourceRect, *source, worldSelectionRect);
   sourcePainter.end();

   // debugging
   //QRect debugRect0 = m_sourceRect;
   //debugRect0.adjust(5, 5, 5, 5);
   //emit printSig( debugRect0, 
   //              *m_source, 
   //               m_sourceRect, 
   //               QString("SOURCE"));

   // create mask to make white pixels transparent
   QBitmap bmap = m_source->createMaskFromColor(Qt::white, Qt::MaskInColor);
   m_source->setMask(bmap);

   QBitmap bmap2 = destination->createMaskFromColor(Qt::white, Qt::MaskInColor);
   destination->setMask(bmap2);

   // debugging
   //QRect debugRect1 = m_sourceRect;
   //debugRect1.adjust(5,m_sourceSize.height() + 5, 5, m_sourceSize.height() + 5);
   //emit printSig( debugRect1, 
   //              *destination, 
   //               destination->rect(),
   //               QString("DESTINATION"));

   // make composition merged pixmap of the 2 temporary pixmaps
   QPainter resultPainter(result);
   resultPainter.setCompositionMode(QPainter::CompositionMode_Source);
   resultPainter.fillRect(result->rect(), Qt::transparent);
   resultPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
   resultPainter.drawPixmap(0, 0, *m_source);

   resultPainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
   resultPainter.fillRect(result->rect(), QColor(0, 0, 0, compositionAlpha));

   QPainter::CompositionMode m_compo = (QPainter::CompositionMode)compositionMode;
   resultPainter.setCompositionMode(m_compo);
   resultPainter.drawPixmap(0, 0, *destination);
   resultPainter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
   resultPainter.fillRect(result->rect(), Qt::white);
   resultPainter.end();

   //QRect debugRect2 = m_sourceRect;
   //debugRect2.adjust(5,2*m_sourceSize.height() +  5, 5, 2*m_sourceSize.height() + 5);
   //emit printSig( debugRect2, 
   //              *result, 
   //               result->rect(),
   //               QString("RESULT"));

   return true;
}

int CSelectionKeeper::getFocusIndex()
{
   return rectFocusIndex;
}

void CSelectionKeeper::updateAllSelections()
{
   if (!getHasBuffer())
   {
      return;
   }
   CRectangularSelection *currSel = 0;
   for (int i = 0; i < allRects.size(); i++)
   {
      currSel = allRects.at(i);
      if (0 == currSel)
      {
         continue;
      }
      currSel->updateContent(m_buffer, &currSel->getWorldContentRect());
   }
   currSel = 0;

   CPolygonSelection *currPoly = 0;
   for (int i = 0; i < allPolygons.size(); i++)
   {
      currPoly = allPolygons.at(i);
      if (0 == currPoly)
      {
         continue;
      }
      currPoly->updateContent(m_buffer, &currPoly->getWorldPolygon().boundingRect());
   }
   currPoly = 0;
}

void CSelectionKeeper::setPressPoint(QPoint &pt)
{
   staticPressPoint = pt; // this point doesnt change when dragging, used to determine no-movement mouse release
   pressPoint = pt;
}

void CSelectionKeeper::setTransformation(TTransformationMode mode, bool square, bool rotate_x, bool rotate_y, bool rotate_z)
{
   squareTrans = square;
   rotate_x_enabled = rotate_x;
   rotate_y_enabled = rotate_y;
   rotate_z_enabled = rotate_z;
   currentTransformationMode = mode;
}

void CSelectionKeeper::doTransform(const QPoint &pt)
{
   doTransformRect(pt);
   doTransformPolygon(pt);
   pressPoint = pt;
}

void CSelectionKeeper::doTransformRect(const QPoint &pt)
{
   QPixmap               *transformResult = 0;
   CRectangularSelection *currSel = 0;
   for (int i = 0; i < allRects.size(); i++)
   {
      // get the selection to perform transformation on
      currSel = allRects.at(i);
      if (0 == currSel || !currSel->getToggle())
      {
         continue;
      }

      // get the pixmap to write the result to
      transformResult = new QPixmap(currSel->getWorldContentRect().size());
      if (0 == transformResult || transformResult->rect().size() == QSize(0, 0))
      {
         continue;
      }
      transformResult->fill(Qt::white);

      // get last transform
      QTransform transformPt = currSel->getTransform();

      // create a new transform, and add it to the last transform
      qreal rot_x = currSel->getAccumRotation_x();
      qreal rot_y = currSel->getAccumRotation_y();
      qreal rot_z = currSel->getAccumRotation_z();
      if (!handleTransform(pressPoint,
         pt,
         currSel->getContentPixmap()->rect(),
         transformPt,
         currSel->getLastTransform(),
         rot_x,
         rot_y,
         rot_z))
      {
         continue;
      }
      currSel->setAccumRotation_x(rot_x);
      currSel->setAccumRotation_y(rot_y);
      currSel->setAccumRotation_z(rot_z);

      QPixmap *toDraw = 0;
      toDraw = currSel->getContentPixmap();
      if (0 == toDraw || toDraw->size() == QSize(0, 0))
      {
         return;
      }

      // debug
      //emit printSig(toDraw->rect().adjusted(20,20,20,20), *toDraw, toDraw->rect(), "toDraw");

      // perform transformation on a QPainter and use it to write the content of the selection to the result pixmap
      // this resulting pixmap will have been transformed.
      QRect transformResRect = transformResult->rect();
      QRect toDrawRect = toDraw->rect();
      QPainter transformPainter(transformResult);
      transformPainter.setTransform(transformPt);
      transformPainter.setRenderHint(QPainter::Antialiasing, true);
      transformPainter.fillRect(transformResRect, Qt::white);
      transformPainter.drawPixmap(transformResRect, *toDraw, toDrawRect);
      transformPainter.end();

      // debug
      //int debugHeight = transformResult->rect().height();
      //emit printSig(transformResult->rect().adjusted(20,debugHeight+20,20,debugHeight+20), *transformResult, transformResult->rect(), "transformResult");

      // write the result pixmap to the extra-pixmap of the selection
      currSel->storeThisPixmap(transformResult, true);
      currSel->setTransform(transformPt);
      transformResult = 0;
      currSel = 0;
   }
}

void CSelectionKeeper::doTransformPolygon(const QPoint &pt)
{
   QPixmap           *transformResult = 0;
   CPolygonSelection *currSel = 0;

   for (int i = 0; i < allPolygons.size(); i++)
   {
      // get the selection to perform transformation on
      currSel = allPolygons.at(i);
      if (0 == currSel || !currSel->setMoving(true))
      {
         continue;
      }

      // get the pixmap to write the result to
      transformResult = new QPixmap(currSel->getWorldPolygon().boundingRect().size());
      if (0 == transformResult || transformResult->rect().size() == QSize(0, 0))
      {
         continue;
      }
      transformResult->fill(Qt::white);

      // get last transform
      QTransform transformPt = currSel->getTransform();

      // create a new transform, and add it to the last transform
      qreal rot_x = currSel->getAccumRotation_x();
      qreal rot_y = currSel->getAccumRotation_y();
      qreal rot_z = currSel->getAccumRotation_z();
      if (!handleTransform(pressPoint,
         pt,
         currSel->getContentPixmap()->rect(),
         transformPt,
         currSel->getLastTransform(),
         rot_x,
         rot_y,
         rot_z))
      {
         continue;
      }
      currSel->setAccumRotation_x(rot_x);
      currSel->setAccumRotation_y(rot_y);
      currSel->setAccumRotation_z(rot_z);

      QPixmap *toDraw = 0;
      toDraw = currSel->getContentPixmap();
      if (0 == toDraw || toDraw->size() == QSize(0, 0))
      {
         return;
      }

      // debug
      //emit printSig(toDraw->rect().adjusted(20,20,20,20), *toDraw, toDraw->rect(), "toDraw");

      // perform transformation on a QPainter and use it to write the content of the selection to the result pixmap
      // this resulting pixmap will have been transformed.
      QRect transformResRect = transformResult->rect();
      QRect toDrawRect = toDraw->rect();
      QPainter transformPainter(transformResult);
      transformPainter.setTransform(transformPt);
      transformPainter.setRenderHint(QPainter::Antialiasing, true);
      transformPainter.fillRect(transformResRect, Qt::white);
      transformPainter.drawPixmap(transformResRect, *toDraw, toDrawRect);
      transformPainter.end();

      // debug
      //int debugHeight = transformResult->rect().height();
      //emit printSig(transformResult->rect().adjusted(20,debugHeight+20,20,debugHeight+20), *transformResult, transformResult->rect(), "transformResult");

      // write the result pixmap to the extra-pixmap of the selection
      currSel->storeThisPixmap(transformResult, true);
      currSel->setTransform(transformPt);
      transformResult = 0;
      currSel = 0;
   }
}

bool CSelectionKeeper::handleTransform(QPoint &start, const QPoint &stop, QRect &rect, QTransform &resTrans, QTransform foundation,
   qreal &rotate_x, qreal &rotate_y, qreal &rotate_z)
{
   QTransform addTrans;
   if (currentTransformationMode == TRANS_ROTATE)
   {
      addTrans = foundation;
   }
   if (addTrans == QTransform(0, 0, 0, 0, 0, 0, 0, 0, 0))
   {
      addTrans = QTransform();
   }

   int dy = stop.y() - start.y();
   int dx = stop.x() - start.x();
   qreal rotationAngle_y = (qreal)dy;
   qreal rotationAngle_x = (qreal)dx;

   QPointF useCenter((qreal)rect.width() / (qreal)2, (qreal)rect.height() / (qreal)2);

   addTrans.translate(-rect.topLeft().x(), -rect.topLeft().y());
   addTrans.translate(useCenter.x(), useCenter.y());

   switch (currentTransformationMode)
   {
   case(TRANS_SHEAR) :
   {
      if (squareTrans)
      {
         addTrans.shear(rotationAngle_y / 50.0, rotationAngle_y / 50.0);
      }
      else
      {
         addTrans.shear(rotationAngle_x / 50.0, rotationAngle_y / 50.0);
      }
      break;
   }
   case(TRANS_SCALE) :
   {
      if (squareTrans)
      {
         addTrans.scale(1 + rotationAngle_y / 50.0, 1 + rotationAngle_y / 50.0);
      }
      else
      {
         addTrans.scale(1 + rotationAngle_x / 50.0, 1 + rotationAngle_y / 50.0);
      }
      break;
   }
   case(TRANS_ROTATE) :
   {
      if (rotate_x_enabled)
      {
         rotate_x += rotationAngle_y;
      }
      if (rotate_y_enabled)
      {
         rotate_y += rotationAngle_x;
      }
      if (rotate_z_enabled)
      {
         rotate_z += rotationAngle_y;
      }
      addTrans.rotate(rotate_x, Qt::XAxis);
      addTrans.rotate(rotate_y, Qt::YAxis);
      addTrans.rotate(rotate_z, Qt::ZAxis);
      break;
   }
   }

   addTrans.translate(-useCenter.x(), -useCenter.y());
   addTrans.translate(rect.topLeft().x(), rect.topLeft().y());

   if (resTrans == QTransform(0, 0, 0, 0, 0, 0, 0, 0, 0) || currentTransformationMode == TRANS_ROTATE)
   {
      resTrans = addTrans;
   }
   else
   {
      resTrans *= addTrans;
   }
   return true;
}

void CSelectionKeeper::finalizeTransformation()
{
   // backup transformation, because they might be needed as foundation for the next session
   for (int i = 0; i < allRects.size(); i++)
   {
      CRectangularSelection *currSel = allRects.at(i);
      if (0 == currSel || !currSel->getToggle())
      {
         continue;
      }
      QTransform pt = currSel->getTransform();
      currSel->setLastTransform(pt);
      currSel->setAccumRotation_x(0);
      currSel->setAccumRotation_y(0);
      currSel->setAccumRotation_z(0);
   }

   for (int i = 0; i < allPolygons.size(); i++)
   {
      CPolygonSelection *currPoly = allPolygons.at(i);
      if (0 == currPoly)
      {
         continue;
      }
      if (currPoly->getCurrentState() == SEL_STATE_MOVING)
      {
         if (!currPoly->setMoving(false))
         {
            continue;
         }
         QTransform pt = currPoly->getTransform();
         currPoly->setLastTransform(pt);
         currPoly->setAccumRotation_x(0);
         currPoly->setAccumRotation_y(0);
         currPoly->setAccumRotation_z(0);
      }
      else if (staticPressPoint == releasePoint)
      {
         currPoly->toggle();
      }
   }

   //rememberLastRotation_x    = 0;
   //rememberLastRotation_y    = 0;
   //rememberLastRotation_z    = 0;
}

//  This will perform the current transform on the content, then write this
//  transformed content back to the original content, for each selection with
//  a valid transform object. The transform object will then be deleted.
void CSelectionKeeper::endTransformation()
{
   CRectangularSelection *currSel = 0;
   for (int i = 0; i < allRects.size(); i++)
   {
      currSel = allRects.at(i);
      QTransform usedTrans = currSel->getTransform();

      // dont transform the content if
      if (0 == currSel || // the content itself doesnt exist
         usedTrans == QTransform(0, 0, 0, 0, 0, 0, 0, 0, 0) || // the transformation is the 0 matrix, which blanks out the content
         usedTrans.isIdentity() || // the transformation is the identity matrix, which does nothing
         0 == currSel->getExtraPixmap()) // the extra pixmap is not defined, meaning no transformation has been done
      {
         continue;
      }
      QPixmap *newContent = new QPixmap(currSel->getWorldContentRect().size());
      QPainter painter(newContent);
      painter.setTransform(usedTrans);
      painter.drawPixmap(currSel->getContentPixmap()->rect(), *currSel->getContentPixmap(), currSel->getContentPixmap()->rect());
      painter.end();
      currSel->updateContent(newContent);
      currSel->deleteTransforms();
      delete newContent;
      newContent = 0;
      currSel = 0;
   }
   currSel = 0;

   // polygons
   CPolygonSelection *currPoly = 0;
   for (int i = 0; i < allPolygons.size(); i++)
   {
      currPoly = allPolygons.at(i);
      QTransform usedTrans = currPoly->getTransform();

      // dont transform the content if
      if (0 == currPoly || // the content itself doesnt exist
         usedTrans == QTransform(0, 0, 0, 0, 0, 0, 0, 0, 0) || // the transformation is the 0 matrix, which blanks out the content
         usedTrans.isIdentity() || // the transformation is the identity matrix, which does nothing
         0 == currPoly->getExtraPixmap()) // the extra pixmap is not defined, meaning no transformation has been done
      {
         continue;
      }
      QPixmap *newContent = new QPixmap(currPoly->getWorldPolygon().boundingRect().size());
      QPainter painter(newContent);
      QPainterPath path;
      path.addPolygon(currPoly->getLocalPolygon());
      painter.setClipPath(path);
      painter.setTransform(usedTrans);
      painter.drawPixmap(currPoly->getContentPixmap()->rect(), *currPoly->getContentPixmap(), currPoly->getContentPixmap()->rect());
      painter.end();
      currPoly->updateContent(newContent);
      currPoly->deleteTransforms();
      delete newContent;
      newContent = 0;
      currPoly = 0;
   }
   currPoly = 0;
}

//  This function (used on mouse press event) determines the area type which the
//  cursor is currently hovering over , related to the polygon selections
TPolygonDragArea CSelectionKeeper::getPressPointArea(const QPoint &pt, bool doUpdateLocal)
{
   TPolygonDragArea dragArea = POLY_NO_DRAG;
   CPolygonSelection *currSel = 0;
   for (int i = 0; i < allPolygons.size(); i++)
   {
      currSel = allPolygons.at(i);
      if (0 == currSel)
      {
         continue;
      }
      dragArea = currSel->getAreaAtPoint(pt);
      if (dragArea == POLY_SELECTION_DRAG || dragArea == POLY_BOX_DRAG)
      {
         if (dragArea == POLY_BOX_DRAG)
         {
            dragboxFocusPolygon = i;
         }
         else
         {
            dragboxFocusPolygon = -1;
         }
         break;
      }
      else
      {
         dragboxFocusPolygon = -1;
      }
   }
   currSel = 0;
   if (doUpdateLocal)
   {
      pressPointArea = dragArea;
   }
   return dragArea;
}

TPolygonDragArea CSelectionKeeper::getPressPointArea()
{
   return pressPointArea; 
}

void CSelectionKeeper::setIsDefiningPolygon(bool val)
{
   if (val)
   {
      isDraggingPolygonSelection = !val;
      isDraggingPolygonBox = !val;
      isStatic = !val;
   }
   isDefiningPolygon = val;
}

void CSelectionKeeper::setIsDraggingPolygonSelection(bool val)
{
   if (val)
   {
      isDefiningPolygon = !val;
      isDraggingPolygonBox = !val;
      isStatic = !val;
   }
   isDraggingPolygonSelection = val;
}

void CSelectionKeeper::setIsDraggingPolygonBox(bool val)
{
   if (val)
   {
      isDraggingPolygonSelection = !val;
      isDefiningPolygon = !val;
      isStatic = !val;
   }
   isDraggingPolygonBox = val;
}

void CSelectionKeeper::setIsStatic(bool val)
{
   if (val)
   {
      isDraggingPolygonSelection = !val;
      isDefiningPolygon = !val;
      isDraggingPolygonBox = !val;
   }
   isStatic = val;
}

void CSelectionKeeper::addPolygonSelection(const QPolygon &polygon)
{
   if (!getHasBuffer())
   {
      return;
   }

   isDraggingPolygonSelection = false;
   isDefiningPolygon = false;
   isDraggingPolygonBox = false;

   CPolygonSelection *newSelection = 0;
   newSelection = new CPolygonSelection(polygon, m_buffer);
   if (0 != newSelection)
   {
      connect(newSelection, SIGNAL(printSig(QRect &, const QPixmap &, QRect &, const QString &)),
         this, SIGNAL(printSig(QRect &, const QPixmap &, QRect &, const QString &)));

      allPolygons << newSelection;
   }
}

void CSelectionKeeper::dragPolygonDragbox(int dx, int dy)
{
   if (!getHasBuffer())
   {
      return;
   }

   if (dragboxFocusPolygon < 0 || dragboxFocusPolygon >= allPolygons.size())
   {
      return;
   }

   CPolygonSelection *currPoly = allPolygons.at(dragboxFocusPolygon);
   if (0 == currPoly)
   {
      return;
   }

   currPoly->moveDragBox(dx, dy, m_buffer);
   currPoly = 0;
}

void CSelectionKeeper::toggleFocusPolygon()
{
   if (!(polygonFocusIndex < allPolygons.size() && polygonFocusIndex >= 0))
   {
      // invalid focus rect, nothing to do
      return;
   }

   CPolygonSelection *focusPoly = allPolygons.at(polygonFocusIndex);
   if (0 != focusPoly)
   {
      focusPoly->toggle();
   }
   focusPoly = 0;
}

void CSelectionKeeper::mouseMovementPolygon(const QPoint *currPoint)
{
   polygonFocusIndex = -1;
   if (0 == currPoint)
   {
      return;
   }

   // find focus polygon
   QList<int> polygonsInFocus;
   int noPolygons = allPolygons.size();
   for (int i = 0; i < noPolygons; i++)
   {
      CPolygonSelection *tmpPolyg = allPolygons.at(i);
      if (0 == tmpPolyg)
      {
         continue;
      }
      if (tmpPolyg->getWorldPolygon().containsPoint(*currPoint, Qt::OddEvenFill))
      {
         polygonsInFocus << i;
      }
      tmpPolyg = 0;
   }

   // prioritize
   QList<int> excludePolys;
   for (int i = 0; i < polygonsInFocus.size(); i++)
   {
      CPolygonSelection *tmpPolyg = allPolygons.at(polygonsInFocus.at(i));
      if (0 == tmpPolyg)
      {
         continue;
      }

      QRect checkRect = tmpPolyg->getWorldPolygon().boundingRect();
      for (int j = 0; j < polygonsInFocus.size(); j++)
      {
         if (i == j)
         {
            continue;
         }

         CPolygonSelection *compPolyg = allPolygons.at(polygonsInFocus.at(j));
         if (0 == compPolyg)
         {
            continue;
         }

         QRect compareRect = compPolyg->getWorldPolygon().boundingRect();
         if (compareRect.contains(checkRect) || !polygonsInFocus.contains(j))
         {
            // if the check rectangle is inside the compare rect, then exclude compare rect
            excludePolys << j;
         }
         compPolyg = 0;
      }
      tmpPolyg = 0;
   }

   CPolygonSelection *polygInFocus = 0;
   for (int i = 0; i < polygonsInFocus.size(); i++)
   {
      int currIndex = polygonsInFocus.at(i);
      if (excludePolys.contains(currIndex))
      {
         continue;
      }
      polygonFocusIndex = currIndex;
      polygInFocus = allPolygons.at(polygonFocusIndex);
   }

   // change color of the selection based on focus
   if (0 != polygInFocus)
   {
      QPen pen;
      pen.setWidth(2);
      pen.setStyle(Qt::DashLine);
      pen.setColor(polygInFocus->getCurrentPen().color());

      // change color of the selection in focus
      polygInFocus->changePen(&pen, true);
   }

   // change color of the selections without focus
   for (int x = 0; x < allPolygons.size(); x++)
   {
      if (x == polygonFocusIndex)
         continue;

      CPolygonSelection *tmp = allPolygons.at(x);
      if (0 != tmp)
      {
         QPen pen;
         pen.setWidth(1);
         pen.setStyle(Qt::DotLine);
         if (tmp->getCurrentState() == SEL_STATE_PASSIVE)
         {
            pen.setColor(Qt::lightGray);
            tmp->changePen(&pen, true);
         }
         else if (tmp->getCurrentState() == SEL_STATE_ACTIVE)
         {
            pen.setColor(Qt::blue);
            tmp->changePen(&pen, true);
         }
      }
      tmp = 0;
   }
}

void CSelectionKeeper::stopMovingRects()
{
   int noPolygons = allPolygons.size();
   CPolygonSelection *tmpPolyg = 0;
   for (int i = 0; i < noPolygons; i++)
   {
      tmpPolyg = allPolygons.at(i);
      if (0 != tmpPolyg && tmpPolyg->getCurrentState() == SEL_STATE_MOVING)
      {
         tmpPolyg->setState(SEL_STATE_ACTIVE);
      }
      tmpPolyg->setDragboxIndex(-1);
   }
   tmpPolyg = 0;
}

CPolygonSelection *CSelectionKeeper::getFocusPolygon()
{
   CPolygonSelection *focus = 0;
   if (hasPolygonFocus())
   {
      focus = allPolygons.at(polygonFocusIndex);
   }
   return focus;
}

bool CSelectionKeeper::hasRects()
{
   if (allRects.size() > 0)
   {
      return true;
   }
   else
   {
      return false;
   }
}

bool CSelectionKeeper::hasPolygons()
{
   if (allPolygons.size() > 0)
   {
      return true;
   }
   else
   {
      return false;
   }
}

void CSelectionKeeper::registerBuffer(QPixmap *buffer)
{
   if (0 != buffer)
   {
      m_buffer = buffer;
      hasBuffer = true;

      for (int i = 0; i < allPolygons.size(); i++)
      {
         if (0 != allPolygons.at(i))
         {
            allPolygons.at(i)->registerBuffer(m_buffer);
         }
      }

      for (int i = 0; i < allRects.size(); i++)
      {
         if (0 != allRects.at(i))
         {
            allRects.at(i)->registerBuffer(m_buffer);
         }
      }
   }
   else
   {
      releaseBuffer();
   }
}

void CSelectionKeeper::releaseBuffer()
{
   for (int i = 0; i < allPolygons.size(); i++)
   {
      if (0 != allPolygons.at(i))
      {
         allPolygons.at(i)->releaseBuffer();
      }
   }

   for (int i = 0; i < allRects.size(); i++)
   {
      if (0 != allRects.at(i))
      {
         allRects.at(i)->releaseBuffer();
      }
   }

   m_buffer = 0;
   hasBuffer = false;
}

bool CSelectionKeeper::getHasBuffer()
{
   if (0 != m_buffer)
   {
      hasBuffer = true;
   }
   else
   {
      hasBuffer = false;
   }
   return hasBuffer;
}