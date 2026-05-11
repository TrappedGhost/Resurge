#pragma once
namespace Resug
{
	class RigidBody
	{
	public:
		RigidBody() = default;

		void CalculateForce();
		void CalculateAcceleration();
		void CalculateVelocity(double ts);

		glm::dvec3 GetDisplacement(double ts);


		const glm::dvec3 GetVelocity() { return Velocity; }
		const double GetVelocityX() { return Velocity.x; }
		const double GetVelocityY() { return Velocity.y; }
		const double GetVelocityZ() { return Velocity.z; }
		const glm::dvec3 GetAcceleration() { return Acceleration; }
		const glm::dvec3 GetForce() { return Force; }
		const double GetMass() { return Mass; }

		void SetVelocity(glm::dvec3 v) {  Velocity = v; }
		void SetVelocityX(double v) {  Velocity.x = v; }
		void SetVelocityY(double v) {  Velocity.y = v; }
		void SetVelocityZ(double v) {  Velocity.z = v; }
		void SetAcceleration(glm::dvec3 a) {  Acceleration = a; }
		void SetForce(glm::dvec3 f) {  Force = f; }
		void SetMass(double m) {  Mass = m; }

	private:
		glm::dvec3 Velocity = glm::dvec3(0.0f);
		glm::dvec3 Acceleration = glm::dvec3(0.0f);
		glm::dvec3 Force = glm::dvec3(0.0f);

		double Mass = 1.0f;

	}; 
}
