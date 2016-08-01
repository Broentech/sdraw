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
//  \file      imageDialog.cpp
//
//  \brief     Dialog used to inspect a QImage or QPixmap buffer
//
//  \author    Stian Broen
//
//  \date      29.07.2016
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 29.07.2016 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QVBoxLayout>

// local includes
#include "imageDialog.h"

using namespace sdraw;

DebugImageDialog::DebugImageDialog(const QPixmap &img, QWidget *parent)
   : QDialog(parent),
   m_quit(0),
   m_imgLabel(0)
{
   m_quit = new QPushButton(this);

   connect(m_quit, SIGNAL(clicked()),
      this, SLOT(accept()));

   m_imgLabel = new QLabel(this);
   m_imgLabel->setPixmap(img);

   QVBoxLayout *lay = new QVBoxLayout;
   lay->addWidget(m_imgLabel);
   lay->addWidget(m_quit);
   this->setLayout(lay);
}

DebugImageDialog::~DebugImageDialog()
{

}
