TEMPLATE = app
TARGET = bragi
macx: CONFIG -= app_bundle
DEPENDPATH += .
INCLUDEPATH += .
QT += quick qml widgets
include(./src/src.pri)
SOURCES += main.cpp
