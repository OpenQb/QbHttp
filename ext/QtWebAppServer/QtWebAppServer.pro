#-------------------------------------------------
#
# Project created by QtCreator 2018-04-10T04:25:55
#
#-------------------------------------------------

TARGET = QtWebAppServer
TEMPLATE = lib
CONFIG += c++11
QT -= gui
QT += core

!win32:VERSION = 1.0.0

include(httpserver.pri)

#CONFIG(debug, debug|release) {
#     mac: TARGET = $$join(TARGET,,,_debug)
#     win32: TARGET = $$join(TARGET,,,d)
#}

TEMPNAME = $${QMAKE_QMAKE}
QTPATH = $$dirname(TEMPNAME)

#OBJECTS_DIR=.obj
#MOC_DIR=.moc
DEFINES+=DECLSPEC=Q_DECL_EXPORT


unix{
    CONFIG += lib_bundle
    QMAKE_PKGCONFIG_DESTDIR = pkgconfig
    FRAMEWORK_HEADERS.version = Versions
    FRAMEWORK_HEADERS.files = $$HEADERS
    FRAMEWORK_HEADERS.path = Headers
    QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS
    target.path=$$QTPATH/../lib/$${LIB_ARCH}
    INSTALLS = target
}

android{
    CONFIG -= android_install
    headers.path=$$[QT_INSTALL_HEADERS]/QtWebAppServer
    headers.files=$$HEADERS
    target.path=$$[QT_HOST_LIBS]
    INSTALLS = headers target
}

win32:*g++* {
    headers.path=$$[QT_INSTALL_HEADERS]/QtWebAppServer
    headers.files=$$HEADERS

    copy_dll.files = $$[QT_HOST_LIBS]/QtWebAppServer.dll
    copy_dll.path = $$QTPATH

    target.path=$$[QT_HOST_LIBS]

    INSTALLS = headers target copy_dll
}

linux:!android{
    headers.path=$$[QT_INSTALL_HEADERS]/QtWebAppServer
    headers.files=$$HEADERS
    target.path=$$[QT_INSTALL_HEADERS]/../lib
    INSTALLS = headers target
}