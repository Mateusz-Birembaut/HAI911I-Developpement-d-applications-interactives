MOC_DIR = ./moc
OBJECTS_DIR = ./obj

INCLUDEPATH += $$PWD

HEADERS       = glwidget.h \
                FileLoader.h \
                Mesh.h \
                Vec3.h \
                window.h \
                mainwindow.h \
                logo.h
SOURCES       = glwidget.cpp \
                Mesh.cpp \
                main.cpp \
                window.cpp \
                mainwindow.cpp \
                logo.cpp

RESOURCES += \
    shaders.qrc

QT           += widgets opengl openglwidgets


