#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "GraphWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT  // Добавлен макрос Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    GraphWidget* graphWidget;
    void setupMenuBar();
};

#endif // MAIN_WINDOW_H