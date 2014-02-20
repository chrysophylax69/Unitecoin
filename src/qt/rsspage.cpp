#include "rsspage.h"
#include "ui_rsspage.h"
#include <QDeclarativeView>
#include <QVBoxLayout>

RSSPage::RSSPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RSSPage)
{
    ui->setupUi(this);
    QDeclarativeView *qmlView = new QDeclarativeView;
    qmlView->setSource(QUrl("qrc:/rssnews/qml/rssnews/rssnews.qml"));
    qmlView->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(qmlView);
}

RSSPage::~RSSPage()
{
    delete ui;
}
