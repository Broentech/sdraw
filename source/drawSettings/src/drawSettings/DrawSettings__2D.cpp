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
//  \file      DrawSettings_2D.cpp
//
//  \brief     Implementation of a class to contains settings for the drawboard
//
//  \author    Stian Broen
//
//  \date      09.04.2012
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 09.04.2012 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QMutex>

// local includes
#include "DrawSettings_2D.h"

using namespace sdraw;

CDrawSettings_2D* CDrawSettings_2D::m_instance = 0;

CDrawSettings_2D::CDrawSettings_2D()
   :QObject(0)
{

}

CDrawSettings_2D::~CDrawSettings_2D()
{

}

CDrawSettings_2D* CDrawSettings_2D::getInstance()
{
   if (0 == m_instance)
   {
      static QMutex mutex;
      mutex.lock();
      if (0 == m_instance)
      {
         m_instance = new CDrawSettings_2D;
      }
      mutex.unlock();
   }
   return m_instance;
}

void CDrawSettings_2D::setBrushSettings(const CBrushSettings &brush)
{
   m_brush = brush;
   emit settingsUpdated();
}

void CDrawSettings_2D::setPenSettings(const CPenSettings &pen)
{
   m_pen = pen;
   emit settingsUpdated();
}

CBrushSettings CDrawSettings_2D::brushSettings() const
{
   return m_brush;
}

CPenSettings CDrawSettings_2D::penSettings() const
{
   return m_pen;
}

CShapeSettings CDrawSettings_2D::shapeSettings() const
{
   return m_shape;
}

void CDrawSettings_2D::setShapeSettings(const CShapeSettings &shape)
{
   m_shape = shape;
   emit settingsUpdated();
}

void CDrawSettings_2D::setShape(shapeType shape)
{
   m_shape.setShape(shape);
}

void CDrawSettings_2D::setRotationAngle(qreal val)
{
   m_shape.setRotationAngle(val);
}

void CDrawSettings_2D::setRoundedRectRadius_X(int val)
{
   m_shape.setRoundedRectRadius_X(val);
}

void CDrawSettings_2D::setRoundedRectRadius_Y(int val)
{
   m_shape.setRoundedRectRadius_Y(val);
}

shapeType CDrawSettings_2D::getShape() const
{
   return m_shape.getShape();
}

qreal CDrawSettings_2D::getRotationAngle() const
{
   return m_shape.getRotationAngle();
}

int CDrawSettings_2D::getRoundedRectRadius_X() const
{
   return m_shape.getRoundedRectRadius_X();
}

int CDrawSettings_2D::getRoundedRectRadius_Y() const
{
   return m_shape.getRoundedRectRadius_Y();
}