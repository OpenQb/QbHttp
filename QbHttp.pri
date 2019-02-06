INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/private/qbhttpserver_p.h \
    $$PWD/private/qbhttprequest_p.h \
    $$PWD/private/qbhttpresponse_p.h \
    $$PWD/private/qbhttpsettings_p.h \
    $$PWD/private/qbhttpstaticfilehandler.h \
    $$PWD/private/qbhttplogging_p.h \
    $$PWD/private/qbhttprequesthandler_p.h \
    $$PWD/private/qbhttpsmartrequesthandler_p.h \
    $$PWD/private/qbhttputil_p.h

SOURCES += \
    $$PWD/private/qbhttpserver.cpp \
    $$PWD/private/qbhttprequest.cpp \
    $$PWD/private/qbhttpresponse.cpp \
    $$PWD/private/qbhttpsettings.cpp \
    $$PWD/private/qbhttprequesthandler.cpp \
    $$PWD/private/qbhttpstaticfilehandler.cpp \
    $$PWD/private/qbhttplogging.cpp \
    $$PWD/private/qbhttpsmartrequesthandler.cpp \
    $$PWD/private/qbhttputil.cpp

HEADERS += \
    $$PWD/QbHttpServer

HEADERS += \
    $$PWD/QbHttp

RESOURCES +=
