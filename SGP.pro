QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bonus.cpp \
    cell.cpp \
    fieldlogger.cpp \
    gamefield.cpp \
    gamefielditerator.cpp \
    main.cpp \
    mainwindow.cpp \
    player.cpp \
    playerlogger.cpp \
    saveload.cpp

HEADERS += \
    bonus.h \
    cell.h \
    enemy.h \
    fieldlogger.h \
    file.h \
    gamefield.h \
    gamefielditerator.h \
    mainwindow.h \
    player.h \
    playerlogger.h \
    saveload.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    images/rectangle2.png \
    rectangle2.png

RESOURCES += \
    images.qrc
