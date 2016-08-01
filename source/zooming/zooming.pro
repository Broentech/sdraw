TOP_DIR = ../../
SCRIPT_DIR = $$(TOP_DIR)/scripts

include(zooming.pri)
QT += widgets

TEMPLATE = lib                 
CONFIG  += staticlib              
CONFIG  += debug_and_release  
CONFIG  += build_all           

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

DESTDIR  = $$TEMPDIR/$$LIBDIR  
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

INSTALL_DIR     = $${TOP_DIR}/install
INSTALL_BIN_DIR = $${INSTALL_DIR}/bin
INSTALL_LIB_DIR = $${INSTALL_DIR}/lib
INSTALL_INC_DIR = $${INSTALL_DIR}/include
INSTALL_DOC_DIR = $${INSTALL_DIR}/doc

build_pass:CONFIG(debug, debug|release) {
  win32: TARGET = $$join(BASENAME,,,d)
} else {
  build_pass:CONFIG(release, debug|release) {
    win32: TARGET = $$BASENAME
  }
}

allclean.commands = rm -rf $$TEMPDIR        
allclean.depends  = distclean vsclean

vsclean.commands  = rm -f *.vcproj*        

QMAKE_EXTRA_TARGETS += allclean
QMAKE_EXTRA_TARGETS += vsclean

target.path   = $$INSTALL_LIB_DIR

headers.files = $$HEADERS
headers.path  = $$INSTALL_INC_DIR/$$PROJNAME

INSTALLS += target
INSTALLS += headers
