#-------------------------------------------------
#
# Project created by QtCreator 2016-10-25T16:34:25
#
#-------------------------------------------------

TEMPLATE = lib
QT       += network
QT       -= gui

TARGET = QtBackgroundProcess
VERSION = 1.2.1

win32 {
	QMAKE_TARGET_COMPANY = "Skycoder42"
	QMAKE_TARGET_PRODUCT = "QtBackgroundProcess"
	QMAKE_TARGET_DESCRIPTION = $$QMAKE_TARGET_PRODUCT
	QMAKE_TARGET_COPYRIGHT = "Felix Barz"

	CONFIG += skip_target_version_ext
} else:mac {
	QMAKE_TARGET_BUNDLE_PREFIX = "com.Skycoder42."
	QMAKE_FRAMEWORK_BUNDLE_NAME = "QtBackgroundProcess"

	CONFIG += lib_bundle
	QMAKE_LFLAGS_SONAME = -Wl,-install_name,@rpath/
	QMAKE_LFLAGS += '-Wl,-rpath,\'@executable_path/../Frameworks\''
} else:unix {
	QMAKE_LFLAGS += '-Wl,-rpath,\'.\''
	QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN/../lib\''
	QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN/lib\''
}

include(../QCtrlSignals/qctrlsignals.pri)

DEFINES += QTBACKGROUNDPROCESS_LIBRARY

SOURCES += \
    qtexception.cpp \
    app.cpp \
    app_p.cpp \
    terminal.cpp \
    terminal_p.cpp \
    masterconnecter.cpp \
    globalterminal.cpp

HEADERS += QtBackgroundProcess \
	qtbackgroundprocess_global.h \
    qtexception.h \
    app.h \
	app_p.h \
    terminal.h \
    terminal_p.h \
    masterconnecter.h \
    globalterminal.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
