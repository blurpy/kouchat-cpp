######################################################################
# Automatically generated by qmake (2.01a) Thu Aug 9 18:41:05 2007
######################################################################

TEMPLATE = app
TARGET = ../kouchat
DEPENDPATH += .
INCLUDEPATH += .
QT += network xml
CONFIG += release
CONFIG -= debug_and_release
win32:LIBS += "C:\Program Files\Microsoft Platform SDK for Windows\Lib\Ws2_32.lib"
RC_FILE = src.rc

# Input
HEADERS += InnstillingerGUI.h \
           kougui.h \
           ListeModell.h \
           mottaker.h \
           NickDTO.h \
           senderen.h
FORMS += innstillinger.ui kougui.ui
SOURCES += InnstillingerGUI.cpp \
           kouchat.cpp \
           kougui.cpp \
           ListeModell.cpp \
           mottaker.cpp \
           NickDTO.cpp \
           senderen.cpp