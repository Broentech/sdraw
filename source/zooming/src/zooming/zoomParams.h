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
//  \file      zoomParams.h
//
//  \brief     Parameters used by the zoomer class
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

#ifndef _ZOOM_PARAMS_H_
#define _ZOOM_PARAMS_H_

#include <QRect>
#include <QSize>

namespace sdraw {
   class ZoomParams
   {
   public:
      explicit ZoomParams();
      ~ZoomParams();

      inline QSize minSource() const { return m_minSource; }
      inline QSize minTarget() const { return m_minTarget; }
      inline int centralizedOffset_width() const { return m_centralizedOffset_width; }
      inline int centralizedOffset_height() const { return m_centralizedOffset_height; }
      inline QPoint zoomCorner() const{ return m_zoomCorner; }
      inline QPoint zoomCenter() const{ return m_zoomCenter; }
      inline QRect zoomSourceRect() const{ return m_zoomSourceRect; }
      inline QRect zoomTargetRect() const{ return m_zoomTargetRect; }
      inline QRect zoomedRect() const{ return m_zoomedRect; }
      inline int currentZoom() const{ return m_currentZoom; }
      inline bool zoomSwitch() const{ return m_zoomSwitch; }
      inline int zoomAmount() const{ return m_zoomAmount; }
      inline int widthZoom() const{ return m_widthZoom; }
      inline int heightZoom() const{ return m_heightZoom; }
      inline int cumulativeZoomInWidth() const{ return m_cumulativeZoomInWidth; }
      inline int cumulativeZoomInHeight() const{ return m_cumulativeZoomInHeight; }
      inline int panX() const{ return m_panX; }
      inline int panY() const{ return m_panY; }

      inline void setminSource(const QSize &size) { m_minSource = size; }
      inline void setminTarget(const QSize &size) { m_minTarget = size; }
      inline void setcentralizedOffset_width(int w) { m_centralizedOffset_width = w; }
      inline void setcentralizedOffset_height(int h) { m_centralizedOffset_height = h; }
      inline void setzoomCorner(const QPoint &p) { m_zoomCorner = p; }
      inline void setzoomCenter(const QPoint &p) { m_zoomCenter = p; }
      inline void setzoomSourceRect(const QRect &rect) { m_zoomSourceRect = rect; }
      inline void setzoomTargetRect(const QRect &rect) { m_zoomTargetRect = rect; }
      inline void setzoomedRect(const QRect &rect) { m_zoomedRect = rect; }
      inline void setcurrentZoom(int z) { m_currentZoom = z; }
      inline void setzoomSwitch(bool val) { m_zoomSwitch = val; }
      inline void setzoomAmount(int amount) { m_zoomAmount = amount; }
      inline void setwidthZoom(int w) { m_widthZoom = w; }
      inline void setheightZoom(int h) { m_heightZoom = h; }
      inline void setcumulativeZoomInWidth(int w) { m_cumulativeZoomInWidth = w; }
      inline void setcumulativeZoomInHeight(int h) { m_cumulativeZoomInHeight = h; }
      inline void setpanX(int x) { m_panX = x; }
      inline void setpanY(int y) { m_panY = y; }

   private:
      QSize m_minSource;
      QSize m_minTarget;
      int m_centralizedOffset_width;
      int m_centralizedOffset_height;
      QPoint m_zoomCorner;
      QPoint m_zoomCenter;
      QRect m_zoomSourceRect;
      QRect m_zoomTargetRect;
      QRect m_zoomedRect;
      int m_currentZoom;
      bool m_zoomSwitch;
      int m_zoomAmount;
      int m_widthZoom;
      int m_heightZoom;
      int m_cumulativeZoomInWidth;
      int m_cumulativeZoomInHeight;
      int m_panX;
      int m_panY;
   };
}

#endif