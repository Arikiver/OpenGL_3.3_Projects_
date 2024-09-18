//#include <GLFW/glfw3.h>
//#include <iostream>
//#include <queue>
//#include <vector>
//
//const int WIDTH = 800;
//const int HEIGHT = 600;
//std::vector<std::vector<int>> grid(WIDTH, std::vector<int>(HEIGHT, 0)); // Example grid
//int fillColor = 1; // Color to fill
//int borderColor = -1; // Color representing the border
//
//void floodFill(int x, int y, int newColor) {
//    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
//    int oldColor = grid[x][y];
//    if (oldColor == newColor || oldColor == borderColor) return; // Don't fill border or same color
//
//    std::queue<std::pair<int, int>> q;
//    q.push({ x, y });
//    grid[x][y] = newColor;
//
//    while (!q.empty()) {
//        auto current = q.front();
//        q.pop();
//
//        int cx = current.first;
//        int cy = current.second;
//
//        std::pair<int, int> directions[] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
//
//        for (const auto& direction : directions) {
//            int dx = direction.first;
//            int dy = direction.second;
//            int nx = cx + dx;
//            int ny = cy + dy;
//            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && grid[nx][ny] == oldColor) {
//                grid[nx][ny] = newColor;
//                q.push({ nx, ny });
//            }
//        }
//    }
//}
//
//void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
//    static double lastClickTime = 0;
//
//    if (button == GLFW_MOUSE_BUTTON_LEFT) {
//        if (action == GLFW_PRESS) {
//            double currentTime = glfwGetTime();
//            if (currentTime - lastClickTime < 0.3) { // 300 ms for double click
//                double xpos, ypos;
//                glfwGetCursorPos(window, &xpos, &ypos);
//                int x = static_cast<int>(xpos);
//                int y = static_cast<int>(ypos);
//                // Adjust coordinates if needed (e.g., for viewport scaling)
//                floodFill(x, y, fillColor);
//                std::cout << "Double Click Detected at (" << x << ", " << y << ")" << std::endl;
//            }
//            lastClickTime = currentTime;
//        }
//    }
//}
//
//void render() {
//    // Add code to render the grid or updated image
//    // This will depend on your specific rendering setup
//}
//
//int main() {
//    if (!glfwInit()) {
//        std::cerr << "Failed to initialize GLFW" << std::endl;
//        return -1;
//    }
//
//    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Flood Fill Example", NULL, NULL);
//    if (!window) {
//        std::cerr << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//
//    glfwMakeContextCurrent(window);
//    glfwSetMouseButtonCallback(window, mouseButtonCallback);
//
//    while (!glfwWindowShouldClose(window)) {
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        render(); // Call render to draw the grid
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    glfwDestroyWindow(window);
//    glfwTerminate();
//    return 0;
//}
