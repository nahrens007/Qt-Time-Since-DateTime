#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*
 * Shutdown Interface
 * Nathan Ahrens
 * Sept. 29 2017
 *
 * This program serves as a program to allow the user to graphically select when to shutdown the computer.
 */

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDateTime>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTimeEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QWidget *centralWidget;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QPushButton *countdownButton;
    QPushButton *exitButton;
    QDateTimeEdit *dateTimeEdit;
    QLabel *currentTime;
    QLabel *remainingTime;
    QDateTime *timeSet;
    void createWindow();
    void updateTime();
    int calculateHours() const;
    int calculateMinutes() const;
    int calculateSeconds() const;
    bool running;

private slots:
    void countdownClicked();
    void exitClicked();

protected:
    virtual void closeEvent(QCloseEvent *);

};

#endif // MAINWINDOW_H
