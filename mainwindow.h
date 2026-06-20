#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPushButton>
#include <QTextEdit>

#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

class MainWindow : public QMainWindow

{

public:
    MainWindow(QWidget *parent = nullptr);
private:
    QNetworkAccessManager *manager;
    QTextEdit *textEdit;
    QPushButton *pushButton;

    QVTKOpenGLNativeWidget *vtkWidget;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;

    void onButtonClicked();
    void onReplyFinished(QNetworkReply *reply);

    void showMesh();
};

#endif
