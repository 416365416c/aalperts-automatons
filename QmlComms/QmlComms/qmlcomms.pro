TARGET = qmlcommsplugin
TEMPLATE = lib
CONFIG += plugin

QT += declarative network

SOURCES += \
    qmlcomms.cpp \
    plugin.cpp

HEADERS += \
    qmlcomms.h
