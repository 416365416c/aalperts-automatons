/* Copyright 2013 Alan Alpert
   License: GNU GPL 3.0       */

#include "qmlscheme_p.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtQml/qqmlinfo.h>
#include <QtQml/qqmlengine.h>

#define SANITY(RET) if (!m_value || !m_compDone) { \
    qmlInfo(this) << "Scheme object error"; \
    return RET;\
}

#define HANDLE(CF) if(!CF) { \
    char *err = (char*)malloc(maxStrSize * sizeof(char)); \
    CHICKEN_get_error_message(err, maxStrSize); \
    QString errStr = QString::fromLocal8Bit(err); \
    qmlInfo(this) << errStr; \
    delete err; \
}

void QmlScheme::static_init()
{
    static bool done = false;
    if(done)
        return;
    done = true;

    //Gotta love copying all these strings around...
    QStringList arguments = QCoreApplication::arguments();
    static QList<QByteArray> args;
    foreach(const QString &s, arguments)
        args << s.toLocal8Bit();
    int argc = args.count();
    char **argv = (char**)malloc(argc*sizeof(char*));//Does this have to leak?
    int argz = 0;
    foreach(const QByteArray &a, args)
        argv[argz++] = const_cast<char*>(a.data()); //Avoids another copy?

    long int h,s,y;
    h=s=y=0;
    CHICKEN_parse_command_line(argc, argv, &h,&s,&y);

    int worked = CHICKEN_initialize(h,s,y, CHICKEN_default_toplevel);
    if (!worked)
        qCritical("Error initializing scheme engine. SchemeScript elements will not work.");
    worked = worked && CHICKEN_run(NULL); //Needed at all? But segfaults otherwise?
    qDebug() << "Initialization: " << worked;
}

QmlScheme::QmlScheme(QObject *parent, C_word cword) : QObject(parent)
{
    m_value = cword;
    m_compDone = true;
}

QmlScheme::~QmlScheme() {
}

QString QmlScheme::toString() {
    SANITY(QString())

    char *ret = (char*)malloc(maxStrSize * sizeof(char));
    HANDLE(CHICKEN_eval_to_string(m_value, ret, maxStrSize))
    QString sret = QString::fromLocal8Bit(ret);
    free(ret);

    return sret;
}

QmlScheme* QmlScheme::f(QmlScheme *a)
{
    SANITY(0)

    C_word ret;
    HANDLE(CHICKEN_apply(m_value, a->m_value, &ret))
    QmlScheme* sret = new QmlScheme(0, ret); //No QObject parent or it won't JS GC?
    QQmlEngine::setObjectOwnership(sret, QQmlEngine::JavaScriptOwnership);

    return sret;
}

void QmlScheme::classBegin()
{
    m_compDone = false;
}

void QmlScheme::componentComplete()
{
    static_init();
    m_compDone = true;
    if(!m_script.isEmpty()) {
        char *strcopy = (char*)malloc((m_script.length() + 1) * sizeof(char));
        strcpy(strcopy, m_script.toLocal8Bit().data());
        HANDLE(CHICKEN_eval_string(strcopy, &m_value)) //eval_string or read?
        free(strcopy);
    }
}

