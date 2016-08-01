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
//  \file      zoomer.h
//
//  \brief     Declaration of class that handles sdraw on QRect
//
//  \author    Stian Broen
//
//  \date      05.02.2012
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 05.02.2012 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

#ifndef _ZOOMER_H_
#define _ZOOMER_H_

// Qt includes
#include <QRect>
#include <QPolygon>
#include <QObject>
#include <QHash>
#include <QSize>

// local includes
#include "zoomParams.h"

class QWheelEvent;

namespace sdraw {
   class Zoomer : public QObject
   {
      Q_OBJECT

   public:
      explicit Zoomer(QObject* parent = 0);
      ~Zoomer();

      bool hasSource();
      void pan(int dx, int dy);
      void clearSourceRect();
      void addSourceRect(QRect source);
      bool getZoomGeo(QRect *source, QRect *target);
      bool performZooming();
      void wheelEvent(QWheelEvent *);
      void addZoomModifier(Qt::KeyboardModifiers mod);
      void removeZoomModifier(Qt::KeyboardModifiers mod);
      void setZoom(int amount = 120, QPoint *mousePos = 0);
      QPoint getBufferPoint(const QPoint &pt, bool panAdjusted = false, bool strict = false);
      QPoint getZoomedPoint(const QPoint &pt);
      QRect getZoomedRect(const QRect &rect);
      QPolygon getZoomedPolygon(const QPolygon &poly);

   private:
      bool zoomInFromZoomedIn();
      bool zoomInFromZoomedOut();
      bool zoomOutFromZoomedIn();
      bool zoomOutFromZoomedOut();
      QPoint mapToZoomedOutRect(const QPoint &pt);
      QPoint mapToZoomedInRect(const QPoint &pt);
      QPoint mapToZoomedInRect_upperLeftCorner(QPoint &pt, const QSize &sz);
      QPoint backTrackPoint(const QPoint &pt, int index);
      QSize  backTrackSize(const QSize  &sz, int index);

   private:
      QList<Qt::KeyboardModifiers> m_mods;
      QList<QRect> m_zoomSources;
      ZoomParams *m_zoomParameters;
      QString m_name;
   };
}

#endif