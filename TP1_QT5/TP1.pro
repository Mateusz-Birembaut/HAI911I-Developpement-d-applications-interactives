MOC_DIR = ./moc
OBJECTS_DIR = ./obj

INCLUDEPATH += $$PWD

HEADERS       = glwidget.h \
                Mesh.h \
                window.h \
                mainwindow.h \
                logo.h \
                FileLoader.h
SOURCES       = glwidget.cpp \
                Mesh.cpp \
                main.cpp \
                window.cpp \
                mainwindow.cpp \
                logo.cpp

RESOURCES += \
    shaders.qrc

QT           += widgets


