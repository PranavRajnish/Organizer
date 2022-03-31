#ifndef DRAGABLETEXTEDIT_H
#define DRAGABLETEXTEDIT_H
#include <QTextEdit>


class DragableTextEdit: public QTextEdit
{
    Q_OBJECT
public:
    explicit DragableTextEdit(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPoint offset;
};

#endif // DRAGABLETEXTEDIT_H
