QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    acceptreq.cpp \
    addnew.cpp \
    listitem.cpp \
    main.cpp \
    chatwindow.cpp \
    friendlist.cpp \
    logwindow.cpp  \
    moredetail.cpp \
    newgroup.cpp \
    register.cpp \
    usrmain.cpp \
    chatmessagebox.cpp
#    c2s.cpp \
#    mytcpserver.cpp \
#    qt_chat_sql.cpp \
#    s2c.cpp \
#    socket.cpp \
#    u2c.cpp

HEADERS += \
    acceptreq.h \
    addnew.h \
    chatwindow.h \
    friendlist.h \
    listitem.h \
    logwindow.h \
    moredetail.h \
    newgroup.h \
    register.h \
    usrmain.h   \
    chatmessagebox.h
#    c2s.h \
#    mytcpserver.h \
#    qt_chat_sql.h \
#    s2c.h \
#    socket.h \
#    u2c.h \

FORMS += \
    acceptreq.ui \
    addnew.ui \
    chatwindow.ui \
    friendlist.ui \
    logwindow.ui \
    moredetail.ui \
    newgroup.ui \
    register.ui \
    usrmain.ui

RESOURCES += \
    source\img.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
