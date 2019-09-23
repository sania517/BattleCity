QT += quick
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    battlecityfield.cpp \
    battlefieldobjects.cpp \
    controller.cpp \
    qmldataview.cpp \
    battletimer.cpp \
    qmltankview.cpp \
    qmlbullet0list.cpp \
    qmlbullet1list.cpp \
    qmlbullet2list.cpp \
    qmlbullet3list.cpp \
    numberoftanks.cpp \
    readerstageinfile.cpp \
    hashobjects.cpp \
    object.cpp \
    bonus.cpp


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    object.h \
    battlefieldobjects.h \
    battlecityfield.h \
     object.h \
    factory_of_object.h \
    factoryofitem.h \
    controller.h \
    field.h \
    qmldataview.h \
    battletimer.h \
    qmltankview.h \
    qmlbullet0list.h \
    qmlbullet1list.h \
    qmlbullet2list.h \
    qmlbullet3list.h \
    numberoftanks.h \
    readerstageinfile.h \
    hashobjects.h \
    bonus.h

DISTFILES +=
