#include "mainwindow.h"

#include <QVBoxLayout>
#include <QWidget>
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>

#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkTriangle.h>

//Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    pushButton(new QPushButton(this)),
    textEdit(new QTextEdit(this)),
    manager(new QNetworkAccessManager(this)),
    vtkWidget(new QVTKOpenGLNativeWidget(this))
{
    // Setup UI
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);
    pushButton->setText("Click me");
    textEdit->clear();
    layout->addWidget(pushButton);
    layout->addWidget(vtkWidget);
    layout->addWidget(textEdit);
    setCentralWidget(central);

    renderWindow =vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderer =vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);
    vtkWidget->setRenderWindow(renderWindow);

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
        // Dopo aver ricevuto la reply mostriamo la mesh simulata
        showMesh();
    }
    else
    {
        QString error = reply->errorString();
        textEdit->setText(error);
    }

    reply->deleteLater();
}

void MainWindow::showMesh()
{
    renderer->RemoveAllViewProps();

    // Simulazione JSON contenente mesh
    QByteArray json = R"(
    {
        "vertices":
        [
            [0.0, 0.0, 0.0],
            [1.0, 0.0, 0.0],
            [0.5, 1.0, 0.0],
            [1.5, 1.0, 0.0]
        ],

        "faces":
        [
            [0,1,2],
            [1,3,2]
        ]
    }
    )";

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);

    if (error.error != QJsonParseError::NoError)
    {
        textEdit->append(
            "\nErrore parsing JSON");

        return;
    }

    QJsonObject root = doc.object();

    auto points =
        vtkSmartPointer<vtkPoints>::New();

    auto cells =
        vtkSmartPointer<vtkCellArray>::New();

    //-----------------------------------
    // Lettura vertici
    //-----------------------------------

    QJsonArray vertices =
        root["vertices"].toArray();

    for (const auto& vertex : vertices)
    {
        QJsonArray v = vertex.toArray();

        if (v.size() != 3)
            continue;

        points->InsertNextPoint(
            v[0].toDouble(),
            v[1].toDouble(),
            v[2].toDouble());
    }

    //-----------------------------------
    // Lettura triangoli
    //-----------------------------------

    QJsonArray faces =
        root["faces"].toArray();

    for (const auto& face : faces)
    {
        QJsonArray f = face.toArray();

        if (f.size() != 3)
            continue;

        auto tri =
            vtkSmartPointer<vtkTriangle>::New();

        tri->GetPointIds()->SetId(
            0,
            f[0].toInt());

        tri->GetPointIds()->SetId(
            1,
            f[1].toInt());

        tri->GetPointIds()->SetId(
            2,
            f[2].toInt());

        cells->InsertNextCell(tri);
    }

    //-----------------------------------
    // Costruzione mesh
    //-----------------------------------

    auto polyData =
        vtkSmartPointer<vtkPolyData>::New();

    polyData->SetPoints(points);
    polyData->SetPolys(cells);

    auto mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();

    mapper->SetInputData(polyData);

    auto actor =
        vtkSmartPointer<vtkActor>::New();

    actor->SetMapper(mapper);

    renderer->AddActor(actor);

    renderer->ResetCamera();

    vtkWidget->renderWindow()->Render();
}

