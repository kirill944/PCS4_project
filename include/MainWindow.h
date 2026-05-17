#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QToolBar>
#include <QIcon>
#include <QFileDialog>
#include <QInputDialog>
#include "GraphWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    GraphWidget* graphWidget;
    void setupMenuBar();
    void setupToolBar();
};

#endif // MAIN_WINDOW_H