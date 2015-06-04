#!/usr/bin/python
#usage gen-qml.py [files]
#files will have a QML shell generated as qml_file
#files assumed to be C++ header files, so qml_file should also end in .h
#a main.cpp will also be spat out, as main.cpp.auto to avoid accidental clobbering. Same for .pro
#input file mostly ignored except for lines of the form //QML [type] [name]
#TODO: Add another command which takes a data member instead, generates set/get/sig, for some situations
#TODO: Bit smarter than all read/write non-const properties
#TODO: Parameterize generation to suit more styles (but till then, use the handy functions)
#Generation logic:
# getter signature - type get_name() const
# setter signature - void set_name(const type &)
# notify signature - virtual void name_changed()
# notify is the virtual function you promise to call every time it changes
# Note that NOTIFY signal is called nameChanged, name_changed is the one in the non-Qt version
#Supported special types:
# char* will be magically turned into QString (does anyone use string?)
# TODO: turn a stdlib container into a qlist
#Unexpected features:
# Handles multiple classes in the one .h
#"Features":
# Some requirements as to the indentation of the source file, like one {/} per line max
# Probably will choke if you use tabs instead of spaces in weird places

import sys
import re

classNameRE = re.compile("class +([^ \n]+)")
QMLRE = re.compile("//QML +([^ ]+) +([^ \n]+)")
indent = "    "
qmlClassNames = []
qmlFileNames = []
def getterName(name):
    return "get_" + name
def setterName(name):
    return "set_" + name
def notifyName(name):
    return name + "_changed"
def generateClass(className, data): #data is type/name tuples
    print "Generating QML wrapper for", className
    ret = "class "
    qmlClassName = "Qml" + className
    qmlClassNames.append(qmlClassName)
    ret += qmlClassName + " : public QObject, public " + className + "\n{\n"+indent+"Q_OBJECT\n"
    for var in data:
        if var[0] == 'char*':
            ret += indent + "Q_PROPERTY(QString " + var[1] + " READ QSTRING_" + getterName(var[1]) + " WRITE QSTRING_" + setterName(var[1]) + " NOTIFY " + var[1] + "Changed )\n"
        else:
            ret += indent + "Q_PROPERTY(" + var[0] + " " + var[1] + " READ " + getterName(var[1]) + " WRITE " + setterName(var[1]) + " NOTIFY " + var[1] + "Changed )\n"
    ret += "public:\n"
    #Constructor time - base constructor always called with no args
    ret += indent + qmlClassName + "(QObject* parent = 0) : QObject(parent), " + className + "() {}\n"
    #Where I could make the auto-getter
    for var in data:
        if var[0] == 'char*':
            ret += indent + "QString QSTRING_" + getterName(var[1]) + "() const { return QString::fromLocal8Bit(" + getterName(var[1]) + "()); }\n"
    #Where I could make the auto-setter
    ret += "public slots:\n"
    for var in data:
        if var[0] == 'char*':
            ret += indent + "void QSTRING_" + setterName(var[1]) + "(const QString &arg) { " + setterName(var[1]) + "(arg.toLocal8Bit().constData()); }\n"
    ret += "signals:\n"
    for var in data:
        ret += indent + "void " + var[1] + "Changed();\n"
    #Disable for datum mode
    ret += "protected:\n"
    for var in data:
        ret += indent + "virtual void " + notifyName(var[1]) + "() { emit " + var[1] + "Changed(); }\n"
    ret += "};\n\n"
    return ret
for inputFileName in sys.argv[1:]:
    print "Processing", inputFileName
    inputFile = open(inputFileName, "r")
    qmlName = "qml_"+inputFileName.split('/')[-1];
    out = open(qmlName, "w")
    headerGuard = qmlName.upper().replace(".","_");
    out.write("#ifndef " + headerGuard + "\n#define " + headerGuard + "\n")
    out.write("#include <QObject>\n#include <QString>\n\n")
    out.write("#include \"" + inputFileName + "\"\n\n")
    qmlFileNames.append(qmlName)
    className = ""
    classData = []
    nestLevel = 0
    for line in inputFile.readlines():
        if nestLevel == 0:
            match = re.search(classNameRE, line)
            if (match != None):
                if '{' in line:
                    nestLevel += 1
                className = line[match.regs[1][0] : match.regs[1][1]]
                classData = []
        else:
            if '{' in line:
                if not '}' in line: #Fix bug for common case of inline function
                    nestLevel += 1
            elif '}' in line:
                nestLevel -= 1
                if nestLevel == 0: #Class finished
                    out.write(generateClass(className, classData))
            match = re.search(QMLRE, line)
            if (match != None):
                classData.append((line[match.regs[1][0] : match.regs[1][1]],line[match.regs[2][0] : match.regs[2][1]]))
    out.write("#endif //header guard\n")
main = open("main.cpp.auto", "w")
main.write("""
#include <QApplication>
#include <QDeclarativeView>
#include <qdeclarative.h>
""")
for fileName in qmlFileNames:
    main.write("#include \"" + fileName + "\"\n")
main.write("""
int main (int argc, char* argv[])
{
    QApplication app(argc, argv);
    QDeclarativeView view;
""")
for className in qmlClassNames:
    main.write(indent + "qmlRegisterType<" + className + ">(\"MyApp\", 1, 0, \"" + className + "\");")
main.write("""
    view.setSource(QUrl::fromLocalFile("main.qml"));
    view.show();
    return app.exec();
}
""")
pro = open("qmlVersion.pro.auto", "w")
pro.write("""
######################################################################
# Based off one generated by qmake (2.01a) Thu Jul 19 22:42:57 2012
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

QT += declarative

# Input
SOURCES += main.cpp
HEADERS +=""")
for fileName in qmlFileNames + sys.argv[1:]:
    pro.write(" " + fileName)
pro.write("\n")
