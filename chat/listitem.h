#ifndef DIALOGITEM_H
#define DIALOGITEM_H

#include <QWidget>

class QPaintEvent;
class QPaintEvent;
class QPainter;
class QLabel;
class QMovie;

class listItem :public QWidget
{
    Q_OBJECT
public:
    explicit listItem(QWidget *parent = nullptr);

    enum List_Type
    {
        Friend,
        Group
    };

    inline QString name() {return d_name;};

    void setItem(QString name, QPixmap pic, List_Type type);
    void setItemDone();

    QSize getRealString(QString src);
    QSize fontRect(QString str);

    inline int get_dialog_id(){return this->d_id;};

protected:
    void paintEvent(QPaintEvent *event); 

private:
    QString d_name; //会话对象名字
    QPixmap d_pic; //会话对象头像
    int d_id; //会话对象标识号码

    bool d_loaded = false; //是否得到响应
    List_Type d_type; // 默认为用户会话
    int m_kuangWidth;
    int m_textWidth;
    int m_spaceWid;
    int m_lineHeight;

    QRect m_iconLeftRect;
    QRect m_iconRightRect;
    QRect m_sanjiaoLeftRect;
    QRect m_sanjiaoRightRect;
    QRect m_kuangLeftRect;
    QRect m_kuangRightRect;
    QRect m_textLeftRect;
    QRect m_textRightRect;
    QPixmap m_leftPixmap;
    QPixmap m_rightPixmap;

    QMovie *d_waitingMovie = Q_NULLPTR;
    QLabel *d_waiting = Q_NULLPTR;
};

#endif // DIALOGITEM_H
