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
//  \file      selectionBase.h
//
//  \brief     Definition of a class that defines the bases of any selection. It contains
//             a "finite state machine" with a defined set of states and transition between
//             states.
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

#ifndef SELECTIONBASE_H
#define SELECTIONBASE_H

// Qt includes
#include <QTransform>
#include <QObject>
#include <QPen>

// solution includes
#include <Common/definitions.h>

// Qt forward declarations
class QPixmap;

namespace sdraw {
   class CSelectionBase : public QObject
   {
      Q_OBJECT

   public:
      CSelectionBase(QObject *parent = 0);
      ~CSelectionBase();
      void registerBuffer(QPixmap *buffer);
      void releaseBuffer();
      bool getHasBuffer();
      bool setState(ESelectionStates newState);
      inline bool getHasExtraContent() { return writeExtraInstead; }
      inline QPen getCurrentPen() { return m_pen; }
      inline ESelectionStates getCurrentState() { return currentState; }
      inline QPixmap *getShadowPixmap() { return worldShadow; }
      inline QPixmap *getFullPixmap() { return bottomPixmap; }
      inline QPixmap *getContentPixmap() { return content; }
      inline QPixmap *getExtraPixmap() { return extraPixmap; }
      inline QPixmap *getTopContent() { return topContent; }
      inline void setTransform(QTransform trans) { transform = trans; }
      inline void setLastTransform(QTransform trans) { lastTransform = trans; }
      inline QTransform getTransform() { return transform; }
      inline QTransform getLastTransform() { return lastTransform; }
      inline qreal getAccumRotation_x(){ return accumRotation_x; }
      inline qreal getAccumRotation_y(){ return accumRotation_y; }
      inline qreal getAccumRotation_z(){ return accumRotation_z; }
      inline void setAccumRotation_x(qreal num){ accumRotation_x = num; }
      inline void setAccumRotation_y(qreal num){ accumRotation_y = num; }
      inline void setAccumRotation_z(qreal num){ accumRotation_z = num; }
      inline void deleteTransforms() { transform = QTransform(0, 0, 0, 0, 0, 0, 0, 0, 0); lastTransform = QTransform(0, 0, 0, 0, 0, 0, 0, 0, 0); }

   protected:
      bool enterPassiveState();
      bool enterActiveState();
      bool enterMovingState();
      bool enterProcessingState();
      bool enterExtendingState();

   protected:
      QTransform transform;
      QTransform lastTransform;
      qreal accumRotation_x;
      qreal accumRotation_y;
      qreal accumRotation_z;
      ESelectionStates currentState;
      QPen m_pen;
      QPixmap *bottomPixmap;
      QPixmap *content;
      QPixmap *topContent;
      QPixmap *worldShadow;
      QPixmap *extraPixmap;
      bool hasBuffer;
      bool hasShadow;
      bool writeExtraInstead;
      QRect worldSelectionRect_withBoxes;
      QRect worldSelectionRect;
      QRect worldContentRect;
      QRect localSelectionRect_withBoxes;
      QRect localSelectionRect;
      QRect localContentRect;
   };
}

#endif