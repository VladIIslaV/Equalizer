#-------------------------------------------------
#
# Project created by QtCreator 2018-02-17T15:00:33
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
QT       += printsupport
QT       += sensors

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Frequencestein
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    qcustomplot.cpp \
    audiointerface.cpp \
    fastfouriertransform.cpp \
    FFT/sources/qcomplexnumber.cpp \
    FFT/sources/qfouriercalculator.cpp \
    FFT/sources/qfourierfixedcalculator.cpp \
    FFT/sources/qfouriertransformer.cpp \
    FFT/sources/qfouriervariablecalculator.cpp \
    FFT/sources/qwindowfunction.cpp

HEADERS += \
        mainwindow.h \
    qcustomplot.h \
    audiointerface.h \
    fastfouriertransform.h \
    FFT/fftreal/Array.h \
    FFT/fftreal/Array.hpp \
    FFT/fftreal/def.h \
    FFT/fftreal/DynArray.h \
    FFT/fftreal/DynArray.hpp \
    FFT/fftreal/FFTReal.h \
    FFT/fftreal/FFTReal.hpp \
    FFT/fftreal/FFTRealFixLen.h \
    FFT/fftreal/FFTRealFixLen.hpp \
    FFT/fftreal/FFTRealFixLenParam.h \
    FFT/fftreal/FFTRealPassDirect.h \
    FFT/fftreal/FFTRealPassDirect.hpp \
    FFT/fftreal/FFTRealPassInverse.h \
    FFT/fftreal/FFTRealPassInverse.hpp \
    FFT/fftreal/FFTRealSelect.h \
    FFT/fftreal/FFTRealSelect.hpp \
    FFT/fftreal/FFTRealUseTrigo.h \
    FFT/fftreal/FFTRealUseTrigo.hpp \
    FFT/fftreal/OscSinCos.h \
    FFT/fftreal/OscSinCos.hpp \
    FFT/headers/qcomplexnumber.h \
    FFT/headers/qfouriercalculator.h \
    FFT/headers/qfourierfixedcalculator.h \
    FFT/headers/qfouriertransformer.h \
    FFT/headers/qfouriervariablecalculator.h \
    FFT/headers/qwindowfunction.h

FORMS += \
        mainwindow.ui

