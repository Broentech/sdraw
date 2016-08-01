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
//  \file      selectionSelector.cpp
//
//  \brief     Implementation of a class specify selection mode
//
//  \author    Stian Broen
//
//  \date      21.09.2011
//
//  \par       Copyright:
//
//             Copyright by Stian Broen
//
//  \par       Revision History
//
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QPushButton>
#include <QAction>
#include <QToolBar>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QDir>
#include <QLabel>
#include <QMenu>
#include <QWidgetAction>

// local includes
#include "selectionSelector.h"
#include "compositionMenu.h"

using namespace sdraw;

CSelectionSelector::CSelectionSelector(QWidget *parent)
   : QWidget(parent)
{
   currentTransformation = default_TRANSFORMATION;
   currentSelectionMode  = default_SELECTIONMODE;

   ignoreWhite         = true;
   inverseSelection    = false;
   cloneSelection      = false;
   combine             = false;
   squareTrans_enabled = true;
   rotate_x_enabled    = false;
   rotate_y_enabled    = false;
   rotate_z_enabled    = true;

   arrangeWidgets();
}

void CSelectionSelector::arrangeWidgets()
{
   arrangeSelectionWidgets();
   arrangeTransformationWidgets();
   arrangeOptionsWidgets();
   arrangeAxisWidgets();
   arrangeTransOptionsWidgets();

   QHBoxLayout *shapeLayout = new QHBoxLayout;
   shapeLayout->addWidget(selectionBar); 
   shapeBox = new QGroupBox(tr("Shapes"), this);
   shapeBox->setLayout(shapeLayout);

   QHBoxLayout *transLayout = new QHBoxLayout;
   transLayout->addWidget(transformationBar); 
   transBox = new QGroupBox(tr("Transformation"), this);
   transBox->setLayout(transLayout);

   QVBoxLayout *optionsLayout = new QVBoxLayout;
   optionsLayout->addWidget(optionsBar);
   optionsBox = new QGroupBox(tr("Options"), this);
   optionsBox->setLayout(optionsLayout);

   QHBoxLayout *axisLayout = new QHBoxLayout;
   axisLayout->addWidget(axisBar);
   axisBox = new QGroupBox(tr("Axis"), this);
   axisBox->setLayout(axisLayout);
   axisBox->setVisible(false);

   QVBoxLayout *transOptionsLayout = new QVBoxLayout;
   transOptionsLayout->addWidget(transOptionsBar);
   transOptionsBox = new QGroupBox(tr("Options"), this);
   transOptionsBox->setLayout(transOptionsLayout);
   transOptionsBox->setVisible(false);

   // total layout
   QHBoxLayout *topLayout = new QHBoxLayout;
   topLayout->addWidget(shapeBox);
   topLayout->addWidget(transBox);

   QHBoxLayout *bottomLayout = new QHBoxLayout;
   bottomLayout->addWidget(optionsBox);
   bottomLayout->addWidget(transOptionsBox);
   bottomLayout->addWidget(axisBox);

   QVBoxLayout *totalLayout = new QVBoxLayout;
   totalLayout->addLayout(topLayout);
   totalLayout->addLayout(bottomLayout);

   setLayout(totalLayout);
}

void CSelectionSelector::rectangularSelectionButtonClicked(bool)
{
   rectangularSelectionButton->setChecked(true);
   polygonSelectionButton    ->setChecked(false);
   freeSelectionButton       ->setChecked(false);
   currentSelectionMode = RECTANGULAR_SELECTION;
   emit updateSelectionModeFromWidgetSig(currentSelectionMode);
}

void CSelectionSelector::polygonSelectionButtonClicked(bool)
{
   rectangularSelectionButton->setChecked(false);
   polygonSelectionButton    ->setChecked(true);
   freeSelectionButton       ->setChecked(false);
   currentSelectionMode = POLYGON_SELECTION;
   emit updateSelectionModeFromWidgetSig(currentSelectionMode);
}

void CSelectionSelector::freeSelectionButtonClicked(bool)
{
   rectangularSelectionButton->setChecked(false);
   polygonSelectionButton    ->setChecked(false);
   freeSelectionButton       ->setChecked(true);
   currentSelectionMode = FREE_SELECTION;
   emit updateSelectionModeFromWidgetSig(currentSelectionMode);
}

void CSelectionSelector::moveButtonClicked  (bool)
{
   transMoveButton  ->setChecked(true);
   transScaleButton ->setChecked(false);
   transRotateButton->setChecked(false);
   transShearButton ->setChecked(false);

   optionsBox->setVisible(true);
   axisBox->setVisible(false);
   transOptionsBox->setVisible(false);

   currentTransformation = TRANS_MOVE;
   emit updateTransformationMode(currentTransformation);
}

void CSelectionSelector::scaleButtonClicked (bool)
{
   transMoveButton  ->setChecked(false);
   transScaleButton ->setChecked(true);
   transRotateButton->setChecked(false);
   transShearButton ->setChecked(false);

   optionsBox->setVisible(false);
   axisBox->setVisible(false);
   transOptionsBox->setVisible(true);

   currentTransformation = TRANS_SCALE;
   emit updateTransformationMode(currentTransformation);
}

void CSelectionSelector::rotateButtonClicked(bool)
{
   transMoveButton  ->setChecked(false);
   transScaleButton ->setChecked(false);
   transRotateButton->setChecked(true);
   transShearButton ->setChecked(false);

   optionsBox->setVisible(false);
   axisBox->setVisible(true);
   transOptionsBox->setVisible(false);

   currentTransformation = TRANS_ROTATE;
   emit updateTransformationMode(currentTransformation);
}

void CSelectionSelector::shearButtonClicked (bool)
{
   transMoveButton  ->setChecked(false);
   transScaleButton ->setChecked(false);
   transRotateButton->setChecked(false);
   transShearButton ->setChecked(true);

   optionsBox->setVisible(false);
   axisBox->setVisible(false);
   transOptionsBox->setVisible(true);

   currentTransformation = TRANS_SHEAR;
   emit updateTransformationMode(currentTransformation);
}

void CSelectionSelector::ignoreWhiteButtonClicked(bool checked)
{
   if(checked)
   {
      ignoreWhite = true;
      emit updateSelectionOptionFromWidgetSig(IGNOREWHITE_SELECTION_ON);
   }
   else
   {
      ignoreWhite = false;
      emit updateSelectionOptionFromWidgetSig(IGNOREWHITE_SELECTION_OFF);
   }
}

void CSelectionSelector::inverseButtonClicked(bool checked)
{
   if(checked)
   {
      inverseSelection = true;
      emit updateSelectionOptionFromWidgetSig(INVERSE_SELECTION_ON);
   }
   else
   {
      inverseSelection = false;
      emit updateSelectionOptionFromWidgetSig(INVERSE_SELECTION_OFF);
   }
}

void CSelectionSelector::cloneButtonClicked(bool checked)
{
   if(checked)
   {
      cloneSelection = true;
      emit updateSelectionOptionFromWidgetSig(CLONE_SELECTION_ON);
   }
   else
   {
      cloneSelection = false;
      emit updateSelectionOptionFromWidgetSig(CLONE_SELECTION_OFF);
   }
}

void CSelectionSelector::compositionModeButtonClicked(bool checked)
{
   if(checked)
   {
      combine = true;
      compositionMenuButton->setVisible(true);
      emit requestCompModeSig();
      emit updateSelectionOptionFromWidgetSig(COMBINE_SELECTION_ON);
   }
   else
   {
      combine = false;
      compositionMenuButton->setVisible(false);
      emit updateSelectionOptionFromWidgetSig(COMBINE_SELECTION_OFF);
   }
}

void CSelectionSelector::compositionMenuButtonClicked(bool)
{
   // TODO : non responsive to window movement, parentRect must get updated
   QRect thisRect = this->geometry();
   QPoint menuPos;
   menuPos.setX(  parentRect.topLeft().x()  +  thisRect.bottomRight().x() - 300);
   menuPos.setY(  parentRect.topLeft().y()  +  thisRect.bottomRight().y() + 8);
   compositionMenu->exec(menuPos);
}

void CSelectionSelector::resondToRequestAll()
{
   emit updateSelectionModeFromWidgetSig(currentSelectionMode);
   emit updateTransformationMode(currentTransformation);
   emit updateRotationAxis(Qt::XAxis, rotate_x_enabled);
   emit updateRotationAxis(Qt::YAxis, rotate_y_enabled);
   emit updateRotationAxis(Qt::ZAxis, rotate_z_enabled);
   emit updateTransformationSquare(squareTrans_enabled);
   if(ignoreWhite)
   {
      emit updateSelectionOptionFromWidgetSig(IGNOREWHITE_SELECTION_ON);
   }
   else
   {
      emit updateSelectionOptionFromWidgetSig(IGNOREWHITE_SELECTION_OFF);
   }
   if(inverseSelection)
   {
      emit updateSelectionOptionFromWidgetSig(INVERSE_SELECTION_ON);
   }
   else
   {
      emit updateSelectionOptionFromWidgetSig(INVERSE_SELECTION_OFF);
   }
   if(cloneSelection)
   {
      emit updateSelectionOptionFromWidgetSig(CLONE_SELECTION_ON);
   }
   else
   {
      emit updateSelectionOptionFromWidgetSig(CLONE_SELECTION_OFF);
   }
}

void CSelectionSelector::arrangeSelectionWidgets()
{
   selectionBar = new QToolBar(tr("Selections"), this);
   selectionBar->setIconSize(QSize(default_SHAPE_BUTTON_WIDTH,default_SHAPE_BUTTON_HEIGHT));
   selectionBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
   rectangularSelectionButton = new QAction(tr("Rectangle"),this);
   polygonSelectionButton     = new QAction(tr("Polygon"  ),this);
   freeSelectionButton        = new QAction(tr("Free"     ),this);
   selectionBar->addAction(rectangularSelectionButton);
   selectionBar->addAction(polygonSelectionButton);
   selectionBar->addAction(freeSelectionButton);

   connect(rectangularSelectionButton, SIGNAL(triggered(bool)), this, SLOT(rectangularSelectionButtonClicked(bool)));
   connect(polygonSelectionButton    , SIGNAL(triggered(bool)), this, SLOT(polygonSelectionButtonClicked    (bool)));
   connect(freeSelectionButton       , SIGNAL(triggered(bool)), this, SLOT(freeSelectionButtonClicked       (bool)));

   rectangularSelectionButton->setToolTip(tr("Rectangular"));
   polygonSelectionButton    ->setToolTip(tr("Polygon"));
   freeSelectionButton       ->setToolTip(tr("Free"));

   rectangularSelectionButton ->setCheckable(true);
   rectangularSelectionButton ->setChecked  (true);
   freeSelectionButton        ->setCheckable(true);
   polygonSelectionButton     ->setCheckable(true);

   // make button background
   background = QPixmap(default_SHAPE_BUTTON_WIDTH,default_SHAPE_BUTTON_HEIGHT);
   QLinearGradient linearGrad(QPointF(0.0,0.0), QPointF(default_SHAPE_BUTTON_WIDTH, default_SHAPE_BUTTON_HEIGHT));
   linearGrad.setColorAt(0, Qt::lightGray);
   linearGrad.setColorAt(1, Qt::white);
   QBrush backgroundBrush(linearGrad);
   QPainter backgroundPainter(&background);
   backgroundPainter.fillRect(QRectF(0,0,default_SHAPE_BUTTON_WIDTH, default_SHAPE_BUTTON_HEIGHT), backgroundBrush);

   // rectangle button icon
   QPixmap tmpPixmap2 = background;
   QPainter painter2( &tmpPixmap2 );
   painter2.setRenderHint(QPainter::Antialiasing,true);
   painter2.drawRect(QRect(10,10,default_SHAPE_BUTTON_WIDTH - 20, default_SHAPE_BUTTON_HEIGHT - 20));
   rectangularSelectionButton->setIcon(QIcon(tmpPixmap2));

   // polygon button icon
   static const QPointF points[4] = {
      QPointF(10.0, 10.0),
      QPointF(default_SHAPE_BUTTON_WIDTH - 10, 20.0),
      QPointF(default_SHAPE_BUTTON_WIDTH - 30.0, default_SHAPE_BUTTON_HEIGHT - 10),
      QPointF(default_SHAPE_BUTTON_WIDTH/2.0, default_SHAPE_BUTTON_HEIGHT/2.0),
   };
   QPixmap tmpPixmap9 = background;
   QPainter painter9( &tmpPixmap9 );
   painter9.setRenderHint(QPainter::Antialiasing,true);
   painter9.drawPolygon(points, 4);
   polygonSelectionButton->setIcon(QIcon(tmpPixmap9));

   freeSelectionButton->setIcon(QIcon(background));
}

void CSelectionSelector::arrangeTransformationWidgets()
{
   transformationBar = new QToolBar(tr("Transformations"), this);
   transformationBar->setIconSize(QSize(default_SHAPE_BUTTON_WIDTH,default_SHAPE_BUTTON_HEIGHT));
   transformationBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
   transMoveButton   = new QAction(tr("Move"  ), this);
   transScaleButton  = new QAction(tr("Scale" ), this);
   transRotateButton = new QAction(tr("Rotate"), this);
   transShearButton  = new QAction(tr("Shear" ), this);
   transformationBar->addAction(transMoveButton);
   transformationBar->addAction(transScaleButton);
   transformationBar->addAction(transRotateButton);
   transformationBar->addAction(transShearButton);

   transMoveButton  ->setCheckable(true);
   transScaleButton ->setCheckable(true);
   transRotateButton->setCheckable(true);
   transShearButton ->setCheckable(true);

   switch(currentTransformation)
   {
      case(TRANS_NO):
      {
         transMoveButton->setChecked(true);
         currentTransformation = TRANS_MOVE;
         break;
      }
      case(TRANS_MOVE):
      {
         transMoveButton->setChecked(true);
         currentTransformation = TRANS_MOVE;
         break;
      }
      case(TRANS_SCALE):
      {
         transScaleButton->setChecked(true);
         break;
      }
      case(TRANS_ROTATE):
      {
         transRotateButton->setChecked(true);
         break;
      }
      case(TRANS_SHEAR):
      {
         transShearButton->setChecked(true);
         break;
      }
      default:
      {
         transMoveButton->setChecked(true);
         currentTransformation = TRANS_MOVE;
         break;
      }
   }

   transMoveButton->setIcon(QIcon(QPixmap::QPixmap(":/images/Transformations/translate.png") //< make an icon from a pixmap created from the argument path
                                  .scaled(default_SHAPE_BUTTON_WIDTH  ,      //< scale the pixmap to this width
                                          default_SHAPE_BUTTON_HEIGHT ,      //< scale the pixmap to this height
                                          Qt::KeepAspectRatio         ,      //< keep the original shape 
                                          Qt::SmoothTransformation)) );      //< use a smoothing algorithm

   transScaleButton->setIcon(QIcon(QPixmap::QPixmap(":/images/Transformations/scale.png")    //< make an icon from a pixmap created from the argument path
                                  .scaled(default_SHAPE_BUTTON_WIDTH  ,      //< scale the pixmap to this width
                                          default_SHAPE_BUTTON_HEIGHT ,      //< scale the pixmap to this height
                                          Qt::KeepAspectRatio         ,      //< keep the original shape 
                                          Qt::SmoothTransformation)) );      //< use a smoothing algorithm

   transRotateButton->setIcon(QIcon(QPixmap::QPixmap(":/images/Transformations/rotate.png")  //< make an icon from a pixmap created from the argument path
                                  .scaled(default_SHAPE_BUTTON_WIDTH   ,     //< scale the pixmap to this width
                                          default_SHAPE_BUTTON_HEIGHT  ,     //< scale the pixmap to this height
                                          Qt::KeepAspectRatio          ,     //< keep the original shape 
                                          Qt::SmoothTransformation))  );     //< use a smoothing algorithm

   transShearButton->setIcon(QIcon(QPixmap::QPixmap(":/images/Transformations/shear.png")    //< make an icon from a pixmap created from the argument path
                                  .scaled(default_SHAPE_BUTTON_WIDTH   ,     //< scale the pixmap to this width
                                          default_SHAPE_BUTTON_HEIGHT  ,     //< scale the pixmap to this height
                                          Qt::KeepAspectRatio          ,     //< keep the original shape 
                                          Qt::SmoothTransformation))  );     //< use a smoothing algorithm
   

   connect(transMoveButton   , SIGNAL(triggered(bool)), this, SLOT(moveButtonClicked  (bool)));
   connect(transScaleButton  , SIGNAL(triggered(bool)), this, SLOT(scaleButtonClicked (bool)));
   connect(transRotateButton , SIGNAL(triggered(bool)), this, SLOT(rotateButtonClicked(bool)));
   connect(transShearButton  , SIGNAL(triggered(bool)), this, SLOT(shearButtonClicked (bool)));

   transMoveButton  ->setToolTip(tr("Move the selection around"));
   transScaleButton ->setToolTip(tr("Enlarge or shrink the content"));
   transRotateButton->setToolTip(tr("Rotate the content"));
   transShearButton ->setToolTip(tr("Shear the content"));

}

void CSelectionSelector::arrangeOptionsWidgets()
{
   optionsBar   = new QToolBar(tr("Options"), this);
   optionsBar->setIconSize(QSize(default_SHAPE_BUTTON_WIDTH,default_SHAPE_BUTTON_HEIGHT));
   optionsBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

   ignoreWhiteButton        = new QAction  (tr("Ignore White"),this);
   inverseButton            = new QAction  (tr("Inverse"     ),this);
   cloneButton              = new QAction  (tr("Clone"       ),this);
   compositionModeButton    = new QAction  (tr("Combine"     ),this);
   compositionMenuButton    = new QAction  (tr("Mode"        ), this);
   compositionMenuButton->setVisible(false);
   compositionMenu = new QMenu(this);
   QWidgetAction *compMenuWidgetAction = new QWidgetAction(this);
   CCompositionMenu *compMenu = new CCompositionMenu(compositionMenu);
   
   connect(compMenu, SIGNAL(compModeSig         (QPainter::CompositionMode)), 
           this    , SLOT(receiveCompositionMode(QPainter::CompositionMode)));
   connect(compMenu, SIGNAL(changeAlphaFromCompositionMenuSig(int)), 
           this    , SLOT(receiveCompositionAlpha            (int)));
   connect(compMenu, SIGNAL(doCloseCompositionMenuSig()), 
           this    , SLOT(closeCompositionMenu       ()));
   connect(this    , SIGNAL(requestCompModeSig()), 
           compMenu, SLOT(currModeRequested   ()));

   emit requestCompModeSig();
   compMenuWidgetAction->setDefaultWidget(compMenu);
   compositionMenu->addAction(compMenuWidgetAction);
   compositionMenuButton->setMenu(compositionMenu);

   // TODO : take these two back into the application
   //optionsBar->addAction(ignoreWhiteButton);
   //optionsBar->addAction(inverseButton);
   optionsBar->addAction(cloneButton);
   optionsBar->addAction(compositionModeButton);
   optionsBar->addAction(compositionMenuButton);

   ignoreWhiteButton    ->setCheckable(true);
   ignoreWhiteButton    ->setChecked  (true);
   inverseButton        ->setCheckable(true);
   cloneButton          ->setCheckable(true);
   compositionModeButton->setCheckable(true);
   compositionMenuButton->setCheckable(false);

   ignoreWhiteButton    ->setToolTip(tr("Don't Overwrite White Pixels"     ));
   inverseButton        ->setToolTip(tr("Select Unselected Area"           ));
   cloneButton          ->setToolTip(tr("Clone Selection"                  ));
   compositionModeButton->setToolTip(tr("Combine Selection with Background"));
   compositionMenuButton->setToolTip(tr("Composition"                      ));

   ignoreWhiteButton->setIcon(QIcon(QPixmap::QPixmap(":/images/Segmentation/ignoreWhite.png")  //< make an icon from a pixmap created from the argument path
                                  .scaled(default_SHAPE_BUTTON_WIDTH    ,  //< scale the pixmap to this width
                                          default_SHAPE_BUTTON_HEIGHT   ,  //< scale the pixmap to this height
                                          Qt::KeepAspectRatio           ,  //< keep the original shape 
                                          Qt::SmoothTransformation))   );  //< use a smoothing algorithm

   inverseButton->setIcon(QIcon(QPixmap::QPixmap(":/images/Segmentation/inverse.png")          //< make an icon from a pixmap created from the argument path
                                  .scaled(default_SHAPE_BUTTON_WIDTH    ,  //< scale the pixmap to this width
                                          default_SHAPE_BUTTON_HEIGHT   ,  //< scale the pixmap to this height
                                          Qt::KeepAspectRatio           ,  //< keep the original shape 
                                          Qt::SmoothTransformation))   );  //< use a smoothing algorithm

   cloneButton->setIcon(QIcon(QPixmap::QPixmap(":/images/Segmentation/clone_.png")              //< make an icon from a pixmap created from the argument path
                                  .scaled(default_SHAPE_BUTTON_WIDTH    ,  //< scale the pixmap to this width
                                          default_SHAPE_BUTTON_HEIGHT   ,  //< scale the pixmap to this height
                                          Qt::KeepAspectRatio           ,  //< keep the original shape 
                                          Qt::SmoothTransformation))   );  //< use a smoothing algorithm

   compositionMenuButton->setIcon(QIcon(QPixmap::QPixmap(":/images/Segmentation/composition.png")  //< make an icon from a pixmap created from the argument path
                                  .scaled(default_SHAPE_BUTTON_WIDTH    ,      //< scale the pixmap to this width
                                          default_SHAPE_BUTTON_HEIGHT   ,      //< scale the pixmap to this height
                                          Qt::KeepAspectRatio           ,      //< keep the original shape 
                                          Qt::SmoothTransformation))   );      //< use a smoothing algorithm
    
   compositionModeButton->setIcon(QIcon(QPixmap::QPixmap(":/images/Segmentation/composition.png")  //< make an icon from a pixmap created from the argument path
                                  .scaled(default_SHAPE_BUTTON_WIDTH    ,      //< scale the pixmap to this width
                                          default_SHAPE_BUTTON_HEIGHT   ,      //< scale the pixmap to this height
                                          Qt::KeepAspectRatio           ,      //< keep the original shape 
                                          Qt::SmoothTransformation))   );      //< use a smoothing algorithm
   

   connect(ignoreWhiteButton     , SIGNAL(triggered(bool)), this, SLOT(ignoreWhiteButtonClicked    (bool)));
   connect(inverseButton         , SIGNAL(triggered(bool)), this, SLOT(inverseButtonClicked        (bool)));
   connect(cloneButton           , SIGNAL(triggered(bool)), this, SLOT(cloneButtonClicked          (bool)));
   connect(compositionModeButton , SIGNAL(triggered(bool)), this, SLOT(compositionModeButtonClicked(bool)));
   connect(compositionMenuButton , SIGNAL(triggered(bool)), this, SLOT(compositionMenuButtonClicked(bool)));
}

void CSelectionSelector::arrangeAxisWidgets()
{
   axisBar   = new QToolBar(tr("Axis"), this);
   axisBar->setIconSize(QSize(default_SHAPE_BUTTON_WIDTH,default_SHAPE_BUTTON_HEIGHT));
   axisBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

   X_axisButton = new QAction ("X",this);
   Y_axisButton = new QAction ("Y",this);
   Z_axisButton = new QAction ("Z",this);

   X_axisButton->setToolTip(tr("Rotate the selection around the X axis"));
   Y_axisButton->setToolTip(tr("Rotate the selection around the Y axis"));
   Z_axisButton->setToolTip(tr("Rotate the selection around the Z axis"));

   axisBar->addAction(X_axisButton);
   axisBar->addAction(Y_axisButton);
   axisBar->addAction(Z_axisButton);

   X_axisButton->setCheckable(true);
   Y_axisButton->setCheckable(true);
   Z_axisButton->setCheckable(true);
   Z_axisButton->setChecked  (true);

   X_axisButton->setIcon(QIcon(QPixmap::QPixmap(":/images/Transformations/x_.png") //< make an icon from a pixmap created from the argument path
                                  .scaled(default_SHAPE_BUTTON_WIDTH    ,  //< scale the pixmap to this width
                                          default_SHAPE_BUTTON_HEIGHT   ,  //< scale the pixmap to this height
                                          Qt::KeepAspectRatio           ,  //< keep the original shape 
                                          Qt::SmoothTransformation))   );  //< use a smoothing algorithm

   Y_axisButton->setIcon(QIcon(QPixmap::QPixmap(":/images/Transformations/y_.png") //< make an icon from a pixmap created from the argument path
                                  .scaled(default_SHAPE_BUTTON_WIDTH    ,  //< scale the pixmap to this width
                                          default_SHAPE_BUTTON_HEIGHT   ,  //< scale the pixmap to this height
                                          Qt::KeepAspectRatio           ,  //< keep the original shape 
                                          Qt::SmoothTransformation))   );  //< use a smoothing algorithm

   Z_axisButton->setIcon(QIcon(QPixmap::QPixmap(":/images/Transformations/z_.png") //< make an icon from a pixmap created from the argument path
                                  .scaled(default_SHAPE_BUTTON_WIDTH    ,  //< scale the pixmap to this width
                                          default_SHAPE_BUTTON_HEIGHT   ,  //< scale the pixmap to this height
                                          Qt::KeepAspectRatio           ,  //< keep the original shape 
                                          Qt::SmoothTransformation))   );  //< use a smoothing algorithm
   

   connect(X_axisButton , SIGNAL(triggered(bool)), this, SLOT(XButtonClicked(bool)));
   connect(Y_axisButton , SIGNAL(triggered(bool)), this, SLOT(YButtonClicked(bool)));
   connect(Z_axisButton , SIGNAL(triggered(bool)), this, SLOT(ZButtonClicked(bool)));
}

void CSelectionSelector::XButtonClicked(bool checked)
{
   rotate_x_enabled = checked;
   emit updateRotationAxis(Qt::XAxis, checked);
   X_axisButton->setChecked(checked);
}

void CSelectionSelector::YButtonClicked(bool checked)
{
   rotate_y_enabled = checked;
   emit updateRotationAxis(Qt::YAxis, checked);
   Y_axisButton->setChecked(checked);
}

void CSelectionSelector::ZButtonClicked(bool checked)
{
   rotate_z_enabled = checked;
   emit updateRotationAxis(Qt::ZAxis, checked);
   Z_axisButton->setChecked(checked);
}

void CSelectionSelector::arrangeTransOptionsWidgets()
{
   transOptionsBar   = new QToolBar(tr("Options"), this);
   transOptionsBar->setIconSize(QSize(default_SHAPE_BUTTON_WIDTH,default_SHAPE_BUTTON_HEIGHT));
   transOptionsBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

   squareTransButton = new QAction (tr("Square"),this);
   squareTransButton->setToolTip(tr("Square transformations"));
   transOptionsBar->addAction(squareTransButton);
   squareTransButton->setCheckable(true);
   squareTransButton->setChecked  (true);

   squareTransButton->setIcon(QIcon(QPixmap::QPixmap(":/images/Transformations/squareTrans.png")       //< make an icon from a pixmap created from the argument path
                                  .scaled(default_SHAPE_BUTTON_WIDTH    ,  //< scale the pixmap to this width
                                          default_SHAPE_BUTTON_HEIGHT   ,  //< scale the pixmap to this height
                                          Qt::KeepAspectRatio           ,  //< keep the original shape 
                                          Qt::SmoothTransformation))   );  //< use a smoothing algorithm
   
   connect(squareTransButton , SIGNAL(triggered(bool)), this, SLOT(squareTransButtonClicked(bool)));
}

void CSelectionSelector::squareTransButtonClicked(bool checked)
{
   squareTrans_enabled = checked;
   emit updateTransformationSquare(squareTrans_enabled);
}

void CSelectionSelector::receiveCompositionMode(QPainter::CompositionMode mode)
{ 
   emit updateCompositionMode(mode); 
}

void CSelectionSelector::receiveCompositionAlpha(int value)
{ 
   emit updateCompositionAlpha(value); 
}

void CSelectionSelector::closeCompositionMenu()
{ 
   compositionMenu->close(); 
}