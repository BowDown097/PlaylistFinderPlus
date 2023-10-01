QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/src

SOURCES += \
    src/model/ipaddress.cpp \
    src/playlistparser/playlistparser.cpp \
    src/scanner/httpheaders.cpp \
    src/scanner/scanner.cpp \
    src/scanner/scantask.cpp \
    src/scanner/scanutil.cpp \
    src/ui/scantablemodel.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/whois/whoisclient.cpp \
    src/whois/whoisutil.cpp

HEADERS += \
    src/model/ipaddress.h \
    src/playlistparser/parseresult.h \
    src/playlistparser/playlistparser.h \
    src/scanner/httpheaders.h \
    src/scanner/ipportkey.h \
    src/scanner/scanner.h \
    src/scanner/scantask.h \
    src/scanner/scanutil.h \
    src/task/controllabletask.h \
    src/task/runcontrollabletask.h \
    src/task/taskcontrol.h \
    src/task/taskexecutor.h \
    src/ui/scanresult.h \
    src/ui/scantablemodel.h \
    src/mainwindow.h \
    src/whois/ipinfo.h \
    src/whois/range.h \
    src/whois/whoisclient.h \
    src/whois/whoisresult.h \
    src/whois/whoisutil.h

FORMS += src/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
