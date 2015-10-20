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
QT += bluetooth
QT += concurrent
QT += network
QT += opengl
QT += printsupport
QT += serialport
QT += svg
QT += widgets
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
HEADERS += $$PWD/serialescape.h
HEADERS += $$PWD/serialwindow.h
HEADERS += $$PWD/serialgraphics.h
HEADERS += $$PWD/serialinterface.h
HEADERS += $$PWD/serialoscilloscope.h
HEADERS += $$PWD/serialtable.h
HEADERS += $$PWD/serialterminal.h
HEADERS += $$PWD/serialtree.h
HEADERS += $$PWD/utilrasterdialog.h
HEADERS += $$PWD/utilvectordialog.h
HEADERS += $$PWD/utilvectordialogwowh.h
SOURCES += $$PWD/main.cpp
SOURCES += $$PWD/serialescape.cpp
SOURCES += $$PWD/serialwindow.cpp
SOURCES += $$PWD/serialgraphics.cpp
SOURCES += $$PWD/serialinterface.cpp
SOURCES += $$PWD/serialoscilloscope.cpp
SOURCES += $$PWD/serialtable.cpp
SOURCES += $$PWD/serialterminal.cpp
SOURCES += $$PWD/serialtree.cpp
SOURCES += $$PWD/utilrasterdialog.cpp
SOURCES += $$PWD/utilvectordialog.cpp
SOURCES += $$PWD/utilvectordialogwowh.cpp
FORMS += $$PWD/serialgraphics.ui
FORMS += $$PWD/serialinterface.ui
FORMS += $$PWD/serialoscilloscope.ui
FORMS += $$PWD/serialtable.ui
FORMS += $$PWD/serialterminal.ui
FORMS += $$PWD/serialtree.ui
FORMS += $$PWD/utilrasterdialog.ui
FORMS += $$PWD/utilvectordialog.ui
FORMS += $$PWD/utilvectordialogwowh.ui
RESOURCES += $$PWD/omniacreator.qrc

# include: InterfaceLibrary

INCLUDEPATH += $$PWD/interfacelibrary
DEPENDPATH += $$PWD/interfacelibrary
IL_DEPENDS += $$PWD/interfacelibrary/il.h
IL_DEPENDS += $$PWD/interfacelibrary/ilshared.h
IL_DEPENDS += $$PWD/interfacelibrary/ildependencies.h
IL_DEPENDS += $$PWD/interfacelibrary/ilcore.h
IL_DEPENDS += $$PWD/interfacelibrary/ilterminal.h
IL_DEPENDS += $$PWD/interfacelibrary/iloscilloscope.h
IL_DEPENDS += $$PWD/interfacelibrary/iltable.h
IL_DEPENDS += $$PWD/interfacelibrary/iltree.h
IL_DEPENDS += $$PWD/interfacelibrary/ilgraphics.h
IL_DEPENDS += $$PWD/interfacelibrary/ilinterface.h
IL_DEPENDS += $$PWD/interfacelibrary/iloscilloscopebar.h
IL_DEPENDS += $$PWD/interfacelibrary/iloscilloscopecurve.h
IL_DEPENDS += $$PWD/interfacelibrary/iloscilloscopegraph.h
IL_DEPENDS += $$PWD/interfacelibrary/iloscilloscopebox.h
IL_DEPENDS += $$PWD/interfacelibrary/ilinterfacegroupbox.h
IL_DEPENDS += $$PWD/interfacelibrary/ilinterfacepushbutton.h
IL_DEPENDS += $$PWD/interfacelibrary/ilinterfaceradiobutton.h
IL_DEPENDS += $$PWD/interfacelibrary/ilinterfacecheckbox.h
IL_DEPENDS += $$PWD/interfacelibrary/ilinterfacelineedit.h
IL_DEPENDS += $$PWD/interfacelibrary/ilinterfaceintspinbox.h
IL_DEPENDS += $$PWD/interfacelibrary/ilinterfacebinspinbox.h
IL_DEPENDS += $$PWD/interfacelibrary/ilinterfacehexspinbox.h
IL_DEPENDS += $$PWD/interfacelibrary/ilinterfacedoublespinbox.h
IL_DEPENDS += $$PWD/interfacelibrary/ilinterfacetimeedit.h
IL_DEPENDS += $$PWD/interfacelibrary/ilinterfacedateedit.h
IL_DEPENDS += $$PWD/interfacelibrary/ilinterfacedatetimeedit.h
IL_DEPENDS += $$PWD/interfacelibrary/ilinterfaceslider.h
IL_DEPENDS += $$PWD/interfacelibrary/ilinterfacelabel.h
IL_DEPENDS += $$PWD/interfacelibrary/ilinterfaceprogressbar.h
IL_DEPENDS += $$PWD/interfacelibrary/ilinterfaceseperator.h
HEADERS += $$IL_DEPENDS

IL_TARGET = $$OUT_PWD/InterfaceLibrary.h
IL_COMMANDS = python $$PWD/interfacelibrary/preprocess.py \
$$PWD/interfacelibrary/il.h \
$$IL_TARGET

!exists($$IL_TARGET):system($$IL_COMMANDS)
il.commands = $$IL_COMMANDS
il.depends = $$IL_DEPENDS
il.target = $$IL_TARGET
QMAKE_EXTRA_TARGETS += il
PRE_TARGETDEPS += $$IL_TARGET

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
