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
//  \file      pixelAlgorithms.cpp
//
//  \brief     Implementations for class containing pixel related algorithms
//
//  \author    Stian Broen
//
//  \date      22.06.2011
//
//  \par       Copyright:
//
//             Copyright by Stian Broen
//
//  \par       Revision History
//
//  \par       01 22.06.2011 Original version       
//
//
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QPoint>
#include <QPainter>
#include <QPixmap>
#include <QPainterPath>
#include <QtDebug>
#include <math.h>

// local includes
#include "pixelAlgorithms.h"
#include "pixelNeighbors.h"
#include "weightedPoint.h"

using namespace sdraw;

PixelAlgo::PixelAlgo(QObject *parent)
   :QObject(parent),
   xMax(1),
   yMax(1)
{

}


//  Based on the argument point and pixel map buffer, find the limiting area
//  which surrounds the point, and fill it using the argument painter
QPolygonF PixelAlgo::polygonizeBorder(const QPoint &pt, QPixmap &buffer)
{
   QPolygonF polygon;

   // use qimage, it is made for direct pixel access
   QImage img = buffer.toImage();
   if (img.isNull())
   {
      return polygon;
   }

   xMax = buffer.width();
   yMax = buffer.height();
   QPolygonF taggedPoints;
   int recursions = 0;
   QString lastError;

   // using the rawTracePolygon algorithm to find polygon
   rawTracePolygon(
      lastError,
      polygon,
      img,
      pt,
      taggedPoints,
      &recursions
      );

   // use polygon and argument painter to perform filling operation
   return polygon;
}



//  Find the point that is on the horizontal line from the argument pt point to
//  the first point in the argument pixmap that has a color different than the color
//  at pt.
void PixelAlgo::click2edge(const QPoint &pt, QPoint &endPt, QImage &buffer)
{
   if (buffer.isNull())
   {
      return;
   }

   QPoint startPt = pt;
   endPt = startPt;
   int nextX = pt.x();
   QRgb startColor = buffer.pixel(pt);

   // find start point of bounding area
   while (buffer.pixel(endPt) == startColor && nextX < buffer.width())
   {
      nextX++;
      endPt.setX(nextX);
   }
   nextX = nextX - 2;
   endPt.setX(nextX);
}


//  This recursive function creates a polygon (one per recursion) and adds it
//  to the argument list. The algorithm starts by moving from the argument point (pt) withing the
//  argument image (image) until it hits a color that is different from color at the argument point.
//
//                      |//////
//                       \//////
//                        \///////
//                         |////////
//                             \///////
//          X ----------------->MP///////
//                               \///////
//                               |///////
//
//  From there, it starts inspecting the 3x3 neighbourhood pixels to find out where to go next.
//
//    ----------------     MP - the mid pixel of the neighborhood
//    | a1 | a2 | a3 |     a1 - upper left corner pixel
//    ----------------     a2 - upper mid pixel
//    | b1 | MP | b3 |     a3 - upper right corner pixel
//    ----------------     b1 - left mid pixel
//    | c1 | c2 | c3 |     b3 - right mid pixel
//    ----------------     c1 - bottom left corner pixel
//                         c2 - bottom mid pixel
//                         c3 - bottom right pixel
//
//  To reduce the amount of next MP dilemmas, the corner pixels (a1, a3, c1, c3) takes priority over
//  the mid pixels (a2, b1, b3, c2).
//  To avoid ending up in deadlocks, the 4 last MPs are stored in each iteration
//
//    ------//////////
//    | L4 |//////////
//    ------//////////
//    | L3 |//////////
//    ----------------     L1 - Bread crumb pixel 1
//    | L2 | L1 | a3 |     L2 - Bread crumb pixel 2
//    ----------------     L3 - Bread crumb pixel 3
//    | b1 | MP | b3 |     L4 - Bread crumb pixel 4
//    ----------------     
//    | c1 | c2 | c3 |     In this scenario, a1 and a2 are excluded as candidates for the next MP
//    ----------------
//
//
//  \param     QString &lastKnownError  - "NONE" if everything went ok
//             QPolygonF &polygon       - polygon to be created
//             const QImage &img        - image
//             const QPoint &pt         - start point
//             QPolygonF                - 
//             int *currRecurs          - current recursion reference
//             int maxRecurs            - maximum allowed recursions
//             QPoint *_L1              - pointer to L1
//             QPoint *_L2              - pointer to L1
//             QPoint *_L3              - pointer to L1
//             QPoint *_L4              - pointer to L1
//
void PixelAlgo::rawTracePolygon(
   QString &lastKnownError,
   QPolygonF &polygon,
   const QImage &img,
   const QPoint &pt,
   QPolygonF &dontGoHere,
   int *currRecurs,
   int maxRecurs,
   QPoint *_L1,
   QPoint *_L2,
   QPoint *_L3,
   QPoint *_L4)
{

   // check that the argument conditions to proceed are met
   if (!img.valid(pt)) // the argument start point must be valid within the image
   {
      lastKnownError = "NO IMAGE DEFINED";
      return;
   }

#ifdef _PRINT_ASCII_EXPL
   // init private members
   lastKnownError = "NONE";   // used for printing neighborhood to std out, debugging perposes
   QString a1FILL = "    ";   // used for printing neighborhood to std out, debugging perposes
   QString a2FILL = "    ";   // used for printing neighborhood to std out, debugging perposes
   QString a3FILL = "    ";   // used for printing neighborhood to std out, debugging perposes
   QString b1FILL = "    ";   // used for printing neighborhood to std out, debugging perposes
   QString b3FILL = "    ";   // used for printing neighborhood to std out, debugging perposes
   QString c1FILL = "    ";   // used for printing neighborhood to std out, debugging perposes
   QString c2FILL = "    ";   // used for printing neighborhood to std out, debugging perposes
   QString c3FILL = "    ";   // used for printing neighborhood to std out, debugging perposes
#endif

   QRgb startColor = img.pixel(pt); // the color of the pixel at the point where the user clicked
   PixelNeighborhood *neighbors = 0;          // used for evaluating 3x3 pixel neighourhood, fundamental object 
   QPoint startPt = pt;            // point of the mouse cursor where the user clicked, or if its not the first recursion its the first border point
   QPoint secondStartPt = startPt;       // this is needed incase second(or further) recursion starts out linearly                
   QPoint thirdStartPt = startPt;
   int nextX = pt.x();        // used to find the first point
   int pointCnt = 0;             // count the amount of found points
   int removeCount = 0;             // count the number of removed points (linearity), debugging perposes
   bool initHit = false;         // if the entry points starts with linearity, remove that point the where it stops being linear
   bool trench = false;
   bool dontGo2MP = false;
   bool trenchEntrance = false;
   bool deadEnd = false;
   bool savePnt = false;
   QList <QString> candidates;

   bool isTrenchExit = false;
   QPoint savedPoint;
   QPoint trenchExitPoint;
   QPoint MP;  // central pixel of the 3x3 neighborhood
   QPoint L1;  // breadcrumb closest to MP
   QPoint L2;  // second breadcrumb
   QPoint L3;  // third breadbrumb
   QPoint L4;  // fourth breadcrumb

   // if the algorithm has entered another recursion, the breadcrumbs are passed on
   if (_L1 != 0 && _L2 != 0 && _L3 != 0 && _L4 != 0)
   {
      // if breadcrumbs are defined, it is assumed that argument MP is already along the border
      // so there is no search for it.
      L1 = *_L1;
      L2 = *_L2;
      L3 = *_L3;
      L4 = *_L4;
   }

   // in the initial recursion, there are no breadcrumb and the first border pixel must be located
   else
   {
      // find start point of bounding area
      while (img.pixel(startPt) == startColor)
      {
         nextX++;
         if (nextX >= xMax)
         {
            lastKnownError = "OUT OF PICTURE BORDER";
            return;
         }
         startPt.setX(nextX);
      }
      nextX--;
      startPt.setX(nextX);

      if ((nextX - pt.x()) > 1)
         L1 = QPoint(startPt.x() - 1, startPt.y());
      else
         L1 = QPoint(startPt.x(), startPt.y() - 1);
      L2 = L1;
      L3 = L1;
      L4 = L1;
   }

   // add start point to polygon
   MP = startPt;
   polygon << MP;

   // create neighborhood around MP
   neighbors = new PixelNeighborhood(MP.x(),
      MP.y(),
      &startColor,
      &img,
      &L1,
      &L2,
      &L3,
      &L4);

   // ----
   // start iterations around border starting with start point
   // ----

   // loop while MP has not reached the start point, at which it has reached the pixel where it started
   while ((MP != startPt && MP != secondStartPt && MP != thirdStartPt) || pointCnt < 2)
   {
      // calculate weights
      if (!neighbors || !neighbors->calculateWeights(&trenchEntrance, &deadEnd))
      {
         lastKnownError = "WEIGHT CALCULATION FAILED";
         return;
      }

      if (MP.x() > xMax || MP.y() > yMax)
      {
         lastKnownError = "OUT OF PICTURE BORDER";
         return;
      }

      // get the best candidates for the new MP
      candidates.clear();
      for (int i = 0; i < 5; i++)
      {
         candidates = neighbors->getBestCandidates();
         if (candidates.size() != 0)
            break;
      }

      // if no candidates for the next MP is found, there is nothing more to do, return with the points found
      if (candidates.size() == 0)
      {
         // there are no candidates, stop
         // check for dead end
         if (deadEnd)
         {
            CWeightedPoint *tmpWP = neighbors->getPoint("MP");
            if (0 != tmpWP)
            {
               MP = QPoint(tmpWP->x(), tmpWP->y());
               polygon << MP;
            }
         }
         break;
      }

      // DEBUGGING - output neighborhood to std out
#ifdef _PRINT_ASCII_EXPL

      if (neighbors->geta1()->colorDiff)
         a1FILL = "XXXX";
      else if ((QPoint)*neighbors->geta1() == L1)
         a1FILL = " L1 ";
      else if ((QPoint)*neighbors->geta1() == L2)
         a1FILL = " L2 ";
      else
         a1FILL = "    ";

      if (neighbors->geta2()->colorDiff)
         a2FILL = "XXXX";
      else if ((QPoint)*neighbors->geta2() == L1)
         a2FILL = " L1 ";
      else if ((QPoint)*neighbors->geta2() == L2)
         a2FILL = " L2 ";
      else
         a2FILL = "    ";

      if (neighbors->geta3()->colorDiff)
         a3FILL = "XXXX";
      else if ((QPoint)*neighbors->geta3() == L1)
         a3FILL = " L1 ";
      else if ((QPoint)*neighbors->geta3() == L2)
         a3FILL = " L2 ";
      else
         a3FILL = "    ";

      if (neighbors->getb1()->colorDiff)
         b1FILL = "XXXX";
      else if ((QPoint)*neighbors->getb1() == L1)
         b1FILL = " L1 ";
      else if ((QPoint)*neighbors->getb1() == L2)
         b1FILL = " L2 ";
      else
         b1FILL = "    ";

      if (neighbors->getb3()->colorDiff)
         b3FILL = "XXXX";
      else if ((QPoint)*neighbors->getb3() == L1)
         b3FILL = " L1 ";
      else if ((QPoint)*neighbors->getb3() == L2)
         b3FILL = " L2 ";
      else
         b3FILL = "    ";

      if (neighbors->getc1()->colorDiff)
         c1FILL = "XXXX";
      else if ((QPoint)*neighbors->getc1() == L1)
         c1FILL = " L1 ";
      else if ((QPoint)*neighbors->getc1() == L2)
         c1FILL = " L2 ";
      else
         c1FILL = "    ";

      if (neighbors->getc2()->colorDiff)
         c2FILL = "XXXX";
      else if ((QPoint)*neighbors->getc2() == L1)
         c2FILL = " L1 ";
      else if ((QPoint)*neighbors->getc2() == L2)
         c2FILL = " L2 ";
      else
         c2FILL = "    ";

      if (neighbors->getc3()->colorDiff)
         c3FILL = "XXXX";
      else if ((QPoint)*neighbors->getc3() == L1)
         c3FILL = " L1 ";
      else if ((QPoint)*neighbors->getc3() == L2)
         c3FILL = " L2 ";
      else
         c3FILL = "    ";

      qDebug() << QString("----------------");
      qDebug() << QString("|%1|%2|%3|").arg(a1FILL).arg(a2FILL).arg(a3FILL);
      qDebug() << QString("----------------");
      qDebug() << QString("|%1|%2|%3|").arg(b1FILL).arg(" MP ").arg(b3FILL);
      qDebug() << QString("----------------");
      qDebug() << QString("|%1|%2|%3|").arg(c1FILL).arg(c2FILL).arg(c3FILL);
      qDebug() << QString("----------------");

      qDebug() << QString("%1 - %2 - X:%3 - Y:%4").arg(pointCnt).arg("MP").arg(MP.x()).arg(MP.y());
      //qDebug() << QString("%1 - %2 - X:%3 - Y:%4").arg(pointCnt).arg(tmp->name).arg(tmp->x()).arg(tmp->y());
#endif
      /*****/

      if (trenchEntrance && candidates.size() == 2)
      {
         if (pointCnt == 0)
         {
            trench = false;
            savePnt = true;
            savedPoint = MP;
         }
         else if (!trench)
         {
            if (candidates.at(0).contains(":IN_TRENCH"))
            {
               QString tmpStr = candidates.at(0);
               QString tmpStr2 = candidates.at(1);
               candidates.clear();
               candidates << tmpStr2 << tmpStr;
            }
            trench = true;
         }
         else
         {
            trench = false;
         }
      }

      // if there are more than 1 candidate, start recursions
      // note 23.07.2011 : the algorithm will sometimes reach an initial dilemma at startup, when there are
      // 2 alternative paths to take on when MP reaches the polygon border. One solution is to simply choose one
      // of them arbritarely. This is the current solution.
      if ((candidates.size() > 1 && polygon.size() > 1) && !(trenchEntrance && !trench))
      {
         if (candidates.size() > 4)
         {
            // this means that an error has occured, can't continue
            lastKnownError = "IMPOSSIBLE SITUATION REACHED, MORE THAN 4 PATHS";
            return;
         }

         // more than 1 solutions exists AND the size of the polygon is larger than 1, meaning that the
         // algorithm is not in the first iteration. This situation is assumed to be very rare, it will be the
         // case when 1 empty pixel is the gap between 2 polygons. If for instance the gap is 2 pixels, the
         // algorithm will continue normally as if its still 1 polygon. Note that even though I'm talking about
         // 2 polygons, it really is still just 1 programmatically.

         dontGoHere << startPt;
         dontGoHere << secondStartPt;
         dontGoHere << thirdStartPt;

         QPoint newMP;
         QPoint L1Copy;
         QPoint L2Copy;
         QPoint L3Copy;
         QPoint L4Copy;

         for (int i = 1; i < candidates.size(); i++)
         {
            *currRecurs += 1;
            newMP = QPoint(neighbors->getPoint(candidates.at(i))->x(), neighbors->getPoint(candidates.at(i))->y());
            L1Copy = MP;
            L2Copy = L1;
            L3Copy = L2;
            L4Copy = L3;

            // put another recursion on the stack with the breadcrumbs from the current iteration
            rawTracePolygon(lastKnownError,
               polygon,
               img,
               newMP,
               dontGoHere,
               currRecurs,
               maxRecurs,
               &L1Copy,
               &L2Copy,
               &L3Copy,
               &L4Copy);
            trench = false;
         }
      }

      QPoint tmpPnt;
      QPoint tmpPnt2;
      CWeightedPoint *tmp = 0;
      CWeightedPoint *tmp2 = 0;

      // meaning its a trench exit
      if (trenchEntrance && !trench && candidates.size() == 2)
      {
         isTrenchExit = true;
         trenchExitPoint = MP;
         pointCnt = 0;
         tmp2 = neighbors->getPoint(candidates.at(1));
         tmpPnt2.setX(tmp2->x());
         tmpPnt2.setY(tmp2->y());
         polygon << MP;
      }

      // get the first candidate (there should at this point be only 1)
      tmp = neighbors->getPoint(candidates.at(0));
      tmpPnt.setX(tmp->x());
      tmpPnt.setY(tmp->y());

      // Reaching here means that the algorithm had a solution for the current neighborhood

      // ******************************
      // * FINISHED CURRENT ITERATION *
      // ******************************

      // move to new MP and update breadcrumbs
      L4 = L3;
      L3 = L2;
      L2 = L1;
      L1 = MP;
      MP = tmpPnt;

      // check if we have reached the end
      if (dontGoHere.contains(MP))
         break;

      if (dontGo2MP)
      {
         dontGo2MP = false;
         dontGoHere << MP;
      }
      if (0 != tmp2)
      {
         dontGoHere << tmpPnt2;
      }

      // update neighbourhood
      neighbors->defineNeighbourhood(0, MP.x(), MP.y());
      neighbors->setL1(0, L1.x(), L1.y());
      neighbors->setL2(0, L2.x(), L2.y());
      neighbors->setL3(0, L3.x(), L3.y());
      neighbors->setL4(0, L4.x(), L4.y());

      // add MP to vector
      polygon << MP;

      // check for linearity between the last 3 points, remove point 2 if its on the line
      // between 1 and 3
      int polySize = polygon.size();
      if (polySize > 2)
      {
         if (isLinear(&polygon.at(polySize - 3),
            &polygon.at(polySize - 2),
            &polygon.at(polySize - 1)))
         {
            // the second point is redundant, remove it if it isnt the first point
            if (initHit)
            {
               pointCnt--;
               polygon.remove(polySize - 2);
               removeCount++;
            }
            else if (!trench)
            {
               initHit = true;
               pointCnt--;
               secondStartPt = L2;
               thirdStartPt = L1;
            }
            else
               pointCnt--;
         }
         else if (!initHit)
         {
            // the start point must be moved to the first found point that is not linear with earlier points.
            // this means that if the algorithm starts out with a straight line, the start point must be moved to the
            // end of this line. This is because if the algorithm has entered a new recursion the algorithm will enter
            // an endless loop since the start-conditions for a new recursion will be the same on both sides otherwise.
            initHit = true;
            pointCnt = 0;
            startPt = MP;
         }
      }

      pointCnt++;
   }

   if (savePnt)
      polygon << savedPoint;

   // clean up
   delete neighbors;
   neighbors = 0;

   return;
}

//  Check if the parameter points are on a straight line. Works like this :
//
//  1. Map the points a,b and c so that a falls into origo
//  2. Calculate the climb factor using f(x) = a + climb * x where a=0 -> climb = f(x) / x -> climb = c.y / c.x
//  3. Check if b.y = climb * b.x, if so, then the point b is on the straight line between a and c
bool PixelAlgo::isLinear(const QPointF *a, const QPointF *b, const QPointF *c)
{
   // map points so that a falls into origo
   float cx = c->x() - a->x();
   float cy = c->y() - a->y();
   float bx = b->x() - a->x();
   float by = b->y() - a->y();

   // check for vertical line, if it is, then simply compare the x coordinate of b and c, if the are the same, then the points are 
   // on the line
   if (cx == 0)
   {
      if (b->x() == c->x())
         return true;
      else
         return false;
   }

   // calculate climb-factor
   float climb = cy / cx;

   // calculate the y-value for the straight line between a and c at b.x.
   // pixel values are whole numbers, so compare to the nearest 2 up and down
   float yF = climb * bx;
   //int   yFfloor = floor(yF);
   //int   yFCeil  = ceil(yF);

   // compare this y-value with b.y, if they are the same then we got a line
   //if(by == yFfloor || by == yFCeil)
   if (by == yF)
      return true;
   else
      return false;
}