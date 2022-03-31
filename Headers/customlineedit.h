#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QLineEdit>
#include <QObject>

class CustomLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CustomLineEdit(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
   void mousePressed(QMouseEvent *event);
   void mouseMoved(QMouseEvent *event);
   void mouseReleased(QMouseEvent *event);

public slots:
   void onHandChange(Qt::CursorShape cursorShape);
};

#endif // CUSTOMLINEEDIT_H
