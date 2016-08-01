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
//  \file      pixelAlgorithms.h
//
//  \brief     Definitions for class containing pixel related algorithms
//
//  \author    Stian Broen
//
//  \date      22.06.2011
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 22.06.2011 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

#ifndef PIXEL_ALGORITHMS_H
#define PIXEL_ALGORITHMS_H

// Qt includes
#include <QObject>
#include <QPoint>
#include <QList>
#include <QPolygonF>
#include <QImage>
#include <QColor>

class QPixmap;
class QPainter;
class PixelNeighborhood;

#define _PRINT_ASCII_EXPL

namespace sdraw {
   class PixelAlgo : public QObject
   {
      Q_OBJECT

   public:
      explicit PixelAlgo(QObject *parent);
      QPolygonF polygonizeBorder(const QPoint &pt, QPixmap &buffer);
      void click2edge(const QPoint &pt, QPoint &endPt, QImage &buffer);

   private:
      void rawTracePolygon(QString &lastKnownError,
         QPolygonF &polygon,
         const QImage &img,
         const QPoint &pt,
         QPolygonF &dontGoHere,
         int *currRecurs,
         int maxRecurs = 10,
         QPoint *_L1 = 0,
         QPoint *_L2 = 0,
         QPoint *_L3 = 0,
         QPoint *_L4 = 0);

      bool isLinear(const QPointF *a, const QPointF *b, const QPointF *c);

   private:
      int xMax, yMax;
   };
}

#endif