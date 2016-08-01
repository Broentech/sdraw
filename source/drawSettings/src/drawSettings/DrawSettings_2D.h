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
//  \file      DrawSettings_2D.h
//
//  \brief     Declaration of a class to contains settings for the drawboard
//
//  \author    Stian Broen
//
//  \date      03.04.2012
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 03.04.2012 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

#ifndef DRAW_SETTINGS_2D_H
#define DRAW_SETTINGS_2D_H

// Qt includes
#include <QObject>
#include <QHash>

// local includes
#include "BrushSettings.h"
#include "PenSettings.h"
#include "ShapeSettings.h"

namespace sdraw {
   class CDrawSettings_2D : public QObject
   {
      Q_OBJECT

   public:
      static CDrawSettings_2D* getInstance();
      ~CDrawSettings_2D();
      CBrushSettings brushSettings() const;
      CPenSettings penSettings() const;

      CShapeSettings shapeSettings() const;
      void setShape(shapeType shape);
      void setRotationAngle(qreal val);
      void setRoundedRectRadius_X(int val);
      void setRoundedRectRadius_Y(int val);
      shapeType getShape() const;
      qreal getRotationAngle() const;
      int getRoundedRectRadius_X() const;
      int getRoundedRectRadius_Y() const;

   signals:
      void settingsUpdated();

   public slots:
      void setBrushSettings(const CBrushSettings &brush);
      void setPenSettings(const CPenSettings &pen);
      void setShapeSettings(const CShapeSettings &shape);

   private:
      static CDrawSettings_2D* m_instance;
      explicit CDrawSettings_2D();
      CDrawSettings_2D(const CDrawSettings_2D &other);
      CDrawSettings_2D& operator=(const CDrawSettings_2D& rhs);
      CBrushSettings m_brush;
      CPenSettings m_pen;
      CShapeSettings m_shape;
   };
}

#endif