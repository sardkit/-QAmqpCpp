QT -= gui
QT += core network

CONFIG += c++11 c++17
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES +=  \
    QMqManager/QRabbitmqMgr.cpp \
    QMqManager/QTcpClient.cpp \
    QMqManager/QTcpConnectionHandler.cpp \
    main.cpp

HEADERS += \
    QMqManager/QRabbitmqMgr.h \
    QMqManager/QTcpClient.h \
    QMqManager/QTcpConnectionHandler.h \
    
INCLUDEPATH += $$PWD/QMqManager


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# >>>>>>>>>>> user define <<<<<<<<<

## RabbitMQ client
INCLUDEPATH += $$PWD/AmpqCpp/include
win32 {
    LIBS += $$PWD/AmpqCpp/lib/amqpcpp.lib
}

