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
//  \file      penWidthSelector.h
//
//  \brief     Declaration of a class to define pen width
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

#ifndef PEN_WIDTH_SELECTOR
#define PEN_WIDTH_SELECTOR

// Qt includes
#include <QDial>

// Qt forward declarations
class QMouseEvent;

namespace sdraw {
   class CPenWidthSelector : public QDial
   {
      Q_OBJECT

   public:
      CPenWidthSelector(QWidget *parent = 0);
      ~CPenWidthSelector();

   private: // functions
      void mouseReleaseEvent(QMouseEvent *e);
      void mouseMoveEvent(QMouseEvent *e);

   signals:
      void updatingWidthSig(int movingVal);
      void updateWidthSig(int val);
   };
}

#endif