#pragma once

class Graphics {
public:
    // SHAPES
// ELLIPSE
    static void ellipse(float cx, float cy, float cx2, float cy2, float eccentricity, const float* color);
    static void ellipseOutline(float cx, float cy, float cx2, float cy2, float eccentricity, const float* color);
    // CIRCLE
    static void circle(float centerX, float centerY, float radius, const float* color, int nSeg);
    static void circleOutline(double centerX, double centerY, double radius, const float* color, int nSeg);
    // TRIANGLE
    static void triangle(float centerX, float centerY, float sideLength, const float* color);
    static void triangleOutline(float centerX, float centerY, float sideLength, const float* color);
    // RECTANGLE
    static void rectangle(double x1, double y1, double width, double height, const float* color);
    static void rectangleOutline(double x, double y, double width, double height, const float* color);
    static void rectangleOutlinePoints(double x1, double y1, double x2, double y2, const float* color);
    // LINE
    static void drawLine(float x1, float y1, float x2, float y2, const float* color);
    // FPS
    static void drawFPS(GLFWwindow* window);
private:
};