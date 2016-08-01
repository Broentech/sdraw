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
//  \brief     Implementation of pixmap  segment used in commands
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

// Qt includes
#include <QPixmap>
#include <QRect>

// local includes
#include "drawboard_pixmapSegment.h"

using namespace sdraw;

C2DPixmapSegment::C2DPixmapSegment(QPixmap *content, QRect *geometry, int index, int offset)
   :m_index(index),
   m_offset(offset)
{
   if (0 != content)
   {
      m_content = content->copy();
   }
   if (0 != geometry)
   {
      m_geometry = *geometry;
   }
}

C2DPixmapSegment::~C2DPixmapSegment()
{

}

int C2DPixmapSegment::getIndex()
{
   return m_index;
}

void C2DPixmapSegment::setIndex(int val)
{
   m_index = val;
}

QPixmap C2DPixmapSegment::getPixmap()
{ 
   return m_content;
}

QRect C2DPixmapSegment::getGeometry()
{
   return m_geometry;
}

int C2DPixmapSegment::getOffset()
{
   return m_offset;
}