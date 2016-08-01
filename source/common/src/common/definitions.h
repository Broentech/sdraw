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
//  \file      definitions.h
//
//  \brief     Definitions for the 2D painter. Contains all GUI parameters
//
//  \author    Stian Broen
//
//  \date      16.06.2011
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 16.06.2011 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define PI 3.14159265

#include <QRect>

// definition of the different available operation modes
typedef enum TOperation
{
   DRAW        ,
   POLYGONIZER ,
   FILLER      ,
   SELECTER    ,
   TEXT        ,
   INSPECTER   ,
};
#define default_OPERATION TOperation::DRAW

// enum to define selection
typedef enum TSelectionMode
{
   RECTANGULAR_SELECTION ,
   POLYGON_SELECTION     ,
   FREE_SELECTION        ,
};
#define default_SELECTIONMODE RECTANGULAR_SELECTION

// the quadrants of a circle
typedef enum 
{
   undefined ,
   first     ,
   second    ,
   third     ,
   fourth
}Quadrant;

// enum to define action to perform on selection
typedef enum TSelectionAction
{
   COPY_ACTION   ,
   CUT_ACTION    ,
   PASTE_ACTION  ,
   DELETE_ACTION ,
   CANCEL_ACTION ,
};

// enum to define selection action option
typedef enum TSelectionOption
{
   IGNOREWHITE_SELECTION_ON  ,
   IGNOREWHITE_SELECTION_OFF ,
   INVERSE_SELECTION_ON      ,
   INVERSE_SELECTION_OFF     ,
   CLONE_SELECTION_ON        ,
   CLONE_SELECTION_OFF       ,
   COMBINE_SELECTION_ON      ,
   COMBINE_SELECTION_OFF     ,  
};

// directional definitions
typedef enum TDirection
{
   UPWARDS                 ,
   DOWNWARDS               ,
   LEFT                    ,
   RIGHT                   ,
   FROMUPRIGHTWARDSTODOWN  ,
   FROMUPLEFTWARDSTODOWN   ,
   FROMDOWNRIGHTWARDSTOUP  ,
   FROMDOWNLEFTWARDSTODOUP ,
};

// selection dragbox definitions
typedef enum TDragArea
{
   NO_DRAG          ,
   SELECTION_DRAG   ,
   UPPER_LEFT_DRAG  ,
   UPPER_MID_DRAG   ,
   UPPER_RIGHT_DRAG ,
   MID_LEFT_DRAG    ,
   MID_RIGHT_DRAG   ,
   BOTTOM_LEFT_DRAG ,
   BOTTOM_MID_DRAG  ,
   BOTTOM_RIGHT_DRAG,
};

// selection polygon dragbox definitions
typedef enum TPolygonDragArea
{
   POLY_NOT_DEFINED    , // case when there is no point
   POLY_NO_DRAG        , // case when a point is outside a polygon
   POLY_SELECTION_DRAG , // case when a point is inside a polygon
   POLY_BOX_DRAG       , // case when a point is inside a polygon dragbox
};

typedef enum TTransformationMode
{
   TRANS_NO,
   TRANS_MOVE,
   TRANS_SCALE,
   TRANS_ROTATE,
   TRANS_SHEAR,
};
#define default_TRANSFORMATION TRANS_MOVE

// rotational definitions
typedef enum TRotation
{
   // AXIS_DIRECTION
   X_UP               ,
   X_DOWN             ,
   Y_LEFT             ,
   Y_RIGHT            ,
   Z_CLOCKWISE        ,
   Z_COUNTERCLOCKWISE ,
};

// shape type definitions
typedef enum shapeType
{
   SNoShape,
   SLine,
   SRectangle,
   SRoundedRectangle,
   SEllipse,
   SArc,
   SPie,
   SChord,
   SPolyline,
   SPolygon
};

// definition pen default values
#define pen_MAX_WIDTH     40
#define pen_MIN_WIDTH      1
#define pen_DEFAULT_WIDTH  1
#define pen_DEFAULT_STYLE Qt::SolidLine
#define pen_DEFAULT_CAP   Qt::RoundCap
#define pen_DEFAULT_JOIN  Qt::MiterJoin

// definition brush default values
#define brush_DEFAULT_PATTERN   Qt::SolidPattern
#define gradient_DEFAULT_SPREAD QGradient::PadSpread

#define default_BRUSH_BUTTON_WIDTH  100
#define default_BRUSH_BUTTON_HEIGHT 60

// definition for tooltype selection
#define default_OPERATION TOperation::DRAW
#define default_TOOLTYPESELECTOR_WIDTH  100
#define default_TOOLTYPESELECTOR_HEIGHT  70

// shape button size
#define default_SHAPE_BUTTON_WIDTH        100
#define default_SHAPE_BUTTON_HEIGHT        60
#define default_SHAPE_BUTTON_WIDTH_SMALL   40
#define default_SHAPE_BUTTON_HEIGHT_SMALL  40

#define default_PEN_BUTTON_WIDTH  100
#define default_PEN_BUTTON_HEIGHT 60
#define default_MAX_PEN_WIDTH     30
#define default_MIN_PEN_WIDTH     1

// class containing settings, initialy all settings are set to OFF/false
class CSettings
{

private:
   bool antialiasing;

public:
   CSettings()
   {
      antialiasing = false;
   }
   bool getAntialiasing()
   {
      return antialiasing;
   }
   void setAntialiasing(bool onOrOff)
   {
      antialiasing = onOrOff;
   }
};

struct TSelection
{
   QPoint outsideUpperLeft;
   QPoint outsideBottomRight;
   QRect rectangularSelectionErase;
   QRect rectangularSelection;
   QRect contentRect;
   QRect upperLeftCornerDragArea;
   QRect upperMidDragArea;
   QRect upperRightDragArea;
   QRect midLeftDragArea;
   QRect midRightDragArea;
   QRect bottomLeftDragArea;
   QRect bottomMidDragArea;
   QRect bottomRightDragArea;
};

enum ESelectionStates
{
   SEL_STATE_PASSIVE    ,
   SEL_STATE_ACTIVE     ,
   SEL_STATE_MOVING     ,
   SEL_STATE_EXTENDING  ,
   SEL_STATE_PROCESSING ,
};

enum EPolygonConversion
{
   NO_CONVERSION,
   TO_LINES,
   TO_RECT,
   TO_ROUNDED_RECT,
   TO_ELLIPSE,
   TO_PIE,
   TO_ARC,
   TO_CHORD,
};

#endif