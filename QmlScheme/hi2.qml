import QtQml 2.0
import "QmlScheme" 0.1

QtObject {
    property var acc: SchemeScript { script: "'(1)" }
    property var foo: SchemeScript {
        script: '(lambda (x) (print (string-append "hello world " (number->string x))) (cons (+ x 1) \'()))'
    }
    property Timer timmy: Timer {
        interval: 100
        repeat: true
        running: true
        onTriggered: acc = foo.f(acc);
    }
}
