TEMPLATE = lib
TARGET = QbHttp
QT += qml quick core

CONFIG += plugin c++11
CONFIG -= android_install

INCLUDEPATH += $$PWD


TARGET = $$qtLibraryTarget($$TARGET)
uri = Qb.Http



include(QbHttp.pri)
include(ext/QtWebAppServer/httpserver.pri)


DISTFILES = qmldir QbHttp.qmltypes

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmltypes.commands = qmlplugindump -nonrelocatable Qb.Http 1.0 > $$PWD/QbHttp.qmltypes
QMAKE_EXTRA_TARGETS += qmltypes

qmldir.files = qmldir QbHttp.qmltypes
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}

HEADERS += \
    qbhttp_plugin.h

SOURCES += \
    qbhttp_plugin.cpp
