#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPushButton>
#include <QTextEdit>

class MainWindow : public QMainWindow

{

public:
    MainWindow(QWidget *parent = nullptr);
private:
    QNetworkAccessManager *manager;
    QTextEdit *textEdit;
    QPushButton *pushButton;
    void onButtonClicked();
    void onReplyFinished(QNetworkReply *reply);
};

#endif
