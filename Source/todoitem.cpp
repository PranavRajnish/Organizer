#include <QApplication>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QDebug>
#include <QSizePolicy>
#include <QGraphicsDropShadowEffect>
#include <QPainter>

#include "todoitem.h"

ToDoItem::ToDoItem(QWidget *parent, int todoNumber, QString newTodoContent, bool bisChecked, QColor currentColor, QFont currentFont): QFrame(parent), todoContent{newTodoContent}, horizontalLayout{new QHBoxLayout(this)}, pushButton{new QPushButton(this)}, lineEdit{new QLineEdit(this)},
    dotIconPath{":/icons/Icons/dot.png"}, checkIconPath{":/icons/Icons/check.png"}, isChecked{bisChecked}, offset{0,0}, todoNumber{0}
{

    horizontalLayout->addWidget(pushButton, 0);
    horizontalLayout->addWidget(lineEdit, 10);
    pushButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    pushButton->setFixedSize(18,20);
    pushButton->setObjectName("Check Button");

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setColor(QColor(1,0,0));
    shadow->setOffset(1);
    pushButton->setGraphicsEffect(shadow);

    QGraphicsDropShadowEffect* shadow1 = new QGraphicsDropShadowEffect(this);
    shadow1->setColor(QColor(1,0,0));
    shadow1->setOffset(1);
    pushButton->setGraphicsEffect(shadow1);

    lineEdit->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
    lineEdit->setObjectName("Todo Element");

    QGraphicsDropShadowEffect* shadow2 = new QGraphicsDropShadowEffect(this);
    shadow2->setColor(QColor(1,0,0));
    shadow2->setOffset(1);
    lineEdit->setGraphicsEffect(shadow1);

    this->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    this->setLineWidth(2);
    this->setMidLineWidth(1);

    this->todoNumber = todoNumber;
    lineEdit->setText(todoContent);

    QPainter painter(pushButton);
    if(isChecked)
    {
        //pixmap.load(checkIconPath);
        pushButton->setIcon(QIcon(checkIconPath));

    }
    else
    {
        //pixmap.load(dotIconPath);
        pushButton->setIcon(QIcon(dotIconPath));
    }

//    QIcon buttonIcon(pixmap);
//    pushButton->setIcon(buttonIcon);
    pushButton->setIconSize(pushButton->size());

    setTextProperties(currentColor, currentFont);

    connect(pushButton, &QPushButton::clicked, this, &ToDoItem::checkToDo);
    connect(lineEdit, &QLineEdit::textEdited, this, &ToDoItem::onTextChange);
}

void ToDoItem::onTextChange(const QString &text)
{
    todoContent = text;
}

void ToDoItem::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        offset = event->pos();
    }
    QWidget::mousePressEvent(event);
}

void ToDoItem::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
         int distance = (event->pos() - offset).manhattanLength();
         //qDebug() << "Distance:"<<distance;
         if( distance >= QApplication::startDragDistance())
         {

             QByteArray ba;
             QDataStream dataStream(&ba,QIODevice::WriteOnly);

             dataStream << lineEdit->text() << isChecked;
             QMimeData * mimeData = new QMimeData;
             mimeData->setData("application/x-qtdraggabletodo",ba);

             QDrag * drag = new QDrag(this);
             drag->setMimeData(mimeData);
             //dragPixmap = dragPixmap.scaled(25,25);
             //drag->setPixmap(dragPixmap);

             if(drag->exec(Qt::MoveAction | Qt::CopyAction,Qt::CopyAction) == Qt::MoveAction)
             {
//                                //Move data
//                                 qDebug()<< "Moving data";
//                                 child->close();
                 qDebug()<<"Moving todo";
                 return;
             }
             else
             {
                 qDebug()<<"hey todo";
                 emit deleteMe(todoNumber);
                 return;
             }
         }
    }
}

void ToDoItem::checkToDo()
{
    QPixmap pixmap;
    if(isChecked)
    {
        pixmap.load(dotIconPath);
        isChecked = false;
    }
    else
    {
        pixmap.load(checkIconPath);
        isChecked = true;
    }
    QIcon buttonIcon(pixmap);
    pushButton->setIcon(buttonIcon);
    pushButton->setIconSize(pushButton->size());
}

bool ToDoItem::getIsChecked() const
{
    return isChecked;
}

void ToDoItem::setTextProperties(QColor color, QFont font)
{
    QString tempContent = todoContent;
    lineEdit->clear();
    QString hexColor = color.name();
    lineEdit->setStyleSheet("QLineEdit{"
                            "color: " + hexColor + ";"
                            "font-family: " + font.family() + ";"
                            "font-size: " + QString::number(font.pointSize()) + "pt;"
                            "font-style: " + font.styleName() + ";"
                            "font-weight: " + QString::number(font.weight()) + ";"
                            "background-color: #B36DFF;"
                            "border-radius: 5px;}");
    todoContent = tempContent;
    lineEdit->setText(tempContent);
}

void ToDoItem::setTodoNumber(int newTodoNumber)
{
    todoNumber = newTodoNumber;
}

int ToDoItem::getTodoNumber() const
{
    return todoNumber;
}

