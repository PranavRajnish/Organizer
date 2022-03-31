#include <QPainter>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QPen>
#include <QJsonArray>
#include <QJsonObject>
#include <QPoint>

#include "displayframe.h"
#include "sidebar.h"
#include "todoitem.h"

DisplayFrame* DisplayFrame::displayFrameInstance{nullptr};
const int DisplayFrame::GROW_RATE{10};

DisplayFrame *DisplayFrame::getInstance(QWidget *parent)
{
    if(displayFrameInstance == nullptr)
    {
        displayFrameInstance = new DisplayFrame(parent);
        displayFrameInstance->setMouseTracking(true);
    }

    return displayFrameInstance;
}

DisplayFrame::DisplayFrame(QWidget *parent): QFrame(parent)
{
    this->setAcceptDrops(true);
    this->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    this->setBackgroundRole(QPalette::Base);

    databaseHelper = DatabaseHelper::getInstance();
    connect(databaseHelper, &DatabaseHelper::draggableData, this, &DisplayFrame::loadDraggables);
    databaseHelper->loadData();

    //connect(Sidebar::getInstance(), &Sidebar::imageReady, this, &DisplayFrame::onImageReady);
}

void DisplayFrame::mouseMoveEvent(QMouseEvent *event)
{

    if(mapToParent(event->pos()).x() > parentWidget()->size().width() - 20)
    {
        this->setCursor(Qt::SizeHorCursor);
        if(event->buttons() == Qt::LeftButton)
        {
            this->resize(size().width() + GROW_RATE, size().height());
            emit scrolling(GROW_RATE, 0);
        }
    }
    else if(mapToParent(event->pos()).y() > parentWidget()->size().height() - 20)
    {
        this->setCursor(Qt::SizeVerCursor);
        if(event->buttons() == Qt::LeftButton)
        {
            this->resize(size().width(), size().height() + GROW_RATE);
            emit scrolling(0, GROW_RATE);
        }
    }

    else
    {
        this->setCursor(Qt::ArrowCursor);
    }
}

void DisplayFrame::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor("#BABCCF"));
    painter.setPen(pen);

    for(int i = GRID_OFFSET; i<= this->width(); i += this->GRID_UNIT)
    {
        for(int j = GRID_OFFSET; j<= this->height(); j+=this->GRID_UNIT)
        {
            painter.drawPoint(i,j);
        }
    }
}

QSize DisplayFrame::sizeHint() const
{
    return QSize(600, 600);
}

void DisplayFrame::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasUrls()){
           QList<QUrl> urls = event->mimeData()->urls();
           if(urls.count() > 1)
               return;

           QFileInfo file(urls.at(0).toLocalFile());
           QPixmap mPixmap;
           if(isImage(file.absoluteFilePath()) && (mPixmap.load(file.absoluteFilePath()))){
              //ui->label->setPixmap(mPixmap.scaled(ui->label->size())) ;
               DraggableImage *draggable = new DraggableImage(mPixmap, this);
               draggable->setGeometry(qMax(0,event->pos().x() - 150),qMax(0,event->pos().y() - 150),300,300);
               draggable->show();
           }
    }

    else if(event->mimeData()->hasFormat("application/x-qtdraggable"))
    {
        QByteArray ba = event->mimeData()->data("application/x-qtdraggable");
        QDataStream dataStream(&ba,QIODevice::ReadOnly);

        Sidebar::WidgetType widgetType;

        dataStream >> widgetType;
        switch(widgetType)
        {
        case(Sidebar::Note):
        {
            QSize size(100, 100);
            createDraggable(new DraggableTextEdit(this), QRect(QPoint(qMax(0,event->pos().x() - size.width()/2), qMax(0,event->pos().y() - size.height()/2)), size));
        }
        break;
        case(Sidebar::CircleEdit):
        {
            QSize size(100, 100);
            createDraggable(new DraggableCircleEdit(this), QRect(QPoint(qMax(0,event->pos().x() - size.width()/2), qMax(0,event->pos().y() - size.height()/2)), size));
        }
            break;
        case(Sidebar::Label):
        {
            QSize size(100, 50);
            createDraggable(new DraggableLineEdit(this), QRect(QPoint(qMax(0,event->pos().x() - size.width()/2), qMax(0,event->pos().y() - size.height()/2)), size));

        }
            break;
        case(Sidebar::Todo):
        {
            QSize size(100, 100);
            DraggableToDo* todo = new DraggableToDo(this);
            createDraggable(todo, QRect(QPoint(qMax(0,event->pos().x() - size.width()/2), qMax(0,event->pos().y() - size.height()/2)), size));
            todo->addTodo();
        }
            break;
        case(Sidebar::Arrow):
        {
            QSize size(150, 150);
            createDraggable(new DraggableArrow(this), QRect(QPoint(qMax(0,event->pos().x() - size.width()/2), qMax(0,event->pos().y() - size.height()/2)), size));
        }
            break;
        default:
            break;
        }
    }
}

bool DisplayFrame::isImage(QString fullpath)
{
    QFileInfo file(fullpath);
    return ((file.suffix()=="png") ||
            (file.suffix() =="PNG") ||
            (file.suffix() == "jpg") ||
            (file.suffix() =="JPG")||
            (file.suffix() == "jpeg") ||
            (file.suffix() =="JPeG"));
}

void DisplayFrame::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qtdraggable"))
    {

       event->setDropAction(Qt::CopyAction);
       event->accept();
       //event->ignore();

    }
    else
    {
        event->accept();
    }
}

void DisplayFrame::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qtdraggable"))
    {
       event->setDropAction(Qt::CopyAction);
       event->accept();
       //event->ignore();

    }
    else
    {
        event->accept();
    }
}

void DisplayFrame::dragLeaveEvent(QDragLeaveEvent *event)
{
    QWidget::dragLeaveEvent(event);
}

void DisplayFrame::createDraggable(Draggable *draggable, const QRect &geometry, QString content)
{
    draggable->setGeometry(geometry);
    draggable->show();
    QPoint parentPos(geometry.x(),geometry.y());
    QPoint gridPos = draggable->getGridPoint(parentPos);

    draggable->move(gridPos.x(), gridPos.y());
    if(draggable->draggableType == Draggable::TextEdit || draggable->draggableType == Draggable::LineEdit || draggable->draggableType == Draggable::Circle)
    {
        draggable->setContent(content);
    }
}

void DisplayFrame::getDraggableInfo(const QJsonObject& json, QRect &geometry, QString &content)
{
        QJsonArray sizeArray = json["size"].toArray();
        QSize size(sizeArray[0].toInt(), sizeArray[1].toInt());
        QJsonArray posArray = json["pos"].toArray();
        QPoint pos(posArray[0].toInt(), posArray[1].toInt());
        if(json.contains("content") && json["content"].isString())
        {
            content = json["content"].toString();
        }

        geometry.setTopLeft(pos);
        geometry.setSize(size);

}

void DisplayFrame::getTextStyle(const QJsonObject& json, QFont &font, QColor &color)
{
    if(json.contains("font") && json["font"].isString())
    {
        QString stringFont = json["font"].toString();
        font.fromString(stringFont);
    }
    if(json.contains("color") && json["color"].isArray())
    {
        QJsonArray colorArray = json["color"].toArray();
        color = QColor::fromRgb(colorArray.at(0).toInt(), colorArray.at(1).toInt(), colorArray.at(2).toInt());
        qDebug()<<color;
    }
}
void DisplayFrame::loadDraggables(QJsonDocument loadDoc)
{
    qDebug()<<"Trying to load";
    QJsonObject json = loadDoc.object();
    QRect geometry;
    QString content;
    if(json.contains("textedits") && json["textedits"].isArray())
    {
        QJsonArray textEdits = json["textedits"].toArray();
        for(int i = 0; i < textEdits.size(); i++)
        {
            QJsonObject jDraggable = textEdits[i].toObject();
            getDraggableInfo(jDraggable, geometry, content);

            DraggableTextEdit* draggableTextEdit = new DraggableTextEdit(this);
            createDraggable(draggableTextEdit, geometry, content);
            QFont font;
            QColor color;
            getTextStyle(jDraggable, font, color);
            draggableTextEdit->setFont(font);
            draggableTextEdit->setColor(color);
        }
    }
    if(json.contains("lineedits") && json["lineedits"].isArray())
    {
        QJsonArray lineEdits = json["lineedits"].toArray();
        for(int i = 0; i < lineEdits.size(); i++)
        {
            QJsonObject jDraggable = lineEdits[i].toObject();
            getDraggableInfo(jDraggable, geometry, content);
            DraggableLineEdit* draggableLineEdit = new DraggableLineEdit(this);
            createDraggable(draggableLineEdit, geometry, content);
            QFont font;
            QColor color;
            getTextStyle(jDraggable, font, color);
            draggableLineEdit->setFont(font);
            draggableLineEdit->setColor(color);
        }
    }

    if(json.contains("circleedits") && json["circleedits"].isArray())
    {
        QJsonArray circleEdits = json["circleedits"].toArray();
        for(int i = 0; i < circleEdits.size(); i++)
        {
            QJsonObject jDraggable = circleEdits[i].toObject();
            getDraggableInfo(jDraggable, geometry, content);
            DraggableCircleEdit* draggableCircleEdit = new DraggableCircleEdit(this);
            createDraggable(draggableCircleEdit, geometry, content);
            QFont font;
            QColor color;
            getTextStyle(jDraggable, font, color);
            draggableCircleEdit->setFont(font);
            draggableCircleEdit->setColor(color);
        }
    }

    if(json.contains("todos") && json["todos"].isArray())
    {
        QJsonArray todos = json["todos"].toArray();
        for(int i = 0; i < todos.size(); i++)
        {
            QJsonObject jDraggable = todos[i].toObject();
            getDraggableInfo(jDraggable, geometry, content);
            DraggableToDo *draggableTodo = new DraggableToDo(this);
            createDraggable(draggableTodo, geometry, content);
            QJsonArray jtodoElements = jDraggable["todoElements"].toArray();
            for (int j = 0; j < jtodoElements.size(); j++)
            {
                QJsonObject jtodoElement = jtodoElements[j].toObject();
                draggableTodo->toDoItems.append(new ToDoItem(draggableTodo, draggableTodo->toDoItems.length() + 1, jtodoElement["content"].toString(), jtodoElement["checked"].toBool()));
                if(!draggableTodo->toDoItems.isEmpty())
                {
                    connect(draggableTodo->toDoItems.last(), &ToDoItem::deleteMe, draggableTodo, &DraggableToDo::deleteTodo);
                    draggableTodo->innerVerticalLayout->addWidget(draggableTodo->toDoItems.last());
                }
            }
            QFont font;
            QColor color;
            getTextStyle(jDraggable, font, color);
            draggableTodo->setFont(font);
            draggableTodo->setColor(color);
        }
    }

    if(json.contains("arrows") && json["arrows"].isArray())
    {
        QJsonArray arrows = json["arrows"].toArray();
        for(int i = 0; i < arrows.size(); i++)
        {
            QJsonObject jDraggable = arrows[i].toObject();
            getDraggableInfo(jDraggable, geometry, content);
            DraggableArrow *draggableArrow = new DraggableArrow(this);
            createDraggable(draggableArrow, geometry);

            QVariant arrowAngle = jDraggable["arrowangle"].toVariant();
            draggableArrow->setArrowAngle(arrowAngle.toFloat());

            int arrowLength = jDraggable["arrowlength"].toInt();
            draggableArrow->setArrowLength(arrowLength);

            QJsonArray arrowHeadPos = jDraggable["arrowheadpos"].toArray();
            draggableArrow->setArrowHeadPos(QPoint(arrowHeadPos.at(0).toInt(), arrowHeadPos.at(1).toInt()));

            draggableArrow->drawArrow();

        }
    }

    if(json.contains("images") && json["images"].isArray())
    {
        QJsonArray jImages = json["images"].toArray();
        for(int i = 0; i < jImages.size(); i++)
        {
            QJsonObject jDraggable = jImages[i].toObject();
            getDraggableInfo(jDraggable, geometry, content);

            QByteArray base64Data = jDraggable["data"].toString().toUtf8();
            QPixmap mPixmap;
            mPixmap.loadFromData(QByteArray::fromBase64(base64Data), "PNG");
            //QPixmap mPixmap(jDraggable["data"].toString().toUtf8());
            DraggableImage* draggableImage = new DraggableImage(mPixmap, this);
            createDraggable(draggableImage, geometry, content);
        }
    }
}

void DisplayFrame::onImageReady(QString fileName)
{
    QPixmap mPixmap;
    qDebug()<<"Image File Path :"<<fileName;
    if(mPixmap.load(fileName))
    {
       //ui->label->setPixmap(mPixmap.scaled(ui->label->size())) ;
        DraggableImage *draggableImage = new DraggableImage(mPixmap, this);
        draggableImage->setGeometry(0,0,200,200);
        draggableImage->show();
    }
}
