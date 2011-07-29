TEMPLATE = app
CONFIG(debug, debug|release):TARGET = srutils-debug
else:TARGET = srutils
QT += core \
    gui \
    xml
HEADERS += srutils.h
SOURCES += main.cpp \
    srutils.cpp
FORMS += srutils.ui
RESOURCES += 
