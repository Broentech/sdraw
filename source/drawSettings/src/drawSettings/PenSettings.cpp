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
//  \file      PenSettings.cpp
//
//  \brief     Implementation of a class to contains pen settings for the drawboard
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

// Qt includes
#include <QDir>
#include <QPixmap>

// local includes
#include "PenSettings.h"

using namespace sdraw;

CPenSettings::CPenSettings()
   ://m_shapeSettings(new CShapeSettings),
   m_capStyle(Qt::RoundCap),
   m_joinStyle(Qt::MiterJoin),
   m_style(Qt::SolidLine),
   m_color(QColor(Qt::black)),
   m_miterLimit(2.),
   m_dashOffset(0.),
   m_width(1.),
   m_penTexturePixmap(0),
   m_usePenTexture(false)
{
   updatePen();
}

CPenSettings::~CPenSettings()
{
   if(0 != m_penTexturePixmap)
   {
      delete m_penTexturePixmap;
      m_penTexturePixmap = 0;
   }
   //m_shapeSettings = 0;
}

void CPenSettings::updatePen()
{
   m_pen.setCapStyle (m_capStyle);
   m_pen.setJoinStyle(m_joinStyle);
   m_pen.setColor    (m_color);
   m_pen.setWidthF   (m_width);
   if(m_style == Qt::CustomDashLine)
   {
      m_pen.setDashOffset(m_dashOffset);
   }
   else if(m_joinStyle == Qt::MiterJoin)
   {
      m_pen.setMiterLimit(m_miterLimit);
   }
}

Qt::PenCapStyle CPenSettings::getCapStyle() const
{
   return m_capStyle;
}

Qt::PenJoinStyle CPenSettings::getJoinStyle() const
{
   return m_joinStyle;
}

Qt::PenStyle CPenSettings::getStyle() const
{
   return m_style;
}

QColor CPenSettings::getColor() const
{
   return m_color;
}

qreal CPenSettings::getMiterLimit() const
{
   return m_miterLimit;
}

qreal CPenSettings::getDashOffset() const
{
   return m_dashOffset;
}

qreal CPenSettings::getWidth() const
{
   return m_width;
}

QPixmap *CPenSettings::getPenTexturePixmap() const
{
   return m_penTexturePixmap;
}

QString CPenSettings::getPenTexturePath() const
{
   return m_penTexturePath;
}

bool CPenSettings::getUsePenTexture() const
{
   return m_usePenTexture;
}

QPen CPenSettings::getPen() const
{
   return m_pen;
}

//CShapeSettings* CPenSettings::getShapeSettings() const
//{
//   return m_shapeSettings;
//}

void CPenSettings::setCapStyle(Qt::PenCapStyle capStyle, bool doUpdatePen)
{
   m_capStyle = capStyle;
   if(doUpdatePen)
   {
      updatePen();
   }
}

void CPenSettings::setJoinStyle(Qt::PenJoinStyle joinStyle, bool doUpdatePen)
{
   m_joinStyle = joinStyle;
   if(doUpdatePen)
   {
      updatePen();
   }
}

void CPenSettings::setStyle(Qt::PenStyle style, bool doUpdatePen)
{
   m_style = style;
   if(doUpdatePen)
   {
      updatePen();
   }
}

void CPenSettings::setColor(QColor color, bool doUpdatePen)
{
   m_color = color;
   if(doUpdatePen)
   {
      updatePen();
   }
}

void CPenSettings::setMiterLimit(qreal val, bool doUpdatePen)
{
   m_miterLimit = val;
   if(doUpdatePen)
   {
      updatePen();
   }
}

void CPenSettings::setDashOffset(qreal val, bool doUpdatePen)
{
   m_dashOffset = val;
   if(doUpdatePen)
   {
      updatePen();
   }
}

void CPenSettings::setWidth(qreal val, bool doUpdatePen)
{
   m_width = val;
   if(doUpdatePen)
   {
      updatePen();
   }
}

void CPenSettings::setPenTexturePixmap(QPixmap *pixmap, bool doUpdatePen)
{
   if(0 != pixmap && !pixmap->isNull())
   {
      m_penTexturePixmap = &pixmap->copy();
      if(doUpdatePen)
      {
         updatePen();
      }
   }
}

void CPenSettings::setPenTexturePath(const QString &path, bool doUpdatePen)
{
   if (!path.isEmpty() && QDir::QDir(path).exists())
   {
      m_penTexturePath = path;
      if(doUpdatePen)
      {
         updatePen();
      }
   }
}

void CPenSettings::setUsePenTexture(bool val, bool doUpdatePen)
{
   m_usePenTexture = val;
   if(doUpdatePen)
   {
      updatePen();
   }
}

//void CPenSettings::setShapeSettings(CShapeSettings *shape)
//{
//   if(0 == shape)
//   {
//      return;
//   }
//   m_shapeSettings = shape;
//}

CPenSettings& CPenSettings::operator=(const CPenSettings &compare)
{
   if(this == &compare)
   {
      return *this;
   } 

   this->setCapStyle(compare.getCapStyle(), false);
   this->setJoinStyle(compare.getJoinStyle(), false);
   this->setStyle(compare.getStyle(), false);
   this->setColor(compare.getColor(), false);
   this->setMiterLimit(compare.getMiterLimit(), false);
   this->setDashOffset(compare.getDashOffset(), false);
   this->setWidth(compare.getWidth(), false);
   this->setPenTexturePixmap(compare.getPenTexturePixmap(), false);
   this->setPenTexturePath(compare.getPenTexturePath(), false);
   this->setUsePenTexture(compare.getUsePenTexture(), false);
   //this->setShapeSettings(compare.getShapeSettings());
   this->updatePen();
   return *this;
}

bool CPenSettings::operator==(const CPenSettings &compare)
{
   return (
      //*m_shapeSettings == *compare.getShapeSettings() &&
      m_capStyle == compare.getCapStyle() &&
      m_joinStyle == compare.getJoinStyle() &&
      m_style == compare.getStyle() &&
      m_color == compare.getColor() &&
      m_miterLimit == compare.getMiterLimit() &&
      m_dashOffset == compare.getDashOffset() &&
      m_width == compare.getWidth() &&
      m_penTexturePath == compare.getPenTexturePath() &&
      m_usePenTexture == compare.getUsePenTexture() &&
      m_pen == compare.getPen()
      );
}

bool CPenSettings::operator!=(const CPenSettings &compare)
{
   return !(*this==compare);
}