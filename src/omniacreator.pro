# by: Kwabena W. Agyeman

PROJECT_FULL_NAME = "Omnia_Creator"
PROJECT_FULL_NAME_WO_SPACES = "$$replace(PROJECT_FULL_NAME, "_", "")"
PROJECT_SHORT_NAME = "$$lower($${PROJECT_FULL_NAME_WO_SPACES})"
PROJECT_VERSION = "0.1"
PROJECT_VENDOR = "$${PROJECT_FULL_NAME}"
PROJECT_COPYRIGHT = "2015_Kwabena_W._Agyeman"
PROJECT_DOMAIN_NAME = "$${PROJECT_SHORT_NAME}.com"
PROJECT_URL = "http://$${PROJECT_DOMAIN_NAME}/"
PROJECT_URL_WO_SLASH = "http://$${PROJECT_DOMAIN_NAME}"
PROJECT_EMAIL = "$${PROJECT_SHORT_NAME}@$${PROJECT_DOMAIN_NAME}"

###############################################################################
# Make ########################################################################
###############################################################################

TEMPLATE = app
TARGET = $${PROJECT_SHORT_NAME}

QT += bluetooth
QT += concurrent
QT += network
QT += opengl
QT += printsupport
QT += serialport
QT += svg
QT += widgets

RC_ICONS = $${PWD}/media/icons/$${PROJECT_SHORT_NAME}-icon/$${PROJECT_SHORT_NAME}.ico
ICON = $${PWD}/media/icons/$${PROJECT_SHORT_NAME}-icon/$${PROJECT_SHORT_NAME}.icns

DEFINES += PROJECT_FULL_NAME=\"$${PROJECT_FULL_NAME}\"
DEFINES += PROJECT_FULL_NAME_WO_SPACES=\"$${PROJECT_FULL_NAME_WO_SPACES}\"
DEFINES += PROJECT_SHORT_NAME=\"$${PROJECT_SHORT_NAME}\"
DEFINES += PROJECT_VERSION=\"$${PROJECT_VERSION}\"
DEFINES += PROJECT_VENDOR=\"$${PROJECT_VENDOR}\"
DEFINES += PROJECT_COPYRIGHT=\"$${PROJECT_COPYRIGHT}\"
DEFINES += PROJECT_DOMAIN_NAME=\"$${PROJECT_DOMAIN_NAME}\"
DEFINES += PROJECT_URL=\"$${PROJECT_URL}\"
DEFINES += PROJECT_URL_WO_SLASH=\"$${PROJECT_URL_WO_SLASH}\"
DEFINES += PROJECT_EMAIL=\"$${PROJECT_EMAIL}\"

# include: OmniaCreator

OC_TARGET = $${OUT_PWD}/$${PROJECT_SHORT_NAME}.qrc
OC_COMMANDS = echo \
^<RCC^>\
    ^<qresource prefix=\"/\"^>\
        ^<file alias=\"$${PROJECT_SHORT_NAME}.png\"^>$${PWD}/media/icons/$${PROJECT_SHORT_NAME}-icon/$${PROJECT_SHORT_NAME}.png^</file^>\
        ^<file alias=\"splash-small.png\"^>$${PWD}/media/splash/$${PROJECT_SHORT_NAME}-splash/splash-small.png^</file^>\
        ^<file alias=\"InterfaceLibrary.h\"^>$${OUT_PWD}/InterfaceLibrary.h^</file^>\
        ^<file alias=\"ilfullduplexserialdriver.spin\"^>$${PWD}/interfacelibrary/ilfullduplexserialdriver.spin^</file^>\
        ^<file alias=\"ilcore.spin\"^>$${PWD}/interfacelibrary/ilcore.spin^</file^>\
        ^<file alias=\"iloscilloscope.spin\"^>$${PWD}/interfacelibrary/iloscilloscope.spin^</file^>\
        ^<file alias=\"iloscilloscopebar.spin\"^>$${PWD}/interfacelibrary/iloscilloscopebar.spin^</file^>\
        ^<file alias=\"iloscilloscopebox.spin\"^>$${PWD}/interfacelibrary/iloscilloscopebox.spin^</file^>\
        ^<file alias=\"iloscilloscopecurve.spin\"^>$${PWD}/interfacelibrary/iloscilloscopecurve.spin^</file^>\
        ^<file alias=\"iloscilloscopegraph.spin\"^>$${PWD}/interfacelibrary/iloscilloscopegraph.spin^</file^>\
        ^<file alias=\"ilterminal.spin\"^>$${PWD}/interfacelibrary/ilterminal.spin^</file^>\
        ^<file alias=\"Serial Core Demo.spin\"^>$${PWD}/interfacelibrary/examples/Serial Core Demo.spin^</file^>\
        ^<file alias=\"Plot Bar Demo.spin\"^>$${PWD}/interfacelibrary/examples/Plot Bar Demo.spin^</file^>\
        ^<file alias=\"Plot Box Demo.spin\"^>$${PWD}/interfacelibrary/examples/Plot Box Demo.spin^</file^>\
        ^<file alias=\"Plot Curve Demo.spin\"^>$${PWD}/interfacelibrary/examples/Plot Curve Demo.spin^</file^>\
        ^<file alias=\"Plot Graph Demo.spin\"^>$${PWD}/interfacelibrary/examples/Plot Graph Demo.spin^</file^>\
        ^<file alias=\"Serial Terminal Demo.spin\"^>$${PWD}/interfacelibrary/examples/Serial Terminal Demo.spin^</file^>\
    ^</qresource^>\
^</RCC^>> $${OC_TARGET}

oc.commands = $${OC_COMMANDS}
oc.depends = $${PWD}/$${PROJECT_SHORT_NAME}.pro
oc.target = $${OC_TARGET}
QMAKE_EXTRA_TARGETS += oc
PRE_TARGETDEPS += $${OC_TARGET}

!exists($${OC_TARGET}): system($${OC_COMMANDS})

HEADERS += $${PWD}/$${PROJECT_SHORT_NAME}.h
HEADERS += $${PWD}/main.h
HEADERS += $${PWD}/serialescape.h
HEADERS += $${PWD}/serialwindow.h
HEADERS += $${PWD}/serialgraphics.h
HEADERS += $${PWD}/serialinterface.h
HEADERS += $${PWD}/serialoscilloscope.h
HEADERS += $${PWD}/serialtable.h
HEADERS += $${PWD}/serialterminal.h
HEADERS += $${PWD}/serialtree.h
HEADERS += $${PWD}/utilrasterdialog.h
HEADERS += $${PWD}/utilvectordialog.h
HEADERS += $${PWD}/utilvectordialogwowh.h
SOURCES += $${PWD}/main.cpp
SOURCES += $${PWD}/serialescape.cpp
SOURCES += $${PWD}/serialwindow.cpp
SOURCES += $${PWD}/serialgraphics.cpp
SOURCES += $${PWD}/serialinterface.cpp
SOURCES += $${PWD}/serialoscilloscope.cpp
SOURCES += $${PWD}/serialtable.cpp
SOURCES += $${PWD}/serialterminal.cpp
SOURCES += $${PWD}/serialtree.cpp
SOURCES += $${PWD}/utilrasterdialog.cpp
SOURCES += $${PWD}/utilvectordialog.cpp
SOURCES += $${PWD}/utilvectordialogwowh.cpp
FORMS += $${PWD}/serialgraphics.ui
FORMS += $${PWD}/serialinterface.ui
FORMS += $${PWD}/serialoscilloscope.ui
FORMS += $${PWD}/serialtable.ui
FORMS += $${PWD}/serialterminal.ui
FORMS += $${PWD}/serialtree.ui
FORMS += $${PWD}/utilrasterdialog.ui
FORMS += $${PWD}/utilvectordialog.ui
FORMS += $${PWD}/utilvectordialogwowh.ui
RESOURCES += $${OC_TARGET}

# include: InterfaceLibrary

INCLUDEPATH += $${PWD}/interfacelibrary
DEPENDPATH += $${PWD}/interfacelibrary
IL_DEPENDS += $${PWD}/interfacelibrary/il.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilshared.h
IL_DEPENDS += $${PWD}/interfacelibrary/ildependencies.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilcore.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilterminal.h
IL_DEPENDS += $${PWD}/interfacelibrary/iloscilloscope.h
IL_DEPENDS += $${PWD}/interfacelibrary/iltable.h
IL_DEPENDS += $${PWD}/interfacelibrary/iltree.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilgraphics.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilinterface.h
IL_DEPENDS += $${PWD}/interfacelibrary/iloscilloscopebar.h
IL_DEPENDS += $${PWD}/interfacelibrary/iloscilloscopecurve.h
IL_DEPENDS += $${PWD}/interfacelibrary/iloscilloscopegraph.h
IL_DEPENDS += $${PWD}/interfacelibrary/iloscilloscopebox.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilinterfacegroupbox.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilinterfacepushbutton.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilinterfaceradiobutton.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilinterfacecheckbox.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilinterfacelineedit.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilinterfaceintspinbox.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilinterfacebinspinbox.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilinterfacehexspinbox.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilinterfacedoublespinbox.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilinterfacetimeedit.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilinterfacedateedit.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilinterfacedatetimeedit.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilinterfaceslider.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilinterfacelabel.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilinterfaceprogressbar.h
IL_DEPENDS += $${PWD}/interfacelibrary/ilinterfaceseperator.h
HEADERS += $${IL_DEPENDS}

IL_TARGET = $${OUT_PWD}/InterfaceLibrary.h
IL_COMMANDS = python -u $${PWD}/interfacelibrary/preprocess.py \
$${PWD}/interfacelibrary/il.h $${IL_TARGET}

il.commands = $${IL_COMMANDS}
il.depends = $${IL_DEPENDS}
il.target = $${IL_TARGET}
QMAKE_EXTRA_TARGETS += il
PRE_TARGETDEPS += $${IL_TARGET}

!exists($${IL_TARGET}): system($${IL_COMMANDS})

# include: QCustomPlot

DEPENDPATH += $${PWD}/qcustomplot
INCLUDEPATH += $${PWD}/qcustomplot
HEADERS += $${PWD}/qcustomplot/qcustomplot.h
SOURCES += $${PWD}/qcustomplot/qcustomplot.cpp

# include: FFTW3

win32: DEPENDPATH += $${PWD}/fftw3/windows/32
win64: DEPENDPATH += $${PWD}/fftw3/windows/64
win32: INCLUDEPATH += $${PWD}/fftw3/windows/32
win64: INCLUDEPATH += $${PWD}/fftw3/windows/64
win32: HEADERS += $${PWD}/fftw3/windows/32/fftw3.h
win64: HEADERS += $${PWD}/fftw3/windows/64/fftw3.h
win32: LIBS += -L$${PWD}/fftw3/windows/32 -llibfftw3-3
win64: LIBS += -L$${PWD}/fftw3/windows/64 -llibfftw3-3

###############################################################################
# Make Install ################################################################
###############################################################################

DOMAIN_NAMES = $$split(PROJECT_DOMAIN_NAME, ".")
REV_PROJECT_DOMAIN_NAME = $$member(DOMAIN_NAMES, 1).$$member(DOMAIN_NAMES, 0)
INSTALLS_DIR = $${OUT_PWD}/packages/$${REV_PROJECT_DOMAIN_NAME}/data

target.path = $${INSTALLS_DIR}
INSTALLS += target

win32|win64:
{
    windeploy.path = $${INSTALLS_DIR}
    windeploy.extra = windeployqt $${INSTALLS_DIR}
    INSTALLS += windeploy
}

fftw.path = $${INSTALLS_DIR}
win32: fftw.files = $${PWD}/fftw3/windows/32/libfftw3-3.dll
win64: fftw.files = $${PWD}/fftw3/windows/64/libfftw3-3.dll
INSTALLS += fftw

# release step 1:

sign.path = $${INSTALLS_DIR}
sign.extra = python -u $${PWD}/sign.py $${INSTALLS_DIR}
# INSTALLS += sign

# release step 2:

ifw_config.path = $${OUT_PWD}/config
ifw_config.extra = echo \
^<?xml version=\"1.0\" encoding=\"UTF-8\"?^>\
^<Installer^>\
    ^<Name^>$$replace(PROJECT_FULL_NAME, "_", " ")^</Name^>\
    ^<Version^>$${PROJECT_VERSION}^</Version^>\
    ^<Title^>$$replace(PROJECT_FULL_NAME, "_", " ")^</Title^>\
    ^<Publisher^>$$replace(PROJECT_VENDOR, "_", " ")^</Publisher^>\
    ^<ProductUrl^>$${PROJECT_URL}^</ProductUrl^>\
    ^<InstallerApplicationIcon^>$${PWD}/media/icons/$${PROJECT_SHORT_NAME}-icon/$${PROJECT_SHORT_NAME}^</InstallerApplicationIcon^>\
    ^<InstallerWindowIcon^>$${PWD}/src/media/icons/$${PROJECT_SHORT_NAME}-icon/$${PROJECT_SHORT_NAME}.png^</InstallerWindowIcon^>\
    ^<RunProgram^>@TargetDir@/$${PROJECT_SHORT_NAME}^</RunProgram^>\
    ^<StartMenuDir^>$$replace(PROJECT_FULL_NAME, "_", " ")^</StartMenuDir^>\
    ^<TargetDir^>@ApplicationsDir@/$$replace(PROJECT_FULL_NAME, "_", " ")^</TargetDir^>\
^</Installer^>> $${OUT_PWD}/config/config.xml
INSTALLS += ifw_config

ifw_package.path = $${OUT_PWD}/packages/$${REV_PROJECT_DOMAIN_NAME}/meta
ifw_package.extra = echo \
^<?xml version=\"1.0\" encoding=\"UTF-8\"?^>\
^<Package^>\
    ^<DisplayName^>$$replace(PROJECT_FULL_NAME, "_", " ")^</DisplayName^>\
    ^<Description^>^</Description^>\
    ^<Version^>$${PROJECT_VERSION}^</Version^>\
    ^<ReleaseDate^>^</ReleaseDate^>\
    ^<Name^>$${REV_PROJECT_DOMAIN_NAME}^</Name^>\
    ^<Licenses^>\
        ^<License name=\"End User License Agreement\" file=\"LICENSE\" /^>\
    ^</Licenses^>\
    ^<Script^>script.qs^</Script^>\
    ^<Default^>true^</Default^>\
    ^<ForcedInstallation^>true^</ForcedInstallation^>\
    ^<RequiresAdminRights^>true^</RequiresAdminRights^>\
^</Package^>> $${OUT_PWD}/packages/$${REV_PROJECT_DOMAIN_NAME}/meta/package.xml
INSTALLS += ifw_package

ifw_license.path = $${OUT_PWD}/packages/$${REV_PROJECT_DOMAIN_NAME}/meta
ifw_license.files = $${PWD}/../LICENSE
INSTALLS += ifw_license

ifw_script.path = $${OUT_PWD}/packages/$${REV_PROJECT_DOMAIN_NAME}/meta
ifw_script.extra = echo \
function Component()\
{\
}> $${OUT_PWD}/packages/$${REV_PROJECT_DOMAIN_NAME}/meta/script.qs
INSTALLS += ifw_script

# release step 3:

win32|win64: { INSTALLER_NAME = $${OUT_PWD}/$${PROJECT_SHORT_NAME}.exe
} else:macx: { INSTALLER_NAME = $${OUT_PWD}/$${PROJECT_SHORT_NAME}.app
} else: { INSTALLER_NAME = $${OUT_PWD}/$${PROJECT_SHORT_NAME}.run }

ifw_binarycreator.path = $${OUT_PWD}
ifw_binarycreator.extra = binarycreator --offline-only --verbose \
-c $${OUT_PWD}/config/config.xml -p $${OUT_PWD}/packages $${INSTALLER_NAME}
INSTALLS += ifw_binarycreator

# release step 4:

ifw_sign.path = $${OUT_PWD}
ifw_sign.extra = python -u $${PWD}/sign.py $${INSTALLER_NAME}
INSTALLS += ifw_sign
