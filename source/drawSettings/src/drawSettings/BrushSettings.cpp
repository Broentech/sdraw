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
//  \file      BrushSettings.cpp
//
//  \brief     Implementation of a class to contains brush settings for the drawboard
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
//  \par       01 05.04.2012 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QFileInfo>
#include <QPixmap>

// local includes
#include "BrushSettings.h"

using namespace sdraw;

CBrushSettings::CBrushSettings()
   :m_brushTexturePixmap(0),
   m_useBrushTexture(false),
   m_brushStyle(Qt::NoBrush),
   m_spread(QGradient::PadSpread),
   m_alpha(255),
   m_brushColor(QColor(Qt::black)),
   m_gradCenter(QPointF(0.0, 0.0)),
   m_gradFocalPt(QPointF(0.0, 0.0)),
   m_gradAngle(90.)
{
   updateBrush();
}

CBrushSettings::~CBrushSettings()
{
   if(0 != m_brushTexturePixmap)
   {
      delete m_brushTexturePixmap;
      m_brushTexturePixmap = 0;
   }
}

QString CBrushSettings::explain() const
{
   return explainBrush(m_brush);
}

QString sdraw::explainBrush(const QBrush &brush)
{
   QString ret;
   switch (brush.style())
   {
   case(Qt::LinearGradientPattern) :
   {
      QLinearGradient *linGrad = (QLinearGradient *)(brush.gradient());
      ret.append("Gradient style : LinearGradientPattern\n");
      ret.append("Start : X=" + QString::number(linGrad->start().x()) + ", Y=" + QString::number(linGrad->start().y()) + "\n");
      ret.append("Stop : X=" + QString::number(linGrad->finalStop().x()) + ", Y=" + QString::number(linGrad->finalStop().y()) + "\n");
      break;
   }
   case(Qt::ConicalGradientPattern) :
      ret.append("Gradient style : ConicalGradientPattern\n");
      break;

   case(Qt::RadialGradientPattern) :
      ret.append("Gradient style : RadialGradientPattern\n");
      break;

   default:
   ret.append("Gradient style : None\n");
   break;
   }

   const QGradient *grad = brush.gradient();
   if (0 != grad)
   {
      QVector< QPair<qreal, QColor> > colors = grad->stops();
      if (colors.size() > 0)
      {
         ret.append("COLORS: \n");
         for (int i = 0; i < colors.size(); i++)
         {
            QPair<qreal, QColor> p = colors.at(i);
            ret.append(QString::number(p.first) + " : R=" + QString::number(p.second.red()) + ", G=" + QString::number(p.second.green()) + ", B=" + QString::number(p.second.blue()) + "\n");
         }
      }
   }

   return ret;
}

void CBrushSettings::setGradientCenter(const QPointF &p)
{
   m_gradCenter = p;
   updateBrush();
}

void CBrushSettings::setGradientFocal(const QPointF &p)
{
   m_gradFocalPt = p;
   updateBrush();
}

void CBrushSettings::updateBrush()
{
   m_brush.setStyle(m_brushStyle);
   QMapIterator<qreal, QColor> it(m_gradientColors);
   switch(m_brushStyle)
   {
   case(Qt::LinearGradientPattern):
   {
      QLinearGradient  linGradient(m_gradCenter, m_gradFocalPt);
      while (it.hasNext())
      {
         it.next();
         linGradient.setColorAt(it.key(), it.value());
      }
      linGradient.setSpread(m_spread);
      m_brush = QBrush(linGradient);
      break;
   }
   case(Qt::ConicalGradientPattern):
   {
      if (m_gradCenter.isNull())
      {
         m_brush.setStyle(Qt::NoBrush);
         m_brush.setColor(m_brushColor);
         break;
      }
      QConicalGradient conGradient = QConicalGradient(m_gradCenter, m_gradAngle);
      while (it.hasNext())
      {
         it.next();
         conGradient.setColorAt(it.key(), it.value());
      }
      m_brush = QBrush(conGradient);
      break;
   }
   case(Qt::RadialGradientPattern):
   {
      if (m_gradCenter.isNull() || m_gradFocalPt.isNull())
      {
         m_brush.setStyle(Qt::NoBrush);
         m_brush.setColor(m_brushColor);
         break;
      }
      QRadialGradient radGradient = QRadialGradient(m_gradCenter, m_gradAngle, m_gradFocalPt);
      while (it.hasNext())
      {
         it.next();
         radGradient.setColorAt(it.key(), it.value());
      }
      radGradient.setSpread(m_spread);
      m_brush = QBrush(radGradient);
      break;
   }
   default:
      m_brush.setColor(m_brushColor);
      break;
   }
}

QMap<qreal, QColor> CBrushSettings::getGradientColors()const
{
   return m_gradientColors;
}

QString CBrushSettings::getBrushTexturePath()const
{
   return m_brushTexturePath;
}

QPixmap *CBrushSettings::getBrushTexturePixmap()const
{
   return m_brushTexturePixmap;
}

bool CBrushSettings::getUseBrushTexture()const
{
   return m_useBrushTexture;
}

Qt::BrushStyle CBrushSettings::getBrushStyle()const
{
   return m_brushStyle;
}

QGradient::Spread CBrushSettings::getBrushSpread()const
{
   return m_spread;
}

int CBrushSettings::getAlpha()const
{
   return m_alpha;
}

QColor CBrushSettings::getBrushColor()const
{
   return m_brushColor;
}

QBrush CBrushSettings::getBrush()const
{
   return m_brush;
}

CPenSettings CBrushSettings::getPenSettings() const
{
   return m_penSettings;
}

qreal CBrushSettings::getAngle() const
{
   return m_gradAngle;
}

void CBrushSettings::addGradientColor(qreal index, QColor color, bool doUpdateBrush)
{
   m_gradientColors.insert(index, color);
   if(doUpdateBrush)
   {
      updateBrush();
   }
}

void CBrushSettings::changeGradientColor(qreal oldPos, qreal newPos, const QColor &color, bool doUpdateBrush)
{
   if(!m_gradientColors.contains(oldPos))
   {
      return addGradientColor(newPos, color, doUpdateBrush);
   }
   QMutableMapIterator<qreal, QColor> it(m_gradientColors);
   while(it.hasNext())
   {
      it.next();
      qreal  pos = it.key();
      if(pos == oldPos)
      {
         it.remove();
         return addGradientColor(newPos, color, doUpdateBrush);
      }
   }
}

void CBrushSettings::removeGradientColor(qreal pos)
{
   if(!m_gradientColors.contains(pos))
   {
      return;
   }

   QMutableMapIterator<qreal, QColor> it(m_gradientColors);
   while(it.hasNext())
   {
      it.next();
      if(it.key() == pos)
      {
         it.remove();
         return;
      }
   }
   updateBrush();
}

void CBrushSettings::setGradientColors(QMap<qreal, QColor> colors, bool doUpdateBrush)
{
   m_gradientColors.clear();
   m_gradientColors = colors;
   if(doUpdateBrush)
   {
      updateBrush();
   }
}

void CBrushSettings::setBrushTexturePath(const QString &path, bool doUpdateBrush)
{
   if(!QFileInfo::QFileInfo(path).exists())
   {
      return;
   }
   m_brushTexturePath = path;
   if(doUpdateBrush)
   {
      updateBrush();
   }
}

void CBrushSettings::setBrushTexturePixmap(QPixmap *pixmap, bool doUpdateBrush)
{
   if(0 == pixmap)
   {
      return;
   }
   m_brushTexturePixmap = pixmap;
   if(doUpdateBrush)
   {
      updateBrush();
   }
}

void CBrushSettings::setUseBrushTexture(bool val, bool doUpdateBrush)
{
   m_useBrushTexture = val;
   if(doUpdateBrush)
   {
      updateBrush();
   }
}

void CBrushSettings::setBrushStyle(Qt::BrushStyle style, bool doUpdateBrush)
{
   m_brushStyle = style;
   if(doUpdateBrush)
   {
      updateBrush();
   }
}

void CBrushSettings::setBrushSpread(QGradient::Spread spread, bool doUpdateBrush)
{
   m_spread = spread;
   if(doUpdateBrush)
   {
      updateBrush();
   }
}

void CBrushSettings::setAlpha(int val, bool doUpdateBrush)
{
   if(val >= 0 && val <= 255)
   {
      m_alpha = val;
      if(doUpdateBrush)
      {
         updateBrush();
      }
   }
}

void CBrushSettings::setBrushColor(QColor color, bool doUpdateBrush)
{
   m_brushColor = color;
   if(doUpdateBrush)
   {
      updateBrush();
   }
}

void CBrushSettings::setAngle(qreal angle, bool doUpdateBrush)
{
   m_gradAngle = angle;
   if(doUpdateBrush)
   {
      updateBrush();
   }
}

void CBrushSettings::setPenSettings(const CPenSettings &pen)
{
   m_penSettings = pen;
}

CBrushSettings& CBrushSettings::operator=(const CBrushSettings &compare)
{
   if(this == &compare)
   {
      return *this;
   }

   m_penSettings = compare.getPenSettings();
   m_gradientColors = compare.getGradientColors();
   m_brushTexturePath = compare.getBrushTexturePath();
   if (0 != compare.getBrushTexturePixmap())
   {
      *m_brushTexturePixmap = *compare.getBrushTexturePixmap();
   }
   m_useBrushTexture = compare.getUseBrushTexture();
   m_brushStyle = compare.getBrushStyle();
   m_spread = compare.getBrushSpread();
   m_alpha = compare.getAlpha();
   m_brushColor = compare.getBrushColor();
   m_gradCenter = compare.gradientCenterPoint();
   m_gradFocalPt = compare.gradientFocalPoint();
   m_gradAngle = compare.getAngle();
   m_brush = compare.getBrush();

   return *this;
}

bool CBrushSettings::operator==(const CBrushSettings &compare)
{
   return (
      compare.getAlpha() == this->m_alpha &&
      compare.getBrush() == this->m_brush &&
      compare.getBrushColor() == this->m_brushColor &&
      compare.getBrushSpread() == this->m_spread &&
      compare.getBrushStyle() == this->m_brushStyle &&
      compare.getBrushTexturePath() == this->m_brushTexturePath &&
      compare.getUseBrushTexture() == this->m_useBrushTexture &&
      compare.getGradientColors() == this->m_gradientColors &&
      compare.getPenSettings() == this->m_penSettings &&
      m_gradCenter == compare.gradientCenterPoint() &&
      m_gradFocalPt == compare.gradientFocalPoint() &&
      m_gradAngle == compare.getAngle()
      );
}

bool CBrushSettings::operator!=(const CBrushSettings &compare)
{
   return !(*this==compare);
}