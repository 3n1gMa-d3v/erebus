TEMPLATE = app
TARGET = spectre
#find . -type d
QT += testlib webenginewidgets webchannel

TEMPLATE = subdirs

SUBDIRS = \
    tor \   # sub-project names
    leveldb \   # sub-project names
    db4.8 \   # sub-project names
    src \   # sub-project names

src.depends = tor leveldb db4.8

#  CONFIG += ordered
