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
//  \file      colorWidget.h
//
//  \brief     Declaration of a class to display selected Color
//
//  \author    Stian Broen
//
//  \date      August 2011
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

#ifndef COLORWIDGET_H
#define COLORWIDGET_H

// Qt includes
#include <QPushButton>

// forward declarations
class QLabel;
class QColor;
class QString;

namespace sdraw {
   class CColorWidget : public QPushButton
   {
      Q_OBJECT

   public:
      CColorWidget(QWidget *parent = 0);
      ~CColorWidget();

      void setColor(QColor color);
      void setLabel(const QString &txt);
      QColor getColor();

      public slots:
      void setSelected(bool checked);

   signals:
      void colorSelected(QColor color, const QString &label);

   private:
      QLabel *label;
      QLabel *colorDisplay;
      QColor color;
   };
}

#endif