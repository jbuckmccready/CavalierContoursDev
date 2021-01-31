QT += core quick

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
  interactiveui/demofuncs.h \
  interactiveui/flatcolorgeometrynode.h \
  interactiveui/geometrycanvasitem.h \
  interactiveui/graphicshelpers.h \
  interactiveui/plinecombinealgorithmview.h \
  interactiveui/plineoffsetalgorithmview.h \
  interactiveui/plineoffsetislandsalgorithmview.h \
  interactiveui/plinesegmentnode.h \
  interactiveui/pointsetnode.h \
  interactiveui/polylinenode.h \
  interactiveui/rawoffsetsegmentsnode.h \
  interactiveui/simplecirclenode.h \
  interactiveui/spatialindexboundingboxesnode.h \


SOURCES += \
  interactiveui/demofuncs.cpp \
  interactiveui/flatcolorgeometrynode.cpp \
  interactiveui/geometrycanvasitem.cpp \
  interactiveui/graphicshelpers.cpp \
  interactiveui/main.cpp \
  interactiveui/plinecombinealgorithmview.cpp \
  interactiveui/plineoffsetalgorithmview.cpp \
  interactiveui/plineoffsetislandsalgorithmview.cpp \
  interactiveui/plinesegmentnode.cpp \
  interactiveui/pointsetnode.cpp \
  interactiveui/polylinenode.cpp \
  interactiveui/rawoffsetsegmentsnode.cpp \
  interactiveui/simplecirclenode.cpp \
  interactiveui/spatialindexboundingboxesnode.cpp \

INCLUDEPATH += CavalierContours/include

RESOURCES += interactiveui/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target