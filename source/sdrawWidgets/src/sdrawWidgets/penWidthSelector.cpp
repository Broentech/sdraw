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
//  \file      penWidthSelector.cpp
//
//  \brief     Implementation of a class to define pen width
//
//  \author    Stian Broen
//
//  \date      01.05.2012
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
#include <QMouseEvent>

// local includes
#include "penWidthSelector.h"

using namespace sdraw;

CPenWidthSelector::CPenWidthSelector(QWidget *parent)
   :QDial(parent)
{
   this->setRange(1, 30);
   this->setValue(1);
}

CPenWidthSelector::~CPenWidthSelector()
{

}

void CPenWidthSelector::mouseReleaseEvent(QMouseEvent *e)
{
   QDial::mouseReleaseEvent(e);
   if(e == 0)
   {
      return;
   }
   emit updateWidthSig(this->value());
}

void CPenWidthSelector::mouseMoveEvent(QMouseEvent *e)
{
   QDial::mouseMoveEvent(e);
   if(e == 0)
   {
      return;
   }
   emit updatingWidthSig(this->value());
}