#include "Globals.h"
#include "Application.h"
#include "ModulePhysics3D.h"
#include "Primitive.h"

// TODO 1: ...and the 3 libraries based on how we compile (Debug or Release)
// use the _DEBUG preprocessor define
#ifdef _DEBUG
#pragma comment( lib, "Bullet/libx86/BulletCollision_debug.lib" )
#pragma comment( lib, "Bullet/libx86/BulletDynamics_debug.lib" )
#pragma comment( lib, "Bullet/libx86/LinearMath_debug.lib" )
#else
#pragma comment( lib, "Bullet/libx86/BulletCollision.lib" )
#pragma comment( lib, "Bullet/libx86/BulletDynamics.lib" )
#pragma comment( lib, "Bullet/libx86/LinearMath.lib" )
#endif
	

ModulePhysics3D::ModulePhysics3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug_draw = NULL;
	debug = true;

	// TODO 2: Create collision configuration, dispacher
	// broad _phase and solver
	collision_config = new btDefaultCollisionConfiguration;
	collision_dispatcher = new btCollisionDispatcher(collision_config);
	broad_phase = new btDbvtBroadphase;
	constraint_solver = new btSequentialImpulseConstraintSolver;

	// Uncomment this to enable debug drawer
	debug_draw = new DebugDrawer();
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{
	delete debug_draw;

	// TODO 2: and destroy them!
	delete collision_config;
	delete collision_dispatcher;
	delete broad_phase;
	delete constraint_solver;

}

// ---------------------------------------------------------
bool ModulePhysics3D::Start()
{
	LOG("Creating Physics environment");

	// TODO 3: Create the world and set default gravity
	// Have gravity defined in a macro!
	world = new btDiscreteDynamicsWorld(collision_dispatcher,
										broad_phase,
										constraint_solver,
										collision_config);

	world->setGravity({ 0, YGRAVITY, 0 });
	// Uncomment this line to have the world use our debug drawer
	world->setDebugDrawer(debug_draw);

	{
		// TODO 5: Create a big rectangle as ground
		// Big rectangle as ground

		btCollisionShape* ground_shape = new btBoxShape(btVector3(100.0f, 1.0f, 100.0f));

		btDefaultMotionState* defaul = new btDefaultMotionState();
		btRigidBody::btRigidBodyConstructionInfo rbinfo(0.0f, defaul, ground_shape);

		btRigidBody* ground = new btRigidBody(rbinfo);

		world->addRigidBody(ground);
	}

	return true;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PreUpdate(float dt)
{
	// TODO 4: step the world
	world->stepSimulation(dt, 15);

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
	{
		world->debugDrawWorld();
		
		if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			// TODO 6: Create a Solid Sphere when pressing 1 on camera position
		}
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics3D::CleanUp()
{
	LOG("Destroying 3D Physics simulation");

	// TODO 3: ... and destroy the world here!

	return true;
}

// =============================================

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	line.origin.Set(from.getX(), from.getY(), from.getZ());
	line.destination.Set(to.getX(), to.getY(), to.getZ());
	line.color.Set(color.getX(), color.getY(), color.getZ());
	line.Render();
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	point.transform.translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	point.color.Set(color.getX(), color.getY(), color.getZ());
	point.Render();
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("Bullet warning: %s", warningString);
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	LOG("Bullet draw text: %s", textString);
}

void DebugDrawer::setDebugMode(int debugMode)
{
	mode = (DebugDrawModes) debugMode;
}

int	 DebugDrawer::getDebugMode() const
{
	return mode;
}
