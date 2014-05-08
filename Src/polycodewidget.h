#ifndef POLYCODEWIDGET_H
#define POLYCODEWIDGET_H

//Polycode includes.
#include "PolycodeView.h"
#include "PolyCoreServices.h"
#include "PolyCoreInput.h"
#include "PolyMaterialManager.h"
#include "PolyThreaded.h"
#include "PolyGLRenderer.h"
#include "PolyGLSLShaderModule.h"
#include "PolyRectangle.h"

//Qt includes.
#include <QGLWidget>
#include <QMutex>
#include <QElapsedTimer>
#include <QMouseEvent>
#include <QKeyEvent>


//The widget is in the Polycode namespace.
namespace Polycode {
class _PolyExport QtCoreMutex : public CoreMutex {
public:
    QMutex *pMutex;
};

//! The widget uses pure Qt and Polycode code for all the functionality which means it's cross platform. The widget has a constructor like other polycode cores (SDL - Win32) and can be used the same way. You will ofcourse have to add the widget to an existing window or call show method to display it.
class PolycodeWidget : public QGLWidget, public Core
{
    Q_OBJECT
public:
    //! Constructor like other cores.
    explicit PolycodeWidget(PolycodeView *view, int xRes, int yRes, bool fullScreen, bool vSync, int aaLevel, int anisotropyLevel, int frameRate, int monitorIndex=-1, bool retinaSupport=false);
    //! Sets the video mode.
    void setVideoMode(int xRes, int yRes, bool fullScreen, bool vSync, int aaLevel, int anisotropyLevel);
    //! Create a mutex.
    CoreMutex *createMutex();
    //! Lock a certain mutex.
    void lockMutex(CoreMutex *mutex);
    //! Unlock a certain mutex.
    void unlockMutex(CoreMutex *mutex);
    //! Copy to the clipboard.
    void copyStringToClipboard(const String& str);
    //! Read from clipboard.
    String getClipboardString();

    //! Enable/Disable and show/hide the mouse.
    void enableMouse(bool newval);
    //! Grab the mouse.
    void captureMouse(bool newval);
    //! Set the mouse cursor shape.
    void setCursor(int cursorType);

    //! Create a new thread.
    void createThread(Threaded *target);

    //! Create a new folder.
    void createFolder(const String& folderPath);
    //! Copy file.
    void copyDiskItem(const String& itemPath, const String& destItemPath);
    //! Move a file.
    void moveDiskItem(const String& itemPath, const String& destItemPath);
    //! Removes a file.
    void removeDiskItem(const String& itemPath);
    //! Opens the native folder picker.
    String openFolderPicker();
    //! Opens file picker.
    std::vector<String> openFilePicker(std::vector<CoreFileExtension> extensions, bool allowMultiple);

    //! Uses QProcess instead of platform specific methods.
    String executeExternalCommand(String command, String args, String inDirectory="");
    //! Opens a url. This is also platform indpendent. It's implemented using Qt code.
    void openURL(String url);


    //! Resize the view.
    void resizeTo(int xRes, int yRes);
    //! Not Implemented Yet.
    void warpCursor(int x, int y);
    //! Gets the ticks in ms. Also platform independent.
    unsigned int getTicks();

    //! Update the scene.
    bool Update();
    //! Render the scene.
    void Render();
    std::vector<Rectangle> getVideoModes();
protected:
    //! Intialize opengl. Needed by Qt.
    void initializeGL();
    //! Paint on the widget.
    void paintGL();
    //! Handle resizing.
    void resizeGL(int width, int height);
    //! Handle resizing.
    void resize(int w, int h);
    //! Propagates mouse press events to polycode so they can be handled by polycode event handlers.
    void mousePressEvent(QMouseEvent *event);
    //! Propagates mouse movement events to polycode so they can be handled by polycode event handlers.
    void mouseMoveEvent(QMouseEvent *event);
    //! Propagates mouse release events to polycode so they can be handled by polycode event handlers.
    void mouseReleaseEvent(QMouseEvent *event);
    //! Propagates key press events to polycode so they can be handled by polycode event handlers.
    void keyPressEvent(QKeyEvent *event);
    //! Propagates key release events to polycode so they can be handled by polycode event handlers.
    void keyReleaseEvent(QKeyEvent *event);
    //! Propagates wheel events to polycode so they can be handled by polycode event handlers.
    void wheelEvent(QWheelEvent *event);
private:
    //! Handles special keys.
    bool checkSpecialKeyEvents(PolyKEY key);
    //! Holdes a boolean to indicate if this widget can be resized or not.
    bool resizableWindow;
    //! Timer for ticks.
    QElapsedTimer timer;
    //! needed internally for mouse delta calculations.
    QPointF lastPos;
};
}
#endif // POLYCODEWIDGET_H
