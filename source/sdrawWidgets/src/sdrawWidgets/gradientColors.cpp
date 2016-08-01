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
//  \file      gradientColors.cpp
//
//  \brief     Implementation of a class to define gradient colors
//
//  \author    Stian Broen
//
//  \date      20.05.2012
//
//  \par       Copyright:
//
//             Copyright by Stian Broen
//
//  \par       Revision History
//
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLinearGradient>
#include <QBrush>
#include <QDial>
#include <QSpinBox>
#include <QMouseEvent>

// solution includes
#include <common/definitions.h>

// local includes
#include "gradientColors.h"

using namespace sdraw;

GradientArrow::GradientArrow()
{
   QPolygon area;
   area << QPoint(0,5) << QPoint(5, 0) << QPoint(10,5) << QPoint(10, 20) << QPoint(0,20) << QPoint(0,5);
   m_area = area;
}

CGradientColorsView::CGradientColorsView(QWidget *parent)
   :QWidget(parent),
   m_isMoving(false)
{
   backgroundHorSpace = 5;
   backgroundVerSpace = 5;
   qRegisterMetaType<GradientArrow>("GradientArrow");
   setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
   setFocusPolicy(Qt::StrongFocus);
   background = 0;
   viewRect = QRect(QPoint(0,0), QPoint(0,0));
   setMinimumSize(parent->width() - 10, 100);
   setMouseTracking(true);
   update();
}

CGradientColorsView::~CGradientColorsView()
{
   delete background;
}

void CGradientColorsView::createBackground(int vertLineSpace, int horLineSpace)
{
   backgroundVerSpace = vertLineSpace;
   backgroundHorSpace = horLineSpace;
   if(0 != background)
   {
      delete background;
      background = 0;
   }
   background = new QPixmap(QSize(800,800));
   background->fill(Qt::white);
   QPainter painter(background);
   
   // draw vertical lines
   int x = 0;
   while(x < background->width())
   {
      QPoint lineStart(x,0);
      QPoint lineStop(x, background->height());
      painter.drawLine(lineStart, lineStop);
      x += vertLineSpace;
   }

   // draw horizontal lines
   int y = 0;
   while(y < background->height())
   {
      QPoint lineStart(0, y);
      QPoint lineStop (background->width(), y);
      painter.drawLine(lineStart, lineStop);
      y += horLineSpace;
   }
   painter.end();
   update();
}

QPixmap* CGradientColorsView::getBackGround()
{
   if(0 == background)
   {
      createBackground(backgroundVerSpace, backgroundHorSpace);
   }
   return background;
}

void CGradientColorsView::toggleBackground(bool onOrOff)
{
   if(onOrOff == false)
   {
      if(0 == background)
      {
         background = new QPixmap(viewRect.size());
      }
      background->fill(Qt::transparent);
      update();
   }
   else
   {
      createBackground(backgroundVerSpace, backgroundHorSpace);
   }
}



void CGradientColorsView::clearArrows()
{
   m_arrows.clear();
}

void CGradientColorsView::setFocusColor(const QColor &col)
{
   QMutableHashIterator<int, GradientArrow> it(m_arrows);
   int count = 0;
   while(it.hasNext())
   {
      it.next();
      GradientArrow anArrow = it.value();
      if(anArrow.m_hasFocus)
      {
         count++;
         anArrow.m_color = col;
         it.setValue(anArrow);
      }
   }
   if(count > 1)
   {
      //bool debugBreak = true;
   }
}

void CGradientColorsView::paintEvent(QPaintEvent *)
{
   if(viewRect.size().isNull()  || 
      viewRect.size().isEmpty() ||
      viewRect.topLeft() == viewRect.bottomRight())
   {
      viewRect = QRect(QPoint(0,0), QPoint(this->width() - 10, this->height()/3*2 - 10));
      viewRect.translate(5,5);
      createBackground();
   }
   QPainter painter(this);
   if(0 != background)
   {
      painter.drawPixmap(viewRect, *background, viewRect);
   }
   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

   painter.setRenderHint(QPainter::Antialiasing);
   QPoint gradStart = QPoint(viewRect.topLeft ().x() ,viewRect.bottomLeft ().y()/2);
   QPoint gradStop  = QPoint(viewRect.topRight().x(), viewRect.bottomRight().y()/2);
   QLinearGradient grad(gradStart, gradStop);

   int count = 0;

   QHashIterator<int, GradientArrow> it(m_arrows);
   while(it.hasNext())
   {
      it.next();
      GradientArrow m_arrow = it.value();
      grad.setColorAt(m_arrow.m_position, m_arrow.m_color);
      QPolygon arrowPolygon = m_arrow.m_area;
      arrowPolygon.translate(m_arrow.m_position * (this->width() - 10), this->height()/3*2);
      QPainterPath paintPath;
      paintPath.addPolygon(arrowPolygon);
      painter.setBrush(QBrush(m_arrow.m_color));
      painter.drawPath(paintPath);
      painter.setBrush(QBrush(Qt::NoBrush));
      if(m_arrow.m_hasFocus)
      {
         count++;

         QPen lastPen = painter.pen();
         QPen boldPen;
         boldPen.setWidth(2);
         painter.setPen(boldPen);
         painter.drawPolygon(arrowPolygon);
         painter.setPen(lastPen);
      }
      else
      {
         painter.drawPolygon(arrowPolygon);
      }
   }

   if(count > 1)
   {
      //bool debugBreak = true;
   }
   QBrush brush(grad);
   painter.fillRect(viewRect, brush);
   painter.drawRect(viewRect);
   painter.end();
}

void CGradientColorsView::mousePressEvent(QMouseEvent *event)
{
   dragStart = event->pos();
   QMutableHashIterator<int, GradientArrow> it(m_arrows);
   it.toBack();
   int focusCount = 0;
   while(it.hasPrevious())
   {
      it.previous();
      GradientArrow arrow = it.value();
      QPolygon poly = arrow.m_area;
      poly.translate(arrow.m_position * (this->width() - 10), this->height()/3*2);
      if(poly.containsPoint(dragStart, Qt::OddEvenFill))
      {
         ++focusCount;
         
         arrow.m_hasFocus = true;
         emit newFocusColor(arrow.m_color, arrow.m_index);
      }
      else
      {
         arrow.m_hasFocus = false;
      }
      it.setValue(arrow);
   }
   if (focusCount > 0)
   {
      m_isMoving = true;
      update();
   }
   else
   {
      m_isMoving = false;
   }
}

void CGradientColorsView::mouseMoveEvent(QMouseEvent *event)
{
   if (!m_isMoving)
   {
      return;
   }
   QMutableHashIterator<int, GradientArrow> it(m_arrows);
   int count = 0;
   while(it.hasNext())
   {
      it.next();
      GradientArrow arrow = it.value();
      if(arrow.m_hasFocus)
      {
         count++;
         qreal dPos = (qreal)(event->pos().x() - dragStart.x())/((qreal)this->width() - 10); 
         if(arrow.m_position + dPos > 1 || arrow.m_position + dPos < 0)
         {
            return;
         }
         qreal lastPos = arrow.m_position;
         arrow.m_position += dPos;
         emit arrowMove(lastPos, arrow);
         it.setValue(arrow);
      }
   }
   if(count >= 1)
   {
      dragStart = event->pos();
      update();
   }
}

void CGradientColorsView::mouseReleaseEvent(QMouseEvent *)
{
   m_isMoving = false;
}

void CGradientColorsView::changeArrow(GradientArrow arrow)
{
   if(!m_arrows.contains(arrow.m_index))
   {
      return;
   }
   if(arrow.m_hasFocus)
   {
      QMutableHashIterator<int, GradientArrow> it(m_arrows);
      while(it.hasNext())
      {
         it.next();
         GradientArrow anArrow = it.value();
         anArrow.m_hasFocus = false;
         it.setValue(anArrow);
      }
   }
   m_arrows.insert(arrow.m_index, arrow);
   update();
}

void CGradientColorsView::addArrow(GradientArrow arrow)
{
   while (m_arrows.contains(arrow.m_index))
   {
      ++arrow.m_index;
   }
   if (arrow.m_hasFocus)
   {
      QMutableHashIterator<int, GradientArrow> it(m_arrows);
      while (it.hasNext())
      {
         it.next();
         GradientArrow anArrow = it.value();
         anArrow.m_hasFocus = false;
         it.setValue(anArrow);
      }
   }
   m_arrows.insert(arrow.m_index, arrow);
   update();
}

void CGradientColorsView::removeArrow(int index)
{
   if (m_arrows.size() <= 2)
   {
      return;
   }
   bool newFocus = false;
   if (m_arrows.contains(index))
   {
      newFocus = m_arrows[index].m_hasFocus;
      m_arrows.remove(index);
   }
   QMutableHashIterator<int, GradientArrow> it(m_arrows);
   if (index == -1)
   {
      while (it.hasNext())
      {
         it.next();
         GradientArrow anArrow = it.value();
         if (anArrow.m_hasFocus)
         {
            newFocus = true;
            it.remove();
         }
      }
   }

   if (newFocus)
   {
      it.toBack();
      if (it.hasPrevious())
      {
         it.previous();
         GradientArrow newFocus = it.value();
         newFocus.m_hasFocus = true;
         it.setValue(newFocus);
      }
   }
   update();
}

CGradientColors::CGradientColors(QWidget *parent)
   :QWidget(parent)
{
   qRegisterMetaType<GradientArrow>("GradientArrow");
   arrangeLayout(); 
}

CGradientColors::~CGradientColors()
{

}

void CGradientColors::newFocusColor(const QColor &col, int index)
{
   disconnect(alphaDial, SIGNAL(valueChanged(int)),
      this, SLOT(alphaChanged(int)));

   disconnect(alphaSpinbox, SIGNAL(valueChanged(int)),
      this, SLOT(alphaChanged(int)));

   alphaDial->setValue(col.alpha());

   connect(alphaDial, SIGNAL(valueChanged(int)),
      this, SLOT(alphaChanged(int)));

   connect(alphaSpinbox, SIGNAL(valueChanged(int)),
      this, SLOT(alphaChanged(int)));

   QMutableHashIterator<int, GradientArrow> it(colorView->arrows());
   while (it.hasNext())
   {
      it.next();
      GradientArrow arrow = it.value();
      int key = it.key();
      if (key == index)
      {
         arrow.m_color = col;
         arrow.m_hasFocus = true;
         emit setGradientColor(arrow.m_position, arrow.m_position, arrow.m_color);
      }
      else
      {
         arrow.m_hasFocus = false;
      }
      it.setValue(arrow);
   }

   disconnect(colorPicker, SIGNAL(colorChanged(const QColor &)),
      this, SLOT(colorFromPicker(const QColor &)));

   colorPicker->setColor(col);

   connect(colorPicker, SIGNAL(colorChanged(const QColor &)),
      this, SLOT(colorFromPicker(const QColor &)));
}

void CGradientColors::arrangeLayout()
{
   colorView = new CGradientColorsView(this);

   connect(colorView, SIGNAL(arrowMove(qreal, GradientArrow)),
      this, SLOT(arrowMove(qreal, GradientArrow)));

   connect(this, SIGNAL(changeArrow(GradientArrow)),
      colorView, SLOT(changeArrow(GradientArrow)));

   connect(this, SIGNAL(addArrow(GradientArrow)),
      colorView, SLOT(addArrow(GradientArrow)));

   connect(this, SIGNAL(removeArrow(int)),
      colorView, SLOT(removeArrow(int)));

   connect(colorView, SIGNAL(newFocusColor(const QColor &, int)),
      this, SLOT(newFocusColor(const QColor &, int)));

   connect(colorView, SIGNAL(arrowMove(qreal, GradientArrow)),
      this, SLOT(arrowMove(qreal, GradientArrow)));

   addArrow(Qt::white, 0.0);
   addArrow(Qt::black, 1.0);

   colorPicker = new ChooseColor(this);
   colorPicker->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
   colorPicker->setColor(Qt::black);

   connect(colorPicker, SIGNAL(colorChanged(const QColor &)),
      this, SLOT(colorFromPicker(const QColor &)));

   QVBoxLayout *colorLayout = new QVBoxLayout;
   colorLayout->addWidget(colorPicker);
   QGroupBox *colorGroup = new QGroupBox(this);
   colorGroup->setTitle(tr("Color"));
   colorGroup->setLayout(colorLayout);

   alphaDial = new QDial(this);
   alphaDial->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
   alphaDial->setRange(0, 255);
   alphaDial->setValue(255);

   alphaSpinbox = new QSpinBox(this);
   alphaSpinbox->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
   alphaSpinbox->setRange(0, 255);
   alphaSpinbox->setValue(255);

   QHBoxLayout *alphaControlLayout = new QHBoxLayout;
   alphaControlLayout->addWidget(alphaDial);
   alphaControlLayout->addWidget(alphaSpinbox);
   QGroupBox *alphaBox = new QGroupBox(this);
   alphaBox->setTitle(tr("Alpha"));
   alphaBox->setLayout(alphaControlLayout);

   showBackgroundGrid = new QCheckBox(this);
   showBackgroundGrid->setChecked(true);
   showBackgroundGrid->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

   connect(showBackgroundGrid, SIGNAL(stateChanged(int)), 
      this, SLOT(toggleBackgroundGrid(int)));

   backGroundSpacingSpinbox = new QSpinBox(this);
   backGroundSpacingSpinbox->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
   backGroundSpacingSpinbox->setRange(2, 15);
   backGroundSpacingSpinbox->setValue(5);

   connect(backGroundSpacingSpinbox, SIGNAL(valueChanged(int)), 
      this, SLOT(changeBackgroundGridSpacing(int)));

   QHBoxLayout *backgroundLayout = new QHBoxLayout;
   backgroundLayout->addWidget(showBackgroundGrid);
   backgroundLayout->addWidget(backGroundSpacingSpinbox);
   QGroupBox *backgroundGroup = new QGroupBox(this);
   backgroundGroup->setTitle(tr("Background"));
   backgroundGroup->setLayout(backgroundLayout);

   QHBoxLayout *allAlphaSettingsLayout = new QHBoxLayout;
   allAlphaSettingsLayout->addWidget(alphaBox);
   allAlphaSettingsLayout->addWidget(backgroundGroup);

   connect(alphaDial, SIGNAL(valueChanged(int)),
      alphaSpinbox, SLOT(setValue(int)));

   connect(alphaSpinbox, SIGNAL(valueChanged(int)),
      alphaDial, SLOT(setValue(int)));

   connect(alphaDial, SIGNAL(valueChanged(int)),
      this, SLOT(alphaChanged(int)));

   connect(alphaSpinbox, SIGNAL(valueChanged(int)),
      this, SLOT(alphaChanged(int)));

   addColorButton = new QPushButton(this);
   addColorButton->setText(tr("Add Color"));

   connect(addColorButton, SIGNAL(clicked()),
      this, SLOT(addColorButtonClicked()));

   removeColorButton = new QPushButton(this);
   removeColorButton->setText(tr("Remove Color"));

   connect(removeColorButton, SIGNAL(clicked()),
      this, SLOT(deleteFocusArrow()));

   resetButton = new QPushButton(this);
   resetButton->setText(tr("Reset"));

   connect(resetButton, SIGNAL(clicked()),
      this, SLOT(resetToDefault()));

   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget(addColorButton);
   buttonLayout->addWidget(removeColorButton);
   buttonLayout->addWidget(resetButton);

   QVBoxLayout *optionsLayout = new QVBoxLayout;
   optionsLayout->addLayout(allAlphaSettingsLayout);
   optionsLayout->addLayout(buttonLayout);

   QGroupBox *optionsGroup = new QGroupBox(this);
   optionsGroup->setTitle(tr("Options"));
   optionsGroup->setLayout(optionsLayout);

   QGridLayout *grid = new QGridLayout;
   grid->addWidget(colorView, 0, 0, 1, 2);
   grid->addWidget(colorGroup, 1, 0, 1, 1);
   grid->addWidget(optionsGroup, 1, 1, 1, 1);
   setLayout(grid);
}

void CGradientColors::toggleBackgroundGrid(int state)
{
   if(0 == colorView)
   {
      return;
   }
   Qt::CheckState _state = static_cast<Qt::CheckState>(state);
   switch(_state)
   {
   case(Qt::Unchecked):
      colorView->toggleBackground(false);
      emit backgroundChanged(false);
      break;
   case(Qt::Checked):
      colorView->toggleBackground(true);
      emit backgroundChanged(true);
      break;
   default:
      break;
   }
}

void CGradientColors::changeBackgroundGridSpacing(int spacing)
{
   if(0 != colorView)
   {
      colorView->createBackground(spacing, spacing);
   }
   emit backgroundChanged(showBackgroundGrid->isChecked());
}

void CGradientColors::alphaChanged(int val)
{
   QColor col = colorPicker->getColor();
   col.setAlpha(val);
   colorPicker->setColor(col);

   QHashIterator<int, GradientArrow> it(colorView->arrows());
   while(it.hasNext())
   {
      it.next();
      GradientArrow arrow = it.value();
      if(arrow.m_hasFocus)
      {
         emit setGradientColor(arrow.m_position, arrow.m_position, col);
         return;
      }
   }
}

void CGradientColors::colorFromPicker(const QColor &col)
{
   QColor colWAlpha = col;
   colWAlpha.setAlpha(alphaDial->value());
   colorView->setFocusColor(colWAlpha);

   QHash<int, GradientArrow> arrws = colorView->arrows();
   QHashIterator<int, GradientArrow> it(arrws);
   while(it.hasNext())
   {
      it.next();
      GradientArrow arrow = it.value();
      if(arrow.m_hasFocus)
      {
         arrow.m_color = colWAlpha;
         emit setGradientColor(arrow.m_position, arrow.m_position, arrow.m_color);
         emit changeArrow(arrow);
         return;
      }
   }
}

bool CGradientColors::getAllColors(QMap<qreal, QColor> &colors)
{
   if (colorView->arrows().isEmpty())
   {
      return false;
   }

   QHashIterator<int, GradientArrow> it(colorView->arrows());
   while(it.hasNext())
   {
      it.next();
      colors.insert(it.value().m_position, it.value().m_color);
   }
   return true;
}

void CGradientColors::arrowMove(qreal lastPos, GradientArrow arrow)
{
   emit setGradientColor(lastPos, arrow.m_position, arrow.m_color);
}

void CGradientColors::addColorButtonClicked()
{
   addArrow(colorPicker->getColor(), 0.5, true);
}

void CGradientColors::addArrow(const QColor &color, qreal position, bool hasFocus)
{
   emit addGradientColor(position, color);
   GradientArrow arrow;
   arrow.m_hasFocus = hasFocus;
   arrow.m_color = color;
   arrow.m_position = position;
   emit addArrow(arrow);


   //if(arrow.m_hasFocus)
   //{
   //   QMutableHashIterator<int, GradientArrow> it(m_arrows);
   //   while(it.hasNext())
   //   {
   //      it.next();
   //      GradientArrow anArrow = it.value();
   //      anArrow.m_hasFocus = false;
   //      it.setValue(anArrow);
   //   }
   //}

   //int index = 0;
   //while(m_arrows.contains(index))
   //{
   //   index++;
   //}
   //arrow.m_index = index;
   //m_arrows.insert(arrow.m_index, arrow);
   
}

void CGradientColors::deleteFocusArrow()
{
   emit removeArrow(-1);
   //if (colorView->arrows().size() <= 2)
   //{
   //   return;
   //}
   //QHashIterator<int, GradientArrow> it(colorView->arrows());
   //while(it.hasNext())
   //{
   //   it.next();
   //   GradientArrow arrow = it.value();
   //   if(arrow.m_hasFocus)
   //   {
   //      emit removeGradientColor(arrow.m_position);
   //      if(0 != colorView)
   //      {
   //         colorView->removeArrow(arrow.m_index);
   //      }
   //      break;
   //   }
   //}
}

void CGradientColors::resetToDefault()
{
   if(0 != colorView)
   {
      colorView->clearArrows();
   }

   alphaDial->setValue(255);
   backGroundSpacingSpinbox->setValue(5);
   showBackgroundGrid->setChecked(true);
   addArrow(Qt::white, 0.0);
   addArrow(Qt::black, 1.0);

   disconnect(colorPicker, SIGNAL(colorChanged(const QColor &)), 
      this, SLOT(colorFromPicker(const QColor &)));

   colorPicker->setColor(Qt::black);

   connect(colorPicker, SIGNAL(colorChanged(const QColor &)), 
      this, SLOT(colorFromPicker(const QColor &)));

   emit gradientReset();
}

void CGradientColors::setSingleColorMode()
{
   disconnect(colorPicker, SIGNAL(colorChanged(const QColor &)), 
      this, SLOT(colorFromPicker(const QColor &)));

   disconnect(this, SIGNAL(changeArrow(GradientArrow)), 
      colorView, SLOT(changeArrow(GradientArrow)));

   disconnect(colorView, SIGNAL(newFocusColor(const QColor &, int)), 
      this, SLOT(newFocusColor(const QColor &, int)));

   disconnect(colorView, SIGNAL(arrowMove(qreal, GradientArrow)), 
      this, SLOT(arrowMove(qreal, GradientArrow)));

   connect(colorPicker, SIGNAL(colorChanged(const QColor &)), 
      this, SLOT(addAlphaAndSendSingleColor(const QColor &)));

   //removeColorButton->setHidden(true);
   //addColorButton->setHidden(true);
   //colorView->setHidden(true);
}

void CGradientColors::setMultiColorMode()
{
   connect(colorPicker, SIGNAL(colorChanged(const QColor &)), 
      this, SLOT(colorFromPicker(const QColor &)));

   connect(colorView, SIGNAL(arrowMove(int, qreal)), 
      this, SLOT(arrowMove(int, qreal)));

   connect(this, SIGNAL(changeArrow(GradientArrow)), 
      colorView, SLOT(changeArrow(GradientArrow)));

   connect(colorView, SIGNAL(newFocusColor(const QColor &)), 
      this, SLOT(newFocusColor(const QColor &)));

   connect(colorView, SIGNAL(arrowMove(qreal, GradientArrow)), 
      this, SLOT(arrowMove(qreal, GradientArrow)));

   disconnect(colorPicker, SIGNAL(colorChanged(const QColor &)), 
      this, SLOT(addAlphaAndSendSingleColor(const QColor &)));

   //removeColorButton->setEnabled(true);
   //addColorButton->setEnabled(true);
   //colorView->setEnabled(true);
}

QPixmap* CGradientColors::getBackGround()
{
   if(0 == colorView)
   {
      return 0;
   }
   return colorView->getBackGround();
}

void CGradientColors::addAlphaAndSendSingleColor(const QColor &color)
{
   QColor alphaCol = color;
   alphaCol.setAlpha(alphaDial->value());
   emit setSingleColor(alphaCol);
}
