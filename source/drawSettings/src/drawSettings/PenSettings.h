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
//  \file      PenSettings.h
//
//  \brief     Declaration of a class to contains pen settings for the drawboard
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

#ifndef PEN_SETTINGS_H
#define PEN_SETTINGS_H

// Qt includes
#include <QColor>
#include <QPen>
#include <QPixmap>

// local includes
#include "ShapeSettings.h"

namespace sdraw {
   class CPenSettings
   {
   public:
      CPenSettings();
      ~CPenSettings();
      Qt::PenCapStyle getCapStyle() const;
      Qt::PenJoinStyle getJoinStyle() const;
      Qt::PenStyle getStyle() const;
      QColor getColor() const;
      qreal getMiterLimit() const;
      qreal getDashOffset() const;
      qreal getWidth() const;
      QPixmap *getPenTexturePixmap() const;
      QString getPenTexturePath() const;
      bool getUsePenTexture() const;
      QPen getPen() const;
      //CShapeSettings *getShapeSettings() const;
      void setCapStyle(Qt::PenCapStyle capStyle, bool doUpdatePen = true);
      void setJoinStyle(Qt::PenJoinStyle joinStyle, bool doUpdatePen = true);
      void setStyle(Qt::PenStyle style, bool doUpdatePen = true);
      void setColor(QColor color, bool doUpdatePen = true);
      void setMiterLimit(qreal val, bool doUpdatePen = true);
      void setDashOffset(qreal val, bool doUpdatePen = true);
      void setWidth(qreal val, bool doUpdatePen = true);
      void setPenTexturePixmap(QPixmap *pixmap, bool doUpdatePen = true);
      void setPenTexturePath(const QString &path, bool doUpdatePen = true);
      void setUsePenTexture(bool val, bool doUpdatePen = true);
      void setPen(QPen pen, bool doUpdatePen = true);
      //void setShapeSettings(CShapeSettings *shape);
      CPenSettings& operator=(const CPenSettings &compare);
      bool operator==(const CPenSettings &compare);
      bool operator!=(const CPenSettings &compare);

   private:
      void updatePen();

   private:
      //CShapeSettings *m_shapeSettings;
      Qt::PenCapStyle m_capStyle;
      Qt::PenJoinStyle m_joinStyle;
      Qt::PenStyle m_style;
      QColor m_color;
      qreal m_miterLimit;
      qreal m_dashOffset;
      qreal m_width;
      QPixmap *m_penTexturePixmap;
      QString m_penTexturePath;
      bool m_usePenTexture;
      QPen m_pen;
   };
}

#endif