QT       += core gui widgets printsupport webenginewidgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    PackedLib/polygoninputerwidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    PackedLib/PackedList.h \
    PackedLib/PackedObject.h \
    PackedLib/polygoninputerwidget.h \
    PackedListItem.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    assets/help/e06722e6-8cc7-11ea-8b25-0cc47a792c0a_id_e06722e6-8cc7-11ea-8b25-0cc47a792c0a_files/background01.jpg \
    assets/help/e06722e6-8cc7-11ea-8b25-0cc47a792c0a_id_e06722e6-8cc7-11ea-8b25-0cc47a792c0a_files/background02.jpg \
    assets/help/e06722e6-8cc7-11ea-8b25-0cc47a792c0a_id_e06722e6-8cc7-11ea-8b25-0cc47a792c0a_files/background03.jpg \
    assets/help/e06722e6-8cc7-11ea-8b25-0cc47a792c0a_id_e06722e6-8cc7-11ea-8b25-0cc47a792c0a_files/background04.jpg \
    assets/help/e06722e6-8cc7-11ea-8b25-0cc47a792c0a_id_e06722e6-8cc7-11ea-8b25-0cc47a792c0a_files/background05.jpg \
    assets/help/e06722e6-8cc7-11ea-8b25-0cc47a792c0a_id_e06722e6-8cc7-11ea-8b25-0cc47a792c0a_files/background06.jpg \
    assets/help/e06722e6-8cc7-11ea-8b25-0cc47a792c0a_id_e06722e6-8cc7-11ea-8b25-0cc47a792c0a_files/background07.jpg \
    assets/help/e06722e6-8cc7-11ea-8b25-0cc47a792c0a_id_e06722e6-8cc7-11ea-8b25-0cc47a792c0a_files/background08.jpg \
    assets/help/e06722e6-8cc7-11ea-8b25-0cc47a792c0a_id_e06722e6-8cc7-11ea-8b25-0cc47a792c0a_files/background09.jpg \
    assets/help/e06722e6-8cc7-11ea-8b25-0cc47a792c0a_id_e06722e6-8cc7-11ea-8b25-0cc47a792c0a_files/background10.jpg \
    assets/help/e06722e6-8cc7-11ea-8b25-0cc47a792c0a_id_e06722e6-8cc7-11ea-8b25-0cc47a792c0a_files/background11.jpg \
    assets/help/e06722e6-8cc7-11ea-8b25-0cc47a792c0a_id_e06722e6-8cc7-11ea-8b25-0cc47a792c0a_files/background12.jpg \
    assets/help/help.html \
    assets/openfile.png

RESOURCES += \
    assets.qrc

RC_ICONS = $$PWD/assets/cutter.ico
