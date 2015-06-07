#-------------------------------------------------
#
# Project created by QtCreator 2013-11-02T13:30:10
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = prjeditor
TEMPLATE = app

SOURCES += main.cpp\
        cmainwindow.cpp \
    cprojectlistwidget.cpp \
    cprojectwidget.cpp \
    ctoolbarwidget.cpp \
    cbutton.cpp \
    cfoldertreeview.cpp \
    cfoldersortfilter.cpp \
    ceditorwidget.cpp \
    ctextedit.cpp \
    ceditortabwidget.cpp \
    ceditortabbar.cpp \
    ceditortabbarlabelwidget.cpp \
    ctexteditstyle.cpp \
    ctexteditstylelist.cpp \
    csyntaxhighlighter.cpp \
    cabstracttextdocumentlayout.cpp \
    cdialogfind.cpp \
    cdialogfindreplace.cpp \
    cdialogfindinfiles.cpp \
    cfindfromfilestreeview.cpp \
    csidebar.cpp \
    cdialogquerymessage.cpp \
    csidebarupdown.cpp \
    cdialogfindcompobox.cpp \
    crunproject.cpp \
    cdialogrunprgram.cpp \
    cdialoglineeditbase.cpp \
    cdialogabout.cpp \
    ctextedithighlightrule.cpp \
    ctextedithighlightrules.cpp

HEADERS  += cmainwindow.h \
    cprojectlistwidget.h \
    cprojectwidget.h \
    ctoolbarwidget.h \
    application.h \
    cbutton.h \
    cfoldertreeview.h \
    cfoldersortfilter.h \
    ceditorwidget.h \
    ctextedit.h \
    ceditortabwidget.h \
    ceditortabbar.h \
    ceditortabbarlabelwidget.h \
    ctexteditstyle.h \
    ctexteditstylelist.h \
    csyntaxhighlighter.h \
    cabstracttextdocumentlayout.h \
    cdialogfind.h \
    cdialogfindreplace.h \
    cdialogfindinfiles.h \
    cfindfromfilestreeview.h \
    csidebar.h \
    cdialogquerymessage.h \
    csidebarupdown.h \
    cdialogfindcompobox.h \
    isidebarupdown.h \
    crunproject.h \
    cdialogrunprgram.h \
    cdialoglineeditbase.h \
    cdialogabout.h \
    isidebar.h \
    ctextedithighlightrule.h \
    ctextedithighlightrules.h

FORMS    +=

RESOURCES += \
    images.qrc

DEFINES += HIGHLIGHT_FILEPATH="\\\"/usr/share/prj-editor/highlights.xml\\\""

highlights.path = /usr/share/prj-editor/
highlights.files = xml/highlights.xml

desktopfile.path = /usr/share/applications
desktopfile.files = desktopfile/prjeditor.desktop

prjeditordesktopimage.path = /usr/share/pixmaps
prjeditordesktopimage.files = desktopfile/prjeditor.png

theappfile.path = /usr/local/bin
theappfile.files = $$TARGET

INSTALLS += highlights theappfile desktopfile prjeditordesktopimage
