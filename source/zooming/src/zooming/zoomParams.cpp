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
//  \file      zoomParams.cpp
//
//  \brief     Parameters used by the zoomer class
//
//  \author    Stian Broen
//
//  \date      05.02.2012
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 05.02.2012 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

#include "zoomParams.h"

using namespace sdraw;

ZoomParams::ZoomParams()
   : m_centralizedOffset_width(0),
   m_centralizedOffset_height(0),
   m_zoomSwitch(false),
   m_zoomCenter(QPoint(0, 0)),
   m_currentZoom(0),
   m_zoomAmount(0),
   m_widthZoom(0),
   m_heightZoom(0),
   m_panX(0),
   m_panY(0),
   m_cumulativeZoomInWidth(0),
   m_cumulativeZoomInHeight(0)
{

}

ZoomParams::~ZoomParams()
{

}
