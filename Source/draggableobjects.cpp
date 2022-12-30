#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QMimeData>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QtMath>
#include <QGraphicsDropShadowEffect>
#include <QSpacerItem>
#include <QBitmap>
#include <QFontDialog>
#include <QColorDialog>
#include <QMenu>
#include <QAction>
#include <QTextStream>
#include <QBuffer>

#include "Headers/sidebar.h"
#include "Headers/draggableobjects.h"
#include "Headers/displayframe.h"

DraggableTextEdit::DraggableTextEdit(QWidget *parent): Draggable(parent), textEdit1{new CustomTextEdit(this)}
{

    this->setFrameStyle(QFrame::NoFrame);

    textEdit1->setGeometry(QRect(this->rect()));

    textEdit1->setFrameShape(QFrame::Panel);
    textEdit1->setFrameShadow(QFrame::Raised);
    textEdit1->setLineWidth(4);
    textEdit1->setMidLineWidth(0);

    textEdit1->setText(content);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setColor(QColor(1,0,0));
    shadow->setOffset(3);
    this->setGraphicsEffect(shadow);

    this->setObjectName("DraggableTextEdit");
    this->draggableType = TextEdit;
    this->setMinimumSize(75,100);

    textEdit1->setContextMenuPolicy(Qt::CustomContextMenu);
    textEdit1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textEdit1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(textEdit1, &QWidget::customContextMenuRequested, this, &DraggableTextEdit::showContextMenu);

    connect(textEdit1, &QTextEdit::textChanged, this, &DraggableTextEdit::onTextChange);
    connect(textEdit1, &CustomTextEdit::mousePressed, this, &Draggable::onComponentMouseButtonPressed);
    connect(textEdit1, &CustomTextEdit::mouseMoved, this, &Draggable::onComponentMouseButtonMoved);
    connect(textEdit1, &CustomTextEdit::mouseReleased, this, &Draggable::onComponentMouseButtonReleased);

    connect(this, &Draggable::changeHand, textEdit1, &CustomTextEdit::onHandChange);
}

QSize DraggableTextEdit::minimumSizeHint() const
{
    return QSize(50, 50);
}

void DraggableTextEdit::setContent(QString newContent)
{
    textEdit1->setText(newContent);
    content = newContent;
}

void DraggableTextEdit::setFont(QFont newFont)
{
    currentFont = newFont;
    textEdit1->setFont(newFont);
}

void DraggableTextEdit::setColor(QColor newColor)
{
    currentColor = newColor;
    QString tempContent = textEdit1->toPlainText();
    textEdit1->clear();
    textEdit1->setTextColor(newColor);
    content = tempContent;
    textEdit1->setText(content);
}

void DraggableTextEdit::resizeEvent(QResizeEvent *)
{
    textEdit1->setGeometry(QRect(this->rect()));
    this->update();
}

QSize DraggableTextEdit::sizeHint() const
{
    return QSize(100, 125);
}


void DraggableTextEdit::showContextMenu(QPoint pos)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction action1("Font", this);
    QAction action2("Color", this);

    connect(&action1, &QAction::triggered, this, &DraggableTextEdit::changeFont);
    connect(&action2,  &QAction::triggered, this,  &DraggableTextEdit::changeColor);
    contextMenu.addAction(&action1);
    contextMenu.addAction(&action2);

    contextMenu.exec(mapToGlobal(pos));
}


void DraggableTextEdit::onTextChange()
{
    content = textEdit1->toPlainText();
}

void DraggableTextEdit::changeFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(
                    &ok, currentFont, this);
    if (ok)
    {
        this->setFont(font);
    }
}

void DraggableTextEdit::changeColor()
{
    QColor color = QColorDialog::getColor(textEdit1->textColor());

    this->setColor(color);
}

DraggableLineEdit::DraggableLineEdit(QWidget *parent) :Draggable(parent), currentTextColor(0,0,0), frameColor(24,25,38)
{
    this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));

    lineEdit1 = new CustomLineEdit(this);
    lineEdit1->setGeometry(QRect(this->rect()));

    lineEdit1->setText(content);
    lineEdit1->setAutoFillBackground(true);

    QPalette palette = lineEdit1->palette();
    palette.setColor(QPalette::Base, Qt::transparent);
    lineEdit1->setPalette(palette);
    this->setColor(currentColor);

    this->draggableType = LineEdit;
    this->setObjectName("DraggableLineEdit");

    this->setMinimumSize(60,20);

    lineEdit1->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(lineEdit1, &QWidget::customContextMenuRequested, this, &DraggableLineEdit::showContextMenu);

    connect(lineEdit1, &QLineEdit::textChanged, this, &DraggableLineEdit::onTextChange);
    connect(lineEdit1, &CustomLineEdit::mousePressed, this, &Draggable::onComponentMouseButtonPressed);
    connect(lineEdit1, &CustomLineEdit::mouseMoved, this, &Draggable::onComponentMouseButtonMoved);
    connect(lineEdit1, &CustomLineEdit::mouseReleased, this, &Draggable::onComponentMouseButtonReleased);

    connect(this, &Draggable::changeHand, lineEdit1, &CustomLineEdit::onHandChange);
}


QSize DraggableLineEdit::sizeHint() const
{
    return QSize(60, 20);
}

void DraggableLineEdit::resizeEvent(QResizeEvent *)
{
    lineEdit1->setGeometry(QRect(this->rect()));
    this->update();
}

void DraggableLineEdit::onTextChange(const QString &text)
{
    content = text;
}

void DraggableLineEdit::showContextMenu(QPoint pos)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction action1("Font", this);
    QAction action2("Color", this);

    connect(&action1, &QAction::triggered, this, &DraggableLineEdit::changeFont);
    connect(&action2,  &QAction::triggered, this,  &DraggableLineEdit::changeColor);
    contextMenu.addAction(&action1);
    contextMenu.addAction(&action2);

    contextMenu.exec(mapToGlobal(pos));
}

void DraggableLineEdit::setFont(QFont newFont)
{
    currentFont = newFont;
    lineEdit1->setFont(newFont);
}

void DraggableLineEdit::setColor(QColor newColor)
{
    qDebug()<<"Current Font :"<<currentFont;
    currentColor = newColor;
    QString hexColor = newColor.name();
    QString tempContent = content;
    lineEdit1->clear();
    lineEdit1->setStyleSheet("CustomLineEdit:hover{"
                             "color: " + hexColor + ";"
                             "border: 0px;"
                             "background-color: rgba(255, 255, 255, 10)}"

                             "CustomLineEdit{"
                                 "color: " + hexColor + ";"
                                 "border: 0px;"
                                 "background-color: transparent}");

    lineEdit1->setFont(currentFont);
    content = tempContent;
    lineEdit1->setText(content);
}

void DraggableLineEdit::changeFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(
                    &ok, currentFont, this);
    if (ok)
    {
        this->setFont(font);
    }
}

void DraggableLineEdit::changeColor()
{
    QColor color = QColorDialog::getColor(currentTextColor);
    setColor(color);
}

void DraggableLineEdit::setContent(QString newContent)
{
    lineEdit1->setText(newContent);
    content = newContent;
}

DraggableCircleEdit::DraggableCircleEdit(QWidget *parent):
    Draggable(parent), circleEdit1{new CircleEdit(this)}, cursorY{0}, padding{5}
{
    this->setMinimumSize(60,60);
    this->draggableType = Circle;
    circleEdit1->setFrameShape(QFrame::NoFrame);
    circleEdit1->setGeometry(this->x() + padding, this->y() + padding, this->rect().width() - padding, this->rect().height() - padding);

    QPalette palette = circleEdit1->palette();
    palette.setColor(QPalette::Base, Qt::transparent);
    circleEdit1->setPalette(palette);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setColor(QColor(1,0,0));
    shadow->setOffset(2);
    this->setGraphicsEffect(shadow);
    circleEdit1->setAlignment(Qt::AlignCenter);
    cursorY = circleEdit1->cursorRect().y();

    circleEdit1->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(circleEdit1, &QWidget::customContextMenuRequested, this, &DraggableCircleEdit::showContextMenu);
    connect(circleEdit1, &QTextEdit::cursorPositionChanged, this, &DraggableCircleEdit::onCursorPosChange);
    connect(circleEdit1, &QTextEdit::textChanged, this, &DraggableCircleEdit::onTextChange);
    connect(circleEdit1, &CircleEdit::mousePressed, this, &Draggable::onComponentMouseButtonPressed);
    connect(circleEdit1, &CircleEdit::mouseMoved, this, &Draggable::onComponentMouseButtonMoved);
    connect(circleEdit1, &CircleEdit::mouseReleased, this, &Draggable::onComponentMouseButtonReleased);

    connect(this, &Draggable::changeHand, circleEdit1, &CircleEdit::onHandChange);
}

void DraggableCircleEdit::onTextChange()
{
    content = circleEdit1->toPlainText();
}

void DraggableCircleEdit::resizeEvent(QResizeEvent *event)
{
    circleEdit1->setGeometry(padding,padding, event->size().width() - 2*padding, event->size().height() - 2*padding);
    update();
}

void DraggableCircleEdit::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(30,150,104));

    painter.drawEllipse(circleEdit1->geometry());

    painter.setRenderHint(QPainter::Antialiasing, false);
    QRegion circularMask( QRect(0,0,size().width(), size().height()), QRegion::Ellipse );
    this->setMask(circularMask);
}

void DraggableCircleEdit::showContextMenu(QPoint pos)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction action1("Font", this);
    QAction action2("Color", this);

    connect(&action1, &QAction::triggered, this, &DraggableCircleEdit::changeFont);
    connect(&action2,  &QAction::triggered, this,  &DraggableCircleEdit::changeColor);
    contextMenu.addAction(&action1);
    contextMenu.addAction(&action2);

    contextMenu.exec(mapToGlobal(pos));
}

void DraggableCircleEdit::setFont(QFont newFont)
{
    currentFont = newFont;
    circleEdit1->setFont(newFont);
}

void DraggableCircleEdit::setColor(QColor newColor)
{
    currentColor = newColor;

    QString formattedContent;
    for(int i = 0; i < content.size(); i++)
    {
        if(content[i].isLetterOrNumber())
            formattedContent+=content[i];
        else if(content[i] == ' ')
        {
            if(formattedContent.size() == 0 || !(formattedContent[formattedContent.size() - 1] == ' '))
                formattedContent+=content[i];
        }
    }
    circleEdit1->clear();
    circleEdit1->setTextColor(newColor);
    content = formattedContent;
    this->setContent(content);

}


void DraggableCircleEdit::changeFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(
                    &ok, currentFont, this);
    if (ok)
    {
        this->setFont(font);
    }
}

void DraggableCircleEdit::changeColor()
{
    QColor color = QColorDialog::getColor(circleEdit1->textColor());

    this->setColor(color);
}

void DraggableCircleEdit::setContent(QString newContent)
{
    content = newContent;
    qDebug()<<"Setting"<< newContent;
    QTextCursor cursor;
    QString sep(" ");
    QStringList words = newContent.split(sep);

    for(int i = 0; i < words.size(); i++)
    {
            cursor = circleEdit1->textCursor();
            cursor.clearSelection();
            cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
            cursor.insertText(words[i]);;

            if(circleEdit1->cursorRect().y() < circleEdit1->rect().center().y())
            {
                if(!isPointInEllispe(circleEdit1->cursorRect().x(), circleEdit1->cursorRect().y()))
                {
                    cursor.removeSelectedText();
                    while(!isPointInEllispe(circleEdit1->cursorRect().x(), circleEdit1->cursorRect().y()))
                    {
                        if(circleEdit1->cursorRect().x() < circleEdit1->rect().center().x())
                        {
                            qDebug()<<"Move cursor";
                            circleEdit1->textCursor().insertText(" ");
                        }
                        else if(circleEdit1->cursorRect().x() > circleEdit1->rect().center().x())
                        {
                            circleEdit1->textCursor().insertText("\n");
                        }
                    }
                    cursor.insertText(words[i]);
                }

            }

            else
            {
                if(!isPointInEllispe(circleEdit1->cursorRect().bottomLeft().x(), circleEdit1->cursorRect().bottomLeft().y()))
                {
                    cursor.removeSelectedText();
                    while(!isPointInEllispe(circleEdit1->cursorRect().bottomLeft().x(), circleEdit1->cursorRect().bottomLeft().y()))
                    {
                        if(circleEdit1->cursorRect().x() < circleEdit1->rect().center().x())
                        {
                            circleEdit1->textCursor().insertText(" ");
                        }
                        else if(circleEdit1->cursorRect().x() > circleEdit1->rect().center().x())
                        {
                            circleEdit1->textCursor().insertText("\n");
                        }
                    }
                    cursor.insertText(words[i]);
                }
            }
            circleEdit1->textCursor().insertText(" ");

    }
}

void DraggableCircleEdit::write(QJsonObject &jsonObject)
{
    QPoint posInParent = this->pos();
    QJsonArray jpos{posInParent.x(), posInParent.y()};
    QJsonArray jsize{size().width(), size().height()};
    jsonObject["pos"] = jpos;
    jsonObject["size"] = jsize;
    QString formattedContent{""};
    content = circleEdit1->toPlainText();
    for(int i = 0; i < content.size(); i++)
    {
        if(content[i].isLetterOrNumber())
            formattedContent+=content[i];
        else if(content[i] == ' ')
        {
            if(formattedContent.size() == 0 || !(formattedContent[formattedContent.size() - 1] == ' '))
                formattedContent+=content[i];
        }
    }
    jsonObject["content"] = formattedContent;

    QJsonArray jcolor{currentColor.red(), currentColor.green(), currentColor.blue()};
    jsonObject["font"] = currentFont.toString();
    jsonObject["color"] = jcolor;
}

bool DraggableCircleEdit::isPointInEllispe(float x, float y)
{
    QRect ellipseRect = circleEdit1->rect();
    float result = pow(x - ellipseRect.center().x(), 2)/pow(ellipseRect.width()/2,2) + pow(y - ellipseRect.center().y(), 2)/pow(ellipseRect.height()/2,2);
    if(result < 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void DraggableCircleEdit::calculateMousePosition(QMouseEvent *event)
{
    QRect ellipseRect = circleEdit1->rect();
    float result = pow(event->pos().x() - ellipseRect.center().x(), 2)/pow(ellipseRect.width()/2,2) + pow(event->pos().y() - ellipseRect.center().y(), 2)/pow(ellipseRect.height()/2,2);
    if(event->pos().x() > this->rect().x() + this->rect().width()/2 && event->pos().y() > this->rect().x() + this->rect().width()/2 && abs(result - 1) <= 0.05)
    {
        //qDebug()
        this->setCursor(Qt::OpenHandCursor);
        emit changeHand(Qt::OpenHandCursor);
        setEdge(BottomRight);
    }

    else if(event->pos().x() <= this->rect().x() + this->rect().width() && event->pos().x() >= this->rect().x() &&
            event->pos().y() <= this->rect().y() + this->rect().height() && event->pos().y() >= this->rect().y())
    {
        emit changeHand(Qt::ArrowCursor);
        setEdge(Top);
    }

    else
    {
        if(!getBDragging() || !getBResizing())
            this->setCursor(Qt::ArrowCursor);
        setEdge(None);
    }
}



void DraggableCircleEdit::onCursorPosChange()
{
    if(circleEdit1->cursorRect().y() < circleEdit1->rect().center().y())
    {

        if(circleEdit1->cursorRect().x() < circleEdit1->rect().center().x() && !isPointInEllispe(circleEdit1->cursorRect().x(), circleEdit1->cursorRect().y()))
        {
            qDebug()<<"Move cursor";
            circleEdit1->textCursor().insertText(" ");
        }
        else if(circleEdit1->cursorRect().x() > circleEdit1->rect().center().x() && !isPointInEllispe(circleEdit1->cursorRect().x(), circleEdit1->cursorRect().y()))
        {
            circleEdit1->textCursor().insertText("\n");
        }
    }
    else
    {
        if(circleEdit1->cursorRect().x() < circleEdit1->rect().center().x() && !isPointInEllispe(circleEdit1->cursorRect().bottomLeft().x(), circleEdit1->cursorRect().bottomLeft().y()))
        {
            circleEdit1->textCursor().insertText(" ");
        }
    }
}


DraggableToDo::DraggableToDo(QWidget *parent):
    Draggable(parent), innerVerticalLayout{new QVBoxLayout()}, addToDoButton{new QPushButton("Add", this)}, outerVerticalLayout{new QVBoxLayout(this)}, draggedTodoContent{""},
    draggedTodoCheck{false}
{
    this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
    this->draggableType = Todo;
    this->setAcceptDrops(true);

    QHBoxLayout* temp = new QHBoxLayout();
    temp->setContentsMargins(8,3,8,3);
    addToDoButton->setFixedSize(80,25);
    addToDoButton->setObjectName("AddToDo");
    temp->addWidget(addToDoButton);

    QGraphicsDropShadowEffect* shadow1 = new QGraphicsDropShadowEffect(this);
    shadow1->setColor(QColor(1,0,0));
    shadow1->setOffset(1);
    addToDoButton->setGraphicsEffect(shadow1);

    outerVerticalLayout->addLayout(innerVerticalLayout);

    outerVerticalLayout->addStretch();
    outerVerticalLayout->addLayout(temp);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setColor(QColor(1,0,0));
    shadow->setOffset(3);
    this->setGraphicsEffect(shadow);

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &QWidget::customContextMenuRequested, this, &DraggableToDo::showContextMenu);
    connect(addToDoButton, &QPushButton::clicked, this, &DraggableToDo::addTodo);
    //connect(addToDoButton, &QPushButton::clicked, signalMapper, &QSignalMapper::map);
    this->setMinimumSize(100,100);
}

QSize DraggableToDo::sizeHint() const
{
    return QSize(60, 20);
}

void DraggableToDo::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qtdraggabletodo"))
    {

       event->setDropAction(Qt::CopyAction);
       event->accept();

    }
    else
    {
        event->accept();
    }
}

void DraggableToDo::dragLeaveEvent(QDragLeaveEvent *event)
{
    QWidget::dragLeaveEvent(event);
}

void DraggableToDo::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasFormat("application/x-qtdraggabletodo"))
    {
        QByteArray ba = event->mimeData()->data("application/x-qtdraggabletodo");
        QDataStream dataStream(&ba,QIODevice::ReadOnly);

        dataStream >> draggedTodoContent >> draggedTodoCheck;

        addTodo();
    }
}

void DraggableToDo::addTodo()
{

    toDoItems.append(new ToDoItem(this, toDoItems.length() + 1, draggedTodoContent, draggedTodoCheck, currentColor, currentFont));
    if(!toDoItems.isEmpty())
    {
        connect(toDoItems.last(), &ToDoItem::deleteMe, this, &DraggableToDo::deleteTodo);
        innerVerticalLayout->addWidget(toDoItems.last());
        toDoItems.last()->show();
    }
}

void DraggableToDo::deleteTodo(int todoNumber)
{
    ToDoItem *temp = toDoItems.at(todoNumber - 1);
    for(int i=todoNumber; i<toDoItems.length(); i++)
    {
        toDoItems[i - 1] = toDoItems[i];
    }
    temp->deleteLater();
    toDoItems.removeAt(toDoItems.length() - 1);
    for(int i=todoNumber - 1; i<toDoItems.length(); i++)
    {
        toDoItems.at(i)->setTodoNumber(toDoItems.at(i)->getTodoNumber() - 1);
    }

}

void DraggableToDo::write(QJsonObject &jsonObject)
{
    QPoint posInParent = this->pos();
    QJsonArray jpos{posInParent.x(), posInParent.y()};
    QJsonArray jsize{size().width(), size().height()};
    jsonObject["pos"] = jpos;
    jsonObject["size"] = jsize;
    QJsonArray jtodos;
    for(int i = 0; i < toDoItems.size(); i++)
    {
        QJsonObject jtodoElement;
        jtodoElement["content"] = toDoItems[i]->todoContent;
        jtodoElement["checked"] = toDoItems[i]->getIsChecked();
        jtodos.append(jtodoElement);
    }
    jsonObject["todoElements"] = jtodos;

    QJsonArray jcolor{currentColor.red(), currentColor.green(), currentColor.blue()};
    jsonObject["font"] = currentFont.toString();
    jsonObject["color"] = jcolor;
}

void DraggableToDo::showContextMenu(QPoint pos)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction action1("Font", this);
    QAction action2("Color", this);

    connect(&action1, &QAction::triggered, this, &DraggableToDo::changeFont);
    connect(&action2,  &QAction::triggered, this,  &DraggableToDo::changeColor);
    contextMenu.addAction(&action1);
    contextMenu.addAction(&action2);

    contextMenu.exec(mapToGlobal(pos));
}

void DraggableToDo::setFont(QFont newFont)
{
    currentFont = newFont;
    for(auto& todo : toDoItems)
        todo->setTextProperties(currentColor, newFont);
}

void DraggableToDo::setColor(QColor newColor)
{
    currentColor = newColor;
    for(auto& todo : toDoItems)
        todo->setTextProperties(newColor, currentFont);
}

void DraggableToDo::changeFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(
                    &ok, currentFont, this);
    if (ok)
    {
        this->setFont(font);
    }
}

void DraggableToDo::changeColor()
{
    currentColor = QColorDialog::getColor(currentColor);
    this->setColor(currentColor);
}


DraggableArrow::DraggableArrow(QWidget *parent):
    Draggable(parent),pixmapLabel{new QLabel(this)}, arrowPivot(0,0), arrowHeadPos(0,0), hitLength{36}, arrowAngle{0}, arrowLength{75}, arrowHeadMoved{false}
{
    this->draggableType = Arrow;
    pixmapLabel->setGeometry(this->rect());
    pixmap = QPixmap(pixmapLabel->width() - 2, pixmapLabel->height() -2);
    pixmap.fill(Qt::gray);
    arrowPivot = this->rect().center() - QPoint(hitLength/2, hitLength/2);
    arrowHeadPos = QPoint(this->width() - hitLength, this->height()/2 - hitLength/2);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setColor(QColor(1,0,0));
    shadow->setOffset(2);
    this->setGraphicsEffect(shadow);

    this->setMinimumSize(100,100);
}

void DraggableArrow::write(QJsonObject &jsonObject)
{
    QPoint posInParent = this->pos();
    QJsonArray jpos{posInParent.x(), posInParent.y()};
    QJsonArray jsize{size().width(), size().height()};
    QJsonArray jheadPos{arrowHeadPos.x(), arrowHeadPos.y()};
    jsonObject["pos"] = jpos;
    jsonObject["size"] = jsize;
    jsonObject["arrowlength"] = arrowLength;
    jsonObject["arrowangle"] = arrowAngle;
    jsonObject["arrowheadpos"] = jheadPos;
}

QSize DraggableArrow::sizeHint() const
{
    return QSize(400, 200);
}

void DraggableArrow::resizeEvent(QResizeEvent *)
{

    pixmapLabel->setGeometry(this->rect());
    if(!arrowHeadMoved)
    {
        arrowHeadPos = QPoint(this->width() - hitLength, this->height()/2 - hitLength/2);
    }
    arrowPivot = this->rect().center() - QPoint(hitLength/2, hitLength/2);
    drawArrow();
}

void DraggableArrow::drawArrow()
{   
    QPixmap maskPixmap(this->size());
    QPixmap arrowPixmap(this->size());
    maskPixmap.fill(Qt::transparent);
    arrowPixmap.fill(Qt::transparent);
    QPainter painter(&maskPixmap);
    QPainter painter2(&arrowPixmap);

    QPen pen;
    pen.setColor(Qt::black);
    painter.drawRect(this->rect());

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter2.setRenderHint(QPainter::Antialiasing, true);
    painter2.setRenderHint(QPainter::SmoothPixmapTransform);

    painter.translate(arrowPivot.x(), arrowPivot.y());
    painter2.translate(arrowPivot.x(), arrowPivot.y());


    painter.translate(-(this->width())/2, -(this->height())/2);
    painter.setBrush(Qt::yellow);
    //qDebug()<<arrowHeadPos;
    painter.drawRect(arrowHeadPos.x() - hitLength, arrowHeadPos.y() -  hitLength, hitLength, hitLength);
    painter.translate((this->width())/2, (this->height())/2);


    painter.rotate(arrowAngle);
    painter2.rotate(arrowAngle);

    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.setBrush(QColor(255,154,86));
    painter2.setPen(pen);
    painter2.setBrush(QColor(255,154,86));
    painter.drawRect(0.0, 0.0,arrowLength - 10.0,6.0);
    painter2.drawRect(0.0, 0.0,arrowLength - 10.0,6.0);
    QPointF points[3] = {QPointF(arrowLength - 12.0,-5.0),
                         QPointF(arrowLength, 3.0),
                         QPointF(arrowLength - 12.0,11.0)
                        };


    QPainterPath path;
    path.moveTo(points[0]);
    path.lineTo(points[1]);
    path.lineTo(points[2]);
    path.lineTo(points[0]);
    painter.fillPath(path, QBrush (QColor(123,250,60)));
    painter2.fillPath(path, QBrush (QColor(255,154,86)));

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter2.setRenderHint(QPainter::Antialiasing, false);
    pixmapLabel->setPixmap(arrowPixmap);
    //clearMask();
    this->setMask(maskPixmap.mask());
}

void DraggableArrow::setArrowLength(int newArrowLength)
{
    arrowLength = newArrowLength;
}

void DraggableArrow::setArrowAngle(float newArrowAngle)
{
    arrowAngle = newArrowAngle;
}


void DraggableArrow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        qDebug() << "Released!";

        Sidebar *sidebar = Sidebar::getInstance();
        if(sidebar->isOnTrash(mapToGlobal(event->pos()), this))
        {
            qDebug()<<"Trashed!";
            this->deleteLater();
        }

        if(this->getBDragging())
        {
            this->setBDragging(false);
        }

        else if(this->getBResizing())
        {
            this->setBResizing(false);
            arrowHeadPos = QPoint(this->width()/2 + ((arrowLength - 10) * cos(qDegreesToRadians(arrowAngle))),
                                  this->height()/2 + ((arrowLength - 10) * sin(qDegreesToRadians(arrowAngle))));
            drawArrow();
        }
    }
}

void DraggableArrow::calculateMousePosition(QMouseEvent *event)
{
    if(event->pos().x() > arrowHeadPos.x() - hitLength/2 && event->pos().x() < arrowHeadPos.x() + hitLength/2 && event->pos().y() > arrowHeadPos.y() -  hitLength/2 && event->pos().y() < arrowHeadPos.y() + hitLength/2)
    {
        this->setEdge(Draggable::Custom1);
        if(!arrowHeadMoved)
        {
            arrowHeadMoved = true;
        }
    }

    else if(event->pos().x() > arrowPivot.x() - hitLength/2 && event->pos().x() < arrowPivot.x() + hitLength/2 && event->pos().y() > arrowPivot.y() - hitLength/2 && event->pos().y() < arrowPivot.y() + hitLength/2)
    {
        this->setEdge(Draggable::Custom2);
    }

    else
    {
        this->setEdge(None);
    }

    qDebug()<<"Edge"<<getEdge();
}

void DraggableArrow::moveWidget(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton && !getBResizing())
    {

        if( getEdge() == Custom2 || getBDragging())
        {
            setBDragging(true);
            QPoint parentPos = mapToParent(event->pos() - offset);
            QPoint centrePos = mapToParent(this->rect().center());
            if(this->parentWidget() != nullptr )
            {
                if(centrePos.x() > 0 && centrePos.y() > 0 && centrePos.x() < this->parentWidget()->width() && centrePos.y() < this->parentWidget()->height())
                {
                    this->move(parentPos);
                    this->parentWidget()->update();
                }
            }
            else
            {
                qDebug()<<"Null";
            }
         }
    }
}

void DraggableArrow::resizeWidget(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton && !this->getBDragging())
    {
        if( this->getEdge() == Custom1 || this->getBResizing())
        {
            this->setBResizing(true);
            QVector2D mouseDirection(event->pos() - rect().center());
            arrowLength = mouseDirection.length();
            mouseDirection.normalize();
            QVector2D X(QPoint(1,0));
            float dotProd = QVector2D::dotProduct(X, mouseDirection);
            float determinant = 1 * mouseDirection.y() -  0 * mouseDirection.x();
            arrowAngle = qRadiansToDegrees(atan2(determinant, dotProd));

            QPoint centrePos = mapToParent(rect().center());
            QPoint gridPoint = getGridPoint(centrePos + QPoint(arrowLength +10 , arrowLength + 10));

            if(gridPoint.x() - this->getParentPosWhenClicked().x() > this->rect().width() && gridPoint.y() - this->getParentPosWhenClicked().y() > this->rect().height())
            {
                    this->setGeometry(this->getParentPosWhenClicked().x() - DisplayFrame::GRID_UNIT, this->getParentPosWhenClicked().y() - DisplayFrame::GRID_UNIT,
                                      gridPoint.x() - (this->getParentPosWhenClicked().x() - DisplayFrame::GRID_UNIT), gridPoint.y() - (this->getParentPosWhenClicked().y() - DisplayFrame::GRID_UNIT));
                    this->setParentPosWhenClicked(QPoint(this->getParentPosWhenClicked().x() - DisplayFrame::GRID_UNIT, this->getParentPosWhenClicked().y() - DisplayFrame::GRID_UNIT));

            }

            else if(gridPoint.x() - this->getParentPosWhenClicked().x() < this->rect().width() && gridPoint.y() - this->getParentPosWhenClicked().y() < this->rect().height())
            {
                if(this->minimumSize().width() < gridPoint.x() - this->getParentPosWhenClicked().x() && this->minimumSize().height() < gridPoint.y() - this->getParentPosWhenClicked().y())
                {
                        this->setGeometry(this->getParentPosWhenClicked().x() + DisplayFrame::GRID_UNIT, this->getParentPosWhenClicked().y() + DisplayFrame::GRID_UNIT,
                                          gridPoint.x() - (this->getParentPosWhenClicked().x() + DisplayFrame::GRID_UNIT), gridPoint.y() - (this->getParentPosWhenClicked().y() + DisplayFrame::GRID_UNIT));
                        this->setParentPosWhenClicked(QPoint(this->getParentPosWhenClicked().x() + DisplayFrame::GRID_UNIT, this->getParentPosWhenClicked().y() + DisplayFrame::GRID_UNIT));

                }

            }
            drawArrow();

        }
    }
}

void DraggableArrow::setArrowHeadPos(QPoint newArrowHeadPos)
{
    arrowHeadPos = newArrowHeadPos;
}

DraggableImage::DraggableImage(QPixmap pixmap, QWidget *parent): Draggable(parent), pixmapLabel{new QLabel(this)}
{
    this->draggableType = Image;
    this->pixmap = pixmap;
    pixmap.scaled(pixmapLabel->size());
    pixmapLabel->setPixmap(pixmap);
    this->setMinimumSize(150,150);
}

void DraggableImage::resizeEvent(QResizeEvent *)
{
    pixmapLabel->setGeometry(this->rect());
    pixmapLabel->setPixmap(pixmap.scaled(pixmapLabel->width(),pixmapLabel->height()));
}

void DraggableImage::write(QJsonObject &jsonObject)
{
    QPoint posInParent = this->pos();
    QJsonArray jpos{posInParent.x(), posInParent.y()};
    QJsonArray jsize{size().width(), size().height()};
    jsonObject["pos"] = jpos;
    jsonObject["size"] = jsize;


    QBuffer buff;
    buff.open(QIODevice::WriteOnly);
    pixmap.save(&buff, "PNG");

    QString binaryData(buff.data().toBase64().data());
    qDebug()<<"Start";
    jsonObject["data"] = binaryData;
    qDebug()<<"Sent";
}
