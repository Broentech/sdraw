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
//  \file      selectionBase.cpp
//
//  \brief     Implementation
//
//  \author    Stian Broen
//
//  \date      30.12.2011
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 30.12.2011 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QState>
#include <QFinalState>
#include <QEventTransition>

// local includes
#include "selectionBase.h"

using namespace sdraw;

CSelectionBase::CSelectionBase(QObject *parent)
   :QObject(parent),
   hasBuffer(false),
   currentState(SEL_STATE_ACTIVE),
   bottomPixmap(0),
   content(0),
   topContent(0),
   worldShadow(0),
   extraPixmap(0),
   hasShadow(false),
   writeExtraInstead(false),
   accumRotation_x(0),
   accumRotation_y(0),
   accumRotation_z(0)
{
   
}

CSelectionBase::~CSelectionBase()
{
   delete bottomPixmap;
   content = 0;
   delete topContent;
   delete worldShadow;
   delete extraPixmap;
}

void CSelectionBase::registerBuffer(QPixmap *buffer)
{
   if(0 != buffer)
   {
      content = buffer;
      hasBuffer = true;
   }
   else
   {
      releaseBuffer();
   }
}

void CSelectionBase::releaseBuffer()
{
   content = 0;
   hasBuffer = false;
}

bool CSelectionBase::getHasBuffer()
{
   if(0 == content)
   {
      hasBuffer = false;
   }
   else
   {
      hasBuffer = true;
   }
   return hasBuffer;
}

bool CSelectionBase::setState(ESelectionStates newState)
{
   bool returnMe = false;
   switch(newState)
   {
   case(SEL_STATE_PASSIVE):
      returnMe = enterPassiveState();
      break;

   case(SEL_STATE_ACTIVE):
      returnMe = enterActiveState();
      break;

   case(SEL_STATE_MOVING):
      returnMe = enterMovingState();
      break;

   case(SEL_STATE_PROCESSING):
      returnMe = enterProcessingState();
      break;

   case(SEL_STATE_EXTENDING):
      returnMe = enterExtendingState();

   default:
      returnMe = false;
      break;
   }

   return returnMe;
}

bool CSelectionBase::enterPassiveState()
{
   bool returnMe = false;
   switch(currentState)
   {
   case(SEL_STATE_PASSIVE):
      // transition into same state
      break;

   case(SEL_STATE_ACTIVE):
      // transition from active state to passive state
      currentState = SEL_STATE_PASSIVE;
      returnMe = true;
      break;

   case(SEL_STATE_MOVING):
      // transition from moving to passive, not allowed
      break;

   case(SEL_STATE_PROCESSING):
      // transition from processing to passive, not allowed
      break;

   case(SEL_STATE_EXTENDING):
      // transition from extending into passive, not allowed
      break;

   default:
      break;
   }

   return returnMe;
}

bool CSelectionBase::enterActiveState()
{
   bool returnMe = false;
   switch(currentState)
   {
   case(SEL_STATE_PASSIVE):
      // transition from passive to active
      currentState = SEL_STATE_ACTIVE;
      returnMe = true;
      break;

   case(SEL_STATE_ACTIVE):
      // transition into same state
      break;

   case(SEL_STATE_MOVING):
      // transition from moving to active (movement ended)
      currentState = SEL_STATE_ACTIVE;
      returnMe = true;
      break;

   case(SEL_STATE_PROCESSING):
      // transition from processing to active (processing ended)
      currentState = SEL_STATE_ACTIVE;
      returnMe = true;
      break;

   case(SEL_STATE_EXTENDING):
      // transition from extending into active
      currentState = SEL_STATE_ACTIVE;
      returnMe = true;
      break;

   default:
      break;
   }

   return returnMe;
}

bool CSelectionBase::enterMovingState()
{
   bool returnMe = false;
   switch(currentState)
   {
   case(SEL_STATE_PASSIVE):
      // transition from passive to moving, not allowed
      break;

   case(SEL_STATE_ACTIVE):
      // transition from active to moving
      currentState = SEL_STATE_MOVING;
      returnMe = true;
      break;

   case(SEL_STATE_MOVING):
      // transition into same state
      break;

   case(SEL_STATE_PROCESSING):
      // transition from processing to moving, not allowed
      break;

   case(SEL_STATE_EXTENDING):
      // transition from extending into moving, not allowed
      break;

   default:
      break;
   }

   return returnMe;
}

bool CSelectionBase::enterProcessingState()
{
   bool returnMe = false;
   switch(currentState)
   {
   case(SEL_STATE_PASSIVE):
      // transition from passive to processing, not allowed
      break;

   case(SEL_STATE_ACTIVE):
      // transition from active to processing
      currentState = SEL_STATE_PROCESSING;
      returnMe = true;
      break;

   case(SEL_STATE_MOVING):
      // transition from moving to processing, not allowed
      break;

   case(SEL_STATE_PROCESSING):
      // transition into same state
      break;

   case(SEL_STATE_EXTENDING):
      // transition from extending into processing, not allowed
      break;

   default:
      break;
   }

   return returnMe;
}

bool CSelectionBase::enterExtendingState()
{
   bool returnMe = false;
   switch(currentState)
   {
   case(SEL_STATE_PASSIVE):
      // transition from passive to extending, not allowed
      break;

   case(SEL_STATE_ACTIVE):
      // transition from active to extending
      currentState = SEL_STATE_EXTENDING;
      returnMe = true;
      break;

   case(SEL_STATE_MOVING):
      // transition from moving to extending, not allowed
      break;

   case(SEL_STATE_PROCESSING):
      // transition from processing into extending, not allowed
      break;
      
   case(SEL_STATE_EXTENDING):
      // transition into same state
      break;

   default:
      break;
   }

   return returnMe;
}
