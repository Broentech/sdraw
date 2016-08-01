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
//  \file      draw_Segments.cpp
//
//  \brief     Implementation of the draw board selection functionality
//
//  \author    Stian Broen
//
//  \date      16.06.2011
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 16.06.2011 Original version
//
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <math.h>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QImage>
#include <QBitmap>
#include <QCursor>

// local includes
#include "draw_Segments.h"

using namespace sdraw;

CSegmenter::CSegmenter(QObject* parent)
   : CDrawBase(parent),
   isDefiningRect(false),
   hasClearedSelections(false),
   _copyBuffer(0),
   transparentSelection(false),
   inverseSelection(false),
   isStationary(false),
   isDraggingShape(false),
   isTransforming(false),
   squareTrans(false),
   rotate_x_enabled(false),
   rotate_y_enabled(false),
   rotate_z_enabled(false),
   useComposition(false),
   compositionAlpha(255),
   dx(0),
   dy(0),
   currentDragArea(NO_DRAG),
   currentTransformationMode(TRANS_MOVE),
   isBorrow(false)
{
   selectionKeeper = new CSelectionKeeper(this);

   connect(selectionKeeper, SIGNAL(printSig(QRect &, const QPixmap &, QRect &, const QString &)),
      this, SLOT(printPixmap(QRect &, const QPixmap &, QRect &, const QString &)));

}

CSegmenter::~CSegmenter()
{

}

void CSegmenter::updateSelectionTransformationSquare(bool enable)
{
   squareTrans = enable;
}

void CSegmenter::updateSelectionRotationAxis(Qt::Axis axis, bool enable)
{
   switch(axis)
   {
      case(Qt::XAxis):rotate_x_enabled = enable;break;
      case(Qt::YAxis):rotate_y_enabled = enable;break;
      case(Qt::ZAxis):rotate_z_enabled = enable;break;
      default:break;
   }
}

//void C2D_Drawboard_Selection::updateOperation(TOperation operation)
//{
//   _aidBuffer.fill(Qt::white);
//
//   C2D_DrawBoard_Filler::updateOperation(operation);
//   switch(mOperation)
//   {
//      case(DRAW):
//      {
//         isDraggingShape = false;
//         break;
//      }
//      case(POLYGONIZER):
//      {
//         isDraggingShape = false;
//         break;
//      }
//      case(FILLER):
//      {
//         isDraggingShape = false;
//         break;
//      }
//      case(SELECTER):
//      {
//         activeMouse = true;
//         //setMouseTracking(false);
//         aidOn = false;
//         isDraggingShape = false;
//         break;
//      }
//      case(TEXT):
//      {
//         isDraggingShape = false;
//         break;
//      }
//      case(INSPECTER):
//      {
//         isDraggingShape = false;
//         break;
//      }
//      default:
//      {
//         break;
//      }
//   }
//}

void CSegmenter::updateSelectionMode(TSelectionMode selection)
{
   switch(currentSelectionMode)
   {
      case(RECTANGULAR_SELECTION):
      {
         if(selection == POLYGON_SELECTION)
         {
            //setMouseTracking(true);
            emit activateMouseMovement(true);
            emit updateCursorSig(Qt::ArrowCursor);
         }

         break;
      }
      case(POLYGON_SELECTION):
      {
         if(selection == RECTANGULAR_SELECTION)
         {
            polygonSelection.clear();
            //setMouseTracking(true);
            emit activateMouseMovement(true);
            emit updateCursorSig(Qt::ArrowCursor);
         }
         break;
      }
      case(FREE_SELECTION):
      {
         break;
      }
      default:
      {
         break;
      }
   }
   
   currentSelectionMode = selection;
}

void CSegmenter::updateSelectionOption(TSelectionOption selectionOption)
{
   switch(selectionOption)
   {
      case(IGNOREWHITE_SELECTION_ON):
      {
         transparentSelection = true;
         break;
      }
      case(IGNOREWHITE_SELECTION_OFF):
      {
         transparentSelection = false;
         break;
      }
      case(INVERSE_SELECTION_ON):
      {
         inverseSelection = true;
         break;
      }
      case(INVERSE_SELECTION_OFF):
      {
         inverseSelection = false;
         break;
      }
      case(CLONE_SELECTION_ON):
      {
         isBorrow = true;
         break;
      }
      case(CLONE_SELECTION_OFF):
      {
         isBorrow = false;
         break;
      }
      case(COMBINE_SELECTION_ON):
      {
         useComposition = true;
         break;
      }
      case(COMBINE_SELECTION_OFF):
      {
         useComposition = false;
         break;
      }
      default:
      {
         break;
      }
   }
}

void CSegmenter::updateSelectionCompMode(QPainter::CompositionMode mode)
{
   compositionMode = mode;
}

void CSegmenter::updateSelectionTransformation(TTransformationMode mode)
{
   currentTransformationMode = mode;

   switch(currentTransformationMode)
   {
      case(TRANS_MOVE):
      {
         selectionKeeper->endTransformation();
         break;
      }
      case(TRANS_SCALE):
      {
         isTransforming  = false;
         isDraggingShape = false;
         break;
      }
      case(TRANS_ROTATE):
      {
         isTransforming  = false;
         isDraggingShape = false;
         break;
      }
      case(TRANS_SHEAR):
      {
         isTransforming  = false;
         isDraggingShape = false;
         break;
      }
      default:
      {
         break;
      }
   }
}

void CSegmenter::updateSelectionCompAlpha(int value)
{
   compositionAlpha = value;
}

void CSegmenter::performSelectionAction_noPt(TSelectionAction selectionAction)
{
   // TODO
}

void CSegmenter::performSelectionAction(const QPoint &pt, TSelectionAction selectionAction)
{
   switch(currentSelectionMode)
   {
      case(RECTANGULAR_SELECTION):
      {
         rectangularAction(pt, selectionAction);
         break;
      }
      case(POLYGON_SELECTION):
      {
         polygonAction(pt, selectionAction);
         break;
      }
      case(FREE_SELECTION):
      {
         break;
      }
      default:
      {
         break;
      }
   }
}

void CSegmenter::selecterPressEvent(QMouseEvent* event)
{
   mousePressTime = QTime::currentTime();
   QPoint eventPt = event->pos();

   // this class does not perform sdraw, but if sdraw has been performed on the target pixmap
   // we need to use the registered zoomer object to retrieve the correct point
   if(hasRegisteredZoomer() && m_zoomer->hasSource())
   {
      pressPoint = m_zoomer->getBufferPoint(eventPt, true, true);
   }
   else
   {
      pressPoint = eventPt;
   }

   //pressPoint = zoomer->getBufferPoint("_buffer", eventPt, true);
   switch(currentSelectionMode)
   {
      case(RECTANGULAR_SELECTION):
      {
         rectangularPress(pressPoint);
         break;
      }
      case(POLYGON_SELECTION):
      {
         polygonPress(pressPoint);
         break;
      }
      case(FREE_SELECTION):
      {
         break;
      }
      default:
      {
         break;
      }
   }
}

void CSegmenter::selecterMoveEvent(QMouseEvent* event)
{
   QPoint currPoint;// = zoomer->getBufferPoint("_buffer", event->pos(), true);

   QPoint eventPt = event->pos();

   // this class does not perform sdraw, but if sdraw has been performed on the target pixmap
   // we need to use the registered zoomer object to retrieve the correct point
   if(hasRegisteredZoomer() && m_zoomer->hasSource())
   {
      currPoint = m_zoomer->getBufferPoint(eventPt, true, true);
   }
   else
   {
      currPoint = eventPt;
   }

   switch(currentSelectionMode)
   {
      case(RECTANGULAR_SELECTION):
      {
         rectangularMove(currPoint);
         break;
      }
      case(POLYGON_SELECTION):
      {
         polygonMove(currPoint);
         break;
      }
      case(FREE_SELECTION):
      {
         break;
      }
      default:
      {
         break;
      }
   }
}

void CSegmenter::selecterDoubleclickEvent(QMouseEvent* event)
{
   QPoint pt;// = zoomer->getBufferPoint("_buffer", event->pos(), true);

   QPoint eventPt = event->pos();

   // this class does not perform sdraw, but if sdraw has been performed on the target pixmap
   // we need to use the registered zoomer object to retrieve the correct point
   if(hasRegisteredZoomer() && m_zoomer->hasSource())
   {
      pt = m_zoomer->getBufferPoint(eventPt, true, true);
   }
   else
   {
      pt = eventPt;
   }

   switch(currentSelectionMode)
   {
      case(RECTANGULAR_SELECTION):
      {
         //rectangularDoubleClick(doubleClickPoint);
         break;
      }
      case(POLYGON_SELECTION):
      {
         polygonDoubleClick(pt);
         break;
      }
      case(FREE_SELECTION):
      {
         break;
      }
      default:
      {
         break;
      }
   }
}

void CSegmenter::selecterReleaseEvent(QMouseEvent* event)
{
   mouseReleaseTime = QTime::currentTime();

   QPoint releasePoint;// = zoomer->getBufferPoint("_buffer", eventPt, true);

   QPoint eventPt = event->pos();

   // this class does not perform sdraw, but if sdraw has been performed on the target pixmap
   // we need to use the registered zoomer object to retrieve the correct point
   if(hasRegisteredZoomer() && m_zoomer->hasSource())
   {
      releasePoint = m_zoomer->getBufferPoint(eventPt, true, true);
   }
   else
   {
      releasePoint = eventPt;
   }

   int secondsDiff = abs(mouseReleaseTime.second() - mousePressTime.second());
   switch(currentSelectionMode)
   {
      case(RECTANGULAR_SELECTION):
      {
         if(isStationary && secondsDiff <= 1)
         {
            blankAidBuffer();
            //eraseAidRectangles();
            if(!isTransforming)
            {
               selectionKeeper->toggleFocusRect();
               selectionKeeper->finalizeTransformation();
            }

            isDraggingShape = false;
            isTransforming  = false;
            isDefiningRect  = false;
            rectangularMove(releasePoint);
            //setMouseTracking(true);
         }
         else
         {
            rectangularRelease(releasePoint);
         }
         break;
      }
      case(POLYGON_SELECTION):
      {
         polygonRelease(releasePoint);
         polygonMove(releasePoint);
         break;
      }
      //case(FREE_SELECTION):
      //{
      //   break;
      //}
      default:
      {
         break;
      }
   }
   isStationary = true;
}

void CSegmenter::rectangularPress(const QPoint &pressPoint, bool doCopy)
{
   if(!selectionKeeper->hasFocus())
   {
      isDefiningRect = true;
      //setMouseTracking(true);
      emit activateMouseMovement(true);
      aidOn = true;
      _last = pressPoint;
      return; 
   }

   dragStart = pressPoint;
   currentDragArea = selectionKeeper->getFocusDragArea();
   if(currentTransformationMode == TRANS_MOVE)
   {     
      if(currentDragArea != NO_DRAG)
      {
         if(currentDragArea == SELECTION_DRAG)
         {
            doInitSelectionMovement();
         }
         else
         {
            if(hasRegisteredBuffer())
            {
               // user is dragging on a dragbox. Make sure any intersection selections are drawn back to the buffer
               // or else they will disappear
               selectionKeeper->drawIntersectionsToWorld(m_buffer);
            }
         }
         isDraggingShape = true;
         //emit activateMouseMovement(false);
         //setMouseTracking(false);
      }
   }
   else // transformations
   {
      if(currentDragArea == SELECTION_DRAG)
      {
         doInitSelectionTransformation(pressPoint);
      }
      else if(currentDragArea != NO_DRAG)
      {
         if(hasRegisteredBuffer())
         {
            if(hasRegisteredBuffer())
            {
               // user is dragging on a dragbox. Make sure any intersection selections are drawn back to the buffer
               // or else they will disappear
               selectionKeeper->drawIntersectionsToWorld(m_buffer);
            }

            // require mouse left button to be held down to register movement
            //setMouseTracking(false);
            emit activateMouseMovement(false);

            // dragging dragbox
            isDraggingShape = true;
         }
      }
   }

   emit requestUpdate();
}

void CSegmenter::rectangularMove(const QPoint &currPoint)
{
   int xDiff = abs(pressPoint.x () - currPoint.x());
   int yDiff = abs(pressPoint.y () - currPoint.y());

   if(isDefiningRect)
   {
      if(isStationary && (xDiff >= 3 && 
                          yDiff >= 3 ))
      {
         isStationary = false;
      }
      if(hasRegisteredAidBuffer())
      {
         blankAidBuffer();
         drawAidRectangle(currPoint);
      }
   }
   else 
   {
      switch(currentTransformationMode)
      {
         case(TRANS_MOVE):
         {
            if(isDraggingShape)
            {
               
               if(isStationary && (xDiff >= 1 || 
                                   yDiff >= 1 ))
               {
                  isStationary = false;
               }

               dx = currPoint.x() - dragStart.x();
               dy = currPoint.y() - dragStart.y();
               dragStart = currPoint;
               selectionKeeper->setComposition(useComposition, (int)compositionMode, compositionAlpha);
               switch(currentDragArea)
               {
                  case(SELECTION_DRAG):
                  {
                     selectionKeeper->dragToggledPolygonSelection(dx,dy);
                     selectionKeeper->dragToggledRectangleSelections(dx, dy);
                     emit requestUpdate();
                     break;
                  }
                  default:
                  {
                     if(hasRegisteredBuffer())
                     {
                        if(!isBorrow && hasClearedSelections)
                        {
                           // this means the buffer content at the selections has been blanked out, but the blanked out
                           // areas have been kept in the selections. Draw them back to buffer
                           selectionKeeper->castShadows(m_buffer);
                           hasClearedSelections = false;
                        }

                        selectionKeeper->dragBox(dx, dy, m_buffer);
                     }
                     emit requestUpdate();
                     break;
                  }
               }
            }
            else
            {
               Qt::CursorShape shape = selectionKeeper->mouseMovementRect(&currPoint);
               emit updateCursorSig(shape);
            }
            break;
         }
         default: // transformations
         {
            if(isTransforming)
            {
               // transform both rectangular and polygon selections
               selectionKeeper->doTransform(currPoint);
            }
            else if(isDraggingShape)
            {
               // dragbox
               if(isStationary && (xDiff >= 1 || 
                                   yDiff >= 1 ))
               {
                  isStationary = false;
               }

               dx = currPoint.x() - dragStart.x();
               dy = currPoint.y() - dragStart.y();
               dragStart = currPoint;
               selectionKeeper->setComposition(useComposition, (int)compositionMode, compositionAlpha);

               if(hasRegisteredBuffer())
               {
                  if(!isBorrow && hasClearedSelections)
                  {
                     // this means the buffer content at the selections has been blanked out, but the blanked out
                     // areas have been kept in the selections. Draw them back to buffer
                     selectionKeeper->castShadows(m_buffer);
                     hasClearedSelections = false;
                  }

                  selectionKeeper->dragBox(dx, dy, m_buffer);
               }
            }
            else
            {
               Qt::CursorShape shape = selectionKeeper->mouseMovementRect(&currPoint);
               emit updateCursorSig(shape);
            }
            emit requestUpdate();
            break;
         }
      }
   }

//   emit requestUpdate();
}

void CSegmenter::rectangularRelease(const QPoint &releasePoint)
{
   if(isDraggingShape || isTransforming)
   {
      if(isDraggingShape)
      {
         TDragArea currentDragType = selectionKeeper->getFocusDragArea();
         if(currentDragType != NO_DRAG)
         {
            selectionKeeper->collapseSelections();
         }
         if(isBorrow && hasRegisteredBuffer())
         {
            // draw content of selection back to _buffer (dont include rects)
            selectionKeeper->drawFocusToArg(m_buffer);
         }
      }
      if(isTransforming)
      {
         selectionKeeper->finalizeTransformation();
      }
   }
   else
   {
      if((abs(_last.x() - releasePoint.x()) > 12 && 
          abs(_last.y() - releasePoint.y()) > 12))
      {
         selectionKeeper->addRectangularSelection(&_last, &releasePoint, true);
         isDefiningRect = false;
      }

      if(hasRegisteredAidBuffer())
      {
         blankAidBuffer();
         //eraseAidRectangles();
      }
   }

   isDraggingShape = false;
   isTransforming  = false;
   isDefiningRect  = false;
   rectangularMove(releasePoint);
   emit activateMouseMovement(true);
   //setMouseTracking(true);
}

void CSegmenter::rectangularAction(const QPoint &currPos, TSelectionAction selectionAction)
{
   //QPoint currPos = zoomer->getBufferPoint("_buffer", this->mapFromGlobal(QCursor::pos()));
   //QPoint currPos = this->mapFromGlobal(QCursor::pos());
   switch(selectionAction)
   {
      case(COPY_ACTION):
      {
         if(!selectionKeeper->hasFocus())
         {
            return;
         }

         // copy the content of the rectangular selection to the local copy buffer.
         CRectangularSelection *focusRect = selectionKeeper->getFocusRect();
         if(0 == focusRect)
         {
            return;
         }
         if(0 != _copyBuffer)
         {
            delete _copyBuffer;
            _copyBuffer = 0;
         }
         _copyBuffer = new QPixmap(focusRect->getWorldContentRect().size());
		   _copyBuffer->fill(Qt::white);

         QPixmap *contentToCopy = focusRect->getContentPixmap();
         if(0 == contentToCopy)
         {
            return;
         }
         QPainter painter(_copyBuffer);
         painter.drawPixmap(_copyBuffer->rect(), *contentToCopy, contentToCopy->rect());
         painter.end();
         focusRect = 0;

         break;
      }
      case(CUT_ACTION):
      {
         if(!selectionKeeper->hasFocus())
         {
            return;
         }

         // copy the content of the rectangular selection to the local copy buffer.
         CRectangularSelection *focusRect = selectionKeeper->getFocusRect();
         if(0 == focusRect)
         {
            return;
         }
         if(0 != _copyBuffer)
         {
            delete _copyBuffer;
            _copyBuffer = 0;
         }
         _copyBuffer = new QPixmap(focusRect->getWorldContentRect().size());
         _copyBuffer->fill(Qt::white);

         QPixmap *contentToCopy = focusRect->getContentPixmap();
         if(0 == contentToCopy)
         {
            return;
         }

         // copy focus content to local copy buffer
         QPainter copyPainter(_copyBuffer);
         copyPainter.drawPixmap(_copyBuffer->rect(), *contentToCopy, contentToCopy->rect());
         copyPainter.end();

         if(hasRegisteredBuffer())
         {
            // blank out the _buffer content at the focus rectangle
            selectionKeeper->clearWorldAt(m_buffer);
         }

         // blank out the content of the focus rectangle
         selectionKeeper->clearContentAt(-1, true);

         // finished
         focusRect = 0;
         break;
      }
      case(PASTE_ACTION):
      {
         if(0 == _copyBuffer || _copyBuffer->isNull())
         {
            return;
         }

         if(hasRegisteredBuffer())
         {

            QBitmap bmap = _copyBuffer->createMaskFromColor(Qt::white, Qt::MaskInColor);
            _copyBuffer->setMask(bmap);

            //write copy to current mouse position
            QSize copySize = _copyBuffer->size();
            QPoint bl(currPos.x() + copySize.width(), currPos.y() + copySize.height());
            QRect copyRect(currPos, bl);
            copyRect.adjust(3,3,3,3); // cheating, lol
            QPainter painter(m_buffer);
            painter.drawPixmap(copyRect, *_copyBuffer, _copyBuffer->rect());
            copyRect.adjust(-3,-3,-3,-3); // cheating, lol
            painter.end();

            // if copy pixmap overlaps any part of any selection, write the overlapping part to those selections
            selectionKeeper->considerDrawingPixmap(_copyBuffer, copyRect);
            selectionKeeper->makeWorldShadows();
            selectionKeeper->drawAllSelections(m_buffer, false);
         }
         break;
      }
      case(DELETE_ACTION):
      {
         // only allow delete on focus selections
         if(!selectionKeeper->hasFocus())
         {
            break;
         }
         
         if(hasRegisteredBuffer())
         {
            // blank out the _buffer content at the focus rectangle
            selectionKeeper->clearWorldAt(m_buffer);
         }
         
         // blank out the content of the focus rectangle
         selectionKeeper->clearContentAt(-1, true);

         // finished
         break;
      }
      case(CANCEL_ACTION):
      {
         // only allow cancel on focus selections
         if(!selectionKeeper->hasFocus())
         {
            break;
         }

         if(hasRegisteredBuffer())
         {
            // draw focus rect to buffer
            selectionKeeper->drawFocusToArg(m_buffer);
         }

         // delete focus selection
         selectionKeeper->deleteFocusRect();

         // return to default cursor
         emit updateCursorSig(Qt::ArrowCursor);

         // finished
         break;
      }
      default:
      {
         break;
      }
   }

   // refresh state by doing a "fake" move
   rectangularMove(currPos);
}

void CSegmenter::polygonPress(const QPoint &pressPoint)
{
   TPolygonDragArea pressArea = selectionKeeper->getPressPointArea(pressPoint);
   if(pressArea == POLY_NO_DRAG)
   {
      selectionKeeper->setIsDefiningPolygon();

      if(polygon.size() == 0)
      {
         emit activateMouseMovement(true);
         //setMouseTracking(true);
      }
      polygonSelection << pressPoint;
      aidOn = true;
   }
   else if(pressArea == POLY_SELECTION_DRAG)
   {
      selectionKeeper->setIsDraggingPolygonSelection();
      if(currentTransformationMode == TRANS_MOVE)
      {
         doInitSelectionMovement();
         polygonPressPoint = pressPoint;
         emit activateMouseMovement(false);
         //setMouseTracking(false);
      }
      else // TRANSFORMATIONS
      {
         doInitSelectionTransformation(pressPoint);
      }
   }
   else // POLY_BOX_DRAG
   {
      polygonPressPoint = pressPoint;
      emit activateMouseMovement(false);
      //setMouseTracking(false);

      selectionKeeper->setIsDraggingPolygonBox();

      if(hasRegisteredBuffer())
      {
         // user is dragging on a dragbox. Make sure any intersection selections are drawn back to the buffer
         // or else they will disappear
         selectionKeeper->drawIntersectionsToWorld(m_buffer);
      }
   }

   emit requestUpdate();
}

void CSegmenter::polygonMove(const QPoint &currPoint)
{
   dx = currPoint.x() - polygonPressPoint.x();
   dy = currPoint.y() - polygonPressPoint.y();
   polygonPressPoint = currPoint;

   if(selectionKeeper->getIsDefiningPolygon())
   {
      if(polygonSelection.size() == 1 && hasRegisteredAidBuffer())
      {
         drawAidLine(polygonSelection.at(polygonSelection.size() - 1), currPoint);
      }
      else if(polygonSelection.size() > 1)
      {
         polygonSelection << currPoint;
         if(hasRegisteredAidBuffer())
         {
            drawAidPolygon(polygonSelection);
         }
         polygonSelection.pop_back();
      }
   }
   else if(selectionKeeper->getIsDraggingPolygonSelection())
   {
      if(isTransforming)
      {
         // transform both rectangular and polygon selections
         selectionKeeper->doTransform(currPoint);
      }
      else
      {
         selectionKeeper->setComposition(useComposition, (int)compositionMode, compositionAlpha);
         selectionKeeper->dragToggledRectangleSelections(dx, dy);
         selectionKeeper->dragToggledPolygonSelection(dx,dy);
      }
   }
   else if(selectionKeeper->getIsDraggingPolygonBox())
   {
      if(!isBorrow && hasClearedSelections)
      {
         // this means the buffer content at the selections has been blanked out, but the blanked out
         // areas have been kept in the selections. Draw them back to buffer
         if(hasRegisteredBuffer())
         {
            selectionKeeper->castShadows(m_buffer);
         }
         hasClearedSelections = false;
      }
      selectionKeeper->dragPolygonDragbox(dx,dy);
   }
   else if(selectionKeeper->getIsStatic())
   {
      // update the cursor
      TPolygonDragArea arrowArea = selectionKeeper->getPressPointArea(currPoint, false);
      switch(arrowArea)
      {
         case(POLY_NOT_DEFINED):
         {
            emit updateCursorSig(Qt::ArrowCursor);
            break;
         }
         case(POLY_NO_DRAG):
         {
            emit updateCursorSig(Qt::ArrowCursor);
            break;
         }
         case(POLY_SELECTION_DRAG):
         {
            emit updateCursorSig(Qt::OpenHandCursor);
            break;
         }
         case(POLY_BOX_DRAG):
         {
            emit updateCursorSig(Qt::SizeAllCursor);
            break;
         }
         default:
         {
            emit updateCursorSig(Qt::ArrowCursor);
            break;
         }
      }
      selectionKeeper->mouseMovementPolygon(&currPoint);
   }
   else
   {
      // weird stuff
   }
   emit requestUpdate();
}

void CSegmenter::polygonDoubleClick(const QPoint &doubleClickPoint)
{
   if(selectionKeeper->getIsDefiningPolygon() && polygonSelection.size() > 2)
   {
      // create polygon selection object
      selectionKeeper->addPolygonSelection(polygonSelection);

      if(hasRegisteredAidBuffer())
      {
         // clear the aid rectangle
         blankAidBuffer();
      }

      // reset the temporary polygon selection
      polygonSelection.clear();

      // stop defining polygon
      selectionKeeper->setIsStatic();
   }

   emit requestUpdate();
}

void CSegmenter::polygonRelease(const QPoint &releasePoint)
{
   polygonPressPoint.setX(0);
   polygonPressPoint.setY(0);
   
   if(!selectionKeeper->getIsDefiningPolygon())
   {
      selectionKeeper->setIsStatic();
   }

   if(isTransforming)
   {
      isTransforming  = false;
      selectionKeeper->setReleasePoint(releasePoint);
      selectionKeeper->finalizeTransformation();
      return;
   }

   TPolygonDragArea releaseArea = selectionKeeper->getPressPointArea(releasePoint);
   switch(releaseArea)
   {
      case(POLY_NOT_DEFINED):
      {
         emit updateCursorSig(Qt::ArrowCursor);
         break;
      }
      case(POLY_NO_DRAG):
      {
         emit updateCursorSig(Qt::ArrowCursor);
         break;
      }
      case(POLY_SELECTION_DRAG):
      {
         selectionKeeper->collapseSelections();
         if(isBorrow && hasRegisteredBuffer())
         {
            // draw content of selection back to _buffer
            selectionKeeper->drawFocusToArg(m_buffer);
         }
         selectionKeeper->toggleFocusPolygon();
         emit updateCursorSig(Qt::OpenHandCursor);
         break;
      }
      case(POLY_BOX_DRAG):
      {
         emit updateCursorSig(Qt::SizeAllCursor);
         break;
      }
      default:
      {
         emit updateCursorSig(Qt::ArrowCursor);
         break;
      }
   }
   selectionKeeper->stopMovingRects();
   emit requestUpdate();
}

void CSegmenter::polygonAction(const QPoint &currPos, TSelectionAction selectionAction)
{
   //QPoint currPos = zoomer->getBufferPoint("_buffer",this->mapFromGlobal(QCursor::pos()));
   switch(selectionAction)
   {
      case(COPY_ACTION):
      {
         if(!selectionKeeper->hasPolygonFocus())
         {
            // no selection in focus, need to mouse over an existing polygon selection
            return;
         }

         // get the polygon selection in focus
         CPolygonSelection *focusPoly = selectionKeeper->getFocusPolygon();
         if(0 == focusPoly)
         {
            return;
         }

         if(0 != _copyBuffer)
         {
            delete _copyBuffer;
            _copyBuffer = 0;
         }
         _copyBuffer = new QPixmap(focusPoly->getWorldPolygon().boundingRect().size());
		   _copyBuffer->fill(Qt::white);

         QPixmap *contentToCopy = focusPoly->getContentPixmap();
         if(0 == contentToCopy)
         {
            focusPoly = 0;
            return;
         }

         QPainterPath path;
         path.addPolygon(focusPoly->getLocalPolygon());
         QPainter painter(_copyBuffer);
         painter.setClipPath(path);
         painter.drawPixmap(_copyBuffer->rect(), *contentToCopy, contentToCopy->rect());
         painter.end();
         focusPoly = 0;

         break;
      }
      case(CUT_ACTION):
      {
         if(!selectionKeeper->hasPolygonFocus())
         {
            // no selection in focus, need to mouse over an existing polygon selection
            return;
         }

         // get the polygon selection in focus
         CPolygonSelection *focusPoly = selectionKeeper->getFocusPolygon();
         if(0 == focusPoly)
         {
            return;
         }

         if(0 != _copyBuffer)
         {
            delete _copyBuffer;
            _copyBuffer = 0;
         }
         _copyBuffer = new QPixmap(focusPoly->getWorldPolygon().boundingRect().size());
		   _copyBuffer->fill(Qt::white);

         QPixmap *contentToCopy = focusPoly->getContentPixmap();
         if(0 == contentToCopy)
         {
            focusPoly = 0;
            return;
         }

         QPainterPath path;
         path.addPolygon(focusPoly->getLocalPolygon());
         QPainter painter(_copyBuffer);
         painter.setClipPath(path);
         painter.drawPixmap(_copyBuffer->rect(), *contentToCopy, contentToCopy->rect());
         painter.end();

         if(hasRegisteredBuffer())
         {
            // blank out the _buffer content at the focus rectangle
            selectionKeeper->clearWorldAt(m_buffer);
         }

         // blank out the content of the focus rectangle
         selectionKeeper->clearPolygonContentAt(-1, true);

         focusPoly = 0;

         break;
      }
      case(PASTE_ACTION):
      {

         if(0 == _copyBuffer || _copyBuffer->isNull())
         {
            return;
         }

         if(hasRegisteredBuffer())
         {

            QBitmap bmap = _copyBuffer->createMaskFromColor(Qt::white, Qt::MaskInColor);
            _copyBuffer->setMask(bmap);

            //write copy to current mouse position
            QSize copySize = _copyBuffer->size();
            QPoint bl(currPos.x() + copySize.width(), currPos.y() + copySize.height());
            QRect copyRect(currPos, bl);
            copyRect.adjust(3,3,3,3); // cheating, lol
            QPainter painter(m_buffer);
            painter.drawPixmap(copyRect, *_copyBuffer, _copyBuffer->rect());
            copyRect.adjust(-3,-3,-3,-3); // cheating, lol
            painter.end();

            // if copy pixmap overlaps any part of any selection, write the overlapping part to those selections
            selectionKeeper->considerDrawingPixmap(_copyBuffer, copyRect);
            selectionKeeper->makeWorldShadows();
            selectionKeeper->drawAllSelections(m_buffer, false);
         }
         break;
      }
      case(DELETE_ACTION):
      {
         if(!selectionKeeper->hasPolygonFocus())
         {
            // no selection in focus, need to mouse over an existing polygon selection
            return;
         }

         if(hasRegisteredBuffer())
         {
            // blank out the _buffer content at the focus polygon
            selectionKeeper->clearWorldAt(m_buffer);
         }

         // blank out the content of the focus rectangle
         selectionKeeper->clearPolygonContentAt(-1, true);

         // finished
         break;
      }
      case(CANCEL_ACTION):
      {
         if(hasRegisteredBuffer())
         {
            // draw focus rect to buffer
            selectionKeeper->drawFocusToArg(m_buffer);
         }

         // delete focus selection
         selectionKeeper->deleteFocusPolygon();

         // return to default cursor
         emit updateCursorSig(Qt::ArrowCursor);

         // finished
         break;
      }
      default:
      {
         break;
      }
   }

   emit requestUpdate();
}

void CSegmenter::paintSelections(QPaintDevice *buffer)
{
   // draw buffer content first
   //C2D_DrawBoard_Draw::paintEvent(event);
   if(0 == selectionKeeper)
   {
      return;
   }

   if(buffer == 0)
   {
      // draw to internal buffer reference
      if(hasRegisteredBuffer())
      {
         if(hasRegisteredZoomer())
         {
            // draw content of the selection keeper
            selectionKeeper->drawAllSelections(m_buffer, true, m_zoomer, m_bufferName);
         }
         else
         {
            selectionKeeper->drawAllSelections(m_buffer, true);
         }
      }
   }
   else
   {
      // draw to argument buffer
      if(hasRegisteredZoomer())
      {
         // draw content of the selection keeper
         selectionKeeper->drawAllSelections(buffer, true, m_zoomer, m_bufferName);
      }
      else
      {
         selectionKeeper->drawAllSelections(buffer, true);
      }
   }
}

void CSegmenter::doInitSelectionMovement()
{
   // write buffer content at selection rectangles to selections
   emit updateCursorSig(Qt::ClosedHandCursor);
   if(!isBorrow)
   {                  
      // before blanking out the buffer content at the selections, make a "world shadow" so that the
      // selections can now what was there before it was blanked out
      if(!hasClearedSelections)
      {
         selectionKeeper->makeWorldShadows();
      }

      if(hasRegisteredBuffer())
      {
         // blank out, except selections that has "extra" content (because of composition)
         selectionKeeper->clearWorldAll(m_buffer);
         hasClearedSelections = true;
      }
   }
}

void CSegmenter::doInitSelectionTransformation(QPoint pt)
{
   // let the selection keeper know how we want to transform
   selectionKeeper->setTransformation(currentTransformationMode, 
                                      squareTrans, 
                                      rotate_x_enabled, 
                                      rotate_y_enabled, 
                                      rotate_z_enabled);

   // refresh all content of the selections with the current world pixmap
   selectionKeeper->updateAllSelections();

   if(hasRegisteredBuffer())
   {
      // blank out buffer at selections
      selectionKeeper->clearWorldAll(m_buffer, true);
   }

   // remember the point where transformation started
   selectionKeeper->setPressPoint(pt);

   // indicate transformation with closed hand cursor
   emit updateCursorSig(Qt::ClosedHandCursor);

   // set state
   isTransforming = true;
}


void CSegmenter::printPixmap(QRect &target, const QPixmap &content, QRect &source, const QString &text, bool doUpdate)
{
   if(content.isNull() || target.isNull() || hasRegisteredBuffer())
   {
      return;
   }

   QSize contentSize = content.size();
   QSize targetSize = target.size();
   QSize sourceSize = source.size();
   QRect delRect(0,0,200,500);
   QPainter painter(m_buffer);
   painter.fillRect(delRect, Qt::white);
   painter.fillRect(target, Qt::white);
   painter.drawRect(target);
   painter.drawText(target.topLeft(), text);
   painter.drawPixmap(target, content, source);
   painter.end();

   if(doUpdate)
   {
      emit requestUpdate();
   }
}

void CSegmenter::registerBuffer(QPixmap *buffer)
{
   if(0 == buffer)
   {
      selectionKeeper->releaseBuffer();
   }
   else
   {
      isDraggingShape = false;
      selectionKeeper->registerBuffer(buffer);
   }
   CDrawBase::registerBuffer(buffer);
}

void CSegmenter::releaseBuffer()
{
   selectionKeeper->releaseBuffer();
   CDrawBase::releaseBuffer();
}

bool CSegmenter::hasRegisteredBuffer()
{
   return selectionKeeper->getHasBuffer();
}
void CSegmenter::copyActionTriggered(const QPoint &pt, bool)
{
   performSelectionAction(pt, COPY_ACTION);
   emit requestUpdate();
}
void CSegmenter::cutActionTriggered(const QPoint &pt, bool)
{
   performSelectionAction(pt, CUT_ACTION);
   emit requestUpdate();
}
void CSegmenter::pasteActionTriggered(const QPoint &pt, bool)
{
   performSelectionAction(pt, PASTE_ACTION);
   emit requestUpdate();
}
void CSegmenter::deleteActionTriggered(const QPoint &pt, bool)
{
   performSelectionAction(pt, DELETE_ACTION);
   emit requestUpdate();
}
void CSegmenter::cancelActionTriggered(const QPoint &pt, bool)
{
   performSelectionAction(pt, CANCEL_ACTION);
   emit requestUpdate();
}