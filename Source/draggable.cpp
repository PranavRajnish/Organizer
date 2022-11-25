#include <QTextEdit>
#include <QMouseEvent>
#include <QDebug>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QtMath>
#include <QMenu>
#include <QAction>

#include "Headers/draggable.h"
#include "Headers/displayframe.h"
#include "Headers/sidebar.h"

QList<Draggable*> Draggable::savedObjects{};
Draggable::Draggable(QWidget *parent) : QFrame(parent), draggableType{Default}, content{""}, buttonThickness{10}, currentColor(22,23,31), currentFont("Helvetica [Cronyx]", 12),
                                        parentPosWhenClicked(0,0), bDragging{false}, bResizing{false}, edge{None}
{
    this->setMouseTracking(true);
    this->setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum));
    //qDebug() << "Right" << this->rect().x() + this->rect().width();
    savedObjects.push_back(this);
    this->raise();
}

Draggable::~Draggable()
{
    savedObjects.removeAt(savedObjects.indexOf(this));
}

void Draggable::onComponentMouseButtonPressed(QMouseEvent *event)
{
    mousePressEvent(event);
}

void Draggable::onComponentMouseButtonMoved(QMouseEvent *mouseEvent)
{
    mouseMoveEvent(mouseEvent);
}

void Draggable::onComponentMouseButtonReleased(QMouseEvent *mouseEvent)
{
    mouseReleaseEvent(mouseEvent);
}

void Draggable::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        qDebug() << "Clicked!";
        offset = event->pos();
        parentPosWhenClicked = mapToParent(event->pos() - offset);
        qDebug()<<offset;
        if(edge == BottomRight)
        {
            this->setCursor(Qt::ClosedHandCursor);
            emit changeHand(Qt::ClosedHandCursor);
        }

    }
}

QPoint Draggable::getGridPoint(QPoint mousePos)
{
    int remainderPosX = mousePos.x() % DisplayFrame::GRID_UNIT;
    int remainderPosY = mousePos.y() % DisplayFrame::GRID_UNIT;
    int gridPosX{};
    int gridPosY{};
    //QSize parentWidgetSize = this->parentWidget()->size();

    if(remainderPosX < DisplayFrame::GRID_UNIT/2)
    {

        gridPosX = qMax(0, mousePos.x() - remainderPosX);
    }
    else
    {
        //gridPosX = qMin(parentWidgetSize.width() - this->width(), mousePos.x() + (DisplayFrame::GRID_UNIT - remainderPosX));
        gridPosX = mousePos.x() + DisplayFrame::GRID_UNIT - remainderPosX;
    }

    if(remainderPosY < DisplayFrame::GRID_UNIT/2)
    {
        gridPosY = qMax(0, mousePos.y() - remainderPosY);
    }
    else
    {
        //gridPosY = qMin(parentWidgetSize.height() - this->height(), mousePos.y() + (DisplayFrame::GRID_UNIT - remainderPosY));
        gridPosY = mousePos.y() + (DisplayFrame::GRID_UNIT - remainderPosY);
    }

    return QPoint(gridPosX + DisplayFrame::GRID_OFFSET, gridPosY + DisplayFrame::GRID_OFFSET);
}

void Draggable::write(QJsonObject &jsonObject)
{
    QPoint posInParent = this->pos();
    QJsonArray jpos{posInParent.x(), posInParent.y()};
    QJsonArray jsize{size().width(), size().height()};
    QJsonArray jcolor{currentColor.red(), currentColor.green(), currentColor.blue()};
    jsonObject["content"] = content;
    jsonObject["pos"] = jpos;
    jsonObject["size"] = jsize;
    jsonObject["font"] = currentFont.toString();
    jsonObject["color"] = jcolor;
}

void Draggable::setContent(QString newContent)
{
    content = newContent;
}

void Draggable::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        qDebug() << "Released!";
        Sidebar *sidebar = Sidebar::getInstance();
        this->setCursor(Qt::ArrowCursor);
        if(sidebar->isOnTrash(mapToGlobal(event->pos()), this))
        {
            qDebug()<<"Trashed!";
            this->deleteLater();
        }
        if(bDragging)
        {
        //Snapping to grid
            bDragging = false;
            QPoint parentPos = mapToParent(event->pos() - offset);
            QPoint gridPos = getGridPoint(parentPos);

            qDebug()<<"Snap to:"<<gridPos.x()<<gridPos.y();
            this->move(gridPos.x(), gridPos.y());
            this->parentWidget()->update();
        }

        if(bResizing)
        {
            bResizing = false;
        }
    }
}

void Draggable::mouseMoveEvent(QMouseEvent *event)
{
    //Finding mouse position relative to widget.
    calculateMousePosition(event);
    moveWidget(event);
    resizeWidget(event);
}

void Draggable::leaveEvent(QEvent *)
{
    edge = None;
}

void Draggable::calculateMousePosition(QMouseEvent *event)
{
    if(event->pos().x() <= this->rect().x() + this->rect().width() && event->pos().x() >= this->rect().x() + this->rect().width() - buttonThickness
            && event->pos().y() <= this->rect().y() + this->rect().height() && event->pos().y() >= this->rect().y() + this->rect().height() - buttonThickness)
    {
        //qDebug()
        this->setCursor(Qt::OpenHandCursor);
        emit changeHand(Qt::OpenHandCursor);
        edge = BottomRight;
    }

    //else if(event->pos().x() <= this->rect().x() + this->rect().width() && event->pos().x() >= this->rect().x() && event->pos().y() <= (this->rect().y() + buttonThickness)
                //&& event->pos().y() >= this->rect().y())
    else if(event->pos().x() <= this->rect().x() + this->rect().width() && event->pos().x() >= this->rect().x() &&
            event->pos().y() <= this->rect().y() + this->rect().height() && event->pos().y() >= this->rect().y())
    {
        //qDebug()<<"Top";
        this->setCursor(Qt::ArrowCursor);
        emit changeHand(Qt::ArrowCursor);
        edge = Top;
    }

    else
    {
        if(!bResizing)
        {
            this->setCursor(Qt::ArrowCursor);
            qDebug()<<"Emitting arrow";
            emit changeHand(Qt::ArrowCursor);
        }
        edge = None;
    }
}

void Draggable::moveWidget(QMouseEvent* event)
{
    if(event->buttons() == Qt::LeftButton && !bResizing)
    {

        if( edge == Top || bDragging)
        {
            //Moving with mouse
            qDebug()<<"Holding";
            bDragging = true;
            QPoint parentPos = mapToParent(event->pos() - offset);
            QPoint windowPos = mapToGlobal(event->pos());
            if(this->parentWidget() != nullptr )
            {
                //Keeping object within frame
                if(windowPos.x() > 0 && windowPos.y() > 0 && parentPos.x() < (this->parentWidget()->width()) - (this->width()) && parentPos.y() < (this->parentWidget()->height()) - (this->height()))
                {
                    this->move(parentPos);
                    //this->update(QRect(this->pos() - QPoint(10,10), this->size() + QSize(10,10)));
                    QRect updateArea(this->pos() - QPoint(20,20),mapToParent(this->rect().bottomRight()) + QPoint(20,20));
                    this->parentWidget()->update(updateArea);
                    //this->update(updateArea);
                }
            }
            else
            {
                qDebug()<<"Null";
            }
         }
    }
}

void Draggable::resizeWidget(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton && !bDragging)
    {
        if( edge == BottomRight || bResizing)
        {
            this->setCursor(Qt::ClosedHandCursor);
            emit changeHand(Qt::ClosedHandCursor);
            bResizing = true;
            QPoint mousePoint = mapToParent(event->pos());
            QPoint gridPoint = getGridPoint(mousePoint);
            //qDebug()<<"Mouse pod: "<< mousePoint<<" \n" << "Closest grid point: "<< gridPoint;
            this->setGeometry(parentPosWhenClicked.x(),parentPosWhenClicked.y(), gridPoint.x() - parentPosWhenClicked.x(),gridPoint.y() - parentPosWhenClicked.y());
            this->parentWidget()->update(this->rect());
        }
    }
}

QPoint Draggable::getParentPosWhenClicked() const
{
    return parentPosWhenClicked;
}

void Draggable::setParentPosWhenClicked(QPoint newParentPosWhenClicked)
{
    parentPosWhenClicked = newParentPosWhenClicked;
}


bool Draggable::getBResizing() const
{
    return bResizing;
}

void Draggable::setBResizing(bool newBResizing)
{
    bResizing = newBResizing;
}

Draggable::Edge Draggable::getEdge() const
{
    return edge;
}

void Draggable::setEdge(Edge newEdge)
{
    edge = newEdge;
}


bool Draggable::getBDragging() const
{
    return bDragging;
}

void Draggable::setBDragging(bool newBDragging)
{
    bDragging = newBDragging;
}

