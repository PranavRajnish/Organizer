#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QFileDialog>

#include "Headers/sidebar.h"
#include "Headers/displayframe.h"
#include "Headers/draggableobjects.h"

Sidebar* Sidebar::sidebarInstance{nullptr};

Sidebar::Sidebar(QWidget *parent) :
    QFrame(parent), buttonSize{40, 40}, offset(0,0), widgetType{None}, numberOfButtons{7}
{

    setMouseTracking(true);
    buttonIcons << "square" << "circle" << "label" << "todo" << "arrow" << "clip" <<"trash";
    this->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
    stackUnder(DisplayFrame::getInstance());
    connect(this, &Sidebar::imageReady, DisplayFrame::getInstance(), &DisplayFrame::onImageReady);
}

QSize Sidebar::sizeHint() const
{
    return QSize(40, buttonSize.height() * numberOfButtons);
}

QSize Sidebar::minimumSizeHint() const
{
    return QSize(40, window()->height());
}

bool Sidebar::isOnTrash(QPoint pos, Draggable*)
{
    QPoint newPos = this->mapFromGlobal(pos);
    QRect buttonRect(0, (numberOfButtons - 1) * buttonSize.height(), buttonSize.width(), buttonSize.height());

    return buttonRect.contains(newPos);
}

Sidebar* Sidebar::getInstance(QWidget *parent)
{
    if(sidebarInstance == nullptr)
    {
        sidebarInstance = new Sidebar(parent);
    }
    return sidebarInstance;
}

void Sidebar::resizeEvent(QResizeEvent*)
{
    buttonSize = QSize(this->width(), this->width());
    qDebug()<<"Resize:"<<buttonSize;
}

void Sidebar::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        offset = event->pos();
        parentPosWhenClicked = mapToParent(event->pos() - offset);
        calculateMousePosition(event);
    }
    QWidget::mousePressEvent(event);
}

void Sidebar::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
         int distance = (event->pos() - offset).manhattanLength();
         if( distance >= QApplication::startDragDistance())
         {

             if(widgetType != None && widgetType != Image)
             {

                 QString iconPath = ":/icons/Icons/";
                 iconPath = iconPath + buttonIcons[widgetType] + ".png";
                 QPixmap dragPixmap(iconPath);

                 QByteArray ba;

                 QDataStream dataStream(&ba,QIODevice::WriteOnly);

                 dataStream << widgetType;

                 QMimeData * mimeData = new QMimeData;
                 mimeData->setData("application/x-qtdraggable",ba);

                 QDrag * drag = new QDrag(this);
                 drag->setMimeData(mimeData);
                 dragPixmap = dragPixmap.scaled(25,25);
                 drag->setPixmap(dragPixmap);

                 if(drag->exec(Qt::MoveAction | Qt::CopyAction,Qt::CopyAction) == Qt::MoveAction)
                 {
                     return;
                 }
                 else
                 {
                 }

             }
         }
    }
}

void Sidebar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //qDebug()<<"Painting:";
    for(int i =0; i<numberOfButtons; i++)
    {
        QRect buttonRect(0, i * buttonSize.height(), buttonSize.width(), buttonSize.height());
        QString iconPath = ":/icons/Icons/";
        iconPath = iconPath + buttonIcons[i] + ".png";
        QRect iconRect(10, i * buttonSize.height() + 10, buttonSize.width() - 20,  buttonSize.height() - 20);
        QIcon buttonIcon(iconPath);
        buttonIcon.paint(&painter, iconRect);
    }
}

void Sidebar::calculateMousePosition(QMouseEvent *event)
{
    if(event->pos().y() < numberOfButtons * buttonSize.height())
    {

        if(event->pos().y() >= 0 && event->pos().y() < buttonSize.height())
        {
            widgetType = Note;
        }
        else if(event->pos().y() < 2 * buttonSize.height())
        {
            widgetType = CircleEdit;
        }
        else if(event->pos().y() < 3 * buttonSize.height())
        {
            widgetType = Label;
        }
        else if(event->pos().y() < 4 * buttonSize.height())
        {
            widgetType = Todo;
        }
        else if(event->pos().y() < 5 * buttonSize.height())
        {
            widgetType = Arrow;
        }
        else if(event->pos().y() < 6 * buttonSize.height())
        {
            imageSelected();
            widgetType = Image;
        }
        else
        {
            widgetType = None;
        }
    }
    else
    {
        widgetType = None;
    }

    qDebug()<<widgetType;
}

void Sidebar::imageSelected()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp)"));
    emit imageReady(fileName);
}

const QSize &Sidebar::getButtonSize() const
{
    return buttonSize;
}

int Sidebar::getNumberOfButtons() const
{
    return numberOfButtons;
}

void Sidebar::setNumberOfButtons(int newNumberOfButtons)
{
    numberOfButtons = newNumberOfButtons;
}


