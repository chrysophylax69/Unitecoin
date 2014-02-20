#ifndef CHARTPAGE_H
#define CHARTPAGE_H

#include <QWidget>
#include <QTimer>
#include "qcustomplot.h"

namespace Ui {
class ChartPage;
}
class ClientModel;
class WalletModel;


class ChartPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit ChartPage(QWidget *parent = 0);
    ~ChartPage();

    void setClientModel(ClientModel *clientModel);
    void setWalletModel(WalletModel *walletModel);
    void setTxtText(QString txt);

    
public slots:
    void notifyNewBlocks(int count, int countOfPeers);
    void addChartPoints();

private slots:
    void on_rbDay_clicked();
    void on_rbWeek_clicked();
    void on_rbMonth_clicked();

    void on_btnRefresh_clicked();

    void horzScrollBarChanged(int value);
    void vertScrollBarChanged(int value);
    void xAxisChanged(QCPRange range);
    void yAxisChanged(QCPRange range);


private:
    Ui::ChartPage *ui;
    ClientModel *clientModel;
    WalletModel *walletModel;
    int lastBlockHeight;
    int rangeChoice;
    bool buildingChart;
    void setupChart();
    QTimer *timer;

    enum groupRange {
        Day = 60 * 60 * 24,
        Week = Day * 7,
        Month = Day * 31
    };

};



#endif // CHARTPAGE_H
