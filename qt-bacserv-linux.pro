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


INCLUDEPATH += "../bacnet-src/src/"
INCLUDEPATH += "../bacnet-src/src/bacnet/basic/object/"
INCLUDEPATH += "../bacnet-src/src/bacnet/basic/service/"

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

SOURCES += \
	main.cpp \
	mainwindow.cpp \
	../bacnet-src/src/bacnet/basic/object/device.c \
	../bacnet-src/src/bacnet/basic/object/ai.c \
	../bacnet-src/src/bacnet/basic/object/ao.c \
	../bacnet-src/src/bacnet/basic/object/av.c \
	../bacnet-src/src/bacnet/basic/object/bi.c \
	../bacnet-src/src/bacnet/basic/object/bo.c \
	../bacnet-src/src/bacnet/basic/object/blo.c \
	../bacnet-src/src/bacnet/basic/object/bv.c \
	../bacnet-src/src/bacnet/basic/object/calendar.c \
	../bacnet-src/src/bacnet/basic/object/channel.c \
	../bacnet-src/src/bacnet/basic/object/color_object.c \
	../bacnet-src/src/bacnet/basic/object/color_temperature.c \
	../bacnet-src/src/bacnet/basic/object/command.c \
	../bacnet-src/src/bacnet/basic/object/csv.c \
	../bacnet-src/src/bacnet/basic/object/iv.c \
	../bacnet-src/src/bacnet/basic/object/lc.c \
	../bacnet-src/src/bacnet/basic/object/lo.c \
	../bacnet-src/src/bacnet/basic/object/lsp.c \
	../bacnet-src/src/bacnet/basic/object/ms-input.c \
	../bacnet-src/src/bacnet/basic/object/mso.c \
	../bacnet-src/src/bacnet/basic/object/msv.c \
	../bacnet-src/src/bacnet/basic/object/osv.c \
	../bacnet-src/src/bacnet/basic/object/piv.c \
	../bacnet-src/src/bacnet/basic/object/nc.c  \
	../bacnet-src/src/bacnet/basic/object/netport.c  \
	../bacnet-src/src/bacnet/basic/object/time_value.c \
	../bacnet-src/src/bacnet/basic/object/trendlog.c \
	../bacnet-src/src/bacnet/basic/object/schedule.c \
	../bacnet-src/src/bacnet/basic/object/access_credential.c \
	../bacnet-src/src/bacnet/basic/object/access_door.c \
	../bacnet-src/src/bacnet/basic/object/access_point.c \
	../bacnet-src/src/bacnet/basic/object/access_rights.c \
	../bacnet-src/src/bacnet/basic/object/access_user.c \
	../bacnet-src/src/bacnet/basic/object/access_zone.c \
	../bacnet-src/src/bacnet/basic/object/credential_data_input.c \
	../bacnet-src/src/bacnet/basic/object/acc.c \
	../bacnet-src/src/bacnet/basic/object/bacfile.c
	../bacnet-src/src/bacnet/basic/service/h_ts.c \

HEADERS += \
    mainwindow.h \
    ../bacnet-src/src/bacnet/basic/object/device.h \
    ../bacnet-src/src/bacnet/basic/service/h_ts.h \

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$PWD/../bacnet-src/apps/lib/ -lbacnet

INCLUDEPATH += $$PWD/../bacnet-src/apps/lib
DEPENDPATH += $$PWD/../bacnet-src/apps/lib

unix:!macx: PRE_TARGETDEPS += $$PWD/../bacnet-src/apps/lib/libbacnet.a

QTPLUGIN += qlinuxfb
