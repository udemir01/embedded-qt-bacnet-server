QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += \
"DBACDL_BIP=1" \
"DBBMD_ENABLED=1" \
"DBBMD_CLIENT_ENABLED" \
"DPRINT_ENABLED=1" \
"DBACAPP_ALL" \
"DBACFILE" \
"DINTRINSIC_REPORTING" \
"DBACNET_TIME_MASTER" \
"DBACNET_PROPERTY_LISTS=1" \
"DBACNET_PROTOCOL_REVISION=24" \

QMAKE_CC = gcc
QMAKE_CXX = g++

QMAKE_CFLAGS += \
-Os \
-std=gnu89 \
-Wall -Wextra -Wfloat-equal -Wconversion -Wparentheses \
-pedantic -Wunused-parameter -Wunused-variable -Wreturn-type \
-Wunused-function -Wreturn-type -Wunused-value \
-Wswitch-default -Wuninitialized -Winit-self \
-Wno-sign-conversion -Wno-conversion -Wno-sign-compare \
-Wno-long-long -Wno-implicit-fallthrough -Wno-attributes \
-ffunction-sections -fdata-sections \

QMAKE_LFLAGS = -Wl,-lc,-lgcc,-lrt,-lm,--gc-sections
QMAKE_PFLAGS = -pthread

INCLUDEPATH += $$PWD/src/
INCLUDEPATH += $$PWD/src/bacnet/
INCLUDEPATH += $$PWD/src/bacnet/service

SOURCES += \
main.cpp \
mainwindow.cpp \
bacnetserver.cpp \
$$PWD/src/bacnet/*.c \
$$PWD/src/bacnet/basic/npdu/*.c \
$$PWD/src/bacnet/datalink/*.c \
$$PWD/src/bacnet/basic/object/*.c \
$$PWD/src/bacnet/basic/service/*.c \
$$PWD/src/bacnet/basic/tsm/*.c \
$$PWD/src/bacnet/basic/bbmd/*.c \
$$PWD/src/bacnet/basic/bbmd6/*.c \
$$PWD/src/bacnet/basic/binding/*.c \
$$PWD/src/bacnet/basic/client/*.c \
$$PWD/src/bacnet/basic/sys/*.c \
$$PWD/ports/linux/*-init.c \

HEADERS += \
mainwindow.h \
bacnetserver.h \
$$PWD/src/bacnet/*.h \
$$PWD/src/bacnet/basic/npdu/*.h \
$$PWD/src/bacnet/basic/services.h \
$$PWD/src/bacnet/datalink/*.h \
$$PWD/src/bacnet/basic/object/*.h \
$$PWD/src/bacnet/basic/service/*.h \
$$PWD/src/bacnet/basic/tsm/*.h \
$$PWD/src/bacnet/basic/bbmd/*.h \
$$PWD/src/bacnet/basic/bbmd6/*.h \
$$PWD/src/bacnet/basic/binding/*.h \
$$PWD/src/bacnet/basic/client/*.h \
$$PWD/src/bacnet/basic/sys/*.h \

FORMS += \
mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QTPLUGIN += qlinuxfb
