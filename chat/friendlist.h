#ifndef FRIENDLIST_H
#define FRIENDLIST_H

#include <QWidget>

namespace Ui {
class FriendList;
}

class FriendList : public QWidget
{
    Q_OBJECT

public:
    explicit FriendList(QWidget *parent = nullptr);
    ~FriendList();

private:
    Ui::FriendList *ui;
};

#endif // FRIENDLIST_H
