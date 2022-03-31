#ifndef TODOITEM_H
#define TODOITEM_H

#include <QWidget>
#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QIcon>

class ToDoItem : public QFrame
{
    Q_OBJECT
public:
    QString todoContent;

    explicit ToDoItem(QWidget *parent = nullptr, int todoNumber = 0, QString todoContent = "", bool bisChecked = false, QColor currentColor = QColor(), QFont currentFont = QFont());

    int getTodoNumber() const;
    void setTodoNumber(int newTodoNumber);

    bool getIsChecked() const;

    void setTextProperties(QColor, QFont);

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

private:
    void checkToDo();

    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QLineEdit *lineEdit;
    QString dotIconPath;
    QString checkIconPath;

    bool isChecked;
    QPoint offset;
    int todoNumber;

private slots:
    void onTextChange(const QString &text);
signals:
    void deleteMe(int todoNumber);


};

#endif // TODOITEM_H
