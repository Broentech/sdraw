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
//  \file      brushSelector.cpp
//
//  \brief     Declaration of a class to define brush
//
//  \author    Stian Broen
//
//  \date      August-2011
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
#include <QPushButton>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFile>
#include <QDir>
#include <QLabel>
#include <QSpacerItem>
#include <QPainter>
#include <QButtonGroup>

// local includes
#include "brushSelector.h"

using namespace sdraw;

CBrushSelector::CBrushSelector(QWidget *parent)
   : SdrawCustomWidget(parent),
   gradientColorsLayout(0),
   m_gradientColors(0),
   gradientColorsLabel(0),
   drawBackground(true),
   totalLayout(0),
   patternButtons(0),
   patternButtonsLayout(0),
   solidPatternButton(0),
   dense1PatternButton(0),
   dense2PatternButton(0),
   dense3PatternButton(0),
   dense4PatternButton(0),
   dense5PatternButton(0),
   dense6PatternButton(0),
   dense7PatternButton(0),
   noBrushButton(0),
   horPatternButton(0),
   verPatternButton(0),
   crossPatternButton(0),
   bDiagPatternButton(0),
   fDiagPatternButton(0),
   diagCrossPatternButton(0),
   linearGradientPatternButton(0),
   radialGradientPatternButton(0),
   conicalGradientPatternButton(0),
   angleSelector(0),
   spreadButtonsLayout(0),
   spreadButtons(0),
   padSpreadButton(0),
   repeatSpreadButton(0),
   reflectSpreadButton(0)
{
   arrangeGradientColors();
   arrangePatternButtons();
   arrangeSpreadButtons();

   somePatternButtonClicked(static_cast<int>(Qt::NoBrush));

   // arrange layouts
   QVBoxLayout *forBox = new QVBoxLayout;
   forBox->addLayout(gradientColorsLayout);
   forBox->addLayout(patternButtonsLayout);
   forBox->addLayout(spreadButtonsLayout);
   QGroupBox *brushBox = new QGroupBox(this);
   brushBox->setTitle(tr("Brush Settings"));
   brushBox->setLayout(forBox);

   totalLayout = new QVBoxLayout;
   totalLayout->addLayout(SdrawCustomWidget::setCloseLayout());
   totalLayout->addWidget(brushBox);
   setLayout(totalLayout);

   updateSettings();
}

void CBrushSelector::updateSettings() const
{
   CBrushSettings brsh = CDrawSettings_2D::getInstance()->brushSettings();
   QMap<qreal, QColor> colors;
   if (m_gradientColors->getAllColors(colors))
   {
      brsh.setGradientColors(colors);
   }
   CDrawSettings_2D::getInstance()->setBrushSettings(brsh);
}

CBrushSelector::~CBrushSelector()
{

}

void CBrushSelector::arrangePatternButtons()
{
   // create buttons
   solidPatternButton = new QPushButton(this);
   dense1PatternButton = new QPushButton(this);
   dense2PatternButton = new QPushButton(this);
   dense3PatternButton = new QPushButton(this);
   dense4PatternButton = new QPushButton(this);
   dense5PatternButton = new QPushButton(this);
   dense6PatternButton = new QPushButton(this);
   dense7PatternButton = new QPushButton(this);
   noBrushButton = new QPushButton(this);
   horPatternButton = new QPushButton(this);
   verPatternButton = new QPushButton(this);
   crossPatternButton = new QPushButton(this);
   bDiagPatternButton = new QPushButton(this);
   fDiagPatternButton = new QPushButton(this);
   diagCrossPatternButton = new QPushButton(this);
   linearGradientPatternButton = new QPushButton(this);
   radialGradientPatternButton = new QPushButton(this);
   conicalGradientPatternButton = new QPushButton(this);

   // set tooltip
   solidPatternButton->setToolTip(tr("Uniform color."));
   dense1PatternButton->setToolTip(tr("Extremely dense brush pattern."));
   dense2PatternButton->setToolTip(tr("Very dense brush pattern."));
   dense3PatternButton->setToolTip(tr("Somewhat dense brush pattern."));
   dense4PatternButton->setToolTip(tr("Half dense brush pattern."));
   dense5PatternButton->setToolTip(tr("Somewhat sparse brush pattern."));
   dense6PatternButton->setToolTip(tr("Very sparse brush pattern."));
   dense7PatternButton->setToolTip(tr("Extremely sparse brush pattern."));
   noBrushButton->setToolTip(tr("No brush pattern."));
   horPatternButton->setToolTip(tr("Horizontal lines."));
   verPatternButton->setToolTip(tr("Vertical lines."));
   crossPatternButton->setToolTip(tr("Crossing horizontal and vertical lines."));
   bDiagPatternButton->setToolTip(tr("Backward diagonal lines."));
   fDiagPatternButton->setToolTip(tr("Forward diagonal lines."));
   diagCrossPatternButton->setToolTip(tr("Crossing diagonal lines."));
   linearGradientPatternButton->setToolTip(tr("Linear gradient"));
   radialGradientPatternButton->setToolTip(tr("Radial gradient"));
   conicalGradientPatternButton->setToolTip(tr("Conical gradient, define angle using the angle-selector below"));

   // set button sizes
   solidPatternButton->setIconSize(QSize(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT));
   dense1PatternButton->setIconSize(QSize(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT));
   dense2PatternButton->setIconSize(QSize(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT));
   dense3PatternButton->setIconSize(QSize(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT));
   dense4PatternButton->setIconSize(QSize(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT));
   dense5PatternButton->setIconSize(QSize(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT));
   dense6PatternButton->setIconSize(QSize(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT));
   dense7PatternButton->setIconSize(QSize(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT));
   noBrushButton->setIconSize(QSize(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT));
   horPatternButton->setIconSize(QSize(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT));
   verPatternButton->setIconSize(QSize(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT));
   crossPatternButton->setIconSize(QSize(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT));
   bDiagPatternButton->setIconSize(QSize(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT));
   fDiagPatternButton->setIconSize(QSize(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT));
   diagCrossPatternButton->setIconSize(QSize(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT));
   linearGradientPatternButton->setIconSize(QSize(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT));
   radialGradientPatternButton->setIconSize(QSize(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT));
   conicalGradientPatternButton->setIconSize(QSize(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT));

   // set checkable
   solidPatternButton->setCheckable(true);
   dense1PatternButton->setCheckable(true);
   dense2PatternButton->setCheckable(true);
   dense3PatternButton->setCheckable(true);
   dense4PatternButton->setCheckable(true);
   dense5PatternButton->setCheckable(true);
   dense6PatternButton->setCheckable(true);
   dense7PatternButton->setCheckable(true);
   noBrushButton->setCheckable(true);
   horPatternButton->setCheckable(true);
   verPatternButton->setCheckable(true);
   crossPatternButton->setCheckable(true);
   bDiagPatternButton->setCheckable(true);
   fDiagPatternButton->setCheckable(true);
   diagCrossPatternButton->setCheckable(true);
   linearGradientPatternButton->setCheckable(true);
   radialGradientPatternButton->setCheckable(true);
   conicalGradientPatternButton->setCheckable(true);

   noBrushButton->setChecked(true);

   // draw standard pattern icons
   redrawStandardPatterns();

   // draw gradient button icons
   redrawGradientButtons();

   patternButtons = new QButtonGroup(this);

   patternButtons->addButton(solidPatternButton, static_cast<int>(Qt::SolidPattern));
   patternButtons->addButton(dense1PatternButton, static_cast<int>(Qt::Dense1Pattern));
   patternButtons->addButton(dense2PatternButton, static_cast<int>(Qt::Dense2Pattern));
   patternButtons->addButton(dense3PatternButton, static_cast<int>(Qt::Dense3Pattern));
   patternButtons->addButton(dense4PatternButton, static_cast<int>(Qt::Dense4Pattern));
   patternButtons->addButton(dense5PatternButton, static_cast<int>(Qt::Dense5Pattern));
   patternButtons->addButton(dense6PatternButton, static_cast<int>(Qt::Dense6Pattern));
   patternButtons->addButton(dense7PatternButton, static_cast<int>(Qt::Dense7Pattern));
   patternButtons->addButton(noBrushButton, static_cast<int>(Qt::NoBrush));
   patternButtons->addButton(horPatternButton, static_cast<int>(Qt::HorPattern));
   patternButtons->addButton(verPatternButton, static_cast<int>(Qt::VerPattern));
   patternButtons->addButton(crossPatternButton, static_cast<int>(Qt::CrossPattern));
   patternButtons->addButton(bDiagPatternButton, static_cast<int>(Qt::BDiagPattern));
   patternButtons->addButton(fDiagPatternButton, static_cast<int>(Qt::FDiagPattern));
   patternButtons->addButton(diagCrossPatternButton, static_cast<int>(Qt::DiagCrossPattern));
   patternButtons->addButton(linearGradientPatternButton, static_cast<int>(Qt::LinearGradientPattern));
   patternButtons->addButton(radialGradientPatternButton, static_cast<int>(Qt::RadialGradientPattern));
   patternButtons->addButton(conicalGradientPatternButton, static_cast<int>(Qt::ConicalGradientPattern));

   patternButtons->setExclusive(true);
   connect(patternButtons, SIGNAL(buttonClicked(int)), this, SLOT(somePatternButtonClicked(int)));

   // layout
   patternButtonsLayout = new QVBoxLayout;
   QGridLayout *patternGridLayout = new QGridLayout;

   patternGridLayout->addWidget(solidPatternButton, 0, 0);
   patternGridLayout->addWidget(dense1PatternButton, 0, 1);
   patternGridLayout->addWidget(dense2PatternButton, 0, 2);
   patternGridLayout->addWidget(dense3PatternButton, 0, 3);
   patternGridLayout->addWidget(dense4PatternButton, 0, 4);
   patternGridLayout->addWidget(dense5PatternButton, 0, 5);
   patternGridLayout->addWidget(dense6PatternButton, 1, 0);
   patternGridLayout->addWidget(dense7PatternButton, 1, 1);
   patternGridLayout->addWidget(noBrushButton, 1, 2);
   patternGridLayout->addWidget(horPatternButton, 1, 3);
   patternGridLayout->addWidget(verPatternButton, 1, 4);
   patternGridLayout->addWidget(crossPatternButton, 1, 5);
   patternGridLayout->addWidget(bDiagPatternButton, 2, 0);
   patternGridLayout->addWidget(fDiagPatternButton, 2, 1);
   patternGridLayout->addWidget(diagCrossPatternButton, 2, 2);
   patternGridLayout->addWidget(linearGradientPatternButton, 2, 3);
   patternGridLayout->addWidget(radialGradientPatternButton, 2, 4);
   patternGridLayout->addWidget(conicalGradientPatternButton, 2, 5);

   QGroupBox *patternGroups = new QGroupBox(this);
   patternGroups->setTitle(tr("Patterns"));
   patternGroups->setLayout(patternGridLayout);

   patternButtonsLayout->addSpacing(10);
   patternButtonsLayout->addWidget(patternGroups);
}

void CBrushSelector::arrangeSpreadButtons()
{
   // create buttons
   padSpreadButton = new QPushButton(this);
   repeatSpreadButton = new QPushButton(this);
   reflectSpreadButton = new QPushButton(this);
   angleSelector = new CAngleSelector(this);

   // set tooltip
   padSpreadButton->setToolTip(tr("Pad Spread : The area is filled with the closest stop color."));
   repeatSpreadButton->setToolTip(tr("Repeat Spread : The gradient is repeated outside the gradient area."));
   reflectSpreadButton->setToolTip(tr("Reflect Spread : The gradient is reflected outside the gradient area."));

   // set button sizes
   padSpreadButton->setIconSize(QSize(150, 225));
   repeatSpreadButton->setIconSize(QSize(150, 225));
   reflectSpreadButton->setIconSize(QSize(150, 225));
   padSpreadButton->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
   repeatSpreadButton->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
   reflectSpreadButton->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

   redrawGradientSpreadButtons(true);

   // set buttons to checkable, check pad spread as default
   padSpreadButton->setCheckable(true);
   repeatSpreadButton->setCheckable(true);
   reflectSpreadButton->setCheckable(true);
   padSpreadButton->setChecked(true);

   spreadButtons = new QButtonGroup(this);
   spreadButtons->addButton(padSpreadButton, static_cast<int>(QGradient::PadSpread));
   spreadButtons->addButton(repeatSpreadButton, static_cast<int>(QGradient::RepeatSpread));
   spreadButtons->addButton(reflectSpreadButton, static_cast<int>(QGradient::ReflectSpread));

   // make connections
   connect(spreadButtons, SIGNAL(buttonClicked(int)),
      this, SLOT(someSpreadButtonClicked(int)));

   connect(angleSelector, SIGNAL(setAngle(qreal)),
      this, SLOT(angleChange(qreal)));

   connect(angleSelector, SIGNAL(defineAngle(qreal)),
      this, SLOT(receiveDefinedAngle(qreal)));

   // define layout
   QHBoxLayout *spreadLayout = new QHBoxLayout;
   spreadLayout->addWidget(padSpreadButton);
   spreadLayout->addWidget(repeatSpreadButton);
   spreadLayout->addWidget(reflectSpreadButton);
   spreadLayout->addWidget(angleSelector);
   spreadLayout->addSpacing(50);

   QGroupBox *spreadGroup = new QGroupBox(this);
   spreadGroup->setTitle(tr("Gradient Spread"));
   spreadGroup->setLayout(spreadLayout);

   spreadButtonsLayout = new QVBoxLayout;
   spreadButtonsLayout->addWidget(spreadGroup);
   spreadButtonsLayout->addSpacing(15);
}

void CBrushSelector::redrawGradientSpreadButtons(bool force)
{
   redrawPadSpreadButton(force);
   redrawRepeatSpreadButton(force);
   redrawReflectSpreadButton(force);
}

void CBrushSelector::redrawPadSpreadButton(bool force)
{
   if ((0 != padSpreadButton && CDrawSettings_2D::getInstance()->brushSettings().getBrushStyle() == Qt::LinearGradientPattern) || force)
   {

      QPixmap tmpPixmap1 = QPixmap(QSize(150, 225));
      QPainter painter1(&tmpPixmap1);
      painter1.setRenderHint(QPainter::Antialiasing, true);

      if (drawBackground && 0 != m_gradientColors)
      {
         QPixmap *background = m_gradientColors->getBackGround();
         if (0 != background)
         {
            painter1.drawPixmap(tmpPixmap1.rect(), *background, tmpPixmap1.rect());
            painter1.setCompositionMode(QPainter::CompositionMode_SourceOver);
         }
         background = 0;
      }

      QLinearGradient linearGrad(QPointF(25, 37.5), QPointF(125, 187.5));

      if (CDrawSettings_2D::getInstance()->brushSettings().getGradientColors().size() < 2)
      {
         linearGrad.setColorAt(0, Qt::white);
         linearGrad.setColorAt(1, Qt::black);
      }
      else
      {
         QMap<qreal, QColor> _lincols = CDrawSettings_2D::getInstance()->brushSettings().getGradientColors();
         QMapIterator<qreal, QColor> it(_lincols);
         while (it.hasNext())
         {
            it.next();
            linearGrad.setColorAt(it.key(), it.value());
         }
      }
      linearGrad.setSpread(QGradient::PadSpread);
      QBrush brush1(linearGrad);
      painter1.fillRect(tmpPixmap1.rect(), brush1);
      painter1.end();
      padSpreadButton->setIcon(QIcon(tmpPixmap1));
   }
   else if (0 != padSpreadButton && CDrawSettings_2D::getInstance()->brushSettings().getBrushStyle() == Qt::RadialGradientPattern)
   {
      // icon for the linear gradient button
      QPixmap tmpPixmap1 = QPixmap(QSize(150, 225));
      QPainter painter1(&tmpPixmap1);
      painter1.setRenderHint(QPainter::Antialiasing, true);

      if (drawBackground && 0 != m_gradientColors)
      {
         QPixmap *background = m_gradientColors->getBackGround();
         if (0 != background)
         {
            painter1.drawPixmap(tmpPixmap1.rect(), *background, tmpPixmap1.rect());
            painter1.setCompositionMode(QPainter::CompositionMode_SourceOver);
         }
         background = 0;
      }

      QRadialGradient radGrad(QPointF(75, 112.5), 75);

      if (CDrawSettings_2D::getInstance()->brushSettings().getGradientColors().size() < 2)
      {
         radGrad.setColorAt(0, Qt::white);
         radGrad.setColorAt(1, Qt::black);
      }
      else
      {
         QMap<qreal, QColor> gradcols = CDrawSettings_2D::getInstance()->brushSettings().getGradientColors();
         QMapIterator<qreal, QColor> it(gradcols);
         while (it.hasNext())
         {
            it.next();
            radGrad.setColorAt(it.key(), it.value());
         }
      }
      radGrad.setSpread(QGradient::PadSpread);
      QBrush brush1(radGrad);
      painter1.fillRect(tmpPixmap1.rect(), brush1);
      painter1.end();
      padSpreadButton->setIcon(QIcon(tmpPixmap1));
   }
   else if (0 != padSpreadButton)
   {
      padSpreadButton->setIcon(QIcon(":/images/padSpread.png"));
   }
}

void CBrushSelector::redrawRepeatSpreadButton(bool force)
{
   if ((0 != repeatSpreadButton && CDrawSettings_2D::getInstance()->brushSettings().getBrushStyle() == Qt::LinearGradientPattern) || force)
   {

      QPixmap tmpPixmap1 = QPixmap(QSize(150, 225));
      QPainter painter1(&tmpPixmap1);
      painter1.setRenderHint(QPainter::Antialiasing, true);

      if (drawBackground && 0 != m_gradientColors)
      {
         QPixmap *background = m_gradientColors->getBackGround();
         if (0 != background)
         {
            painter1.drawPixmap(tmpPixmap1.rect(), *background, tmpPixmap1.rect());
            painter1.setCompositionMode(QPainter::CompositionMode_SourceOver);
         }
         background = 0;
      }

      QLinearGradient linearGrad(QPointF(25, 37.5), QPointF(125, 187.5));

      if (CDrawSettings_2D::getInstance()->brushSettings().getGradientColors().size() < 2)
      {
         linearGrad.setColorAt(0, Qt::white);
         linearGrad.setColorAt(1, Qt::black);
      }
      else
      {
         QMap<qreal, QColor> gradcols = CDrawSettings_2D::getInstance()->brushSettings().getGradientColors();
         QMapIterator<qreal, QColor> it(gradcols);
         while (it.hasNext())
         {
            it.next();
            linearGrad.setColorAt(it.key(), it.value());
         }
      }
      linearGrad.setSpread(QGradient::RepeatSpread);
      QBrush brush1(linearGrad);
      painter1.fillRect(tmpPixmap1.rect(), brush1);
      painter1.end();
      repeatSpreadButton->setIcon(QIcon(tmpPixmap1));
   }
   else if (0 != repeatSpreadButton && CDrawSettings_2D::getInstance()->brushSettings().getBrushStyle() == Qt::RadialGradientPattern)
   {
      // icon for the linear gradient button
      QPixmap tmpPixmap1 = QPixmap(QSize(150, 225));
      QPainter painter1(&tmpPixmap1);
      painter1.setRenderHint(QPainter::Antialiasing, true);

      if (drawBackground && 0 != m_gradientColors)
      {
         QPixmap *background = m_gradientColors->getBackGround();
         if (0 != background)
         {
            painter1.drawPixmap(tmpPixmap1.rect(), *background, tmpPixmap1.rect());
            painter1.setCompositionMode(QPainter::CompositionMode_SourceOver);
         }
         background = 0;
      }
      QRadialGradient radGrad(QPointF(75, 112.5), 75);

      if (CDrawSettings_2D::getInstance()->brushSettings().getGradientColors().size() < 2)
      {
         radGrad.setColorAt(0, Qt::white);
         radGrad.setColorAt(1, Qt::black);
      }
      else
      {
         QMap<qreal, QColor> gradcols = CDrawSettings_2D::getInstance()->brushSettings().getGradientColors();
         QMapIterator<qreal, QColor> it(gradcols);
         while (it.hasNext())
         {
            it.next();
            radGrad.setColorAt(it.key(), it.value());
         }
      }
      radGrad.setSpread(QGradient::RepeatSpread);
      QBrush brush1(radGrad);
      painter1.fillRect(tmpPixmap1.rect(), brush1);
      painter1.end();
      repeatSpreadButton->setIcon(QIcon(tmpPixmap1));
   }
   else if (0 != repeatSpreadButton)
   {
      repeatSpreadButton->setIcon(QIcon(":/images/repeatSpread.png"));
   }
}

void CBrushSelector::redrawReflectSpreadButton(bool force)
{
   if ((0 != reflectSpreadButton&& CDrawSettings_2D::getInstance()->brushSettings().getBrushStyle() == Qt::LinearGradientPattern) || force)
   {

      QPixmap tmpPixmap1 = QPixmap(QSize(150, 225));
      QPainter painter1(&tmpPixmap1);
      painter1.setRenderHint(QPainter::Antialiasing, true);

      if (drawBackground && 0 != m_gradientColors)
      {
         QPixmap *background = m_gradientColors->getBackGround();
         if (0 != background)
         {
            painter1.drawPixmap(tmpPixmap1.rect(), *background, tmpPixmap1.rect());
            painter1.setCompositionMode(QPainter::CompositionMode_SourceOver);
         }
         background = 0;
      }

      QLinearGradient linearGrad(QPointF(25, 37.5), QPointF(125, 187.5));

      if (CDrawSettings_2D::getInstance()->brushSettings().getGradientColors().size() < 2)
      {
         linearGrad.setColorAt(0, Qt::white);
         linearGrad.setColorAt(1, Qt::black);
      }
      else
      {
         QMap<qreal, QColor> gradcols = CDrawSettings_2D::getInstance()->brushSettings().getGradientColors();
         QMapIterator<qreal, QColor> it(gradcols);
         while (it.hasNext())
         {
            it.next();
            linearGrad.setColorAt(it.key(), it.value());
         }
      }
      linearGrad.setSpread(QGradient::ReflectSpread);
      QBrush brush1(linearGrad);
      painter1.fillRect(tmpPixmap1.rect(), brush1);
      painter1.end();
      reflectSpreadButton->setIcon(QIcon(tmpPixmap1));
   }
   else if (0 != reflectSpreadButton && CDrawSettings_2D::getInstance()->brushSettings().getBrushStyle() == Qt::RadialGradientPattern)
   {
      // icon for the linear gradient button
      QPixmap tmpPixmap1 = QPixmap(QSize(150, 225));
      QPainter painter1(&tmpPixmap1);
      painter1.setRenderHint(QPainter::Antialiasing, true);

      if (drawBackground && 0 != m_gradientColors)
      {
         QPixmap *background = m_gradientColors->getBackGround();
         if (0 != background)
         {
            painter1.drawPixmap(tmpPixmap1.rect(), *background, tmpPixmap1.rect());
            painter1.setCompositionMode(QPainter::CompositionMode_SourceOver);
         }
         background = 0;
      }

      QRadialGradient radGrad(QPointF(75, 112.5), 75);

      if (CDrawSettings_2D::getInstance()->brushSettings().getGradientColors().size() < 2)
      {
         radGrad.setColorAt(0, Qt::white);
         radGrad.setColorAt(1, Qt::black);
      }
      else
      {
         QMap<qreal, QColor> gradcols = CDrawSettings_2D::getInstance()->brushSettings().getGradientColors();
         QMapIterator<qreal, QColor> it(gradcols);
         while (it.hasNext())
         {
            it.next();
            radGrad.setColorAt(it.key(), it.value());
         }
      }
      radGrad.setSpread(QGradient::ReflectSpread);
      QBrush brush1(radGrad);
      painter1.fillRect(tmpPixmap1.rect(), brush1);
      painter1.end();
      reflectSpreadButton->setIcon(QIcon(tmpPixmap1));
   }
   else if (0 != reflectSpreadButton)
   {
      reflectSpreadButton->setIcon(QIcon(":/images/reflectSpread.png"));
   }
}

void CBrushSelector::arrangeGradientColors()
{
   gradientColorsLabel = 0;
   m_gradientColors = new CGradientColors(this);

   connect(m_gradientColors, SIGNAL(addGradientColor(qreal, const QColor &)),
      this, SLOT(addGradientColor(qreal, const QColor &)));

   connect(m_gradientColors, SIGNAL(setGradientColor(qreal, qreal, const QColor &)),
      this, SLOT(setGradientColor(qreal, qreal, const QColor &)));

   connect(m_gradientColors, SIGNAL(removeGradientColor(qreal)),
      this, SLOT(removeGradientColor(qreal)));

   connect(m_gradientColors, SIGNAL(gradientReset()),
      this, SLOT(gradientReset()));

   connect(m_gradientColors, SIGNAL(backgroundChanged(bool)),
      this, SLOT(backgroundChanged(bool)));

   connect(m_gradientColors, SIGNAL(setSingleColor(const QColor &)),
      this, SLOT(setSingleColor(const QColor &)));

   gradientColorsLayout = new QVBoxLayout;
   gradientColorsLayout->addWidget(m_gradientColors);
}

void CBrushSelector::addGradientColor(qreal, const QColor &)
{
   updateSettings();
   redrawGradientButtons();
}

void CBrushSelector::setGradientColor(qreal lastPos, qreal pos, const QColor &color)
{
   updateSettings();
   redrawGradientButtons();
   
}

void CBrushSelector::removeGradientColor(qreal pos)
{
   updateSettings();
   redrawGradientButtons();
}

void CBrushSelector::redrawGradientButtons()
{
   redrawGradientSpreadButtons();

   // icon for the linear gradient button
   QPixmap tmpPixmap15 = QPixmap(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT);
   QPainter painter15(&tmpPixmap15);

   if (drawBackground && 0 != m_gradientColors)
   {
      QPixmap *background = m_gradientColors->getBackGround();
      if (0 != background)
      {
         painter15.drawPixmap(tmpPixmap15.rect(), *background, tmpPixmap15.rect());
         painter15.setCompositionMode(QPainter::CompositionMode_SourceOver);
      }
      background = 0;
   }

   QLinearGradient linearGrad(QPointF(0.0, 0.0), QPointF(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT));

   if (CDrawSettings_2D::getInstance()->brushSettings().getGradientColors().size() < 2)
   {
      linearGrad.setColorAt(0, Qt::white);
      linearGrad.setColorAt(1, Qt::black);
   }
   else
   {
      QMap<qreal, QColor> gradcols = CDrawSettings_2D::getInstance()->brushSettings().getGradientColors();
      QMapIterator<qreal, QColor> it(gradcols);
      while (it.hasNext())
      {
         it.next();
         linearGrad.setColorAt(it.key(), it.value());
      }
   }
   QBrush brush15(linearGrad);
   painter15.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), brush15);
   linearGradientPatternButton->setIcon(QIcon(tmpPixmap15));

   // icon for the radial gradient button
   QPixmap tmpPixmap16 = QPixmap(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT);
   QPainter painter16(&tmpPixmap16);

   if (drawBackground && 0 != m_gradientColors)
   {
      QPixmap *background = m_gradientColors->getBackGround();
      if (0 != background)
      {
         painter16.drawPixmap(tmpPixmap16.rect(), *background, tmpPixmap16.rect());
         painter16.setCompositionMode(QPainter::CompositionMode_SourceOver);
      }
      background = 0;
   }

   QRadialGradient radialGrad(QPointF((qreal)default_BRUSH_BUTTON_WIDTH / 2.0, (qreal)default_BRUSH_BUTTON_HEIGHT / 2.0), (qreal)default_BRUSH_BUTTON_WIDTH / 2.0);
   if (CDrawSettings_2D::getInstance()->brushSettings().getGradientColors().size() < 2)
   {
      radialGrad.setColorAt(0, Qt::white);
      radialGrad.setColorAt(1, Qt::black);
   }
   else
   {
      QMap<qreal, QColor> gradcols = CDrawSettings_2D::getInstance()->brushSettings().getGradientColors();
      QMapIterator<qreal, QColor> it(gradcols);
      while (it.hasNext())
      {
         it.next();
         radialGrad.setColorAt(it.key(), it.value());
      }
   }
   QBrush brush16(radialGrad);
   painter16.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), brush16);
   radialGradientPatternButton->setIcon(QIcon(tmpPixmap16));

   if (0 != angleSelector)
   {
      angleChange(angleSelector->getAngle());
   }
   else
   {
      angleChange(90);
   }

   update();
}

void CBrushSelector::gradientReset()
{
   if (0 == m_gradientColors)
   {
      return;
   }
   CBrushSettings brs = CDrawSettings_2D::getInstance()->brushSettings();
   brs.setBrushColor(Qt::black);
   QMap<qreal, QColor> colors;
   if (m_gradientColors->getAllColors(colors))
   {
      brs.setGradientColors(colors);
   }
   if (0 != angleSelector)
   {
      angleSelector->defAngle(90);
      brs.setAngle(90);
   }
   CDrawSettings_2D::getInstance()->setBrushSettings(brs);
   redrawStandardPatterns();
   redrawGradientButtons();
}

void CBrushSelector::backgroundChanged(bool onOrOff)
{
   drawBackground = onOrOff;
   redrawGradientButtons();
}

void CBrushSelector::somePatternButtonClicked(int id)
{
   CBrushSettings brs = CDrawSettings_2D::getInstance()->brushSettings();
   Qt::BrushStyle tmpStyle = static_cast<Qt::BrushStyle>(id);
   brs.setBrushStyle(tmpStyle);
   CDrawSettings_2D::getInstance()->setBrushSettings(brs);

   if (tmpStyle == Qt::LinearGradientPattern || tmpStyle == Qt::RadialGradientPattern || tmpStyle == Qt::ConicalGradientPattern)
   {
      if (0 != m_gradientColors)
      {
         m_gradientColors->setMultiColorMode();
      }
      if (tmpStyle == Qt::ConicalGradientPattern && 0 != angleSelector)
      {
         redrawGradientSpreadButtons(true);
         angleSelector->setEnabled(true);
         if (0 != padSpreadButton) padSpreadButton->setEnabled(false);
         if (0 != repeatSpreadButton) repeatSpreadButton->setEnabled(false);
         if (0 != reflectSpreadButton) reflectSpreadButton->setEnabled(false);
      }
      else if (0 != angleSelector)
      {
         redrawGradientSpreadButtons();
         angleSelector->setEnabled(false);
         if (0 != padSpreadButton) padSpreadButton->setEnabled(true);
         if (0 != repeatSpreadButton) repeatSpreadButton->setEnabled(true);
         if (0 != reflectSpreadButton) reflectSpreadButton->setEnabled(true);
      }

      //if(0 != m_gradientColors) m_gradientColors->setEnabled(true);
   }
   else
   {
      if (0 != m_gradientColors)
      {
         m_gradientColors->setSingleColorMode();
      }
      if (0 != angleSelector) angleSelector->setEnabled(false);
      if (0 != padSpreadButton) padSpreadButton->setEnabled(false);
      if (0 != repeatSpreadButton) repeatSpreadButton->setEnabled(false);
      if (0 != reflectSpreadButton) reflectSpreadButton->setEnabled(false);
      //if(0 != m_gradientColors   ) m_gradientColors   ->setEnabled(false);
   }
}

void CBrushSelector::someSpreadButtonClicked(int id)
{
   CBrushSettings brs = CDrawSettings_2D::getInstance()->brushSettings();
   QGradient::Spread tmpSpread = static_cast<QGradient::Spread>(id);
   brs.setBrushSpread(tmpSpread);
   CDrawSettings_2D::getInstance()->setBrushSettings(brs);
}

//  The angle of the angleWidget changed, update the conical gradient icon and
//  emit the change
void CBrushSelector::angleChange(qreal angle)
{
   // icon for the conical gradient button
   QPixmap tmpPixmap17 = QPixmap(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT);
   QPainter painter17(&tmpPixmap17);

   if (drawBackground && 0 != m_gradientColors)
   {
      QPixmap *background = m_gradientColors->getBackGround();
      if (0 != background)
      {
         painter17.drawPixmap(tmpPixmap17.rect(), *background, tmpPixmap17.rect());
         painter17.setCompositionMode(QPainter::CompositionMode_SourceOver);
      }
      background = 0;
   }

   QConicalGradient conicalGrad(QPointF((qreal)default_BRUSH_BUTTON_WIDTH / 2.0, (qreal)default_BRUSH_BUTTON_HEIGHT / 2.0), angle);
   if (CDrawSettings_2D::getInstance()->brushSettings().getGradientColors().size() < 2)
   {
      conicalGrad.setColorAt(0, Qt::white);
      conicalGrad.setColorAt(1, Qt::black);
   }
   else
   {
      QMap<qreal, QColor> gradcols = CDrawSettings_2D::getInstance()->brushSettings().getGradientColors();
      QMapIterator<qreal, QColor> it(gradcols);
      while (it.hasNext())
      {
         it.next();
         conicalGrad.setColorAt(it.key(), it.value());
      }
   }
   QBrush brush17(conicalGrad);
   painter17.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), brush17);
   conicalGradientPatternButton->setIcon(QIcon(tmpPixmap17));
}

void CBrushSelector::receiveDefinedAngle(qreal angle)
{
   CBrushSettings brs = CDrawSettings_2D::getInstance()->brushSettings();
   brs.setAngle(angle);
   CDrawSettings_2D::getInstance()->setBrushSettings(brs);
}

void CBrushSelector::setSingleColor(const QColor &color)
{
   CBrushSettings brs = CDrawSettings_2D::getInstance()->brushSettings();
   brs.setBrushColor(color);
   CDrawSettings_2D::getInstance()->setBrushSettings(brs);
   redrawStandardPatterns();
}

void CBrushSelector::redrawStandardPatterns()
{
   //QPixmap *background = 0;
   QColor color = CDrawSettings_2D::getInstance()->brushSettings().getBrushColor();

   //if(drawBackground && 0 != m_gradientColors)
   //{
   //   background = m_gradientColors->getBackGround();
   //}

   if (0 != solidPatternButton)
   {
      // icon for the solid button
      QPixmap tmpPixmap = QPixmap(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT);
      QPainter painter(&tmpPixmap);
      //if(0 != background)
      //{
      //   painter.drawPixmap(tmpPixmap.rect(), *background, tmpPixmap.rect());
      //   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
      //}
      QBrush brush(Qt::SolidPattern);
      brush.setColor(color);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), Qt::white);
      painter.fillRect(tmpPixmap.rect(), brush);
      painter.end();
      solidPatternButton->setIcon(QIcon(tmpPixmap));
   }

   if (0 != noBrushButton)
   {
      // icon for the no brush button
      QPixmap tmpPixmap = QPixmap(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT);
      QPainter painter(&tmpPixmap);
      //if(0 != background)
      //{
      //   painter.drawPixmap(tmpPixmap.rect(), *background, tmpPixmap.rect());
      //   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
      //}
      QBrush brush(Qt::NoBrush);
      brush.setColor(color);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), Qt::white);
      painter.fillRect(tmpPixmap.rect(), brush);
      painter.end();

      noBrushButton->setIcon(QIcon(tmpPixmap));
   }

   if (0 != dense1PatternButton)
   {
      // icon for the dense 1 button
      QPixmap tmpPixmap = QPixmap(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT);
      QPainter painter(&tmpPixmap);
      //if(0 != background)
      //{
      //   painter.drawPixmap(tmpPixmap.rect(), *background, tmpPixmap.rect());
      //   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
      //}
      QBrush brush(Qt::Dense1Pattern);
      brush.setColor(color);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), Qt::white);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), brush);
      dense1PatternButton->setIcon(QIcon(tmpPixmap));
   }

   if (0 != dense2PatternButton)
   {
      // icon for the dense 2 button
      QPixmap tmpPixmap = QPixmap(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT);
      QPainter painter(&tmpPixmap);

      //if(0 != background)
      //{
      //   painter.drawPixmap(tmpPixmap.rect(), *background, tmpPixmap.rect());
      //   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
      //}

      QBrush brush(Qt::Dense2Pattern);
      brush.setColor(color);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), Qt::white);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), brush);
      dense2PatternButton->setIcon(QIcon(tmpPixmap));
   }

   if (0 != dense3PatternButton)
   {
      // icon for the dense 3 button
      QPixmap tmpPixmap = QPixmap(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT);
      QPainter painter(&tmpPixmap);

      //if(0 != background)
      //{
      //   painter.drawPixmap(tmpPixmap.rect(), *background, tmpPixmap.rect());
      //   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
      //}

      QBrush brush(Qt::Dense3Pattern);
      brush.setColor(color);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), Qt::white);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), brush);
      dense3PatternButton->setIcon(QIcon(tmpPixmap));
   }

   if (0 != dense4PatternButton)
   {
      // icon for the dense 4 button
      QPixmap tmpPixmap = QPixmap(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT);
      QPainter painter(&tmpPixmap);

      //if(0 != background)
      //{
      //   painter.drawPixmap(tmpPixmap.rect(), *background, tmpPixmap.rect());
      //   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
      //}

      QBrush brush(Qt::Dense4Pattern);
      brush.setColor(color);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), Qt::white);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), brush);
      dense4PatternButton->setIcon(QIcon(tmpPixmap));
   }

   if (0 != dense5PatternButton)
   {
      // icon for the dense 5 button
      QPixmap tmpPixmap = QPixmap(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT);
      QPainter painter(&tmpPixmap);

      //if(0 != background)
      //{
      //   painter.drawPixmap(tmpPixmap.rect(), *background, tmpPixmap.rect());
      //   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
      //}

      QBrush brush(Qt::Dense5Pattern);
      brush.setColor(color);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), Qt::white);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), brush);
      dense5PatternButton->setIcon(QIcon(tmpPixmap));
   }

   if (0 != dense6PatternButton)
   {
      // icon for the dense 6 button
      QPixmap tmpPixmap = QPixmap(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT);
      QPainter painter(&tmpPixmap);

      //if(0 != background)
      //{
      //   painter.drawPixmap(tmpPixmap.rect(), *background, tmpPixmap.rect());
      //   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
      //}

      QBrush brush(Qt::Dense6Pattern);
      brush.setColor(color);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), Qt::white);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), brush);
      dense6PatternButton->setIcon(QIcon(tmpPixmap));
   }

   if (0 != dense7PatternButton)
   {
      // icon for the dense 7 button
      QPixmap tmpPixmap = QPixmap(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT);
      QPainter painter(&tmpPixmap);

      //if(0 != background)
      //{
      //   painter.drawPixmap(tmpPixmap.rect(), *background, tmpPixmap.rect());
      //   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
      //}

      QBrush brush(Qt::Dense7Pattern);
      brush.setColor(color);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), Qt::white);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), brush);
      dense7PatternButton->setIcon(QIcon(tmpPixmap));
   }

   if (0 != horPatternButton)
   {
      // icon for the horizontal lines button
      QPixmap tmpPixmap = QPixmap(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT);
      QPainter painter(&tmpPixmap);

      //if(0 != background)
      //{
      //   painter.drawPixmap(tmpPixmap.rect(), *background, tmpPixmap.rect());
      //   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
      //}

      QBrush brush(Qt::HorPattern);
      brush.setColor(color);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), Qt::white);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), brush);
      horPatternButton->setIcon(QIcon(tmpPixmap));
   }

   if (0 != verPatternButton)
   {
      // icon for the vertical lines button
      QPixmap tmpPixmap = QPixmap(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT);
      QPainter painter(&tmpPixmap);

      //if(0 != background)
      //{
      //   painter.drawPixmap(tmpPixmap.rect(), *background, tmpPixmap.rect());
      //   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
      //}

      QBrush brush(Qt::VerPattern);
      brush.setColor(color);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), Qt::white);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), brush);
      verPatternButton->setIcon(QIcon(tmpPixmap));
   }

   if (0 != crossPatternButton)
   {
      // icon for the cross pattern button
      QPixmap tmpPixmap = QPixmap(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT);
      QPainter painter(&tmpPixmap);

      //if(0 != background)
      //{
      //   painter.drawPixmap(tmpPixmap.rect(), *background, tmpPixmap.rect());
      //   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
      //}

      QBrush brush(Qt::CrossPattern);
      brush.setColor(color);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), Qt::white);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), brush);
      crossPatternButton->setIcon(QIcon(tmpPixmap));
   }

   if (0 != bDiagPatternButton)
   {
      // icon for the backwards diagonal lines button
      QPixmap tmpPixmap = QPixmap(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT);
      QPainter painter(&tmpPixmap);

      //if(0 != background)
      //{
      //   painter.drawPixmap(tmpPixmap.rect(), *background, tmpPixmap.rect());
      //   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
      //}

      QBrush brush(Qt::BDiagPattern);
      brush.setColor(color);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), Qt::white);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), brush);
      bDiagPatternButton->setIcon(QIcon(tmpPixmap));
   }

   if (0 != fDiagPatternButton)
   {
      // icon for the forward diagonal lines button
      QPixmap tmpPixmap = QPixmap(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT);
      QPainter painter(&tmpPixmap);

      //if(0 != background)
      //{
      //   painter.drawPixmap(tmpPixmap.rect(), *background, tmpPixmap.rect());
      //   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
      //}

      QBrush brush(Qt::FDiagPattern);
      brush.setColor(color);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), Qt::white);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), brush);
      fDiagPatternButton->setIcon(QIcon(tmpPixmap));
   }

   if (0 != diagCrossPatternButton)
   {
      // icon for the dense crossing diagonal lines button
      QPixmap tmpPixmap = QPixmap(default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT);
      QPainter painter(&tmpPixmap);

      //if(0 != background)
      //{
      //   painter.drawPixmap(tmpPixmap.rect(), *background, tmpPixmap.rect());
      //   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
      //}

      QBrush brush(Qt::DiagCrossPattern);
      brush.setColor(color);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), Qt::white);
      painter.fillRect(QRectF(0.0, 0.0, default_BRUSH_BUTTON_WIDTH, default_BRUSH_BUTTON_HEIGHT), brush);
      diagCrossPatternButton->setIcon(QIcon(tmpPixmap));
   }

   // background = 0;
}

void CBrushSelector::mouseReleaseEvent(QMouseEvent *)
{
   QApplication::setActiveWindow(this);
}

void CBrushSelector::hideEvent(QHideEvent *)
{
   QMap<qreal, QColor> colors;
   CBrushSettings brs = CDrawSettings_2D::getInstance()->brushSettings();
   if (m_gradientColors->getAllColors(colors))
   {
      brs.setGradientColors(colors);
   }
   brs.setAngle(angleSelector->getAngle());
   CDrawSettings_2D::getInstance()->setBrushSettings(brs);
}