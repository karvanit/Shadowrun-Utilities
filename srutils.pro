TEMPLATE = app
CONFIG(debug, debug|release):TARGET = srutils-debug
else:TARGET = srutils
QT += core \
    gui \
    xml
HEADERS += initiativeinput.h \
    inputtable.h \
    combatactor.h \
    dice.h \
    combatactivity.h \
    mainview.h \
    rollinfo.h \
    rollbar.h \
    srutils.h
SOURCES += initiativeinput.cpp \
    inputtable.cpp \
    combatactivity.cpp \
    mainview.cpp \
    shadowrun.cpp \
    rollbar.cpp \
    main.cpp \
    srutils.cpp
FORMS += initiativeinput.ui \
    mainview.ui \
    rollbar.ui \
    srutils.ui
RESOURCES += 
FLEXSOURCES += roller.l
BISONSOURCES += roller.ypp
include(bison.prf)
include(flex.prf)
