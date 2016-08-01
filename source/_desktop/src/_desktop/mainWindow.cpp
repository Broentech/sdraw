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
//  \file      mainWindow.cpp
//
//  \brief     Implementation of the outer frame class
//
//  \author    Stian Broen
//
//  \date      16.06.2011
//
//  \par       Copyright:
//
//             Copyright by Stian Broen
//
//  \par       Revision History
//
//  \par       01 16.06.2011 Original version       
//
//
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QStatusBar>
#include <QAction>
#include <QFile>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QContextMenuEvent>
#include <QActionGroup>
#include <QColorDialog>
#include <QPixmap>
#include <QStatusBar>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QToolButton>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QWidgetAction>
#include <QMoveEvent>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMessageBox>

// local includes
#include "mainWindow.h"

using namespace sdraw;

CMainWindow::CMainWindow(QWidget *parent)
   :QMainWindow(parent)
{
   newDialog = 0;
   setContextMenuPolicy(Qt::PreventContextMenu);
   //m_drawSettings = new CDrawSettings_2D(this);
   initMainFrame();
   initToolBars();
   initFinal();
   toLog(QString("Ready"));
}

CMainWindow::~CMainWindow()
{
   QHash<QString, CDrawBoard *> drawBoards;
   QHashIterator<QString, CDrawBoard *> it(drawBoards);
   while (it.hasNext())
   {
      it.next();
      CDrawBoard *curr = it.value();
      delete curr;
      curr = NULL;
   }
   drawBoards.clear();

   delete display;
   delete m_centralWidget;
   delete penMenu;
   delete brushMenu;
   delete shapeMenu;
   delete settingsMenu;
   delete selectionMenu;
   delete colorBar;
   delete selectionToolBar;
   delete drawDefBar;
   delete basicOperationsBar;
   delete colorActions;
   delete drawTypeActions;
   delete drawDefActions;
   delete selectionActions;
   delete newDrawboardAction;
   delete saveDrawboardAction;
   delete openDrawboardAction;
   delete load;
   delete save;
   delete printPreview;
   delete print;
   delete quit;
   delete painterAction;
   delete fillerAction;
   delete selecterAction;
   delete penMenuAction;
   delete brushMenuAction;
   delete shapeMenuAction;
   delete settingsMenuAction;
   delete selectionMenuAction;
   delete angle;
   delete colorMenu;
   delete selectionSpecifier;
   delete typeSelector;
}

void CMainWindow::initMainFrame(QStringList argList, QWidget *parent)
{
   setupFileMenu();
}

void CMainWindow::toLog(QString &txt)
{
   QString write2log = QString("%1\n").arg(txt);
   statusBar()->showMessage(write2log);
}

void CMainWindow::contextMenuEvent(QContextMenuEvent * event)
{
   switch (event->reason())
   {
   case(QContextMenuEvent::Mouse) :
   {
      QMainWindow::contextMenuEvent(event);
      break;
   }
   case(QContextMenuEvent::Keyboard) :
   {
      QMainWindow::contextMenuEvent(event);
      break;
   }
   case(QContextMenuEvent::Other) :
   {
      QMainWindow::contextMenuEvent(event);
      break;
   }
   default:
   {
      QMainWindow::contextMenuEvent(event);
      break;
   }
   }
}

void CMainWindow::setupFileMenu()
{
   // init members
   load = 0;
   save = 0;
   printPreview = 0;
   print = 0;
   quit = 0;

   load = new QAction(tr("&Load"), this);
   save = new QAction(tr("&Save"), this);
   print = new QAction(tr("&Print"), this);
   printPreview = new QAction(tr("Print Pre&view"), this);
   quit = new QAction(tr("&Quit"), this);

   load->setIcon(QIcon(":/images/load.png"));
   save->setIcon(QIcon(":/images/save.png"));
   print->setIcon(QIcon(":/images/print.png"));
   printPreview->setIcon(QIcon(":/images/print.png"));
   quit->setIcon(QIcon(":/images/quit.png"));

   // set shortcuts
   load->setShortcut(QKeySequence::Open  /*Qt::CTRL + Qt::Key_L*/);
   save->setShortcut(QKeySequence::Save  /*Qt::CTRL + Qt::Key_S*/);
   print->setShortcut(QKeySequence::Print /*Qt::CTRL + Qt::Key_P*/);
   quit->setShortcut(QKeySequence::Quit  /*Qt::CTRL + Qt::Key_Q*/);

   // set tool tip
   load->setToolTip(tr("Load an image"));
   save->setToolTip(tr("Save image"));
   print->setToolTip(tr("Print image"));
   quit->setToolTip(tr("Quit the application unconditionally"));

   // menu bar
   QMenu* file = new QMenu(tr("&File"), menuBar());
   menuBar()->addMenu(file);
   file->addAction(load);
   file->addAction(save);
   file->addAction(printPreview);
   file->addAction(print);
   file->addAction(quit);
}

void CMainWindow::initToolBars()
{
   // init pointers
   angle = 0;

   drawDefActions = new QActionGroup(this);
   drawDefBar = new QToolBar(tr("Define Painter"), this);
   drawDefBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
   drawDefBar->setIconSize(QSize(40, 40));

   // setup toolbars
   initBasicToolbar();
   initToolTypeMenu();   //< initialize the lowest level menu, where you choose what you want to do
   initPenMenu();        //< initialize the pen penu, where you define the pen used for drawing
   initBrushMenu();      //< initialize the brush menu, where you define the brush, gradients, etc.
   initShapeMenu();      //< initialize the shape menu, where you define what shapes you want to draw
   initSettingsMenu();   //< initialize the settings menu, where you define misc settings
   initSelectionMenu();  //< initialize the selection menu, where you define selection options
   //initColors();

   addToolBar(Qt::LeftToolBarArea, drawDefBar);

   receiveOperation(TOperation::DRAW);
}

void CMainWindow::initBasicToolbar()
{
   basicOperationsBar = new QToolBar(this);

   // ->setIcon(QIcon(":/images/quit.png" ));
   newDrawboardAction = new QAction(tr("New"), this);
   saveDrawboardAction = new QAction(tr("Save"), this);
   openDrawboardAction = new QAction(tr("Open"), this);

   newDrawboardAction->setIcon(QIcon(":/images/new.png"));
   saveDrawboardAction->setIcon(QIcon(":/images/save.png"));
   openDrawboardAction->setIcon(QIcon(":/images/open.png"));

   basicOperationsBar->addAction(newDrawboardAction);
   basicOperationsBar->addAction(saveDrawboardAction);
   basicOperationsBar->addAction(openDrawboardAction);

   addToolBar(Qt::TopToolBarArea, basicOperationsBar);
}

void CMainWindow::initToolTypeMenu()
{
   painterAction = new QAction(this);
   fillerAction = new QAction(this);
   selecterAction = new QAction(this);

   connect(painterAction, SIGNAL(triggered()),
      this, SLOT(painterActionTriggered()));

   connect(fillerAction, SIGNAL(triggered()),
      this, SLOT(fillerActionTriggered()));

   connect(selecterAction, SIGNAL(triggered()),
      this, SLOT(selecterActionTriggered()));

   painterAction->setCheckable(true);
   painterAction->setChecked(true);
   fillerAction->setCheckable(true);
   selecterAction->setCheckable(true);

   painterAction->setToolTip(tr("Draw"));
   fillerAction->setToolTip(tr("Fill"));
   selecterAction->setToolTip(tr("Segmentation"));

   painterAction->setIcon(QIcon(":/images/penIcon.png"));
   fillerAction->setIcon(QIcon(":/images/fill.png"));
   selecterAction->setIcon(QIcon(":/images/selectionIcon.png"));

   drawDefBar->addAction(painterAction);
   drawDefBar->addAction(fillerAction);
   drawDefBar->addAction(selecterAction);
   drawDefBar->addSeparator();
}

void CMainWindow::painterActionTriggered()
{
   fillerAction->setChecked(false);
   selecterAction->setChecked(false);
   receiveOperation(TOperation::DRAW);
}

void CMainWindow::fillerActionTriggered()
{
   painterAction->setChecked(false);
   selecterAction->setChecked(false);
   receiveOperation(TOperation::FILLER);
}

void CMainWindow::selecterActionTriggered()
{
   painterAction->setChecked(false);
   fillerAction->setChecked(false);
   receiveOperation(TOperation::SELECTER);
}

void CMainWindow::initPenMenu()
{
   // allocate menu members
   penMenuAction = new QAction(drawDefActions);
   penMenu = new QMenu(this);
   QWidgetAction *penSelectorWidgetAction = new QWidgetAction(this);
   CPenSelector *penSelector = new CPenSelector(penMenu);

   // misc
   penMenuAction->setToolTip(tr("Pen"));
   penMenuAction->setShortcut(tr("Ctrl+E"));
   penMenuAction->setIcon(QIcon(":/images/pen2.png"));
   penMenuAction->setMenu(penMenu);
   drawDefBar->addAction(penMenuAction);
   penSelectorWidgetAction->setDefaultWidget(penSelector);
   penMenu->addAction(penSelectorWidgetAction);

   // make connections to main window
   //connect(penSelector  , SIGNAL(penUpdateSig(QPen &)), 
   //        this         , SLOT(receivePen(QPen &)));
   connect(penSelector, SIGNAL(sendPenSettings(const CPenSettings *)),
      this, SLOT(receivePenSettings(const CPenSettings *)));

   connect(penSelector, SIGNAL(closeSignal()),
      this, SLOT(closePenMenu()));

   connect(penMenuAction, SIGNAL(triggered(bool)),
      this, SLOT(penActionTriggeredSlot(bool)));

   // request default settings
   //emit updatePenSig(penSelector->getPen());
}

void CMainWindow::initBrushMenu()
{
   // allocate menu members
   brushMenuAction = new QAction(drawDefActions);
   brushMenu = new QMenu(this);
   QWidgetAction *brushSelectorWidgetAction = new QWidgetAction(this);
   CBrushSelector *brushSelector = new CBrushSelector(brushMenu);

   // misc
   brushSelectorWidgetAction->setDefaultWidget(brushSelector);
   brushMenu->addAction(brushSelectorWidgetAction);
   brushMenuAction->setToolTip(tr("Brush"));
   brushMenuAction->setIcon(QIcon(":/images/brush.png"));
   brushMenuAction->setMenu(brushMenu);
   drawDefBar->addAction(brushMenuAction);

   // make connections to main window
   connect(brushMenuAction, SIGNAL(triggered(bool)),
      this, SLOT(brushActionTriggeredSlot(bool)));

   connect(brushSelector, SIGNAL(closeSignal()),
      this, SLOT(closeBrushMenu()));

   //connect(brushSelector, SIGNAL(sendBrushSettings(const CBrushSettings *)),
   //   this, SLOT(receiveBrushSettings(const CBrushSettings *)));
}

void CMainWindow::initShapeMenu()
{
   shapeMenuAction = new QAction(drawDefActions);
   shapeMenuAction->setToolTip(tr("Shapes"));
   connect(shapeMenuAction, SIGNAL(triggered(bool)), this, SLOT(shapeActionTriggeredSlot(bool)));
   shapeMenu = new QMenu(this);
   shapeMenuAction->setIcon(QIcon(":/images/shapes.png"));
   QWidgetAction *shapeSelectorWidgetAction = new QWidgetAction(this);
   CShapeSelector *shapeSelector = new CShapeSelector(shapeMenu);
   shapeSelectorWidgetAction->setDefaultWidget(shapeSelector);
   shapeMenu->addAction(shapeSelectorWidgetAction);

   connect(shapeSelector, SIGNAL(shapeChangeSig(shapeType)),
      this, SLOT(receiveShape(shapeType)));

   connect(shapeSelector, SIGNAL(xRadChangeSig(int)),
      this, SLOT(receiveRadiusForRoundedRec_X(int)));

   connect(shapeSelector, SIGNAL(yRadChangeSig(int)),
      this, SLOT(receiveRadiusForRoundedRec_Y(int)));

   connect(shapeSelector, SIGNAL(doCloseShapeMenuSig()),
      this, SLOT(closeShapeMenu()));

   shapeMenuAction->setMenu(shapeMenu);
   drawDefBar->addAction(shapeMenuAction);
}

void CMainWindow::initSettingsMenu()
{
   settingsMenuAction = new QAction(drawDefActions);
   settingsMenuAction->setToolTip(tr("Settings"));

   connect(settingsMenuAction, SIGNAL(triggered(bool)),
      this, SLOT(settingsActionTriggeredSlot(bool)));

   settingsMenu = new QMenu(this);
   settingsMenuAction->setIcon(QIcon(":/images/tool_kit.png"));
   QWidgetAction *settingsWidgetAction = new QWidgetAction(this);
   CSettingsMenu *settings = new CSettingsMenu(settingsMenu);
   settingsWidgetAction->setDefaultWidget(settings);
   settingsMenu->addAction(settingsWidgetAction);

   connect(settings, SIGNAL(settingsSig(CSettings &)),
      this, SLOT(receiveSettings(CSettings &)));

   connect(this, SIGNAL(requestSettings()),
      settings, SLOT(settingsRequest()));

   settingsMenuAction->setMenu(settingsMenu);
   drawDefBar->addAction(settingsMenuAction);
}

void CMainWindow::initSelectionMenu()
{

   selectionMenuAction = new QAction(drawDefActions);
   selectionMenuAction->setToolTip(tr("Segmentation"));

   connect(selectionMenuAction, SIGNAL(triggered(bool)),
      this, SLOT(selectionMenuActionTriggeredSlot(bool)));

   selectionMenu = new QMenu(this);
   selectionMenuAction->setIcon(QIcon(":/images/shapes.png"));
   QWidgetAction *selectionWidgetAction = new QWidgetAction(this);
   selectionSpecifier = new CSelectionSelector(this);
   selectionWidgetAction->setDefaultWidget(selectionSpecifier);
   selectionMenu->addAction(selectionWidgetAction);
   selectionMenuAction->setMenu(selectionMenu);
   drawDefBar->addAction(selectionMenuAction);

   // create toolbar and its widgets
   //selectionActions = new QActionGroup(this);
   //selectionToolBar   = new QToolBar("Define Selection", this);
   //selectionToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
   //selectionToolBar->setIconSize(QSize(40,40));
   //selectionToolBar->setAllowedAreas(Qt::NoToolBarArea);
   //selectionSpecifier = new CSelectionSelector(this);
   //selectionToolBar->addWidget(selectionSpecifier);
   //addToolBar(selectionToolBar);

   // connections
   connect(this, SIGNAL(requestSelectionMode()),
      selectionSpecifier, SLOT(resondToRequestAll()));


   // NOT IMPLEMENTED :
   //connect(selectionSpecifier, SIGNAL(updateSelectionActionFromWidgetSig(TSelectionAction )), 
   //        this, SIGNAL(selectionActionFromToolbarSig(TSelectionAction)));

   connect(selectionSpecifier, SIGNAL(updateSelectionModeFromWidgetSig(TSelectionMode)),
      this, SIGNAL(selectionModeFromToolbarSig(TSelectionMode)));

   connect(selectionSpecifier, SIGNAL(updateSelectionOptionFromWidgetSig(TSelectionOption)),
      this, SIGNAL(selectionOptionFromToolbarSig(TSelectionOption)));

   connect(selectionSpecifier, SIGNAL(updateCompositionMode(QPainter::CompositionMode)),
      this, SIGNAL(selectionCompModeFromToolbarSig(QPainter::CompositionMode)));

   connect(selectionSpecifier, SIGNAL(updateCompositionAlpha(int)),
      this, SIGNAL(selectionCompAlphaFromToolbarSig(int)));

   connect(selectionSpecifier, SIGNAL(updateTransformationMode(TTransformationMode)),
      this, SIGNAL(selectionTransformationFromToolbarSig(TTransformationMode)));

   connect(selectionSpecifier, SIGNAL(updateRotationAxis(Qt::Axis, bool)),
      this, SIGNAL(selectionRotationAxisFromToolbarSig(Qt::Axis, bool)));

   connect(selectionSpecifier, SIGNAL(updateTransformationSquare(bool)),
      this, SIGNAL(selectionTransformationSquareFromToolbarSig(bool)));

}

//void CMainWindow::initColors()
//{
//   // set up color actions
//   colorActions = new QActionGroup(this);
//   colorActions->setExclusive(true);
//   colorBar = new QToolBar("Colors", this);
//   colorBar->setAllowedAreas(Qt::NoToolBarArea);
//   colorMenu = new CColorMenu(this);
//
//   connect(colorMenu, SIGNAL(setColor1Sig(QColor &)),
//      this, SLOT(getColor1FromColorMenuSlot(QColor &)));
//
//   connect(colorMenu, SIGNAL(setColor2Sig(QColor &)),
//      this, SLOT(getColor2FromColorMenuSlot(QColor &)));
//
//   connect(colorMenu, SIGNAL(setPenColorSig(QColor &)),
//      this, SLOT(getPenColorFromColorMenuSlot(QColor &)));
//
//   connect(this, SIGNAL(requestColors()),
//      colorMenu, SLOT(respondToColorRequest()));
//
//   colorBar->addWidget(colorMenu);
//   addToolBar(Qt::BottomToolBarArea, colorBar);
//}

void CMainWindow::receiveOperation(TOperation operation)
{
   emit operationFromToolbarSig(operation);
   switch (operation)
   {
   case(DRAW) :
   {
      penMenuAction->setVisible(true);
      brushMenuAction->setVisible(false);
      shapeMenuAction->setVisible(true);
      settingsMenuAction->setVisible(true);
      //selectionToolBar  ->setVisible(false);
      selectionMenuAction->setVisible(false);
      break;
   }
   case(POLYGONIZER) :
   {
      penMenuAction->setVisible(false);
      brushMenuAction->setVisible(false);
      shapeMenuAction->setVisible(false);
      settingsMenuAction->setVisible(false);
      //selectionToolBar  ->setVisible(false);
      selectionMenuAction->setVisible(false);
      break;
   }
   case(FILLER) :
   {
      penMenuAction->setVisible(true);
      brushMenuAction->setVisible(true);
      shapeMenuAction->setVisible(false);
      settingsMenuAction->setVisible(true);
      //selectionToolBar  ->setVisible(false);
      selectionMenuAction->setVisible(false);
      break;
   }
   case(SELECTER) :
   {
      emit requestSelectionMode();
      //drawDefActions  ->setVisible(false);
      //selectionToolBar->setVisible(true);

      penMenuAction->setVisible(false);
      brushMenuAction->setVisible(false);
      shapeMenuAction->setVisible(false);
      settingsMenuAction->setVisible(false);
      //selectionToolBar  ->setVisible(false);
      selectionMenuAction->setVisible(true);

      selectionSpecifier->setParentRect(this->geometry());
      break;
   }
   case(TEXT) :
   {
      //drawDefActions  ->setVisible(false);
      //selectionToolBar->setVisible(false);

      penMenuAction->setVisible(false);
      brushMenuAction->setVisible(false);
      shapeMenuAction->setVisible(false);
      settingsMenuAction->setVisible(false);
      //selectionToolBar  ->setVisible(false);
      selectionMenuAction->setVisible(false);

      break;
   }
   case(INSPECTER) :
   {
      //drawDefActions  ->setVisible(false);
      //selectionToolBar->setVisible(false);

      penMenuAction->setVisible(false);
      brushMenuAction->setVisible(false);
      shapeMenuAction->setVisible(false);
      settingsMenuAction->setVisible(false);
      //selectionToolBar  ->setVisible(false);
      selectionMenuAction->setVisible(true);

      break;
   }
   default:
   {
      break;
   }
   }
}

void CMainWindow::moveEvent(QMoveEvent *event)
{
   QRect oldGeo = this->geometry();
   QPoint oldPos = event->oldPos();
   QPoint newPos = event->pos();
   int dx = newPos.x() - oldPos.x();
   int dy = newPos.y() - oldPos.y();
   QPoint ul = QPoint(oldGeo.topLeft().x() + dx, oldGeo.topLeft().y() + dy);
   QPoint br = QPoint(oldGeo.bottomRight().x() + dx, oldGeo.bottomRight().y() + dy);
   QRect newRect(ul, br);
   selectionSpecifier->setParentRect(this->geometry());
}

void CMainWindow::penActionTriggeredSlot(bool)
{
   openMenu(drawDefBar, penMenuAction, penMenu);
}

void CMainWindow::brushActionTriggeredSlot(bool)
{
   openMenu(drawDefBar, brushMenuAction, brushMenu);
}

void CMainWindow::shapeActionTriggeredSlot(bool)
{
   openMenu(drawDefBar, shapeMenuAction, shapeMenu);
}

void CMainWindow::settingsActionTriggeredSlot(bool)
{
   openMenu(drawDefBar, settingsMenuAction, settingsMenu);
}

void CMainWindow::selectionMenuActionTriggeredSlot(bool)
{
   openMenu(drawDefBar, selectionMenuAction, selectionMenu);
}

//  Opens the argument menu using the argument toolbar and qaction. Note that the
//  menu needs to be the child object of the action, and the action needs to  be
//  child object of the toolbar to get the correct geometry
void CMainWindow::openMenu(QToolBar *fromToolBar, QAction *fromAction, QMenu *menuToOpen)
{
   if (
      0 == fromToolBar ||
      0 == fromAction ||
      0 == menuToOpen
      )
   {
      return;
   }

   int extraXOffset = 0;
   int extraYOffset = 0;
   if (fromToolBar->orientation() == Qt::Vertical)
   {
      extraXOffset = 62;
      extraYOffset = -46;
   }

   QRect appGeo = this->geometry();
   QRect toolBarGeo;
   QRect widgGeo;
   QPoint menuPos;
   QList<QWidget *> tmpList = fromAction->associatedWidgets();
   for (int i = 0; i < tmpList.size(); i++)
   {
      QWidget *tmpWidget = tmpList.at(i);
      if (tmpWidget == fromToolBar)
      {
         toolBarGeo = tmpWidget->geometry();
      }
      else
      {
         widgGeo = tmpWidget->geometry();
         menuPos.setX(appGeo.x() + toolBarGeo.x() + widgGeo.x() + extraXOffset);
         menuPos.setY(appGeo.y() + toolBarGeo.y() + widgGeo.y() + widgGeo.height() + extraYOffset);
      }
   }
   menuToOpen->exec(menuPos);
}

void CMainWindow::chooseType(QAction* action)
{
   int newType = action->data().value<int>();
   emit typeChange(newType);
}

//  Slot to open a file selection dialog to let the user choose a file for
//  brush texture
//void CMainWindow::chooseBrushTexture()
//{
//   QString filePath = QFileDialog::getOpenFileName(this,
//      tr("Open Image"), "C:/", tr("Image Files (*.png *.jpg *.bmp)"));
//
//   if (QFile::QFile(filePath).exists())
//   {
//      emit brushTexturePath(filePath);
//   }
//   else
//   {
//      emit toLog(QString("No file was selected"));
//   }
//}

//  Slot receiving updated angle values from the angleWidget. Updates the angle
//  menu action name with the new value and sends the updated angle value to the
//  drawboard
void CMainWindow::getAngle(qreal _angle)
{
   emit updateAngleSig(_angle);
}

void CMainWindow::receiveSettings(CSettings &settings)
{
   emit settingsFromToolbarSig(settings);
}

void CMainWindow::closePenMenu()
{
   penMenu->close();
}

void CMainWindow::closeShapeMenu()
{
   shapeMenu->close();
}

void CMainWindow::closeBrushMenu()
{
   brushMenu->close();
}

void CMainWindow::initFinal()
{
   if (newDrawboardAction)  connect(newDrawboardAction, SIGNAL(triggered()), this, SLOT(slotNew()));
   if (saveDrawboardAction) connect(saveDrawboardAction, SIGNAL(triggered()), this, SLOT(slotSave()));
   if (openDrawboardAction) connect(openDrawboardAction, SIGNAL(triggered()), this, SLOT(slotLoad()));

   if (load) connect(load, SIGNAL(triggered()), this, SLOT(slotLoad()));
   if (save) connect(save, SIGNAL(triggered()), this, SLOT(slotSave()));
   //if(quit) connect ( quit , SIGNAL( triggered() ), qApp , SLOT( quit    () ) );

   m_centralWidget = new QMdiArea(this);
   m_centralWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
   m_centralWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
   m_centralWidget->tileSubWindows();
   m_centralWidget->setActivationOrder(QMdiArea::CreationOrder);

   // set background

   QLinearGradient linearGrad(QPointF(0.0, 0.0), QPointF(1600, 1200));
   linearGrad.setColorAt(0, Qt::lightGray);
   linearGrad.setColorAt(1, Qt::white);
   QBrush backgroundBrush(linearGrad);
   m_centralWidget->setBackground(backgroundBrush);

   //QString bgPath  = QString("%1/resources/backgrounds/clouds2.jpg" ).arg(installDir);
   //if(QFile::QFile(bgPath).exists())
   //{
   //   QImage bgImg;
   //   bgImg.load(bgPath);
   //   m_centralWidget->setBackground(QBrush(bgImg));
   //}

   setCentralWidget(m_centralWidget);

   emit requestSettings();
   //emit requestColors();
   emit requestOperation();

   //slotNewDrawBoard("1");
   //slotNewDrawBoard("2");
   //slotNewDrawBoard("3");
   //slotNewDrawBoard("4");
}

void CMainWindow::slotNewDrawBoard(
   const QString &name,
   const int width,
   const int height
   )
{
   CDrawBoardFrame *testWidget = new CDrawBoardFrame;
   CDrawBoard *drawBoard = new CDrawBoard(testWidget, name, QSize(width, height));
   drawBoard->initBuffers();
   drawBoard->setFocusPolicy(Qt::WheelFocus);
   drawBoard->setAttribute(Qt::WA_Hover);
   testWidget->setDrawBoard(drawBoard);

   testWidget->setWindowTitle(name);
   QSize m_minSize = QSize(width, height + 40);
   QList<QMdiSubWindow *> windowList = m_centralWidget->subWindowList();

   m_centralWidget->addSubWindow(testWidget);
   resizeSubMDI(m_minSize);

   // debugging
   //drawBoard->slotLoad("C:/Users/stianb/Pictures/lolz/_rect.bmp");
   //emit requestOperation();

   connectDrawBoard(drawBoard);

   emit requestSettings();
   //emit requestColors();

   testWidget->show();
   drawBoards.insert(name, drawBoard);
   receiveOperation(TOperation::DRAW);
}

void CMainWindow::connectDrawBoard(CDrawBoard *drawBoard)
{
   if (NULL == drawBoard)
   {
      return;
   }

   if (printPreview)
   {
      connect(printPreview, SIGNAL(triggered()),
         drawBoard, SLOT(slotPrintPreview()));
   }
   if (print)
   {
      connect(print, SIGNAL(triggered()),
         drawBoard, SLOT(slotPrint()));
   }

   connect(drawBoard, SIGNAL(mouseCoordSig(const QPoint &)),
      this, SLOT(catchMouseInfo(const QPoint &)));

   //connect(this, SIGNAL(colorChange(int, const QColor &)),
   //   drawBoard, SLOT(slotchangePen(int, const QColor &)));

   //connect(this, SIGNAL(penTexturePath(const QString &)),
   //   drawBoard, SLOT(setPenTexturePath(const QString &)));

   //connect(this, SIGNAL(doUseTexturePath(bool)),
   //   drawBoard, SLOT(setUsePenTexture(bool)));

   //connect(this, SIGNAL(updatePenSig(QPen &)),
   //   drawBoard, SLOT(updatePen(QPen &)));

   //connect(this, SIGNAL(updateBrushSig(QBrush &)),
   //   drawBoard, SLOT(updateBrush(QBrush &)));

   //connect(this, SIGNAL(udpateShapeSig(shapeType)),
   //   drawBoard, SLOT(updateShape(shapeType)));

   connect(this, SIGNAL(updateAngleSig(qreal)),
      drawBoard, SLOT(updateAngle(qreal)));

   //connect(this, SIGNAL(updateRadRoundedRecSig_X(int)),
   //   drawBoard, SLOT(updateRoundedRect_X(int)));

   //connect(this, SIGNAL(updateRadRoundedRecSig_Y(int)),
   //   drawBoard, SLOT(updateRoundedRect_Y(int)));

   //connect(this, SIGNAL(brushTexturePath(const QString &)),
   //   drawBoard, SLOT(setBrushTexturePath(const QString &)));

   connect(drawBoard, SIGNAL(info(const QString &)),
      this, SLOT(printInfo(const QString &)));

   //connect(this, SIGNAL(color1FromToolbarSig(QColor &)),
   //   drawBoard, SLOT(slotchangePen1(QColor &)));

   //connect(this, SIGNAL(color2FromToolbarSig(QColor &)),
   //   drawBoard, SLOT(slotchangePen2(QColor &)));

   //connect(this, SIGNAL(penColorFromToolbarSig(QColor &)),
   //   drawBoard, SLOT(slotChangePenColor(QColor &)));

   connect(this, SIGNAL(settingsFromToolbarSig(CSettings &)),
      drawBoard, SLOT(updateSettings(CSettings  &)));

   connect(this, SIGNAL(operationFromToolbarSig(TOperation)),
      drawBoard, SLOT(updateOperation(TOperation)));

   connect(this, SIGNAL(selectionModeFromToolbarSig(TSelectionMode)),
      drawBoard, SLOT(updateSelectionMode(TSelectionMode)));

   connect(this, SIGNAL(selectionActionFromToolbarSig(TSelectionAction)),
      drawBoard, SLOT(performSelectionAction(TSelectionAction)));

   connect(this, SIGNAL(enterInterpPointsSelectionSig()),
      drawBoard, SLOT(doEnterInterpSelection()));

   connect(drawBoard, SIGNAL(newSizeSig(const QSize &)),
      this, SLOT(resizeSubMDI(const QSize &)));

   connect(this, SIGNAL(selectionModeFromToolbarSig(TSelectionMode)),
      drawBoard, SIGNAL(selectionModeFromToolbarSig(TSelectionMode)));

   connect(this, SIGNAL(selectionOptionFromToolbarSig(TSelectionOption)),
      drawBoard, SIGNAL(selectionOptionFromToolbarSig(TSelectionOption)));

   connect(this, SIGNAL(selectionCompModeFromToolbarSig(QPainter::CompositionMode)),
      drawBoard, SIGNAL(selectionCompModeFromToolbarSig(QPainter::CompositionMode)));

   connect(this, SIGNAL(selectionCompAlphaFromToolbarSig(int)),
      drawBoard, SIGNAL(selectionCompAlphaFromToolbarSig(int)));

   connect(this, SIGNAL(selectionTransformationFromToolbarSig(TTransformationMode)),
      drawBoard, SIGNAL(selectionTransformationFromToolbarSig(TTransformationMode)));

   connect(this, SIGNAL(selectionRotationAxisFromToolbarSig(Qt::Axis, bool)),
      drawBoard, SIGNAL(selectionRotationAxisFromToolbarSig(Qt::Axis, bool)));

   connect(this, SIGNAL(selectionTransformationSquareFromToolbarSig(bool)),
      drawBoard, SIGNAL(selectionTransformationSquareFromToolbarSig(bool)));
}

void CMainWindow::resizeSubMDI(const QSize &size)
{
   QMdiSubWindow *activeSubWindow = 0;
   QList<QMdiSubWindow *> windowList = m_centralWidget->subWindowList();
   if (windowList.size() > 0)
   {
      activeSubWindow = windowList.last();
      if (0 == activeSubWindow)
      {
         return;
      }
   }
   else
   {
      return;
   }

   CDrawBoardFrame *frame = (CDrawBoardFrame *)activeSubWindow->widget();
   if (0 == frame)
   {
      return;
   }

   frame->setMinimumSize(size);
   activeSubWindow->resize(QSize(size.width() + 14, size.height() + 43));
   frame = 0;
   activeSubWindow = 0;
}

void CMainWindow::printInfo(const QString &txt)
{
   QString toPrint = txt;
   toLog(toPrint);
}

void CMainWindow::catchMouseInfo(const QPoint &pt)
{
   QString toLogStr = QString("%1,%2").arg(pt.x()).arg(pt.y());
   toLog(toLogStr);
}

void CMainWindow::slotNew()
{
   QString uniqueName = tr("Untitled_0");
   QHashIterator<QString, CDrawBoard*> it(drawBoards);
   QStringList names;
   while (it.hasNext())
   {
      it.next();
      names << it.key();
   }
   int counter = 1;
   while (names.contains(uniqueName))
   {
      uniqueName[uniqueName.size() - 1] = QString("%1").arg(counter).at(0);
      counter++;
   }

   newDialog = new CNewDrawboardDialog(this);
   newDialog->setImgName(uniqueName);

   connect(newDialog, SIGNAL(newImageSig(const QString &, const int, const int)),
      this, SLOT(receiveNewImage(const QString &, const int, const int)));

   newDialog->show();
}

void CMainWindow::receiveNewImage(const QString &name, const int width, const int height)
{
   slotNewDrawBoard(name, width, height);
}

void CMainWindow::slotLoad()
{
   // get an image file from a file dialog
   QString filename = QFileDialog::getOpenFileName(this, "Open Image", ".", "Images(*.bmp *.jpg *.png)");
   if (filename.isEmpty())
   {
      return;
   }

   // open the image
   QImage *img = new QImage(filename);
   if (!img->load(filename))
   {
      QMessageBox::warning(this, "Load error", "Could not load file");
      return;
   }

   // create a subwindow
   QFileInfo imgInfo(filename);
   QString mN = imgInfo.baseName();
   int     mW = img->size().width();
   int     mH = img->size().height();

   slotNewDrawBoard(mN, mW, mH);

   // retrieve the subwindow from the central mdi widget
   QMdiSubWindow *activeSubWindow = m_centralWidget->activeSubWindow();
   if (0 == activeSubWindow)
   {
      return;
   }
   CDrawBoardFrame *drawFrame = (CDrawBoardFrame *)activeSubWindow->widget();
   if (0 == drawFrame)
   {
      return;
   }

   // retrieve the drawboard pointer from local map
   QString mName = drawFrame->getName();
   if (!drawBoards.contains(mName))
   {
      return;
   }

   // initialize the draboard with the loaded image
   CDrawBoard *drawBoard = drawBoards.value(drawFrame->getName());
   drawBoard->slotLoad(filename, img);
   drawBoard = 0;
   drawFrame = 0;
   delete img;
   img = 0;
}

void CMainWindow::slotSave()
{
   QMdiSubWindow *activeSubWindow = m_centralWidget->activeSubWindow();
   if (0 == activeSubWindow)
   {
      return;
   }
   CDrawBoard *drawBoard = (CDrawBoard *)activeSubWindow->widget();
   if (0 == drawBoard)
   {
      return;
   }

   QString filename = QFileDialog::getSaveFileName(this, QString(tr("Save Image")), QString(), "Images(*.bmp *.jpg *.png)");
   if (!filename.isEmpty())
   {
      drawBoard->slotSave(filename);
   }
}

void CMainWindow::receiveShapeSettings(const CShapeSettings *shapeSettings)
{
   if(0 == shapeSettings)
   {
      return;
   }
}