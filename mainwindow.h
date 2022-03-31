#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>

#include "displayframe.h"
#include "sidebar.h"
#include "draggable.h"
#include "databasehelper.h"

QT_BEGIN_NAMESPACE

namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual QSize sizeHint() const override;


protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void closeEvent(QCloseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;


private:

    Ui::MainWindow *ui;

    Sidebar *sidebar;
    DisplayFrame *displayFrame;
    QWidget *window;
    QScrollArea *scrollArea;
    DatabaseHelper *databaseHelper;

    bool bsafeToClose;
    QString curUsername;
    QString curPassword;

private slots:
    void safeToClose();
    void onScrolling(int x, int y);
    void onLogin();

};
#endif // MAINWINDOW_H
