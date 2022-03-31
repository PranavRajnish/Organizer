#ifndef DRAGGABLEOBJECTS_H
#define DRAGGABLEOBJECTS_H

#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QSignalMapper>

#include "draggable.h"
#include "circleedit.h"
#include "customtextedit.h"
#include "customlineedit.h"
#include "todoitem.h"


class DraggableTextEdit : public Draggable
{
    Q_OBJECT
    public:
        explicit DraggableTextEdit(QWidget *parent = nullptr);
        virtual QSize minimumSizeHint() const override;
        void setContent(QString newContent) override;

        void setFont(QFont newFont);
        void setColor(QColor newColor);

    protected :
        void resizeEvent(QResizeEvent *event) override;
        QSize sizeHint() const override;

    private:
        CustomTextEdit *textEdit1;

    private slots:
        void onTextChange();
        void showContextMenu(QPoint);
        void changeFont();
        void changeColor();

};


class DraggableLineEdit : public Draggable
{
    Q_OBJECT
    public:
        explicit DraggableLineEdit(QWidget *parent = nullptr);
        void setContent(QString newContent) override;

        void setFont(QFont newFont);
        void setColor(QColor newColor);

    protected :
        QSize sizeHint() const override;
        void resizeEvent(QResizeEvent *event) override;

    private:
        CustomLineEdit *lineEdit1;
        QColor currentTextColor;
        QColor frameColor;
    private slots:
        void onTextChange(const QString &text);
        void showContextMenu(QPoint);
        void changeFont();
        void changeColor();

};


//Circluar text edit
class DraggableCircleEdit : public Draggable
{
    Q_OBJECT

    public:
        explicit DraggableCircleEdit(QWidget *parent = nullptr);
        void setContent(QString newContent) override;
        //virtual QSize minimumSizeHint() const override;
        void write(QJsonObject &jsonObject) override;

        void setFont(QFont newFont);
        void setColor(QColor newColor);

    protected :
        //QSize sizeHint() const override;
        virtual void resizeEvent(QResizeEvent *event) override;
        virtual void paintEvent(QPaintEvent *event) override;

    private:
        bool isPointInEllispe(float x, float y);
        void calculateMousePosition(QMouseEvent *event) override;

        CircleEdit *circleEdit1;
        int cursorY;
        int padding;

    private slots:
        void onCursorPosChange();
        void onTextChange();
        void showContextMenu(QPoint);
        void changeFont();
        void changeColor();

};


//To-do list implementation
class DraggableToDo : public Draggable
{
    Q_OBJECT
public:
    explicit DraggableToDo(QWidget *parent = nullptr);

    virtual void write(QJsonObject &jsonObject) override;
    QList<ToDoItem*> toDoItems;
    QVBoxLayout *innerVerticalLayout;

    void setFont(QFont newFont);
    void setColor(QColor newColor);

protected :
    QSize sizeHint() const override;

    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    //virtual void dragMoveEvent(QDragMoveEvent *event) override;
    virtual void dragLeaveEvent(QDragLeaveEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;


private:

    QPushButton *addToDoButton;
    QVBoxLayout *outerVerticalLayout;
    QSignalMapper* signalMapper;

    QString draggedTodoContent;
    bool draggedTodoCheck;

private slots:
    void showContextMenu(QPoint);
    void changeFont();
    void changeColor();

public slots:
    void addTodo();
    void deleteTodo(int todoNumber);
};



//Arrows implementation
class DraggableArrow : public Draggable
{
    Q_OBJECT
public:
    explicit DraggableArrow(QWidget *parent = nullptr);
    //void paintEvent(QPaintEvent *event) override;
    virtual void write(QJsonObject &jsonObject) override;

    void setArrowAngle(float newArrowAngle);

    void setArrowLength(int newArrowLength);

    void drawArrow();

    void setArrowHeadPos(QPoint newArrowHeadPos);

protected :
    QSize sizeHint() const override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void moveWidget(QMouseEvent *event) override;
    virtual void calculateMousePosition(QMouseEvent *event) override;
    virtual void resizeWidget(QMouseEvent *event) override;


private:


    QPixmap pixmap;
    QLabel *pixmapLabel;
    QPoint arrowPivot;
    QPoint arrowHeadPos;
    float hitLength;
    float arrowAngle;
    int arrowLength;
    bool arrowHeadMoved;

};


class DraggableImage : public Draggable
{
    Q_OBJECT
public:
    explicit DraggableImage(QPixmap pixmap,QWidget *parent = nullptr);
    //void paintEvent(QPaintEvent *event) override;

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    QPixmap pixmap;
    QLabel *pixmapLabel;

    // Draggable interface
public:
    void write(QJsonObject &jsonObject) override;
};
#endif // DRAGGABLELINEEDIT_H
