#include "inputhandlers.h"
#include <PolyEvent.h>
#include <PolyInputEvent.h>
#include <PolySceneImage.h>
#include <PolyCamera.h>
#include <QDebug>
InputHandler::InputHandler(Core *core,Scene *scene)
{
    this->core = core;
    this->scene = scene;
    //Adding the event listners.
    core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEMOVE);
    core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEWHEEL_UP);
    core->getInput()->addEventListener(this, InputEvent::EVENT_MOUSEWHEEL_DOWN);
}
void InputHandler::handleEvent(Event *e) {
    if(e->getDispatcher() == core->getInput()) {
        InputEvent *inputEvent = (InputEvent*)e;
        switch(e->getEventCode()) {
        //Mouse move for rotating the scene by mouse movement.
        case InputEvent::EVENT_MOUSEMOVE:
            static int lastx = inputEvent->mousePosition.x;
            if(core->getInput()->getMouseButtonState(CoreInput::MOUSE_BUTTON1)) {
                if (inputEvent->mousePosition.x > lastx) {
                    scene->rootEntity.Yaw(2);
                } else {
                    scene->rootEntity.Yaw(-2);
                }
                lastx = inputEvent->mousePosition.x;
            }
            break;
        case InputEvent::EVENT_MOUSEWHEEL_UP:
            //Zoom out with wheel.
            if(this->scene->getActiveCamera()->getFOV() >= 0) {
                this->scene->getActiveCamera()->setFOV(this->scene->getActiveCamera()->getFOV() - 1);
            }
            break;
            //Zoom in with wheel.
        case InputEvent::EVENT_MOUSEWHEEL_DOWN:
            if(this->scene->getActiveCamera()->getFOV() <= 170) {
                this->scene->getActiveCamera()->setFOV(this->scene->getActiveCamera()->getFOV() + 1);
            }
            break;
        }
    }
}

bool InputHandler::Update() {
    //Updating : add animation here if you want.
    return core->Update();
}
