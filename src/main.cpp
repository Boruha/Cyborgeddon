// Irrlicht include different namespaces. We won't use them yet
// These namespaces are irr, core, scene, video, io, gui
#include <irrlicht/irrlicht.h>

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
        return 1;
    }

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
        driver->beginScene(true, true, irr::video::SColor(255,255,255,255));

        sceneManager->drawAll();
        gui->drawAll();

        driver->endScene();
    }

	return 0;
}