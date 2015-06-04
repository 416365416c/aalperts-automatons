#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/qqml.h>

#include "qmlscheme_p.h"

QT_BEGIN_NAMESPACE

class QmlSchemePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    virtual void registerTypes(const char *uri)
    {
        qmlRegisterType<QmlScheme>(uri, 0, 1, "SchemeScript");
        qmlProtectModule("SchemeScript", 0);
    }
};

QT_END_NAMESPACE

#include "plugin.moc"
