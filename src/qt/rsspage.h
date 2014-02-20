#ifndef RSSPAGE_H
#define RSSPAGE_H

#include <QDialog>

namespace Ui {
class RSSPage;
}

class RSSPage : public QDialog
{
    Q_OBJECT
    
public:
    explicit RSSPage(QWidget *parent = 0);
    ~RSSPage();
    
private:
    Ui::RSSPage *ui;
};

#endif // RSSPAGE_H
