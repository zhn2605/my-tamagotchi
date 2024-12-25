#include "physics.hpp"

PhysicsEngine::PhysicsEngine()
{
	this->globalAcceleration = vec3{0, -9.81, 0};
}

PhysicsEngine::PhysicsEngine(vec3 globalAcceleration)
{
	this->globalAcceleration = globalAcceleration;
}

PhysicsEngine::~PhysicsEngine()
{
	for (PhysicsComponent* c : components)
	{
		delete c;
	}
}

void PhysicsEngine::Update(float dt)
{
	vec3 netAccel;
	for (PhysicsComponent* c : components)
	{
		if (c->affectedByGlobalAccel)
			netAccel = c->accel + globalAcceleration;
		else
			netAccel = c->accel;
		// TODO!
	}
}

PhysicsComponent* PhysicsEngine::CreateComponent(vec3 pos)
{
	PhysicsComponent* c = new PhysicsComponent{ pos, vec3(0.0f), vec3(0.0f), true, 0.0f };
	components.push_back(c);
	return c;
}