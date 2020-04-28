#pragma once

class Camera
{
public:
	Camera() {}
	Camera(glm::vec3 _initialPosition, glm::vec3 _worldUp, GLfloat _initialYaw, GLfloat _initialPitch, GLfloat _initialSpeed, GLfloat _initialAngularSpeed);
	~Camera() {}

	void keyControl(bool* _keys, GLfloat _deltaTime);

	void mouseControl(bool* _buttons, GLfloat _deltaX, GLfloat _deltaY);

	glm::mat4 calculateViewMatrix();

	glm::vec3 getPosition() const { return mPosition; }
	glm::vec3 getFront() const { return mFront; }

	GLfloat getYaw() const { return mYaw; }
	GLfloat getPitch() const { return mPitch; }

private:
	// Position of the camera.
	glm::vec3 mPosition;
	
	// Orientation of the camera.
	glm::vec3 mFront;
	glm::vec3 mUp;
	glm::vec3 mRight;

	// World up reference.
	glm::vec3 mWorldUp;

	// Horizontal rotation.
	GLfloat mYaw;

	// Vertical rotation.
	GLfloat mPitch;

	// Movement speeds.
	GLfloat mSpeed;
	GLfloat mAngularSpeed;

	// Update data.
	void update();
};