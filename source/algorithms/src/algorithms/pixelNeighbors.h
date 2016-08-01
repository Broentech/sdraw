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
//  \file      pixelNeighbors.h
//
//  \brief     Definitions for class to define a 3x3 neighborhood around a pixel
//
//             ----------------     MP - the mid pixel of the neighborhood
//             | a1 | a2 | a3 |     a1 - upper left corner pixel
//             ----------------     a2 - upper mid pixel
//             | b1 | MP | b3 |     a3 - upper right corner pixel
//             ----------------     b1 - left mid pixel
//             | c1 | c2 | c3 |     b3 - right mid pixel
//             ----------------     c1 - bottom left corner pixel
//                                  c2 - bottom mid pixel
//                                  c3 - bottom right pixel
//
//  \author    Stian Broen
//
//  \date      27.06.2011
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 27.06.2011 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

#ifndef PIXEL_NEIGHBORS_H
#define PIXEL_NEIGHBORS_H

// Qt includes
#include <QList>
#include <QString>
#include <QRgb>

// local includes
#include "weightedPoint.h"

// Qt forward declarations
class QImage;
class QPoint;

namespace sdraw {
   class PixelNeighborhood
   {
   public:
      explicit PixelNeighborhood(int x,
         int y,
         QRgb *_startColor = 0,
         const QImage *_img = 0,
         QPoint *_L1 = 0,
         QPoint *_L2 = 0,
         QPoint *_L3 = 0,
         QPoint *_L4 = 0);

      ~PixelNeighborhood();

      CWeightedPoint *getMP();
      CWeightedPoint *geta1();
      CWeightedPoint *geta2();
      CWeightedPoint *geta3();
      CWeightedPoint *getb1();
      CWeightedPoint *getb3();
      CWeightedPoint *getc1();
      CWeightedPoint *getc2();
      CWeightedPoint *getc3();
      CWeightedPoint *getL1();
      CWeightedPoint *getL2();
      CWeightedPoint *getL3();
      CWeightedPoint *getL4();

      void setMP(CWeightedPoint *wpnt = 0, int x = -1, int y = -1);
      void seta1(CWeightedPoint *wpnt = 0, int x = -1, int y = -1);
      void seta2(CWeightedPoint *wpnt = 0, int x = -1, int y = -1);
      void seta3(CWeightedPoint *wpnt = 0, int x = -1, int y = -1);
      void setb1(CWeightedPoint *wpnt = 0, int x = -1, int y = -1);
      void setb3(CWeightedPoint *wpnt = 0, int x = -1, int y = -1);
      void setc1(CWeightedPoint *wpnt = 0, int x = -1, int y = -1);
      void setc2(CWeightedPoint *wpnt = 0, int x = -1, int y = -1);
      void setc3(CWeightedPoint *wpnt = 0, int x = -1, int y = -1);
      void setL1(CWeightedPoint *wpnt = 0, int x = -1, int y = -1);
      void setL2(CWeightedPoint *wpnt = 0, int x = -1, int y = -1);
      void setL3(CWeightedPoint *wpnt = 0, int x = -1, int y = -1);
      void setL4(CWeightedPoint *wpnt = 0, int x = -1, int y = -1);

      bool defineNeighbourhood(CWeightedPoint *newMP = 0, int x = -1, int y = -1);

      bool calculateWeights(bool *isTrenchEntrance, bool *isDeadEnd);
      QList<QString> getBestCandidates(
         QPoint *_L1 = 0,
         QPoint *_L2 = 0,
         QPoint *_L3 = 0,
         QPoint *_L4 = 0
         );
      CWeightedPoint *getPoint(const QString &_name);
      const QImage *getImg();
      QRgb *getColor();
      bool isInNeighborhood(const CWeightedPoint *pt);
      bool isBreadcrumb(const CWeightedPoint *point);
      int holeCount();

   private:
      bool checkForDeadEnd();
      bool checkForTrenchEntrance(int addedWeight);
      bool inspectCornerPixels(int addedWeight);
      bool inspectMidPixels();
      bool findPixelColors();
      QList<QString> calcWeightsNoBread(int *currHighest);
      QList<QString> calcWeights(int *currHighest);
      QList<QString> backTrace();
      void initAllBreadcrumbs(QPoint *_L1 = 0,
         QPoint *_L2 = 0,
         QPoint *_L3 = 0,
         QPoint *_L4 = 0);

   private:
      const QImage *img;
      QRgb *startColor;
      QList <QString> allPts;
      int currX, currY, a1x, a1y, a2x, a2y, a3x, a3y, b1x, b1y, b3x, b3y, c1x, c1y, c2x, c2y, c3x, c3y;
      CWeightedPoint *MP, *a1, *a2, *a3, *b1, *b3, *c1, *c2, *c3, *L1, *L2, *L3, *L4;
   };
}

#endif