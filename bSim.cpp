#include "engine.h"

double bound = 3000;
double originBound = 0;
double boundDim = originBound + bound;
double friction = 0.5;
bool checkWithOther = true;

class Particle {
public:
    // Default constructor
    Particle() : radius(1), mass(1), id(0) {}
    // Constructor
    Particle(double m, double r, double xp, double yp, double vX, double vY) {
        velocity[0] = vX;
        velocity[1] = vY;
        oldPos[0] = xp - 0.0;
        oldPos[1] = yp + 0.0;
        currentPos[0] = xp;
        currentPos[1] = yp;
        newPos[0] = xp;
        newPos[1] = yp;
        force[0] = 0;
        force[1] = accelerationGravity;
        acceleration[0] = 0;
        acceleration[1] = 0;
        radius = r;
        mass = m;
    }
    double orientation = 0.4;
    double oldPos[2] = { 0,0 };
    double currentPos[2] = { 0,0 };
    double newPos[2] = { 0,0 };
    double velocity[2] = { 0,0 };
    double acceleration[2] = { 0,0 };
    double force[2] = { 0,0 };
    // variables
    double radius;
    double mass;
    int id = 0;
    double accelerationGravity = -0.10;
    // public functions..
    void show() {
        Graphics::circleOutline(currentPos[0], currentPos[1], radius, COLOR_KHAKI,10);
    }
    void update(double deltaTime) {
        // calculate acceleration by dividing the forces acting on the particle by it's mass
        acceleration[0] = force[0] / mass;
        acceleration[1] = force[1] / mass;
        // Calculate where the new position of the ball will be ~~ (gravity is kept seperate from acceleration for simplicity)
        newPos[0] = 2 * currentPos[0] - oldPos[0] + acceleration[0] * deltaTime * deltaTime;
        newPos[1] = 2 * currentPos[1] - oldPos[1] + acceleration[1] + accelerationGravity * deltaTime * deltaTime;
        // change the old position to the currentPosition
        oldPos[0] = currentPos[0];
        oldPos[1] = currentPos[1];
        // Applys this new position to the balls current position
        currentPos[0] = newPos[0];
        currentPos[1] = newPos[1];
        // calculate velocity based on old position to new positon
        velocity[0] = (currentPos[0] - oldPos[0]);
        velocity[1] = (currentPos[1] - oldPos[1]);
    }
    void boundToSquare(double bound) {
        // left
        if (currentPos[0] - radius < originBound) {
            currentPos[0] = originBound + radius; // Place the particle at the boundary
            oldPos[0] = currentPos[0] + velocity[0] * friction;
        }
        // right
        else if (currentPos[0] + radius > boundDim) {
            currentPos[0] = originBound + bound - radius; // Place the particle at the boundary
            oldPos[0] = currentPos[0] + velocity[0] * friction;
        }

        // bottom
        if (currentPos[1] - radius < originBound) {
            currentPos[1] = originBound + radius; // Place the particle at the boundary
            oldPos[1] = currentPos[1] + velocity[1] * friction;
        }
        // top
        else if (currentPos[1] + radius > boundDim) {
            currentPos[1] = originBound + bound - radius; // Place the particle at the boundary
            oldPos[1] = currentPos[1] + velocity[1] * friction;
        }
    }
private:
};
std::vector<Particle> balls;

bool isColl(Particle& b1, Particle& b2) {
    if (b1.id == b2.id) {
        return false;
    }
    double dx = b2.currentPos[0] - b1.currentPos[0];
    double dy = b2.currentPos[1] - b1.currentPos[1];
    if (std::sqrt(dx * dx + dy * dy) <= b1.radius + b2.radius) {
        return true;
    }
    return false;
}
void resolveCollisions(Particle& b1, Particle& b2) {
    if (b1.id == b2.id) {
        return;
    }
    // Calculate the relative velocity
    double dx = b2.currentPos[0] - b1.currentPos[0];
    double dy = b2.currentPos[1] - b1.currentPos[1];
    double dvx = b2.velocity[0] - b1.velocity[0];
    double dvy = b2.velocity[1] - b1.velocity[1];
    double dotProduct = dx * dvx + dy * dvy;
    double dist = std::sqrt(dx * dx + dy * dy);
    double normalX = dx / dist;
    double normalY = dy / dist;
    // Separate the particles to avoid overlap
    double overlap = (b1.radius + b2.radius) - dist;
    double separationX = 0.5 * overlap * normalX;
    double separationY = 0.5 * overlap * normalY;
    //apply velocity
    b1.currentPos[0] -= separationX;
    b1.currentPos[1] -= separationY;
    b2.currentPos[0] += separationX;
    b2.currentPos[1] += separationY;
}
void resolveCollisionsID(int id1, int id2) {
    Particle* b1 = nullptr;
    Particle* b2 = nullptr;

    // Find the particles with the specified IDs
    for (int i = 0; i < balls.size(); i++) {
        if (balls[i].id == id1) {
            b1 = &balls[i];
        }
        else if (balls[i].id == id2) {
            b2 = &balls[i];
        }

        // If both particles are found, break out of the loop
        if (b1 != nullptr && b2 != nullptr) {
            break;
        }
    }

    // Check if both particles were found
    if (b1 != nullptr && b2 != nullptr) {
        double dx = b2->currentPos[0] - b1->currentPos[0];
        double dy = b2->currentPos[1] - b1->currentPos[1];
        double dist = std::sqrt(dx * dx + dy * dy);
        double overlap = (b1->radius + b2->radius) - dist;

        double contactAngle = std::atan2(dy, dx);
        double cosAngle = std::cos(contactAngle);
        double sinAngle = std::sin(contactAngle);

        double displacement1 = 0.5 * overlap * (b2->mass / (b1->mass + b2->mass));
        double displacement2 = 0.5 * overlap * (b1->mass / (b1->mass + b2->mass));

        b1->currentPos[0] -= (displacement1 * cosAngle);
        b1->currentPos[1] -= (displacement1 * sinAngle);
        b2->currentPos[0] += (displacement2 * cosAngle);
        b2->currentPos[1] += (displacement2 * sinAngle);
    }
}
void nSquared(std::vector<Particle>& balls) {
    for (int a = 0; a < balls.size(); a++) {
        for (int b = 0; b < balls.size(); b++) {
            if (isColl(balls[a], balls[b])) {
                resolveCollisions(balls[a], balls[b]);
            }
        }
    }
}
void attract(Particle& b1, Particle& b2) {
    double dx = b2.currentPos[0] - b1.currentPos[0];
    double dy = b2.currentPos[1] - b1.currentPos[1];
    double distSquared = dx * dx + dy * dy;
    double dist = std::sqrt(distSquared);
    // Calculate gravitational force magnitude using Newton's law of gravitation
    double fgN = bigG * b1.mass * b2.mass;
    double fgD = distSquared;
    double fg = fgN / fgD;
    // Calculate gravitational force components
    double fx = fg * (dx / dist);
    double fy = fg * (dy / dist);
    // Apply gravitational force to the balls' velocities
    b1.velocity[0] += fx / b1.mass;
    b1.velocity[1] += fy / b1.mass;
    b2.velocity[0] -= fx / b2.mass;
    b2.velocity[1] -= fy / b2.mass;
}

class GridPartition {
public:
    GridPartition(double* boundary, int gridSize) {
        this->boundary[0] = boundary[0];
        this->boundary[1] = boundary[1];
        this->boundary[2] = boundary[2];
        this->boundary[3] = boundary[3];
        this->size = gridSize;
        // Calculate the width and height of each grid cell
        cellWidth = (boundary[2] - boundary[0]) / size;
        cellHeight = (boundary[3] - boundary[1]) / size;
        // Compute inverse cell width and height to avoid redundant calculations
        invCellWidth = 1.0 / cellWidth;
        invCellHeight = 1.0 / cellHeight;
        // Initialize the grid cells as a 1D vector
        grids.resize(size * size);
    }

    bool insert(Particle& particle) {
        // Determine which grid cell the particle belongs to
        int col = static_cast<int>(std::floor((particle.currentPos[0] - boundary[0]) * invCellWidth));
        int row = static_cast<int>(std::floor((particle.currentPos[1] - boundary[1]) * invCellHeight));
        // Check if the particle is within the grid boundaries
        if (row >= 0 && row < size && col >= 0 && col < size) {
            grids[row * size + col].push_back(&particle);
            return true;
        }
        return false;
    }

    void collision() {
        // Resolve collisions within the same grid cell
        for (int i = 0; i < grids.size(); i++) {
            resolveCollisionsInCell(grids[i]);
        }

        // Resolve collisions between particles in neighboring grids
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                resolveNeighborCollisions(i, j);
            }
        }
    }

    void resolveCollisionsInCell(std::vector<Particle*>& cell) {
        // Loop through particles in a single grid cell and check for collisions
        for (int a = 0; a < cell.size(); a++) {
            for (int b = a + 1; b < cell.size(); b++) {
                Particle& particleA = *(cell[a]);
                Particle& particleB = *(cell[b]);

                // Check if particleA and particleB collide
                if (isColl(particleA, particleB)) {
                    // Resolve the collision (adjust velocities or positions)
                    resolveCollisions(particleA, particleB);
                }
            }
        }
    }
    void resolveNeighborCollisions(int row, int col) {
        // Loop through neighboring cells and check for collisions
        for (int xOffset = -1; xOffset <= 1; xOffset++) {
            for (int yOffset = -1; yOffset <= 1; yOffset++) {
                int neighborRow = row + yOffset;
                int neighborCol = col + xOffset;

                // Check if the neighbor coordinates are within bounds
                if (neighborRow >= 0 && neighborRow < size && neighborCol >= 0 && neighborCol < size) {
                    std::vector<Particle*>& currentCell = grids[row * size + col];
                    std::vector<Particle*>& neighborCell = grids[neighborRow * size + neighborCol];

                    // Use a spatial data structure or optimized algorithm for collision detection
                    resolveCollisionsBetweenCells(currentCell, neighborCell);
                }
            }
        }
    }
    void resolveCollisionsBetweenCells(std::vector<Particle*>& cellA, std::vector<Particle*>& cellB) {
        // Loop through all pairs of particles in cellA and cellB
        for (int a = 0; a < cellA.size(); a++) {
            for (int b = 0; b < cellB.size(); b++) {
                Particle& particleA = *(cellA[a]);
                Particle& particleB = *(cellB[b]);

                // Check if particleA and particleB collide
                if (isColl(particleA, particleB)) {
                    // Resolve the collision (adjust velocities or positions)
                    resolveCollisions(particleA, particleB);
                }
            }
        }
    }
    void remove() {
        // Clear all particle lists in grid cells
        for (int i = 0; i < grids.size(); i++) {
            grids[i].clear();
        }
    }
    void update(std::vector<Particle>& particles) {
        remove();
        // Insert particles into grid cells
        for (int i = 0; i < particles.size(); i++) {
            insert(particles[i]);
        }

        collision();
    }
    void draw() {
        // Visualize the grid cells using Graphics::rectangleOutlinePoints
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                double x1 = boundary[0] + col * cellWidth;
                double y1 = boundary[1] + row * cellHeight;
                double x2 = x1 + cellWidth;
                double y2 = y1 + cellHeight;

                // Draw a red outline rectangle for each grid cell
                Graphics::rectangleOutlinePoints(x1, y1, x2, y2, COLOR_RED);
            }
        }
    }

private:
    double boundary[4];
    int size;
    double cellWidth;
    double cellHeight;
    double invCellWidth;  // Precomputed inverse cell width
    double invCellHeight; // Precomputed inverse cell height

    // 1D vector to represent grid cells
    std::vector<std::vector<Particle*>> grids;

    // Use a hash table or quadtree for efficient collision detection
    std::unordered_map<int, std::vector<Particle*>> spatialHash;
};
double boundary[4] = { originBound, originBound, bound, bound };
int gl = ((bound / 10) / 2) - 5;
GridPartition grid(boundary, gl);

void BSIM::display(GLFWwindow& window, Camera2d& cam)
{
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT);
    // draw the boundary of the physics simulation
    Graphics::rectangleOutline(originBound - 10, originBound - 10, bound + 20, bound + 20, COLOR_WHITE);
  //  grid.draw();
    for (int i = 0; i < balls.size(); i++) {
        balls[i].show();
    }
    // Swap buffers
    glfwSwapBuffers(&window);
}
void BSIM::update(GLFWwindow& window, float deltaTime, Camera2d& cam) {
    int numSubsteps = 1; // Adjust the number of substeps as needed
    // Calculate substep size
    double substepDeltaTime = deltaTime / numSubsteps;
    // Update particle positions and velocities
    for (int substep = 0; substep < numSubsteps; substep++) {
        for (int i = 0; i < balls.size(); i++) {
            balls[i].update(substepDeltaTime);
            balls[i].boundToSquare(bound);
        }
            grid.update(balls);
    }
     // Apply camera transformation
    cam.apply();
}

int idn = 1;
void createNewBallsInCircle(Camera2d& cam, double xp, double yp, int numBalls) {
    // Seed the random number generator
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Create a random number generator for the size within the range [10, 100]
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> sizeDistribution(3.0, 8.0);

    // Convert screen coordinates to simulation coordinates
    double worldX, worldY;
    cam.screenToWorld(xp, yp, worldX, worldY);

    // Define the radius of the circle
    double circleRadius = 90.0; // Adjust this as needed

    // Initialize ball properties
    //double size = sizeDistribution(gen); // Adjust this as needed
    double size = 9;
    double vx = 0.0;
    double vy = 0.0;
    bool collides = false;

    for (int i = 0; i < numBalls; ++i) {
        // Generate a random angle and radius within the circle
        double randomAngle = 2.0 * std::acos(-1.0) * (static_cast<double>(std::rand()) / RAND_MAX);
        double randomRadius = circleRadius * std::sqrt(static_cast<double>(std::rand()) / RAND_MAX);

        // Calculate the position of the new ball in polar coordinates
        double ballX = worldX + randomRadius * std::cos(randomAngle);
        double ballY = worldY + randomRadius * std::sin(randomAngle);

        if (checkWithOther) {
            // Check for collision with existing balls
            for (const Particle& existingBall : balls) {
                double dx = ballX - existingBall.currentPos[0];
                double dy = ballY - existingBall.currentPos[1];
                double distance = std::sqrt(dx * dx + dy * dy);
                double collisionRadius = size + existingBall.radius; // Adjust this as needed
                if (distance < collisionRadius) {
                    collides = true;
                    break;
                }
            }
        }

        // Check for collision with bounds
        if (ballX - size < originBound || ballX + size > boundDim ||
            ballY - size < originBound || ballY + size > boundDim) {
            collides = true;
        }

        // Create the ball
        if (!collides) {
            Particle newBall(size, size, ballX, ballY, vx, vy);
            newBall.id = idn;
            balls.push_back(newBall);
            idn++;
        }

        // Reset the collides flag for the next ball
        collides = false;
    }
}

int BSIM::game(int width, int height, Camera2d& cam)
{
    // Initialize GLFW
    glfwInit();
    // Disable VSync
    glfwSwapInterval(0);
    // Create a window and context
    GLFWwindow* window = glfwCreateWindow(width, height, "BallSim", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Set the camera object as the user pointer for the window
    glfwSetWindowUserPointer(window, &cam);
    // Make the window's context current
    glfwMakeContextCurrent(window);
    // Register callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // load audio files
    // init ball objects
    int leftMouseButtonState = GLFW_RELEASE;
    //initialize camera starting position
    cam.setCameraPosition((originBound + bound / 2), (originBound + bound / 2));
    // State variable to track if we are in ball placement mode
    bool mouseButtonDown = false;
    bool codeExecuted = false;
    // Event loop
    double prevTime = glfwGetTime();
    int frameCount = 0;
    while (!glfwWindowShouldClose(window)) {
        // Measure frame time at the beginning of the frame
        double frameStartTime = glfwGetTime();
        // Calculate delta time
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - prevTime;
        prevTime = currentTime;
        // Handle GLFW events
        glfwPollEvents();
        // ball creation
        int newLeftMouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (newLeftMouseButtonState == GLFW_PRESS && !mouseButtonDown) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            createNewBallsInCircle(cam, xpos, ypos, 500);
        }

        // phsyics sim loop
        update(*window, deltaTime, cam);
        display(*window, cam);

        // Calculate frame rate
        frameCount++;
        // Print FPS once per second
        if (frameCount >= 60) {
            double frameEndTime = glfwGetTime();
            double frameTime = frameEndTime - frameStartTime;
            double fps = 1.0 / frameTime;
            std::string fpsString = "FPS: " + std::to_string(static_cast<int>(fps));
            std::cout << '\r' << fpsString << std::flush;
            frameCount = 0;
        }
    }
    std::cout << " NUMBER OF BALLS: { " << balls.size() << " }     ";
    // Clean up
    glfwTerminate();
    return 0;
}
