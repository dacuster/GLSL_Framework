#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <iostream>

#include <Camera.h>

Camera::Camera(glm::vec3 _initialPosition, glm::vec3 _worldUp, GLfloat _initialYaw, GLfloat _initialPitch, GLfloat _initialSpeed, GLfloat _initialAngularSpeed)
{
	mPosition = _initialPosition;
	mWorldUp = _worldUp;
	mYaw = _initialYaw;
	mPitch = _initialPitch;
	mFront = glm::vec3(0.0f, 0.0f, -1.0f);

	mSpeed = _initialSpeed;
	mAngularSpeed = _initialAngularSpeed;

	update();
}

void Camera::keyControl(bool* _keys, GLfloat _deltaTime)
{
	// Calculate velocity this frame.
	GLfloat velocity = mSpeed * _deltaTime;

	// Move forward by speed.
	if (_keys[GLFW_KEY_W])
	{
		mPosition += mFront * velocity;
	}

	// Move backward by speed.
	if (_keys[GLFW_KEY_S])
	{
		mPosition -= mFront * velocity;
	}

	// Move left by speed.
	if (_keys[GLFW_KEY_A])
	{
		mPosition -= mRight * velocity;
	}

	// Move right by speed.
	if (_keys[GLFW_KEY_D])
	{
		mPosition += mRight * velocity;
	}

	// Move down by speed.
	if (_keys[GLFW_KEY_Q])
	{
		mPosition -= mUp * velocity;
	}

	// Move up by speed.
	if (_keys[GLFW_KEY_E])
	{
		mPosition += mUp * velocity;
	}
}

void Camera::mouseControl(GLfloat _deltaX, GLfloat _deltaY)
{
	_deltaX *= mAngularSpeed;
	_deltaY *= mAngularSpeed;

	mYaw += _deltaX;
	mPitch += _deltaY;

	//if (mPitch > 89.0f)
	//{
	//	mPitch = 89.0f;
	//}

	//if (mPitch < -89.0f)
	//{
	//	mPitch = -89.0f;
	//}

	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

void Camera::update()
{
	mFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront.y = sin(glm::radians(mPitch));
	mFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));

	mFront = glm::normalize(mFront);

	mRight = glm::normalize(glm::cross(mFront, mWorldUp));
	mUp = glm::normalize(glm::cross(mRight, mFront));
}
