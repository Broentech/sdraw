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
//  \file      main.cpp
//
//  \brief     Application entry point
//
//  \author    Stian Broen
//
//  \date      26.03.2011
//
//  \par       Copyright:
//
//             Stian Broen
//
//  \par       Revision History
//
//  \par       01 26.03.2011 Original version
//
//             
//
//
//h-//////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QApplication>
#include <QDir>

// local includes
#include "mainWindow.h"

using namespace sdraw;

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);
   CMainWindow *mainWindow = new CMainWindow;
   mainWindow->setWindowIcon(QIcon(":/icons/palette.png"));
   mainWindow->setWindowTitle("Broentech Sdraw");
   mainWindow->setWindowState(Qt::WindowMaximized);
   mainWindow->show();
   return app.exec();
}