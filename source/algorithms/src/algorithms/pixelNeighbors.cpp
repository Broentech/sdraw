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
//  \file      pixelNeighbors.cpp
//
//  \brief     Implementations for class to define a 3x3 neighborhood around a pixel
//
//  \author    Stian Broen
//
//  \date      27.06.2011
//
//  \par       Copyright:
//
//             Copyright by Stian Broen
//
//  \par       Revision History
//
//  \par       01 27.06.2011 Original version       
//
//
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QImage>
#include <QPoint>
#include <QColor>

// local includes
#include "pixelNeighbors.h"
#include "weightedPoint.h"

using namespace sdraw;
PixelNeighborhood::PixelNeighborhood(int x,
   int y,
   QRgb *_startColor,
   const QImage *_img,
   QPoint *_L1,
   QPoint *_L2,
   QPoint *_L3,
   QPoint *_L4)
   :img(_img),
   startColor(_startColor),
   MP(0),
   a1(0),
   a2(0),
   a3(0),
   b1(0),
   b3(0),
   c1(0),
   c2(0),
   c3(0)
{
   if (_L1 != 0) L1 = new CWeightedPoint(_L1->x(), _L1->y(), "L1"); else _L1 = 0;
   if (_L2 != 0) L2 = new CWeightedPoint(_L2->x(), _L2->y(), "L2"); else _L2 = 0;
   if (_L3 != 0) L3 = new CWeightedPoint(_L3->x(), _L3->y(), "L3"); else _L3 = 0;
   if (_L4 != 0) L4 = new CWeightedPoint(_L4->x(), _L4->y(), "L4"); else _L4 = 0;

   // get the coordinates of the MP window
   defineNeighbourhood(0, x, y);
}


//  Define the neighbourhood around the argument point
bool PixelNeighborhood::defineNeighbourhood(CWeightedPoint *newMP, int x, int y)
{
   if (newMP == 0)
   {
      if (x == -1 || y == -1)
         return false;

      currX = x;
      currY = y;

      // set MP
      if (MP == 0)
         MP = new CWeightedPoint(currX, currY, "MP");
      else
      {
         MP->setX(currX);
         MP->setY(currY);
         MP->name = "MP";
      }
   }
   else
   {
      // set MP
      if (MP == 0)
         MP = new CWeightedPoint(newMP->x(), newMP->y(), "MP");
      else
         MP = newMP;

      // set neighbourhood coordinates
      currX = MP->x();
      currY = MP->y();
   }
   a1x = currX - 1;
   a1y = currY - 1;
   a2x = currX;
   a2y = currY - 1;
   a3x = currX + 1;
   a3y = currY - 1;
   b1x = currX - 1;
   b1y = currY;
   b3x = currX + 1;
   b3y = currY;
   c1x = currX - 1;
   c1y = currY + 1;
   c2x = currX;
   c2y = currY + 1;
   c3x = currX + 1;
   c3y = currY + 1;

   // set neighbourhood points
   if (a1 != 0)
   {
      a1->setX(a1x);
      a1->setY(a1y);
      a1->name = "a1";
   }
   else
   {
      a1 = new CWeightedPoint(a1x, a1y, "a1"); allPts << "a1";
   }

   if (a2 != 0)
   {
      a2->setX(a2x);
      a2->setY(a2y);
      a2->name = "a2";
   }
   else
   {
      a2 = new CWeightedPoint(a2x, a2y, "a2"); allPts << "a2";
   }

   if (a3 != 0)
   {
      a3->setX(a3x);
      a3->setY(a3y);
      a3->name = "a3";
   }
   else
   {
      a3 = new CWeightedPoint(a3x, a3y, "a3"); allPts << "a3";
   }

   if (b1 != 0)
   {
      b1->setX(b1x);
      b1->setY(b1y);
      b1->name = "b1";
   }
   else
   {
      b1 = new CWeightedPoint(b1x, b1y, "b1"); allPts << "b1";
   }

   if (b3 != 0)
   {
      b3->setX(b3x);
      b3->setY(b3y);
      b3->name = "b3";
   }
   else
   {
      b3 = new CWeightedPoint(b3x, b3y, "b3"); allPts << "b3";
   }

   if (c1 != 0)
   {
      c1->setX(c1x);
      c1->setY(c1y);
      c1->name = "c1";
   }
   else
   {
      c1 = new CWeightedPoint(c1x, c1y, "c1"); allPts << "c1";
   }

   if (c2 != 0)
   {
      c2->setX(c2x);
      c2->setY(c2y);
      c2->name = "c2";
   }
   else
   {
      c2 = new CWeightedPoint(c2x, c2y, "c2"); allPts << "c2";
   }

   if (c3 != 0)
   {
      c3->setX(c3x);
      c3->setY(c3y);
      c3->name = "c3";
   }
   else
   {
      c3 = new CWeightedPoint(c3x, c3y, "c3"); allPts << "c3";
   }

   return true;
}


PixelNeighborhood::~PixelNeighborhood()
{
   delete MP;
   delete a1;
   delete a2;
   delete a3;
   delete b1;
   delete b3;
   delete c1;
   delete c2;
   delete c3;
   img = 0;
   L1  = 0;
   L2  = 0;
   L3  = 0;
   L4  = 0;
}


//  Calculates the weight of the neighborhood pixels, based on the colors
bool PixelNeighborhood::calculateWeights(bool *isTrenchEntrance, bool *isDeadEnd)
{
   int tmpCnt = 0;

   // make sure all necessary parameters are defined before proceeding
   if (img != 0 &&
      startColor != 0 &&
      a1 != 0 &&
      a2 != 0 &&
      a3 != 0 &&
      b1 != 0 &&
      b3 != 0 &&
      c1 != 0 &&
      c2 != 0 &&
      c3 != 0)
   {

      // reset weights
      a1->weight = 0;
      a2->weight = 0;
      a3->weight = 0;
      b1->weight = 0;
      b3->weight = 0;
      c1->weight = 0;
      c2->weight = 0;
      c3->weight = 0;

      // check the color of each pixel in the MP window, if it equals the
      // start color then increase weight (+2 if corner, +1 if mid)
      if (!findPixelColors())
         return false;

      // check if the MP is headed into a trench, in that case, the entrance and each corner should get equal weight
      if (checkForTrenchEntrance(2))
      {
         *isTrenchEntrance = true;
         return true;
      }
      else
         *isTrenchEntrance = false;

      // check if MP is at a dead end
      if (checkForDeadEnd())
      {
         *isDeadEnd = true;
         return true;
      }
      else
         *isDeadEnd = false;

      // inspect corner points, and give weights
      if (!inspectCornerPixels(2))
         return false;

      // Inspect mid points , if there are 2 or more neighbouring colored pixels, give +3 weight
      if (!inspectMidPixels())
         return false;

      return true;
   }
   else
      return false;
}


//  Check if MP is closing in on a trench. If thats the case, then the algorithm must know that it must go both
//  in the trench, and continue outside, therefore equal weights are given
bool PixelNeighborhood::checkForTrenchEntrance(int addedWeight)
{
   if (a1 == 0 || a2 == 0 || a3 == 0 || b1 == 0 || b3 == 0 || c1 == 0 || c2 == 0 || c3 == 0)
      return false;

   //  ----------------    ----------------    ----------------    ----------------
   //  |XXXX|    |XXXX|    |XXXX|    |XXXX|    |XXXX|    |XXXX|    |XXXX|    |XXXX| 
   //  ----------------    ----------------    ----------------    ---------------- 
   //  |XXXX| MP |XXXX|    |XXXX| MP |XXXX|    |XXXX| MP |XXXX|    |XXXX| MP |XXXX| 
   //  ----------------    ----------------    ----------------    ---------------- 
   //  |    |    |    |    |    |    |XXXX|    |XXXX|    |    |    |XXXX|    |XXXX| 
   //  ----------------    ----------------    ----------------    ----------------
   if (a1->colorDiff == true &&
      a3->colorDiff == true &&
      b1->colorDiff == true &&
      b3->colorDiff == true)
   {
      if (a2->colorDiff == false &&
         c1->colorDiff == false &&
         c2->colorDiff == false &&
         c3->colorDiff == false)
      {
         a2->weight = addedWeight;
         a2->name = "a2:IN_TRENCH";
         c1->weight = addedWeight;
         c3->weight = addedWeight;
         return true;
      }
      else if (a2->colorDiff == false &&
         c1->colorDiff == false &&
         c2->colorDiff == false &&
         c3->colorDiff == true)
      {
         a2->weight = addedWeight;
         a2->name = "a2:IN_TRENCH";
         c1->weight = addedWeight;
         c2->weight = addedWeight;
         return true;
      }
      else if (a2->colorDiff == false &&
         c1->colorDiff == true &&
         c2->colorDiff == false &&
         c3->colorDiff == false)
      {
         a2->weight = addedWeight;
         a2->name = "a2:IN_TRENCH";
         c2->weight = addedWeight;
         c3->weight = addedWeight;
         return true;
      }
      else if (a2->colorDiff == false &&
         c1->colorDiff == true &&
         c2->colorDiff == false &&
         c3->colorDiff == true)
      {
         a2->weight = addedWeight;
         a2->name = "a2:IN_TRENCH";
         c2->weight = addedWeight;
         c2->name = "c2:IN_TRENCH";
         return true;
      }
   }

   //  ----------------    ----------------    ----------------    
   //  |    |    |    |    |XXXX|    |    |    |    |    |XXXX|    
   //  ----------------    ----------------    ----------------    
   //  |XXXX| MP |XXXX|    |XXXX| MP |XXXX|    |XXXX| MP |XXXX|    
   //  ----------------    ----------------    ----------------    
   //  |XXXX|    |XXXX|    |XXXX|    |XXXX|    |XXXX|    |XXXX|    
   //  ----------------    ----------------    ----------------    
   else if (b1->colorDiff == true &&
      b3->colorDiff == true &&
      c1->colorDiff == true &&
      c3->colorDiff == true)
   {
      if (a1->colorDiff == false &&
         a2->colorDiff == false &&
         a3->colorDiff == false &&
         c2->colorDiff == false)
      {
         c2->weight = addedWeight;
         c2->name = "c2:IN_TRENCH";
         a1->weight = addedWeight;
         a3->weight = addedWeight;
         return true;
      }
      else if (a1->colorDiff == true &&
         a2->colorDiff == false &&
         a3->colorDiff == false &&
         c2->colorDiff == false)
      {
         c2->weight = addedWeight;
         c2->name = "c2:IN_TRENCH";
         a2->weight = addedWeight;
         a3->weight = addedWeight;
         return true;
      }
      else if (a1->colorDiff == false &&
         a2->colorDiff == false &&
         a3->colorDiff == true &&
         c2->colorDiff == false)
      {
         c2->weight = addedWeight;
         c2->name = "c2:IN_TRENCH";
         a1->weight = addedWeight;
         a2->weight = addedWeight;
         return true;
      }
   }

   //  ----------------    ----------------    ----------------    ----------------    
   //  |XXXX|XXXX|    |    |XXXX|XXXX|XXXX|    |XXXX|XXXX|    |    |XXXX|XXXX|XXXX|    
   //  ----------------    ----------------    ----------------    ----------------    
   //  |    | MP |    |    |    | MP |    |    |    | MP |    |    |    | MP |    |    
   //  ----------------    ----------------    ----------------    ----------------    
   //  |XXXX|XXXX|    |    |XXXX|XXXX|    |    |XXXX|XXXX|XXXX|    |XXXX|XXXX|XXXX|    
   //  ----------------    ----------------    ----------------    ----------------   
   else if (a1->colorDiff == true &&
      a2->colorDiff == true &&
      c1->colorDiff == true &&
      c2->colorDiff == true)
   {
      if (b1->colorDiff == false &&
         a3->colorDiff == false &&
         b3->colorDiff == false &&
         c3->colorDiff == false)
      {
         b1->weight = addedWeight;
         b1->name = "b1:IN_TRENCH";
         a3->weight = addedWeight;
         c3->weight = addedWeight;
         return true;
      }
      else if (b1->colorDiff == false &&
         a3->colorDiff == true &&
         b3->colorDiff == false &&
         c3->colorDiff == false)
      {
         b1->weight = addedWeight;
         b1->name = "b1:IN_TRENCH";
         b3->weight = addedWeight;
         c3->weight = addedWeight;
         return true;
      }
      else if (b1->colorDiff == false &&
         a3->colorDiff == false &&
         b3->colorDiff == false &&
         c3->colorDiff == true)
      {
         b1->weight = addedWeight;
         b1->name = "b1:IN_TRENCH";
         a3->weight = addedWeight;
         b3->weight = addedWeight;
         return true;
      }
      else if (b1->colorDiff == false &&
         a3->colorDiff == true &&
         b3->colorDiff == false &&
         c3->colorDiff == true)
      {
         b1->weight = addedWeight;
         b1->name = "b1:IN_TRENCH";
         b3->weight = addedWeight;
         b3->name = "b3:IN_TRENCH";
         return true;
      }
   }

   //  ----------------    ----------------    ---------------- 
   //  |    |XXXX|XXXX|    |    |XXXX|XXXX|    |XXXX|XXXX|XXXX| 
   //  ----------------    ----------------    ---------------- 
   //  |    | MP |    |    |    | MP |    |    |    | MP |    | 
   //  ----------------    ----------------    ---------------- 
   //  |    |XXXX|XXXX|    |XXXX|XXXX|XXXX|    |    |XXXX|XXXX| 
   //  ----------------    ----------------    ---------------- 
   else if (a2->colorDiff == true &&
      a3->colorDiff == true &&
      c2->colorDiff == true &&
      c3->colorDiff == true)
   {
      if (a1->colorDiff == false &&
         b1->colorDiff == false &&
         c1->colorDiff == false &&
         b3->colorDiff == false)
      {
         b3->weight = addedWeight;
         b3->name = "b3:IN_TRENCH";
         a1->weight = addedWeight;
         c1->weight = addedWeight;
         return true;
      }
      else if (a1->colorDiff == false &&
         b1->colorDiff == false &&
         c1->colorDiff == true &&
         b3->colorDiff == false)
      {
         a1->weight = addedWeight;
         b1->weight = addedWeight;
         b3->weight = addedWeight;
         b3->name = "b3:IN_TRENCH";
         return true;
      }
      else if (a1->colorDiff == true &&
         b1->colorDiff == false &&
         c1->colorDiff == false &&
         b3->colorDiff == false)
      {
         b1->weight = addedWeight;
         c1->weight = addedWeight;
         b3->weight = addedWeight;
         b3->name = "b3:IN_TRENCH";
         return true;
      }
   }

   return false;
}


//  Check if MP is at a dead end
bool PixelNeighborhood::checkForDeadEnd()
{
   if(a1 == 0 || a2 == 0 || a3 == 0 || b1 == 0 || b3 == 0 || c1 == 0 || c2 == 0 || c3 == 0)
      return false;

   //  ----------------
   //  |XXXX|XXXX|XXXX|
   //  ----------------
   //  |XXXX| MP |XXXX|
   //  ----------------
   //  |XXXX|    |XXXX|
   //  ----------------
   if(a1->colorDiff == true  &&
      a2->colorDiff == true  &&
      a3->colorDiff == true  && 
      b1->colorDiff == true  &&
      b3->colorDiff == true  &&
      c1->colorDiff == true  &&
      c2->colorDiff == false &&
      c3->colorDiff == true   )
   {
      c2->name = ("c2:DEAD_END");
      return true;
   }
   //  ----------------
   //  |XXXX|XXXX|XXXX|
   //  ----------------
   //  |    | MP |XXXX|
   //  ----------------
   //  |XXXX|XXXX|XXXX|
   //  ----------------
   else if(a1->colorDiff == true  &&
           a2->colorDiff == true  &&
           a3->colorDiff == true  && 
           b1->colorDiff == false &&
           b3->colorDiff == true  &&
           c1->colorDiff == true  &&
           c2->colorDiff == true  &&
           c3->colorDiff == true   )
   {
      b1->name = ("b1:DEAD_END");
      return true;
   }
   //  ----------------
   //  |XXXX|    |XXXX|
   //  ----------------
   //  |XXXX| MP |XXXX|
   //  ----------------
   //  |XXXX|XXXX|XXXX|
   //  ----------------
   else if(a1->colorDiff == true  &&
           a2->colorDiff == false &&
           a3->colorDiff == true  && 
           b1->colorDiff == true  &&
           b3->colorDiff == true  &&
           c1->colorDiff == true  &&
           c2->colorDiff == true  &&
           c3->colorDiff == true   )
   {
      a2->name = ("a2:DEAD_END");
      return true;
   }
   //  ----------------
   //  |XXXX|XXXX|XXXX|
   //  ----------------
   //  |XXXX| MP |    |
   //  ----------------
   //  |XXXX|XXXX|XXXX|
   //  ----------------
   else if(a1->colorDiff == true  &&
           a2->colorDiff == true  &&
           a3->colorDiff == true  && 
           b1->colorDiff == true  &&
           b3->colorDiff == false &&
           c1->colorDiff == true  &&
           c2->colorDiff == true  &&
           c3->colorDiff == true   )
   {
      b3->name = ("b3:DEAD_END");
      return true;
   }
   else
      return false;
}

//  Inspect the pixels in the corners and give weights based on neighbouring pixelcolors
bool PixelNeighborhood::inspectCornerPixels(int addedWeight)
{
   if (a1 == 0 || a2 == 0 || a3 == 0 || b1 == 0 || b3 == 0 || c1 == 0 || c2 == 0 || c3 == 0)
      return false;

   // inspect a1
   if (a1->colorDiff == false)
   {
      // this scenario means that a1 is NOT a candidate because it's not near the border
      // -----------
      // | a1 |    |
      // -----------
      // |    | MP |    
      // -----------
      if (a2->colorDiff == false &&
         b1->colorDiff == false)
      {
         a1->weight = 0;
      }

      // this scenario means that a1 is NOT a candidate because it's on the other side of the border
      // -----------
      // | a1 |XXXX|
      // -----------
      // |XXXX| MP |    
      // -----------
      else if (a2->colorDiff == true &&
         b1->colorDiff == true)
      {
         a1->weight = 0;
      }

      // this scenario means that a1 IS a candidate
      // -----------
      // | a1 |    |
      // -----------
      // |XXXX| MP |    
      // -----------
      else if (a2->colorDiff == false &&
         b1->colorDiff == true)
      {
         a1->weight = a1->weight + addedWeight;
      }

      // this scenario means that a1 IS a candidate
      // -----------
      // | a1 |XXXX|
      // -----------
      // |    | MP |    
      // -----------
      else if (a2->colorDiff == true &&
         b1->colorDiff == false)
      {
         a1->weight = a1->weight + addedWeight;
      }
      else
         return false;
   }

   // inspect a3
   if (a3->colorDiff == false)
   {
      // this scenario means that a3 is NOT a candidate because it's not near the border
      // -----------
      // |    | a3 |
      // -----------
      // | MP |    |    
      // -----------
      if (a2->colorDiff == false &&
         b3->colorDiff == false)
      {
         a3->weight = 0;
      }

      // this scenario means that a1 is NOT a candidate because it's on the other side of the border
      // -----------
      // |XXXX| a3 |
      // -----------
      // | MP |XXXX|    
      // -----------
      else if (a2->colorDiff == true &&
         b3->colorDiff == true)
      {
         a3->weight = 0;
      }

      // this scenario means that a1 IS a candidate
      // -----------
      // |XXXX| a3 |
      // -----------
      // | MP |    |    
      // -----------
      else if (a2->colorDiff == true &&
         b3->colorDiff == false)
      {
         a3->weight = a3->weight + addedWeight;
      }

      // this scenario means that a1 IS a candidate
      // -----------
      // |    | a3 |
      // -----------
      // | MP |XXXX|    
      // -----------
      else if (a2->colorDiff == false &&
         b3->colorDiff == true)
      {
         a3->weight = a3->weight + addedWeight;
      }
      else
         return false;
   }

   // inspect c1
   if (c1->colorDiff == false)
   {
      // this scenario means that a3 is NOT a candidate because it's not near the border
      // -----------
      // |    | MP |
      // -----------
      // | c1 |    |    
      // -----------
      if (b1->colorDiff == false &&
         c2->colorDiff == false)
      {
         c1->weight = 0;
      }

      // this scenario means that a1 is NOT a candidate because it's on the other side of the border
      // -----------
      // |XXXX| MP |
      // -----------
      // | c1 |XXXX|    
      // -----------
      else if (b1->colorDiff == true &&
         c2->colorDiff == true)
      {
         c1->weight = 0;
      }

      // this scenario means that a1 IS a candidate
      // -----------
      // |XXXX| MP |
      // -----------
      // | c1 |    |    
      // -----------
      else if (b1->colorDiff == true &&
         c2->colorDiff == false)
      {
         c1->weight = c1->weight + addedWeight;
      }

      // this scenario means that a1 IS a candidate
      // -----------
      // |    | MP |
      // -----------
      // | c1 |XXXX|    
      // -----------
      else if (b1->colorDiff == false &&
         c2->colorDiff == true)
      {
         c1->weight = c1->weight + addedWeight;
      }
      else
         return false;
   }

   // inspect c3
   if (c3->colorDiff == false)
   {
      // this scenario means that c3 is NOT a candidate because it's not near the border
      // -----------
      // | MP |    |
      // -----------
      // |    | c3 |    
      // -----------
      if (b3->colorDiff == false &&
         c2->colorDiff == false)
      {
         c3->weight = 0;
      }

      // this scenario means that c3 is NOT a candidate because it's on the other side of the border
      // -----------
      // | MP |XXXX|
      // -----------
      // |XXXX| c3 |    
      // -----------
      else if (b3->colorDiff == true &&
         c2->colorDiff == true)
      {
         c3->weight = 0;
      }

      // this scenario means that c3 IS a candidate
      // -----------
      // | MP |XXXX|
      // -----------
      // |    | c3 |    
      // -----------
      else if (b3->colorDiff == true &&
         c2->colorDiff == false)
      {
         c3->weight = c3->weight + addedWeight;
      }

      // this scenario means that c3 IS a candidate
      // -----------
      // | MP |    |
      // -----------
      // |XXXX| c3 |    
      // -----------
      else if (b3->colorDiff == false &&
         c2->colorDiff == true)
      {
         c3->weight = c3->weight + addedWeight;
      }

      else
         return false;
   }
   return true;
}


//  Inspect the pixels on the mid vertical and mid horizontal lines in the neighbourhood
bool PixelNeighborhood::inspectMidPixels()
{
   if (img == 0 || startColor == 0 || a1 == 0 || a2 == 0 || a3 == 0 || b1 == 0 || b3 == 0 || c1 == 0 || c2 == 0 || c3 == 0)
      return false;

   int tmpCnt = 0;
   // inspect a2
   if (a2->colorDiff == false)
   {
      if (a1->colorDiff == true) tmpCnt++;
      if (a3->colorDiff == true) tmpCnt++;
      if (b1->colorDiff == true) tmpCnt++;
      if (b3->colorDiff == true) tmpCnt++;
      if (tmpCnt == 1)
         a2->weight++;
      else if (tmpCnt > 1 && tmpCnt < 5)
         a2->weight = a2->weight + 3;
      else
         a2->weight = 0;
   }
   // inspect b1
   if (b1->colorDiff == false)
   {
      tmpCnt = 0;
      if (a1->colorDiff == true) tmpCnt++;
      if (a2->colorDiff == true) tmpCnt++;
      if (c1->colorDiff == true) tmpCnt++;
      if (c2->colorDiff == true) tmpCnt++;
      if (tmpCnt == 1)
         b1->weight++;
      else if (tmpCnt > 1 && tmpCnt < 5)
         b1->weight = b1->weight + 3;
      else
         b1->weight = 0;
   }
   // inspect b3
   if (b3->colorDiff == false)
   {
      tmpCnt = 0;
      if (a2->colorDiff == true) tmpCnt++;
      if (a3->colorDiff == true) tmpCnt++;
      if (c2->colorDiff == true) tmpCnt++;
      if (c3->colorDiff == true) tmpCnt++;
      if (tmpCnt == 1)
         b3->weight++;
      else if (tmpCnt > 1 && tmpCnt < 5)
         b3->weight = b3->weight + 3;
      else
         b3->weight = 0;
   }
   // inspect c2
   if (c2->colorDiff == false)
   {
      int tmpCnt = 0;
      if (b1->colorDiff == true) tmpCnt++;
      if (c1->colorDiff == true) tmpCnt++;
      if (b3->colorDiff == true) tmpCnt++;
      if (c3->colorDiff == true) tmpCnt++;
      if (tmpCnt == 1)
         c2->weight++;
      else if (tmpCnt > 1 && tmpCnt < 5)
         c2->weight = c2->weight + 3;
      else
         c2->weight = 0;
   }
   return true;
}


//  Use the neighbourhood point coordinates to inspect their corresponding color
//  in the picture.
bool PixelNeighborhood::findPixelColors()
{
   if (img == 0 || startColor == 0 || a1 == 0 || a2 == 0 || a3 == 0 || b1 == 0 || b3 == 0 || c1 == 0 || c2 == 0 || c3 == 0)
      return false;
   // compare the point coordinates with the local pixels to see if the colors differ from the initial color
   QRgb compareColor = *startColor;

   // inspect a1 color
   QRgb currColor = img->pixel((QPoint)*a1);
   if (currColor != compareColor)
   {
      a1->colorDiff = true;
   }
   else
   {
      a1->colorDiff = false;
   }

   // inspect a2 color
   currColor = img->pixel((QPoint)*a2);
   if (currColor != compareColor)
   {
      a2->colorDiff = true;
   }
   else
   {
      a2->colorDiff = false;
   }

   // inspect a3 color
   currColor = img->pixel((QPoint)*a3);
   if (currColor != compareColor)
   {
      a3->colorDiff = true;
   }
   else
   {
      a3->colorDiff = false;
   }

   // inspect b1 color
   currColor = img->pixel((QPoint)*b1);
   if (currColor != compareColor)
   {
      b1->colorDiff = true;
   }
   else
   {
      b1->colorDiff = false;
   }

   // inspect b3 color
   currColor = img->pixel((QPoint)*b3);
   if (currColor != compareColor)
   {
      b3->colorDiff = true;
   }
   else
   {
      b3->colorDiff = false;
   }

   // inspect c1 color
   currColor = img->pixel((QPoint)*c1);
   if (currColor != compareColor)
   {
      c1->colorDiff = true;
   }
   else
   {
      c1->colorDiff = false;
   }

   // inspect c2 color
   currColor = img->pixel((QPoint)*c2);
   if (currColor != compareColor)
   {
      c2->colorDiff = true;
   }
   else
   {
      c2->colorDiff = false;
   }

   // inspect c3 color
   currColor = img->pixel((QPoint)*c3);
   if (currColor != compareColor)
   {
      c3->colorDiff = true;
   }
   else
   {
      c3->colorDiff = false;
   }
   return true;
}



//  Inspects the weightings of the neighborhood and returns a list with strings of
//  the names of the pixels that has the highest weight
QList<QString> PixelNeighborhood::getBestCandidates(QPoint *_L1, QPoint *_L2, QPoint *_L3, QPoint *_L4)
{
   QList<QString> returnMe;
   int currHighest = 0;

   // take care of any eventual changes in the breadcrumbs
   initAllBreadcrumbs(_L1,_L2,_L3,_L4);

   // if some breadcrumbs are undefined
   if(!L1 || !L2 || !L3 || !L4)
      returnMe = calcWeightsNoBread(&currHighest);

   // if all breadcrumbs are defined
   else
   {
      returnMe = calcWeights(&currHighest);

      // check if MP is at a dead end, if so, move backwards along the breadcrumbs
      if(currHighest == 0)
      {
         returnMe.clear();
         //returnMe = backTrace();
      }
   }

   return returnMe;
}


//  Initializes all breadcrumbs
void PixelNeighborhood::initAllBreadcrumbs(
   QPoint *_L1,
   QPoint *_L2,
   QPoint *_L3,
   QPoint *_L4
   )
{
   if (_L1)
   {
      if (L1)
      {
         L1->setX(_L1->x());
         L1->setY(_L1->y());
      }
      else
         L1 = new CWeightedPoint(_L1->x(), _L1->y(), "L1");
   }

   if (_L2)
   {
      if (L2)
      {
         L2->setX(_L2->x());
         L2->setY(_L2->y());
      }
      else
         L2 = new CWeightedPoint(_L2->x(), _L2->y(), "L2");
   }

   if (_L3)
   {
      if (L3)
      {
         L3->setX(_L3->x());
         L3->setY(_L3->y());
      }
      else
         L3 = new CWeightedPoint(_L3->x(), _L3->y(), "L3");
   }

   if (_L4)
   {
      if (L4)
      {
         L4->setX(_L4->x());
         L4->setY(_L4->y());
      }
      else
         L4 = new CWeightedPoint(_L4->x(), _L4->y(), "L4");
   }
}


//  Calculate the weights of the neighbourhood, given no breadcrumbs
QList<QString> PixelNeighborhood::calcWeightsNoBread(int *currHighest)
{
   QList<QString> returnMe;

   // check a1
   if (a1->weight >= *currHighest)
   {
      if (a1->weight > *currHighest)
         returnMe.clear();
      *currHighest = a1->weight;
      returnMe << a1->name;
   }

   // check a2
   if (a2->weight >= *currHighest)
   {
      if (a2->weight > *currHighest)
         returnMe.clear();
      *currHighest = a2->weight;
      returnMe << a2->name;
   }

   // check a3
   if (a3->weight >= *currHighest)
   {
      if (a3->weight > *currHighest)
         returnMe.clear();
      *currHighest = a3->weight;
      returnMe << a3->name;
   }

   // check b1
   if (b1->weight >= *currHighest)
   {
      if (b1->weight > *currHighest)
         returnMe.clear();
      *currHighest = b1->weight;
      returnMe << b1->name;
   }

   // check b3
   if (b3->weight >= *currHighest)
   {
      if (b3->weight > *currHighest)
         returnMe.clear();
      *currHighest = b3->weight;
      returnMe << b3->name;
   }

   // check c1
   if (c1->weight >= *currHighest)
   {
      if (c1->weight > *currHighest)
         returnMe.clear();
      *currHighest = c1->weight;
      returnMe << c1->name;
   }

   // check c2
   if (c2->weight >= *currHighest)
   {
      if (c2->weight > *currHighest)
         returnMe.clear();
      *currHighest = c2->weight;
      returnMe << c2->name;
   }

   // check c3
   if (c3->weight >= *currHighest)
   {
      if (c3->weight > *currHighest)
         returnMe.clear();
      *currHighest = c3->weight;
      returnMe << c3->name;
   }

   return returnMe;
}


//  Calculate the weights of the neighbourhood, take breadcrumbs into consideration
QList<QString> PixelNeighborhood::calcWeights(int *currHighest)
{
   QList<QString> returnMe;

   // check a1
   if (
      (a1->weight >= *currHighest) &&
      ((QPoint)*a1 != (QPoint)*L1) &&
      ((QPoint)*a1 != (QPoint)*L2) &&
      ((QPoint)*a1 != (QPoint)*L3) &&
      ((QPoint)*a1 != (QPoint)*L4)
      )
   {
      if (a1->weight > *currHighest)
         returnMe.clear();
      *currHighest = a1->weight;
      returnMe << a1->name;
   }

   // check a2
   if ((a2->weight >= *currHighest) &&
      ((QPoint)*a2 != (QPoint)*L1) &&
      ((QPoint)*a2 != (QPoint)*L2) &&
      ((QPoint)*a2 != (QPoint)*L3) &&
      ((QPoint)*a2 != (QPoint)*L4))
   {
      if (a2->weight > *currHighest)
         returnMe.clear();
      *currHighest = a2->weight;
      returnMe << a2->name;
   }

   // check a3
   if ((a3->weight >= *currHighest) &&
      ((QPoint)*a3 != (QPoint)*L1) &&
      ((QPoint)*a3 != (QPoint)*L2) &&
      ((QPoint)*a3 != (QPoint)*L3) &&
      ((QPoint)*a3 != (QPoint)*L4))
   {
      if (a3->weight > *currHighest)
         returnMe.clear();
      *currHighest = a3->weight;
      returnMe << a3->name;
   }

   // check b1
   if ((b1->weight >= *currHighest) &&
      ((QPoint)*b1 != (QPoint)*L1) &&
      ((QPoint)*b1 != (QPoint)*L2) &&
      ((QPoint)*b1 != (QPoint)*L3) &&
      ((QPoint)*b1 != (QPoint)*L4))
   {
      if (b1->weight > *currHighest)
         returnMe.clear();
      *currHighest = b1->weight;
      returnMe << b1->name;
   }

   // check b3
   if ((b3->weight >= *currHighest) &&
      ((QPoint)*b3 != (QPoint)*L1) &&
      ((QPoint)*b3 != (QPoint)*L2) &&
      ((QPoint)*b3 != (QPoint)*L3) &&
      ((QPoint)*b3 != (QPoint)*L4))
   {
      if (b3->weight > *currHighest)
         returnMe.clear();
      *currHighest = b3->weight;
      returnMe << b3->name;
   }

   // check c1
   if ((c1->weight >= *currHighest) &&
      ((QPoint)*c1 != (QPoint)*L1) &&
      ((QPoint)*c1 != (QPoint)*L2) &&
      ((QPoint)*c1 != (QPoint)*L3) &&
      ((QPoint)*c1 != (QPoint)*L4))
   {
      if (c1->weight > *currHighest)
         returnMe.clear();
      *currHighest = c1->weight;
      returnMe << c1->name;
   }

   // check c2
   if ((c2->weight >= *currHighest) &&
      ((QPoint)*c2 != (QPoint)*L1) &&
      ((QPoint)*c2 != (QPoint)*L2) &&
      ((QPoint)*c2 != (QPoint)*L3) &&
      ((QPoint)*c2 != (QPoint)*L4))
   {
      if (c2->weight > *currHighest)
         returnMe.clear();
      *currHighest = c2->weight;
      returnMe << c2->name;
   }

   // check c3
   if ((c3->weight >= *currHighest) &&
      ((QPoint)*c3 != (QPoint)*L1) &&
      ((QPoint)*c3 != (QPoint)*L2) &&
      ((QPoint)*c3 != (QPoint)*L3) &&
      ((QPoint)*c3 != (QPoint)*L4))
   {
      if (c3->weight > *currHighest)
         returnMe.clear();
      *currHighest = c3->weight;
      returnMe << c3->name;
   }

   return returnMe;
}



//  In some situations, when there are sharp edges the MP can get trapped. There
//  are 3 types of traps within the neighborhood (for each 90 degree rotation)
//
//  trap 1 :
//  ----------------    In this scenario, MP has reached a pixel where there are
//  |XXXX|XXXX|    |    candidates for the next MP, because it has reached a dead end
//  ----------------    where only L1 is visible within the neighborhood. The only
//  | L1 | MP |XXXX|    logical thing to do is to try to move backwards along the last
//  ----------------    visible breadcrumb, which is L1.
//  |XXXX|XXXX|XXXX| 
//  ----------------
//
//  trap 2 :
//  ----------------    This scenario is the same as trap 1, except that L2 is also visible
//  |XXXX|XXXX|    |    in which case the only logical thing to do is to do the same as in trap 1
//  ----------------    except this time the next candidate will be L2, since we already know L1
//  | L1 | MP |XXXX|    leads to a dead end.
//  ---------------- 
//  | L2 |XXXX|XXXX| 
//  ----------------
//
//  trap 3 :
//  ----------------    In scenario 1 and 2, MP gets moved backwards along the breadcrumbs. However
//  |XXXX|XXXX|XXXX|    it is still possible that the MP is trapped, if there is a breadcrumb blocking
//  ----------------    the way. The solution would be to move MP to the OLDEST breadcrumb. This can be
//  | LA | MP |LA-1|    the case if the edge is very sharp and narrow. (4 <= A <= 2). Note that in practice
//  ----------------    the solution is the same as for trap 2.
//  |XXXX|XXXX|XXXX|    
//  ----------------    
//
//  NOTE : the algorithm is at this point only operating with 4 breadcrumbs. If the edge is extremely narrow
//  there is a risk that MP will move backwards where it already has been. TODO
//
QList<QString> PixelNeighborhood::backTrace()
{
   QList<QString> returnMe;

   // L1 always has to be in the neighbourhood, if its not, then something is obviously wrong TODO: notify
   if(!isInNeighborhood(L1))
      return returnMe;

   // check for trap 1
   if(!isInNeighborhood(L2))
   {
      L1 = MP;
      returnMe << "L1";
   }

   // check for trap 2/3
   else
   {
      L1 = MP;
      if(isInNeighborhood(L3))
      {
         L3 = L2;
         L2 = L1;
         returnMe << "L3";
      }
      else if(isInNeighborhood(L4))
      {
         L4 = L3;
         L3 = L2;
         L2 = L1;
         returnMe << "L4";
      }
      else
      {
         returnMe << "L2";
      }
   }
   return returnMe;
}


//  Detect the number of holes in the neighborhood
int PixelNeighborhood::holeCount()
{
   int holes = 0;

   if(a1->colorDiff == false && !isBreadcrumb(a1))
      holes++;
   if(a2->colorDiff == false && !isBreadcrumb(a2))
      holes++;
   if(a3->colorDiff == false && !isBreadcrumb(a3))
      holes++;
   if(b1->colorDiff == false && !isBreadcrumb(b1))
      holes++;
   if(b3->colorDiff == false && !isBreadcrumb(b3))
      holes++;
   if(c1->colorDiff == false && !isBreadcrumb(c1))
      holes++;
   if(c2->colorDiff == false && !isBreadcrumb(c2))
      holes++;
   if(c3->colorDiff == false && !isBreadcrumb(c3))
      holes++;
   
   return holes;
}


//  Check if a point is a breadcrumb
bool PixelNeighborhood::isBreadcrumb(const CWeightedPoint *point)
{
   if(*point == *L1) return true;
   if(*point == *L2) return true;
   if(*point == *L3) return true;
   if(*point == *L4) return true;
   return false;
}


//  Check if argument point is within the current neighborhood
bool PixelNeighborhood::isInNeighborhood(const CWeightedPoint *pt)
{
   if(MP == pt) 
      return true;
   else if((QPoint)*a1 == (QPoint)*pt) 
      return true;
   else if((QPoint)*a2 == (QPoint)*pt) 
      return true;
   else if((QPoint)*a3 == (QPoint)*pt) 
      return true;
   else if((QPoint)*b1 == (QPoint)*pt) 
      return true;
   else if((QPoint)*b3 == (QPoint)*pt) 
      return true;
   else if((QPoint)*c1 == (QPoint)*pt) 
      return true;
   else if((QPoint)*c2 == (QPoint)*pt) 
      return true;
   else if((QPoint)*c3 == (QPoint)*pt) 
      return true;
   else return false;
}


//  Return the weighted point that corresponds to the argument name
CWeightedPoint * PixelNeighborhood::getPoint(const QString &_name)
{
   if(MP && MP->name == _name) 
      return MP;
   else if(a1 && a1->name == _name) 
      return a1;
   else if(a2 && a2->name == _name) 
      return a2;
   else if(a3 && a3->name == _name) 
      return a3;
   else if(b1 && b1->name == _name) 
      return b1;
   else if(b3 && b3->name == _name) 
      return b3;
   else if(c1 && c1->name == _name) 
      return c1;
   else if(c2 && c2->name == _name) 
      return c2;
   else if(c3 && c3->name == _name) 
      return c3;
   else if(L1 && L1->name == _name) 
      return L1;
   else if(L2 && L2->name == _name) 
      return L2;
   else if(L3 && L3->name == _name) 
      return L3;
   else if(L4 && c3->name == _name) 
      return L4;
   else return 0;
}


const QImage * PixelNeighborhood::getImg()
{
   return img;
}

QRgb * PixelNeighborhood::getColor()
{
   return startColor;
}

CWeightedPoint * PixelNeighborhood::getMP()
{ 
   return MP; 
}
CWeightedPoint * PixelNeighborhood::geta1()
{ 
   return a1; 
}
CWeightedPoint * PixelNeighborhood::geta2()
{ 
   return a2; 
}
CWeightedPoint * PixelNeighborhood::geta3()
{ 
   return a3; 
}
CWeightedPoint * PixelNeighborhood::getb1()
{ 
   return b1; 
}
CWeightedPoint * PixelNeighborhood::getb3()
{ 
   return b3; 
}
CWeightedPoint * PixelNeighborhood::getc1()
{ 
   return c1; 
}
CWeightedPoint * PixelNeighborhood::getc2()
{ 
   return c2; 
}
CWeightedPoint * PixelNeighborhood::getc3()
{ 
   return c3; 
}
CWeightedPoint * PixelNeighborhood::getL1()
{ 
   return L1; 
}
CWeightedPoint * PixelNeighborhood::getL2()
{ 
   return L2; 
}
CWeightedPoint * PixelNeighborhood::getL3()
{ 
   return L3; 
}
CWeightedPoint * PixelNeighborhood::getL4()
{ 
   return L4; 
}

//  Set private members. Can be set either by giving an argument pointer to a weighted point
//  OR by specifying new coordinates, in which case the point must exist. Weight is reset to 0
void PixelNeighborhood::setMP(CWeightedPoint *wpnt, int x, int y)
{
   if (wpnt != 0)
   {
      MP = wpnt;
      MP->weight = 0;
      currX = wpnt->x();
      currY = wpnt->y();
   }
   else
   {
      if (MP != 0 && x > -1 && y > -1)
      {
         currX = x;
         currY = y;
         MP->weight = 0;
         MP->setX(currX);
         MP->setY(currY);
      }
   }
}

void PixelNeighborhood::seta1(CWeightedPoint *wpnt, int x, int y)
{
   if (wpnt != 0)
   {
      a1 = wpnt;
      a1->weight = 0;
      a1x = wpnt->x();
      a1y = wpnt->y();
   }
   else
   {
      if (a1 != 0 && x > -1 && y > -1)
      {
         a1x = x;
         a1y = y;
         a1->weight = 0;
         a1->setX(a1x);
         a1->setY(a1y);
      }
   }
}

void PixelNeighborhood::seta2(CWeightedPoint *wpnt, int x, int y)
{
   if (wpnt != 0)
   {
      a2 = wpnt;
      a2->weight = 0;
      a2x = wpnt->x();
      a2y = wpnt->y();
   }
   else
   {
      if (a2 != 0 && x > -1 && y > -1)
      {
         a2x = x;
         a2y = y;
         a2->weight = 0;
         a2->setX(a2x);
         a2->setY(a2y);
      }
   }
}

void PixelNeighborhood::seta3(CWeightedPoint *wpnt, int x, int y)
{
   if (wpnt != 0)
   {
      a3 = wpnt;
      a3->weight = 0;
      a3x = wpnt->x();
      a3y = wpnt->y();
   }
   else
   {
      if (a3 != 0 && x > -1 && y > -1)
      {
         a3x = x;
         a3y = y;
         a3->weight = 0;
         a3->setX(a3x);
         a3->setY(a3y);
      }
   }
}

void PixelNeighborhood::setb1(CWeightedPoint *wpnt, int x, int y)
{
   if (wpnt != 0)
   {
      b1 = wpnt;
      b1->weight = 0;
      b1x = wpnt->x();
      b1y = wpnt->y();
   }
   else
   {
      if (b1 != 0 && x > -1 && y > -1)
      {
         b1x = x;
         b1y = y;
         b1->weight = 0;
         b1->setX(b1x);
         b1->setY(b1y);
      }
   }
}

void PixelNeighborhood::setb3(CWeightedPoint *wpnt, int x, int y)
{
   if (wpnt != 0)
   {
      b3 = wpnt;
      b3->weight = 0;
      b3x = wpnt->x();
      b3y = wpnt->y();
   }
   else
   {
      if (b3 != 0 && x > -1 && y > -1)
      {
         b3x = x;
         b3y = y;
         b3->weight = 0;
         b3->setX(b3x);
         b3->setY(b3y);
      }
   }
}

void PixelNeighborhood::setc1(CWeightedPoint *wpnt, int x, int y)
{
   if (wpnt != 0)
   {
      c1 = wpnt;
      c1->weight = 0;
      c1x = wpnt->x();
      c1y = wpnt->y();
   }
   else
   {
      if (c1 != 0 && x > -1 && y > -1)
      {
         c1x = x;
         c1y = y;
         c1->weight = 0;
         c1->setX(c1x);
         c1->setY(c1y);
      }
   }
}

void PixelNeighborhood::setc2(CWeightedPoint *wpnt, int x, int y)
{
   if (wpnt != 0)
   {
      c2 = wpnt;
      c2->weight = 0;
      c2x = wpnt->x();
      c2y = wpnt->y();
   }
   else
   {
      if (c2 != 0 && x > -1 && y > -1)
      {
         c2x = x;
         c2y = y;
         c2->weight = 0;
         c2->setX(c2x);
         c2->setY(c2y);
      }
   }
}

void PixelNeighborhood::setc3(CWeightedPoint *wpnt, int x, int y)
{
   if (wpnt != 0)
   {
      c3 = wpnt;
      c3->weight = 0;
      c3x = wpnt->x();
      c3y = wpnt->y();
   }
   else
   {
      if (c3 != 0 && x > -1 && y > -1)
      {
         c3x = x;
         c3y = y;
         c3->weight = 0;
         c3->setX(c3x);
         c3->setY(c3y);
      }
   }
}

void PixelNeighborhood::setL1(CWeightedPoint *wpnt, int x, int y)
{
   if (wpnt != 0)
   {
      L1 = wpnt;
   }
   else
   {
      if (L1 != 0 && x > -1 && y > -1)
      {
         L1->setX(x);
         L1->setY(y);
      }
   }
}

void PixelNeighborhood::setL2(CWeightedPoint *wpnt, int x, int y)
{
   if (wpnt != 0)
   {
      L2 = wpnt;
   }
   else
   {
      if (L2 != 0 && x > -1 && y > -1)
      {
         L2->setX(x);
         L2->setY(y);
      }
   }
}

void PixelNeighborhood::setL3(CWeightedPoint *wpnt, int x, int y)
{
   if (wpnt != 0)
   {
      L3 = wpnt;
   }
   else
   {
      if (L3 != 0 && x > -1 && y > -1)
      {
         L3->setX(x);
         L3->setY(y);
      }
   }
}

void PixelNeighborhood::setL4(CWeightedPoint *wpnt, int x, int y)
{
   if (wpnt != 0)
   {
      L4 = wpnt;
   }
   else
   {
      if (L4 != 0 && x > -1 && y > -1)
      {
         L4->setX(x);
         L4->setY(y);
      }
   }
}