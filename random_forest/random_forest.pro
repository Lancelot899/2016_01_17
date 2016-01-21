TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11


SOURCES += main.cpp \
    desion_tree.cpp \
    random_forest.cpp

HEADERS += \
    desion_tree.h \
    random_forest.h \
    thread_pool.h

