#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->buttonGenerate, SIGNAL(clicked()), this, SLOT(generate()));

    ui->tableMatrix->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableMatrix->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::generate()
{
    ui->tableMatrix->setRowCount(ui->spinBox_Y->value());
    ui->tableMatrix->setColumnCount(ui->spinBox_X->value());

    m_maxCells = std::max(ui->spinBox_X->value(), ui->spinBox_Y->value());

    changeFontSize();
    fillTableMatrix();
    fillLineSpiral();
}

void MainWindow::changeFontSize()
{
    m_fnt.setPixelSize(ui->tableMatrix->size().width() * m_fontScale / m_maxCells);
    ui->tableMatrix->setFont(m_fnt);
}

void MainWindow::fillTableMatrix()
{
    for (int i = 0, value = 1; i < ui->spinBox_Y->value(); i++)
    {
        for (int j = 0; j < ui->spinBox_X->value(); j++, value++)
        {
            QTableWidgetItem* cell = new QTableWidgetItem(QString::number(value));
            cell->setTextAlignment(Qt::AlignCenter);
            ui->tableMatrix->setItem(i, j, cell);
        }
    }
}

void MainWindow::fillLineSpiral()
{
    int yy = ui->spinBox_Y->value(), xx = ui->spinBox_X->value();
    QString spiral;

    if (yy == 0)
        return;

    std::vector<std::vector<bool>> seen(yy, std::vector<bool>(xx, false));
    int dr[] = { 0, 1, 0, -1 };
    int dc[] = { 1, 0, -1, 0 };

    int x = 0, y = 0, di = 0;

    for (int i = 0; i < xx * yy; i++)
    {
        spiral += ui->tableMatrix->item(x, y)->text() + ' ';
        seen[x][y] = true;
        int newX = x + dr[di];
        int newY = y + dc[di];

        if (0 <= newX && newX < yy && 0 <= newY && newY < xx
            && !seen[newX][newY])
        {
            x = newX;
            y = newY;
        } else
        {
            di = (di + 1) % 4;
            x += dr[di];
            y += dc[di];
        }
    }

    ui->lineSpiralMatrix->setText(spiral);
}

MainWindow::~MainWindow()
{
    delete ui;
}
