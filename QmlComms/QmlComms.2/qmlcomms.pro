TARGET = qmlcommsplugin
TEMPLATE = lib
CONFIG += plugin

QT += qml network

SOURCES += \
    ../QmlComms/qmlcomms.cpp \
    plugin.cpp

HEADERS += \
    ../QmlComms/qmlcomms.h
