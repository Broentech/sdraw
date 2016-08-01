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
//  \file      drawboard_newDialog.h
//
//  \brief     Definition of a dialog window to define a new drawboard
//
//  \author    Stian Broen
//
//  \date      18.03.2012
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 18.03.2012 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

#ifndef NEW_DRAWBOARD_DIALOG_H
#define NEW_DRAWBOARD_DIALOG_H

// Qt includes
#include <QDialog>

// Qt forward declarations
class QLabel;
class QLineEdit;
class QSpinBox;
class QPushButton;

namespace sdraw {
   class CNewDrawboardDialog : public QDialog
   {
      Q_OBJECT

   public:
      CNewDrawboardDialog(QWidget *parent = 0);
      void setImgName(const QString &name);

   signals:
      void newImageSig(const QString &name, const int width, const int height);

   protected:
      void paintEvent(QPaintEvent * event);

   private slots:
      void OKButtonClicked();
      void cancelButtonClicked();

   private:
      void layoutGUI();

   private:
      QString imgName;
      QLabel *nameLabel;
      QLineEdit *nameEdit;
      QLabel *widthLabel;
      QSpinBox *widhtSpinBox;
      QLabel *heightLabel;
      QSpinBox *heightSpinBox;
      QPushButton *OKButton;
      QPushButton *cancelButton;
   };
}

#endif