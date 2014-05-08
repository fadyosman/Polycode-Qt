#include <QApplication>
#include "polycodewidget.h"
#include <PolycodeView.h>
#include <PolyScene.h>
#include <PolyScenePrimitive.h>
#include <PolyCamera.h>
#include <PolySceneImage.h>
#include <PolyCoreServices.h>
#include <PolyResourceManager.h>
#include "inputhandlers.h"
#include <QTimer>
#include <PolySceneLabel.h>
using namespace Polycode;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PolycodeView *view = new PolycodeView("Space Fighter",true);
    PolycodeWidget *widget = new PolycodeWidget(view, 800,600,false,true,0,0,90, 0, true);

    Scene *scene = new Scene();
    ScenePrimitive *ground = new ScenePrimitive(ScenePrimitive::TYPE_PLANE, 5,5);
    ground->setColor(1.0,0.0,0.0,1.0);
    scene->addEntity(ground);
    scene->clearColor = Color(1.0,0.0,0.0,0.0);
    InputHandler handler(widget,scene);
    ScenePrimitive *box = new ScenePrimitive(ScenePrimitive::TYPE_BOX, 1,1,1);
    box->setColor(0.0,1.0,0.0,1.0);
    box->setPosition(0.0, 0.5, 0.0);
    scene->addEntity(box);
    //scene->addEntity(image);
    scene->getDefaultCamera()->setPosition(7,7,7);
    scene->getDefaultCamera()->lookAt(Vector3(0,0,0));
    widget->show();
    QTimer timer;
    QObject::connect(&timer,SIGNAL(timeout()),widget,SLOT(update()));
    return a.exec();
}
