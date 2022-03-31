#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QFrame>
#include <QPushButton>
#include <QIcon>

#include "draggable.h"

class Sidebar : public QFrame
{
    Q_OBJECT
    static Sidebar *sidebarInstance;

public:


    virtual QSize sizeHint() const override;
    virtual QSize minimumSizeHint() const override;

    bool isOnTrash(QPoint pos, Draggable *draggable);

    static Sidebar* getInstance(QWidget *parent = nullptr);

    Sidebar(Sidebar &other) = delete;

    void operator=(const Sidebar &) = delete;
    enum WidgetType{
        Note,
        CircleEdit,
        Label,
        Todo,
        Arrow,
        Image,
        Trash,
        None
    };


    int getNumberOfButtons() const;
    void setNumberOfButtons(int newNumberOfButtons);

    const QSize &getButtonSize() const;

protected:

    explicit Sidebar(QWidget *parent = nullptr);
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

//        void leaveEvent(QEvent * event) override;
    virtual void paintEvent(QPaintEvent *event) override;


    void calculateMousePosition(QMouseEvent *event);

private:

    void imageSelected();

    QPushButton *addWidgetButton1, *addWidgetButton2, *addWidgetButton3, *addWidgetButton4, *addWidgetButton5;
    QSize buttonSize;
    QPoint offset;
    QPoint parentPosWhenClicked;
    QStringList buttonIcons;
    WidgetType widgetType;

    int numberOfButtons;

private slots:
//    void on_addWidgetButtonclicked();
//    void on_addWidgetButtonclicked2();
//    void on_addWidgetButtonclicked3();
//    void on_addWidgetButtonclicked4();
//    void on_addWidgetButtonclicked5();

signals:
        void imageReady(QString fileName);
};

#endif // SIDEBAR_H
