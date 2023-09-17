#pragma once
class Application
{
public:
    Application(double screen_width, double screen_height) {
        WIDTH = screen_width;
        HEIGHT = screen_height;
    }
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }
    double WIDTH;
    double HEIGHT;
private:
    GLFWwindow* window;

    VkInstance instance;

    void initWindow();

    void initVulkan();

    void mainLoop();

    void cleanup();

    void createInstance();
};

