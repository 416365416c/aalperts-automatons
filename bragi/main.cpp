#include <cstdio>
#include <QGuiApplication>
#include <QQuickView>
#include <QFontDatabase>
#include "src/bragi.h"
//#define USING_MENUS
#ifdef USING_MENUS
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <qwindowcontainer.h>
#endif

int main (int argc, char* argv[]) {
#ifdef USING_MENUS
    QApplication app(argc, argv);
#else
    QGuiApplication app(argc, argv);
#endif
    int ok = QFontDatabase::addApplicationFont("content/fonts/VeraMoBd.ttf");
    if(ok == -1)
        printf("Not happy with the fonts :(");
    Bragi::registerTypes("Bragi", 0, 1);
#ifdef USING_MENUS
    QQuickView view;
    QMainWindow main;
    Bragi::populateMenus(main.menuBar());
    QWindowContainer container;
    container.setWindow(&view);
    main.setCentralWidget(&container);
    main.show();
    view.resize(640, 480);
    main.resize(640, 480);
    container.resize(640, 480);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl::fromLocalFile("main.qml"));
    view.show();
#else
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl::fromLocalFile("main.qml"));
    view.show();
#endif
    return app.exec();
}
