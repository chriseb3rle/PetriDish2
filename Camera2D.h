#pragma once

class Camera2d {
public:
    Camera2d() {
        rotation_ = 0.0f;
        cameraX_ = 0.0f;
        cameraY_ = 0.0f;
        zoom_ = 1.0f;
        isDragging_ = false;
        lastMouseX_ = 0.0f;
        lastMouseY_ = 0.0f;
        screenWidth_ = screenWidth;
        screenHeight_ = screenHeight;
    }
    void setWindowSize(double width, double height) {
        screenWidth_ = width;
        screenHeight_ = height;
    }
    float getCameraX() const;
    float getCameraY() const;
    glm::vec2 getPosition() const;
    void setCameraPosition(double x, double y);
    void zoom(double factor);
    void apply();
    void handleMouseInput(GLFWwindow* window, double xpos, double ypos);
    void handleMouseButton(GLFWwindow* window, int button, int action, int mods);
    void setZoom(double zoomLevel) {
        zoom_ = 1.0f / (zoomLevel * 0.01f);
    }

    void screenToWorld(double screenX, double screenY, double& worldX, double& worldY) const {
        // Undo rotation transformation
        double unrotatedX = (screenX - screenWidth_ / 2.0) / zoom_;
        double unrotatedY = (screenHeight_ / 2.0 - screenY) / zoom_;

        // Undo translation
        double translatedX = unrotatedX + cameraX_;
        double translatedY = unrotatedY + cameraY_;

        worldX = translatedX;
        worldY = translatedY;
    }

private:
    double rotation_;
    double cameraX_;
    double cameraY_;
    double zoom_;
    bool isDragging_;
    double lastMouseX_;
    double lastMouseY_;
    double screenWidth_;
    double screenHeight_;
};