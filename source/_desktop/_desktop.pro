TOP_DIR = ../../
SCRIPT_DIR = $$(TOP_DIR)/scripts

INSTALL_DIR     = $${TOP_DIR}/install
INSTALL_BIN_DIR = $${INSTALL_DIR}/bin
INSTALL_LIB_DIR = $${INSTALL_DIR}/lib
INSTALL_INC_DIR = $${INSTALL_DIR}/include
INSTALL_DOC_DIR = $${INSTALL_DIR}/doc

INCLUDEPATH *= $$PWD/src
INCLUDEPATH += $$PWD/../

include($$PWD/../common/common.pri)
include($$PWD/../algorithms/algorithms.pri)
include($$PWD/../sdrawWidgets/sdrawWidgets.pri)
include($$PWD/../drawboard/drawboard.pri)
include($$PWD/../zooming/zooming.pri)
include($$PWD/../drawLib/drawLib.pri)
include($$PWD/../drawCommands/drawCommands.pri)
include($$PWD/../drawSettings/drawSettings.pri)
include($$PWD/../_debugStuff/_debugStuff.pri)

build_pass:CONFIG(debug, debug|release) 
{
}


build_pass:CONFIG(release, debug|release) 
{
}

TEMPLATE = app                
CONFIG  += debug_and_release  
CONFIG  += build_all          

QT += widgets
QT += printsupport

PROJNAME = $$basename(PWD)   
BASENAME = $$PROJNAME        

TEMP = $$PWD/src/$$PROJNAME/*.h     
for(a,TEMP) {
   exists($$a) {
      HEADERS *= $$a
   }
}

TEMP = $$PWD/src/$$PROJNAME/*.cpp   
for(a,TEMP) {
   exists($$a) {
      SOURCES *= $$a
   }
}

TEMP = $$PWD/src/ui/*.ui           
for(a,TEMP) {
   exists($$a) {
      FORMS *= $$a
   }
}

TEMP = $$PWD/src/rc/*.qrc           
for(a,TEMP) {
   exists($$a) {
      RESOURCES *= $$a
   }
}

TEMPDIR  = $$PWD/tmp
DESTDIR  = $$TEMPDIR/bin
MOC_DIR  = $$TEMPDIR/moc
UI_DIR   = $$TEMPDIR/uic
RCC_DIR  = $$TEMPDIR/rcc
build_pass:CONFIG(debug, debug|release) {
  OBJECTS_DIR = $$TEMPDIR/obj/debug
} else {
  build_pass:CONFIG(release, debug|release) {
    OBJECTS_DIR = $$TEMPDIR/obj/release
  }
}

build_pass:CONFIG(debug, debug|release) {
  win32:TARGET = $$join(BASENAME,,,d)
} else {
  build_pass:CONFIG(release, debug|release) {
    win32:TARGET = $$BASENAME
  }
}

allclean.commands = rm -rf $$TEMPDIR         
allclean.depends  = distclean vsclean

vsclean.commands  = rm -f *.vcproj*         

QMAKE_EXTRA_TARGETS += allclean
QMAKE_EXTRA_TARGETS += vsclean

target.path = $$INSTALL_BIN_DIR

INSTALLS += target
