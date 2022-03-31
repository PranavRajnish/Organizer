#ifndef DISPLAYFRAME_H
#define DISPLAYFRAME_H

#include <QFrame>
#include <QSize>
#include <QJsonDocument>


#include "draggableobjects.h"
#include "databasehelper.h"

class DisplayFrame : public QFrame
{
    Q_OBJECT
    static DisplayFrame *displayFrameInstance;
public:


    static DisplayFrame* getInstance(QWidget *parent = nullptr);

    DisplayFrame(DisplayFrame &other) = delete;

    void operator=(const DisplayFrame &) = delete;

    constexpr static const int GRID_UNIT{20};

    static const int GRID_OFFSET{1};
    static const int GROW_RATE;

protected:

    explicit DisplayFrame(QWidget *parent = nullptr);

    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dragMoveEvent(QDragMoveEvent *event) override;
    virtual void dragLeaveEvent(QDragLeaveEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;

    void paintEvent(QPaintEvent *) override;
    QSize sizeHint() const override;


private:
    DatabaseHelper *databaseHelper;

    bool isImage(QString fullpath);
    void createDraggable(Draggable *draggable, const QRect &geometry, QString content = "");

    void getDraggableInfo(const QJsonObject& json, QRect& geometry, QString& content);
    void getTextStyle(const QJsonObject& json, QFont& font, QColor& color);

public slots:
    void loadDraggables(QJsonDocument loadDoc);
    void onImageReady(QString fileName);

signals:
    void scrolling(int x, int y);
};

#endif // DISPLAYFRAME_H
