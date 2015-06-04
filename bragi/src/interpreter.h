#include <QQuickItem>
#include <QQmlComponent>

class Interpreter : public QQuickItem
{
    Q_OBJECT
    /*
     * Like a custom Loader
     */
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)
    Q_PROPERTY(QString effectiveSource READ effectiveSource NOTIFY effectiveSourceChanged)

    Q_PROPERTY(QQuickItem* instance READ instance NOTIFY instanceChanged)
public:
    Interpreter(QQuickItem* parent=0);
    ~Interpreter();
    QString lastError() const
    {
        return m_lastError;
    }

    QString source() const
    {
        return m_source;
    }

    bool running() const
    {
        return m_running;
    }

    bool valid() const
    {
        return m_valid;
    }

    QString effectiveSource() const
    {
        return m_effectiveSource;
    }

    QQuickItem* instance() const
    {
        return m_instance;
    }

public slots:
    void setSource(QString arg)
    {
        if (m_source != arg) {
            m_source = arg;
            tryLoading();
            emit sourceChanged(arg);
        }
    }

    void setRunning(bool arg)
    {
        if (m_running != arg) {
            m_running = arg;
            if (m_running)
                tryLoading();
            else
                setInstance(0);
            emit runningChanged(arg);
        }
    }

signals:
    void lastErrorChanged(QString arg);

    void sourceChanged(QString arg);

    void runningChanged(bool arg);

    void validChanged(bool arg);

    void effectiveSourceChanged(QString arg);

    void instanceChanged(QQuickItem* arg);
protected:
    virtual void componentComplete();
private:
    void tryLoading();
    void setInstance(QQuickItem* i) {
        delete m_instance;
        m_instance = i;
        emit instanceChanged(i);
    }

    QString m_lastError;
    QString m_source;
    bool m_running;
    bool m_valid;
    QString m_effectiveSource;
    QQmlComponent* m_component;
    QQuickItem* m_instance;
};
