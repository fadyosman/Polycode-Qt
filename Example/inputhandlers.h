#ifndef INPUTHANDLERS_H
#define INPUTHANDLERS_H
#include <PolyEvent.h>
#include <PolyCore.h>
#include <PolySceneSprite.h>
#include <PolyScene.h>
using namespace Polycode;
class InputHandler : public EventHandler
{
public:
    InputHandler(Core *core, Scene *scene);
    void handleEvent(Event *e);
    bool Update();
private:
    Core *core;
    Scene *scene;
};

#endif // INPUTHANDLERS_H
