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
//  \file      weightedPoint.cpp
//
//  \brief     Definition of a class that is an extension of QPoint to include weight
//
//  \author    Stian Broen
//
//  \date      27.06.2011
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 27.06.2011 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

#include "weightedPoint.h"

using namespace sdraw;

CWeightedPoint::CWeightedPoint(int x, int y, const QString &_name)
{
   this->QPoint::QPoint(x ,y);
   weight = 0;
   colorDiff = false;
   name = _name;
}

CWeightedPoint::~CWeightedPoint()
{
   
}