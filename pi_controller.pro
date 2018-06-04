QT += charts
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    chart.cpp \
    rootwidget.cpp \
    sequence.cpp

QML_IMPORT_PATH =

QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    chart.h \
    rootwidget.h \
    sequence.h \
    pindefs.h \
    nanocoater.h

unix:!macx: LIBS += -lpigpio
