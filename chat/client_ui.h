#ifndef CLIENT_UI_H
#define CLIENT_UI_H

#include <QWidget>

namespace Ui {
class client_ui;
}

class client_ui : public QWidget
{
    Q_OBJECT

public:
    explicit client_ui(QWidget *parent = nullptr);
    ~client_ui();

private:
    Ui::client_ui *ui;
};

#endif // CLIENT_UI_H
