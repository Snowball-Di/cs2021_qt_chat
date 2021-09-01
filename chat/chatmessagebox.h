#ifndef CHATMESSAGEBOX_H
#define CHATMESSAGEBOX_H


#include <QWidget>

class QPaintEvent;
class QPainter;
class QLabel;
class QMovie;

class chatmessagebox : public QWidget
{
    Q_OBJECT
public:
    explicit chatmessagebox(QWidget *parent = nullptr);

    //标识聊天框中消息的类型
    enum Text_Type{
        User_System,//系统
        User_Me,    //自己
        User_She,   //用户
        User_Time,  //时间
    };

    void setItemSuccess();
    void setItem(QString text, time_t time, QSize allSize, Text_Type userType);

    QSize getRealString(QString src);
    QSize fontRect(QString str);

    inline QString text() {return m_msg;}
    inline time_t time() {return m_time;}
    inline Text_Type userType() {return m_userType;}

protected:
    void paintEvent(QPaintEvent *event);

private:
    QString m_msg;
    time_t m_time;
    QString m_curTime;

    QSize m_allSize;
    Text_Type m_userType = User_System;

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
    QLabel* m_loading = Q_NULLPTR;
    QMovie* m_loadingMovie = Q_NULLPTR;
    bool m_isSending = false;
};

#endif // CHATMESSAGEBOX_H
