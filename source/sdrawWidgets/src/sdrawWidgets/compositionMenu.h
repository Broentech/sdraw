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
//  \file      compositionMenu.h
//
//  \brief     Declaration of a class to selection which composition mode to use
//
//  \author    Stian Broen
//
//  \date      07.10.2011
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

#ifndef COMPOSITIONMENU_H
#define COMPOSITIONMENU_H

// Qt includes
#include <QWidget>
#include <QPainter>

// solution includes
#include <common/definitions.h>

// forward declarations
class QPushButton;
class QToolButton;
class QDial;
class QLabel;
class QComboBox;

namespace sdraw {
   class CCompositionMenu : public QWidget
   {
      Q_OBJECT

   public:

      CCompositionMenu(QWidget *parent = 0);
      ~CCompositionMenu();

      public slots:

      void closeMenu(bool);

      void sourceButtonClicked(bool);
      void destinationButtonClicked(bool);
      void sourceOverButtonClicked(bool);
      void destinationOverButtonClicked(bool);
      void sourceInButtonClicked(bool);
      void destinationInButtonClicked(bool);
      void sourceOutButtonClicked(bool);
      void destinationOutButtonClicked(bool);
      void sourceAtopButtonClicked(bool);
      void destinationAtopButtonClicked(bool);
      void clearButtonClicked(bool);
      void xorButtonClicked(bool);

      void currModeRequested();
      void alphaChanged(int value);
      void comboboxActivated(int index);

   signals:

      void compModeSig(QPainter::CompositionMode mode);
      void doCloseCompositionMenuSig();

      void changeAlphaFromCompositionMenuSig(int value);

   private:

      QPainter::CompositionMode currMode;
      void arrangeButtons();

      QToolButton *closeButton;

      QPushButton *sourceButton;
      QPushButton *destinationButton;
      QPushButton *sourceOverButton;
      QPushButton *destinationOverButton;
      QPushButton *sourceInButton;
      QPushButton *destinationInButton;
      QPushButton *sourceOutButton;
      QPushButton *destinationOutButton;
      QPushButton *sourceAtopButton;
      QPushButton *destinationAtopButton;
      QPushButton *clearButton;
      QPushButton *xorButton;

      QLabel *dialText;
      QDial *alphaDial;
      int currAlpha;

      QComboBox *additionalModes;
      int lastAddModeIndex;

   };
}

#endif