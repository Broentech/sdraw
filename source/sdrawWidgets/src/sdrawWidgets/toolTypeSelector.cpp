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
//  \file      toolTypeSelector.cpp
//
//  \brief     Implementation of a class choose tool type from
//
//  \author    Stian Broen
//
//  \date      20.09.2011
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
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDir>
#include <QFile>

// local includes
#include "toolTypeSelector.h"

using namespace sdraw;

CToolTypeSelector::CToolTypeSelector(QWidget *parent)
   : QWidget(parent)
{
   currOperation = default_OPERATION;
   arrangeButtons();
}

CToolTypeSelector::~CToolTypeSelector()
{

}

void CToolTypeSelector::arrangeButtons()
{
   painterButton      = new QToolButton(this);
   polygonToolsButton = new QToolButton(this);
   fillerButton       = new QToolButton(this);
   selecterButton     = new QToolButton(this);
   textButton         = new QToolButton(this);
   inspecterButton    = new QToolButton(this);

   connect(painterButton     , SIGNAL(clicked(bool)), this, SLOT(painterButtonClicked     (bool)));
   connect(polygonToolsButton, SIGNAL(clicked(bool)), this, SLOT(polygonToolsButtonClicked(bool)));
   connect(fillerButton      , SIGNAL(clicked(bool)), this, SLOT(fillerButtonClicked      (bool)));
   connect(selecterButton    , SIGNAL(clicked(bool)), this, SLOT(selecterButtonClicked    (bool)));
   connect(textButton        , SIGNAL(clicked(bool)), this, SLOT(textButtonClicked        (bool)));
   connect(inspecterButton   , SIGNAL(clicked(bool)), this, SLOT(inspecterButtonClicked   (bool)));

   painterButton     ->setCheckable(true);
   polygonToolsButton->setCheckable(true);
   fillerButton      ->setCheckable(true);
   selecterButton    ->setCheckable(true);
   textButton        ->setCheckable(true);
   inspecterButton   ->setCheckable(true);
   painterButton     ->setChecked(true);

   painterButton     ->setToolTip(tr("Painting"  ));
   polygonToolsButton->setToolTip(tr("Polygon"   ));
   fillerButton      ->setToolTip(tr("Filling"   ));
   selecterButton    ->setToolTip(tr("Selection" ));
   textButton        ->setToolTip(tr("Text"      ));
   inspecterButton   ->setToolTip(tr("Inspection"));

   // give icons to buttons
   painterButton     ->setIcon(QIcon(":/images/ToolTypeSelector/penIcon.png"          ));
   polygonToolsButton->setIcon(QIcon(":/images/ToolTypeSelector/polygonIcon.png"      ));
   fillerButton      ->setIcon(QIcon(":/images/ToolTypeSelector/filledPolygonIcon.png"));
   selecterButton    ->setIcon(QIcon(":/images/ToolTypeSelector/selectionIcon.png"    ));
   textButton        ->setIcon(QIcon(":/images/ToolTypeSelector/textIcon.png"         ));
   inspecterButton   ->setIcon(QIcon(":/images/ToolTypeSelector/inspectionIcon.png"   ));

   QHBoxLayout *firstLayout = new QHBoxLayout;
   firstLayout->addWidget(painterButton);
   firstLayout->addWidget(polygonToolsButton);
   firstLayout->addWidget(fillerButton);

   QHBoxLayout *secondLayout = new QHBoxLayout;
   secondLayout->addWidget(selecterButton);
   secondLayout->addWidget(textButton);
   secondLayout->addWidget(inspecterButton);

   QVBoxLayout *totalLayout = new QVBoxLayout;
   totalLayout->addLayout(firstLayout);
   totalLayout->addLayout(secondLayout);

   QGroupBox *groupBox = new QGroupBox(tr("Operations"), this);
   groupBox->setLayout(totalLayout);
   groupBox->setAlignment(Qt::AlignHCenter);

   QHBoxLayout *gLayout = new QHBoxLayout;
   gLayout->addWidget(groupBox);
   setLayout(gLayout);
}

void CToolTypeSelector::operationRequest()
{
   emit operationSig(currOperation);
}

void CToolTypeSelector::painterButtonClicked(bool)
{
   painterButton     ->setChecked(true);
   polygonToolsButton->setChecked(false);
   fillerButton      ->setChecked(false);
   selecterButton    ->setChecked(false);
   textButton        ->setChecked(false);
   inspecterButton   ->setChecked(false);
   currOperation = DRAW;
   emit operationSig(currOperation);
}

void CToolTypeSelector::polygonToolsButtonClicked(bool)
{
   painterButton     ->setChecked(false);
   polygonToolsButton->setChecked(true);
   fillerButton      ->setChecked(false);
   selecterButton    ->setChecked(false);
   textButton        ->setChecked(false);
   inspecterButton   ->setChecked(false);
   currOperation = POLYGONIZER;
   emit operationSig(currOperation);
}

void CToolTypeSelector::fillerButtonClicked(bool)
{
   painterButton     ->setChecked(false);
   polygonToolsButton->setChecked(false);
   fillerButton      ->setChecked(true);
   selecterButton    ->setChecked(false);
   textButton        ->setChecked(false);
   inspecterButton   ->setChecked(false);
   currOperation = FILLER;
   emit operationSig(currOperation);
}

void CToolTypeSelector::selecterButtonClicked(bool)
{
   painterButton     ->setChecked(false);
   polygonToolsButton->setChecked(false);
   fillerButton      ->setChecked(false);
   selecterButton    ->setChecked(true);
   textButton        ->setChecked(false);
   inspecterButton   ->setChecked(false);
   currOperation = SELECTER;
   emit operationSig(currOperation);
}

void CToolTypeSelector::textButtonClicked(bool)
{
   painterButton     ->setChecked(false);
   polygonToolsButton->setChecked(false);
   fillerButton      ->setChecked(false);
   selecterButton    ->setChecked(false);
   textButton        ->setChecked(true);
   inspecterButton   ->setChecked(false);
   currOperation = TEXT;
   emit operationSig(currOperation);
}

void CToolTypeSelector::inspecterButtonClicked(bool)
{
   painterButton     ->setChecked(false);
   polygonToolsButton->setChecked(false);
   fillerButton      ->setChecked(false);
   selecterButton    ->setChecked(false);
   textButton        ->setChecked(false);
   inspecterButton   ->setChecked(true);
   currOperation = INSPECTER;
   emit operationSig(currOperation);
}