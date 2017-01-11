#-------------------------------------------------
#
# Project created by QtCreator 2016-11-15T06:34:42
#
#-------------------------------------------------
############################################################################
# Projekt:	Diplomarbeit: Autmatische Abfüllanlage
# Host:		Android System
#
# Entwickler:	Wögerbauer Stefan
# E-Mail:	woegste@hotmail.com
#
############################################################################


QT       += core
QT       += gui
QT       += bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = automatische_Abfuellanlage_Client
TEMPLATE = app


SOURCES +=  main.cpp\
            mainwindow.cpp \
            remoteselector.cpp \
            bluetoothtransmissionclient.cpp \
            mixture.cpp

HEADERS  += mainwindow.h \
            remoteselector.h \
            bluetoothtransmissionclient.h \
            mixture.h

FORMS    += mainwindow.ui \
            remoteselector.ui

CONFIG +=   mobility
MOBILITY = 

DISTFILES += \
            android/AndroidManifest.xml \
            android/gradle/wrapper/gradle-wrapper.jar \
            android/gradlew \
            android/res/values/libs.xml \
            android/build.gradle \
            android/gradle/wrapper/gradle-wrapper.properties \
            android/gradlew.bat \
            LICENSE.md

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
