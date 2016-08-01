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
//  \file      penSelector.cpp
//
//  \brief     Declaration of a class to define pen
//
//  \author    Stian Broen
//
//  \date      August-2011
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
#include <QToolButton>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFile>
#include <QDir>
#include <QLabel>
#include <QSpacerItem>
#include <QSpinBox>
#include <QSlider>
#include <QPen>
#include <QPixmap>
#include <QTimer>
#include <QPainter>
#include <QColorDialog>
#include <QApplication>
#include <QGroupBox>

// solution includes
#include <DrawSettings/DrawSettings_2D.h>

// local includes
#include "penSelector.h"

using namespace sdraw;

CPenSelector::CPenSelector(QWidget *parent)
   : SdrawCustomWidget(parent)
{
   m_widthSelector = 0;
   allButtonsLayout = new QGridLayout;

   // setup all widgets
   arrangeWidthSelection();
   arrangeStyleButtons();
   arrangeCapButtons();
   arrangeJoinButtons();
   arrangeColorSelection();
   arrangeDemo();

   QGroupBox *box = new QGroupBox(this);
   box->setTitle(tr("Pen Settings"));
   box->setLayout(allButtonsLayout);

   totalLayout = new QVBoxLayout;
   totalLayout->addLayout(SdrawCustomWidget::setCloseLayout());

   totalLayout->addWidget(box);
   setLayout(totalLayout);
}

CPenSelector::~CPenSelector()
{

}

void CPenSelector::arrangeDemo()
{
   demo = new CPenDemo(this);
   QHBoxLayout *demoLayout = new QHBoxLayout;
   demoLayout->addWidget(demo);

   QGroupBox *demoBox = new QGroupBox(this);
   demoBox->setTitle(tr("Demo"));
   demoBox->setLayout(demoLayout);

   allButtonsLayout->addWidget(demoBox, 3, 1, 3, 1);
}

void CPenSelector::arrangeColorSelection()
{
   colorPicker = new ChooseColor(this);
   colorPicker->setMinimumHeight(3*default_PEN_BUTTON_HEIGHT);

   connect(colorPicker, SIGNAL(colorChanged(const QColor &)), 
      this, SLOT(colorChanged(const QColor &)));

   //QLabel *colorLabel = new QLabel(tr("<b>Color</b>"), this);
   //allButtonsLayout->addWidget(colorLabel, 6, 0);

   QHBoxLayout *colorLayout = new QHBoxLayout;
   colorLayout->addWidget(colorPicker);

   QGroupBox *colorGroup = new QGroupBox(this);
   colorGroup->setTitle(tr("Color"));
   colorGroup->setLayout(colorLayout);

   allButtonsLayout->addWidget(colorGroup, 5, 0, 1, 1);
   //allButtonsLayout->addWidget(colorPicker, 7,0,4,3);
}

void CPenSelector::mouseReleaseEvent(QMouseEvent *)
{
   QApplication::setActiveWindow(this);
}

void CPenSelector::colorChanged(const QColor &col)
{
   CPenSettings pns = CDrawSettings_2D::getInstance()->penSettings();
   pns.setColor(col);
   CDrawSettings_2D::getInstance()->setPenSettings(pns);
}

void CPenSelector::arrangeWidthSelection()
{
   // create top label
   //QLabel *widthLabel = new QLabel(this);
   //widthLabel->setText(tr("<b>Width</b>"));

   m_widthSelector    = new CPenWidthSelector(this);

   connect(m_widthSelector, SIGNAL(updatingWidthSig(int)), 
      this, SLOT(widthChanging(int)));

   connect(m_widthSelector, SIGNAL(updateWidthSig  (int)), 
      this, SLOT(widthChanged (int)));

   QHBoxLayout *widthLayout = new QHBoxLayout;
   widthLayout->addWidget(m_widthSelector);

   QGroupBox *widthGroup = new QGroupBox(this);
   widthGroup->setTitle(tr("Width"));
   widthGroup->setLayout(widthLayout);
   allButtonsLayout->addWidget(widthGroup, 2, 1, 1, 1);
}

void CPenSelector::widthChanging(int)
{

}

void CPenSelector::widthChanged(int val)
{
   CPenSettings pns = CDrawSettings_2D::getInstance()->penSettings();
   pns.setWidth(val);
   CDrawSettings_2D::getInstance()->setPenSettings(pns);
}

void CPenSelector::arrangeStyleButtons()
{
   // create top label
   //QLabel *styleLabel = new QLabel(this);
   //styleLabel->setText(tr("<b>Style</b>"));

   // create buttons
   solidButton      = new QPushButton(this);
   dashButton       = new QPushButton(this);
   dotButton        = new QPushButton(this);
   dashDotButton    = new QPushButton(this);
   dashDotDotButton = new QPushButton(this);

   // set tooltip
   solidButton      ->setToolTip(tr("Solid Line : A plain line."));
   dashButton       ->setToolTip(tr("Dash Line : Dashes separated by a few pixels."));
   dotButton        ->setToolTip(tr("Dot Line : Dots separated by a few pixels."));
   dashDotButton    ->setToolTip(tr("Dash Dot Line : Alternate dots and dashes."));
   dashDotDotButton ->setToolTip(tr("Dash Dot Dot Line : One dash, two dots, one dash, two dots."));

   // set button sizes
   solidButton      ->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   dashButton       ->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   dotButton        ->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   dashDotButton    ->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   dashDotDotButton ->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));

   // give icons to buttons
   solidButton     ->setIcon(QIcon(":/images/SolidLine.png"     ));
   dashButton      ->setIcon(QIcon(":/images/DashLine.png"      ));
   dotButton       ->setIcon(QIcon(":/images/DotLine.png"       ));
   dashDotButton   ->setIcon(QIcon(":/images/DashDotLine.png"   ));
   dashDotDotButton->setIcon(QIcon(":/images/DashDotDotLine.png"));

   // set buttons to checkable, check solidLine as default
   solidButton      ->setCheckable(true);
   dashButton       ->setCheckable(true);
   dotButton        ->setCheckable(true);
   dashDotButton    ->setCheckable(true);
   dashDotDotButton ->setCheckable(true);
   solidButton      ->setChecked(true);

   styleButtons = new QButtonGroup(this);
   styleButtons->addButton(solidButton      , static_cast<int>(Qt::SolidLine     ));
   styleButtons->addButton(dashButton       , static_cast<int>(Qt::DashLine      ));
   styleButtons->addButton(dotButton        , static_cast<int>(Qt::DotLine       ));
   styleButtons->addButton(dashDotButton    , static_cast<int>(Qt::DashDotLine   ));
   styleButtons->addButton(dashDotDotButton , static_cast<int>(Qt::DashDotDotLine));

   styleButtons->setExclusive(true);
   connect(styleButtons, SIGNAL(buttonClicked(int)), this, SLOT(someStyleButtonClicked(int)));

   // make connections
   //connect(solidButton     , SIGNAL(clicked(bool)), this, SLOT(solidStyleClicked     (bool)));
   //connect(dashButton      , SIGNAL(clicked(bool)), this, SLOT(dashStyleClicked      (bool)));
   //connect(dotButton       , SIGNAL(clicked(bool)), this, SLOT(dotStyleClicked       (bool)));
   //connect(dashDotButton   , SIGNAL(clicked(bool)), this, SLOT(dashDotStyleClicked   (bool)));
   //connect(dashDotDotButton, SIGNAL(clicked(bool)), this, SLOT(dashDotDotStyleClicked(bool)));

   // make solid button layout
   if(0 == allButtonsLayout)
   {
      return;
   }
   //allButtonsLayout->addWidget(styleLabel,0,0);

   QHBoxLayout *styleButtonsLayout = new QHBoxLayout;
   styleButtonsLayout->addWidget(solidButton);
   styleButtonsLayout->addWidget(dotButton);
   styleButtonsLayout->addWidget(dashButton);
   styleButtonsLayout->addWidget(dashDotButton);
   styleButtonsLayout->addWidget(dashDotDotButton);
   QGroupBox *styleBox = new QGroupBox(this);
   styleBox->setTitle(tr("Style"));
   styleBox->setLayout(styleButtonsLayout);

   allButtonsLayout->addWidget(styleBox, 1, 0, 1, 2);

   /*allButtonsLayout->addLayout(createButtonLayout(solidButton      , QString(tr("Solid"       ))), 1, 0);
   allButtonsLayout->addLayout(createButtonLayout(dotButton        , QString(tr("Dot"         ))), 1, 1);
   allButtonsLayout->addLayout(createButtonLayout(dashButton       , QString(tr("Dash"        ))), 1, 2);
   allButtonsLayout->addLayout(createButtonLayout(dashDotButton    , QString(tr("Dash-Dot"    ))), 1, 3);
   allButtonsLayout->addLayout(createButtonLayout(dashDotDotButton , QString(tr("Dash-Dot-Dot"))), 1, 4);*/

   //QLabel *descriptionLabelSolid = new QLabel(tr("Solid"), this);
   ////QHBoxLayout *solidLabelLayout = new QHBoxLayout;
   ////solidLabelLayout->addStretch();
   ////solidLabelLayout->addWidget(descriptionLabelSolid);
   ////solidLabelLayout->addStretch();
   ////QVBoxLayout *solidLayout = new QVBoxLayout;
   ////solidLayout->addWidget(solidButton);
   ////solidLayout->addLayout(solidLabelLayout);

   //// make dot button layout
   //QLabel *descriptionLabelDot = new QLabel(tr("Dot"), this);
   ////QHBoxLayout *dotLabelLayout = new QHBoxLayout;
   ////dotLabelLayout->addStretch();
   ////dotLabelLayout->addWidget(descriptionLabelDot);
   ////dotLabelLayout->addStretch();
   ////QVBoxLayout *dotLayout = new QVBoxLayout;
   ////dotLayout->addWidget(dotButton);
   ////dotLayout->addLayout(dotLabelLayout);

   //// make dash button layout
   //QLabel *descriptionLabelDash = new QLabel(tr("Dash"), this);
   ////QHBoxLayout *dashLabelLayout = new QHBoxLayout;
   ////dashLabelLayout->addStretch();
   ////dashLabelLayout->addWidget(descriptionLabelDash);
   ////dashLabelLayout->addStretch();
   ////QVBoxLayout *dashLayout = new QVBoxLayout;
   ////dashLayout->addWidget(dashButton);
   ////dashLayout->addLayout(dashLabelLayout);

   //// make dash-dot button layout
   //QLabel *descriptionLabelDashDot = new QLabel(tr("Dash-Dot"), this);
   ////QHBoxLayout *dashDotLabelLayout = new QHBoxLayout;
   ////dashDotLabelLayout->addStretch();
   ////dashDotLabelLayout->addWidget(descriptionLabelDashDot);
   ////dashDotLabelLayout->addStretch();
   ////QVBoxLayout *dashDotLayout = new QVBoxLayout;
   ////dashDotLayout->addWidget(dashButton);
   ////dashDotLayout->addLayout(dashDotLabelLayout);

   //QLabel *descriptionLabelDashDotDot = new QLabel(tr("Dash-Dot-Dot"), this);

   //// description labels layout
   //QHBoxLayout *descriptionLayout = new QHBoxLayout;
   //descriptionLayout->addWidget(descriptionLabelSolid);
   //descriptionLayout->addWidget(descriptionLabelDot);
   //descriptionLayout->addWidget(descriptionLabelDash);
   //descriptionLayout->addWidget(descriptionLabelDashDot);
   //descriptionLayout->addWidget(descriptionLabelDashDotDot);

   //descriptionLayout->setAlignment(descriptionLabelSolid      , Qt::AlignHCenter);
   //descriptionLayout->setAlignment(descriptionLabelDot        , Qt::AlignHCenter);
   //descriptionLayout->setAlignment(descriptionLabelDash       , Qt::AlignHCenter);
   //descriptionLayout->setAlignment(descriptionLabelDashDot    , Qt::AlignHCenter);
   //descriptionLayout->setAlignment(descriptionLabelDashDotDot , Qt::AlignHCenter);

   //// define layout
   //QHBoxLayout *styleLayout = new QHBoxLayout;
   //styleLayout->addWidget(solidButton);
   //styleLayout->addWidget(dashButton);
   //styleLayout->addWidget(dotButton);
   //styleLayout->addWidget(dashDotButton);
   //styleLayout->addWidget(dashDotDotButton);

   //styleAndTextLayout = new QVBoxLayout;
   //styleAndTextLayout->addWidget(styleLabel);
   //styleAndTextLayout->addLayout(styleLayout);
   //styleAndTextLayout->addLayout(descriptionLayout);
   //styleAndTextLayout->addSpacing(15);
}

void CPenSelector::arrangeCapButtons()
{
   // create top label
   //QLabel *capLabel = new QLabel(this);
   //capLabel->setText(tr("<b>Cap</b>"));

   // create buttons
   squareButton = new QPushButton(this);
   flatButton   = new QPushButton(this);
   roundButton  = new QPushButton(this);

   // set tooltip
   squareButton->setToolTip(tr("Square Cap : A square line end that covers the end point and extends beyond it by half the line width."));
   flatButton  ->setToolTip(tr("Flat Cap : A square line end that does not cover the end point of the line."));
   roundButton ->setToolTip(tr("Round Cap : A rounded line end."));

   // set button sizes
   squareButton->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   flatButton  ->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   roundButton ->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   squareButton->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
   flatButton  ->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
   roundButton ->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));

   // give icons to buttons
   squareButton->setIcon(QIcon(":/images/squareCap.png"));
   flatButton  ->setIcon(QIcon(":/images/flatCap.png"  ));
   roundButton ->setIcon(QIcon(":/images/roundCap.png" ));

   // set buttons to checkable, check round cap as default
   squareButton ->setCheckable(true);
   flatButton   ->setCheckable(true);
   roundButton  ->setCheckable(true);
   roundButton  ->setChecked(true);

   capButtons = new QButtonGroup(this);
   capButtons->addButton(squareButton , static_cast<int>(Qt::SquareCap ));
   capButtons->addButton(flatButton   , static_cast<int>(Qt::FlatCap   ));
   capButtons->addButton(roundButton  , static_cast<int>(Qt::RoundCap  ));

   capButtons->setExclusive(true);
   connect(capButtons, SIGNAL(buttonClicked(int)), this, SLOT(someCapButtonClicked(int)));

   // make cap button layout
   if(0 == allButtonsLayout)
   {
      return;
   }

   QHBoxLayout *capLayout = new QHBoxLayout;
   capLayout->addWidget(squareButton);
   capLayout->addWidget(flatButton);
   capLayout->addWidget(roundButton);

   QGroupBox *capBox = new QGroupBox(this);
   capBox->setTitle(tr("Cap"));
   capBox->setLayout(capLayout);

   allButtonsLayout->addWidget(capBox, 2, 0, 1, 1);

   //allButtonsLayout->addWidget(capLabel, 2, 0);
   //allButtonsLayout->addLayout(createButtonLayout(squareButton , QString(tr("Square" ))), 3, 0);
   //allButtonsLayout->addLayout(createButtonLayout(flatButton   , QString(tr("Flat"   ))), 3, 1);
   //allButtonsLayout->addLayout(createButtonLayout(roundButton  , QString(tr("Round"  ))), 3, 2);

   //// make connections
   //connect(squareButton , SIGNAL(clicked(bool)), this, SLOT(squareCapClicked (bool)));
   //connect(flatButton   , SIGNAL(clicked(bool)), this, SLOT(flatCapClicked   (bool)));
   //connect(roundButton  , SIGNAL(clicked(bool)), this, SLOT(roundCapClicked  (bool)));

   //// make button description labels
   //QLabel *descriptionLabelSquare   = new QLabel(tr("Square"), this);
   //QLabel *descriptionLabelFlat     = new QLabel(tr("Flat"  ), this);
   //QLabel *descriptionLabelRoundCap = new QLabel(tr("Round" ), this);

   //// description labels layout
   //QVBoxLayout *roundBoth = new QVBoxLayout;
   //roundBoth->addWidget(roundButton);
   //roundBoth->addWidget(descriptionLabelRoundCap);
   //roundBoth->setAlignment(descriptionLabelRoundCap , Qt::AlignHCenter);

   //QVBoxLayout *flatBoth = new QVBoxLayout;
   //flatBoth->addWidget(flatButton);
   //flatBoth->addWidget(descriptionLabelFlat);
   //flatBoth->setAlignment(descriptionLabelFlat , Qt::AlignHCenter);

   //QVBoxLayout *squareBoth = new QVBoxLayout;
   //squareBoth->addWidget(squareButton);
   //squareBoth->addWidget(descriptionLabelSquare);
   //squareBoth->setAlignment(descriptionLabelSquare , Qt::AlignHCenter);

   //QHBoxLayout *allCapLayout = new QHBoxLayout;
   //allCapLayout->addLayout(roundBoth);
   //allCapLayout->addLayout(flatBoth);
   //allCapLayout->addLayout(squareBoth);
   //allCapLayout->addSpacing(240);

   //// define layout
   //capAndTextLayout = new QVBoxLayout;
   //capAndTextLayout->addWidget(capLabel);
   //capAndTextLayout->addLayout(allCapLayout);
   //capAndTextLayout->addSpacing(15);
}

void CPenSelector::arrangeJoinButtons()
{
   // create top label
   //QLabel *joinLabel = new QLabel(tr("<b>Join</b>"), this);

   // create buttons
   bevelJoinButton = new QPushButton(this);
   miterJoinButton = new QPushButton(this);
   roundJoinButton = new QPushButton(this);

   // set tooltip
   bevelJoinButton->setToolTip(tr("Bevel Join : The triangular notch between the two lines is filled."));
   miterJoinButton->setToolTip(tr("Miter Join : The outer edges of the lines are extended to meet at an angle, and this area is filled."));
   roundJoinButton->setToolTip(tr("Round Join : A circular arc between the two lines is filled."));

   // set button sizes
   bevelJoinButton->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   miterJoinButton->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   roundJoinButton->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   bevelJoinButton->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
   miterJoinButton->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
   roundJoinButton->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));

   // set icon paths
   bevelJoinButton->setIcon(QIcon(":/images/bevelJoin.png"));
   miterJoinButton->setIcon(QIcon(":/images/miterJoin.png"));
   roundJoinButton->setIcon(QIcon(":/images/roundJoin.png"));

   // set buttons to checkable, check round cap as default
   bevelJoinButton ->setCheckable(true);
   miterJoinButton ->setCheckable(true);
   roundJoinButton ->setCheckable(true);
   miterJoinButton ->setChecked(true);

   joinButtons = new QButtonGroup(this);
   joinButtons->addButton(bevelJoinButton , static_cast<int>(Qt::BevelJoin ));
   joinButtons->addButton(miterJoinButton , static_cast<int>(Qt::MiterJoin ));
   joinButtons->addButton(roundJoinButton , static_cast<int>(Qt::RoundJoin ));

   joinButtons->setExclusive(true);
   connect(joinButtons, SIGNAL(buttonClicked(int)), this, SLOT(someJoinButtonClicked(int)));

   // make cap button layout
   if(0 == allButtonsLayout)
   {
      return;
   }

   QHBoxLayout *joinLayout = new QHBoxLayout;
   joinLayout->addWidget(bevelJoinButton);
   joinLayout->addWidget(miterJoinButton);
   joinLayout->addWidget(roundJoinButton);

   QGroupBox * joinBox = new QGroupBox(this);
   joinBox->setTitle(tr("Join"));
   joinBox->setLayout(joinLayout);

   allButtonsLayout->addWidget(joinBox, 4, 0, 1, 1);

   //allButtonsLayout->addWidget(joinLabel, 4, 0);
   //allButtonsLayout->addLayout(createButtonLayout(bevelJoinButton , QString(tr("Bevel" ))), 5, 0);
   //allButtonsLayout->addLayout(createButtonLayout(miterJoinButton , QString(tr("Miter" ))), 5, 1);
   //allButtonsLayout->addLayout(createButtonLayout(roundJoinButton , QString(tr("Join"  ))), 5, 2);

   //// make connections
   //connect(bevelJoinButton , SIGNAL(clicked(bool)), this, SLOT(bevelJoinClicked (bool)));
   //connect(miterJoinButton , SIGNAL(clicked(bool)), this, SLOT(miterJoinClicked (bool)));
   //connect(roundJoinButton , SIGNAL(clicked(bool)), this, SLOT(roundJoinClicked (bool)));

   //// make button description labels
   //QLabel *descriptionLabelBevel     = new QLabel(tr("Bevel"), this);
   //QLabel *descriptionLabelMiter     = new QLabel(tr("Miter"), this);
   //QLabel *descriptionLabelRoundJoin = new QLabel(tr("Round"), this);

   //// description labels layout
   //QHBoxLayout *joinDescriptionLayout = new QHBoxLayout;
   //joinDescriptionLayout->addSpacing(40);
   //joinDescriptionLayout->addWidget(descriptionLabelBevel);
   //joinDescriptionLayout->addSpacing(12);
   //joinDescriptionLayout->addWidget(descriptionLabelMiter);
   //joinDescriptionLayout->addSpacing(7);
   //joinDescriptionLayout->addWidget(descriptionLabelRoundJoin);
   //joinDescriptionLayout->addSpacing(210);

   //// define layout
   //QHBoxLayout *joinLayout = new QHBoxLayout;
   //joinLayout->addWidget(miterJoinButton);
   //joinLayout->addWidget(bevelJoinButton);
   //joinLayout->addWidget(roundJoinButton);
   //joinLayout->addSpacing(235);
   //joinAndTextLayout = new QVBoxLayout;
   //joinAndTextLayout->addWidget(joinLabel);
   //joinAndTextLayout->addLayout(joinLayout);
   //joinAndTextLayout->addLayout(joinDescriptionLayout);
   //joinAndTextLayout->addSpacing(20);
}

void CPenSelector::someStyleButtonClicked(int id)
{
   Qt::PenStyle tmpStyle = static_cast<Qt::PenStyle>(id);
   CPenSettings pns = CDrawSettings_2D::getInstance()->penSettings();
   pns.setStyle(tmpStyle);
   CDrawSettings_2D::getInstance()->setPenSettings(pns);
}

void CPenSelector::someCapButtonClicked(int id)
{
   Qt::PenCapStyle tmpStyle = static_cast<Qt::PenCapStyle>(id);
   CPenSettings pns = CDrawSettings_2D::getInstance()->penSettings();
   pns.setCapStyle(tmpStyle);
   CDrawSettings_2D::getInstance()->setPenSettings(pns);
}

void CPenSelector::someJoinButtonClicked(int id)
{
   Qt::PenJoinStyle tmpStyle = static_cast<Qt::PenJoinStyle>(id);

   CPenSettings pns = CDrawSettings_2D::getInstance()->penSettings();
   pns.setJoinStyle(tmpStyle);
   CDrawSettings_2D::getInstance()->setPenSettings(pns);
}