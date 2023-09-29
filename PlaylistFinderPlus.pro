QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/src

SOURCES += \
    src/ddns/ddnsservice.cpp \
    src/model/ipaddress.cpp \
    src/playlistparser/playlistparser.cpp \
    src/scanner/scanner.cpp \
    src/scanner/util.cpp \
    src/ui/scantablemodel.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/ddns/ddns.h \
    src/ddns/ddnsservice.h \
    src/model/ipaddress.h \
    src/playlistparser/parseresult.h \
    src/playlistparser/playlistparser.h \
    src/scanner/httpheaders.h \
    src/scanner/ipportkey.h \
    src/scanner/scanner.h \
    src/scanner/util.h \
    src/ui/scanresult.h \
    src/ui/scantablemodel.h \
    src/mainwindow.h

FORMS += src/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
