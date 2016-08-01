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
//  \file      colorWidget.cpp
//
//  \brief     Declaration of a class to display selected Color
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
#include <QColor>
#include <QLabel>
#include <QString>
#include <QPixMap>

// local includes
#include "colorWidget.h"

using namespace sdraw;

CColorWidget::CColorWidget(QWidget *parent)
   : QPushButton(parent)
{
   setFixedSize(43,55);
   setCheckable(true);

   label        = new QLabel(this);
   colorDisplay = new QLabel(this);
   color        = QColor("black");

   label->setGeometry(5,5,37,10);

   QPixmap tmpPixmap = QPixmap(33,28);
   tmpPixmap.fill(color);
   colorDisplay->setPixmap(tmpPixmap);
   colorDisplay->setGeometry(5,18,33,33);

   connect(this, SIGNAL(clicked(bool)), this, SLOT(setSelected(bool)));
}

CColorWidget::~CColorWidget()
{

}

void CColorWidget::setColor(QColor _color)
{
   color = _color;
   QPixmap tmpPixmap = QPixmap(33,28);
   tmpPixmap.fill(color);
   colorDisplay->setPixmap(tmpPixmap);
   emit colorSelected(color, label->text());
}

void CColorWidget::setSelected(bool checked)
{
   if(!checked)
   {
      setChecked(false);
   }
   else
   {
      setChecked(true);
      emit colorSelected(color, label->text());
   }
}

void CColorWidget::setLabel(const QString &txt)
{
   label->setText(txt);
}

QColor CColorWidget::getColor()
{
   return color;
}
