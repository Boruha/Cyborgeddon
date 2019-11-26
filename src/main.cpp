#include <irrlicht/irrlicht.h>
#include <iostream>
#include <vector>

using namespace irr;

using core::vector3df;
using video::ITexture;
using scene::ISceneNode;
using video::IVideoDriver;
using scene::ISceneManager;
using video::SColor;
using video::EDT_OPENGL;
using core::dimension2d;
using scene::ICameraSceneNode;

using std::cerr;
using std::endl;



enum EntityID : u_int8_t { CUBE_ID, CAMERA_ID };



class EventReceiver : public IEventReceiver
{
private:
	bool keys[KEY_KEY_CODES_COUNT]{};

public:
	EventReceiver()
	{
		for (bool& key : keys)
		{
			key = false;
		}
	}

	inline bool OnEvent(const SEvent& event) override
	{
		if(event.EventType == EET_KEY_INPUT_EVENT)
		{
			keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
		}

		return false;
	}

	[[nodiscard]] inline virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return keys[keyCode];
	}
};



struct Transformable
{
/*	Vector3f	position;	*/
/*	Vector3f	rotation;	*/
/*	Vector3f	scale;		*/

	vector3df	position;
	vector3df	rotation;
	vector3df	scale;
};

struct Velocity
{
/*	Vector3f	velocity;	*/

	vector3df	velocity	= vector3df(0,0,0);
};

struct Gravity
{
/*	Vector3f	gravity;	*/

	vector3df	gravity 	= vector3df(0,-1,0);
};

struct Renderable
{
	explicit Renderable(ISceneNode* node) : node(node){}

	Transformable 	transformable;
	ISceneNode*		node;
};

struct Camera
{
	explicit Camera(ICameraSceneNode* camera) : cameraNode(camera){}
	ICameraSceneNode*	cameraNode;
};



struct Entity
{
	explicit Entity(EntityID ID) : ID(ID){}
	//[[nodiscard]] inline EntityID getID() const { return ID; }
protected:
	EntityID ID;
};

struct EntityCube : Entity
{
	explicit EntityCube(ISceneManager* sceneManager) : Entity(CUBE_ID), renderable(sceneManager->addCubeSceneNode(10.f))
	{
		renderable.node->setPosition(vector3df(0,0,5));
	}

	Renderable 	renderable;
	Velocity 	velocity;
	Gravity 	gravity;
};

struct EntityCamera : Entity
{
	explicit EntityCamera(ISceneManager* sceneManager) : Entity(CAMERA_ID), camera(sceneManager->addCameraSceneNode())
	{
		camera.cameraNode->setPosition(vector3df(0,20,-30));
	}

	Camera			camera;
};


struct InputSystem
{
	inline static void input(const EventReceiver& eventReceiver, const Transformable& trans, Velocity& vel)
	{
		vel.velocity.X = vel.velocity.Z = 0;

		if(eventReceiver.IsKeyDown(irr::KEY_KEY_W) && !eventReceiver.IsKeyDown(irr::KEY_KEY_S))
			vel.velocity.Z = 1;
		else if (!eventReceiver.IsKeyDown(irr::KEY_KEY_W) && eventReceiver.IsKeyDown(irr::KEY_KEY_S))
			vel.velocity.Z = -1;

		if(eventReceiver.IsKeyDown(irr::KEY_KEY_D) && !eventReceiver.IsKeyDown(irr::KEY_KEY_A))
			vel.velocity.X = 1;
		else if (!eventReceiver.IsKeyDown(irr::KEY_KEY_D) && eventReceiver.IsKeyDown(irr::KEY_KEY_A))
			vel.velocity.X = -1;

		if(eventReceiver.IsKeyDown(irr::KEY_SPACE) && trans.position.Y == 0)
			vel.velocity.Y = 4;
	}
};

struct MovementSystem
{
	inline static void update(Transformable& trans, const Velocity& vel) { trans.position += vel.velocity; }
};

struct GravitySystem
{
	inline static void update(Transformable& trans, Velocity& vel, const Gravity& gra)
	{
		vel.velocity += gra.gravity;

		if(trans.position.Y <= 0)
		{
			trans.position.Y = vel.velocity.Y = 0;
		}
	}
};

struct RenderSystem
{
	inline static void draw(IVideoDriver* driver, ISceneManager* sceneManager)
	{
		driver->beginScene(true, true, SColor(255,255,255,255));
		sceneManager->drawAll();
		driver->endScene();
	}
};



int main()
{
	IrrlichtDevice* device;

	device = createDevice(EDT_OPENGL,dimension2d<u32>(640, 480));

	if(!device)
	{
		cerr << "Device not found" << endl;
		return 1;
	}

	EventReceiver eventReceiver;
	device->setEventReceiver(&eventReceiver);

	device->setWindowCaption(L"Window's name!");

	IVideoDriver* 	driver 			= device->getVideoDriver();
	ISceneManager*	sceneManager	= device->getSceneManager();

	std::vector<EntityCube*> cubes;
	std::vector<EntityCamera*> cameras;

	EntityCube cube(sceneManager);
	cubes.push_back(&cube);

	EntityCamera camera(sceneManager);
	camera.camera.cameraNode->setTarget(cube.renderable.node->getPosition());
	cameras.push_back(&camera);

	while(device->run())
	{
		for(EntityCube* c : cubes)
		{
			// No se como hacer que los tres vectores de Transformable "apunten" a los valores de node
			c->renderable.transformable.position 	= c->renderable.node->getPosition();
			c->renderable.transformable.rotation 	= c->renderable.node->getRotation();
			c->renderable.transformable.scale 		= c->renderable.node->getScale();

			InputSystem::input(eventReceiver, c->renderable.transformable, c->velocity);
			MovementSystem::update(c->renderable.transformable, c->velocity);
			GravitySystem::update(c->renderable.transformable, c->velocity, c->gravity);

			// No se como hacer que los tres vectores de Transformable "apunten" a los valores de node
			c->renderable.node->setPosition(c->renderable.transformable.position);
			c->renderable.node->setRotation(c->renderable.transformable.rotation);
			c->renderable.node->setScale(c->renderable.transformable.scale);
		}

		RenderSystem::draw(driver, sceneManager);
	}

	device->drop();

	return 0;
}