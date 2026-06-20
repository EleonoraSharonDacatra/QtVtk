#include "mainwindow.h"

#include <QVBoxLayout>
#include <QWidget>

#include <QNetworkRequest>
#include <QUrl>

//Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    pushButton(new QPushButton(this)),
    textEdit(new QTextEdit(this)),
    manager(new QNetworkAccessManager(this))
{
    // Setup UI
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);
    pushButton->setText("Click me");
    textEdit->clear();
    layout->addWidget(pushButton);
    layout->addWidget(textEdit);
    setCentralWidget(central);

    // Connect button and signals
    connect(pushButton, &QPushButton::clicked,
            this, &MainWindow::onButtonClicked);
    connect(manager, &QNetworkAccessManager::finished,
            this, &MainWindow::onReplyFinished);
}

void MainWindow::onButtonClicked()
{
    QUrl url("https://jsonplaceholder.typicode.com/todos/1");
    manager->get(QNetworkRequest(url));
}

void MainWindow::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray data = reply->readAll();
        textEdit->setText(data);
    }
    else
    {
        QString error = reply->errorString();
        textEdit->setText(error);
    }

    reply->deleteLater();
}