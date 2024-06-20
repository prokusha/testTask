#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void generate();
    void changeFontSize();
    void fillTableMatrix();
    void fillLineSpiral();

private:
    Ui::MainWindow *ui;
    QFont m_fnt;

    int m_maxCells = 0;
    float m_fontScale = 0.3;
};
#endif // MAINWINDOW_H
