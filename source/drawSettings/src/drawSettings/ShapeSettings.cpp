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
//  \file      ShapeSettings.cpp
//
//  \brief     Implementation of a class to contains shape settings for the drawboard
//
//  \author    Stian Broen
//
//  \date      05.04.2012
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 05.04.2012 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

// local includes
#include "ShapeSettings.h"

using namespace sdraw;

CShapeSettings::CShapeSettings()
   :m_shape(SNoShape),
   m_rotationAngle(0.),
   m_roundedRectRadius_X(0),
   m_roundedRectRadius_Y(0)
{

}

CShapeSettings::~CShapeSettings()
{

}

void CShapeSettings::setShape(shapeType shape)
{
   m_shape = shape;
}

void CShapeSettings::setRotationAngle(qreal val)
{
   m_rotationAngle = val;
}

void CShapeSettings::setRoundedRectRadius_X(int val)
{
   m_roundedRectRadius_X = val;
}

void CShapeSettings::setRoundedRectRadius_Y(int val)
{
   m_roundedRectRadius_Y = val;
}

shapeType CShapeSettings::getShape() const
{
   return m_shape;
}

qreal CShapeSettings::getRotationAngle() const
{
   return m_rotationAngle;
}

int CShapeSettings::getRoundedRectRadius_X() const
{
   return m_roundedRectRadius_X;
}

int CShapeSettings::getRoundedRectRadius_Y() const
{
   return m_roundedRectRadius_Y;
}

CShapeSettings& CShapeSettings::operator=(const CShapeSettings &compare)
{
   if(this == &compare)
   {
      return *this;
   } 
   this->setShape(compare.getShape());
   this->setRotationAngle(compare.getRotationAngle());
   this->setRoundedRectRadius_X(compare.getRoundedRectRadius_X());
   this->setRoundedRectRadius_Y(compare.getRoundedRectRadius_Y());
   return *this;
}

bool CShapeSettings::operator==(const CShapeSettings &compare)
{
   if(
      compare.getShape() == this->m_shape &&
      compare.getRoundedRectRadius_X() == this->m_roundedRectRadius_X &&
      compare.getRoundedRectRadius_Y() == this->m_roundedRectRadius_Y &&
      compare.getRotationAngle() == this->m_rotationAngle
      )
   {
      return true;
   }
   else
   {
      return false;
   }
}

bool CShapeSettings::operator!=(const CShapeSettings &compare)
{
   if(
      compare.getShape() != this->m_shape ||
      compare.getRoundedRectRadius_X() != this->m_roundedRectRadius_X ||
      compare.getRoundedRectRadius_Y() != this->m_roundedRectRadius_Y ||
      compare.getRotationAngle() != this->m_rotationAngle
      )
   {
      return true;
   }
   else
   {
      return false;
   }
}