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
//  \file      settingsMenu.cpp
//
//  \brief     Implementation of a widget which contains draw settings
//
//  \author    Stian Broen
//
//  \date      18.09.2011
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
#include <QRadioButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

// local includes
#include "settingsMenu.h"

using namespace sdraw;

CSettingsMenu::CSettingsMenu(QWidget *parent)
   : QWidget(parent)
{
   if(parent)
      setParent(parent);

   QLabel *antialiasingLabel = new QLabel("Antialiasing");
   antialiasingLabel->setGeometry(antialiasingLabel->x(), antialiasingLabel->y(), 
                                  default_LABEL_LENGTH, default_LABEL_HEIGHT);
   antialiasingON  = new QRadioButton(this);
   antialiasingON->setText("On");
   connect(antialiasingON, SIGNAL(clicked(bool)), this, SLOT(antialiasingOnClicked(bool)));
   antialiasingON->setChecked(true);

   antialiasingOFF = new QRadioButton(this);
   antialiasingOFF->setText("Off");
   connect(antialiasingOFF, SIGNAL(clicked(bool)), this, SLOT(antialiasingOffClicked(bool)));
 
   if(parent)
   {
      connect(parent, SIGNAL(requestSettings()), this , SLOT(settingsRequest()));
   }

   QHBoxLayout *antialiasingLayout = new QHBoxLayout;
   antialiasingLayout->setSizeConstraint(QLayout::SetMaximumSize);
   antialiasingLayout->addWidget(antialiasingLabel);
   antialiasingLayout->addWidget(antialiasingON);
   antialiasingLayout->addWidget(antialiasingOFF);

   QVBoxLayout *totalLayout = new QVBoxLayout;
   totalLayout->addLayout(antialiasingLayout);
   setLayout(totalLayout);

   settings.setAntialiasing(true);
}

CSettingsMenu::~CSettingsMenu()
{

}

void CSettingsMenu::antialiasingOnClicked(bool checked)
{
   if(checked)
   {
      settings.setAntialiasing(true);
      emit settingsSig(settings);
   }
}

void CSettingsMenu::antialiasingOffClicked(bool checked)
{
   if(checked)
   {
      settings.setAntialiasing(false);
      emit settingsSig(settings);
   }
}

void CSettingsMenu::settingsRequest()
{
   emit settingsSig(settings);
}