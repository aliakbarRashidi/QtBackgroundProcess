TEMPLATE = app

QT += core backgroundprocess
QT -= gui
CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = DemoApp

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
	testapp.cpp

HEADERS += \
	testapp.h

DISTFILES += \
	README.md

target.path = $$[QT_INSTALL_EXAMPLES]/backgroundprocess/DemoApp
INSTALLS += target

#not found by linker?
unix:!mac {
	LIBS += -L$$OUT_PWD/../../../lib #required to make this the first place to search
	LIBS += -L$$[QT_INSTALL_LIBS] -licudata
	LIBS += -L$$[QT_INSTALL_LIBS] -licui18n
	LIBS += -L$$[QT_INSTALL_LIBS] -licuuc
}

#add lib dir to rpath
mac: QMAKE_LFLAGS += '-Wl,-rpath,\'$$OUT_PWD/../../../lib\''
