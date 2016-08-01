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
/*!
//  \file      _selectionBase.cpp
//
//  \brief     Implementation of a class to serve as a base for custom selection widgets
//
//  \author    Stian Broen
//
//  \date      28.04.2012
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
*/
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>

// local includes
#include "_selectionBase.h"

using namespace sdraw;

SdrawCustomWidget::SdrawCustomWidget(QWidget *parent)
   : QWidget(parent),
   closeButton(0)
{

}

SdrawCustomWidget::~SdrawCustomWidget()
{

}

QHBoxLayout * SdrawCustomWidget::setCloseLayout()
{
   closeButton = new QToolButton(this);
   closeButton->setToolTip(tr("Close Menu"));
   closeButton->setIcon(QIcon(":/images/blackCancel.png"));

   connect(closeButton, SIGNAL(clicked(bool)),
      this, SLOT(closeMenu(bool)));

   QHBoxLayout *closeLayout = new QHBoxLayout;
   closeLayout->addWidget(closeButton);
   closeLayout->setAlignment(closeButton, Qt::AlignRight);
   return closeLayout;
}

void SdrawCustomWidget::closeMenu(bool)
{
   emit closeSignal();
}

QVBoxLayout * SdrawCustomWidget::createButtonLayout(QPushButton *button, const QString &buttonName)
{
   if(button == 0 || buttonName.isNull() || buttonName == QString(""))
   {
      return 0;
   }
   QLabel *descriptionLabel = new QLabel(buttonName, this);
   QHBoxLayout *labelLayout = new QHBoxLayout;
   labelLayout->addStretch();
   labelLayout->addWidget(descriptionLabel);
   labelLayout->addStretch();
   QVBoxLayout *layout = new QVBoxLayout;
   layout->addWidget(button);
   layout->addLayout(labelLayout);
   return layout;
}