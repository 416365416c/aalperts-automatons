#include <qqmlextensionplugin.h>
#include <qqml.h>

#include "../QmlComms/qmlcomms.h"

QT_BEGIN_NAMESPACE

class QmlCommsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.gitorious.QmlHelpers.QmlComms")

public:
    virtual void registerTypes(const char *uri)
    {
        qmlRegisterType<QmlComms>(uri,1,0,"Networking");
    }
};

QT_END_NAMESPACE

#include "plugin.moc"
