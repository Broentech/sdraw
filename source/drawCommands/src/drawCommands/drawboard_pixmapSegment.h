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
//  \file      drawboard_pixmapSegment.h
//
//  \brief     Declaration of pixmap  segment used in commands
//
//  \author    Stian Broen
//
//  \date      22.03.2012
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 22.03.2012 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

#ifndef DRAWBOARD_PIXMAPSEGMENT_H
#define DRAWBOARD_PIXMAPSEGMENT_H

// Qt includes
#include <QPixmap>
#include <QRect>

namespace sdraw {
   class C2DPixmapSegment
   {
   public:
      C2DPixmapSegment(QPixmap *content = 0,
         QRect *geometry = 0,
         int index = -1,
         int offset = 0);
      ~C2DPixmapSegment();
      int getIndex();
      int getOffset();
      void setIndex(int val);
      QPixmap getPixmap();
      QRect getGeometry();

   private:
      QPixmap m_content;
      QRect m_geometry;
      int m_index;
      int m_offset;
   };
}
#endif