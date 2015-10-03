#include <QApplication>
#include <QtQuick/QQuickView>
#include <QtQml/QtQml>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QQuickView view;

    view.engine()->addImportPath(LIB_DIR);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///qml/GlowControl.qml"));
    view.show();

    return a.exec();
}
