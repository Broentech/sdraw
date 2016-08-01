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
//  \file      rectangularSelection.cpp
//
//  \brief     Implementation of CRectangularSelection
//
//  \author    Stian Broen
//
//  \date      24.10.2011
//
//  \par       Copyright:
//
//             Copyright by Stian Broen
//
//  \par       Revision History
//
//  \par       01 24.10.2011 Original version       
//
//
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QPainter>
#include <QTransform>
#include <QBitmap>

// solution includes
#include <zooming/zoomer.h>

// local includes
#include "rectangularSelection.h"

using namespace sdraw;

CRectangularSelection::CRectangularSelection(const QPoint &upperLeft, const QPoint &bottomRight, QPixmap *_content)
   :accumRotation_x(0),
   accumRotation_y(0),
   accumRotation_z(0),
   isToggled(true),
   hasShadow(false),
   writeExtraInstead(false),
   currentDragArea(NO_DRAG),
   bottomPixmap(0),
   content(0),
   topContent(0),
   worldShadow(0),
   extraPixmap(0),
   local_contentRect(QRect(0, 0, 0, 0))
{
   registerBuffer(_content, upperLeft, bottomRight);

   // define coordinates for all rectangles, both world and local
   defineWorldRects();
   defineLocalRects();

   //local_contentRect = local_rectangularSelection;
   contentRect = rectangularSelection;

   bufferSel.contentRect = contentRect;

   QPen pen;
   pen.setWidth(1);
   pen.setColor(Qt::blue);
   pen.setStyle(Qt::DashLine);
   drawSelection(/*true,*/ &pen);
}

void CRectangularSelection::registerBuffer(QPixmap *buffer,  const QPoint &upperLeft, const QPoint &bottomRight)
{
   if(0 != buffer)
   {
      if(abs(upperLeft.x() - bottomRight.x()) > 0 && abs(upperLeft.y() - bottomRight.y()) > 0)
      {
         outsideUpperLeft   = upperLeft  ;
         outsideBottomRight = bottomRight;
      }
      else
      {
         outsideUpperLeft   = buffer->rect().topLeft();
         outsideBottomRight = buffer->rect().bottomRight();
      }

      // define local boundaries
      contentSize.setWidth (abs(outsideBottomRight.x() - outsideUpperLeft.x()));
      contentSize.setHeight(abs(outsideBottomRight.y() - outsideUpperLeft.y()));

      totalSize.setWidth (contentSize.width()  + 8);
      totalSize.setHeight(contentSize.height() + 8);

      mUpperLeft   = QPoint(3,3);
      mBottomRight = QPoint(contentSize.width() + 3, contentSize.height() + 3);
      local_rectangularSelection.setTopLeft    (mUpperLeft);
      local_rectangularSelection.setBottomRight(mBottomRight);

      // define world boundaries
      if(upperLeft.y() < bottomRight.y() && 
         upperLeft.x() < bottomRight.x())
      {
         outsideUpperLeft   = upperLeft;
         outsideBottomRight = bottomRight;
      }
      else if(upperLeft.y() > bottomRight.y() &&
              upperLeft.x() < bottomRight.x())
      {
         outsideUpperLeft   = QPoint(upperLeft  .x() , bottomRight.y());
         outsideBottomRight = QPoint(bottomRight.x() , upperLeft  .y());
      }
      else if(upperLeft.y() < bottomRight.y() &&
              upperLeft.x() > bottomRight.x())
      {
         outsideUpperLeft    = QPoint(bottomRight.x() , upperLeft  .y());
         outsideBottomRight  = QPoint(upperLeft  .x() , bottomRight.y());
      }
      else if(upperLeft.y() > bottomRight.y() &&
              upperLeft.x() > bottomRight.x())
      {
         outsideUpperLeft   = bottomRight;
         outsideBottomRight = upperLeft;
      }

      rectangularSelection.setTopLeft(outsideUpperLeft);
      rectangularSelection.setBottomRight(outsideBottomRight);

      m_buffer   = buffer;
      hasBuffer  = true;
      content    = new QPixmap(rectangularSelection.size());
      topContent = 0;

      local_contentRect = content->rect();
      QPainter painter(content);
      painter.drawPixmap(local_contentRect, *m_buffer, rectangularSelection);
      painter.end();
   }
   else
   {
      releaseBuffer();
   }
}

void CRectangularSelection::releaseBuffer()
{
   m_buffer = 0;
   hasBuffer = false;
}

bool CRectangularSelection::getHasBuffer()
{
   if(0 != m_buffer)
   {
      hasBuffer = true;
   }
   else
   {
      hasBuffer = false;
   }
   return hasBuffer;
}

bool CRectangularSelection::confirmGeometries()
{
   if((abs(outsideUpperLeft.x() - outsideBottomRight.x())    <= 0 || abs(outsideUpperLeft.y() - outsideBottomRight.y())     <= 0) ||
      (abs(mUpperLeft      .x() - mBottomRight      .x())    <= 0 || abs(mUpperLeft      .y() - mBottomRight      .y())     <= 0) ||
      (rectangularSelectionErase                    .width() <= 0 || rectangularSelectionErase                    .height() <= 0) ||
      (rectangularSelection                         .width() <= 0 || rectangularSelection                         .height() <= 0) ||
      (contentRect                                  .width() <= 0 || contentRect                                  .height() <= 0) ||
      (upperLeftCornerDragArea                      .width() <= 0 || upperLeftCornerDragArea                      .height() <= 0) ||
      (upperMidDragArea                             .width() <= 0 || upperMidDragArea                             .height() <= 0) ||
      (upperRightDragArea                           .width() <= 0 || upperRightDragArea                           .height() <= 0) ||
      (midLeftDragArea                              .width() <= 0 || midLeftDragArea                              .height() <= 0) ||
      (midRightDragArea                             .width() <= 0 || midRightDragArea                             .height() <= 0) ||
      (bottomLeftDragArea                           .width() <= 0 || bottomLeftDragArea                           .height() <= 0) ||
      (bottomMidDragArea                            .width() <= 0 || bottomMidDragArea                            .height() <= 0) ||
      (bottomRightDragArea                          .width() <= 0 || bottomRightDragArea                          .height() <= 0) ||
      (local_rectangularSelectionErase              .width() <= 0 || local_rectangularSelectionErase              .height() <= 0) ||
      (local_rectangularSelection                   .width() <= 0 || local_rectangularSelection                   .height() <= 0) ||
      (local_contentRect                            .width() <= 0 || local_contentRect                            .height() <= 0) ||
      (local_upperLeftCornerDragArea                .width() <= 0 || local_upperLeftCornerDragArea                .height() <= 0) ||
      (local_upperMidDragArea                       .width() <= 0 || local_upperMidDragArea                       .height() <= 0) ||
      (local_upperRightDragArea                     .width() <= 0 || local_upperRightDragArea                     .height() <= 0) ||
      (local_midLeftDragArea                        .width() <= 0 || local_midLeftDragArea                        .height() <= 0) ||
      (local_midRightDragArea                       .width() <= 0 || local_midRightDragArea                       .height() <= 0) ||
      (local_bottomLeftDragArea                     .width() <= 0 || local_bottomLeftDragArea                     .height() <= 0) ||
      (local_bottomMidDragArea                      .width() <= 0 || local_bottomMidDragArea                      .height() <= 0) ||
      (local_bottomRightDragArea                    .width() <= 0 || local_bottomRightDragArea                    .height() <= 0)  )
   {
      return false;
   }

   return true;
}

CRectangularSelection::~CRectangularSelection()
{
   delete content;
}

void CRectangularSelection::clearContent()
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

   drawSelection(/*false, */0, true);
}

void CRectangularSelection::drawSelection(const QPen *pen, bool keepPen)
{
   if(!confirmGeometries())
   {
      return;
   }

   if(bottomPixmap == 0)
   {
      if(totalSize.isEmpty() || totalSize.isNull())
      {
         return;
      }
      bottomPixmap = new QPixmap(totalSize);
      bottomPixmap->fill(Qt::white);
   }
   QPainter painter(bottomPixmap);

   // clear surrounding selection rectangle
   QPen whitePen;
   whitePen.setWidth(2);
   whitePen.setColor(Qt::white);
   painter.setPen(whitePen);
   painter.drawRect(local_rectangularSelection);

   // set painter pen
   if(!keepPen)
   {
      if(0 == pen)
      {
         QPen _pen(Qt::DashLine);
         _pen.setColor(Qt::blue);
         _pen.setWidth(1);
         m_pen = _pen;
         painter.setPen( _pen );
      }
      else
      {
         m_pen = *pen;
         painter.setPen( *pen);
      }
   }
   else
   {
      painter.setPen(m_pen);
   }

   if(bottomPixmap == 0)
   {
      bottomPixmap = new QPixmap(totalSize);
      bottomPixmap->fill(Qt::white);
   }
   
   // draw surrounding selection rectangle
   painter.drawRect(local_rectangularSelection);

   // draw dragboxes
   QPen dragPen(Qt::SolidLine);
   dragPen.setColor(Qt::black);
   painter.setPen(dragPen);
   painter.fillRect(local_upperLeftCornerDragArea, Qt::white);
   painter.drawRect(local_upperLeftCornerDragArea);
   painter.fillRect(local_upperMidDragArea, Qt::white);
   painter.drawRect(local_upperMidDragArea);
   painter.fillRect(local_upperRightDragArea, Qt::white);
   painter.drawRect(local_upperRightDragArea);
   painter.fillRect(local_midLeftDragArea, Qt::white);
   painter.drawRect(local_midLeftDragArea);
   painter.fillRect(local_midRightDragArea, Qt::white);
   painter.drawRect(local_midRightDragArea);
   painter.fillRect(local_bottomLeftDragArea, Qt::white);
   painter.drawRect(local_bottomLeftDragArea);
   painter.fillRect(local_bottomMidDragArea, Qt::white);
   painter.drawRect(local_bottomMidDragArea);
   painter.fillRect(local_bottomRightDragArea, Qt::white);
   painter.drawRect(local_bottomRightDragArea);
   painter.end();

   QBitmap bmap = bottomPixmap->createMaskFromColor(Qt::white, Qt::MaskInColor);
                  bottomPixmap->setMask(bmap);
}
//f+//////////////////////////////////////////////////////////////////////////
//
//  Name:	drawContent - OUTDATED : as a rule, never write content to
//                        buffer containing the selection rectangle itself !!!!
/*!
//
//  \param  QPixmap *_content
//
//  \return N/A
//    
//  \author Stian Broen 
//
//  \date   24.10.2011
//
*/
//f-//////////////////////////////////////////////////////////////////////////
//void CRectangularSelection::drawContent(QPixmap *_content)
//{
//   if(0 == _content)
//      return;
//
//   if(bottomPixmap == 0)
//   {
//      bottomPixmap = new QPixmap(totalSize);
//      bottomPixmap->fill(Qt::white);
//   }
//
//   QPainter contentPainter(bottomPixmap); 
//   contentPainter.drawPixmap(local_rectangularSelection, *_content, _content->rect());
//   contentPainter.end();
//
//   QBitmap bmap = _content->createMaskFromColor(Qt::white, Qt::MaskInColor);
//   _content->setMask(bmap);
//}


void CRectangularSelection::drawContentToWorld(QPaintDevice *targetPixmap,
   Zoomer *zoomer,
   const QString &zoomBufferName)
{
   if (0 == targetPixmap || !confirmGeometries())
   {
      return;
   }

   QRect targetSel;
   if (0 != zoomer && !zoomBufferName.isNull())
   {
      targetSel = zoomer->getZoomedRect(rectangularSelection);
   }
   else
   {
      targetSel = rectangularSelection;
   }

   QPainter painter(targetPixmap);
   if (writeExtraInstead)
   {
      if (0 != extraPixmap)
      {
         painter.drawPixmap(targetSel, *extraPixmap, extraPixmap->rect());
      }
   }
   else
   {
      if (0 != content)
      {
         painter.drawPixmap(targetSel, *content, content->rect());
      }
   }
   if (0 != topContent)
   {
      QBitmap bmap = topContent->createMaskFromColor(Qt::white, Qt::MaskInColor);
      topContent->setMask(bmap);

      painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
      painter.drawPixmap(targetSel, *topContent, topContent->rect());
   }
   painter.end();
}

void CRectangularSelection::collapseContents()
{
   if (!confirmGeometries())
   {
      return;
   }
   if (0 != content)
   {
      if (0 != topContent)
      {
         QBitmap bmap = topContent->createMaskFromColor(Qt::white, Qt::MaskInColor);
         topContent->setMask(bmap);
         QRect topContentRect = topContent->rect();
         QPainter painter(content);
         painter.drawPixmap(local_contentRect, *topContent, topContentRect);
         painter.end();

         if (0 != extraPixmap)
         {
            QPainter extraPainter(extraPixmap);
            extraPainter.drawPixmap(local_contentRect, *topContent, topContentRect);
            extraPainter.end();
         }
      }
   }
}

void CRectangularSelection::drawSelectionToWorld(QPaintDevice *targetPixmap,
   Zoomer *zoomer,
   const QString &zoomBufferName)
{
   if (0 == targetPixmap || !confirmGeometries())
   {
      return;
   }

   QRect targetTotal;
   QRect targetSel;

   if (0 != zoomer && !zoomBufferName.isNull())
   {
      targetTotal = zoomer->getZoomedRect(rectangularSelectionErase);
      targetSel = zoomer->getZoomedRect(rectangularSelection);
   }
   else
   {
      targetTotal = rectangularSelectionErase;
      targetSel = rectangularSelection;
   }

   QPainter painter(targetPixmap);
   if (0 != bottomPixmap)
   {
      //QBitmap bmap = bottomPixmap->createMaskFromColor(Qt::white, Qt::MaskInColor);
      //bottomPixmap->setMask(bmap);
      painter.drawPixmap(targetTotal, *bottomPixmap, local_rectangularSelectionErase);
   }
   if (writeExtraInstead)
   {
      //writeExtraInstead = false;
      if (0 != extraPixmap)
      {
         QBitmap bmap = extraPixmap->createMaskFromColor(Qt::white, Qt::MaskInColor);
         extraPixmap->setMask(bmap);

         //painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
         painter.drawPixmap(targetSel, *extraPixmap, extraPixmap->rect());

      }
   }
   else
   {
      if (0 != content)
      {
         QBitmap bmap = content->createMaskFromColor(Qt::white, Qt::MaskInColor);
         content->setMask(bmap);

         //painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
         painter.drawPixmap(targetSel, *content, content->rect());

      }
   }
   if (0 != topContent)
   {
      QBitmap bmap = topContent->createMaskFromColor(Qt::white, Qt::MaskInColor);
      topContent->setMask(bmap);
      painter.drawPixmap(targetSel, *topContent, topContent->rect());
   }
   painter.end();
}

//  When 2 selection intersects, the content of the moving selection is written
//  into the selection that is stationary, at the local intersection rectangle
void CRectangularSelection::drawIntersectionContent(const QPixmap *outsideContent,
   const QRect &world_outsideContentRect,
   const QRect &local_outsideContentRect)
{
   if (0 == outsideContent ||
      !world_outsideContentRect.intersects(rectangularSelection) ||
      !confirmGeometries())
   {
      return;
   }

   QRect intersectionRect = world_outsideContentRect.intersected(rectangularSelection);
   QPoint inter_topLeft = intersectionRect.topLeft();
   QPoint inter_bottomRight = intersectionRect.bottomRight();
   if (inter_topLeft == inter_bottomRight)
   {
      return;
   }

   // determine the local target equivalent to the intersection rectangle
   int target_topLeft_dx = inter_topLeft.x() - rectangularSelection.topLeft().x();
   int target_topLeft_dy = inter_topLeft.y() - rectangularSelection.topLeft().y();
   int target_bottomRight_dx = inter_bottomRight.x() - rectangularSelection.bottomRight().x();
   int target_bottomRight_dy = inter_bottomRight.y() - rectangularSelection.bottomRight().y();
   int target_topLeft_x = local_rectangularSelection.topLeft().x() + target_topLeft_dx;
   int target_topLeft_y = local_rectangularSelection.topLeft().y() + target_topLeft_dy;
   int target_bottomRight_x = local_rectangularSelection.bottomRight().x() + target_bottomRight_dx;
   int target_bottomRight_y = local_rectangularSelection.bottomRight().y() + target_bottomRight_dy;
   QPoint target_ul(target_topLeft_x, target_topLeft_y);
   QPoint target_br(target_bottomRight_x, target_bottomRight_y);
   QRect  target_LocalRect(target_ul, target_br);

   // determine the local source equivalent to the intersection rectangle
   int source_topLeft_dx = inter_topLeft.x() - world_outsideContentRect.topLeft().x();
   int source_topLeft_dy = inter_topLeft.y() - world_outsideContentRect.topLeft().y();
   int source_bottomRight_dx = inter_bottomRight.x() - world_outsideContentRect.bottomRight().x();
   int source_bottomRight_dy = inter_bottomRight.y() - world_outsideContentRect.bottomRight().y();
   int source_topLeft_x = local_outsideContentRect.topLeft().x() + source_topLeft_dx;
   int source_topLeft_y = local_outsideContentRect.topLeft().y() + source_topLeft_dy;
   int source_bottomRight_x = local_outsideContentRect.bottomRight().x() + source_bottomRight_dx;
   int source_bottomRight_y = local_outsideContentRect.bottomRight().y() + source_bottomRight_dy;
   QPoint source_ul(source_topLeft_x, source_topLeft_y);
   QPoint source_br(source_bottomRight_x, source_bottomRight_y);
   QRect  source_LocalRect(source_ul, source_br);

   // draw intersected content to top layer pixmap
   if (0 != topContent)
   {
      delete topContent;
   }
   topContent = new QPixmap(local_contentRect.size());
   topContent->fill(Qt::white);

   QPainter painter(topContent);
   painter.drawPixmap(target_LocalRect, *outsideContent, source_LocalRect);
   painter.end();

   // debugging
   //QRect debugRect0 = outsideContent->rect();
   //debugRect0.adjust(15, 15, 15, 15);
   //emit printSig( target_LocalRect, 
   //              *outsideContent, 
   //               source_LocalRect, 
   //               QString("OUTSIDE CONTENT"));

   QBitmap bmap = topContent->createMaskFromColor(Qt::white, Qt::MaskInColor);
   topContent->setMask(bmap);
}

QPen CRectangularSelection::getCurrentPen()
{
   return m_pen;
}

void CRectangularSelection::defineLocalRects()
{
   // coordinates for the eraseSelectionArea
   int deleteRectangleUpperLeft_X = 0;
   int deleteRectangleUpperLeft_Y = 0;
   int deleteRectangleBottomRight_X = 0;
   int deleteRectangleBottomRight_Y = 0;

   // upper left dragbox
   int newUpperLeft_X = local_rectangularSelection.topLeft().x() - 3;
   int newUpperLeft_Y = local_rectangularSelection.topLeft().y() - 3;
   int newBottomRight_X = local_rectangularSelection.topLeft().x() + 3;
   int newBottomRight_Y = local_rectangularSelection.topLeft().y() + 3;
   // Use topleft coordinates from ths dragbox for the eraseSelection topleft point
   deleteRectangleUpperLeft_X = newUpperLeft_X;
   deleteRectangleUpperLeft_Y = newUpperLeft_Y;
   QPoint tmpUpperLeft(newUpperLeft_X, newUpperLeft_Y);
   QPoint tmpBottomRight(newBottomRight_X, newBottomRight_Y);
   local_upperLeftCornerDragArea = QRect(tmpUpperLeft, tmpBottomRight);

   // upper mid dragbox
   newUpperLeft_X = local_rectangularSelection.topLeft().x() - 3 +
      int(qreal(local_rectangularSelection.bottomRight().x() - local_rectangularSelection.topLeft().x()) / 2.0);
   newUpperLeft_Y = local_rectangularSelection.topLeft().y() - 3;
   newBottomRight_X = local_rectangularSelection.topLeft().x() + 3 +
      int(qreal(local_rectangularSelection.bottomRight().x() - local_rectangularSelection.topLeft().x()) / 2.0);
   newBottomRight_Y = local_rectangularSelection.topLeft().y() + 3;
   tmpUpperLeft = QPoint(newUpperLeft_X, newUpperLeft_Y);
   tmpBottomRight = QPoint(newBottomRight_X, newBottomRight_Y);
   local_upperMidDragArea = QRect(tmpUpperLeft, tmpBottomRight);

   // upper right dragbox
   newUpperLeft_X = local_rectangularSelection.bottomRight().x() - 3;
   newUpperLeft_Y = local_rectangularSelection.topLeft().y() - 3;
   newBottomRight_X = local_rectangularSelection.bottomRight().x() + 3;
   newBottomRight_Y = local_rectangularSelection.topLeft().y() + 3;
   tmpUpperLeft = QPoint(newUpperLeft_X, newUpperLeft_Y);
   tmpBottomRight = QPoint(newBottomRight_X, newBottomRight_Y);
   local_upperRightDragArea = QRect(tmpUpperLeft, tmpBottomRight);

   // mid left dragbox
   newUpperLeft_X = local_rectangularSelection.topLeft().x() - 3;
   newUpperLeft_Y = local_rectangularSelection.topLeft().y() - 3 +
      int((qreal)(local_rectangularSelection.bottomRight().y() - local_rectangularSelection.topLeft().y()) / 2.0);
   newBottomRight_X = local_rectangularSelection.topLeft().x() + 3;
   newBottomRight_Y = local_rectangularSelection.topLeft().y() + 3 +
      int((qreal)(local_rectangularSelection.bottomRight().y() - local_rectangularSelection.topLeft().y()) / 2.0);
   tmpUpperLeft = QPoint(newUpperLeft_X, newUpperLeft_Y);
   tmpBottomRight = QPoint(newBottomRight_X, newBottomRight_Y);
   local_midLeftDragArea = QRect(tmpUpperLeft, tmpBottomRight);

   // mid right dragbox
   newUpperLeft_X = local_rectangularSelection.bottomRight().x() - 3;
   newUpperLeft_Y = local_rectangularSelection.topLeft().y() - 3 +
      int((qreal)(local_rectangularSelection.bottomRight().y() - local_rectangularSelection.topLeft().y()) / 2.0);
   newBottomRight_X = local_rectangularSelection.bottomRight().x() + 3;
   newBottomRight_Y = local_rectangularSelection.topLeft().y() + 3 +
      int((qreal)(local_rectangularSelection.bottomRight().y() - local_rectangularSelection.topLeft().y()) / 2.0);
   tmpUpperLeft = QPoint(newUpperLeft_X, newUpperLeft_Y);
   tmpBottomRight = QPoint(newBottomRight_X, newBottomRight_Y);
   local_midRightDragArea = QRect(tmpUpperLeft, tmpBottomRight);

   // bottom left dragbox
   newUpperLeft_X = local_rectangularSelection.topLeft().x() - 3;
   newUpperLeft_Y = local_rectangularSelection.bottomRight().y() - 3;
   newBottomRight_X = local_rectangularSelection.topLeft().x() + 3;
   newBottomRight_Y = local_rectangularSelection.bottomRight().y() + 3;
   tmpUpperLeft = QPoint(newUpperLeft_X, newUpperLeft_Y);
   tmpBottomRight = QPoint(newBottomRight_X, newBottomRight_Y);
   local_bottomLeftDragArea = QRect(tmpUpperLeft, tmpBottomRight);

   // bottom mid dragbox
   newUpperLeft_X = local_rectangularSelection.topLeft().x() - 3 +
      int(qreal(local_rectangularSelection.bottomRight().x() - local_rectangularSelection.topLeft().x()) / 2.0);
   newUpperLeft_Y = local_rectangularSelection.bottomRight().y() - 3;
   newBottomRight_X = local_rectangularSelection.topLeft().x() + 3 +
      int(qreal(local_rectangularSelection.bottomRight().x() - local_rectangularSelection.topLeft().x()) / 2.0);
   newBottomRight_Y = local_rectangularSelection.bottomRight().y() + 3;
   tmpUpperLeft = QPoint(newUpperLeft_X, newUpperLeft_Y);
   tmpBottomRight = QPoint(newBottomRight_X, newBottomRight_Y);
   local_bottomMidDragArea = QRect(tmpUpperLeft, tmpBottomRight);

   // bottom right dragbox
   newUpperLeft_X = local_rectangularSelection.bottomRight().x() - 3;
   newUpperLeft_Y = local_rectangularSelection.bottomRight().y() - 3;
   newBottomRight_X = local_rectangularSelection.bottomRight().x() + 3;
   newBottomRight_Y = local_rectangularSelection.bottomRight().y() + 3;
   // Use bottomRight coordinates from ths dragbox for the eraseSelection bottomRight point
   deleteRectangleBottomRight_X = newBottomRight_X + 1;
   deleteRectangleBottomRight_Y = newBottomRight_Y + 1;
   tmpUpperLeft = QPoint(newUpperLeft_X, newUpperLeft_Y);
   tmpBottomRight = QPoint(newBottomRight_X, newBottomRight_Y);
   local_bottomRightDragArea = QRect(tmpUpperLeft, tmpBottomRight);

   // create delete selection area
   QPoint eraseUpperLeft(deleteRectangleUpperLeft_X, deleteRectangleUpperLeft_Y);
   QPoint eraseBottomRight(deleteRectangleBottomRight_X, deleteRectangleBottomRight_Y);
   local_rectangularSelectionErase = QRect(eraseUpperLeft, eraseBottomRight);
}

void CRectangularSelection::defineWorldRects()
{
   // coordinates for the eraseSelectionArea
   int deleteRectangleUpperLeft_X = 0;
   int deleteRectangleUpperLeft_Y = 0;
   int deleteRectangleBottomRight_X = 0;
   int deleteRectangleBottomRight_Y = 0;

   // upper left dragbox
   int newUpperLeft_X = rectangularSelection.topLeft().x() - 3;
   int newUpperLeft_Y = rectangularSelection.topLeft().y() - 3;
   int newBottomRight_X = rectangularSelection.topLeft().x() + 3;
   int newBottomRight_Y = rectangularSelection.topLeft().y() + 3;
   // Use topleft coordinates from ths dragbox for the eraseSelection topleft point
   deleteRectangleUpperLeft_X = newUpperLeft_X;
   deleteRectangleUpperLeft_Y = newUpperLeft_Y;
   QPoint tmpUpperLeft(newUpperLeft_X, newUpperLeft_Y);
   QPoint tmpBottomRight(newBottomRight_X, newBottomRight_Y);
   upperLeftCornerDragArea = QRect(tmpUpperLeft, tmpBottomRight);

   // upper mid dragbox
   newUpperLeft_X = rectangularSelection.topLeft().x() - 3 +
      int(qreal(rectangularSelection.bottomRight().x() - rectangularSelection.topLeft().x()) / 2.0);
   newUpperLeft_Y = rectangularSelection.topLeft().y() - 3;
   newBottomRight_X = rectangularSelection.topLeft().x() + 3 +
      int(qreal(rectangularSelection.bottomRight().x() - rectangularSelection.topLeft().x()) / 2.0);
   newBottomRight_Y = rectangularSelection.topLeft().y() + 3;
   tmpUpperLeft = QPoint(newUpperLeft_X, newUpperLeft_Y);
   tmpBottomRight = QPoint(newBottomRight_X, newBottomRight_Y);
   upperMidDragArea = QRect(tmpUpperLeft, tmpBottomRight);

   // upper right dragbox
   newUpperLeft_X = rectangularSelection.bottomRight().x() - 3;
   newUpperLeft_Y = rectangularSelection.topLeft().y() - 3;
   newBottomRight_X = rectangularSelection.bottomRight().x() + 3;
   newBottomRight_Y = rectangularSelection.topLeft().y() + 3;
   tmpUpperLeft = QPoint(newUpperLeft_X, newUpperLeft_Y);
   tmpBottomRight = QPoint(newBottomRight_X, newBottomRight_Y);
   upperRightDragArea = QRect(tmpUpperLeft, tmpBottomRight);

   // mid left dragbox
   newUpperLeft_X = rectangularSelection.topLeft().x() - 3;
   newUpperLeft_Y = rectangularSelection.topLeft().y() - 3 +
      int((qreal)(rectangularSelection.bottomRight().y() - rectangularSelection.topLeft().y()) / 2.0);
   newBottomRight_X = rectangularSelection.topLeft().x() + 3;
   newBottomRight_Y = rectangularSelection.topLeft().y() + 3 +
      int((qreal)(rectangularSelection.bottomRight().y() - rectangularSelection.topLeft().y()) / 2.0);
   tmpUpperLeft = QPoint(newUpperLeft_X, newUpperLeft_Y);
   tmpBottomRight = QPoint(newBottomRight_X, newBottomRight_Y);
   midLeftDragArea = QRect(tmpUpperLeft, tmpBottomRight);

   // mid right dragbox
   newUpperLeft_X = rectangularSelection.bottomRight().x() - 3;
   newUpperLeft_Y = rectangularSelection.topLeft().y() - 3 +
      int((qreal)(rectangularSelection.bottomRight().y() - rectangularSelection.topLeft().y()) / 2.0);
   newBottomRight_X = rectangularSelection.bottomRight().x() + 3;
   newBottomRight_Y = rectangularSelection.topLeft().y() + 3 +
      int((qreal)(rectangularSelection.bottomRight().y() - rectangularSelection.topLeft().y()) / 2.0);
   tmpUpperLeft = QPoint(newUpperLeft_X, newUpperLeft_Y);
   tmpBottomRight = QPoint(newBottomRight_X, newBottomRight_Y);
   midRightDragArea = QRect(tmpUpperLeft, tmpBottomRight);

   // bottom left dragbox
   newUpperLeft_X = rectangularSelection.topLeft().x() - 3;
   newUpperLeft_Y = rectangularSelection.bottomRight().y() - 3;
   newBottomRight_X = rectangularSelection.topLeft().x() + 3;
   newBottomRight_Y = rectangularSelection.bottomRight().y() + 3;
   tmpUpperLeft = QPoint(newUpperLeft_X, newUpperLeft_Y);
   tmpBottomRight = QPoint(newBottomRight_X, newBottomRight_Y);
   bottomLeftDragArea = QRect(tmpUpperLeft, tmpBottomRight);

   // bottom mid dragbox
   newUpperLeft_X = rectangularSelection.topLeft().x() - 3 +
      int(qreal(rectangularSelection.bottomRight().x() - rectangularSelection.topLeft().x()) / 2.0);
   newUpperLeft_Y = rectangularSelection.bottomRight().y() - 3;
   newBottomRight_X = rectangularSelection.topLeft().x() + 3 +
      int(qreal(rectangularSelection.bottomRight().x() - rectangularSelection.topLeft().x()) / 2.0);
   newBottomRight_Y = rectangularSelection.bottomRight().y() + 3;
   tmpUpperLeft = QPoint(newUpperLeft_X, newUpperLeft_Y);
   tmpBottomRight = QPoint(newBottomRight_X, newBottomRight_Y);
   bottomMidDragArea = QRect(tmpUpperLeft, tmpBottomRight);

   // bottom right dragbox
   newUpperLeft_X = rectangularSelection.bottomRight().x() - 3;
   newUpperLeft_Y = rectangularSelection.bottomRight().y() - 3;
   newBottomRight_X = rectangularSelection.bottomRight().x() + 3;
   newBottomRight_Y = rectangularSelection.bottomRight().y() + 3;
   // Use bottomRight coordinates from ths dragbox for the eraseSelection bottomRight point
   deleteRectangleBottomRight_X = newBottomRight_X + 1;
   deleteRectangleBottomRight_Y = newBottomRight_Y + 1;
   tmpUpperLeft = QPoint(newUpperLeft_X, newUpperLeft_Y);
   tmpBottomRight = QPoint(newBottomRight_X, newBottomRight_Y);
   bottomRightDragArea = QRect(tmpUpperLeft, tmpBottomRight);

   // create delete selection area
   QPoint eraseUpperLeft(deleteRectangleUpperLeft_X, deleteRectangleUpperLeft_Y);
   QPoint eraseBottomRight(deleteRectangleBottomRight_X, deleteRectangleBottomRight_Y);
   rectangularSelectionErase = QRect(eraseUpperLeft, eraseBottomRight);
}

//  Retrieve the cursor shape that corrensponds to the current position of the
//  cursor over the selection
Qt::CursorShape CRectangularSelection::getHoverShape(const QPoint &pt)
{
   if (!confirmGeometries())
   {
      return Qt::ArrowCursor;
   }

   // check if inside upperLeft dragbox
   if (pt.x() >= upperLeftCornerDragArea.topLeft().x() &&
      pt.x() <= upperLeftCornerDragArea.bottomRight().x() &&
      pt.y() >= upperLeftCornerDragArea.topLeft().y() &&
      pt.y() <= upperLeftCornerDragArea.bottomRight().y())
   {
      currentDragArea = UPPER_LEFT_DRAG;
      return Qt::SizeFDiagCursor;
   }

   // check if inside bottomRight dragbox
   else if (pt.x() >= bottomRightDragArea.topLeft().x() &&
      pt.x() <= bottomRightDragArea.bottomRight().x() &&
      pt.y() >= bottomRightDragArea.topLeft().y() &&
      pt.y() <= bottomRightDragArea.bottomRight().y())
   {
      currentDragArea = BOTTOM_RIGHT_DRAG;
      return Qt::SizeFDiagCursor;
   }

   // check if inside midLeft dragbox
   else if (pt.x() >= midLeftDragArea.topLeft().x() &&
      pt.x() <= midLeftDragArea.bottomRight().x() &&
      pt.y() >= midLeftDragArea.topLeft().y() &&
      pt.y() <= midLeftDragArea.bottomRight().y())
   {
      currentDragArea = MID_LEFT_DRAG;
      return Qt::SizeHorCursor;
   }

   // check if inside midRight dragbox
   else if (pt.x() >= midRightDragArea.topLeft().x() &&
      pt.x() <= midRightDragArea.bottomRight().x() &&
      pt.y() >= midRightDragArea.topLeft().y() &&
      pt.y() <= midRightDragArea.bottomRight().y())
   {
      currentDragArea = MID_RIGHT_DRAG;
      return Qt::SizeHorCursor;
   }

   // check if inside midTop dragbox
   else if (pt.x() >= upperMidDragArea.topLeft().x() &&
      pt.x() <= upperMidDragArea.bottomRight().x() &&
      pt.y() >= upperMidDragArea.topLeft().y() &&
      pt.y() <= upperMidDragArea.bottomRight().y())
   {
      currentDragArea = UPPER_MID_DRAG;
      return Qt::SizeVerCursor;
   }

   // check if inside bottom mid dragbox
   else if (pt.x() >= bottomMidDragArea.topLeft().x() &&
      pt.x() <= bottomMidDragArea.bottomRight().x() &&
      pt.y() >= bottomMidDragArea.topLeft().y() &&
      pt.y() <= bottomMidDragArea.bottomRight().y())
   {
      currentDragArea = BOTTOM_MID_DRAG;
      return Qt::SizeVerCursor;
   }

   // check if inside upperRight dragbox
   else if (pt.x() >= upperRightDragArea.topLeft().x() &&
      pt.x() <= upperRightDragArea.bottomRight().x() &&
      pt.y() >= upperRightDragArea.topLeft().y() &&
      pt.y() <= upperRightDragArea.bottomRight().y())
   {
      currentDragArea = UPPER_RIGHT_DRAG;
      return Qt::SizeBDiagCursor;
   }

   // check if inside bottomLeft dragbox
   else if (pt.x() >= bottomLeftDragArea.topLeft().x() &&
      pt.x() <= bottomLeftDragArea.bottomRight().x() &&
      pt.y() >= bottomLeftDragArea.topLeft().y() &&
      pt.y() <= bottomLeftDragArea.bottomRight().y())
   {
      currentDragArea = BOTTOM_LEFT_DRAG;
      return Qt::SizeBDiagCursor;
   }

   // check if inside selection
   else if (pt.x() >= rectangularSelection.topLeft().x() &&
      pt.x() <= rectangularSelection.bottomRight().x() &&
      pt.y() >= rectangularSelection.topLeft().y() &&
      pt.y() <= rectangularSelection.bottomRight().y())
   {
      currentDragArea = SELECTION_DRAG;
      return Qt::OpenHandCursor;
   }
   else
   {
      currentDragArea = NO_DRAG;
      return Qt::ArrowCursor;
   }
}

//  Retrieve the cursor shape that corrensponds to the current position of the
//  cursor over the selection
void CRectangularSelection::dragSelection(int dx, int dy)
{
   if (!confirmGeometries())
   {
      return;
   }

   // world
   outsideUpperLeft = QPoint(outsideUpperLeft.x() + dx, outsideUpperLeft.y() + dy);
   outsideBottomRight = QPoint(outsideBottomRight.x() + dx, outsideBottomRight.y() + dy);
   rectangularSelectionErase.adjust(dx, dy, dx, dy);
   rectangularSelection.adjust(dx, dy, dx, dy);
   upperLeftCornerDragArea.adjust(dx, dy, dx, dy);
   upperMidDragArea.adjust(dx, dy, dx, dy);
   upperRightDragArea.adjust(dx, dy, dx, dy);
   midLeftDragArea.adjust(dx, dy, dx, dy);
   midRightDragArea.adjust(dx, dy, dx, dy);
   bottomLeftDragArea.adjust(dx, dy, dx, dy);
   bottomMidDragArea.adjust(dx, dy, dx, dy);
   bottomRightDragArea.adjust(dx, dy, dx, dy);
   contentRect.adjust(dx, dy, dx, dy);
}

//  Redefine selection coordinates based on argument dx and dy. It is recommended
//  to give the world pixmap as argument to update the content. Otherwise the previous
//  content is remaining constant so that it does no longer fit the rectangular selection
void CRectangularSelection::dragSelectionBox(int dx, int dy, QPixmap *worldContent)
{
   if (0 != extraPixmap && 0 != worldContent && confirmGeometries())
   {
      // if extra pixmap is present, write it back to world and erase both content and extra.
      QPainter extraPainter(worldContent);
      extraPainter.fillRect(rectangularSelection, Qt::white);
      extraPainter.drawPixmap(rectangularSelection, *extraPixmap, extraPixmap->rect());
      extraPainter.end();

      // DEBUGGING
      //QRect debugRect = extraPixmap->rect();
      //emit printSig(debugRect, *extraPixmap, debugRect, QString("Extra Map"));

      delete extraPixmap;
      extraPixmap = 0;
      writeExtraInstead = false;
   }


   // local
   QPoint localBottomRight = local_rectangularSelection.bottomRight();

   // local content
   QPoint localContentUpperLeft = local_contentRect.topLeft();
   QPoint localContentBottomRight = local_contentRect.bottomRight();

   // world
   QPoint worldUpperLeft = rectangularSelection.topLeft();
   QPoint worldUpperRight = rectangularSelection.topRight();
   QPoint worldBottomRight = rectangularSelection.bottomRight();
   QPoint worldBottomLeft = rectangularSelection.bottomLeft();

   // world content
   QPoint worldContentUpperLeft = contentRect.topLeft();
   QPoint worldContentBottomRight = contentRect.bottomRight();

   // set coordinates
   switch (currentDragArea)
   {
   case(UPPER_LEFT_DRAG) :
   {
      // local selection
      localBottomRight.setX(localBottomRight.x() - dx);
      localBottomRight.setY(localBottomRight.y() - dy);

      // local content
      localContentUpperLeft.setX(localContentUpperLeft.x() - dx);
      localContentUpperLeft.setY(localContentUpperLeft.y() - dy);
      localContentBottomRight.setX(localContentBottomRight.x() - dx);
      localContentBottomRight.setY(localContentBottomRight.y() - dy);

      // world selection
      worldUpperLeft.setX(worldUpperLeft.x() + dx);
      worldUpperLeft.setY(worldUpperLeft.y() + dy);
      worldUpperRight.setY(worldUpperRight.y() + dy);
      worldBottomLeft.setX(worldBottomLeft.x() + dx);

      // world content
      worldContentUpperLeft.setX(worldContentUpperLeft.x() - dx);
      worldContentUpperLeft.setY(worldContentUpperLeft.y() - dy);
      worldContentBottomRight.setX(worldContentBottomRight.x() - dx);
      worldContentBottomRight.setY(worldContentBottomRight.y() - dy);

      break;
   }
   case(UPPER_MID_DRAG) :
   {
      // local selection
      localBottomRight.setY(localBottomRight.y() - dy);

      // local content
      localContentUpperLeft.setY(localContentUpperLeft.y() - dy);
      localContentBottomRight.setY(localContentBottomRight.y() - dy);

      // world selection
      worldUpperLeft.setY(worldUpperLeft.y() + dy);
      worldUpperRight.setY(worldUpperRight.y() + dy);

      // world content
      worldContentUpperLeft.setY(worldContentUpperLeft.y() - dy);
      worldContentBottomRight.setY(worldContentBottomRight.y() - dy);

      break;
   }
   case(UPPER_RIGHT_DRAG) :
   {
      // local selection
      localBottomRight.setX(localBottomRight.x() + dx);
      localBottomRight.setY(localBottomRight.y() - dy);

      // local content
      localContentUpperLeft.setY(localContentUpperLeft.y() - dy);
      localContentBottomRight.setY(localContentBottomRight.y() - dy);

      // world selection
      worldUpperLeft.setY(worldUpperLeft.y() + dy);
      worldUpperRight.setY(worldUpperRight.y() + dy);
      worldUpperRight.setX(worldUpperRight.x() + dx);
      worldBottomRight.setX(worldBottomRight.x() + dx);

      // world content
      worldContentUpperLeft.setY(worldContentUpperLeft.y() - dy);
      worldContentBottomRight.setY(worldContentBottomRight.y() - dy);

      break;
   }
   case(MID_LEFT_DRAG) :
   {
      // local selection
      localBottomRight.setX(localBottomRight.x() - dx);

      // local content
      localContentUpperLeft.setX(localContentUpperLeft.x() - dx);
      localContentBottomRight.setX(localContentBottomRight.x() - dx);

      // world selection
      worldUpperLeft.setX(worldUpperLeft.x() + dx);
      worldBottomLeft.setX(worldBottomLeft.x() + dx);

      // world content
      worldContentUpperLeft.setX(worldContentUpperLeft.x() - dx);
      worldContentBottomRight.setX(worldContentBottomRight.x() - dx);

      break;
   }
   case(MID_RIGHT_DRAG) :
   {
      // local selection
      localBottomRight.setX(localBottomRight.x() + dx);

      // world selection
      worldUpperRight.setX(worldUpperRight.x() + dx);
      worldBottomRight.setX(worldBottomRight.x() + dx);

      break;
   }
   case(BOTTOM_LEFT_DRAG) :
   {
      // local selection
      localBottomRight.setX(localBottomRight.x() - dx);
      localBottomRight.setY(localBottomRight.y() + dy);

      // local content
      localContentUpperLeft.setX(localContentUpperLeft.x() - dx);
      localContentBottomRight.setX(localContentBottomRight.x() - dx);

      // world selection
      worldUpperLeft.setX(worldUpperLeft.x() + dx);
      worldBottomLeft.setX(worldBottomLeft.x() + dx);
      worldBottomLeft.setY(worldBottomLeft.y() + dy);
      worldBottomRight.setY(worldBottomRight.y() + dy);

      // world content
      worldContentUpperLeft.setX(worldContentUpperLeft.x() - dx);
      worldContentBottomRight.setX(worldContentBottomRight.x() - dx);

      break;
   }
   case(BOTTOM_MID_DRAG) :
   {
      // local selection
      localBottomRight.setY(localBottomRight.y() + dy);

      // world selection
      worldBottomRight.setY(worldBottomRight.y() + dy);
      worldBottomLeft.setY(worldBottomLeft.y() + dy);

      break;
   }
   case(BOTTOM_RIGHT_DRAG) :
   {
      // local selection
      localBottomRight.setX(localBottomRight.x() + dx);
      localBottomRight.setY(localBottomRight.y() + dy);

      // world selection
      worldUpperRight.setX(worldUpperRight.x() + dx);
      worldBottomRight.setX(worldBottomRight.x() + dx);
      worldBottomRight.setY(worldBottomRight.y() + dy);
      worldBottomLeft.setY(worldBottomLeft.y() + dy);

      break;
   }
   default:
   {
      break;
   }
   }

   // check drag limits
   if ((abs(worldBottomRight.x()) - worldUpperLeft.x() < 14) || (abs(worldBottomRight.y()) - worldUpperLeft.y() < 14))
   {
      return;
   }

   // redefine coordinate rectangles

   // local selection
   local_rectangularSelection.setBottomRight(localBottomRight);

   // world selection
   rectangularSelection.setTopLeft(worldUpperLeft);
   rectangularSelection.setTopRight(worldUpperRight);
   rectangularSelection.setBottomRight(worldBottomRight);
   rectangularSelection.setBottomLeft(worldBottomLeft);

   if (0 != worldContent)
   {
      // in this case the content pixmap is getting redefined and redrawn from the world content

      // delete old
      delete content;
      content = 0;

      // TODO : 03.12.2011 : rFigure out what to when user drag on dragbow, and the selection has extra content from composition

      // make new
      contentRect = rectangularSelection;
      content = new QPixmap(rectangularSelection.size());
      content->fill(Qt::white);
      local_contentRect = content->rect();

      // draw world content at world coordinates to local content pixmap at local coordinates
      QPainter redrawPainter(content);
      redrawPainter.drawPixmap(local_contentRect, *worldContent, rectangularSelection);
      redrawPainter.end();
   }
   else
   {
      // local content
      local_contentRect.setTopLeft(localContentUpperLeft);
      local_contentRect.setBottomRight(localContentBottomRight);

      // world content
      contentRect.setTopLeft(worldContentUpperLeft);
      contentRect.setBottomRight(worldContentBottomRight);
   }

   // define coordinates for all rectangles, both world and local
   defineWorldRects();
   defineLocalRects();

   // resize
   totalSize.setWidth(local_rectangularSelectionErase.width());
   totalSize.setHeight(local_rectangularSelectionErase.height());

   if (0 != bottomPixmap)
   {
      // NOTE : this probably isn't effective performancewise, but currently isnt a bottleneck, keep for now
      delete bottomPixmap;
      bottomPixmap = 0;
   }

   bottomPixmap = new QPixmap(totalSize);

   // redraw
   bottomPixmap->fill(Qt::white);

   drawSelection(/*true, */0, false);
}

//  On some occasions, it is necessary to make an extra copy of the world pixmap
//  at the selection. This function will write that area to the local worldShadow pixmap
void CRectangularSelection::makeWorldShadow(const QPixmap *worldPixmap)
{
   if (0 == worldPixmap ||
      !confirmGeometries())
   {
      return;
   }

   // define the pixmap, erase old one if it exists
   if (0 != worldShadow)
   {
      delete worldShadow;
      worldShadow = 0;
   }
   worldShadow = new QPixmap(local_rectangularSelection.size());

   // draw world to shadow
   QPainter painter(worldShadow);
   painter.drawPixmap(worldShadow->rect(), *worldPixmap, rectangularSelection);
   painter.end();

   hasShadow = true;
}

void CRectangularSelection::updateContent(const QPixmap *newContent, const QRect *newContentRect)
{
   // check conditions
   if (0 == newContent)
   {
      // there is nothing to draw, can't continue
      return;
   }
   if (0 != newContentRect &&
      newContentRect->size() != rectangularSelection.size())
   {
      // the argument geometry doesnt match the local geometry of the selection, results would have
      // been scaled if we had continued
      return;
   }
   else if (newContent->rect().size() != this->local_contentRect.size())
   {
      // the argument geometry wasnt defined, so the geometry of the argument pixmap must match the
      // local selection geometry, but it didnt so we can't continue
      return;
   }

   // define and blank out content pixmap
   if (0 == content)
   {
      content = new QPixmap(newContent->rect().size());
   }
   content->fill(Qt::white);

   QPainter painter(content);
   if (0 == newContentRect)
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

void CRectangularSelection::storeThisPixmap(const QPixmap *worldPixmap, bool enforce)
{
   writeExtraInstead = enforce;

   if (!confirmGeometries() ||
      0 == worldPixmap ||
      local_rectangularSelection.size() != worldPixmap->rect().size())
   {
      // arg pixmap must exist and the size must match the local coordinates
      return;
   }
   if (0 == extraPixmap)
   {
      extraPixmap = new QPixmap(local_rectangularSelection.size());
   }

   extraPixmap->fill(Qt::white);
   QPainter painter(extraPixmap);
   painter.drawPixmap(extraPixmap->rect(), *worldPixmap, worldPixmap->rect());
   painter.end();
}

QPixmap* CRectangularSelection::getShadowPixmap()
{
   return worldShadow;
}

QPixmap* CRectangularSelection::getFullPixmap()
{
   return bottomPixmap;
}

QPixmap* CRectangularSelection::getContentPixmap()
{
   return content;
}

QRect CRectangularSelection::getWorldFullRect()
{
   return rectangularSelectionErase;
}

QRect CRectangularSelection::getWorldContentRect()
{
   return rectangularSelection;
}

QRect CRectangularSelection::getLocalFullRect()
{
   return local_rectangularSelectionErase;
}

QRect CRectangularSelection::getLocalContentRect()
{
   return local_rectangularSelection;
}

void CRectangularSelection::changePen(const QPen *pen, bool redrawContent)
{
   drawSelection(/*redrawContent,*/ pen);
}

void CRectangularSelection::toggle()
{
   QPen pen;
   pen.setWidth(1);
   pen.setStyle(Qt::DashLine);

   if(isToggled)
   {
      pen.setColor(Qt::lightGray);
      isToggled = false;
      changePen(&pen, true);
   }
   else
   {
      pen.setColor(Qt::blue);
      isToggled = true;
      changePen(&pen, true);
   }
}

bool CRectangularSelection::getToggle()
{
   return isToggled;
}

TDragArea CRectangularSelection::getCurrentDragArea()
{
   return currentDragArea;
}

bool CRectangularSelection::getHasExtraContent()
{
   return writeExtraInstead;
}

QTransform CRectangularSelection::getTransform()
{
   return transform;
}

QTransform CRectangularSelection::getLastTransform()
{
   return lastTransform;
}

void CRectangularSelection::setLastTransform(QTransform trans)
{
   lastTransform = trans;
}

void CRectangularSelection::deleteTransforms()
{
   transform = QTransform(0,0,0,0,0,0,0,0,0);
   lastTransform = QTransform(0,0,0,0,0,0,0,0,0);
}