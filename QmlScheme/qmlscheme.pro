TARGET = QmlScheme/qmlschemeplugin
TEMPLATE = lib
CONFIG += plugin

QT += core qml

LIBS += -lchicken

HEADERS += \
    qmlscheme_p.h

SOURCES += \
    plugin.cpp \
    qmlscheme.cpp

