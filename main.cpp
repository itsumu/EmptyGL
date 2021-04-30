#include <iostream>
#include <string>

#include <cxxopts.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "camera.h"
#include "geometry.h"
#include "scene.h"
#include "shader.h"

using std::cout;
using std::cerr;
using std::endl;
using std::shared_ptr;
using std::make_shared;

// Initialize window manager
bool initWindowManager() {
    return glfwInit();
}

auto camera = make_shared<Camera>(0, 0, 5, 0, 1, 0, 0, 0);

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset) {
    camera->processMouseScroll(static_cast<float>(y_offset));
}

void mouse_callback(GLFWwindow* window, double x, double y) {
    static bool first_click = true;
    static float last_x = 0;
    static float last_y = 0;

    if (first_click) {
        last_x = static_cast<float>(x);
        last_y = static_cast<float>(y);
        first_click = false;
    }

    float x_offset = static_cast<float>(x) - last_x;
    float y_offset = last_y - static_cast<float>(y); // reversed since y-coordinates go from bottom to top

    last_x = static_cast<float>(x);
    last_y = static_cast<float>(y);

    camera->processMouseMovement(x_offset, y_offset);
}

// Create window
shared_ptr<GLFWwindow> createWindowAndContext(const unsigned int width, const unsigned int height) {
    // OpenGL context setup
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window with OpenGL context
    shared_ptr<GLFWwindow> window(glfwCreateWindow(width, height, "ToonShading", nullptr, nullptr));
    if (window == nullptr) {
        cerr << "Failed to create GLFW window" << endl;
        return nullptr;
    }

    // Make context current for this window
    glfwMakeContextCurrent(window.get());

    // Handle window resize
    glfwSetFramebufferSizeCallback(window.get(),
                                   [](GLFWwindow *window, const int width, const int height){
                                       glViewport(0, 0, width, height);
                                   });

    // Handle mouse input
    glfwSetCursorPosCallback(window.get(), mouse_callback);
    glfwSetScrollCallback(window.get(), scroll_callback);
    glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load OpenGL library
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cerr << "Failed to initialize GLAD" << endl;
    }

    return window;
}

// Keyboard control
void processInput(GLFWwindow *window, Camera *camera, float delta_time) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->processKeyboard(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->processKeyboard(BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->processKeyboard(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->processKeyboard(RIGHT, delta_time);
}

int main(int argc, char** argv) {
    // Parse args
    cxxopts::Options options("EmptyGL");
    options.add_options()
        ("mesh", "Mesh file path", cxxopts::value<std::string>()->default_value("../data/cube/cube.obj"))
        ("vertex", "Vertex shader path", cxxopts::value<std::string>()->default_value("../shaders/empty.vert"))
        ("fragment", "Fragment shader path", cxxopts::value<std::string>()->default_value("../shaders/empty.frag"))
        ("width", "Screen resolution in width", cxxopts::value<unsigned int>()->default_value("1920"))
        ("height", "Screen resolution in height", cxxopts::value<unsigned int>()->default_value("1080"))
        ;
    auto args = options.parse(argc, argv);
    const std::string mesh_file_path = args["mesh"].as<std::string>();
    const std::string vertex_file_path = args["vertex"].as<std::string>();
    const std::string fragment_file_path = args["fragment"].as<std::string>();
    const unsigned int screen_width = args["width"].as<unsigned int>();
    const unsigned int screen_height = args["height"].as<unsigned int>();

    // Set up window and OpenGL context
    if (!initWindowManager()) {
        cerr << "Window manager initialization failed" << endl;
        return -1;
    }

    shared_ptr<GLFWwindow> window = createWindowAndContext(screen_width, screen_height);

    // Set up camera

    // Set up shaders
    glEnable(GL_DEPTH_TEST);
    auto shader = make_shared<Shader>(vertex_file_path, fragment_file_path);

    // Load model
    cout << "Loading model..." << endl;
    vector<string> mesh_file_path_list = {mesh_file_path};
    auto scene = make_shared<Scene>(mesh_file_path_list);
    cout << "Model loaded!" << endl;

    // Main loop
    float last_frame_time = 0.0f;
    while (!glfwWindowShouldClose(window.get())) {
        // Timing
        auto current_time = static_cast<float>(glfwGetTime());
        float delta_time = current_time - last_frame_time;
        last_frame_time = current_time;

        // Process user input
        processInput(window.get(), camera.get(), delta_time);

        // One render pass
            // Clear all buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Activate shader
        shader->use();

            // Projection transformation
        Eigen::Matrix4f projection_matrix = perspective(degree2Radian(camera->zoom),
                                                        (float) screen_width / (float) screen_height,
                                                        0.1f, 1000.0f);
        Eigen::Matrix4f view_matrix = camera->getViewMatrix();
        Eigen::Matrix4f model_matrix = Eigen::Matrix4f::Identity();

        shader->setMat4("model", model_matrix);
        shader->setMat4("view", view_matrix);
        shader->setMat4("projection", projection_matrix);

            // Draw
        scene->draw(shader.get());

        // New frame
        glfwPollEvents();
        glfwSwapBuffers(window.get());
    }

    // Release resources
    shader->release();

	return 0;
}