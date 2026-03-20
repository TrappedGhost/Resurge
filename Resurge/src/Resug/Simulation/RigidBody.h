#pragma once
namespace Resug
{
	class RigidBody
	{
	public:
		RigidBody() = default;

		void CalculateForce();
		void CalculateAcceleration();
		void CalculateVelocity(float ts);

		glm::vec3 GetDisplacement(float ts);


		const glm::vec3 GetVelocity() { return Velocity; }
		const float GetVelocityX() { return Velocity.x; }
		const float GetVelocityY() { return Velocity.y; }
		const float GetVelocityZ() { return Velocity.z; }
		const glm::vec3 GetAcceleration() { return Acceleration; }
		const glm::vec3 GetForce() { return Force; }
		const float GetMass() { return Mass; }

		void SetVelocity(glm::vec3 v) {  Velocity = v; }
		void SetVelocityX(float v) {  Velocity.x = v; }
		void SetVelocityY(float v) {  Velocity.y = v; }
		void SetVelocityZ(float v) {  Velocity.z = v; }
		void SetAcceleration(glm::vec3 a) {  Acceleration = a; }
		void SetForce(glm::vec3 f) {  Force = f; }
		void SetMass(float m) {  Mass = m; }

	private:
		glm::vec3 Velocity = glm::vec3(0.0f);
		glm::vec3 Acceleration = glm::vec3(0.0f);
		glm::vec3 Force = glm::vec3(0.0f);

		float Mass = 1.0f;

	}; 
}
