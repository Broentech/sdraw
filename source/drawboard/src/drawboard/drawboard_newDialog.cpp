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
//  \file      drawboard_newDialog.cpp
//
//  \brief     Implementation of a dialog window to define a new drawboard
//
//  \author    Stian Broen
//
//  \date      18.03.2012
//
//  \par       Copyright:
//
//             Copyright by Stian Broen
//
//  \par       Revision History
//
//  \par       01 18.03.2012 Original version       
//
//
//h-//////////////////////////////////////////////////////////////////////////

// qt includes
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPainter>

// local includes
#include "drawboard_newDialog.h"

using namespace sdraw;

CNewDrawboardDialog::CNewDrawboardDialog(QWidget *parent)
   :QDialog(parent)
{
   setAttribute(Qt::WA_DeleteOnClose);
   setWindowTitle(tr("New Image"));
   imgName = "Untitled";
   layoutGUI();
}

void CNewDrawboardDialog::setImgName(const QString &name)
{
   imgName = name;
   nameEdit->setText(imgName);
}

void CNewDrawboardDialog::paintEvent(QPaintEvent * event)
{
   QRect thisRect = this->rect();
   QLinearGradient linearGrad(thisRect.topLeft(), thisRect.bottomRight());
   linearGrad.setColorAt(0, Qt::white);
   linearGrad.setColorAt(1, Qt::lightGray);
   QBrush backgroundBrush(linearGrad);
   QPainter painter(this);
   painter.fillRect(this->rect(), backgroundBrush);
   painter.end();
   QDialog::paintEvent(event);
}

void CNewDrawboardDialog::layoutGUI()
{
   nameLabel = new QLabel(tr("Name :"), this);
   widthLabel = new QLabel(tr("Width :"), this);
   heightLabel = new QLabel(tr("Height :"), this);

   QVBoxLayout *labelLayout = new QVBoxLayout;
   labelLayout->addWidget(nameLabel);
   labelLayout->addWidget(widthLabel);
   labelLayout->addWidget(heightLabel);

   nameEdit = new QLineEdit(this);
   nameEdit->setText(imgName);

   widhtSpinBox = new QSpinBox(this);
   widhtSpinBox->setMinimum(1);
   widhtSpinBox->setMaximum(4094);
   widhtSpinBox->setValue(500);

   heightSpinBox = new QSpinBox(this);
   heightSpinBox->setMinimum(1);
   heightSpinBox->setMaximum(4094);
   heightSpinBox->setValue(500);

   QVBoxLayout *inputLayout = new QVBoxLayout;
   inputLayout->addWidget(nameEdit);
   inputLayout->addWidget(widhtSpinBox);
   inputLayout->addWidget(heightSpinBox);

   QHBoxLayout *totalInputLayout = new QHBoxLayout;
   totalInputLayout->addLayout(labelLayout);
   totalInputLayout->addLayout(inputLayout);

   OKButton = new QPushButton(tr("OK"), this);
   cancelButton = new QPushButton(tr("Cancel"), this);

   connect(OKButton, SIGNAL(clicked()),
      this, SLOT(OKButtonClicked()));

   connect(cancelButton, SIGNAL(clicked()),
      this, SLOT(cancelButtonClicked()));

   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget(OKButton);
   buttonLayout->addWidget(cancelButton);

   QVBoxLayout *totalLayout = new QVBoxLayout;
   totalLayout->addLayout(totalInputLayout);
   totalLayout->addLayout(buttonLayout);

   this->setLayout(totalLayout);

}

void CNewDrawboardDialog::OKButtonClicked()
{
   emit newImageSig(nameEdit->text(), widhtSpinBox->value(), heightSpinBox->value());
   close();
}

void CNewDrawboardDialog::cancelButtonClicked()
{
   close();
}