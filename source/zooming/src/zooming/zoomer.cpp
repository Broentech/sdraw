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
//  \file      zoomer.cpp
//
//  \brief     Implementation of class that handles sdraw on QRect
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

// Qt includes
#include <QPainter>
#include <QWheelEvent>
#include <QtCore/qmath.h>

// local includes
#include "zoomer.h"

using namespace sdraw;

Zoomer::Zoomer( QObject* parent )
    : QObject( parent ),
    m_zoomParameters(0)
{

}

Zoomer::~Zoomer()
{

}

void Zoomer::addSourceRect(QRect source)
{
   if(!source.isEmpty())
   {
      m_zoomSources.clear();
      m_zoomSources << source;
      if (0 != m_zoomParameters)
      {
         delete m_zoomParameters;
         m_zoomParameters = 0;
      }
      m_zoomParameters = new ZoomParams;
      qreal ratio = (qreal)source.width() / (qreal)source.height();
      int minY = 40;
      int minX = qRound(qreal(minY * ratio));
      m_zoomParameters->setminSource(QSize(minX, minY));
      m_zoomParameters->setminTarget(QSize(minX, minY));
      m_zoomParameters->setzoomSourceRect(source);
      m_zoomParameters->setzoomTargetRect(source);
      performZooming();
   }
}

bool Zoomer::getZoomGeo(QRect *source, QRect *target)
{
   if (0 == source || 0 == target)
   {
      return false;
   }

   QRect returnSource = m_zoomParameters->zoomSourceRect();
   int mPanX = m_zoomParameters->panX();
   int mPanY = m_zoomParameters->panY();
   if (abs(mPanX) > 0 || abs(mPanY) > 0)
   {
      returnSource.adjust(
         -mPanX,
         -mPanY,
         -mPanX,
         -mPanY
         );
   }
   QRect returnTarget = m_zoomParameters->zoomTargetRect();
   *source = returnSource;
   *target = returnTarget;
   return true;
}

void Zoomer::clearSourceRect()
{
   m_zoomSources.clear();
   if (0 != m_zoomParameters)
   {
      delete m_zoomParameters;
      m_zoomParameters = 0;
   }
}

void Zoomer::removeZoomModifier(Qt::KeyboardModifiers mod)
{
   m_mods.removeAll(mod);
}

void Zoomer::addZoomModifier(Qt::KeyboardModifiers mod)
{
   m_mods << mod;
}

void Zoomer::wheelEvent(QWheelEvent *event)
{
   int m_delta = event->delta();
   QPoint currPt = event->pos();
   Qt::KeyboardModifiers mod = event->modifiers();
   if(m_mods.isEmpty() || m_mods.contains(mod))
   {
      setZoom(m_delta, &currPt);
   }
}

void Zoomer::setZoom(int amount, QPoint *mousePos)
{
   if (m_zoomSources.size() < 1)
   {
      return;
   }
   if (0 != mousePos)
   {
      m_zoomParameters->setzoomCenter(*mousePos);
   }
   else
   {
      m_zoomParameters->setzoomCenter(QPoint(
         qRound((qreal)m_zoomSources.at(0).width() / 2.0),
         qRound((qreal)m_zoomSources.at(0).height() / 2.0))
         );
   }
   m_zoomParameters->setcurrentZoom(m_zoomParameters->currentZoom() + amount);
   m_zoomParameters->setzoomAmount(amount);
   m_zoomParameters->setzoomSwitch(true);
   performZooming();
}

bool Zoomer::performZooming()
{
   if (m_zoomSources.size() < 1)
   {
      return false;
   }

   if (m_zoomParameters->zoomAmount() != 0 ||
      m_zoomSources.size() > 1 ||
      (m_zoomParameters->cumulativeZoomInWidth() != 0 &&
      m_zoomParameters->cumulativeZoomInHeight() != 0))
   {
      QPoint origZoomCenter = m_zoomParameters->zoomCenter();
      if (m_zoomParameters->zoomSwitch() == true)
      {
         m_zoomParameters->setzoomAmount(qRound((qreal)m_zoomParameters->zoomAmount() / 10.0));
         m_zoomParameters->setzoomSwitch(false);
         m_zoomParameters->setzoomSourceRect(m_zoomSources.at(0));
         m_zoomParameters->setzoomedRect(m_zoomParameters->zoomSourceRect());
         m_zoomParameters->setwidthZoom(m_zoomParameters->zoomAmount());
         qreal ratio = (qreal)m_zoomParameters->zoomSourceRect().width() /
            (qreal)m_zoomParameters->zoomSourceRect().height();
         m_zoomParameters->setheightZoom(qRound((qreal)m_zoomParameters->widthZoom() / ratio));

         if (m_zoomParameters->zoomAmount() > 0) // zoom in
         {
            if (m_zoomParameters->cumulativeZoomInWidth() < 0 &&
               m_zoomParameters->cumulativeZoomInHeight() < 0)
            {
               if (!zoomInFromZoomedOut())
               {
                  return false;
               }
            }
            else
            {
               if (!zoomInFromZoomedIn())
               {
                  return false;
               }
            }
         }
         else // zoom out
         {
            if (m_zoomSources.size() > 1)
            {
               if (!zoomOutFromZoomedIn())
               {
                  return false;
               }
            }
            else
            {
               if (!zoomOutFromZoomedOut())
               {
                  return false;
               }
            }
         }
      }
   }
   else
   {
      return false;
   }

   return true;
}

bool Zoomer::zoomInFromZoomedIn()
{
   QRect localRect = m_zoomParameters->zoomSourceRect().adjusted(
      0, 
      0, 
      -m_zoomParameters->widthZoom(),
      -m_zoomParameters->heightZoom()
      );

   m_zoomParameters->setzoomedRect(localRect);

   QSize  backTracedSize = backTrackSize(
      localRect.size(),
      m_zoomSources.size() - 1
      );

   bool skipSource = false;
   if (backTracedSize.width() < m_zoomParameters->minSource().width() ||
      backTracedSize.height() < m_zoomParameters->minSource().height())
   {
      skipSource = true;
      backTracedSize = m_zoomParameters->minSource();
   }

   QPoint newZoomCorner = mapToZoomedInRect_upperLeftCorner(
      m_zoomParameters->zoomCenter(),
      backTracedSize
      );
   m_zoomParameters->setzoomCorner(newZoomCorner);

   if (m_zoomParameters->zoomCorner().x() < 0 ||
      m_zoomParameters->zoomCorner().y() < 0)
   {
      return false;
   }

   int zoomedRectWidth = backTracedSize.width() - 1;
   int zoomedRectHeight = backTracedSize.height() - 1;
   int brX = m_zoomParameters->zoomCorner().x() + zoomedRectWidth;
   int brY = m_zoomParameters->zoomCorner().y() + zoomedRectHeight;
   QPoint br(brX, brY);

   QRect newTarget = m_zoomSources.at(0);
   QRect newSource = QRect(m_zoomParameters->zoomCorner(), br);
   m_zoomParameters->setzoomTargetRect(newTarget);
   m_zoomParameters->setzoomSourceRect(newSource);

   if (!skipSource)
   {
      m_zoomSources << m_zoomParameters->zoomSourceRect();
   }
   return true;
}

bool Zoomer::zoomInFromZoomedOut()
{
   if ((m_zoomParameters->cumulativeZoomInWidth() + m_zoomParameters->widthZoom()) > 0 ||
      (m_zoomParameters->cumulativeZoomInHeight() + m_zoomParameters->heightZoom()) > 0)
   {
      return false;
   }
   m_zoomParameters->setcumulativeZoomInWidth(m_zoomParameters->cumulativeZoomInWidth() + m_zoomParameters->widthZoom());
   m_zoomParameters->setcumulativeZoomInHeight(m_zoomParameters->cumulativeZoomInHeight() + m_zoomParameters->heightZoom());
   int dX = qRound((qreal)-m_zoomParameters->cumulativeZoomInWidth() / 2.0);
   int dY = qRound((qreal)-m_zoomParameters->cumulativeZoomInHeight() / 2.0);
   m_zoomParameters->setzoomSourceRect(m_zoomSources.at(0));
   m_zoomParameters->setcentralizedOffset_width(dX);
   m_zoomParameters->setcentralizedOffset_height(dY);

   QRect nextTarget = m_zoomSources.at(0).adjusted(
      dX, 
      dY, 
      m_zoomParameters->cumulativeZoomInWidth() + dX,
      m_zoomParameters->cumulativeZoomInHeight() + dY
      );

   m_zoomParameters->setzoomTargetRect(nextTarget);

   return true;
}

bool Zoomer::zoomOutFromZoomedIn()
{
   m_zoomSources.removeLast();

   QSize  backTracedSize = backTrackSize(
      m_zoomParameters->zoomedRect().size(),
      m_zoomSources.size() - 1
      );

   m_zoomParameters->setzoomCorner(
      mapToZoomedInRect_upperLeftCorner(
      m_zoomParameters->zoomCenter(),
      backTracedSize)
      );

   if (m_zoomParameters->zoomCorner().x() < 0 ||
      m_zoomParameters->zoomCorner().y() < 0)
   {
      return false;
   }

   int zoomedRectWidth = backTracedSize.width() - 1;
   int zoomedRectHeight = backTracedSize.height() - 1;
   int brX = m_zoomParameters->zoomCorner().x() + zoomedRectWidth;
   int brY = m_zoomParameters->zoomCorner().y() + zoomedRectHeight;
   QPoint br(brX, brY);
   m_zoomParameters->setzoomSourceRect(QRect(m_zoomParameters->zoomCorner(), br));
   m_zoomParameters->setzoomTargetRect(m_zoomSources.at(0));
   return true;
}

bool Zoomer::zoomOutFromZoomedOut()
{
   m_zoomParameters->setcumulativeZoomInWidth(m_zoomParameters->cumulativeZoomInWidth() + m_zoomParameters->widthZoom());
   m_zoomParameters->setcumulativeZoomInHeight(m_zoomParameters->cumulativeZoomInHeight() + m_zoomParameters->heightZoom());
   int dX = qRound(-(qreal)m_zoomParameters->cumulativeZoomInWidth() / 2.0);
   int dY = qRound(-(qreal)m_zoomParameters->cumulativeZoomInHeight() / 2.0);
   m_zoomParameters->setcentralizedOffset_width(dX);
   m_zoomParameters->setcentralizedOffset_height(dY);

   QRect nextTarget = m_zoomSources.at(0).adjusted(
      dX, 
      dY, 
      m_zoomParameters->cumulativeZoomInWidth() + dX,
      m_zoomParameters->cumulativeZoomInHeight() + dY
      );

   if (nextTarget.size().width() <= m_zoomParameters->minTarget().width() ||
      nextTarget.size().height() <= m_zoomParameters->minTarget().height())
   {
      int cwz = qRound(-(qreal)(m_zoomSources.at(0).width()) / 2.0) +
         qRound((qreal)m_zoomParameters->minTarget().width() / 2.0) + 1;
      int chz = qRound(-(qreal)(m_zoomSources.at(0).height()) / 2.0) +
         qRound((qreal)m_zoomParameters->minTarget().height() / 2.0) + 1;

      m_zoomParameters->setcentralizedOffset_width(-cwz);
      m_zoomParameters->setcentralizedOffset_height(-chz);
      m_zoomParameters->setcumulativeZoomInWidth(m_zoomParameters->cumulativeZoomInWidth() - m_zoomParameters->widthZoom());
      m_zoomParameters->setcumulativeZoomInHeight(m_zoomParameters->cumulativeZoomInHeight() - m_zoomParameters->heightZoom());

      int blY = m_zoomParameters->minTarget().width() + m_zoomParameters->centralizedOffset_width();
      int blX = m_zoomParameters->minTarget().height() + m_zoomParameters->centralizedOffset_height();
      QPoint tl(
         m_zoomParameters->centralizedOffset_width(),
         m_zoomParameters->centralizedOffset_height()
         );
      QPoint br(blY, blX);
      QRect newTarget = QRect(tl, br);
      m_zoomParameters->setzoomTargetRect(newTarget);

   }
   else
   {
      m_zoomParameters->setzoomTargetRect(nextTarget);
   }
   m_zoomParameters->setzoomSourceRect(m_zoomSources.at(0));
   return true;
}

QPoint Zoomer::getBufferPoint(const QPoint &pt, bool panAdjusted, bool strict)
{
   if ((m_zoomParameters->cumulativeZoomInHeight() == 0 &&
      m_zoomParameters->cumulativeZoomInWidth() == 0 &&
      m_zoomSources.size() < 1))
   {
      return pt;
   }

   QPoint bufferPoint;
   int x1 = pt.x();
   int y1 = pt.y();
   int w1 = m_zoomSources.at(0).width();
   int h1 = m_zoomSources.at(0).height();
   int w2 = 0;
   int h2 = 0;
   int x2 = 0;
   int y2 = 0;
   int offsetX = 0;
   int offsetY = 0;
   qreal scaleX = 0;
   qreal scaleY = 0;

   if (m_zoomSources.size() > 1)
   {
      offsetX = m_zoomParameters->zoomCorner().x();
      offsetY = m_zoomParameters->zoomCorner().y();
      w2 = m_zoomParameters->zoomSourceRect().width();
      h2 = m_zoomParameters->zoomSourceRect().height();
      x2 = qRound((qreal)(x1*w2) / (qreal)w1) + offsetX;
      y2 = qRound((qreal)(y1*h2) / (qreal)h1) + offsetY;
   }
   else if (
      m_zoomParameters->cumulativeZoomInHeight() < 0 &&
      m_zoomParameters->cumulativeZoomInWidth() < 0
      )
   {
      scaleX = (qreal)m_zoomParameters->zoomSourceRect().width() /
         (qreal)m_zoomParameters->zoomTargetRect().width();

      scaleY = (qreal)m_zoomParameters->zoomSourceRect().height() /
         (qreal)m_zoomParameters->zoomTargetRect().height();

      offsetX = qRound((qreal)m_zoomParameters->centralizedOffset_width()  * scaleX);
      offsetY = qRound((qreal)m_zoomParameters->centralizedOffset_height() * scaleY);

      w2 = m_zoomParameters->zoomTargetRect().width();
      h2 = m_zoomParameters->zoomTargetRect().height();
      x2 = qRound((qreal)(x1*w1) / (qreal)w2) - offsetX;
      y2 = qRound((qreal)(y1*h1) / (qreal)h2) - offsetY;
   }
   else
   {
      if (strict)
      {
         int m_panX = m_zoomParameters->panX();
         int m_panY = m_zoomParameters->panY();
         bufferPoint = QPoint(pt.x() - m_panX, pt.y() - m_panY);
         return bufferPoint;
      }
      else
      {
         return pt;
      }
   }

   if (panAdjusted)
   {
      int m_panX = m_zoomParameters->panX();
      int m_panY = m_zoomParameters->panY();
      bufferPoint = QPoint(x2 - m_panX, y2 - m_panY);
   }
   else
   {
      bufferPoint = QPoint(x2, y2);
   }
   return bufferPoint;
}

QPoint Zoomer::getZoomedPoint(const QPoint &pt)
{
   QPoint zoomedPoint;
   QRect source;
   QRect target;
   if (!getZoomGeo(&source, &target))
   {
      return pt;
   }

   QSize sourceSize = source.size();
   QSize targetSize = target.size();

   int w = sourceSize.width();
   int W = targetSize.width();

   int h = sourceSize.height();
   int H = targetSize.height();

   qreal scaleW = ((qreal)W) / ((qreal)w);
   qreal scaleH = ((qreal)H) / ((qreal)h);

   int dx = source.topLeft().x();
   int dy = source.topLeft().y();

   int diffX = pt.x() - dx;
   int diffY = pt.y() - dy;

   qreal realX = (qreal)diffX * scaleW;
   qreal realY = (qreal)diffY * scaleH;

   int x = (quint64)qRound(realX);
   int y = (quint64)qRound(realY);

   zoomedPoint.setX(x);
   zoomedPoint.setY(y);
   return zoomedPoint;
}

QRect Zoomer::getZoomedRect(const QRect &rect)
{
   QPoint origUL = rect.topLeft();
   QPoint origBR = rect.bottomRight();
   QPoint newUL = getZoomedPoint(origUL);
   QPoint tmpBR = getZoomedPoint(origBR);
   int w = tmpBR.x() - newUL.x();
   int h = tmpBR.y() - newUL.y();
   QPoint newBR(newUL.x() + w, newUL.y() + h);
   return QRect(newUL, newBR);
}

QPolygon Zoomer::getZoomedPolygon(const QPolygon &poly)
{
   QPolygon newPoly;
   for(int i=0; i<poly.size(); i++)
   {
      newPoly << getZoomedPoint(poly.at(i));
   }
   return newPoly;
}

QPoint Zoomer::mapToZoomedOutRect(const QPoint &pt)
{
   if(m_zoomSources.size())
   {
      return QPoint(-1,-1);
   }
   if(m_zoomParameters->zoomAmount() == 0)
   {
      return pt;
   }
   int x1 = pt.x();
   int y1 = pt.y();
   int w1 = m_zoomSources.at(0).width();
   int h1 = m_zoomSources.at(0).height();
   int w2 = m_zoomParameters->zoomTargetRect().width();
   int h2 = m_zoomParameters->zoomTargetRect().height();
   int x2 = qRound((qreal)(x1*w1)/(qreal)w2);
   int y2 = qRound((qreal)(y1*h1)/(qreal)h2);
   return QPoint(x2,y2);
}

QPoint Zoomer::mapToZoomedInRect(const QPoint &pt)
{
   int x1,x2,y1,y2,w1,w2,h1,h2 = 0;

   QSize  backTracedSize = backTrackSize(
      m_zoomParameters->zoomedRect().size(),
      m_zoomSources.size() - 1
      );

   if(backTracedSize.isEmpty())
   {
      return QPoint(-1,-1);
   }
   QPoint mappedPt = backTrackPoint(pt, m_zoomSources.size() - 1);
   if(mappedPt.x() < 0 || mappedPt.y() < 0)
   {
      return QPoint(-1,-1);
   }

   QRect lastZoom;
   if(m_zoomSources.size() > 0)
   {
      lastZoom = m_zoomSources.at(m_zoomSources.size() - 1);
   }
   else
   {
      lastZoom = QRect(0,0,m_zoomSources.at(0).width(), m_zoomSources.at(0).height());
   }

   x1 = pt.x();
   y1 = pt.y();
   w1 = backTracedSize.width();
   h1 = backTracedSize.height();
   w2 = lastZoom.width();
   h2 = lastZoom.height();
   x2 = qRound((qreal)(w1*x1)/(qreal)w2) + lastZoom.topLeft().x();
   y2 = qRound((qreal)(h1*y1)/(qreal)h2) + lastZoom.topLeft().y();
   return QPoint(x2, y2);
}

QPoint Zoomer::mapToZoomedInRect_upperLeftCorner(QPoint &pt, const QSize &sz)
{
   if (
      sz.width() <= 0 ||
      sz.height() <= 0 ||
      m_zoomSources.size() < 1
      )
   {
      return QPoint(-1, -1);
   }

   if (m_zoomParameters->zoomAmount() == 0)
   {
      return pt;
   }

   int x1, x2, y1, y2, w1, w2, h1, h2 = 0;
   QPoint origPt = pt;

   pt = backTrackPoint(pt, m_zoomSources.size() - 1);
   QRect lastZoom;
   if (m_zoomSources.size() > 0)
   {
      lastZoom = m_zoomSources.at(m_zoomSources.size() - 1);
   }
   else
   {
      lastZoom = QRect(0, 0, m_zoomSources.at(0).width(), m_zoomSources.at(0).height());
   }

   x1 = pt.x();
   y1 = pt.y();
   w1 = sz.width();
   h1 = sz.height();
   w2 = lastZoom.width();
   h2 = lastZoom.height();
   x2 = x1 - qRound((qreal)(w1*x1) / (qreal)w2) + lastZoom.topLeft().x();
   y2 = y1 - qRound((qreal)(h1*y1) / (qreal)h2) + lastZoom.topLeft().y();
   QPoint cornerPoint(x2, y2);

   return cornerPoint;
}

QPoint Zoomer::backTrackPoint(const QPoint &pt, int index)
{
   if (index < 1 || m_zoomSources.size() < index)
   {
      return pt;
   }

   int w1, w2, x1, x2, h1, h2, y1, y2 = 0;
   QRect lastRect = m_zoomSources.at(index);
   QRect beforeLastRect = m_zoomSources.at(index - 1);
   w1 = beforeLastRect.width();
   w2 = lastRect.width();
   x1 = pt.x();
   x2 = qRound((qreal)(x1*w2) / (qreal)w1);
   h1 = beforeLastRect.height();
   h2 = lastRect.height();
   y1 = pt.y();
   y2 = qRound((qreal)(y1*h2) / (qreal)h1);

   return backTrackPoint(QPoint(x2, y2), index - 1);
}

QSize Zoomer::backTrackSize(const QSize  &sz, int index)
{
   if (index < 1 || m_zoomSources.size() < index)
   {
      return sz;
   }

   QSize lastSize = m_zoomSources.at(index).size();
   QSize beforeLastSize = m_zoomSources.at(index - 1).size();
   int newWidth = sz.width() + lastSize.width() - beforeLastSize.width();
   int newHeight = sz.height() + lastSize.height() - beforeLastSize.height();
   QSize backTracedSize(newWidth, newHeight);
   return backTrackSize(backTracedSize, index - 1);
}

void Zoomer::pan(int dx, int dy)
{
   if (
      m_zoomSources.size() < 1 ||
      (dx == 0 && dy == 0)
      )
   {
      return;
   }

   m_zoomParameters->setpanX(m_zoomParameters->panX() + dx);
   m_zoomParameters->setpanY(m_zoomParameters->panY() + dy);
}

bool Zoomer::hasSource()
{
   return (m_zoomSources.size() > 0);
}