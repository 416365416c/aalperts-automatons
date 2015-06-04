#include <QDebug>
#include "interpreter.h"
Interpreter::Interpreter(QQuickItem* parent)
    : QQuickItem(parent)
    , m_lastError("")
    , m_source("")
    , m_running(true)
    , m_valid(false)
    , m_effectiveSource("")
    , m_component(0)
    , m_instance(0)
{
}

Interpreter::~Interpreter()
{
    delete m_instance;//TODO: Why am I tracking these?
    delete m_component;
}

void Interpreter::componentComplete()
{
    QQuickItem::componentComplete();
    tryLoading();
}

void Interpreter::tryLoading()
{
    if (!m_running || !isComponentComplete())
        return;
    //TODO: Permit async loading
    delete m_component;
    m_component = new QQmlComponent(qmlEngine(this), this);
    m_component->setData(m_source.toAscii(), QUrl::fromLocalFile("./Interpreted.qml"));
    //qDebug() << "Trying: " << m_source << "\nResult:" << m_component->status() << " + " << m_component->errorString();
    if (m_component->isReady()){
        QQuickItem* instance = qobject_cast<QQuickItem*>(m_component->create());
        if(instance) {
            setWidth(qMax(instance->width(), 100.0));
            setHeight(qMax(instance->height(), 100.0));
            instance->setParentItem(this);
            setInstance(instance);
            m_effectiveSource = m_source;
            m_valid = true;
            emit effectiveSourceChanged(m_source);
            emit validChanged(true);
        }
        return;
    }
    //Not made right
    // Leave old instance for now
    m_valid = false;
    emit validChanged(false);
    if (m_component->isError())
        m_lastError = m_component->errorString();
    else
        m_lastError = "My bad :(";//A case we just don't handle yet
    emit lastErrorChanged(m_lastError);
}
