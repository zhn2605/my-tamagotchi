#include <vector>
#include <glm/vec3.hpp>

using namespace glm;

typedef struct PhysicsComponent {
	vec3 pos;
	vec3 vel;
	vec3 accel;
	bool affectedByGlobalAccel;
	float mass;
} PhysicsComponent;

class PhysicsEngine {
public:
	// Creates engine with default global acceleration -9.81
	PhysicsEngine();
	PhysicsEngine(vec3 globalAcceleration);
	~PhysicsEngine();

	void Update(float dt);
	PhysicsComponent* CreateComponent(vec3 pos);

private:
	vec3 globalAcceleration;
	std::vector<PhysicsComponent*> components;
};