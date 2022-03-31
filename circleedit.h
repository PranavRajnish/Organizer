#ifndef CIRCLEEDIT_H
#define CIRCLEEDIT_H

#include <QTextEdit>
#include <QObject>

class CircleEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit CircleEdit(QWidget *parent = nullptr);

    const QString& getCurrentWord() const;

protected:
    virtual void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QString currentWord;

    QWidget interface;

signals:
   void mousePressed(QMouseEvent *event);
   void mouseMoved(QMouseEvent *event);
   void mouseReleased(QMouseEvent *event);

public slots:
   void onHandChange(Qt::CursorShape cursorShape);
};

#endif // CIRCLEEDIT_H
