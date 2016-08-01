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
//  \file      draw_Base.h
//
//  \brief     Declaration of the draw board base functionality
//
//  \author    Stian Broen
//
//  \date      01.04.2012
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 01.04.2012 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

#ifndef DRAW_BASE_H
#define DRAW_BASE_H

// Qt includes
#include <QObject>
#include <QHash>
#include <QString>
#include <QColor>
#include <QBrush>
#include <QPen>

// solution includes
#include <Common/definitions.h>
#include <DrawSettings/DrawSettings_2D.h>
#include <zooming/zoomer.h>

// Qt forward declarations
class QPixmap;
class QPen;

namespace sdraw {
   class CDrawBase : public QObject
   {
      Q_OBJECT

   public:
      CDrawBase(QObject *parent = 0);
      virtual ~CDrawBase();
      
      virtual void registerBuffer(QPixmap *buffer);
      virtual void registerAidBuffer(QPixmap *buffer);
      virtual void registerZoomer(Zoomer *zoomer,
         const QString &bufferName,
         const QString &aidBufferName);
      virtual void releaseBuffer();
      virtual void releaseAidBuffer();
      virtual void releaseZoomer();
      virtual bool hasRegisteredAidBuffer();
      virtual bool hasRegisteredBuffer();
      virtual bool hasRegisteredZoomer();
      void blankAidBuffer();

   signals:
      void requestUpdate();
      void info(const QString &content);
      void activateMouseMovement(bool onOrOff);

   protected:
      void drawAidLine(QPoint start, QPoint stop, QPen *pen = 0);
      void drawAidEllipse(QPoint bottomRight, QPen *pen = 0);
      void drawAidRectangle(QPoint bottomRight, QPen *pen = 0);
      void drawAidRoundedRectangle(QPoint bottomRight, QPen *pen = 0);
      void drawAidArc(QPoint pt, QPen *pen = 0);
      void drawAidPie(QPoint pt, QPen *pen = 0);
      void drawAidChord(QPoint pt, QPen *pen = 0);
      void drawAidPolyline(QPolygon _polyLine, QPen *pen = 0);
      void drawAidPolygon(QPolygon _polygon, QPen *pen = 0);

   protected:
      //shapeType currShape;
      bool aidOn;
      CSettings _settings;
      TOperation mOperation;
      QPoint fillPt;
      bool stateSelectInterpolationPoints;
      bool firstInterpPointSelected;
      bool newInterpolPts;
      QPoint origo;
      QPoint topLeftForCircular;
      QPoint bottomRightForCircular;
      //int roundedRectRadius_X;
      //int roundedRectRadius_Y;
      int clockCount;
      int    radius;
      qreal _angle;
      qreal _lastAngleDiff;
      qreal  alphaDegrees;
      qreal  betaDegrees;
      Quadrant lastQuadrant;
      Quadrant alphaQuadrant;
      QPolygon polyLine;
      QPolygon polygon;
      QPixmap *m_buffer;
      QPixmap *m_aidBuffer;
      Zoomer *m_zoomer;
      QString m_bufferName;
      QString m_aidBufferName;
      bool bufferRegistered;
      bool aidBufferRegistered;
      bool hasZoomer;
      QColor backgroundColor;
      QPoint _last;
      Qt::MouseButton mouseButtonBuff;
      bool aidOnly;
      bool doUsePenTexture;
      QString penTexturePath;
   };
}

#endif