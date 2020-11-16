QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = soccer_result_simltr
TEMPLATE = app

CONFIG += c++2a

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    sqlitedbeditor.cpp \
    teams.cpp

HEADERS += \
    mainwindow.h \
    sqlitedbeditor.h \
    teams.h

FORMS += \
    mainwindow.ui \
    sqlitedbeditor.ui


TRANSLATIONS += \
    soccer_result_simltr_en_DE.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = appIcon.ico

RESOURCES += \
    misc.qrc \
    rsc.qrc

DISTFILES += \
    misc_icons/World_Map.png \
    misc_icons/button.png \
    misc_icons/button_disabled.png \
    misc_icons/button_pressed.png \
    misc_icons/draw.jpg \
    misc_icons/filesave.png \
    misc_icons/filesave_128x128.png \
    misc_icons/filesave_128x128_disabled.png \
    misc_icons/filesave_128x128_pressed.png \
    misc_icons/icon_Qt_78x78px.png \
    misc_icons/loose.jpg \
    misc_icons/mysql.png \
    misc_icons/win.jpg
