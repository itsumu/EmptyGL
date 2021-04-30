//
// Created by andrew on 9/16/20.
//

#include "camera.h"
#include "geometry.h"

Eigen::Matrix4f Camera::getViewMatrix() {
    return lookAt(position, position + front, world_up);
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = movement_speed * deltaTime;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
}

void Camera::processMouseMovement(float x_offset, float y_offset, GLboolean constrain_pitch) {
    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;

    yaw   -= x_offset; // Right movement (Clockwise) for yaw is minus
    pitch += y_offset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrain_pitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::processMouseScroll(float y_offset) {
    zoom -= y_offset;
    if (zoom < 1.0f) zoom = 1.0f;
    if (zoom > 45.0f) zoom = 45.0f;
}

void Camera::updateCameraVectors() {
    // Calculate the new Front vector
    float yaw_from_z = yaw - 180.0f; // Minus 180 degrees since raw yaw is along +z
    front[0] = sin(degree2Radian(yaw_from_z)) * cos(degree2Radian(pitch));
    front[1] = sin(degree2Radian(pitch));
    front[2] = cos(degree2Radian(yaw_from_z)) * cos(degree2Radian(pitch));
    front = front.normalized();

    // Re-calculate the Right and Up vector
    right = front.cross(world_up).normalized();
    up = right.cross(front).normalized();
}