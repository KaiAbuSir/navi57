#-------------------------------------------------
# navi57 -- Project created by Kai Neufeldt, 19 Nov 2010
#-------------------------------------------------

TARGET = navi57
TEMPLATE = app

CONFIG += console

INCLUDEPATH += incl
INCLUDEPATH += ../EncLib/incl

win32 {
DEFINES += _CRT_SECURE_NO_WARNINGS
}

#rem: EncLib is build with different compilers!
win32 {
CONFIG(release, debug|release){
#  LIBS += -L../EncLib-build/$${QMAKE_CXX}/release   #kai: rel.Path: EncLib cannot be loaded!
  LIBS += -LD:/Projekte/EncLib-build/$${QMAKE_CXX}/release
  LIBS += -LD:../geographiclib-1.1/windows/Release
  message("Building release ")
}
CONFIG(debug, debug|release){
#  LIBS += -L../EncLib-build/$${QMAKE_CXX}/debug #kai: rel.Path: EncLib cannot be loaded!
  LIBS += -LD:/Projekte/EncLib-build/$${QMAKE_CXX}/debug
  LIBS += -LD:../geographiclib-1.1/windows/Debug
  message("Building debug")
}
    #just for Visual Studio to find the Qt-dlls
    LIBS += -LC:\Qt\4.7.1.vc2008\lib
}
unix {
    CONFIG(release, debug|release){
      LIBS += -L../EncLib-build/$${QMAKE_CXX}/release
    }
    CONFIG(debug, debug|release){
      LIBS += -L../EncLib-build/$${QMAKE_CXX}/debug
    }
}
message(" Project Path: " $$PWD )
message("qmake version: " $$QMAKE_QMAKE " - Compiler: "  $$QMAKE_CXX " - Spec: " $${QMAKESPEC} )

LIBS += -lEncLib
LIBS += -lGeographicLib

SOURCES += src/main.cpp\
        src/n57_app.cpp\
        src/n57_mainwindow.cpp

HEADERS  += incl/n57_app.h \
    incl/n57_globals.h\
    incl/n57_mainwindow.h\
    incl/n57_texts.incl

RESOURCES += \
    navi57Qt.qrc
