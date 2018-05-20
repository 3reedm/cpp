#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int y, i, shift, *ptrWidth, *ptrResults;
    y = this->height();
    shift = (y - 6 * 30 - 5 * 5) / 2;
    ptrWidth = new int[6];
    ptrResults = new int[6];

    int allTime = 0;
    getResults('o', ptrResults);
    for (i = 0; i < 6; ++i)
        allTime = allTime + ptrResults[i];
    for (i = 0; i < 6; ++i)
        ptrWidth[i] = static_cast<int>(static_cast<float>(ptrResults[i]) / static_cast<float>(allTime) * 1000.F + 10.F);

    char* ptrBuff = new char[9];
    painter.setPen(QPen(Qt::darkGreen, 1, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(Qt::darkGreen, Qt::SolidPattern));
    painter.drawRect(0, shift, ptrWidth[0], 20);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawText(ptrWidth[0] + 3, shift + 14, "My Developed Open");
    std::fill(ptrBuff, ptrBuff + 8, '\0');
    snprintf(ptrBuff, 9, "%5d", ptrResults[0]);
    *std::find(ptrBuff, ptrBuff + 9, '\0') = 's';
    painter.setPen(QPen(Qt::yellow, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawText(-12, shift + 14, ptrBuff);

    painter.setPen(QPen(Qt::darkGreen, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(0, shift + 40, ptrWidth[1], 20);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawText(ptrWidth[1] + 3, shift + 54, "My Developed Line");
    std::fill(ptrBuff, ptrBuff + 8, '\0');
    snprintf(ptrBuff, 9, "%5d", ptrResults[1]);
    *std::find(ptrBuff, ptrBuff + 9, '\0') = 's';
    painter.setPen(QPen(Qt::yellow, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawText(-12, shift + 54, ptrBuff);

    painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
    painter.drawRect(0, shift + 80, ptrWidth[2], 20);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawText(ptrWidth[2] + 3, shift + 94, "LevelDB");
    std::fill(ptrBuff, ptrBuff + 8, '\0');
    snprintf(ptrBuff, 9, "%5d", ptrResults[2]);
    *std::find(ptrBuff, ptrBuff + 9, '\0') = 's';
    painter.setPen(QPen(Qt::yellow, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawText(-12, shift + 94, ptrBuff);

    painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(0, shift + 120, ptrWidth[3], 20);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawText(ptrWidth[3] + 3, shift + 134, "Kyoto Cabinet");
    std::fill(ptrBuff, ptrBuff + 8, '\0');
    snprintf(ptrBuff, 9, "%5d", ptrResults[3]);
    *std::find(ptrBuff, ptrBuff + 9, '\0') = 's';
    painter.setPen(QPen(Qt::yellow, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawText(-12, shift + 134, ptrBuff);

    painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(0, shift + 160, ptrWidth[4], 20);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawText(ptrWidth[4] + 3, shift + 174, "Berkeley DB");
    std::fill(ptrBuff, ptrBuff + 8, '\0');
    snprintf(ptrBuff, 9, "%5d", ptrResults[4]);
    *std::find(ptrBuff, ptrBuff + 9, '\0') = 's';
    painter.setPen(QPen(Qt::yellow, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawText(-12, shift + 174, ptrBuff);

    painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(0, shift + 200, ptrWidth[5], 20);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawText(ptrWidth[5] + 3, shift + 214, "Hamsterdb");
    std::fill(ptrBuff, ptrBuff + 8, '\0');
    snprintf(ptrBuff, 9, "%5d", ptrResults[5]);
    *std::find(ptrBuff, ptrBuff + 9, '\0') = 's';
    painter.setPen(QPen(Qt::yellow, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawText(-12, shift + 214, ptrBuff);

    delete[] ptrResults;
    delete[] ptrWidth;
    delete[] ptrBuff;
}

void MainWindow::getResults(char mode, int* ptrResults)
{
    std::fstream fileResults;
    std::string fileResultsName;

    if (mode == 'w')
        fileResultsName = "resultsWrite";
    else if (mode == 'r')
        fileResultsName = "resultsRead";
    else if (mode == 'o')
        fileResultsName = "resultsOverwrite";
    else
        throw "Нет такого режима";

    fileResults.open(fileResultsName.c_str(), std::ios_base::in);

    char* ptrBuff = new char[5];
    int i = 0, j;
    int* ptrTimes = new int[72];
    std::fill(ptrTimes, ptrTimes + 72, 0);
    while (fileResults.getline(ptrBuff, 5)) {
        ptrTimes[i] = static_cast<int>(strtol(ptrBuff, reinterpret_cast<char**>('\0'), 10));
        ++i;
    }

    for (i = 0; i < 6; ++i) {
        *std::max_element(ptrTimes + i * 12, ptrTimes + (i + 1) * 12) = 0;
        *std::min_element(ptrTimes + i * 12, ptrTimes + (i + 1) * 12) = 0;
    }

    for (i = 0; i < 6; ++i) {
        ptrResults[i] = 0;
        for (j = 0; j < 12; ++j)
            ptrResults[i] += ptrTimes[i * 12 + j];
        ptrResults[i] = static_cast<int>(static_cast<float>(ptrResults[i]) * 0.1F);
    }

    delete[] ptrBuff;
    delete[] ptrTimes;
    fileResults.close();
}
