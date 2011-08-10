TEMPLATE = app
CONFIG(debug, debug|release):TARGET = srutils-debug
else:TARGET = srutils
QT += core \
    gui \
    xml
HEADERS += combatactivity.h \
    mainview.h \
    rollinfo.h \
    shadowrun.h \
    rollbar.h \
    srutils.h
SOURCES += combatactivity.cpp \
    mainview.cpp \
    shadowrun.cpp \
    rollbar.cpp \
    main.cpp \
    srutils.cpp
FORMS += mainview.ui \
    rollbar.ui \
    srutils.ui
RESOURCES += 
FLEXSOURCES += roller.l
BISONSOURCES += roller.ypp
include(bison.prf)
include(flex.prf)
