# by: Kwabena W. Agyeman

TEMPLATE = app
TARGET = omniacreator

QT += concurrent opengl printsupport serialport svg widgets

SOURCES += main.cpp

RESOURCES += omniacreator.qrc

# Include InterfaceLibrary

INCLUDEPATH += $$PWD/interfacelibrary
DEPENDPATH += $$PWD/interfacelibrary

HEADERS += $$PWD/interfacelibrary/il.h \
           $$PWD/interfacelibrary/ilshared.h \
           $$PWD/interfacelibrary/ildependencies.h \
           $$PWD/interfacelibrary/ilcore.h \
           $$PWD/interfacelibrary/ilterminal.h \
           $$PWD/interfacelibrary/iloscilloscope.h \
           $$PWD/interfacelibrary/iltable.h \
           $$PWD/interfacelibrary/iltree.h \
           $$PWD/interfacelibrary/ilgraphics.h \
           $$PWD/interfacelibrary/ilinterface.h \
           $$PWD/interfacelibrary/iloscilloscopebar.h \
           $$PWD/interfacelibrary/iloscilloscopecurve.h \
           $$PWD/interfacelibrary/iloscilloscopegraph.h \
           $$PWD/interfacelibrary/iloscilloscopebox.h \
           $$PWD/interfacelibrary/ilinterfacegroupbox.h \
           $$PWD/interfacelibrary/ilinterfacepushbutton.h \
           $$PWD/interfacelibrary/ilinterfaceradiobutton.h \
           $$PWD/interfacelibrary/ilinterfacecheckbox.h \
           $$PWD/interfacelibrary/ilinterfacelineedit.h \
           $$PWD/interfacelibrary/ilinterfaceintspinbox.h \
           $$PWD/interfacelibrary/ilinterfacebinspinbox.h \
           $$PWD/interfacelibrary/ilinterfacehexspinbox.h \
           $$PWD/interfacelibrary/ilinterfacedoublespinbox.h \
           $$PWD/interfacelibrary/ilinterfacetimeedit.h \
           $$PWD/interfacelibrary/ilinterfacedateedit.h \
           $$PWD/interfacelibrary/ilinterfacedatetimeedit.h \
           $$PWD/interfacelibrary/ilinterfaceslider.h \
           $$PWD/interfacelibrary/ilinterfacelabel.h \
           $$PWD/interfacelibrary/ilinterfaceprogressbar.h \
           $$PWD/interfacelibrary/ilinterfaceseperator.h

# Include QCustomPlot

DEPENDPATH += $$PWD/qcustomplot
INCLUDEPATH += $$PWD/qcustomplot

HEADERS += $$PWD/qcustomplot/qcustomplot.h
SOURCES += $$PWD/qcustomplot/qcustomplot.cpp

# Include FFTW3

DEPENDPATH += $$PWD/fftw/api \
              $$PWD/fftw/dft \
              $$PWD/fftw/dft/scaler \
              $$PWD/fftw/dft/simd \
              $$PWD/fftw/kernal \
              $$PWD/fftw/rdft \
              $$PWD/fftw/rdft/scaler \
              $$PWD/fftw/rdft/simd \
              $$PWD/fftw/reodft \
              $$PWD/fftw/simd-support \
              $$PWD/fftw/threads
INCLUDEPATH += $$PWD/fftw/api \
               $$PWD/fftw/dft \
               $$PWD/fftw/dft/scaler \
               $$PWD/fftw/dft/simd \
               $$PWD/fftw/kernal \
               $$PWD/fftw/rdft \
               $$PWD/fftw/rdft/scaler \
               $$PWD/fftw/rdft/simd \
               $$PWD/fftw/reodft \
               $$PWD/fftw/simd-support \
               $$PWD/fftw/threads

HEADERS += $$PWD/fftw/api/*.h \
           $$PWD/fftw/dft/*.h \
           $$PWD/fftw/dft/scaler/*.h \
           $$PWD/fftw/dft/simd/*.h \
           $$PWD/fftw/kernal/*.h \
           $$PWD/fftw/rdft/*.h \
           $$PWD/fftw/rdft/scaler/*.h \
           $$PWD/fftw/rdft/simd/*.h \
           $$PWD/fftw/reodft/*.h \
           $$PWD/fftw/simd-support/*.h \
           $$PWD/fftw/threads/*.h
SOURCES += $$PWD/fftw/api/*.c \
           $$PWD/fftw/dft/*.c \
           $$PWD/fftw/dft/scaler/*.c \
           $$PWD/fftw/dft/simd/*.c \
           $$PWD/fftw/kernal/*.c \
           $$PWD/fftw/rdft/*.c \
           $$PWD/fftw/rdft/scaler/*.c \
           $$PWD/fftw/rdft/simd/*.c \
           $$PWD/fftw/reodft/*.c \
           $$PWD/fftw/simd-support/*.c \
           $$PWD/fftw/threads/*.c
