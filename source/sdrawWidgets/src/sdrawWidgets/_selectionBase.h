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
//  \file      _selectionBase.h
//
//  \brief     Declaration of a class to serve as a base for custom selection widgets
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
//
//h-//////////////////////////////////////////////////////////////////////////

#ifndef SELECTION_BASE_CLASS
#define SELECTION_BASE_CLASS

// Qt includes
#include <QWidget>

// Qt forward declarations
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QToolButton;

namespace sdraw {
   class SdrawCustomWidget : public QWidget
   {
      Q_OBJECT

   public: 
      SdrawCustomWidget(QWidget *parent = 0);
      ~SdrawCustomWidget();
      QHBoxLayout *setCloseLayout();
      QVBoxLayout *createButtonLayout(QPushButton *button, const QString &buttonName);

   private slots :
      void closeMenu(bool);

   private:
      QToolButton *closeButton;

   signals:
      void closeSignal();
   };
}

#endif