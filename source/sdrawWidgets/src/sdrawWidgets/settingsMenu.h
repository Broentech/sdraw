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
//  \file      settingsMenu.h
//
//  \brief     Declaration of a widget which contains draw settings
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

#ifndef SETTINGSMENU_H
#define SETTINGSMENU_H

#define default_LABEL_LENGTH 50
#define default_LABEL_HEIGHT 20

// Qt includes
#include <QWidget>

// soluion includes
#include <common/definitions.h>

// forward declarations
class QRadioButton;

namespace sdraw {
   class CSettingsMenu : public QWidget
   {
      Q_OBJECT

   public:
      CSettingsMenu(QWidget *parent = 0);
      ~CSettingsMenu();

      public slots:
      void antialiasingOnClicked(bool checked);
      void antialiasingOffClicked(bool checked);
      void settingsRequest();

   signals:
      void settingsSig(CSettings &settings);

   private:
      QRadioButton *antialiasingON;
      QRadioButton *antialiasingOFF;
      CSettings settings;

   };
}

#endif