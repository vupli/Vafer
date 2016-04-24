QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Vafer
TEMPLATE = app

LIBS += -LC:/Qt/SFML/lib

CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics  -lsfml-window -lsfml-system
CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-window-d -lsfml-system-d

INCLUDEPATH += C:/Qt/SFML/include
DEPENDPATH += C:/Qt/SFML/include

SOURCES += main.cpp \
	mainwindow.cpp \
    source/TinyXML/tinystr.cpp \
    source/TinyXML/tinyxml.cpp \
    source/TinyXML/tinyxmlerror.cpp \
    source/TinyXML/tinyxmlparser.cpp \
    options.cpp \
    credits.cpp \
    screensaver.cpp

HEADERS  += mainwindow.h \
    source/TinyXML/tinystr.h \
    source/TinyXML/tinyxml.h \
    source/Animation.hpp \
    source/Bullet.hpp \
    source/Enemy.hpp \
    source/Entity.hpp \
    source/Game.hpp \
    source/HealthBar.hpp \
    source/level.hpp \
    source/MovingPlatform.hpp \
    source/Player.hpp \
    options.h \
    credits.h \
    screensaver.h

FORMS    += mainwindow.ui \
    options.ui \
    credits.ui \
    screensaver.ui

RESOURCES += \
    image.qrc
