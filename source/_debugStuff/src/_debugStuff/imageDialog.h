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
//  \file      imageDialog.h
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

#ifndef _IMAGE_DIALOG_G_
#define _IMAGE_DIALOG_G_

// Qt includes
#include <QDialog>
#include <QPixmap>
#include <QPushButton>
#include <QLabel>

namespace sdraw {
   class DebugImageDialog : public QDialog
   {
   public:
      explicit DebugImageDialog(const QPixmap &img = QPixmap(), QWidget *parent = 0);
      ~DebugImageDialog();

   private:
      QPushButton *m_quit;
      QLabel *m_imgLabel;
   };
}

#endif