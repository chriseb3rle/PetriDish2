#include "engine.h"

int main() {
    // Initialize GLFW
    glfwInit();
    // Disable VSync
    glfwSwapInterval(0);
    // Create a window and context
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "BallSim", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Make the window's context current
    glfwMakeContextCurrent(window);
    // Event loop
    double prevTime = glfwGetTime();
    int frameCount = 0;
    while (!glfwWindowShouldClose(window)) {
        // Handle GLFW events
        glfwPollEvents();
    }
    // Clean up
    glfwTerminate();
    return 0;
}