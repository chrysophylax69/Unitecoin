#include <QTimer>
#include "chartpage.h"
#include "ui_chartpage.h"

#include "main.h"

#include "clientmodel.h"

ChartPage::ChartPage(QWidget *parent) :
    QWidget(parent),
    clientModel(0),
    lastBlockHeight(0),
    rangeChoice(Day),
    ui(new Ui::ChartPage)
{


    ui->setupUi(this);


    ui->hSlider->setVisible(false);
    ui->vSlider->setVisible(false);
    ui->verticalScrollBar->setVisible(false);
    ui->horizontalScrollBar->setVisible(false);



    setupChart();

    // configure scroll bars:
    // Since scroll bars only support integer values, we'll set a high default range of and
    // divide or multiply scroll bar position values to achieve corrected axeis coordinates.
    ui->horizontalScrollBar->setRange(-1, 500000);
    ui->verticalScrollBar->setRange(500, -1);

    // create connection between axes and scroll bars:
    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(horzScrollBarChanged(int)));
    connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(vertScrollBarChanged(int)));
    connect(ui->chartBlock->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    connect(ui->chartBlock->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));

    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(addChartPoints()));
    timer->start(100);


}

ChartPage::~ChartPage()
{
    delete ui;
}

void ChartPage::setTxtText(QString txt)
{
    ui->txtBlocks->appendPlainText(txt);

}

void ChartPage::setupChart()
{
  this->buildingChart=true;

  QCustomPlot *customPlot = ui->chartBlock;
  customPlot->setInteraction(QCP::iRangeDrag, true);
  customPlot->setInteraction(QCP::iSelectPlottables, true);
  customPlot->setInteraction(QCP::iSelectItems, true);
  customPlot->setInteraction(QCP::iRangeZoom, true);
  customPlot->legend->setVisible(true);
  customPlot->legend->setFont(QFont("Helvetica",9));
  // set locale to english, so we get english decimal separator:
  customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));



  // create graph and assign data to it:
  customPlot->addGraph();
  customPlot->graph(0)->setName("Difficulty");
//  customPlot->graph(0)->setData(x, y);
  // give the axes some labels:
  customPlot->xAxis->setLabel("Date");
  customPlot->yAxis->setLabel("Difficulty");
  customPlot->legend->setSelectableParts(QCPLegend::spItems);
  customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  customPlot->xAxis->setDateTimeFormat("dd/MM/yyyy");
  customPlot->xAxis->setAutoTickCount(5);
  //customPlot->yAxis->setAutoTickCount(5);

  // set axes ranges, so we see all data:
  customPlot->yAxis->setRange(-0.1, 3);

  ClientModel *model = this->clientModel;
  // generate some data:
  QVector<double> x(1000), y(1000); //, y1(50000);
  int maxBlocks;
  maxBlocks = model->getNumBlocks();

  if (maxBlocks > 1000)
  {
        maxBlocks=1000;
  }

  int i = 0;
  if (maxBlocks > 0) {
      for (i = 0; i < maxBlocks; i++)
      {
        CBlock blk = model->getBlock(i);
        CBlockIndex* cIndex = model->getBlockIndex(i);
        x[i]=blk.GetBlockTime();
        y[i]=model->getDiff(cIndex);
      }
      customPlot->graph(0)->setData(x, y);
      customPlot->xAxis->setRange(x[i-1]-(60*60*24), x[i-1]);
      this->lastBlockHeight = i;
  }



  customPlot->replot();
  this->buildingChart=false;

}

void ChartPage::addChartPoints()
{
    if (this->buildingChart)
        return;

    this->buildingChart=true;

    ClientModel *model = this->clientModel;
    QCustomPlot *customPlot = ui->chartBlock;

    // generate some data:
    QVector<double> x(250), y(250);
    int maxBlocks = 0;
    maxBlocks = model->getNumBlocks() - this->lastBlockHeight;
    if (maxBlocks == 0)
        return;
    if (maxBlocks > 250)
    {
          maxBlocks = 250;
    } else {
        this->timer->stop();
    }

    int i;
    for (i = 0; i < maxBlocks; i++)
    {
      CBlock blk = model->getBlock(i + this->lastBlockHeight);
      CBlockIndex* cIndex = model->getBlockIndex(i + this->lastBlockHeight);
      x[i]=blk.GetBlockTime();
      y[i]=model->getDiff(cIndex);

    }

    if (this->lastBlockHeight > 0) {
        customPlot->graph(0)->addData(x, y);
    } else {
        customPlot->graph(0)->setData(x, y);
    }

    customPlot->xAxis->setRange(x[i-1] - rangeChoice, x[i-1]);
    customPlot->replot();
    this->buildingChart=false;
    this->lastBlockHeight += i;
}

void ChartPage::setClientModel(ClientModel *model)
{
    this->clientModel = model;
    if(model)
    {
        connect(model, SIGNAL(numBlocksChanged(int,int)), this, SLOT(notifyNewBlocks(int,int)));
        connect(model, SIGNAL(numBlocksChanged(int,int)), this, SLOT(addChartPoints()));
    }
}

void ChartPage::notifyNewBlocks(int count, int countOfPeers)
{
    ui->txtBlocks->appendPlainText(tr("%1").arg(count));
//    addChartPoints();
}





void ChartPage::on_rbDay_clicked()
{
    ui->rbMonth->setChecked(false);
    ui->rbWeek->setChecked(false);

    rangeChoice = Day;
    QCustomPlot *customPlot = ui->chartBlock;
    double mupper = customPlot->xAxis->range().upper;
    customPlot->xAxis->setRange(mupper - rangeChoice, mupper);
    customPlot->replot();
}



void ChartPage::on_rbWeek_clicked()
{
    ui->rbMonth->setChecked(false);
    ui->rbDay->setChecked(false);

    rangeChoice = Week;
    QCustomPlot *customPlot = ui->chartBlock;
    double mupper = customPlot->xAxis->range().upper;
    customPlot->xAxis->setRange(mupper - rangeChoice, mupper);
    customPlot->replot();

}

void ChartPage::on_rbMonth_clicked()
{
    ui->rbDay->setChecked(false);
    ui->rbWeek->setChecked(false);

    rangeChoice = Month;
    QCustomPlot *customPlot = ui->chartBlock;
    double mupper = customPlot->xAxis->range().upper;
    customPlot->xAxis->setRange(mupper - rangeChoice, mupper);
    customPlot->replot();

}

void ChartPage::on_btnRefresh_clicked()
{
    addChartPoints();
}


void ChartPage::horzScrollBarChanged(int value)
{
  if (qAbs(ui->chartBlock->xAxis->range().center()-value*1000000.0) > 0.01) // if user is dragging plot, we don't want to replot twice
  {
    ui->chartBlock->xAxis->setRange(value*1000000.0, ui->chartBlock->xAxis->range().size(), Qt::AlignCenter);

    ui->chartBlock->replot();
  }
}

void ChartPage::vertScrollBarChanged(int value)
{
  if (qAbs(ui->chartBlock->yAxis->range().center()+value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
  {
    ui->chartBlock->yAxis->setRange(-value/100.0, ui->chartBlock->yAxis->range().size(), Qt::AlignCenter);
    ui->chartBlock->replot();
  }
}

void ChartPage::xAxisChanged(QCPRange range)
{
  ui->horizontalScrollBar->setValue(qRound(range.center()/1000000.0)); // adjust position of scroll bar slider
  ui->horizontalScrollBar->setPageStep(qRound(range.size()*1000000.0)); // adjust size of scroll bar slider
}

void ChartPage::yAxisChanged(QCPRange range)
{
  ui->verticalScrollBar->setValue(qRound(-range.center()*100.0)); // adjust position of scroll bar slider
  ui->verticalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

