//
// Created by Andrew on 3/9/2021.
//

#ifndef EMPTYGL_GEOMETRY_H
#define EMPTYGL_GEOMETRY_H

#include <Eigen/Dense>

const float PI = 3.14159265359f;

template<class T>
T degree2Radian(T &degree) {
    return (degree * static_cast<T>(1.0f / 180.0f * PI));
}

Eigen::Matrix4f lookAt(const Eigen::Vector3f &position, const Eigen::Vector3f &target, const Eigen::Vector3f &up);

Eigen::Matrix4f perspective(float fov_y, float aspect_ratio, float z_near, float z_far);

Eigen::Matrix4f orthographic(float left, float right, float bottom, float top, float near, float far);

#endif //EMPTYGL_GEOMETRY_H
