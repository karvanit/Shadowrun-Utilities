TEMPLATE = app
CONFIG(debug, debug|release):TARGET = srutils-debug
else:TARGET = srutils
QT += core \
    gui \
    xml
HEADERS += rollbar.h \
    srutils.h
SOURCES += rollbar.cpp \
    main.cpp \
    srutils.cpp
FORMS += rollbar.ui \
    srutils.ui
RESOURCES += 
FLEXSOURCES +=
BISONSOURCES +=
include(bison.prf)
include(flex.prf)
