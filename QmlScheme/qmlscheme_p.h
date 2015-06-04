/* Copyright 2013 Alan Alpert
   License: GNU GPL 3.0       */

#ifndef QMLSCHEME_P_H
#define QMLSCHEME_P_H

#include <QObject>
#include <QString>
#include <QQmlParserStatus>

//Chicken scheme, a BSD licensed scheme implementation
//See http://wiki.call-cc.org/ for more information
#include <chicken/chicken.h>

const int maxStrSize = 65535;
//TODO: Globals, AOT, source file support, eval convenience, react to script change
class QmlScheme : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    //QString() if not created from a string
    Q_PROPERTY(QString script READ script WRITE setScript NOTIFY scriptChanged)
    //Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)

    Q_CLASSINFO("DefaultProperty", "script")

public:
    QmlScheme(QObject *parent=0) : QObject(parent), m_value(0), m_compDone(true) {}
    QmlScheme(QObject *parent, C_word cword);
    ~QmlScheme();

    QString script() const { return m_script; }
public slots:
    void setScript(const QString &s) { if (m_script == s) return; m_script = s; emit scriptChanged(); }
    QString toString();// == script() if created from a string
    QmlScheme* f(QmlScheme *a = 0);

protected:
    virtual void classBegin();
    virtual void componentComplete();

signals:
    void scriptChanged();

private:
    static void static_init();
    C_word m_value;
    bool m_compDone;
    QString m_script;
};

#endif
