//
// Created by Andrew on 3/9/2021.
//

#include "geometry.h"

Eigen::Matrix4f lookAt(const Eigen::Vector3f &position, const Eigen::Vector3f &target, const Eigen::Vector3f &up) {
    Eigen::Matrix4f view_matrix;

    Eigen::Matrix3f R;
    R.col(2) = (position - target).normalized();
    R.col(0) = up.cross(R.col(2)).normalized();
    R.col(1) = R.col(2).cross(R.col(0));
    view_matrix.topLeftCorner<3, 3>() = R.transpose();
    view_matrix.topRightCorner<3, 1>() = -R.transpose() * position;
    view_matrix.row(3) << 0, 0, 0, 1;

    return view_matrix;
}

Eigen::Matrix4f perspective(float fov_y, float aspect_ratio, float z_near, float z_far) {
    float y_scale = 1.0f / tanf(fov_y / 2.0f);
    float x_scale = y_scale / aspect_ratio;
    Eigen::Matrix4f perspective_matrix;

    perspective_matrix << x_scale, 0, 0, 0,
                          0, y_scale, 0, 0,
                          0, 0, - (z_far + z_near) / (z_far - z_near), - 2 * z_far * z_near / (z_far - z_near),
                          0, 0, -1, 0;
    return perspective_matrix;
}

Eigen::Matrix4f orthographic(float left, float right, float bottom, float top, float near, float far) {
    Eigen::Matrix4f orthographic_matrix;

    orthographic_matrix << 2.0f / (right - left), 0, 0, - (right + left) / (right - left),
                           0, 2.0f / (top - bottom), 0, - (top + bottom) / (top - bottom),
                           0, 0, - 2.0f / (far - near), - (far + near) / (far - near),
                           0, 0, 0, 1;
    return orthographic_matrix;
}
