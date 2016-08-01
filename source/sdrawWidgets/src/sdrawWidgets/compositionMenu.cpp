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
//  \file      compositionMenu.cpp
//
//  \brief     Implementation of a class to selection which composition mode to use
//
//  \author    Stian Broen
//
//  \date      07.10.2011
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
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QDir>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QDial>
#include <QComboBox>

// local includes
#include "compositionMenu.h"

using namespace sdraw;

CCompositionMenu::CCompositionMenu(QWidget *parent)
   : QWidget(parent)
{
   currAlpha = 127;
   arrangeButtons();
}

CCompositionMenu::~CCompositionMenu()
{

}

void CCompositionMenu::closeMenu(bool)
{
   emit doCloseCompositionMenuSig();
}

void CCompositionMenu::arrangeButtons()
{
   closeButton = new QToolButton(this);
   closeButton->setToolTip(tr("Close Menu"));
   closeButton->setIcon(QIcon(":/images/blackCancel.png"));
   connect(closeButton, SIGNAL(clicked(bool)), this, SLOT(closeMenu(bool)));
   QHBoxLayout *closeLayout = new QHBoxLayout;
   closeLayout->addWidget(closeButton);
   closeLayout->setAlignment(closeButton, Qt::AlignRight);

   alphaDial = new QDial(this);
   alphaDial->setRange(0,255);
   currAlpha = 255;
   alphaDial->setValue(currAlpha);
   alphaDial->setToolTip(tr("Composition Alpha"));
   connect(alphaDial  , SIGNAL(valueChanged(int)), 
           this       , SLOT  (alphaChanged(int)));

   sourceButton          = new QPushButton(this);
   destinationButton     = new QPushButton(this);
   sourceOverButton      = new QPushButton(this);
   destinationOverButton = new QPushButton(this);
   sourceInButton        = new QPushButton(this);
   destinationInButton   = new QPushButton(this);
   sourceOutButton       = new QPushButton(this);
   destinationOutButton  = new QPushButton(this);
   sourceAtopButton      = new QPushButton(this);
   destinationAtopButton = new QPushButton(this);
   clearButton           = new QPushButton(this);
   xorButton             = new QPushButton(this);

   sourceButton         ->setToolTip(tr("The output is the source pixel. (This means a basic copy operation \nand is identical to Source Over when the source pixel is opaque)."));
   destinationButton    ->setToolTip(tr("The output is the destination pixel. This means that the blending \nhas no effect. This mode is the inverse of Source."));
   sourceOverButton     ->setToolTip(tr("The alpha of the source is used to blend \nthe pixel on top of the destination."));
   destinationOverButton->setToolTip(tr("The alpha of the destination is used to blend it on top of the source \npixels. This mode is the inverse of Source Over."));
   sourceInButton       ->setToolTip(tr("The output is the source, where the alpha is reduced by that of the destination."));
   destinationInButton  ->setToolTip(tr("The output is the destination, where the alpha is reduced by that of \nthe source. This mode is the inverse of Source In."));
   sourceOutButton      ->setToolTip(tr("The output is the source, where the alpha is reduced by the inverse of destination."));
   destinationOutButton ->setToolTip(tr("The output is the destination, where the alpha is reduced by the \ninverse of the source. This mode is the inverse of Source Out."));
   sourceAtopButton     ->setToolTip(tr("The source pixel is blended on top of the destination, with the \nalpha of the source pixel reduced by the alpha of the destination pixel."));
   destinationAtopButton->setToolTip(tr("The destination pixel is blended on top of the source, with the alpha \nof the destination pixel is reduced by the alpha of the destination pixel. This mode is the inverse Source Atop."));
   clearButton          ->setToolTip(tr("The pixels in the destination are cleared (set to fully transparent) independent of the source."));
   xorButton            ->setToolTip(tr("The source, whose alpha is reduced with the inverse of the destination alpha, \nis merged with the destination, whose alpha is reduced by the inverse of the source alpha. \nXOR is not the same as the bitwise Xor."));

   sourceButton         ->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   destinationButton    ->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   sourceOverButton     ->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   destinationOverButton->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   sourceInButton       ->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   destinationInButton  ->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   sourceOutButton      ->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   destinationOutButton ->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   sourceAtopButton     ->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   destinationAtopButton->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   clearButton          ->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));
   xorButton            ->setIconSize(QSize(default_PEN_BUTTON_WIDTH, default_PEN_BUTTON_HEIGHT));

   sourceButton         ->setIcon(QIcon(":/images/source.png"));
   destinationButton    ->setIcon(QIcon(":/images/destination.png"));
   sourceOverButton     ->setIcon(QIcon(":/images/sourceOver.png"));
   destinationOverButton->setIcon(QIcon(":/images/destinationOver.png"));
   sourceInButton       ->setIcon(QIcon(":/images/sourceIn.png"));
   destinationInButton  ->setIcon(QIcon(":/images/destinationIn.png"));
   sourceOutButton      ->setIcon(QIcon(":/images/sourceOut.png"));
   destinationOutButton ->setIcon(QIcon(":/images/destinationOut.png"));
   sourceAtopButton     ->setIcon(QIcon(":/images/sourceAtop.png"));
   destinationAtopButton->setIcon(QIcon(":/images/destinationAtop.png"));
   clearButton          ->setIcon(QIcon(":/images/clear.png"));
   xorButton            ->setIcon(QIcon(":/images/xor.png"));

   sourceButton         ->setCheckable(true);
   destinationButton    ->setCheckable(true);
   sourceOverButton     ->setCheckable(true);
   destinationOverButton->setCheckable(true);
   sourceInButton       ->setCheckable(true);
   destinationInButton  ->setCheckable(true);
   sourceOutButton      ->setCheckable(true);
   destinationOutButton ->setCheckable(true);
   sourceAtopButton     ->setCheckable(true);
   destinationAtopButton->setCheckable(true);
   clearButton          ->setCheckable(true);
   xorButton            ->setCheckable(true);

   sourceOverButton->setChecked(true);

   connect(sourceButton          , SIGNAL(clicked(bool)), this, SLOT(sourceButtonClicked          (bool)));
   connect(destinationButton     , SIGNAL(clicked(bool)), this, SLOT(destinationButtonClicked     (bool)));
   connect(sourceOverButton      , SIGNAL(clicked(bool)), this, SLOT(sourceOverButtonClicked      (bool)));
   connect(destinationOverButton , SIGNAL(clicked(bool)), this, SLOT(destinationOverButtonClicked (bool)));
   connect(sourceInButton        , SIGNAL(clicked(bool)), this, SLOT(sourceInButtonClicked        (bool)));
   connect(destinationInButton   , SIGNAL(clicked(bool)), this, SLOT(destinationInButtonClicked   (bool)));
   connect(sourceOutButton       , SIGNAL(clicked(bool)), this, SLOT(sourceOutButtonClicked       (bool)));
   connect(destinationOutButton  , SIGNAL(clicked(bool)), this, SLOT(destinationOutButtonClicked  (bool)));
   connect(sourceAtopButton      , SIGNAL(clicked(bool)), this, SLOT(sourceAtopButtonClicked      (bool)));
   connect(destinationAtopButton , SIGNAL(clicked(bool)), this, SLOT(destinationAtopButtonClicked (bool)));
   connect(clearButton           , SIGNAL(clicked(bool)), this, SLOT(clearButtonClicked           (bool)));
   connect(xorButton             , SIGNAL(clicked(bool)), this, SLOT(xorButtonClicked             (bool)));

   QLabel *sourceText          = new QLabel(tr("Source"          ), this);
   QLabel *destionationText    = new QLabel(tr("Destionation"    ), this);
   QLabel *sourceOverText      = new QLabel(tr("Source Over"     ), this);
   QLabel *destinationOverText = new QLabel(tr("Destination Over"), this);

   QLabel *sourceInText        = new QLabel(tr("Source In"       ), this);
   QLabel *destinationInText   = new QLabel(tr("Destination In"  ), this);
   QLabel *sourceOutText       = new QLabel(tr("Source Out"      ), this);
   QLabel *destinationOutText  = new QLabel(tr("Destination Out" ), this);

   QLabel *sourceAtopText      = new QLabel(tr("Source Atop"     ), this);
   QLabel *destinationAtopText = new QLabel(tr("Destination Atop"), this);
   QLabel *clearText           = new QLabel(tr("Clear"           ), this);
   QLabel *xorText             = new QLabel(tr("XOR"             ), this);

   additionalModes = new QComboBox(this);
   additionalModes->addItem(tr("Additional Compositon Modes"), 0);
   additionalModes->addItem(tr("Plus")      , QPainter::CompositionMode_Plus        );
   additionalModes->addItem(tr("Multiply")  , QPainter::CompositionMode_Multiply    );
   additionalModes->addItem(tr("Screen")    , QPainter::CompositionMode_Screen      );
   additionalModes->addItem(tr("Overlay")   , QPainter::CompositionMode_Overlay     );
   additionalModes->addItem(tr("Darken")    , QPainter::CompositionMode_Darken      );
   additionalModes->addItem(tr("Lighten")   , QPainter::CompositionMode_Lighten     );
   additionalModes->addItem(tr("ColorDodge"), QPainter::CompositionMode_ColorDodge  );
   additionalModes->addItem(tr("ColorBurn") , QPainter::CompositionMode_ColorBurn   );
   additionalModes->addItem(tr("HardLight") , QPainter::CompositionMode_HardLight   );
   additionalModes->addItem(tr("SoftLight") , QPainter::CompositionMode_SoftLight   );
   additionalModes->addItem(tr("Difference"), QPainter::CompositionMode_Difference  );
   additionalModes->addItem(tr("Exclusion") , QPainter::CompositionMode_Exclusion    );
   additionalModes->setCurrentIndex(0);
   connect(additionalModes, SIGNAL(activated(int)), this, SLOT(comboboxActivated(int)));

   QHBoxLayout *firstTextLayout = new QHBoxLayout;
   firstTextLayout->addWidget(sourceText);
   firstTextLayout->addWidget(destionationText);
   firstTextLayout->addWidget(sourceOverText);
   firstTextLayout->addWidget(destinationOverText);
   firstTextLayout->setAlignment(sourceText         , Qt::AlignHCenter);
   firstTextLayout->setAlignment(destionationText   , Qt::AlignHCenter);
   firstTextLayout->setAlignment(sourceOverText     , Qt::AlignHCenter);
   firstTextLayout->setAlignment(destinationOverText, Qt::AlignHCenter);

   QHBoxLayout *secondTextLayout = new QHBoxLayout;
   secondTextLayout->addWidget(sourceInText);
   secondTextLayout->addWidget(destinationInText);
   secondTextLayout->addWidget(sourceOutText);
   secondTextLayout->addWidget(destinationOutText);
   secondTextLayout->setAlignment(sourceInText      , Qt::AlignHCenter);
   secondTextLayout->setAlignment(destinationInText , Qt::AlignHCenter);
   secondTextLayout->setAlignment(sourceOutText     , Qt::AlignHCenter);
   secondTextLayout->setAlignment(destinationOutText, Qt::AlignHCenter);

   QHBoxLayout *thirdTextLayout = new QHBoxLayout;
   thirdTextLayout->addWidget(sourceAtopText);
   thirdTextLayout->addWidget(destinationAtopText);
   thirdTextLayout->addWidget(clearText);
   thirdTextLayout->addWidget(xorText);
   thirdTextLayout->setAlignment(sourceAtopText     , Qt::AlignHCenter);
   thirdTextLayout->setAlignment(destinationAtopText, Qt::AlignHCenter);
   thirdTextLayout->setAlignment(clearText          , Qt::AlignHCenter);
   thirdTextLayout->setAlignment(xorText            , Qt::AlignHCenter);

   QHBoxLayout *firstLayout = new QHBoxLayout;
   firstLayout->addWidget(sourceButton);
   firstLayout->addWidget(destinationButton);
   firstLayout->addWidget(sourceOverButton);
   firstLayout->addWidget(destinationOverButton);

   QHBoxLayout *secondLayout = new QHBoxLayout;
   secondLayout->addWidget(sourceInButton);
   secondLayout->addWidget(destinationInButton);
   secondLayout->addWidget(sourceOutButton);
   secondLayout->addWidget(destinationOutButton);

   QHBoxLayout *thirdLayout = new QHBoxLayout;
   thirdLayout->addWidget(sourceAtopButton);
   thirdLayout->addWidget(destinationAtopButton);
   thirdLayout->addWidget(clearButton);
   thirdLayout->addWidget(xorButton);

   // dial layout
   QHBoxLayout *dialLayout = new QHBoxLayout;
   dialLayout->addWidget(alphaDial);
   dialLayout->setAlignment(alphaDial, Qt::AlignHCenter);
   QString alphaText = QString(tr("Alpha : %1")).arg(currAlpha);
   dialText = new QLabel(alphaText, this);
   QHBoxLayout *dialTextLayout = new QHBoxLayout;
   dialTextLayout->addWidget(dialText);
   dialTextLayout->setAlignment(dialText, Qt::AlignHCenter);

   // combobox layout
   QHBoxLayout *comboLayout = new QHBoxLayout;
   comboLayout->addWidget(additionalModes);
   dialLayout->setAlignment(additionalModes, Qt::AlignHCenter);

   // arrange layouts
   QVBoxLayout *totalLayout = new QVBoxLayout;
   totalLayout->addLayout(closeLayout);
   totalLayout->addLayout(dialLayout);
   totalLayout->addLayout(dialTextLayout);
   totalLayout->addSpacing(20);
   totalLayout->addLayout(firstLayout);
   totalLayout->addLayout(firstTextLayout);
   totalLayout->addLayout(secondLayout);
   totalLayout->addLayout(secondTextLayout);
   totalLayout->addLayout(thirdLayout);
   totalLayout->addLayout(thirdTextLayout);
   totalLayout->addSpacing(20);
   totalLayout->addLayout(comboLayout);
   
   setLayout(totalLayout);

   currMode = QPainter::CompositionMode_SourceOver;
}

void CCompositionMenu::comboboxActivated(int index)
{
   if(index == 0)
   {
      additionalModes->setCurrentIndex(lastAddModeIndex);
      return;
   }

   lastAddModeIndex = index;
   sourceButton          ->setChecked(false);
   destinationButton     ->setChecked(false);
   sourceOverButton      ->setChecked(false);
   destinationOverButton ->setChecked(false);
   sourceInButton        ->setChecked(false);
   destinationInButton   ->setChecked(false);
   sourceOutButton       ->setChecked(false);
   destinationOutButton  ->setChecked(false);
   sourceAtopButton      ->setChecked(false);
   destinationAtopButton ->setChecked(false);
   clearButton           ->setChecked(false);
   xorButton             ->setChecked(false);

   currMode = (QPainter::CompositionMode)additionalModes->itemData(index).toInt(); 
   emit compModeSig(currMode);
}

void CCompositionMenu::sourceButtonClicked(bool)
{
   additionalModes->setCurrentIndex(0);
   sourceButton          ->setChecked(true);
   destinationButton     ->setChecked(false);
   sourceOverButton      ->setChecked(false);
   destinationOverButton ->setChecked(false);
   sourceInButton        ->setChecked(false);
   destinationInButton   ->setChecked(false);
   sourceOutButton       ->setChecked(false);
   destinationOutButton  ->setChecked(false);
   sourceAtopButton      ->setChecked(false);
   destinationAtopButton ->setChecked(false);
   clearButton           ->setChecked(false);
   xorButton             ->setChecked(false);

   currMode = QPainter::CompositionMode_Source;
   emit compModeSig(currMode);
}

void CCompositionMenu::destinationButtonClicked(bool)
{
   additionalModes->setCurrentIndex(0);
   sourceButton          ->setChecked(false);
   destinationButton     ->setChecked(true);
   sourceOverButton      ->setChecked(false);
   destinationOverButton ->setChecked(false);
   sourceInButton        ->setChecked(false);
   destinationInButton   ->setChecked(false);
   sourceOutButton       ->setChecked(false);
   destinationOutButton  ->setChecked(false);
   sourceAtopButton      ->setChecked(false);
   destinationAtopButton ->setChecked(false);
   clearButton           ->setChecked(false);
   xorButton             ->setChecked(false);

   currMode = QPainter::CompositionMode_Destination;
   emit compModeSig(currMode);
}

void CCompositionMenu::sourceOverButtonClicked(bool)
{
   additionalModes->setCurrentIndex(0);
   sourceButton          ->setChecked(false);
   destinationButton     ->setChecked(false);
   sourceOverButton      ->setChecked(true);
   destinationOverButton ->setChecked(false);
   sourceInButton        ->setChecked(false);
   destinationInButton   ->setChecked(false);
   sourceOutButton       ->setChecked(false);
   destinationOutButton  ->setChecked(false);
   sourceAtopButton      ->setChecked(false);
   destinationAtopButton ->setChecked(false);
   clearButton           ->setChecked(false);
   xorButton             ->setChecked(false);

   currMode = QPainter::CompositionMode_SourceOver;
   emit compModeSig(currMode);
}

void CCompositionMenu::destinationOverButtonClicked(bool)
{
   additionalModes->setCurrentIndex(0);
   sourceButton          ->setChecked(false);
   destinationButton     ->setChecked(false);
   sourceOverButton      ->setChecked(false);
   destinationOverButton ->setChecked(true);
   sourceInButton        ->setChecked(false);
   destinationInButton   ->setChecked(false);
   sourceOutButton       ->setChecked(false);
   destinationOutButton  ->setChecked(false);
   sourceAtopButton      ->setChecked(false);
   destinationAtopButton ->setChecked(false);
   clearButton           ->setChecked(false);
   xorButton             ->setChecked(false);

   currMode = QPainter::CompositionMode_DestinationOver;
   emit compModeSig(currMode);
}

void CCompositionMenu::sourceInButtonClicked(bool)
{
   additionalModes->setCurrentIndex(0);
   sourceButton          ->setChecked(false);
   destinationButton     ->setChecked(false);
   sourceOverButton      ->setChecked(false);
   destinationOverButton ->setChecked(false);
   sourceInButton        ->setChecked(true);
   destinationInButton   ->setChecked(false);
   sourceOutButton       ->setChecked(false);
   destinationOutButton  ->setChecked(false);
   sourceAtopButton      ->setChecked(false);
   destinationAtopButton ->setChecked(false);
   clearButton           ->setChecked(false);
   xorButton             ->setChecked(false);

   currMode = QPainter::CompositionMode_SourceIn;
   emit compModeSig(currMode);
}

void CCompositionMenu::destinationInButtonClicked(bool)
{
   additionalModes->setCurrentIndex(0);
   sourceButton          ->setChecked(false);
   destinationButton     ->setChecked(false);
   sourceOverButton      ->setChecked(false);
   destinationOverButton ->setChecked(false);
   sourceInButton        ->setChecked(false);
   destinationInButton   ->setChecked(true);
   sourceOutButton       ->setChecked(false);
   destinationOutButton  ->setChecked(false);
   sourceAtopButton      ->setChecked(false);
   destinationAtopButton ->setChecked(false);
   clearButton           ->setChecked(false);
   xorButton             ->setChecked(false);

   currMode = QPainter::CompositionMode_DestinationIn;
   emit compModeSig(currMode);
}

void CCompositionMenu::sourceOutButtonClicked(bool)
{
   additionalModes->setCurrentIndex(0);
   sourceButton          ->setChecked(false);
   destinationButton     ->setChecked(false);
   sourceOverButton      ->setChecked(false);
   destinationOverButton ->setChecked(false);
   sourceInButton        ->setChecked(false);
   destinationInButton   ->setChecked(false);
   sourceOutButton       ->setChecked(true);
   destinationOutButton  ->setChecked(false);
   sourceAtopButton      ->setChecked(false);
   destinationAtopButton ->setChecked(false);
   clearButton           ->setChecked(false);
   xorButton             ->setChecked(false);

   currMode = QPainter::CompositionMode_SourceOut;
   emit compModeSig(currMode);
}

void CCompositionMenu::destinationOutButtonClicked(bool)
{
   additionalModes->setCurrentIndex(0);
   sourceButton          ->setChecked(false);
   destinationButton     ->setChecked(false);
   sourceOverButton      ->setChecked(false);
   destinationOverButton ->setChecked(false);
   sourceInButton        ->setChecked(false);
   destinationInButton   ->setChecked(false);
   sourceOutButton       ->setChecked(false);
   destinationOutButton  ->setChecked(true);
   sourceAtopButton      ->setChecked(false);
   destinationAtopButton ->setChecked(false);
   clearButton           ->setChecked(false);
   xorButton             ->setChecked(false);

   currMode = QPainter::CompositionMode_DestinationOut;
   emit compModeSig(currMode);
}

void CCompositionMenu::sourceAtopButtonClicked(bool)
{
   additionalModes->setCurrentIndex(0);
   sourceButton          ->setChecked(false);
   destinationButton     ->setChecked(false);
   sourceOverButton      ->setChecked(false);
   destinationOverButton ->setChecked(false);
   sourceInButton        ->setChecked(false);
   destinationInButton   ->setChecked(false);
   sourceOutButton       ->setChecked(false);
   destinationOutButton  ->setChecked(false);
   sourceAtopButton      ->setChecked(true);
   destinationAtopButton ->setChecked(false);
   clearButton           ->setChecked(false);
   xorButton             ->setChecked(false);

   currMode = QPainter::CompositionMode_SourceAtop;
   emit compModeSig(currMode);
}

void CCompositionMenu::destinationAtopButtonClicked(bool)
{
   additionalModes->setCurrentIndex(0);
   sourceButton          ->setChecked(false);
   destinationButton     ->setChecked(false);
   sourceOverButton      ->setChecked(false);
   destinationOverButton ->setChecked(false);
   sourceInButton        ->setChecked(false);
   destinationInButton   ->setChecked(false);
   sourceOutButton       ->setChecked(false);
   destinationOutButton  ->setChecked(false);
   sourceAtopButton      ->setChecked(false);
   destinationAtopButton ->setChecked(true);
   clearButton           ->setChecked(false);
   xorButton             ->setChecked(false);

   currMode = QPainter::CompositionMode_DestinationAtop;
   emit compModeSig(currMode);
}

void CCompositionMenu::clearButtonClicked(bool)
{
   additionalModes->setCurrentIndex(0);
   sourceButton          ->setChecked(false);
   destinationButton     ->setChecked(false);
   sourceOverButton      ->setChecked(false);
   destinationOverButton ->setChecked(false);
   sourceInButton        ->setChecked(false);
   destinationInButton   ->setChecked(false);
   sourceOutButton       ->setChecked(false);
   destinationOutButton  ->setChecked(false);
   sourceAtopButton      ->setChecked(false);
   destinationAtopButton ->setChecked(false);
   clearButton           ->setChecked(true);
   xorButton             ->setChecked(false);

   currMode = QPainter::CompositionMode_Clear;
   emit compModeSig(currMode);
}

void CCompositionMenu::xorButtonClicked(bool)
{
   additionalModes->setCurrentIndex(0);
   sourceButton          ->setChecked(false);
   destinationButton     ->setChecked(false);
   sourceOverButton      ->setChecked(false);
   destinationOverButton ->setChecked(false);
   sourceInButton        ->setChecked(false);
   destinationInButton   ->setChecked(false);
   sourceOutButton       ->setChecked(false);
   destinationOutButton  ->setChecked(false);
   sourceAtopButton      ->setChecked(false);
   destinationAtopButton ->setChecked(false);
   clearButton           ->setChecked(false);
   xorButton             ->setChecked(true);

   currMode = QPainter::CompositionMode_Xor;
   emit compModeSig(currMode);
}

void CCompositionMenu::currModeRequested()
{
   emit compModeSig(currMode);
   emit changeAlphaFromCompositionMenuSig(currAlpha);
}

void CCompositionMenu::alphaChanged(int value)
{
   currAlpha = value;
   QString alphaText = QString(tr("Alpha : %1")).arg(currAlpha);
   dialText->setText(alphaText);
   emit changeAlphaFromCompositionMenuSig(currAlpha);
}