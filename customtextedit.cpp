#include "customtextedit.h"
#include <QMouseEvent>
#include <QDebug>

CustomTextEdit::CustomTextEdit(QWidget* parent):QTextEdit(parent)
{
    this->unsetCursor();
    //this->setCursor(cursorShape)
}

void CustomTextEdit::mousePressEvent(QMouseEvent *event)
{
    QTextEdit::mousePressEvent(event);
    emit mousePressed(event);
    event->ignore();
}

void CustomTextEdit::mouseReleaseEvent(QMouseEvent *event)
{
    emit mouseReleased(event);
    event->accept();
}

void CustomTextEdit::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMoved(event);
    event->accept();
}

void CustomTextEdit::onHandChange(Qt::CursorShape cursorShape)
{
    this->viewport()->setCursor(cursorShape);
}

