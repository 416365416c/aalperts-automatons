import QtQml 2.0
import "QmlScheme" 0.1

QtObject {
    property var foo: SchemeScript {
        script: '(print "hello world")'
    }
}
