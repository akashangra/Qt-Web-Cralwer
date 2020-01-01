#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static MainWindow *_mInstance;
public:
    MainWindow(const MainWindow &other) = delete;
    MainWindow& operator=(const MainWindow &other) = delete;

    static MainWindow *getInstance();
    void appendLinks(const QString &link);
    void onCrawlingCompleted();

private slots:
    void on_pushButtonCrawl_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
