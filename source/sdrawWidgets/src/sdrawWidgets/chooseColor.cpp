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
//  \file      chooseColor.cpp
//
//  \brief     
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

// local includes
#include "ChooseColor.h"

using namespace sdraw;

ViewColor::ViewColor(QWidget *parent)
   :QPushButton(parent)
{
   setMinimumSize(10, 10);
}

ViewColor::~ViewColor()
{

}

void ViewColor::closeMenu()
{
   if (0 != menu())
   {
      this->menu()->close();
   }
}

void ViewColor::setPen(const QPen &pen)
{
   m__pen = pen;
}

QPen ViewColor::pen() const
{
   return m__pen;
}

void ViewColor::setColor(const QColor &color)
{
   m__color = color;
}

QColor ViewColor::color() const
{
   return m__color;
}

void ViewColor::changeColor(const QColor &color)
{
   m__color = color;
   emit colorChanged(m__color);
   if (0 != menu())
   {
      menu()->close();
   }
   update();
}

void ViewColor::catchCancel(int)
{
   if (0 != menu())
   {
      menu()->close();
   }
}

void ViewColor::paintEvent(QPaintEvent *)
{
   QPainter p(this);
   p.setPen(m__pen);
   p.setBrush(QBrush(m__color));
   int nw = width() - 4;
   int nh = height() - 4;
   QRect toDraw(
      2,
      2,
      nw,
      nh);
   p.drawRect(toDraw);
}

void ViewColor::mousePressEvent(QMouseEvent *)
{
   static QWidgetAction *colorDialogAction = 0;
   if (0 == colorDialogAction)
   {
      QWidgetAction *colorDialogAction = new QWidgetAction(this);
      m_colorDialog = new QColorDialog(this);
      m_colorDialog->setCurrentColor(m__color);
      m_colorDialog->setOption(QColorDialog::ShowAlphaChannel);

      connect(m_colorDialog, SIGNAL(colorSelected(const QColor &)),
         this, SLOT(changeColor(const QColor &)));

      connect(m_colorDialog, SIGNAL(finished(int)),
         this, SLOT(catchCancel(int)));

      colorDialogAction->setDefaultWidget(m_colorDialog);
      QMenu *colorMenu = new QMenu(this);
      colorMenu->addAction(colorDialogAction);
      this->setMenu(colorMenu);
   }
   this->menu()->show();
}


ChooseColorScreen::ChooseColorScreen(QWidget *parent)
   :QWidget(parent),
   m_label(0)
{
   m_label = new QLabel(this);
   m_label->setPixmap(QPixmap::grabWindow(QApplication::desktop()->winId()));
   m_label->move(0, 0);
   showFullScreen();
}

ChooseColorScreen::~ChooseColorScreen()
{

}

void ChooseColorScreen::mousePressEvent(QMouseEvent *event)
{
   const QPixmap *labelPixmap = m_label->pixmap();
   QImage labelImage = labelPixmap->toImage();
   QColor colorAtPixel = labelImage.pixel(event->pos());
   emit colorSelected(colorAtPixel);
   this->close();
   labelPixmap = 0;
}

ChooseColor::ChooseColor(QWidget *parent)
   :QWidget(parent),
   m_colorTriangle(new QtColorTriangle(parent)),
   m_colorView(new ViewColor(parent)),
   m_layout(new QGridLayout),
   m_screen(0)
{
   connect(m_colorTriangle, SIGNAL(colorChanged(QColor)),
      this, SLOT(colorChgd()));

   m_colorView->setColor(m_colorTriangle->color());

   connect(m_colorTriangle, SIGNAL(colorChanged(QColor)), 
      m_colorView, SLOT(changeColor(QColor)));

   connect(m_colorView, SIGNAL(colorChanged(const QColor &)), 
      this, SLOT(setColor(const QColor &)));

   m_layout->addWidget(m_colorTriangle, 0, 0, 3, 1);
   m_layout->addWidget(m_colorView, 0, 1, 3, 1);
   this->setLayout(m_layout);
}

ChooseColor::~ChooseColor()
{

}

void ChooseColor::closeColorDialog()
{
   m_colorView->closeMenu();
}

QColor ChooseColor::getColor()
{
   return m_colorView->color();
}

void ChooseColor::resizeEvent(QResizeEvent * event)
{
   m_colorView->setMinimumHeight(event->size().height() - 22);
   m_colorView->setMaximumHeight(event->size().height() - 22);
}

void ChooseColor::pickMode()
{
   if (m_screen != 0)
   {
      delete m_screen;
      m_screen = 0;
   }
   m_screen = new ChooseColorScreen;

   connect(m_screen, SIGNAL(colorSelected(QColor)),
      m_colorTriangle, SLOT(setColor(QColor)));
}

void ChooseColor::setColor(const QColor &col)
{
   m_colorView->closeMenu();
   m_colorView->setColor(col);
   m_colorTriangle->setColor(col);
}


void ChooseColor::colorChgd()
{
   emit colorChanged(m_colorView->color());
}
