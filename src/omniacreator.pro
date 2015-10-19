# by: Kwabena W. Agyeman

PROJECT_FULL_NAME = "Omnia_Creator"
PROJECT_FULL_NAME_WO_SPACES = "$$replace(PROJECT_FULL_NAME, "_", "")"
PROJECT_SHORT_NAME = "$$lower($$PROJECT_FULL_NAME_WO_SPACES)"
PROJECT_VERSION = "0.1"
PROJECT_VENDOR = "$$PROJECT_FULL_NAME"
PROJECT_COPYRIGHT = "2015_Kwabena_W._Agyeman"
PROJECT_DOMAIN_NAME = "$${PROJECT_SHORT_NAME}.com"
PROJECT_URL = "http://$${PROJECT_DOMAIN_NAME}/"
PROJECT_URL_WO_SLASH = "http://$${PROJECT_DOMAIN_NAME}"
PROJECT_EMAIL = "$${PROJECT_SHORT_NAME}@$${PROJECT_DOMAIN_NAME}"

# setup: Project

TEMPLATE = app
TARGET = omniacreator
RC_ICONS = media/icons/omniacreator-icon/omniacreator.ico
ICON = media/icons/omniacreator-icon/omniacreator.icns
QT += concurrent
QT += opengl
QT += printsupport
QT += serialport
QT += svg
QT += widgets
QT += network
DEFINES += PROJECT_FULL_NAME=\"$$PROJECT_FULL_NAME\"
DEFINES += PROJECT_FULL_NAME_WO_SPACES=\"$$PROJECT_FULL_NAME_WO_SPACES\"
DEFINES += PROJECT_SHORT_NAME=\"$$PROJECT_SHORT_NAME\"
DEFINES += PROJECT_VERSION=\"$$PROJECT_VERSION\"
DEFINES += PROJECT_VENDOR=\"$$PROJECT_VENDOR\"
DEFINES += PROJECT_COPYRIGHT=\"$$PROJECT_COPYRIGHT\"
DEFINES += PROJECT_DOMAIN_NAME=\"$$PROJECT_DOMAIN_NAME\"
DEFINES += PROJECT_URL=\"$$PROJECT_URL\"
DEFINES += PROJECT_URL_WO_SLASH=\"$$PROJECT_URL_WO_SLASH\"
DEFINES += PROJECT_EMAIL=\"$$PROJECT_EMAIL\"
DEFINES += QT_USE_FAST_CONCATENATION
DEFINES += QT_USE_FAST_OPERATOR_PLUS
DEFINES += QT_USE_QSTRINGBUILDER

# include: OmniaCreator

HEADERS += $$PWD/main.h
#HEADERS += $$PWD/serialescape.h
#SOURCES += $$PWD/serialwindow.h
#HEADERS += $$PWD/serialgraphics.h
#HEADERS += $$PWD/serialinterface.h
#HEADERS += $$PWD/serialoscilloscope.h
#HEADERS += $$PWD/serialtable.h
#HEADERS += $$PWD/serialterminal.h
#HEADERS += $$PWD/serialtree.h
HEADERS += $$PWD/utilrasterdialog.h
HEADERS += $$PWD/utilvectordialog.h
HEADERS += $$PWD/utilvectordialogwowh.h
SOURCES += $$PWD/main.cpp
#SOURCES += $$PWD/serialescape.cpp
#SOURCES += $$PWD/serialwindow.cpp
#SOURCES += $$PWD/serialgraphics.cpp
#SOURCES += $$PWD/serialinterface.cpp
#SOURCES += $$PWD/serialoscilloscope.cpp
#SOURCES += $$PWD/serialtable.cpp
#SOURCES += $$PWD/serialterminal.cpp
#SOURCES += $$PWD/serialtree.cpp
SOURCES += $$PWD/utilrasterdialog.cpp
SOURCES += $$PWD/utilvectordialog.cpp
SOURCES += $$PWD/utilvectordialogwowh.cpp
#FORMS += $$PWD/serialgraphics.ui
#FORMS += $$PWD/serialinterface.ui
#FORMS += $$PWD/serialoscilloscope.ui
#FORMS += $$PWD/serialtable.ui
#FORMS += $$PWD/serialterminal.ui
#FORMS += $$PWD/serialtree.ui
FORMS += $$PWD/utilrasterdialog.ui
FORMS += $$PWD/utilvectordialog.ui
FORMS += $$PWD/utilvectordialogwowh.ui
RESOURCES += $$PWD/omniacreator.qrc

# include: InterfaceLibrary

!exists($$OUT_PWD/InterfaceLibrary.h) {
system(python $$PWD/interfacelibrary/preprocess.py \
$$PWD/interfacelibrary/il.h \
$$OUT_PWD/InterfaceLibrary.h)
}

il.target = $$OUT_PWD/InterfaceLibrary.h
il.commands = python $$PWD/interfacelibrary/preprocess.py \
$$PWD/interfacelibrary/il.h \
$$OUT_PWD/InterfaceLibrary.h
il.depends = $$PWD/interfacelibrary/*.h

QMAKE_EXTRA_TARGETS += il
PRE_TARGETDEPS += $$OUT_PWD/InterfaceLibrary.h

INCLUDEPATH += $$PWD/interfacelibrary
DEPENDPATH += $$PWD/interfacelibrary
HEADERS += $$PWD/interfacelibrary/il.h
HEADERS += $$PWD/interfacelibrary/ilshared.h
HEADERS += $$PWD/interfacelibrary/ildependencies.h
HEADERS += $$PWD/interfacelibrary/ilcore.h
HEADERS += $$PWD/interfacelibrary/ilterminal.h
HEADERS += $$PWD/interfacelibrary/iloscilloscope.h
HEADERS += $$PWD/interfacelibrary/iltable.h
HEADERS += $$PWD/interfacelibrary/iltree.h
HEADERS += $$PWD/interfacelibrary/ilgraphics.h
HEADERS += $$PWD/interfacelibrary/ilinterface.h
HEADERS += $$PWD/interfacelibrary/iloscilloscopebar.h
HEADERS += $$PWD/interfacelibrary/iloscilloscopecurve.h
HEADERS += $$PWD/interfacelibrary/iloscilloscopegraph.h
HEADERS += $$PWD/interfacelibrary/iloscilloscopebox.h
HEADERS += $$PWD/interfacelibrary/ilinterfacegroupbox.h
HEADERS += $$PWD/interfacelibrary/ilinterfacepushbutton.h
HEADERS += $$PWD/interfacelibrary/ilinterfaceradiobutton.h
HEADERS += $$PWD/interfacelibrary/ilinterfacecheckbox.h
HEADERS += $$PWD/interfacelibrary/ilinterfacelineedit.h
HEADERS += $$PWD/interfacelibrary/ilinterfaceintspinbox.h
HEADERS += $$PWD/interfacelibrary/ilinterfacebinspinbox.h
HEADERS += $$PWD/interfacelibrary/ilinterfacehexspinbox.h
HEADERS += $$PWD/interfacelibrary/ilinterfacedoublespinbox.h
HEADERS += $$PWD/interfacelibrary/ilinterfacetimeedit.h
HEADERS += $$PWD/interfacelibrary/ilinterfacedateedit.h
HEADERS += $$PWD/interfacelibrary/ilinterfacedatetimeedit.h
HEADERS += $$PWD/interfacelibrary/ilinterfaceslider.h
HEADERS += $$PWD/interfacelibrary/ilinterfacelabel.h
HEADERS += $$PWD/interfacelibrary/ilinterfaceprogressbar.h
HEADERS += $$PWD/interfacelibrary/ilinterfaceseperator.h

# include: QCustomPlot

DEPENDPATH += $$PWD/qcustomplot
INCLUDEPATH += $$PWD/qcustomplot
HEADERS += $$PWD/qcustomplot/qcustomplot.h
SOURCES += $$PWD/qcustomplot/qcustomplot.cpp

# include: FFTW3

win32: DEPENDPATH += $$PWD/fftw3/windows/32
win64: DEPENDPATH += $$PWD/fftw3/windows/64
win32: INCLUDEPATH += $$PWD/fftw3/windows/32
win64: INCLUDEPATH += $$PWD/fftw3/windows/64
win32: HEADERS += $$PWD/fftw3/windows/32/fftw3.h
win64: HEADERS += $$PWD/fftw3/windows/64/fftw3.h
win32: LIBS += -L$$PWD/fftw3/windows/32 -llibfftw3-3
win64: LIBS += -L$$PWD/fftw3/windows/64 -llibfftw3-3
win32: OTHER_FILES += $$PWD/fftw3/windows/32/libfftw3-3.def
win64: OTHER_FILES += $$PWD/fftw3/windows/64/libfftw3-3.def
win32: OTHER_FILES += $$PWD/fftw3/windows/32/libfftw3-3.dll
win64: OTHER_FILES += $$PWD/fftw3/windows/64/libfftw3-3.dll
