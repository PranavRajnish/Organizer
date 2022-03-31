#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QTextEdit>
#include <QObject>

class CustomTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit CustomTextEdit(QWidget *parent = nullptr);

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

#endif // CUSTOMTEXTEDIT_H
