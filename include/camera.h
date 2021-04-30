#ifndef EMPTYGL_CAMERA_H
#define EMPTYGL_CAMERA_H

#include <vector>
#include <Eigen/Dense>
#include <glad/glad.h>

using Eigen::Vector3f;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         =  0.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  90.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
    // Extrinsics
    Vector3f position;
    Vector3f front;
    Vector3f up;
    Vector3f right;
    Vector3f world_up;
    float yaw;
    float pitch;

    // Camera options
    float movement_speed;
    float mouse_sensitivity;
    float zoom;

    // Default constructor
    Camera() : position(Vector3f(0.0f, 0.0f, 0.0f)), world_up(Vector3f(0.0f, 1.0f, 0.0f)),
                front(Vector3f(0.0f, 0.0f, -1.0f)), yaw(YAW), pitch(PITCH), movement_speed(SPEED),
                mouse_sensitivity(SENSITIVITY), zoom(ZOOM) {
        updateCameraVectors();
    }

    // constructor with vectors
    Camera(Vector3f position, Vector3f up, float yaw, float pitch) :
            front(Vector3f(0.0f, 0.0f, -1.0f)), movement_speed(SPEED),
            mouse_sensitivity(SENSITIVITY), zoom(ZOOM) {
        this->position = position;
        world_up = up;
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors();
    }

    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
            front(Vector3f(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), zoom(ZOOM) {
        position = Vector3f(posX, posY, posZ);
        world_up = Vector3f(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    Eigen::Matrix4f getViewMatrix();

    // Process input received from any keyboard-like input system
    // Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyboard(Camera_Movement direction, float deltaTime);

    // Process input received from a mouse input system
    // Expects the offset value in both the x and y direction
    void processMouseMovement(float x_offset, float y_offset, GLboolean constrain_pitch=true);

    // Process input received from a mouse scroll-wheel event
    // Only requires input on the vertical wheel-axis
    void processMouseScroll(float y_offset);

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};
#endif // EMPTYGL_CAMERA_H