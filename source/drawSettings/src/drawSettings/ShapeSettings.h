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
//  \file      ShapeSettings.h
//
//  \brief     Declaration of a class to contains shape settings for the drawboard
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

#ifndef SHAPE_SETTINGS_H
#define SHAPE_SETTINGS_H

// Qt includes
#include <QtGlobal>

// solution includes
#include <Common/definitions.h>

namespace sdraw {
   class CShapeSettings
   {
   public:
      CShapeSettings();
      ~CShapeSettings();
      void setShape(shapeType shape);
      void setRotationAngle(qreal val);
      void setRoundedRectRadius_X(int val);
      void setRoundedRectRadius_Y(int val);
      shapeType getShape() const;
      qreal getRotationAngle() const;
      int getRoundedRectRadius_X() const;
      int getRoundedRectRadius_Y() const;
      CShapeSettings& operator=(const CShapeSettings &compare);
      bool operator==(const CShapeSettings &compare);
      bool operator!=(const CShapeSettings &compare);

   private:
      shapeType m_shape;
      qreal m_rotationAngle;
      int m_roundedRectRadius_X;
      int m_roundedRectRadius_Y;
   };
}
#endif