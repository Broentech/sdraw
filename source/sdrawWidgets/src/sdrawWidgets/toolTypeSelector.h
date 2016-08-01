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
//  \file      toolTypeSelector.h
//
//  \brief     Declaration of a class choose tool type from
//
//  \author    Stian Broen
//
//  \date      20.09.2011
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

#ifndef TOOLTYPESELECTOR_H
#define TOOLTYPESELECTOR_H

// Qt includes
#include <QWidget>

// solution includes
#include <common/definitions.h>

// forward declarations
class QToolButton;

namespace sdraw {
   class CToolTypeSelector : public QWidget
   {
      Q_OBJECT

   public:
      CToolTypeSelector(QWidget *parent = 0);
      ~CToolTypeSelector();

      public slots:
      void operationRequest();

      void painterButtonClicked(bool checked);
      void polygonToolsButtonClicked(bool checked);
      void fillerButtonClicked(bool checked);
      void selecterButtonClicked(bool checked);
      void textButtonClicked(bool checked);
      void inspecterButtonClicked(bool checked);

   signals:
      void operationSig(TOperation &operation);

   private:

      void arrangeButtons();
      QToolButton *painterButton;
      QToolButton *polygonToolsButton;
      QToolButton *fillerButton;
      QToolButton *selecterButton;
      QToolButton *textButton;
      QToolButton *inspecterButton;

      TOperation currOperation;
   };
}
#endif