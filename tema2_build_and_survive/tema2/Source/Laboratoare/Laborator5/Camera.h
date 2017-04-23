#pragma once
#include <include/glm.h>
#include <include/math.h>

namespace Laborator
{
	class Camera
	{
		public:
			Camera()
			{
				position = glm::vec3(0, 0, 0);
				forward = glm::vec3(0, 0, -1);
				up		= glm::vec3(0, 1, 0);
				right	= glm::vec3(1, 0, 0);
				distanceToTarget = 2;
			}

			Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
			{
				Set(position, center, up);
			}

			~Camera()
			{ }

			// Update camera
			void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
			{
				this->position = position;
				forward = glm::normalize(center-position);
				right	= glm::cross(forward, up);
				this->up = glm::cross(right,forward);
			}

			void translate(glm::vec3 t) {
				position = position + t;
			}

			void MoveForward(float distance)
			{
				glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));

				// movement will keep the camera at the same height always
				// Example: If you rotate up/down your head and walk forward you will still keep the same relative distance (height) to the ground!
				// Translate the camera using the DIR vector computed from forward
			}

			glm::vec3 TranslateForward(float distance)
			{
				glm::vec3 proj;//move with the porjection of vector forward onto the ZoX plane
				proj.y = 0;
				proj.z = glm::dot(glm::vec3(forward), glm::vec3(0, 0, 1));
				proj.x = glm::dot(glm::vec3(forward), glm::vec3(1, 0, 0));
				proj = normalize(proj);
				position = position + proj * distance;
				return proj * distance;
				//genType glm::normalize(genType const & 	x)
				//Returns a vector in the same direction as x but with length of 1.
			}

			void TranslateToTarget(float distance) {

				position = position + forward * distance;
			}
				
			void TranslateUpword(float distance)
			{
				position = position + glm::normalize(up) * distance;
			}

			glm::vec3 TranslateRight(float distance)
			{
				position = position + glm::normalize(right) * distance;
				return glm::normalize(right) * distance;
				// TODO
				// Translate the camera using the "right" vector
				// Usually translation using camera "right' is not very useful because if the camera is rotated around the "forward" vector 
				// translation over the right direction will have an undesired effect; the camera will get closer or farther from the ground
				// Using the projected right vector (onto the ground plane) makes more sense because we will keep the same distance from the ground plane
			}



			void RotateFirstPerson_OX(float angle)
			{


				forward = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(forward, 1));
				forward = glm::normalize(forward);

				up = glm::normalize(glm::cross(right, forward));
				// TODO
				// Compute the new "forward" and "up" vectors
				// Attention! Don't forget to normalize the vectors
				// Use glm::rotate()
			}

			void RotateFirstPerson_OY(float angle)
			{
				forward = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1));
				forward = glm::normalize(forward);
				right = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1));
				right = glm::normalize(right);
				up = glm::normalize(cross(right, forward));
				// TODO
				// Compute the new "forward", "up" and "right" vectors
				// Don't forget to normalize the vectors
				// Use glm::rotate()
			}

			void RotateFirstPerson_OZ(float angle)
			{

				right = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::vec4(right, 1));
				right = glm::normalize(right);
				up = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::vec4(up, 1));
				up = glm::normalize(up);
			}

			void RotateThirdPerson_OX(float angle)
			{
				TranslateToTarget(distanceToTarget);
				RotateFirstPerson_OX(angle);
				TranslateToTarget(-distanceToTarget);
			}

			void RotateThirdPerson_OY(float angle)
			{
				TranslateToTarget(distanceToTarget);
				RotateFirstPerson_OY(angle);
				TranslateToTarget(-distanceToTarget);
			}

			void RotateThirdPerson_OZ(float angle)
			{

				TranslateToTarget(distanceToTarget);
				RotateFirstPerson_OZ(angle);
				TranslateToTarget(-distanceToTarget);
			}

			glm::mat4 GetViewMatrix()
			{
				// Returns the View Matrix
				return glm::lookAt(position, position + forward, up);
			}

			glm::vec3 GetTargetPosition()
			{
				return position + forward * distanceToTarget;
			}

		public:
			float distanceToTarget;
			glm::vec3 position;
			glm::vec3 forward;
			glm::vec3 right;
			glm::vec3 up;
		};
}