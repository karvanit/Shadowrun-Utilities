TEMPLATE = app
CONFIG(debug, debug|release):TARGET = srutils-debug
else:TARGET = srutils
QT += core \
    gui \
    xml
HEADERS += rollinfo.h \
    shadowrun.h \
    rollbar.h \
    srutils.h
SOURCES += shadowrun.cpp \
    rollbar.cpp \
    main.cpp \
    srutils.cpp
FORMS += rollbar.ui \
    srutils.ui
RESOURCES += 
FLEXSOURCES += roller.l
BISONSOURCES += roller.ypp
include(bison.prf)
include(flex.prf)
