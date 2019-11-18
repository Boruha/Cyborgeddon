// Irrlicht include different namespaces. We won't use them yet
// These namespaces are irr, core, scene, video, io, gui
#include <irrlicht/irrlicht.h>

#include <iostream>

class MyEventReceiver : public irr::IEventReceiver
{
    public :

    bool OnEvent(const irr::SEvent& event) override
    {
        if(event.EventType == irr::EET_KEY_INPUT_EVENT)
        {
            keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
        }

        return false;
    }

    [[nodiscard]] virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const
    {
        return keys[keyCode];
    }

    MyEventReceiver()
    {
        for (bool & key : keys)
            key = false;
    }

    private :

        bool keys[irr::KEY_KEY_CODES_COUNT]{};
};

void input(const MyEventReceiver& eventReceiver, const irr::scene::ISceneNode& cube, irr::core::vector3df& cubeVelocity)
{
    cubeVelocity.X = cubeVelocity.Z = 0;

    if(eventReceiver.IsKeyDown(irr::KEY_KEY_W) && !eventReceiver.IsKeyDown(irr::KEY_KEY_S))
        cubeVelocity.Z = 1;
    else if (!eventReceiver.IsKeyDown(irr::KEY_KEY_W) && eventReceiver.IsKeyDown(irr::KEY_KEY_S))
        cubeVelocity.Z = -1;

    if(eventReceiver.IsKeyDown(irr::KEY_KEY_D) && !eventReceiver.IsKeyDown(irr::KEY_KEY_A))
        cubeVelocity.X = 1;
    else if (!eventReceiver.IsKeyDown(irr::KEY_KEY_D) && eventReceiver.IsKeyDown(irr::KEY_KEY_A))
        cubeVelocity.X = -1;

    if(eventReceiver.IsKeyDown(irr::KEY_SPACE) && cube.getPosition().Y == 0)
        cubeVelocity.Y = 4;
}

void update(irr::scene::ISceneNode& cube, irr::core::vector3df& cubeVelocity)
{
    cube.setPosition(cube.getPosition()+cubeVelocity);

    --cubeVelocity.Y;

    if(cube.getPosition().Y <= 0)
    {
        irr::core::vector3df fixedCubePosition = cube.getPosition();
        fixedCubePosition.Y = 0;

        cube.setPosition(fixedCubePosition);

        cubeVelocity.Y = 0;
    }
}

int main()
{
    /*
     *  We need an IrrlichtDevice* first, so we use function irr::createDevice()
     *
     *
     *  Parameters:
     *
     *  type                        ->      irr::video::[EDT_TYPE] (we use EDT_OPENGL)
     *  window size                 ->      irr::core::dimension2d<irr::u32>(width, height)
     *  bits of colour              ->      16 or 32
     *  fullscreen                  ->      boolean
     *  stencilbuffer (for shadows) ->      boolean
     *  vsync                       ->      boolean (only useful if fullscreen is true)
     *  event receiver              ->      irr::IEventReceiver* (trigger events)
     */

    irr::IrrlichtDevice *device;

    device = irr::createDevice(
            irr::video::EDT_OPENGL,
            irr::core::dimension2d<irr::u32>(640, 480),
            16,
            false,
            false,
            false,
            nullptr
    );

    // If we couldn't find a device, we stop the execution
    if (!device)
    {
        std::cerr << "Device not found!" << std::endl;
        return 1;
    }

    MyEventReceiver eventReceiver;
    device->setEventReceiver(&eventReceiver);

    /*
     *   A caption for the window is needed using the function setWindowCaption(const wchar_t*)
     *   We use a wide char string (wchar_t) by adding an 'L' before the string itself
     *   "string" -> L"string"
     *
     *   Notice the following:
     *
     *   wchar_t is 'OS defined' bytes long
     *   char is 1 byte long
     *
     *   suppose wchar_t is 2 bytes long
     *   notice the difference between these two -> "AB" = [[41],[42]] -> L"AB" = [[00 41],[00 42]]
     */

    device->setWindowCaption(L"Window's name!");

    /*
     *  To avoid using device->xxxxxxx the whole time, we get these pointers:
     *
     *  irr::video::IVideoDriver*
     *  irr::scene::ISceneManager*
     *  irr::gui::IGUIEnvironment*
     */

    irr::video::IVideoDriver*   driver          =   device->getVideoDriver();
    irr::scene::ISceneManager*  sceneManager    =   device->getSceneManager();
    irr::gui::IGUIEnvironment*  gui             =   device->getGUIEnvironment();

    /*
     *  We add a hello world label to the window using the gui environment (wchar_t)
     *  Text is placed at rect (x1,y1,x2,y2) being (x1,y1) top-left and (x2,y2) bottom-right
     *
     *  We use addStaticText() function
     *
     *
     *  Parameters:
     *
     *  string          ->  const wchar_t*
     *  text rectangle  ->  irr::core::rect<irr::s32>(x1,y1,x2,y2)
     *  border          ->  boolean
     *  word wrap       ->  boolean (if true, each word is a whole element that needs to fit entirely into the label)
     *                              (otherwise, it will not be shown)
     *  parent item     ->  irr::gui::IGUIElement*
     *  id              ->  irr::s32 (default = -1)
     *  fill background ->  boolean
     *
     *
     *  The font is non-editable
     */

    gui->addStaticText(
            L"Hey! The size of this damn font cannot be resized :)",
            irr::core::rect<irr::s32>(10,10,191,22),
            false,
            true,
            nullptr,
            -1,
            true
    );

    // TODO: MESH (CUBE/BOX), NODE, CAMERA

    irr::scene::ISceneNode* cube = sceneManager->addCubeSceneNode(10.f);
    irr::core::vector3df cubeVelocity;

    cube->setPosition(irr::core::vector3df(0,0,5));
    //cube->setScale(irr::core::vector3df(1,1,1));
    //cube->setRotation(irr::core::vector3df(0,0,0));

    irr::scene::ICameraSceneNode* camera = sceneManager->addCameraSceneNode();
    camera->setPosition(irr::core::vector3df(0,20,-30));
    camera->setTarget(cube->getPosition());

    /*
     *  We set up the scene. Run the device in a loop until device is closed
     *
     *  beginScene(buffer,zbuffer,colour) clears the screen with the colour specified ->
     *  -> (and the buffer if desired)
     *
     *  From now on we can draw everything we want in screen:
     *
     *      1. First our scene manager
     *      2. Then our GUI environment
     *
     *  endScene() presents every draw on screen
     */

    while(device->run())
    {
        input(eventReceiver, *cube, cubeVelocity);
        update(*cube, cubeVelocity);

        driver->beginScene(true, true, irr::video::SColor(255,255,255,255));

        sceneManager->drawAll();
        gui->drawAll();

        driver->endScene();
    }

	return 0;
}