//h+//////////////////////////////////////////////////////////////////////////
/*!
//  \file      shapeSelector.cpp
//
//  \brief     Declaration of a class to draw shape
//
//  \author    Stian Broen
//
//  \date      September-2011
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
*/
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QDir>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QPixmap>
#include <QPainter>
#include <QPoint>
#include <QLabel>

// solution includes
#include <common/definitions.h>
#include <drawSettings/DrawSettings_2D.h>

// local includes
#include "shapeSelector.h"

using namespace sdraw;

CShapeSelector::CShapeSelector(QWidget *parent)
   : QWidget(parent)
{
   CDrawSettings_2D::getInstance()->setShape(SNoShape);
   CDrawSettings_2D::getInstance()->setRoundedRectRadius_X(10);
   CDrawSettings_2D::getInstance()->setRoundedRectRadius_Y(10);

   closeButton = new QToolButton(this);
   closeButton->setToolTip("Close Menu");
   closeButton->setIcon(QIcon(":/images/blackCancel.png"));
   connect(closeButton, SIGNAL(clicked(bool)), this, SLOT(closeMenu(bool)));
   QHBoxLayout *closeLayout = new QHBoxLayout;
   closeLayout->addWidget(closeButton);
   closeLayout->setAlignment(closeButton, Qt::AlignRight);

   arrangeShapeButtons();

   totalLayout = new QVBoxLayout;
   totalLayout->addLayout(closeLayout);
   totalLayout->addLayout(allShapesLayouts);
   setLayout(totalLayout);
}

CShapeSelector::~CShapeSelector()
{

}

int CShapeSelector::getXRad()
{
   return CDrawSettings_2D::getInstance()->getRoundedRectRadius_X();
}

int CShapeSelector::getYRad()
{
   return CDrawSettings_2D::getInstance()->getRoundedRectRadius_Y();
}

void CShapeSelector::setXRad(int radius)
{
   if(radius >= 0 && radius <= MAX_X_RADIUS)
   {
      //xRad = radius;
      CDrawSettings_2D::getInstance()->setRoundedRectRadius_X(radius);
      QPixmap tmpPixmap3 = background;
      QPainter painter3( &tmpPixmap3 );
      painter3.drawRoundedRect(QRect(10, 10, default_SHAPE_BUTTON_WIDTH - 20, default_SHAPE_BUTTON_HEIGHT - 20), 
         CDrawSettings_2D::getInstance()->getRoundedRectRadius_X(), 
         CDrawSettings_2D::getInstance()->getRoundedRectRadius_Y());

      roundedRectShapeButton->setIcon(QIcon(tmpPixmap3));
   }

   //emit xRadChange(xRad);
}

void CShapeSelector::setYRad(int radius)
{
   if(radius >= 0 && radius <= MAX_Y_RADIUS)
   {
      //yRad = radius;
      CDrawSettings_2D::getInstance()->setRoundedRectRadius_Y(radius);
      QPixmap tmpPixmap3 = background;
      QPainter painter3( &tmpPixmap3 );
      painter3.drawRoundedRect(QRect(10,10,default_SHAPE_BUTTON_WIDTH - 20, default_SHAPE_BUTTON_HEIGHT - 20), 
         CDrawSettings_2D::getInstance()->getRoundedRectRadius_X(),
         CDrawSettings_2D::getInstance()->getRoundedRectRadius_Y());

      roundedRectShapeButton->setIcon(QIcon(tmpPixmap3));
   }

   //emit yRadChange(yRad);
}

void CShapeSelector::setShape(shapeType newType)
{
   lineShapeButton       ->setChecked(false);
   rectShapeButton       ->setChecked(false);
   roundedRectShapeButton->setChecked(false);
   ellipseShapeButton    ->setChecked(false);
   arcShapeButton        ->setChecked(false);
   pieShapeButton        ->setChecked(false);
   chordShapeButton      ->setChecked(false);
   polylineShapeButton   ->setChecked(false);
   polygonShapeButton    ->setChecked(false);

   switch(newType)
   {
      case(SLine):
      {
         lineShapeButton->setChecked(true);
         CDrawSettings_2D::getInstance()->setShape(SNoShape);
         //currShape = SNoShape;
         break;
      }
      case(SRectangle):
      {
         rectShapeButton->setChecked(true);
         CDrawSettings_2D::getInstance()->setShape(SRectangle);
         //currShape = SRectangle;
         break;
      }
      case(SRoundedRectangle):
      {
         roundedRectShapeButton->setChecked(true);
         CDrawSettings_2D::getInstance()->setShape(SRoundedRectangle);
         //currShape = SRoundedRectangle;
         break;
      }
      case(SEllipse):
      {
         ellipseShapeButton->setChecked(true);
         CDrawSettings_2D::getInstance()->setShape(SEllipse);
         //currShape = SEllipse;
         break;
      }
      case(SArc):
      {
         arcShapeButton->setChecked(true);
         CDrawSettings_2D::getInstance()->setShape(SArc);
         //currShape = SArc;
         break;
      }
      case(SPie):
      {
         pieShapeButton->setChecked(true);
         CDrawSettings_2D::getInstance()->setShape(SPie);
         //currShape = SPie;
         break;
      }
      case(SChord):
      {
         chordShapeButton->setChecked(true);
         CDrawSettings_2D::getInstance()->setShape(SChord);
         //currShape = SChord;
         break;
      }
      case(SPolyline):
      {
         polylineShapeButton->setChecked(true);
         CDrawSettings_2D::getInstance()->setShape(SPolyline);
         //currShape = SPolyline;
         break;
      }
      case(SPolygon):
      {
         polygonShapeButton->setChecked(true);
         CDrawSettings_2D::getInstance()->setShape(SPolygon);
         //currShape = SPolygon;
         break;
      }
      default:
      {
         CDrawSettings_2D::getInstance()->setShape(SNoShape);
         //currShape = SNoShape;
         break;
      }
   }
}

shapeType CShapeSelector::getShape()
{
   return CDrawSettings_2D::getInstance()->getShape();
   //return currShape;
}

void CShapeSelector::arrangeShapeButtons()
{
   // create top label
   //QLabel *shapeLabel = new QLabel(this);
   //shapeLabel->setText("Shapes");

   // create buttons
   noShapeButton          = new QPushButton(this);
   lineShapeButton        = new QPushButton(this);
   rectShapeButton        = new QPushButton(this);
   ellipseShapeButton     = new QPushButton(this);
   arcShapeButton         = new QPushButton(this);
   pieShapeButton         = new QPushButton(this);
   chordShapeButton       = new QPushButton(this);
   polylineShapeButton    = new QPushButton(this);
   polygonShapeButton     = new QPushButton(this);

   roundedRectShapeButton = new QPushButton(this);
   xRadSelector = new CAngleSelector(this,default_SHAPE_BUTTON_WIDTH + 10,default_SHAPE_BUTTON_WIDTH + 10);
   yRadSelector = new CAngleSelector(this,default_SHAPE_BUTTON_WIDTH + 10,default_SHAPE_BUTTON_WIDTH + 10);
   xRadSelector->defAngle((qreal)CDrawSettings_2D::getInstance()->getRoundedRectRadius_X());
   yRadSelector->defAngle((qreal)CDrawSettings_2D::getInstance()->getRoundedRectRadius_Y());
   xRadSelector->setMax(MAX_X_RADIUS);
   yRadSelector->setMax(MAX_Y_RADIUS);

   // set tooltip
   noShapeButton         ->setToolTip(tr("No shapes, free draw"                ));
   lineShapeButton       ->setToolTip(tr("Draw lines"                          ));
   rectShapeButton       ->setToolTip(tr("Draw rectangles"                     ));
   roundedRectShapeButton->setToolTip(tr("Draw rectangles with rounded corners"));
   ellipseShapeButton    ->setToolTip(tr("Draw ellipses"                       ));
   arcShapeButton        ->setToolTip(tr("Draw arcs"                           ));
   pieShapeButton        ->setToolTip(tr("Draw pies"                           ));
   chordShapeButton      ->setToolTip(tr("Draw chords"                         ));
   polylineShapeButton   ->setToolTip(tr("Draw polylines"                      ));
   polygonShapeButton    ->setToolTip(tr("Draw polygon"                        ));

   // set button sizes
   noShapeButton         ->setIconSize(QSize(default_SHAPE_BUTTON_WIDTH, default_SHAPE_BUTTON_HEIGHT));
   lineShapeButton       ->setIconSize(QSize(default_SHAPE_BUTTON_WIDTH, default_SHAPE_BUTTON_HEIGHT));
   rectShapeButton       ->setIconSize(QSize(default_SHAPE_BUTTON_WIDTH, default_SHAPE_BUTTON_HEIGHT));
   roundedRectShapeButton->setIconSize(QSize(default_SHAPE_BUTTON_WIDTH, default_SHAPE_BUTTON_HEIGHT));
   ellipseShapeButton    ->setIconSize(QSize(default_SHAPE_BUTTON_WIDTH, default_SHAPE_BUTTON_HEIGHT));
   arcShapeButton        ->setIconSize(QSize(default_SHAPE_BUTTON_WIDTH, default_SHAPE_BUTTON_HEIGHT));
   pieShapeButton        ->setIconSize(QSize(default_SHAPE_BUTTON_WIDTH, default_SHAPE_BUTTON_HEIGHT));
   chordShapeButton      ->setIconSize(QSize(default_SHAPE_BUTTON_WIDTH, default_SHAPE_BUTTON_HEIGHT));
   polylineShapeButton   ->setIconSize(QSize(default_SHAPE_BUTTON_WIDTH, default_SHAPE_BUTTON_HEIGHT));
   polygonShapeButton    ->setIconSize(QSize(default_SHAPE_BUTTON_WIDTH, default_SHAPE_BUTTON_HEIGHT));

   noShapeButton         ->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
   lineShapeButton       ->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
   rectShapeButton       ->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
   roundedRectShapeButton->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
   ellipseShapeButton    ->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
   arcShapeButton        ->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
   pieShapeButton        ->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
   chordShapeButton      ->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
   polylineShapeButton   ->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
   polygonShapeButton    ->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));

   // set checkable
   noShapeButton         ->setCheckable(true);
   noShapeButton         ->setChecked  (true);
   lineShapeButton       ->setCheckable(true);
   rectShapeButton       ->setCheckable(true);
   roundedRectShapeButton->setCheckable(true);
   ellipseShapeButton    ->setCheckable(true);
   arcShapeButton        ->setCheckable(true);
   pieShapeButton        ->setCheckable(true);
   chordShapeButton      ->setCheckable(true);
   polylineShapeButton   ->setCheckable(true);
   polygonShapeButton    ->setCheckable(true);

   // make button background
   background = QPixmap(default_SHAPE_BUTTON_WIDTH,default_SHAPE_BUTTON_HEIGHT);
   QLinearGradient linearGrad(QPointF(0.0,0.0), QPointF(default_SHAPE_BUTTON_WIDTH, default_SHAPE_BUTTON_HEIGHT));
   linearGrad.setColorAt(0, Qt::lightGray);
   linearGrad.setColorAt(1, Qt::white);
   QBrush backgroundBrush(linearGrad);
   QPainter backgroundPainter(&background);
   backgroundPainter.fillRect(QRectF(0,0,default_SHAPE_BUTTON_WIDTH, default_SHAPE_BUTTON_HEIGHT), backgroundBrush);

   // make icons
   noShapeButton->setIcon(QIcon(background));

   QPixmap tmpPixmap1 = background;
   QPainter painter1( &tmpPixmap1 );
   painter1.setRenderHint(QPainter::Antialiasing,true);
   painter1.drawLine(QPoint(5,5), QPoint(default_SHAPE_BUTTON_WIDTH - 5, default_SHAPE_BUTTON_HEIGHT - 5));
   lineShapeButton->setIcon(QIcon(tmpPixmap1));

   QPixmap tmpPixmap2 = background;
   QPainter painter2( &tmpPixmap2 );
   painter2.setRenderHint(QPainter::Antialiasing,true);
   painter2.drawRect(QRect(10,10,default_SHAPE_BUTTON_WIDTH - 20, default_SHAPE_BUTTON_HEIGHT - 20));
   rectShapeButton->setIcon(QIcon(tmpPixmap2));

   QPixmap tmpPixmap4 = background;
   QPainter painter4( &tmpPixmap4 );
   painter4.setRenderHint(QPainter::Antialiasing,true);
   painter4.drawEllipse(QRect(10,10,default_SHAPE_BUTTON_WIDTH - 20, default_SHAPE_BUTTON_HEIGHT - 20));
   ellipseShapeButton->setIcon(QIcon(tmpPixmap4));

   QPixmap tmpPixmap5 = background;
   QPainter painter5( &tmpPixmap5 );
   painter5.setRenderHint(QPainter::Antialiasing,true);
   painter5.drawArc(QRect(10,10,default_SHAPE_BUTTON_WIDTH - 20, default_SHAPE_BUTTON_HEIGHT - 20), 30 * 16, 120 * 16);
   arcShapeButton->setIcon(QIcon(tmpPixmap5));

   QPixmap tmpPixmap6 = background;
   QPainter painter6( &tmpPixmap6 );
   painter6.setRenderHint(QPainter::Antialiasing,true);
   painter6.drawPie(QRect(10,10,default_SHAPE_BUTTON_WIDTH - 20, default_SHAPE_BUTTON_HEIGHT - 20), 30 * 16, 120 * 16);
   pieShapeButton->setIcon(QIcon(tmpPixmap6));

   QPixmap tmpPixmap7 = background;
   QPainter painter7( &tmpPixmap7 );
   painter7.setRenderHint(QPainter::Antialiasing,true);
   painter7.drawChord(QRect(10,10,default_SHAPE_BUTTON_WIDTH - 20, default_SHAPE_BUTTON_HEIGHT - 20), 30 * 16, 120 * 16);
   chordShapeButton->setIcon(QIcon(tmpPixmap7));

   static const QPointF points[4] = {
      QPointF(10.0, 10.0),
      QPointF(default_SHAPE_BUTTON_WIDTH - 10, 20.0),
      QPointF(default_SHAPE_BUTTON_WIDTH - 30.0, default_SHAPE_BUTTON_HEIGHT - 10),
      QPointF(default_SHAPE_BUTTON_WIDTH/2.0, default_SHAPE_BUTTON_HEIGHT/2.0),
   };
   QPixmap tmpPixmap8 = background;
   QPainter painter8( &tmpPixmap8 );
   painter8.setRenderHint(QPainter::Antialiasing,true);
   painter8.drawPolyline(points, 4);
   polylineShapeButton->setIcon(QIcon(tmpPixmap8));

   QPixmap tmpPixmap9 = background;
   QPainter painter9( &tmpPixmap9 );
   painter9.setRenderHint(QPainter::Antialiasing,true);
   painter9.drawPolygon(points, 4);
   polygonShapeButton->setIcon(QIcon(tmpPixmap9));

   QPixmap tmpPixmap3 = background;
   QPainter painter3( &tmpPixmap3 );
   painter3.setRenderHint(QPainter::Antialiasing,true);
   painter3.drawRoundedRect(QRect(10, 10, default_SHAPE_BUTTON_WIDTH - 20, default_SHAPE_BUTTON_HEIGHT - 20), 
      CDrawSettings_2D::getInstance()->getRoundedRectRadius_X(), 
      CDrawSettings_2D::getInstance()->getRoundedRectRadius_Y());

   roundedRectShapeButton->setIcon(QIcon(tmpPixmap3));

   // make connections
   connect(noShapeButton         , SIGNAL(clicked(bool)), this, SLOT(noShapeButtonClicked         (bool)));
   connect(lineShapeButton       , SIGNAL(clicked(bool)), this, SLOT(lineShapeButtonClicked       (bool)));
   connect(rectShapeButton       , SIGNAL(clicked(bool)), this, SLOT(rectShapeButtonButtonClicked (bool)));
   connect(ellipseShapeButton    , SIGNAL(clicked(bool)), this, SLOT(ellipseShapeButtonClicked    (bool)));
   connect(arcShapeButton        , SIGNAL(clicked(bool)), this, SLOT(arcShapeButtonClicked        (bool)));
   connect(pieShapeButton        , SIGNAL(clicked(bool)), this, SLOT(pieShapeButtonClicked        (bool)));
   connect(chordShapeButton      , SIGNAL(clicked(bool)), this, SLOT(chordShapeButtonClicked      (bool)));
   connect(polylineShapeButton   , SIGNAL(clicked(bool)), this, SLOT(polylineShapeButtonClicked   (bool)));
   connect(polygonShapeButton    , SIGNAL(clicked(bool)), this, SLOT(polygonShapeButtonClicked    (bool)));
   connect(roundedRectShapeButton, SIGNAL(clicked(bool)), this, SLOT(roundedRectShapeButtonClicked(bool)));

   connect(xRadSelector, SIGNAL(setAngle(qreal)), this, SLOT(xRadChange(qreal)));
   connect(yRadSelector, SIGNAL(setAngle(qreal)), this, SLOT(yRadChange(qreal)));

   // make button description labels
   QLabel *descriptionNoShapeLabel     = new QLabel(tr("No Shape"         ), this);
   QLabel *descriptionLineLabel        = new QLabel(tr("Line"             ), this);
   QLabel *descriptionRectLabel        = new QLabel(tr("Rectangle"        ), this);
   QLabel *descriptionEllipseLabel     = new QLabel(tr("Ellipse"          ), this);
   QLabel *descriptionArcLabel         = new QLabel(tr("Arc"              ), this);
   QLabel *descriptionPieLabel         = new QLabel(tr("Pie"              ), this);
   QLabel *descriptionChordLabel       = new QLabel(tr("Chord"            ), this);
   QLabel *descriptionPolylineLabel    = new QLabel(tr("Polyline"         ), this);
   QLabel *descriptionPolygonLabel     = new QLabel(tr("Polygon"          ), this);
   QLabel *descriptionRoundedRectLabel = new QLabel(tr("Rounded Rectangle"), this);
   QLabel *descriptionXRadLabel        = new QLabel(tr("X Radius"         ), this);
   QLabel *descriptionYRadLabel        = new QLabel(tr("Y Radius"         ), this);

   // description labels layout
   QHBoxLayout *roundedRectDescriptionLayout = new QHBoxLayout;
   roundedRectDescriptionLayout->addSpacing(5);
   roundedRectDescriptionLayout->addWidget(descriptionRoundedRectLabel);
   roundedRectDescriptionLayout->addSpacing(35);
   roundedRectDescriptionLayout->addWidget(descriptionXRadLabel);
   roundedRectDescriptionLayout->addSpacing(20);
   roundedRectDescriptionLayout->addWidget(descriptionYRadLabel);
   //roundedRectDescriptionLayout->addSpacing(200);

   QHBoxLayout *topDescriptionLayout = new QHBoxLayout;
   topDescriptionLayout->addSpacing(30);
   topDescriptionLayout->addWidget(descriptionNoShapeLabel);
   topDescriptionLayout->addSpacing(45);
   topDescriptionLayout->addWidget(descriptionRectLabel);
   topDescriptionLayout->addSpacing(60);
   topDescriptionLayout->addWidget(descriptionLineLabel);
   topDescriptionLayout->addSpacing(5);

   QHBoxLayout *midDescriptionLayout = new QHBoxLayout;
   midDescriptionLayout->addSpacing(40);
   midDescriptionLayout->addWidget(descriptionEllipseLabel);
   midDescriptionLayout->addSpacing(50);
   midDescriptionLayout->addWidget(descriptionArcLabel);
   midDescriptionLayout->addSpacing(40);
   midDescriptionLayout->addWidget(descriptionPieLabel);
   midDescriptionLayout->addSpacing(1);

   QHBoxLayout *bottomDescriptionLayout = new QHBoxLayout;
   bottomDescriptionLayout->addSpacing(40);
   bottomDescriptionLayout->addWidget(descriptionChordLabel);
   bottomDescriptionLayout->addSpacing(30);
   bottomDescriptionLayout->addWidget(descriptionPolylineLabel);
   bottomDescriptionLayout->addSpacing(30);
   bottomDescriptionLayout->addWidget(descriptionPolygonLabel);
   bottomDescriptionLayout->addSpacing(1);

   // layout
   allShapesLayouts = new QVBoxLayout;

   QHBoxLayout *layoutTop = new QHBoxLayout;
   layoutTop->addWidget(noShapeButton);
   layoutTop->addWidget(rectShapeButton);
   layoutTop->addWidget(lineShapeButton);
   

   QHBoxLayout *layoutMid = new QHBoxLayout;
   layoutMid->addWidget(ellipseShapeButton);
   layoutMid->addWidget(arcShapeButton);
   layoutMid->addWidget(pieShapeButton);

   QHBoxLayout *layoutBottom = new QHBoxLayout;
   layoutBottom->addWidget(chordShapeButton);
   layoutBottom->addWidget(polylineShapeButton);
   layoutBottom->addWidget(polygonShapeButton);

   QHBoxLayout *layoutRoundedRect = new QHBoxLayout;
   layoutRoundedRect->addWidget(roundedRectShapeButton);
   layoutRoundedRect->addWidget(xRadSelector);
   layoutRoundedRect->addWidget(yRadSelector);
   layoutRoundedRect->addSpacing(5);

   //allShapesLayouts->addSpacing(10);
   //allShapesLayouts->addWidget(shapeLabel);

   allShapesLayouts->addLayout(layoutTop);
   allShapesLayouts->addLayout(topDescriptionLayout);
   allShapesLayouts->addSpacing(15);
   allShapesLayouts->addLayout(layoutMid);
   allShapesLayouts->addLayout(midDescriptionLayout);
   allShapesLayouts->addSpacing(15);
   allShapesLayouts->addLayout(layoutBottom);
   allShapesLayouts->addLayout(bottomDescriptionLayout);
   allShapesLayouts->addSpacing(15);

   allShapesLayouts->addLayout(layoutRoundedRect);
   allShapesLayouts->addLayout(roundedRectDescriptionLayout);
   allShapesLayouts->addSpacing(15);

}

void CShapeSelector::noShapeButtonClicked         (bool)
{
   noShapeButton         ->setChecked(true);
   lineShapeButton       ->setChecked(false);
   rectShapeButton       ->setChecked(false);
   roundedRectShapeButton->setChecked(false);
   ellipseShapeButton    ->setChecked(false);
   arcShapeButton        ->setChecked(false);
   pieShapeButton        ->setChecked(false);
   chordShapeButton      ->setChecked(false);
   polylineShapeButton   ->setChecked(false);
   polygonShapeButton    ->setChecked(false);

   CDrawSettings_2D::getInstance()->setShape(SNoShape);
   //currShape = SNoShape;
   //emit shapeChangeSig(currShape);
}

void CShapeSelector::lineShapeButtonClicked       (bool)
{
   noShapeButton         ->setChecked(false);
   lineShapeButton       ->setChecked(true);
   rectShapeButton       ->setChecked(false);
   roundedRectShapeButton->setChecked(false);
   ellipseShapeButton    ->setChecked(false);
   arcShapeButton        ->setChecked(false);
   pieShapeButton        ->setChecked(false);
   chordShapeButton      ->setChecked(false);
   polylineShapeButton   ->setChecked(false);
   polygonShapeButton    ->setChecked(false);

   CDrawSettings_2D::getInstance()->setShape(SLine);
   //currShape = SLine;
   //emit shapeChangeSig(currShape);
}

void CShapeSelector::rectShapeButtonButtonClicked (bool)
{
   noShapeButton         ->setChecked(false);
   lineShapeButton       ->setChecked(false);
   rectShapeButton       ->setChecked(true);
   roundedRectShapeButton->setChecked(false);
   ellipseShapeButton    ->setChecked(false);
   arcShapeButton        ->setChecked(false);
   pieShapeButton        ->setChecked(false);
   chordShapeButton      ->setChecked(false);
   polylineShapeButton   ->setChecked(false);
   polygonShapeButton    ->setChecked(false);

   CDrawSettings_2D::getInstance()->setShape(SRectangle);
   //currShape = SRectangle;
   //emit shapeChangeSig(currShape);
}

void CShapeSelector::roundedRectShapeButtonClicked(bool)
{
   noShapeButton         ->setChecked(false);
   lineShapeButton       ->setChecked(false);
   rectShapeButton       ->setChecked(false);
   roundedRectShapeButton->setChecked(true);
   ellipseShapeButton    ->setChecked(false);
   arcShapeButton        ->setChecked(false);
   pieShapeButton        ->setChecked(false);
   chordShapeButton      ->setChecked(false);
   polylineShapeButton   ->setChecked(false);
   polygonShapeButton    ->setChecked(false);

   CDrawSettings_2D::getInstance()->setShape(SRoundedRectangle);
   //currShape = SRoundedRectangle;
   //emit shapeChangeSig(currShape);
}

void CShapeSelector::ellipseShapeButtonClicked    (bool)
{
   noShapeButton         ->setChecked(false);
   lineShapeButton       ->setChecked(false);
   rectShapeButton       ->setChecked(false);
   roundedRectShapeButton->setChecked(false);
   ellipseShapeButton    ->setChecked(true);
   arcShapeButton        ->setChecked(false);
   pieShapeButton        ->setChecked(false);
   chordShapeButton      ->setChecked(false);
   polylineShapeButton   ->setChecked(false);
   polygonShapeButton    ->setChecked(false);

   CDrawSettings_2D::getInstance()->setShape(SEllipse);
   //currShape = SEllipse;
   //emit shapeChangeSig(currShape);
}

void CShapeSelector::arcShapeButtonClicked        (bool)
{
   noShapeButton         ->setChecked(false);
   lineShapeButton       ->setChecked(false);
   rectShapeButton       ->setChecked(false);
   roundedRectShapeButton->setChecked(false);
   ellipseShapeButton    ->setChecked(false);
   arcShapeButton        ->setChecked(true);
   pieShapeButton        ->setChecked(false);
   chordShapeButton      ->setChecked(false);
   polylineShapeButton   ->setChecked(false);
   polygonShapeButton    ->setChecked(false);

   CDrawSettings_2D::getInstance()->setShape(SArc);
   //currShape = SArc;
   //emit shapeChangeSig(currShape);
}

void CShapeSelector::pieShapeButtonClicked        (bool)
{
   noShapeButton         ->setChecked(false);
   lineShapeButton       ->setChecked(false);
   rectShapeButton       ->setChecked(false);
   roundedRectShapeButton->setChecked(false);
   ellipseShapeButton    ->setChecked(false);
   arcShapeButton        ->setChecked(false);
   pieShapeButton        ->setChecked(true);
   chordShapeButton      ->setChecked(false);
   polylineShapeButton   ->setChecked(false);
   polygonShapeButton    ->setChecked(false);

   CDrawSettings_2D::getInstance()->setShape(SPie);
   //currShape = SPie;
   //emit shapeChangeSig(currShape);
}

void CShapeSelector::chordShapeButtonClicked      (bool)
{
   noShapeButton         ->setChecked(false);
   lineShapeButton       ->setChecked(false);
   rectShapeButton       ->setChecked(false);
   roundedRectShapeButton->setChecked(false);
   ellipseShapeButton    ->setChecked(false);
   arcShapeButton        ->setChecked(false);
   pieShapeButton        ->setChecked(false);
   chordShapeButton      ->setChecked(true);
   polylineShapeButton   ->setChecked(false);
   polygonShapeButton    ->setChecked(false);

   CDrawSettings_2D::getInstance()->setShape(SChord);
   //currShape = SChord;
   //emit shapeChangeSig(currShape);
}

void CShapeSelector::polylineShapeButtonClicked   (bool)
{
   noShapeButton         ->setChecked(false);
   lineShapeButton       ->setChecked(false);
   rectShapeButton       ->setChecked(false);
   roundedRectShapeButton->setChecked(false);
   ellipseShapeButton    ->setChecked(false);
   arcShapeButton        ->setChecked(false);
   pieShapeButton        ->setChecked(false);
   chordShapeButton      ->setChecked(false);
   polylineShapeButton   ->setChecked(true);
   polygonShapeButton    ->setChecked(false);

   CDrawSettings_2D::getInstance()->setShape(SPolyline);
   //currShape = SPolyline;
   //emit shapeChangeSig(currShape);
}

void CShapeSelector::polygonShapeButtonClicked    (bool)
{
   noShapeButton         ->setChecked(false);
   lineShapeButton       ->setChecked(false);
   rectShapeButton       ->setChecked(false);
   roundedRectShapeButton->setChecked(false);
   ellipseShapeButton    ->setChecked(false);
   arcShapeButton        ->setChecked(false);
   pieShapeButton        ->setChecked(false);
   chordShapeButton      ->setChecked(false);
   polylineShapeButton   ->setChecked(false);
   polygonShapeButton    ->setChecked(true);

   CDrawSettings_2D::getInstance()->setShape(SPolygon);
   //currShape = SPolygon;
   //emit shapeChangeSig(currShape);
}

void CShapeSelector::xRadChange(qreal angle)
{
   CDrawSettings_2D::getInstance()->setRoundedRectRadius_X(angle);
   //xRad = angle;
   QPixmap tmpPixmap3 = background;
   QPainter painter3( &tmpPixmap3 );
   painter3.setRenderHint(QPainter::Antialiasing,true);
   painter3.drawRoundedRect(QRect(10,10,default_SHAPE_BUTTON_WIDTH - 20, default_SHAPE_BUTTON_HEIGHT - 20), 
      CDrawSettings_2D::getInstance()->getRoundedRectRadius_X(),
      CDrawSettings_2D::getInstance()->getRoundedRectRadius_Y()
      );

   roundedRectShapeButton->setIcon(QIcon(tmpPixmap3));
   roundedRectShapeButtonClicked(true);
   //emit xRadChangeSig(xRad);
}

void CShapeSelector::yRadChange(qreal angle)
{
   CDrawSettings_2D::getInstance()->setRoundedRectRadius_Y(angle);
   //yRad = angle;
   QPixmap tmpPixmap3 = background;
   QPainter painter3( &tmpPixmap3 );
   painter3.setRenderHint(QPainter::Antialiasing,true);
   painter3.drawRoundedRect(QRect(10,10,default_SHAPE_BUTTON_WIDTH - 20, default_SHAPE_BUTTON_HEIGHT - 20), 
      CDrawSettings_2D::getInstance()->getRoundedRectRadius_X(),
      CDrawSettings_2D::getInstance()->getRoundedRectRadius_Y()
      );
   roundedRectShapeButton->setIcon(QIcon(tmpPixmap3));
   roundedRectShapeButtonClicked(true);
   //emit yRadChangeSig(yRad);
}

void CShapeSelector::closeMenu(bool)
{
   emit doCloseShapeMenuSig();
}