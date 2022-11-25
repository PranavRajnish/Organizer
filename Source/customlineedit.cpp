#include "Headers/customlineedit.h"

#include <QMouseEvent>

CustomLineEdit::CustomLineEdit(QWidget* parent):QLineEdit(parent)
{

}

void CustomLineEdit::mousePressEvent(QMouseEvent *event)
{
    QLineEdit::mousePressEvent(event);
    emit mousePressed(event);
    event->ignore();
}

void CustomLineEdit::mouseReleaseEvent(QMouseEvent *event)
{
    emit mouseReleased(event);
    event->accept();
}

void CustomLineEdit::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMoved(event);
    event->accept();
}

void CustomLineEdit::onHandChange(Qt::CursorShape cursorShape)
{
    this->setCursor(cursorShape);
}
