#ifndef DRAGGABLE_H
#define DRAGGABLE_H

#include <QFrame>
#include <QPoint>
#include <QJsonObject>
#include <QList>
#include <QEvent>

class Draggable : public QFrame
{
    Q_OBJECT
public:
    explicit Draggable(QWidget *parent = nullptr);

    ~Draggable();

    enum Edge{
        None,
        BottomRight,
        Top,
        Custom1,
        Custom2
    };

    enum DraggableType{
        TextEdit,
        LineEdit,
        Circle,
        Todo,
        Arrow,
        Image,
        Default
    };

    DraggableType draggableType;
    static QList<Draggable*> savedObjects;

    bool getBDragging() const;
    Draggable::Edge getEdge() const;
    void setBResizing(bool newBResizing);
    bool getBResizing() const;
    QPoint getParentPosWhenClicked() const;
    void setEdge(Edge newEdge);
    void setBDragging(bool newBDragging);
    void setParentPosWhenClicked(QPoint newParentPosWhenClicked);

    QPoint getGridPoint(QPoint mousePos);

    virtual void write(QJsonObject &jsonObject);

    virtual void setContent(QString newContent);

protected:
    QPoint offset;
    QString content;
    int buttonThickness;

    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void leaveEvent(QEvent *) override;

    //void paintEvent(QPaintEvent *event) override;

    virtual void calculateMousePosition(QMouseEvent *event);
    virtual void moveWidget(QMouseEvent *event);
    virtual void resizeWidget(QMouseEvent *event);
    //bool eventFilter(QObject *obj, QEvent *ev) override;

    QColor currentColor;
    QFont currentFont;

private:


    QPoint parentPosWhenClicked;

    bool bDragging;
    bool bResizing;

    Edge edge;

public slots:
    void onComponentMouseButtonPressed(QMouseEvent* mouseEvent);
    void onComponentMouseButtonMoved(QMouseEvent* mouseEvent);
    void onComponentMouseButtonReleased(QMouseEvent* mouseEvent);


signals:
    void fontChanged(QFont font);

    void changeHand(Qt::CursorShape cursorShape);
};

#endif // DRAGGABLE_H
