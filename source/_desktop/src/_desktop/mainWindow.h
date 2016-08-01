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
//  \file      mainWindow.h
//
//  \brief     Declaration of the Broen Draw main window
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
//             02 28.03.2012 - Reorganizing the code
//
//  \par       01 16.06.2011 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

// Qt includes
#include <QMainWindow>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QResizeEvent>

// solution includes
#include <common/definitions.h>
#include <drawboard/drawboard.h>
#include <drawboard/drawboard_newDialog.h>
#include <drawboard/drawboard_GUI.h>
#include <drawSettings/DrawSettings_2D.h>
#include <sdrawWidgets/shapeSelector.h>
#include <sdrawWidgets/settingsMenu.h>
#include <sdrawWidgets/colorWidget.h>
#include <sdrawWidgets/colorMenu.h>
#include <sdrawWidgets/angleSelector.h>
#include <sdrawWidgets/penSelector.h>
#include <sdrawWidgets/brushSelector.h>
#include <sdrawWidgets/toolTypeSelector.h>
#include <sdrawWidgets/selectionSelector.h>

// qt forward declarations
class QMdiArea;
class QAction;
class QContextMenuEvent;
class QActionGroup;
class QToolBar;
class QStatusBar;
class QSpinBox;
class QComboBox;


namespace sdraw {
   class CMainWindow : public QMainWindow
   {
      Q_OBJECT

   public:
      explicit CMainWindow(QWidget *parent = NULL);
      ~CMainWindow();
      void initFinal();

   private:
      void contextMenuEvent(QContextMenuEvent * event);
      void moveEvent(QMoveEvent *event);
      void toLog(QString &txt);
      void setupFileMenu();
      void openMenu(QToolBar *fromToolBar, QAction *fromAction, QMenu *menuToOpen);
      void connectDrawBoard(CDrawBoard *drawBoard);
      void initBasicToolbar();
      void initSelectionMenu();
      void initToolTypeMenu();
      void initPenMenu();
      void initBrushMenu();
      void initShapeMenu();
      void initSettingsMenu();
      void initToolBars();
      void initMainFrame(QStringList argList = QStringList(), QWidget *parent = NULL);

   private slots:
      void receiveShapeSettings(const CShapeSettings *shapeSettings);
      void receiveNewImage(const QString &name, const int width, const int height);
      void slotLoad();
      void slotSave();
      void slotNew();
      void slotNewDrawBoard(const QString &name = QString("Untitled"),
         const int width = 500,
         const int height = 500);
      void printInfo(const QString &txt);
      void catchMouseInfo(const QPoint &pt);
      void resizeSubMDI(const QSize &size);
      void chooseType(QAction* action);
      void getAngle(qreal _angle);
      void penActionTriggeredSlot(bool);
      void brushActionTriggeredSlot(bool);
      void shapeActionTriggeredSlot(bool);
      void settingsActionTriggeredSlot(bool);
      void selectionMenuActionTriggeredSlot(bool);
      void receiveSettings(CSettings &settings);
      void receiveOperation(TOperation operation);
      //void receiveSelectionAction(TSelectionAction selectionAction);
      //void receiveSelectionOption(TSelectionOption selectionOption);
      void closePenMenu();
      void closeShapeMenu();
      void closeBrushMenu();
      void painterActionTriggered();
      void fillerActionTriggered();
      void selecterActionTriggered();

   signals:
      void operationSig(TOperation &operation);
      void updateAngleSig(qreal val);
      void doUseTexturePath(bool value);
      void typeChange(int type);
      void enterInterpPointsSelectionSig();
      void requestSettings();
      void settingsFromToolbarSig(CSettings &settings);
      void requestOperation();
      void operationFromToolbarSig(TOperation operation);
      void requestSelectionMode();
      void selectionActionFromToolbarSig(TSelectionAction selectionAction);
      void selectionOptionFromToolbarSig(TSelectionOption selectionOption);
      void selectionModeFromToolbarSig(TSelectionMode   selection);
      void selectionCompModeFromToolbarSig(QPainter::CompositionMode mode);
      void selectionCompAlphaFromToolbarSig(int value);
      void selectionTransformationFromToolbarSig(TTransformationMode mode);
      void selectionRotationAxisFromToolbarSig(Qt::Axis axis, bool enable);
      void selectionTransformationSquareFromToolbarSig(bool enable);

   private:
      CNewDrawboardDialog *newDialog;
      QMdiArea *m_centralWidget;
      QHash <QString, CDrawBoard*> drawBoards;
      QString lastError;
      QString currSelectedColorButton;
      QString angleLabel;
      QWidget *display;
      QMenu *penMenu;
      QMenu *brushMenu;
      QMenu *shapeMenu;
      QMenu *settingsMenu;
      QMenu *selectionMenu;
      QToolBar *colorBar;
      QToolBar *selectionToolBar;
      QToolBar *drawDefBar;
      QToolBar *basicOperationsBar;
      QActionGroup *colorActions;
      QActionGroup *drawTypeActions;
      QActionGroup *drawDefActions;
      QActionGroup *selectionActions;
      QAction *newDrawboardAction;
      QAction *saveDrawboardAction;
      QAction *openDrawboardAction;
      QAction *load;
      QAction *save;
      QAction *printPreview;
      QAction *print;
      QAction *quit;
      QAction *painterAction;
      QAction *fillerAction;
      QAction *selecterAction;
      QAction *penMenuAction;
      QAction *brushMenuAction;
      QAction *shapeMenuAction;
      QAction *settingsMenuAction;
      QAction *selectionMenuAction;
      QAction *angle;
      CColorMenu *colorMenu;
      CSelectionSelector *selectionSpecifier;
      CToolTypeSelector *typeSelector;
   };
}

#endif