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
//  \file      BrushSettings.h
//
//  \brief     Declaration of a class to contains brush settings for the drawboard
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

#ifndef BRUSH_SETTINGS_H
#define BRUSH_SETTINGS_H

// Qt includes
#include <QColor>
#include <QBrush>
#include <QMap>
#include <QPen>
#include <QPixmap>

// local includes
#include "PenSettings.h"

namespace sdraw {
   class CBrushSettings
   {
   public:
      CBrushSettings();
      ~CBrushSettings();
      QMap<qreal, QColor> getGradientColors() const;
      QString getBrushTexturePath() const;
      QString explain() const;
      QPixmap *getBrushTexturePixmap() const;
      bool getUseBrushTexture() const;
      Qt::BrushStyle getBrushStyle() const;
      QGradient::Spread getBrushSpread() const;
      int getAlpha() const;
      QColor getBrushColor() const;
      QBrush getBrush() const;
      qreal getAngle() const;
      CPenSettings getPenSettings() const;
      void changeGradientColor(qreal oldPos, qreal newPos, const QColor &color, bool doUpdateBrush = true);
      void removeGradientColor(qreal pos);
      void addGradientColor(qreal index, QColor color, bool doUpdateBrush = true);
      void setGradientColors(QMap<qreal, QColor> colors, bool doUpdateBrush = true);
      void setBrushTexturePath(const QString &path, bool doUpdateBrush = true);
      void setBrushTexturePixmap(QPixmap *pixmap, bool doUpdateBrush = true);
      void setUseBrushTexture(bool val, bool doUpdateBrush = true);
      void setBrushStyle(Qt::BrushStyle style, bool doUpdateBrush = true);
      void setBrushSpread(QGradient::Spread spread, bool doUpdateBrush = true);
      void setAlpha(int val, bool doUpdateBrush = true);
      void setBrushColor(QColor color, bool doUpdateBrush = true);
      void setAngle(qreal angle, bool doUpdateBrush = true);
      void setPenSettings(const CPenSettings &pen);
      void setGradientCenter(const QPointF &p);
      void setGradientFocal(const QPointF &p);
      inline QPointF gradientCenterPoint() const { return m_gradCenter; }
      inline QPointF gradientFocalPoint() const { return m_gradFocalPt; }
      CBrushSettings& operator=(const CBrushSettings &compare);
      bool operator==(const CBrushSettings &compare);
      bool operator!=(const CBrushSettings &compare);

   private:
      void updateBrush();

   private:
      CPenSettings m_penSettings;
      QMap<qreal, QColor> m_gradientColors;
      QString m_brushTexturePath;
      QPixmap *m_brushTexturePixmap;
      bool m_useBrushTexture;
      Qt::BrushStyle m_brushStyle;
      QGradient::Spread m_spread;
      int m_alpha;
      QColor m_brushColor;
      QPointF m_gradCenter;
      QPointF m_gradFocalPt;
      qreal m_gradAngle;
      QBrush m_brush;
   };

   QString explainBrush(const QBrush &brush);
}

#endif