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
//  \file      drawboard_GUI.h
//
//  \brief     Declaration of a GUI frame for the 2D_Drawboard
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
//h-//////////////////////////////////////////////////////////////////////////

#ifndef TWOD_DRAWBOARD_CONTAINER
#define TWOD_DRAWBOARD_CONTAINER

// Qt includes
#include <QMainWindow>

// local include
#include "drawboard.h"

// Qt forward declarations
class QAction;
class QMenu;
class QToolBar;
class QUndoGroup;

namespace sdraw {
   class CDrawBoardFrame : public QMainWindow
   {
      Q_OBJECT

   public:
      explicit CDrawBoardFrame(QWidget *parent = 0);
      ~CDrawBoardFrame();
      bool setDrawBoard(CDrawBoard *draw);
      QString getName();

   public slots:
      void centralizeDrawboard();

   protected:
      void paintEvent(QPaintEvent *event);
      void resizeEvent(QResizeEvent *event);

   private slots:
      void updateActions();
      void clearActionTriggered();
      void saveActionTriggered();
      void adjustCanvasRequested();

   private:
      void layoutGUI();

   private:
      QString m_name;
      CDrawBoard *drawBoard;
      QUndoGroup *m_undoGroup;
      QToolBar *mainMenu;
      QAction *clearAction;
      QAction *saveAction;
      QAction *undoAction;
      QAction *redoAction;
      bool m_adjustCanvasRequested;
   };
}

#endif