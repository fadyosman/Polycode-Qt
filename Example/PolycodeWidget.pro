#-------------------------------------------------
#
# Project created by QtCreator 2014-05-04T17:52:09
#
#-------------------------------------------------

QT       += core gui opengl concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PolycodeWidget
TEMPLATE = app

#polycode libs and includes.
LIBS += -lrt -ldl -lpthread /home/fady/Apps/Polycode/Release/Linux/Framework/Core/lib/libPolycore.a /home/fady/Apps/Polycode/Release/Linux/Framework/Core/Dependencies/lib/libfreetype.a /home/fady/Apps/Polycode/Release/Linux/Framework/Core/Dependencies/lib/liblibvorbisfile.a /home/fady/Apps/Polycode/Release/Linux/Framework/Core/Dependencies/lib/liblibvorbis.a /home/fady/Apps/Polycode/Release/Linux/Framework/Core/Dependencies/lib/liblibogg.a /home/fady/Apps/Polycode/Release/Linux/Framework/Core/Dependencies/lib/libopenal.so /home/fady/Apps/Polycode/Release/Linux/Framework/Core/Dependencies/lib/libphysfs.a /home/fady/Apps/Polycode/Release/Linux/Framework/Core/Dependencies/lib/libpng15.a /home/fady/Apps/Polycode/Release/Linux/Framework/Core/Dependencies/lib/libz.a -lGL -lGLU -lSDL /home/fady/Apps/Polycode/Release/Linux/Framework/Modules/lib/libPolycode2DPhysics.a /home/fady/Apps/Polycode/Release/Linux/Framework/Modules/Dependencies/lib/libBox2D.a /home/fady/Apps/Polycode/Release/Linux/Framework/Modules/lib/libPolycode3DPhysics.a /home/fady/Apps/Polycode/Release/Linux/Framework/Modules/Dependencies/lib/libBulletDynamics.a /home/fady/Apps/Polycode/Release/Linux/Framework/Modules/Dependencies/lib/libBulletCollision.a /home/fady/Apps/Polycode/Release/Linux/Framework/Modules/Dependencies/lib/libLinearMath.a -lX11

INCLUDEPATH += /home/fady/Apps/Polycode/Release/Linux/Framework/Core/Dependencies/include /home/fady/Apps/Polycode/Release/Linux/Framework/Core/Dependencies/include/AL /home/fady/Apps/Polycode/Release/Linux/Framework/Core/include /home/fady/Apps/Polycode/Release/Linux/Framework/Modules/include /home/fady/Apps/Polycode/Release/Linux/Framework/Modules/Dependencies/include /home/fady/Apps/Polycode/Release/Linux/Framework/Modules/Dependencies/include/bullet ../Src




SOURCES += main.cpp\
    ../Src/polycodewidget.cpp \
    inputhandlers.cpp

HEADERS  += \
    ../Src/polycodewidget.h \
    inputhandlers.h

FORMS    +=

OTHER_FILES += \
    ptr.png
