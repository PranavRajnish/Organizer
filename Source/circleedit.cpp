#include <QKeyEvent>
#include "circleedit.h"

CircleEdit::CircleEdit(QWidget *parent):QTextEdit(parent)
{

}

void CircleEdit::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case(Qt::Key_Tab):
    case(Qt::Key_Space):
        currentWord.clear();
        break;

    case(Qt::Key_Backspace):
        if(!currentWord.isNull())
        {
            currentWord = currentWord.remove(currentWord.length() - 1,1);
        }
        break;

    case(Qt::Key_Return):
    case(Qt::Key_Up):
    case(Qt::Key_Down):
    case(Qt::Key_Left):
    case(Qt::Key_Right):
    case(Qt::Key_Escape):
        break;
    default:
        currentWord.append(QChar(e->key()));
        break;
    }
    //qDebug()<<currentWord;
    QTextEdit::keyPressEvent(e);

}

void CircleEdit::mousePressEvent(QMouseEvent *event)
{
    QTextEdit::mousePressEvent(event);
    emit mousePressed(event);
    event->ignore();
}

void CircleEdit::mouseReleaseEvent(QMouseEvent *event)
{
    emit mouseReleased(event);
    event->accept();
}

void CircleEdit::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMoved(event);
    event->accept();
}

void CircleEdit::onHandChange(Qt::CursorShape cursorShape)
{
    this->viewport()->setCursor(cursorShape);
}

const QString &CircleEdit::getCurrentWord() const
{
    return currentWord;
}
