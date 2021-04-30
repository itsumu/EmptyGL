//
// Created by Andrew on 3/10/2021.
//

#include <iostream>

#include <Eigen/Dense>

using std::cout;
using std::endl;
using namespace Eigen;

int main() {
    Matrix3f test;
    test << 1, 2, 3,
            5, 6, 7,
            8, 9, 10;
    cout << test << '\n';
    cout << test(0, 1) << '\n';
    return 0;
}