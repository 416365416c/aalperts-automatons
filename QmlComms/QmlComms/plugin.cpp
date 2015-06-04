#include <qdeclarativeextensionplugin.h>
#include <qdeclarative.h>

#include "qmlcomms.h"

QT_BEGIN_NAMESPACE

class QmlCommsPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
public:
    virtual void registerTypes(const char *uri)
    {
        qmlRegisterType<QmlComms>(uri,1,0,"Networking");
    }
};

QT_END_NAMESPACE

#include "plugin.moc"

Q_EXPORT_PLUGIN2(qmlcommsplugin, QT_PREPEND_NAMESPACE(QmlCommsPlugin));

