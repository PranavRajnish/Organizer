#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QJsonArray>
#include <QJsonDocument>
#include <QCloseEvent>
#include <QScrollBar>

#include "logindialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), bsafeToClose{false}
{
    ui->setupUi(this);

    databaseHelper = DatabaseHelper::getInstance(this);
    databaseHelper->setAPIKey("AIzaSyCNuCIN4ao3ulzgyXi5aOz-vpBjdI1Zhy4");
    LoginDialog loginDialog;
    connect(&loginDialog, &LoginDialog::displayWindow, this, &MainWindow::onLogin);
    loginDialog.exec();

    qDebug()<<"after exec";
}

MainWindow::~MainWindow()
{
    delete ui;
}

QSize MainWindow::sizeHint() const
{
    return QSize(900,900);
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    int newWidth, newHeight;
    newWidth = qMax(displayFrame->size().width(), scrollArea->width());
    newHeight = qMax(displayFrame->size().height(), scrollArea->height());
    displayFrame->resize(newWidth, newHeight);
    //window->setGeometry(this->rect());
//    QSize difference = event->size() - event->oldSize();
//    scrollArea->resize(scrollArea->size() + difference);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!bsafeToClose)
    {
        QJsonObject jWindow;
        QJsonArray jtextEdits;
        QJsonArray jlineEdits;
        QJsonArray jcircleEdits;
        QJsonArray jtodos;
        QJsonArray jarrows;
        QJsonArray jimages;
        for(auto draggable : Draggable::savedObjects)
        {
            Draggable::DraggableType draggableType = draggable->draggableType;
            QJsonObject jdraggable;
            qDebug()<<"Draggable type : "<<draggableType;
            draggable->write(jdraggable);
            switch(draggableType)
            {
                case(Draggable::TextEdit):
                {
                    jtextEdits.append(jdraggable);
                    break;
                }
                case(Draggable::LineEdit):
                {
                    jlineEdits.append(jdraggable);
                    break;
                }
                case(Draggable::Circle):
                {
                    jcircleEdits.append(jdraggable);
                    break;
                }
                case(Draggable::Todo):
                {
                    jtodos.append(jdraggable);
                    break;
                }
                case(Draggable::Arrow):
                {
                    jarrows.append(jdraggable);
                    break;
                }
                case(Draggable::Image):
                {
                    jimages.append(jdraggable);
                    break;
                }
                default:
                    break;
            }
        }
        jWindow["textedits"] = jtextEdits;
        jWindow["lineedits"] = jlineEdits;
        jWindow["circleedits"] = jcircleEdits;
        jWindow["todos"] = jtodos;
        jWindow["arrows"] = jarrows;
        jWindow["images"] = jimages;

        QJsonDocument jWindowDoc(jWindow);
        event->ignore();
        databaseHelper->saveData(jWindowDoc);
    }
    else
    {
        event->accept();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_S)
    {
        for(auto draggable : Draggable::savedObjects)
        {
            qDebug()<<"Type :"<< draggable->draggableType<<"  Position :"<< draggable->pos()<<"  Size : "<< draggable->size();
        }
        qDebug()<<"Scroll Area Size"<<scrollArea->size();
        qDebug()<<"Main Window Size"<<this->size();
        qDebug()<<"Display Frame Size"<< DisplayFrame::getInstance()->size();
    }
}

void MainWindow::safeToClose()
{
    qDebug()<<"Trying to quit";
    bsafeToClose = true;
    QApplication::quit();

}

void MainWindow::onScrolling(int x, int y)
{
    //scrollArea->scroll(x,y);
    scrollArea->horizontalScrollBar()->setValue(scrollArea->horizontalScrollBar()->value() + x);
    scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->value() + y);
}

void MainWindow::onLogin()
{
    qDebug()<<"Welcome";

    displayFrame = DisplayFrame::getInstance(this);
    sidebar = Sidebar::getInstance(this);
    window = new QWidget(this);
    scrollArea = new QScrollArea(this);

    window -> setObjectName("window");
    window -> setContentsMargins(3,12,3,10);
    sidebar->setFixedSize(sidebar->getButtonSize().width(), sidebar->getButtonSize().height() * sidebar->getNumberOfButtons());

    scrollArea->setWidget(displayFrame);

    QVBoxLayout *sidebarLayout = new QVBoxLayout();
    sidebarLayout->addWidget(sidebar);
    sidebarLayout->addStretch();

    QHBoxLayout *scrollLayout = new QHBoxLayout();
    displayFrame->setLayout(scrollLayout);

    QHBoxLayout *horizontalLayout = new QHBoxLayout(window);

    horizontalLayout->addLayout(sidebarLayout);
    horizontalLayout->addWidget(scrollArea);

    setCentralWidget(window);

    displayFrame->resize(this->size());


    connect(displayFrame, &DisplayFrame::scrolling, this, &MainWindow::onScrolling);
    connect(databaseHelper, &DatabaseHelper::dataUploaded, this, &MainWindow::safeToClose);
}




