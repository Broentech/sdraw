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
//  \file      polygonSelection.cpp
//
//  \brief     Implementation of a class that defines a polygon selection
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

// Qt includes
#include <QPixmap>
#include <QBitmap>
#include <QPainter>

// local includes
#include "polygonSelection.h"

using namespace sdraw;

CPolygonSelection::CPolygonSelection(const QPolygon &polygon, const QPixmap *_content)
   :CSelectionBase(0)
{
   // set the current "mouse press area" to not defined (not presssed)
   currentClickArea = POLY_NOT_DEFINED;

   // initialize dragbox lists
   activeDragboxIndex = -1;

   // define polygon in world coordinates
   m_world_polygon = polygon;

   // define the selection bounding rectangle in world coordinates
   makeWorldGeo(m_world_polygon);

   // define polygon in local coordinates
   m_local_polygon = translateToLocal(m_world_polygon);

   // define the selection bounding rectangle in local coordinates
   makeLocalGeo(m_local_polygon);
   
   // define all the dragboxes
   defineDragboxes();

   // draw the selection to the lower buffer.
   drawSelection();

   // draw the world content at the selection
   drawContent(_content);
}

CPolygonSelection::~CPolygonSelection()
{

}

QPolygon CPolygonSelection::translateToLocal(const QPolygon &polygon)
{
   QRect bounding = polygon.boundingRect();
   QPoint translatePoint = bounding.topLeft();
   return polygon.translated(QPoint(-translatePoint.x(), -translatePoint.y()));
}

void CPolygonSelection::defineDragboxes()
{
   QPolygon translatedToFullContent = m_local_polygon;
   translatedToFullContent.translate(4,4);
   for(int i=0; i<translatedToFullContent.size(); i++)
   {
      QPoint currPoint   = translatedToFullContent.at(i);
      QPoint topLeft     = QPoint(currPoint.x() - 3, currPoint.y() - 3);
      QPoint bottomRight = QPoint(currPoint.x() + 3, currPoint.y() + 3);
      QRect newDragBox(topLeft, bottomRight);
      local_polygonDragBoxes.insert(i, newDragBox);
   }
   for(int i=0; i<m_world_polygon.size(); i++)
   {
      QPoint currPoint   = m_world_polygon.at(i);
      QPoint topLeft     = QPoint(currPoint.x() - 3, currPoint.y() - 3);
      QPoint bottomRight = QPoint(currPoint.x() + 3, currPoint.y() + 3);
      QRect newDragBox(topLeft, bottomRight);
      world_polygonDragBoxes.insert(i, newDragBox);
   }
}

//  Find out whether the argument point is located outside of the selection
//  is inside the seleciton or is currently positioned over a dragbox
TPolygonDragArea CPolygonSelection::getAreaAtPoint(const QPoint &pt)
{
   currentClickArea = POLY_NO_DRAG;
   // check if the point can be contained in the selection
   if(m_local_polygon.size() < 3)
   {
      // the polygon selection is either zero, a point or a line and thus no point can be located
      // inside it.
      return currentClickArea;
   }

   // check if the point is inside any dragbox
   QHashIterator <int, QRect> it(world_polygonDragBoxes);
   while(it.hasNext())
   {
      it.next();
      QRect checkThis = it.value(); 
      if(checkThis.contains(pt))
      {
         currentClickArea = POLY_BOX_DRAG;
         activeDragboxIndex = it.key();
         return currentClickArea;
      }
   }

   activeDragboxIndex = -1;

   // check if the point is inside the selection
   if(m_world_polygon.containsPoint(pt, Qt::OddEvenFill))
   {
      currentClickArea = POLY_SELECTION_DRAG;
      return currentClickArea;
   }

   // the selection is defined, and the point is outside of it
   return currentClickArea;
}

void CPolygonSelection::drawSelection(const QPen *pen, bool keepPen)
{
   QPolygon translatedToFullContent = m_local_polygon;
   translatedToFullContent.translate(4,4);

   if(m_local_polygon.size() < 3)
   {
      // can't draw this polygon, it is not capable of keeping a content
      return;
   }

   QSize selectionSize = localSelectionRect.size();
   if(0 == bottomPixmap)
   {
      bottomPixmap = new QPixmap(selectionSize);
   }
   else if(bottomPixmap->size() != selectionSize)
   {
      delete bottomPixmap;
      bottomPixmap = new QPixmap(selectionSize);
   }
   if(0 == bottomPixmap)
   {
      return;
   }
   bottomPixmap->fill(Qt::white);

   QPen dottedPen;
   if(!keepPen)
   {
      if(0 == pen)
      {
         dottedPen.setWidth(1);
         dottedPen.setColor(Qt::blue);
         dottedPen.setStyle(Qt::DotLine);
      }
      else
      {
         dottedPen = *pen;
      }
   }
   else
   {
      dottedPen = m_pen;
   }

   m_pen = dottedPen;

   QPen straightPen;
   straightPen.setColor(Qt::black);
   straightPen.setStyle(Qt::SolidLine);

   QPainter painter(bottomPixmap);

   // clear surrounding selection polygon
   QPen whitePen;
   whitePen.setWidth(2);
   whitePen.setColor(Qt::white);
   painter.setPen(whitePen);
   painter.drawPolygon(translatedToFullContent);

   painter.setPen(dottedPen);
   painter.drawPolygon(translatedToFullContent);
   painter.setPen(straightPen);
   QHashIterator<int, QRect> it(local_polygonDragBoxes);
   while(it.hasNext())
   {
      it.next();
      QRect currRect = it.value();
      painter.fillRect(currRect, Qt::white);
      painter.drawRect(currRect);
   }
   painter.end();

}

void CPolygonSelection::changePen(const QPen *pen, bool redrawContent)
{
   if(0 != pen)
   {
      drawSelection(pen);
   }
}

void CPolygonSelection::drawContent(const QPixmap *source)
{
   if(0 == source)
   {
      return;
   }
   if(0 != content)
   {
      delete content;
      content = 0;
   }
   content = new QPixmap(worldContentRect.size());
   content->fill(Qt::white);

   QPainterPath path;
   path.addPolygon(m_local_polygon);

   QPainter painter(content);
   painter.setClipPath(path);
   painter.drawPixmap(content->rect(), *source, m_world_polygon.boundingRect());
   painter.end();
}

void CPolygonSelection::drawSelectionToWorld(QPaintDevice *targetPixmap)
{
   if(0 == targetPixmap)
   {
      return;
   }

   QPainter painter(targetPixmap);
   if(0 != bottomPixmap)
   {
      //QSize debugWorldSize = worldSelectionRect.size();
      //QSize debugBottomSize = bottomPixmap->size();
      //painter.drawRect(worldSelectionRect);

      QBitmap bmap = bottomPixmap->createMaskFromColor(Qt::white, Qt::MaskInColor);
      bottomPixmap->setMask(bmap);
      painter.drawPixmap(worldSelectionRect, *bottomPixmap, bottomPixmap->rect());
   }
   if(writeExtraInstead)
   {
      if(0 != extraPixmap)
      {
         QBitmap bmap = extraPixmap->createMaskFromColor(Qt::white, Qt::MaskInColor);
         extraPixmap->setMask(bmap);
         QPainterPath path;
         path.addPolygon(m_world_polygon);
         painter.setClipPath(path);
         painter.drawPixmap(worldContentRect, *extraPixmap, extraPixmap->rect());

         painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
         painter.drawPixmap(worldSelectionRect, *bottomPixmap, bottomPixmap->rect());
      }
   }
   else
   {
      if(0 != content)
      {
         QBitmap bmap = content->createMaskFromColor(Qt::white, Qt::MaskInColor);
         content->setMask(bmap);
         painter.drawPixmap(worldContentRect, *content, content->rect());
      }
   }
   if(0 != topContent)
   {
      QBitmap bmap = topContent->createMaskFromColor(Qt::white, Qt::MaskInColor);
      topContent->setMask(bmap);
      painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
      painter.drawPixmap(worldContentRect, *topContent, localContentRect);
   }
   painter.end();
}

void CPolygonSelection::drawContentToWorld(QPaintDevice *targetPixmap)
{
   if(0 == targetPixmap)
   {
      return;
   }

   QPainterPath path;
   path.addPolygon(m_world_polygon);
   QPainter painter(targetPixmap);
   painter.setClipPath(path);
   if(writeExtraInstead)
   {
      if(0 != extraPixmap)
      {
         painter.drawPixmap(m_world_polygon.boundingRect(), *extraPixmap, extraPixmap->rect());
      }
   }
   else
   {
      if(0 != content)
      {
         painter.drawPixmap(m_world_polygon.boundingRect(), *content, content->rect());
      }
   }
   if(0 != topContent)
   {
      QBitmap bmap = topContent->createMaskFromColor(Qt::white, Qt::MaskInColor);
      topContent->setMask(bmap);

      painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
      painter.drawPixmap(m_world_polygon.boundingRect(), *topContent, topContent->rect());
   }
   painter.end();
}

void CPolygonSelection::makeWorldGeo(const QPolygon &polygon)
{
   worldContentRect = polygon.boundingRect();
   QPoint ul(worldContentRect.topLeft    ().x() - 4, worldContentRect.topLeft    ().y() - 4);
   QPoint br(worldContentRect.bottomRight().x() + 4, worldContentRect.bottomRight().y() + 4);
   worldSelectionRect = QRect(ul,br);
}

void CPolygonSelection::makeLocalGeo(const QPolygon &polygon)
{
   localContentRect = polygon.boundingRect();
   QPoint ul(localContentRect.topLeft    ().x()    , localContentRect.topLeft    ().y()    );
   QPoint br(localContentRect.bottomRight().x() + 8, localContentRect.bottomRight().y() + 8);
   localSelectionRect = QRect(ul,br);
}

void CPolygonSelection::toggle()
{
   // flip active/passive
   if(currentState == SEL_STATE_PASSIVE)
   {
      if(!setState(SEL_STATE_ACTIVE))
      {
         return;
      }
   }
   else if(currentState == SEL_STATE_ACTIVE)
   {
      if(!setState(SEL_STATE_PASSIVE))
      {
         return;
      }
   }
   else
   {
      return;
   }

   QPen pen;
   pen.setWidth(1);
   pen.setStyle(Qt::DotLine);

   if(currentState == SEL_STATE_ACTIVE)
   {
      pen.setColor(Qt::blue);
   }
   else if(currentState == SEL_STATE_PASSIVE)
   {
      pen.setColor(Qt::lightGray);
   }
   else
   {
      // no valild state
      return;
   }

   changePen(&pen, true);
}

void CPolygonSelection::dragSelection(int dx, int dy)
{
   if(!setMoving(true))
   {
      return;
   }

   QHashIterator<int, QRect> it(world_polygonDragBoxes);
   while(it.hasNext())
   {
      it.next();
      QRect currRect = it.value();
      currRect.adjust(dx, dy, dx, dy);
      world_polygonDragBoxes.insert(it.key(), currRect);
   }
   worldSelectionRect.adjust(dx, dy, dx, dy);
   worldContentRect  .adjust(dx, dy, dx, dy);
   m_world_polygon.translate(dx, dy);
}

bool CPolygonSelection::setMoving(bool onOrOff)
{
   if(onOrOff)
   {
      setState(SEL_STATE_MOVING);
      if(currentState != SEL_STATE_MOVING)
      {
         return false;
      }
      return true;
   }
   else
   {
      setState(SEL_STATE_ACTIVE);
      if(currentState != SEL_STATE_ACTIVE)
      {
         return false;
      }
      return true;
   }
}

QPen CPolygonSelection::getCurrentPen()
{
   return m_pen;
}

void CPolygonSelection::moveDragBox(int dx, int dy, const QPixmap *worldPixmap)
{
   if(world_polygonDragBoxes.contains(activeDragboxIndex))
   {
      QRect dragThis_world = world_polygonDragBoxes.value(activeDragboxIndex);
      for(int i=0; i<m_world_polygon.size(); i++)
      {
         QPoint checkWorldPt = m_world_polygon.point(i);
         if(dragThis_world.contains(checkWorldPt))
         {
            checkWorldPt.setX(checkWorldPt.x() + dx);
            checkWorldPt.setY(checkWorldPt.y() + dy);
            m_world_polygon.setPoint(i, checkWorldPt);
            break;
         }
      }
      dragThis_world.adjust(dx,dy,dx,dy);
      world_polygonDragBoxes.insert(activeDragboxIndex, dragThis_world);
      
      // update geometry
      makeWorldGeo(m_world_polygon);
      m_local_polygon = translateToLocal(m_world_polygon);
      makeLocalGeo(m_local_polygon);
      defineDragboxes();
   }
   drawSelection();
   drawContent(worldPixmap);
}

void CPolygonSelection::setDragboxIndex(int index)
{
   activeDragboxIndex = index;
}

void CPolygonSelection::makeWorldShadow(const QPixmap *worldPixmap)
{
   if(0 == worldPixmap)
   {
      return;
   }

   // define the pixmap, erase old one if it exists
   if(0 != worldShadow)
   {
      delete worldShadow;
      worldShadow = 0;
   }
   worldShadow = new QPixmap(m_local_polygon.boundingRect().size());

   QPainterPath path;
   path.addPolygon(m_local_polygon);
   
   // draw world to shadow
   QPainter painter(worldShadow);
   painter.setClipPath(path);
   painter.drawPixmap(worldShadow->rect(), *worldPixmap, m_world_polygon.boundingRect());
   painter.end();

   hasShadow = true;
}

void CPolygonSelection::clearContent()
{
   if(0 != bottomPixmap)
   {
      bottomPixmap->fill(Qt::white);
   }
   if(0 != content)
   {
      content->fill(Qt::white);
   }
   if(0 != topContent)
   {
      topContent->fill(Qt::white);
   }
   if(0 != worldShadow)
   {
      worldShadow->fill(Qt::white);
   }
   if(0 != extraPixmap)
   {
      extraPixmap->fill(Qt::white);
   }

   drawSelection(0, true);
}

void CPolygonSelection::updateContent(const QPixmap *newContent , const QRect *newContentRect)
{
   // check conditions
   if(0 == newContent)
   {
      // there is nothing to draw, can't continue
      return;
   }
   if(0 != newContentRect && 
      newContentRect->size() != m_world_polygon.boundingRect().size())
   {
      // the argument geometry doesnt match the local geometry of the selection, results would have
      // been scaled if we had continued
      return;
   }
   else if(newContent->rect().size() != m_local_polygon.boundingRect().size())
   {
      // the argument geometry wasnt defined, so the geometry of the argument pixmap must match the
      // local selection geometry, but it didnt so we can't continue
      return;
   }

   // define and blank out content pixmap
   if(0 == content)
   {
      content = new QPixmap(newContent->rect().size());
   }
   content->fill(Qt::white);

   QPainterPath path;
   path.addPolygon(m_local_polygon);
   QPainter painter(content);
   painter.setClipPath(path);
   if(0 == newContentRect)
   {
      // draw the pixmap argument using its own geometry
      painter.drawPixmap(content->rect(), *newContent, newContent->rect());
   }
   else
   {
      // draw the pixmap argument using the argument geometry
      painter.drawPixmap(content->rect(), *newContent, *newContentRect);
   }
   painter.end();
}

void CPolygonSelection::storeThisPixmap(const QPixmap *worldPixmap, bool enforce)
{
   writeExtraInstead = enforce;

   if(0 == worldPixmap || m_local_polygon.boundingRect().size() != worldPixmap->rect().size())
   {
      // arg pixmap must exist and the size must match the local coordinates
      return;
   }
   if(0 == extraPixmap)
   {
      extraPixmap = new QPixmap(m_local_polygon.boundingRect().size());
   }

   extraPixmap->fill(Qt::white);
   QPainter painter(extraPixmap);
   painter.drawPixmap(extraPixmap->rect(), *worldPixmap, worldPixmap->rect());
   painter.end();
}

void CPolygonSelection::drawIntersectionContent(const QPixmap *outsideContent, 
                                                const QRect &world_outsideContentRect, 
                                                const QRect &local_outsideContentRect,
                                                QPolygon &interpoly)
{
   QRect rectangularSelection       = m_world_polygon.boundingRect();
   QRect local_rectangularSelection = m_local_polygon.boundingRect();

   if(0 == content        || //< it is required that this object has a content pixmap
      0 == outsideContent || //< argument pixmap content must be defined
      !world_outsideContentRect.intersects(rectangularSelection)) //< argument pixmap must intersect with the boundingrect of the world polygon
   {
      return;
   }

   QRect intersectionRect   = world_outsideContentRect.intersected(rectangularSelection);
   QPoint inter_topLeft     = intersectionRect.topLeft();
   QPoint inter_bottomRight = intersectionRect.bottomRight();
   if(inter_topLeft == inter_bottomRight)
   {
      return;
   }

   // determine the local target equivalent to the intersection rectangle
   int target_topLeft_dx     = inter_topLeft    .x() - rectangularSelection.topLeft()    .x();
   int target_topLeft_dy     = inter_topLeft    .y() - rectangularSelection.topLeft()    .y();
   int target_bottomRight_dx = inter_bottomRight.x() - rectangularSelection.bottomRight().x();
   int target_bottomRight_dy = inter_bottomRight.y() - rectangularSelection.bottomRight().y();
   int target_topLeft_x      = local_rectangularSelection.topLeft()    .x() + target_topLeft_dx ;
   int target_topLeft_y      = local_rectangularSelection.topLeft()    .y() + target_topLeft_dy ;
   int target_bottomRight_x  = local_rectangularSelection.bottomRight().x() + target_bottomRight_dx ;
   int target_bottomRight_y  = local_rectangularSelection.bottomRight().y() + target_bottomRight_dy ;
   QPoint target_ul (target_topLeft_x    , target_topLeft_y     );
   QPoint target_br (target_bottomRight_x, target_bottomRight_y );
   QRect  target_LocalRect(target_ul, target_br);

   // determine the local source equivalent to the intersection rectangle
   int source_topLeft_dx     = inter_topLeft    .x() - world_outsideContentRect.topLeft()    .x();
   int source_topLeft_dy     = inter_topLeft    .y() - world_outsideContentRect.topLeft()    .y();
   int source_bottomRight_dx = inter_bottomRight.x() - world_outsideContentRect.bottomRight().x();
   int source_bottomRight_dy = inter_bottomRight.y() - world_outsideContentRect.bottomRight().y();
   int source_topLeft_x      = local_outsideContentRect.topLeft()    .x() + source_topLeft_dx;
   int source_topLeft_y      = local_outsideContentRect.topLeft()    .y() + source_topLeft_dy;
   int source_bottomRight_x  = local_outsideContentRect.bottomRight().x() + source_bottomRight_dx;
   int source_bottomRight_y  = local_outsideContentRect.bottomRight().y() + source_bottomRight_dy;
   QPoint source_ul(source_topLeft_x, source_topLeft_y);
   QPoint source_br(source_bottomRight_x, source_bottomRight_y);
   QRect  source_LocalRect(source_ul, source_br);

   // determine the local equivalent of the clipping polygon
   QPoint polyTopLeft  = interpoly.boundingRect().topLeft();
   int shiftedX = -polyTopLeft.x();
   int shiftedY = -polyTopLeft.y();
   QPoint shiftedTopLeft(shiftedX, shiftedY);
   QPoint localTopLeft = target_LocalRect.topLeft();
   QPolygon local_interpoly = interpoly.translated(shiftedTopLeft)   //< move it to local topleft
                                       .translated(localTopLeft);    //< move it to local intersection

   // draw intersected content to top layer pixmap
   if(0 != topContent)
   {
      delete topContent;
   }
   topContent = new QPixmap(content->size());
   topContent->fill(Qt::white);

   QPainter painter(topContent);
   QPainterPath path;
   path.addPolygon(local_interpoly);
   painter.setClipPath(path);
   painter.drawPixmap(target_LocalRect, *outsideContent, source_LocalRect);
   painter.end();

   QBitmap bmap = topContent->createMaskFromColor(Qt::white, Qt::MaskInColor);
   topContent->setMask(bmap);
}

void CPolygonSelection::collapseContents()
{
   if(0 != content)
   {
      if(0 != topContent)
      {
         QBitmap bmap = topContent->createMaskFromColor(Qt::white, Qt::MaskInColor);
         topContent->setMask(bmap);
         QPainter painter(content);
         QPainterPath path;
         path.addPolygon(m_local_polygon);
         painter.setClipPath(path);
         painter.drawPixmap(content->rect(), *topContent, topContent->rect());
         painter.end();

         if(0 != extraPixmap)
         {
            QPainter extraPainter(extraPixmap);
            extraPainter.setClipPath(path);
            extraPainter.drawPixmap(m_local_polygon.boundingRect(), *topContent, topContent->rect());
            extraPainter.end();
         }
      }
   }
}