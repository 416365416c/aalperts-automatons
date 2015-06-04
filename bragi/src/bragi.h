#include <qqml.h>
#include "editor.h"
#include "interpreter.h"
#include "qmlhighlighter.h"
#include <QMenuBar>
#include <QMenu>
namespace Bragi {
    void registerTypes(const char* uri, int maj, int min) {
        qmlRegisterType<Interpreter>(uri, maj, min, "Interpreter");
        qmlRegisterType<Editor>(uri, maj, min, "Editor");
        qmlRegisterUncreatableType<QSyntaxHighlighter>(uri, maj, min, "AbstractSyntaxHighlighter", "Abstract");
        qmlRegisterType<QMLHighlighter>(uri, maj, min, "SyntaxHighlighter");
    }

    void populateMenus(QMenuBar* bar) {
        QMenu* fileMenu = bar->addMenu("File");
        QMenu* editMenu = bar->addMenu("Edit");
        QMenu* findMenu = bar->addMenu("Find");
        QMenu* viewMenu = bar->addMenu("View");
        QMenu* toolsMenu = bar->addMenu("Tools");
        QMenu* snippetsMenu = bar->addMenu("Snippets");
        QMenu* windowMenu = bar->addMenu("Window");
        QMenu* helpMenu = bar->addMenu("Help");
    }
}
