QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    SimpleTest.cpp \
    ../../FPSTester/FPSTesterThread.cpp \
    ../../FPSTester/ImgGetterThread.cpp \
    ../../FPSTester/FPSTester.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    SimpleTest.h \
    ../../FPSTester/FPSTesterThread.h \
    ../../FPSTester/ImgGetterThread.h \
    ../../FPSTester/BlockQueue.h \
    ../../FPSTester/FPSTester.h \

INCLUDEPATH += ../../FPSTester

include(depends.pri)

win32:CONFIG(release, debug|release): LIBS += -L$${OpenCV_LIB_Path} -lopencv_world346
else:win32:CONFIG(debug, debug|release): LIBS += -L$${OpenCV_LIB_Path} -lopencv_world346d

INCLUDEPATH += $$OpenCV_INCLUDE_path
DEPENDPATH += $$OpenCV_INCLUDE_path
