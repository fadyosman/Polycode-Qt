#include "polycodewidget.h"
#include <QDir>
#include <QClipboard>
#include <QApplication>
#include <QtConcurrent>
#include <QFile>
#include <QFileDialog>
#include <PolyEvent.h>
#include <QDesktopServices>
using namespace Polycode;
PolycodeWidget::PolycodeWidget(PolycodeView *view, int xRes, int yRes, bool fullScreen, bool vSync, int aaLevel, int anisotropyLevel, int frameRate, int monitorIndex, bool retinaSupport) :
    Core(xRes, yRes, fullScreen, vSync, aaLevel, anisotropyLevel, frameRate, monitorIndex)
{
    Q_UNUSED(retinaSupport)
    //TODO: Move this to the setVideoMode function.
    QGLFormat format = this->format();
    format.setDepth(true);
    format.setAlpha(true);
    format.setDepthBufferSize(24);
    format.setDoubleBuffer(true);
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    //format.setAlphaBufferSize(8);
    if(vSync) {
        format.setSwapInterval(1);
    } else {
        format.setSwapInterval(0);
    }
    if(aaLevel > 0) {
        format.setSampleBuffers(true);
        format.setSamples(aaLevel);
    } else {
        format.setSampleBuffers(false);
        format.setSamples(0);
    }
    this->setFormat(format);
    this->makeCurrent();
    //create();
    this->resizableWindow = view->resizable;
    defaultWorkingDirectory = QDir::currentPath().toStdString();
    userHomeDirectory = QDir::homePath().toStdString();
    this->setWindowTitle(QString::fromStdString(view->windowTitle.getSTLString()));
    eventMutex = createMutex();
    renderer = new OpenGLRenderer();
    services->setRenderer(renderer);
    setVideoMode(xRes,yRes,fullScreen,vSync,aaLevel,anisotropyLevel);
    this->setMouseTracking(true);
}

void PolycodeWidget::setVideoMode(int xRes, int yRes, bool fullScreen, bool vSync, int aaLevel, int anisotropyLevel) {
    this->xRes = xRes;
    this->yRes = yRes;
    this->fullScreen = fullScreen;
    this->aaLevel = aaLevel;
    if(fullScreen) {
        this->showFullScreen();
    } else {
        this->showNormal();
    }
    QRect g = this->geometry();
    g.setWidth(xRes);
    g.setHeight(yRes);
    this->setGeometry(g);
    renderer->Resize(xRes,yRes);
    dispatchEvent(new Event(), EVENT_CORE_RESIZE);
    Q_UNUSED(anisotropyLevel)
}

CoreMutex *PolycodeWidget::createMutex() {
    QtCoreMutex *mutex = new QtCoreMutex();
    mutex->pMutex = new QMutex();
    return mutex;
}
void PolycodeWidget::lockMutex(CoreMutex *mutex) {
    QtCoreMutex *m = (QtCoreMutex *)mutex;
    m->pMutex->lock();
}
void PolycodeWidget::unlockMutex(CoreMutex *mutex) {
    QtCoreMutex *m = (QtCoreMutex *)mutex;
    m->pMutex->unlock();
}
void PolycodeWidget::copyStringToClipboard(const String &str) {
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString::fromStdString(str.getSTLString()));
}
String PolycodeWidget::getClipboardString() {
    QClipboard *clipboard = QApplication::clipboard();
    QString originalText = clipboard->text();
    return originalText.toStdString();
}
void PolycodeWidget::enableMouse(bool newval) {
    if(newval) {
        this->grabMouse();
        qApp->setOverrideCursor(Qt::BlankCursor);
    } else {
        this->releaseMouse();
        qApp->setOverrideCursor(Qt::ArrowCursor);
    }
    Core::enableMouse(newval);
}
void PolycodeWidget::captureMouse(bool newval) {
    if(newval) {
        this->grabMouse();
    } else {
        this->releaseMouse();
    }
    Core::captureMouse(newval);
}
void PolycodeWidget::setCursor(int cursorType) {
    Qt::CursorShape shape = (Qt::CursorShape)cursorType;
    QCursor cursor(shape);
    qApp->setOverrideCursor(cursor);
}
void PolycodeWidget::createThread(Threaded *target) {
    QtConcurrent::run(target,&Threaded::runThread);
}
void PolycodeWidget::createFolder(const String &folderPath) {
    QDir().mkdir(QString::fromStdString(folderPath.getSTLString()));
}
void PolycodeWidget::copyDiskItem(const String &itemPath, const String &destItemPath) {
    QFile::copy(QString::fromStdString(itemPath.getSTLString()),QString::fromStdString(destItemPath.getSTLString()));
}
void PolycodeWidget::moveDiskItem(const String &itemPath, const String &destItemPath) {
    QDir dir;
    dir.rename(QString::fromStdString(itemPath.getSTLString()),QString::fromStdString(destItemPath.getSTLString()));
}
void PolycodeWidget::removeDiskItem(const String &itemPath) {
    QFile::remove(QString::fromStdString(itemPath.getSTLString()));
}
String PolycodeWidget::openFolderPicker() {
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     "/home",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    return String(path.toStdString());
}
std::vector<String> PolycodeWidget::openFilePicker(std::vector<CoreFileExtension> extensions, bool allowMultiple) {
    //TODO: Fix extensions support.
    Q_UNUSED(extensions)
    QStringList files;
    if(allowMultiple) {
        files = QFileDialog::getOpenFileNames();
    } else {
        QString file = QFileDialog::getOpenFileName();
        files.append(file);
    }
    std::vector<String> filesVector;
    foreach (QString s, files) {
        filesVector.push_back(s.toStdString());
    }
    return filesVector;
}
void PolycodeWidget::openURL(String url) {
    QDesktopServices::openUrl(QString::fromStdString(url.getSTLString()));
}
String PolycodeWidget::executeExternalCommand(String command, String args, String inDirectory) {
    QProcess process;
    if(inDirectory != "") {
        process.setWorkingDirectory(QString::fromStdString(command.getSTLString()));
    }
    QString program = QString::fromStdString(command.getSTLString());
    QString arguments = QString::fromStdString(args.getSTLString());

    process.start(program,arguments.split(" "));

    if(process.error() == QProcess::FailedToStart) {
        return "Unable to execute command";
    } else if (process.error() == QProcess::Crashed) {
        return "Process Crashed";
    }

    while(process.state() == QProcess::Running) {

    }
    String retString;
    QByteArray data = process.readAllStandardOutput();
    retString.setDataWithEncoding(data.data(),String::ENCODING_UTF8);
    return retString;
}
void PolycodeWidget::resizeTo(int xRes, int yRes) {
    if(resizableWindow) {
        renderer->Resize(xRes, yRes);
    }
}
unsigned int PolycodeWidget::getTicks() {
    return (unsigned int)timer.elapsed();
}
void PolycodeWidget::Render() {
    renderer->BeginRender();
    services->Render();
    renderer->EndRender();
    this->swapBuffers();
}
bool PolycodeWidget::Update() {
    if(!running)
        return false;
    return running;
}
void PolycodeWidget::warpCursor(int x, int y) {

}
bool PolycodeWidget::checkSpecialKeyEvents(PolyKEY key) {
    if(key == KEY_a && (input->getKeyState(KEY_LCTRL) || input->getKeyState(KEY_RCTRL))) {
        dispatchEvent(new Event(), Core::EVENT_SELECT_ALL);
        return true;
    }
    if(key == KEY_c && (input->getKeyState(KEY_LCTRL) || input->getKeyState(KEY_RCTRL))) {
        dispatchEvent(new Event(), Core::EVENT_COPY);
        return true;
    }
    if(key == KEY_x && (input->getKeyState(KEY_LCTRL) || input->getKeyState(KEY_RCTRL))) {
        dispatchEvent(new Event(), Core::EVENT_CUT);
        return true;
    }
    if(key == KEY_z && (input->getKeyState(KEY_LCTRL) || input->getKeyState(KEY_RCTRL)) && (input->getKeyState(KEY_LSHIFT) || input->getKeyState(KEY_RSHIFT))) {
        dispatchEvent(new Event(), Core::EVENT_REDO);
        return true;
    }
    if(key == KEY_z && (input->getKeyState(KEY_LCTRL) || input->getKeyState(KEY_RCTRL))) {
        dispatchEvent(new Event(), Core::EVENT_UNDO);
        return true;
    }
    if(key == KEY_v && (input->getKeyState(KEY_LCTRL) || input->getKeyState(KEY_RCTRL))) {
        dispatchEvent(new Event(), Core::EVENT_PASTE);
        return true;
    }
    return false;
}
void PolycodeWidget::initializeGL() {
    ((OpenGLRenderer*)renderer)->Init();
    CoreServices::getInstance()->installModule(new GLSLShaderModule());
    timer.start();
}
void PolycodeWidget::paintGL() {
    this->Render();
    this->Update();
    this->update();
    qApp->processEvents();
}
void PolycodeWidget::resizeGL(int width, int height) {
    this->resizeTo(width,height);
}
void PolycodeWidget::resize(int w, int h) {
    this->resizeGL(w,h);
}
void PolycodeWidget::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::MiddleButton) {
        input->setMouseButtonState(CoreInput::MOUSE_BUTTON3, true, getTicks());
    } else if (event->button() == Qt::LeftButton) {
        input->setMouseButtonState(CoreInput::MOUSE_BUTTON1, true, getTicks());
    } else if (event->button() == Qt::RightButton) {
        input->setMouseButtonState(CoreInput::MOUSE_BUTTON2, true, getTicks());
    }
    lastPos = event->pos();
}
void PolycodeWidget::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::MiddleButton) {
        input->setMouseButtonState(CoreInput::MOUSE_BUTTON3, false, getTicks());
    } else if (event->button() == Qt::LeftButton) {
        input->setMouseButtonState(CoreInput::MOUSE_BUTTON1, false, getTicks());
    } else if (event->button() == Qt::RightButton) {
        input->setMouseButtonState(CoreInput::MOUSE_BUTTON2, false, getTicks());
    }
    lastPos = event->pos();
}
void PolycodeWidget::mouseMoveEvent(QMouseEvent *event) {
    int deltax = event->x() - lastPos.x();
    int deltay = event->y() - lastPos.y();
    input->setDeltaPosition(deltax,deltay);
    input->setMousePosition(event->x(), event->y(), getTicks());
    lastPos = event->pos();
}
void PolycodeWidget::keyPressEvent(QKeyEvent *event) {
    if(!checkSpecialKeyEvents(((PolyKEY)event->key()))) {
        input->setKeyState((PolyKEY)event->key(), (char)event->key(), true, getTicks());
    }
}
void PolycodeWidget::keyReleaseEvent(QKeyEvent *event) {
    input->setKeyState((PolyKEY)event->key(), (char)event->key(), true, getTicks());
}
void PolycodeWidget::wheelEvent(QWheelEvent *event) {
    //Up.
    if(event->delta() > 0) {
        input->mouseWheelUp(getTicks());
    }
    //Down.
    if(event->delta() < 0) {
        input->mouseWheelDown(getTicks());
    }
}
