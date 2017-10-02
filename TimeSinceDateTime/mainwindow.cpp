#include "mainwindow.h"
#include <iostream>
#include <QtConcurrent/QtConcurrent>

using namespace  std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // ensure that the time set is null
    this->timeSet = 0;
    this->running = true;
    // create the GUI window
    this->createWindow();

}

MainWindow::~MainWindow()
{
    // delete all pointers
    delete countdownButton;
    delete exitButton;
    delete dateTimeEdit;
    delete currentTime;
    delete remainingTime;
    delete timeSet;
}

void MainWindow::createWindow()
{

    /* create and format GUI components */

    // create buttons and add their slots
    countdownButton = new QPushButton("&Countdown");
    exitButton = new QPushButton("E&xit");

    // connect the button signals with the slots
    connect(countdownButton, SIGNAL(clicked()), this, SLOT(countdownClicked()));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(exitClicked()));

    // Create and format the DateTime selector
    dateTimeEdit = new QDateTimeEdit;
    dateTimeEdit->setDisplayFormat("MM/dd/yyyy hh:mm:ss");
    dateTimeEdit->setDateTime(QDateTime::currentDateTime().addSecs(3600));

    // Layout for the date/time selector
    QHBoxLayout *dateTimeLayout = new QHBoxLayout;
    dateTimeLayout->addWidget(new QLabel("Date and time:"));
    dateTimeLayout->addWidget(dateTimeEdit);

    // Label for the current time
    currentTime = new QLabel;

    // Layout for the current time
    QHBoxLayout *currentTimeLayout = new QHBoxLayout;
    currentTimeLayout->addWidget(new QLabel("Current time:"));
    currentTimeLayout->addWidget(currentTime);

    // Label for the remaining time
    remainingTime = new QLabel;

    // Layout for the remaining time
    QHBoxLayout *remainingTimeLayout = new QHBoxLayout;
    remainingTimeLayout->addWidget(new QLabel("Remaining time:"));
    remainingTimeLayout->addWidget(remainingTime);

    // Layout for the buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(countdownButton);
    buttonLayout->addWidget(exitButton);

    // Add everything to the main layout
    this->centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(dateTimeLayout);
    mainLayout->addLayout(currentTimeLayout);
    mainLayout->addLayout(remainingTimeLayout);
    mainLayout->addLayout(buttonLayout);

    // setup window and central widget
    this->centralWidget->setLayout(mainLayout);

    this->setCentralWidget(this->centralWidget);

    this->setWindowTitle("Shutdown");
    this->setFixedSize(QSize(300, 125));

    // Start new thread for updating the current and remaing time
    QtConcurrent::run(this, MainWindow::updateTime);
}


void MainWindow::updateTime()
{
    /* Update the current and remaining time */

    QDateTime dateTime;

    // Update times as long as the thread is active
    while (this->running)
    {
        dateTime = QDateTime::currentDateTime();
        QDate date = dateTime.date();
        QTime time = dateTime.time();

        // Set and format the current time.
        this->currentTime->setText(QString("%1").arg(date.month(), 2, 10, QChar('0')) + "/" + QString("%1").arg(date.day(), 2, 10, QChar('0')) + "/" + QString::number(date.year()) + " " +
                                   QString("%1").arg(time.hour(), 2, 10, QChar('0')) + ":" + QString("%1").arg(time.minute(), 2, 10, QChar('0')) + ":" + QString("%1").arg(time.second(), 2, 10, QChar('0')));

        // If the time has not been set for shutdown yet, do not calculate remaining time.
        if (this->timeSet != 0)
            this->remainingTime->setText(QString("%1").arg(calculateHours(), 2, 10, QChar('0')) + ":" + QString("%1").arg(calculateMinutes(), 2, 10, QChar('0')) + ":" + QString("%1").arg(calculateSeconds(), 2, 10, QChar('0')));
        else
            this->remainingTime->setText("");

        // Sleep for 100 MS to prevent constant use of resources
        QThread::msleep(100);
    }
    return;
}

// Slot for countdown button clicked.
void MainWindow::countdownClicked()
{
    // Take time from dateTimeEdit and start countdown.
    timeSet = new QDateTime;
    timeSet->setDate(dateTimeEdit->date());
    timeSet->setTime(dateTimeEdit->time());
    return;
}

// Slot for exit button clicked
void MainWindow::exitClicked()
{
    this->running = false;
    this->close();
    return;
}

// Overrite the close event to ensure the child threads are closed.
void MainWindow::closeEvent(QCloseEvent *)
{
    this->running = false;
    this->close();
    return;
}
int MainWindow::calculateHours() const
{
    // Calculate and return hours
    return QDateTime::currentDateTime().secsTo(*this->timeSet) / (60 * 60);
}

int MainWindow::calculateMinutes() const
{
    qint64 t_seconds = QDateTime::currentDateTime().secsTo(*this->timeSet);
    return (t_seconds / 60) - (t_seconds / (60 * 60)) * 60;
}

int MainWindow::calculateSeconds() const
{
    return QDateTime::currentDateTime().secsTo(*this->timeSet) % 60;
}


