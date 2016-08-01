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
//  \file      drawboard_GUI.cpp
//
//  \brief     Implementation of a GUI frame for the 2D_Drawboard
//
//  \author    Stian Broen
//
//  \date      19.03.2012
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 19.03.2012 Original version
//
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QPainter>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QFile>
#include <QDir>
#include <QResizeEvent>
#include <QUndoGroup>
#include <QUndoStack>
#include <QtCore/qmath.h>
#include <QTimer>

// local includes
#include "drawboard_GUI.h"

using namespace sdraw;

CDrawBoardFrame::CDrawBoardFrame(QWidget *parent)
   :QMainWindow(parent),
   drawBoard(0),
   m_undoGroup(0),
   mainMenu(0),
   clearAction(0),
   saveAction(0),
   undoAction(0),
   redoAction(0),
   m_adjustCanvasRequested(false)
{
   layoutGUI();
}

CDrawBoardFrame::~CDrawBoardFrame()
{
   delete drawBoard;
}

void CDrawBoardFrame::adjustCanvasRequested()
{
   m_adjustCanvasRequested = true;
}

void CDrawBoardFrame::paintEvent(QPaintEvent * event)
{
   QRect thisRect = this->rect();
   QLinearGradient linearGrad(thisRect.topLeft(), thisRect.bottomRight());
   linearGrad.setColorAt(0, Qt::white);
   linearGrad.setColorAt(1, Qt::lightGray);
   QBrush backgroundBrush(linearGrad);
   QPainter painter(this);
   painter.fillRect(this->rect(), backgroundBrush);
   painter.end();
   QMainWindow::paintEvent(event);
   if (m_adjustCanvasRequested)
   {
      m_adjustCanvasRequested = false;
      QTimer::singleShot(5, this, SLOT(centralizeDrawboard()));
   }
}

bool CDrawBoardFrame::setDrawBoard(CDrawBoard *draw)
{
   if (0 == draw)
   {
      return false;
   }
   m_name = draw->getMyName();
   drawBoard = draw;
   setCentralWidget(drawBoard);
   setGeometry(50, 50, drawBoard->width() + 75, drawBoard->height() + 104);

   m_undoGroup = new QUndoGroup(this);
   if (0 == drawBoard->undoStack())
   {
      return false;
   }

   m_undoGroup->addStack(drawBoard->undoStack());
   m_undoGroup->setActiveStack(drawBoard->undoStack());
   QAction *undoAction = m_undoGroup->createUndoAction(this);
   QAction *redoAction = m_undoGroup->createRedoAction(this);

   undoAction->setIcon(QIcon(":/images/undo.png"));
   redoAction->setIcon(QIcon(":/images/redo.png"));

   connect(drawBoard->undoStack(), SIGNAL(indexChanged(int)), 
      this, SLOT(updateActions()));

   connect(drawBoard->undoStack(), SIGNAL(cleanChanged(bool)), 
      this, SLOT(updateActions()));

   connect(drawBoard, SIGNAL(adjustCanvas()),
      this, SLOT(adjustCanvasRequested()));

   undoAction->setShortcut(QKeySequence::Undo);
   redoAction->setShortcut(QKeySequence::Redo);

   mainMenu->addSeparator();
   mainMenu->addAction(undoAction);
   mainMenu->addAction(redoAction);
   updateActions();
   return true;
}

void CDrawBoardFrame::updateActions()
{
   if (0 == drawBoard || 0 == drawBoard->undoStack())
   {
      return;
   }
   if (drawBoard->undoStack()->count() == 0)
   {
      saveAction->setEnabled(false);
   }
   else if (!saveAction->isEnabled())
   {
      saveAction->setEnabled(true);
   }
   adjustCanvasRequested();
}

void CDrawBoardFrame::layoutGUI()
{
   mainMenu = new QToolBar(tr("Options"), this);
   saveAction = new QAction(tr("Save"), this);
   clearAction = new QAction(tr("Clear"), this);

   setWindowIcon(QIcon(":/images/palette.png"));
   clearAction->setIcon(QIcon(":/images/filenew.png"));
   saveAction->setIcon(QIcon(":/images/save.png"));

   mainMenu->addAction(clearAction);
   mainMenu->addAction(saveAction);

   addToolBar(Qt::TopToolBarArea, mainMenu);
}

void CDrawBoardFrame::clearActionTriggered()
{

}

void CDrawBoardFrame::saveActionTriggered()
{

}

void CDrawBoardFrame::resizeEvent(QResizeEvent *event)
{
   QMainWindow::resizeEvent(event);
   centralizeDrawboard();
}

void CDrawBoardFrame::centralizeDrawboard()
{
   if (0 != drawBoard)
   {
      QSize boardSize = drawBoard->getOrigSize();
      QSize thisSize = frameGeometry().size();
      int wDiff = thisSize.width() - boardSize.width();
      int hDiff = thisSize.height() - boardSize.height();
      int dx = 0;
      int dy = 0;
      if (wDiff > 0)
      {
         dx = qFloor((qreal)wDiff / 2.0);
      }
      if (hDiff > 0)
      {
         dy = qFloor((qreal)hDiff / 2.0);
      }
      if (dx > 0 || dy > 0)
      {
         drawBoard->move(dx, dy + 20);
         update();
      }
   }
}

QString CDrawBoardFrame::getName()
{
   return m_name;
}