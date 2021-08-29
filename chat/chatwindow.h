#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>

namespace Ui {
class chatWindow;
}

class chatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit chatWindow(QWidget *parent = nullptr);
    ~chatWindow();

private:
    Ui::chatWindow *ui;
};

#endif // CHATWINDOW_H
