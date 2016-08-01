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
//  \file      drawboard.cpp
//
//  \brief     Implementation of the draw board base class
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
//
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QAction>
#include <QPrintPreviewDialog>
#include <QPrintDialog>
#include <QPrinter>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QImage>
#include <QPainter>
#include <QBitmap>
#include <QCursor>
#include <QFile>
#include <QDir>
#include <QUndoStack>
#include <QUndoCommand>

// solution includes
#include <_debugStuff/imageDialog.h>

// local includes
#include "drawboard.h"

using namespace sdraw;

class CAddPixmapSegmentCommand : public QUndoCommand
{
public:
   CAddPixmapSegmentCommand(CDrawBoard *drawBoard = 0,
      C2DPixmapSegment *pixmapSegment = 0,
      QUndoCommand *parent = 0)
      : QUndoCommand(parent),
      m_drawBoard(drawBoard),
      m_pixmapSegment(pixmapSegment)
   {

   }

   void undo()
   {
      if (0 == m_drawBoard)
      {
         return;
      }
      m_drawBoard->deletePixmapSegment(m_pixmapSegment->getIndex());
   }
   void redo()
   {
      if (0 == m_drawBoard || 0 == m_pixmapSegment)
      {
         return;
      }
      m_drawBoard->addPixmapSegment(m_pixmapSegment);
   }

private:
   CDrawBoard *m_drawBoard;
   C2DPixmapSegment *m_pixmapSegment;
};


CDrawBoard::CDrawBoard(QWidget* parent, const QString &name, const QSize &size)
   : QWidget(parent)
{
   m_undoStack = new QUndoStack(this);

   m_draw = new CDraw2D(this);

   connect(m_draw, SIGNAL(adjustCanvas()),
      this, SIGNAL(adjustCanvas()));

   connect(m_draw, SIGNAL(appendPixmapSegment(C2DPixmapSegment *)),
      this, SLOT(receivePixmapSegment(C2DPixmapSegment *)));

   connect(m_draw, SIGNAL(requestUpdate()),
      this, SLOT(updateSlot()));

   connect(m_draw, SIGNAL(activateMouseMovement(bool)),
      this, SLOT(setMouseActive(bool)));

   m_segmenter = new CSegmenter(this);

   connect(m_segmenter, SIGNAL(updateCursorSig(Qt::CursorShape)),
      this, SLOT(updateCursorSlot(Qt::CursorShape)));

   connect(m_segmenter, SIGNAL(activateMouseMovement(bool)),
      this, SLOT(setMouseActive(bool)));

   connect(m_segmenter, SIGNAL(requestUpdate()),
      this, SLOT(updateSlot()));


   connect(this, SIGNAL(selectionActionFromToolbarSig(TSelectionAction)),
      m_segmenter, SLOT(performSelectionAction_noPt(TSelectionAction)));

   connect(this, SIGNAL(selectionOptionFromToolbarSig(TSelectionOption)),
      m_segmenter, SLOT(updateSelectionOption(TSelectionOption)));

   connect(this, SIGNAL(selectionModeFromToolbarSig(TSelectionMode)),
      m_segmenter, SLOT(updateSelectionMode(TSelectionMode)));

   connect(this, SIGNAL(selectionCompModeFromToolbarSig(QPainter::CompositionMode)),
      m_segmenter, SLOT(updateSelectionCompMode(QPainter::CompositionMode)));

   connect(this, SIGNAL(selectionCompAlphaFromToolbarSig(int)),
      m_segmenter, SLOT(updateSelectionCompAlpha(int)));

   connect(this, SIGNAL(selectionTransformationFromToolbarSig(TTransformationMode)),
      m_segmenter, SLOT(updateSelectionTransformation(TTransformationMode)));

   connect(this, SIGNAL(selectionRotationAxisFromToolbarSig(Qt::Axis, bool)),
      m_segmenter, SLOT(updateSelectionRotationAxis(Qt::Axis, bool)));

   connect(this, SIGNAL(selectionTransformationSquareFromToolbarSig(bool)),
      m_segmenter, SLOT(updateSelectionTransformationSquare(bool)));


   myName = name;
   origSize = size;

   // define debugging pens
   redDebuggingPen.setColor(Qt::red);
   redDebuggingPen.setWidth(10);
   blueDebuggingPen.setColor(Qt::blue);
   blueDebuggingPen.setWidth(10);

   activeMouse = false;

   mainzoomer = new Zoomer(this);
   mainzoomer->addZoomModifier(Qt::ControlModifier);

   aidzoomer = new Zoomer(this);
   aidzoomer->addZoomModifier(Qt::ControlModifier);

   arrangeKeySequenceActions();

   arrowCursor = new QCursor(Qt::ArrowCursor);
   openHandCursor = new QCursor(Qt::OpenHandCursor);

   zoomCursor = 0;
   QString cursorZoomPath = ":/images/magnifyingGlass.png";
   if (QFile::QFile(cursorZoomPath).exists())
   {
      QImage iconImage(cursorZoomPath);
      zoomCursor = new QCursor(QPixmap::fromImage(iconImage));
   }

   backgroundColor = Qt::white;
   resize(size);
}

void CDrawBoard::updateSlot()
{
   update();
}

void CDrawBoard::updateCursorSlot(Qt::CursorShape shape)
{
   setCursor(shape);
}

CDrawBoard::~CDrawBoard()
{
   //m_draw->releaseSegmentUndoHash ();
   m_draw->releaseBuffer();
   m_draw->releaseAidBuffer();
   m_draw->releaseZoomer();
   delete m_undoStack;
   delete m_draw;
   delete m_segmenter;
}

void CDrawBoard::initBuffers()
{
   QSize defaultSize = size();
   _aidBuffer = QPixmap(defaultSize);
   _buffer = QPixmap(defaultSize);

   mainzoomer->addSourceRect(_buffer.rect());
   aidzoomer->addSourceRect(_aidBuffer.rect());

   //_aidBuffer.fill(Qt::transparent);
   QPainter painter(&_aidBuffer);
   painter.fillRect(_aidBuffer.rect(), Qt::white);
   painter.end();

   //_buffer.fill(Qt::transparent);
   QPainter bpainter(&_buffer);
   bpainter.fillRect(_buffer.rect(), Qt::white);
   bpainter.end();

   setMouseTracking(true);
}

bool CDrawBoard::event(QEvent *e)
{
   if(e->type() == QEvent::HoverEnter && !this->hasFocus())
   {
      this->setFocus();
   }

   return QWidget::event(e);
}

/*
void C2D_DrawBoard_Base::resizeEvent(QResizeEvent* event)
{
   int eventWidth   = event->size().width();
   int eventHeight  = event->size().height();
   int bufferWidth  = _buffer.width();
   int bufferHeight = _buffer.height();

   if (eventWidth  > bufferWidth ||
       eventHeight > bufferHeight )
   {
      QPixmap tmp(QSize(eventWidth, eventHeight));
      tmp.fill( Qt::white);
      tmp.fill( Qt::transparent );

      QPainter painter( &tmp );
      painter.drawPixmap( _buffer.rect()     , _buffer    , _buffer.rect()   );
      painter.drawPixmap( _aidBuffer.rect()  , _aidBuffer , _aidBuffer.rect());
      //painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
      //painter.fillRect(tmp.rect(), Qt::white);
      _buffer = tmp;
      _aidBuffer = tmp;
   }
   emit newSizeSig(size());
   QWidget::resizeEvent(event);
}
*/

void CDrawBoard::paintEvent( QPaintEvent *event )
{
   drawPixmapSegments(this, mainzoomer);
   QPainter painter(this);
   painter.drawRect(_buffer.rect().adjusted(0,0,-1,-1));
   if(aidOn)
   {
      if(!aidOnly)
      {
         QBitmap bmap = _aidBuffer.createMaskFromColor(backgroundColor);
         _aidBuffer.setMask(bmap);
      }
      aidOnly = false;
      QRect source;
      QRect target;
      if (aidzoomer->getZoomGeo(&source, &target))
      {
         painter.drawPixmap(target,_aidBuffer, source);
      }
      else
      {
         painter.drawPixmap( 0,0, _aidBuffer );
      }
   }

   if(0 != m_segmenter)
   {
      m_segmenter->paintSelections(this);
   }
   QWidget::paintEvent(event);
}

void CDrawBoard::drawPixmapSegments(QPaintDevice *device, Zoomer *mZoomer)
{
   if (0 == device)
   {
      return;
   }

   QPainter painter(device);
   painter.fillRect(_buffer.rect(), Qt::white);
   if (m_segmentHash.size() <= 0)
   {
      return;
   }
   
   C2DPixmapSegment *currSegment = 0;
   QPixmap m_pix;
   QRect m_geo;
   QMapIterator<int, C2DPixmapSegment*> it(m_segmentHash);
   while (it.hasNext())
   {
      it.next();
      currSegment = it.value();
      if (0 == currSegment)
      {
         continue;
      }
      m_pix = currSegment->getPixmap();
      if (m_pix.isNull())
      {
         currSegment = 0;
         continue;
      }
      m_geo = currSegment->getGeometry();
      if (m_geo.isEmpty() || m_geo.isNull())
      {
         currSegment = 0;
         continue;
      }

      int currOffset = qRound((qreal)currSegment->getOffset() / 2.0);
      if (currOffset != 0)
      {
         m_geo.setBottomRight(QPoint(m_geo.bottomRight().x() + 2 * currOffset,
            m_geo.bottomRight().y() + 2 * currOffset));
         m_geo.adjust(-currOffset,
            -currOffset,
            -currOffset,
            -currOffset);
      }

      QRect drawTarget;
      QRect drawSource;
      if (0 != mZoomer)
      {
         drawTarget = mZoomer->getZoomedRect(m_geo);
         drawSource = m_pix.rect();
         if (drawSource.isEmpty() || drawSource.isNull() ||
            drawTarget.isEmpty() || drawTarget.isNull())
         {
            currSegment = 0;
            continue;
         }
      }
      else
      {
         drawSource = m_pix.rect();
         drawTarget = m_geo;
      }

      painter.drawPixmap(drawTarget, m_pix, drawSource);
      currSegment = 0;
   }
   painter.end();
}

void CDrawBoard::mousePressEvent(QMouseEvent* event)
{
   switch (mOperation)
   {
   case(DRAW) :
   {
      m_draw->drawPressEvent(event);
      break;
   }
   case(FILLER) :
   {
      m_draw->fillerPressEvent(event);
      break;
   }
   case(TEXT) :
   {
      // TODO
      //m_draw->textPressEvent(event);
      break;
   }
   case(SELECTER) :
   {
      m_segmenter->selecterPressEvent(event);
      break;
   }
   default:
   {
      break;
   }
   }
}

void CDrawBoard::mouseMoveEvent( QMouseEvent* event )
{
   QPoint realPoint = mainzoomer->getBufferPoint(event->pos());
   emit mouseCoordSig(realPoint);
   if(event->modifiers() == Qt::ShiftModifier)
   {
      if(!dragStart.isNull())
      {
         int dx = realPoint.x() - dragStart.x();
         int dy = realPoint.y() - dragStart.y();
         mainzoomer->pan(dx, dy);
         aidzoomer->pan(dx, dy);
         update();
      }
      dragStart = realPoint;
      return;
   }
   dragStart = realPoint;
   if(!activeMouse)
   {
      return;
   }
   switch(mOperation)
   {
      case(DRAW) :
      {
         m_draw->drawMoveEvent(event);
         break;
      }
      case(FILLER):
      {
         m_draw->fillerMoveEvent(event);
         break;
      }
      case(TEXT):
      {
         // TODO
         //m_draw->textMoveEvent(event);
         break;
      }
      case(SELECTER):
      {
         m_segmenter->selecterMoveEvent(event);
         break;
      }
      default:
      {
         break;
      }
   }
}

void CDrawBoard::mouseDoubleClickEvent (QMouseEvent *event)
{
   switch(mOperation)
   {
      case(DRAW) :
      {
         m_draw->drawDoubleclickEvent(event);
         break;
      }
      case(FILLER):
      {
         m_draw->fillerDoubleclickEvent(event);
         break;
      }
      case(TEXT):
      {
         // TODO
         //m_draw->textDoubleclickEvent(event);
         break;
      }
      case(SELECTER):
      {
         m_segmenter->selecterDoubleclickEvent(event);
         break;
      }
      default:
      {
         break;
      }
   }
}


//  This virtual method gets called whenever the users releases the
//  mouse over the window and performs the appropriate action
void CDrawBoard::mouseReleaseEvent( QMouseEvent * event)
{
   switch(mOperation)
   {
      case(DRAW) :
      {
         m_draw->drawReleaseEvent(event);
         break;
      }
      case(FILLER):
      {
         m_draw->fillerReleaseEvent(event);
         break;
      }
      case(TEXT):
      {
         // TODO
         //m_draw->textReleaseEvent(event);
         break;
      }
      case(SELECTER):
      {
         m_segmenter->selecterReleaseEvent(event);
         break;
      }

      default:
      break;
   }
}


void CDrawBoard::wheelEvent(QWheelEvent *event)
{
   mainzoomer->wheelEvent(event);
   aidzoomer->wheelEvent(event);
   update();
}

void CDrawBoard::slotLoad(const QString& filename, QImage *img)
{
   QImage *image = 0;
   if(0 == img)
   {
      // load image
      image = new QImage;
      if(!image->load(filename)) 
      {
         QMessageBox::warning( this, "Load error", "Could not load file" );
         return;
      }
   }
   else
   {
      // image is preloaded
      image = img;
   }

   // copy it into the buffer:
   _buffer = QPixmap( image->size() );
   if(_buffer.isNull())
   {
      QMessageBox::warning( this, "Load error", "Could not load file" );
      return;
   }
   _buffer.fill(Qt::white);
   QPainter painter(&_buffer);
   painter.drawImage(0,0,*image);
   painter.end(); // resize will result in a resize event with _buffer being copied to, so we must close it.

   resize(image->size());
   _aidBuffer = _buffer;

   // set initial zoom parameters
   mainzoomer->clearSourceRect();
   mainzoomer->addSourceRect(_buffer.rect());
   aidzoomer->clearSourceRect();
   aidzoomer->addSourceRect(_aidBuffer.rect());

   // finally, schedule a repaint:
   update();
}

//  This method does the actual saving. We hardcode the file type as
//  BMP. Unix users might want to replace this with something more usual
//  on Unix like PNG.
void CDrawBoard::slotSave(const QString& filename)
{
   if (!_buffer.save(filename))
   {
      QMessageBox::warning(this, "Save error", "Could not save file");
   }
}

void CDrawBoard::slotPrintPreview()
{
   // Exec'ed dialogs may be allocated on the stack, but its dangerous in
   // case you one day want to show() it, and forget about it being on the
   // stack.
   QPrintPreviewDialog dialog(this);
   connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(paintOnPrinter(QPrinter*)));
   dialog.exec();
}

void CDrawBoard::slotPrint()
{
   QPrinter printer;
   QPrintDialog dialog(&printer, this);
   if (!dialog.exec())
   {
      return;
   }
   paintOnPrinter(&printer);
}

void CDrawBoard::paintOnPrinter(QPrinter* printer)
{
   const QRect pageRect = printer->pageRect();
   const int paperWidth = pageRect.width();
   const int paperHeight = pageRect.height();

   QPainter painter(printer);
   bool first = true;
   int page = 0;
   const int margin = 3; // margin to ensure that the box doesn't overlap with the image.
   for (int y = 0; y < height(); y += paperHeight - margin)
   {
      for (int x = 0; x < width(); x += paperWidth - margin)
      {
         ++page;
         if (!first)
         {
            printer->newPage();
         }
         first = false;

         // Draw the image
         painter.drawPixmap(QPoint(1, 1), _buffer, QRect(x, y, paperWidth - margin, paperHeight - margin));

         // Draw a page number
         painter.drawText(paperWidth - 50, paperHeight - 20, QString("Page %1").arg(page));

         // Draw the rect indicating the page boundaries.
         painter.drawRect(0, 0, paperWidth - 1, paperHeight - 1);
      }
   }
}

void CDrawBoard::updateSettings(CSettings &settings)
{
   _settings = settings;
}

void CDrawBoard::updateOperation(TOperation operation)
{
   mOperation = operation;

   switch(mOperation)
   {
   case(DRAW):
      if(0 != m_draw)
      {
         m_draw->updateOperation(operation);
         m_draw->registerBuffer(&_buffer);
         m_draw->registerAidBuffer(&_aidBuffer);
         m_draw->registerZoomer(mainzoomer, "_buffer", "_aidbuffer");
      }
      if (0 != m_segmenter)
      {
         m_segmenter->releaseBuffer();
         m_segmenter->releaseAidBuffer();
         m_segmenter->releaseZoomer();
      }
      activeMouse = false;
      break;

   case(FILLER):
      if(0 != m_draw)
      {
         m_draw->updateOperation(operation);
         m_draw->registerBuffer         (&_buffer      );
         m_draw->registerAidBuffer      (&_aidBuffer   );
         m_draw->registerZoomer(mainzoomer, "_buffer", "_aidbuffer");
      }
      if (0 != m_segmenter)
      {
         m_segmenter->releaseBuffer();
         m_segmenter->releaseAidBuffer();
         m_segmenter->releaseZoomer();
      }
      activeMouse = true;
      break;

   case(SELECTER):
      if(0 != m_draw)
      {
         m_draw->releaseBuffer();
         m_draw->releaseAidBuffer();
         m_draw->releaseZoomer();
      }
      if (0 != m_segmenter)
      {
         m_segmenter->registerBuffer(&_buffer);
         m_segmenter->registerAidBuffer(&_aidBuffer);
         m_segmenter->registerZoomer(mainzoomer, "_buffer", "_aidbuffer");
      }
      activeMouse = false;
      break;

   default:
      if (0 != m_draw)
      {
         m_draw->releaseBuffer();
         m_draw->releaseAidBuffer();
         m_draw->releaseZoomer();
      }
      if (0 != m_segmenter)
      {
         m_segmenter->releaseBuffer();
         m_segmenter->releaseAidBuffer();
         m_segmenter->releaseZoomer();
      }
      break;
   }
}

void CDrawBoard::arrangeKeySequenceActions()
{
   copyAction = new QAction(this);
   cutAction = new QAction(this);
   pasteAction = new QAction(this);
   deleteAction = new QAction(this);
   cancelAction = new QAction(this);
   zoomInAction = new QAction(this);
   zoomOutAction = new QAction(this);

   copyAction->setShortcut(QKeySequence::Copy);
   cutAction->setShortcut(QKeySequence::Cut);
   pasteAction->setShortcut(QKeySequence::Paste);
   deleteAction->setShortcut(QKeySequence::Delete);
   cancelAction->setShortcut(Qt::Key_Escape);
   zoomInAction->setShortcut(QKeySequence::ZoomIn);
   zoomOutAction->setShortcut(QKeySequence::ZoomOut);

   this->addAction(copyAction);
   this->addAction(cutAction);
   this->addAction(pasteAction);
   this->addAction(deleteAction);
   this->addAction(cancelAction);
   this->addAction(zoomInAction);
   this->addAction(zoomOutAction);

   connect(copyAction, SIGNAL(triggered(bool)), 
      this, SLOT(copyActionTriggered(bool)));

   connect(cutAction, SIGNAL(triggered(bool)), 
      this, SLOT(cutActionTriggered(bool)));

   connect(pasteAction, SIGNAL(triggered(bool)), 
      this, SLOT(pasteActionTriggered(bool)));

   connect(deleteAction, SIGNAL(triggered(bool)), 
      this, SLOT(deleteActionTriggered(bool)));

   connect(cancelAction, SIGNAL(triggered(bool)), 
      this, SLOT(cancelActionTriggered(bool)));

   connect(zoomInAction, SIGNAL(triggered(bool)), 
      this, SLOT(zoomInActionTriggered(bool)));

   connect(zoomOutAction, SIGNAL(triggered(bool)), 
      this, SLOT(zoomOutActionTriggered(bool)));
}

void CDrawBoard::keyPressEvent(QKeyEvent *event)
{
   if (0 == event)
   {
      return;
   }

   if (event->key() == Qt::Key_Control && 0 != zoomCursor)
   {
      setCursor(*zoomCursor);
   }
   else if (event->key() == Qt::Key_Shift)
   {
      if (0 != openHandCursor)
      {
         setCursor(*openHandCursor);
      }
   }

   QWidget::keyPressEvent(event);
}

void CDrawBoard::keyReleaseEvent(QKeyEvent *event)
{
   if (0 == event)
   {
      return;
   }

   if (0 != arrowCursor)
   {
      setCursor(*arrowCursor);
   }

   QWidget::keyReleaseEvent(event);
}

void CDrawBoard::deletePixmapSegment(int index)
{
   if (m_segmentHash.contains(index))
   {
      m_segmentHash.remove(index);
   }
   drawPixmapSegments(&_buffer);
   update();
}

void CDrawBoard::addPixmapSegment(C2DPixmapSegment *segment)
{
   if (0 == segment)
   {
      return;
   }
   m_segmentHash.insert(segment->getIndex(), segment);
   drawPixmapSegments(&_buffer);

   //DebugImageDialog DBG(_buffer);
   //DBG.exec();

   update();
}

void CDrawBoard::receivePixmapSegment(C2DPixmapSegment *segment)
{
   if (0 == segment)
   {
      return;
   }

   int checkIndex = segment->getIndex();
   while (m_segmentHash.contains(checkIndex))
   {
      checkIndex++;
      segment->setIndex(checkIndex);
   }

   CAddPixmapSegmentCommand *newCommand = new CAddPixmapSegmentCommand(this, segment);
   m_undoStack->push(newCommand);
}

QString CDrawBoard::getMyName()
{
   return myName;
}

QSize CDrawBoard::getOrigSize()
{
   return origSize;
}

QUndoStack* CDrawBoard::undoStack() const 
{
   return m_undoStack;
}


//  This virtual method gets called whenever the users moves the mouse out of the
//  drawboard. It cleans up anything that should not be displayed in the widget while
//  the cursor is not present in it
void CDrawBoard::leaveEvent(QEvent *)
{
   m_draw->blankAidBuffer();
   update();
}

void CDrawBoard::copyActionTriggered(bool)
{
   QPoint pt = mainzoomer->getBufferPoint(this->mapFromGlobal(QCursor::pos()));
   switch(mOperation)
   {
   case(DRAW):
      break;

   case(SELECTER):
      m_segmenter->copyActionTriggered(pt, true);
      break;

   default:break;
   }
}

void CDrawBoard::cutActionTriggered(bool)
{
   QPoint pt = mainzoomer->getBufferPoint(this->mapFromGlobal(QCursor::pos()));
   switch(mOperation)
   {
   case(DRAW):
      break;

   case(SELECTER):
      m_segmenter->cutActionTriggered(pt, true);
      break;

   default:break;
   }
}

void CDrawBoard::pasteActionTriggered(bool)
{
   QPoint pt = mainzoomer->getBufferPoint(this->mapFromGlobal(QCursor::pos()));
   switch(mOperation)
   {
   case(DRAW):
      break;

   case(SELECTER):
      m_segmenter->pasteActionTriggered(pt, true);
      break;

   default:break;
   }
}

void CDrawBoard::deleteActionTriggered(bool)
{
   QPoint pt = mainzoomer->getBufferPoint(this->mapFromGlobal(QCursor::pos()));
   switch(mOperation)
   {
   case(DRAW):
      break;

   case(SELECTER):
      m_segmenter->deleteActionTriggered(pt, true);
      break;

   default:break;
   }
}

void CDrawBoard::cancelActionTriggered(bool)
{
   QPoint pt = mainzoomer->getBufferPoint(this->mapFromGlobal(QCursor::pos()));
   switch(mOperation)
   {
   case(DRAW):
      break;

   case(SELECTER):
      m_segmenter->cancelActionTriggered(pt, true);
      break;

   default:break;
   }
}

void CDrawBoard::zoomInActionTriggered(bool)
{

}

void CDrawBoard::zoomOutActionTriggered(bool)
{

}

//void CDrawBoard::setPenTexturePath(const QString &path)
//{
//   if (0 != m_draw)
//   {
//      m_draw->setPenTexturePath(path);
//   }
//   if (0 != m_segmenter)
//   {
//      m_segmenter->setPenTexturePath(path);
//   }
//}
//
//void CDrawBoard::setUsePenTexture(bool value)
//{
//   if (0 != m_draw)
//   {
//
//   }
//   if (0 != m_segmenter)
//   {
//
//   }
//}
//
//void CDrawBoard::slotchangePen1(QColor &color)
//{
//   if (0 != m_draw)
//   {
//      m_draw->slotchangePen1(color);
//   }
//   if (0 != m_segmenter)
//   {
//      m_segmenter->slotchangePen1(color);
//   }
//}
//
//void CDrawBoard::slotchangePen2(QColor &color)
//{
//   if (0 != m_draw)
//   {
//      m_draw->slotchangePen2(color);
//   }
//   if (0 != m_segmenter)
//   {
//      m_segmenter->slotchangePen2(color);
//   }
//}
//
//void CDrawBoard::slotChangePenColor(QColor &color)
//{
//   if (0 != m_draw)
//   {
//      m_draw->slotChangePenColor(color);
//   }
//   if (0 != m_segmenter)
//   {
//      m_segmenter->slotChangePenColor(color);
//   }
//}
//
//void CDrawBoard::updatePen(QPen &_pen)
//{
//   if (0 != m_draw)
//   {
//      m_draw->updatePen(_pen);
//   }
//   if (0 != m_segmenter)
//   {
//      m_segmenter->updatePen(_pen);
//   }
//}
//
//void CDrawBoard::updateBrush(QBrush &_brush)
//{
//   if (0 != m_draw)
//   {
//      m_draw->updateBrush(_brush);
//   }
//   if (0 != m_segmenter)
//   {
//      m_segmenter->updateBrush(_brush);
//   }
//}
//
//void CDrawBoard::setBrushTexturePath(const QString &path)
//{
//   if (0 != m_draw)
//   {
//      m_draw->setBrushTexturePath(path);
//   }
//   if (0 != m_segmenter)
//   {
//      m_segmenter->setBrushTexturePath(path);
//   }
//}

//void CDrawBoard::updateShape(shapeType shape)
//{
//   if (0 != m_draw)
//   {
//      m_draw->updateShape(shape);
//   }
//}

void CDrawBoard::updateAngle(qreal val)
{
   if (0 != m_draw)
   {
      m_draw->updateAngle(val);
   }
}

//void CDrawBoard::updateRoundedRect_X(int radius)
//{
//   if (0 != m_draw)
//   {
//      m_draw->updateRoundedRect_X(radius);
//   }
//}
//
//void CDrawBoard::updateRoundedRect_Y(int radius)
//{
//   if (0 != m_draw)
//   {
//      m_draw->updateRoundedRect_Y(radius);
//   }
//}

void CDrawBoard::doEnterInterpSelection()
{
   if (0 != m_draw)
   {
      m_draw->doEnterInterpSelection();
   }
}

void CDrawBoard::setMouseActive(bool onOrOff)
{
   activeMouse = onOrOff;
}

void CDrawBoard::updateSelectionMode(TSelectionMode selection)
{
   if (0 != m_segmenter)
   {
      m_segmenter->updateSelectionMode(selection);
   }
}

void CDrawBoard::performSelectionAction(TSelectionAction action)
{
   if (0 != m_segmenter)
   {
      //m_segmenter->performSelectionAction()
   }
}