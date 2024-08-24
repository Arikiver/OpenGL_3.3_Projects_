//#include <iostream>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <cmath>
//
//// Vertex shader source code
//const char* vertexShaderSource = R"(
//   #version 330 core
//   layout (location = 0) in vec3 aPos;
//   void main()
//   {
//       gl_Position = vec4(aPos, 1.0);
//   }
//)";
//
//// Fragment shader source code
//const char* fragmentShaderSource = R"(
//   #version 330 core
//   out vec4 FragColor;
//   uniform vec4 color;
//   void main()
//   {
//       FragColor = color;
//   }
//)";
//
//// Function to generate circle vertices
//void generateCircle(float* vertices, int numSegments, float radius, float centerX, float centerY) {
//    vertices[0] = centerX;
//    vertices[1] = centerY;
//    vertices[2] = 0.0f;
//
//    for (int i = 0; i <= numSegments; ++i) {
//        float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
//        vertices[(i + 1) * 3] = radius * cosf(theta) + centerX;
//        vertices[(i + 1) * 3 + 1] = radius * sinf(theta) + centerY;
//        vertices[(i + 1) * 3 + 2] = 0.0f;
//    }
//}
//
//bool isInsideCircle(float x, float y, float centerX, float centerY, float radius) {
//    return (x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) <= radius * radius;
//}
//
//void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
//    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
//        double xpos, ypos;
//        glfwGetCursorPos(window, &xpos, &ypos);
//
//        int width, height;
//        glfwGetWindowSize(window, &width, &height);
//
//        // Convert from window coordinates to OpenGL coordinates
//        float x = (xpos / width) * 2.0f - 1.0f;
//        float y = 1.0f - (ypos / height) * 2.0f;
//
//        // Check if the sun/moon is clicked
//        float sunX = 0.6f, sunY = 0.6f, sunRadius = 0.15f;
//        if (isInsideCircle(x, y, sunX, sunY, sunRadius)) {
//            bool* isDay = reinterpret_cast<bool*>(glfwGetWindowUserPointer(window));
//            *isDay = !(*isDay); // Toggle day/night
//        }
//    }
//}
//
//void processInput(GLFWwindow* window) {
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//}
//
//int main() {
//    // Initialize GLFW
//    if (!glfwInit()) {
//        std::cerr << "Failed to initialize GLFW" << std::endl;
//        return -1;
//    }
//
//    // Set GLFW context version and profile
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    // Create a windowed mode window and its OpenGL context
//    GLFWwindow* window = glfwCreateWindow(800, 600, "Simple House", NULL, NULL);
//    if (!window) {
//        std::cerr << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//
//    // Make the window's context current
//    glfwMakeContextCurrent(window);
//
//    // Load OpenGL function pointers using glad
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//        std::cerr << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    // Build and compile the shader program
//    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    int shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    // Define the vertices for the house base, roof, and windows
//    float houseVertices[] = {
//        // House base
//        -0.5f, -0.5f, 0.0f, // Bottom left
//         0.5f, -0.5f, 0.0f, // Bottom right
//         0.5f,  0.0f, 0.0f, // Top right
//        -0.5f,  0.0f, 0.0f, // Top left
//
//        // Roof
//        -0.6f,  0.0f, 0.0f, // Bottom left
//         0.6f,  0.0f, 0.0f, // Bottom right
//         0.0f,  0.6f, 0.0f, // Top middle
//    };
//
//    unsigned int houseIndices[] = {
//        // House base
//        0, 1, 2,
//        2, 3, 0,
//
//        // Roof
//        4, 5, 6,
//    };
//
//    // Generate the vertices for the sun/moon (circle)
//    const int numSegments = 20;
//    float sunVertices[(numSegments + 2) * 3];
//    generateCircle(sunVertices, numSegments, 0.15f, 0.6f, 0.6f);
//
//    // Generate buffers and arrays for house
//    unsigned int houseVBO, houseVAO, houseEBO;
//    glGenVertexArrays(1, &houseVAO);
//    glGenBuffers(1, &houseVBO);
//    glGenBuffers(1, &houseEBO);
//
//    glBindVertexArray(houseVAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, houseVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(houseVertices), houseVertices, GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, houseEBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(houseIndices), houseIndices, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    glBindVertexArray(0);
//
//    // Generate buffers and arrays for sun/moon
//    unsigned int sunVBO, sunVAO;
//    glGenVertexArrays(1, &sunVAO);
//    glGenBuffers(1, &sunVBO);
//
//    glBindVertexArray(sunVAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(sunVertices), sunVertices, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    glBindVertexArray(0);
//
//    bool isDay = true;
//    glfwSetWindowUserPointer(window, &isDay);
//    glfwSetMouseButtonCallback(window, mouseButtonCallback);
//
//    // Render loop
//    while (!glfwWindowShouldClose(window)) {
//        // Process input
//        processInput(window);
//
//        // Render
//        if (isDay) {
//            glClearColor(0.5f, 0.8f, 1.0f, 1.0f); // Light blue sky
//        }
//        else {
//            glClearColor(0.05f, 0.05f, 0.2f, 1.0f); // Dark blue night sky
//        }
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        // Draw house
//        glUseProgram(shaderProgram);
//        glBindVertexArray(houseVAO);
//
//        // Set colors for windows based on time of day
//        glUniform4f(glGetUniformLocation(shaderProgram, "color"), 1.0f, 1.0f, 1.0f, 1.0f); // White for house and roof
//        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
//
//        // Draw sun/moon
//        glBindVertexArray(sunVAO);
//        glUniform4f(glGetUniformLocation(shaderProgram, "color"), isDay ? 1.0f : 0.9f, isDay ? 0.9f : 0.8f, 0.0f, 1.0f); // Yellow for sun, light gray for moon
//        glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 2); // 1 center vertex + numSegments
//
//        // Swap buffers and poll IO events
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // Cleanup
//    glDeleteVertexArrays(1, &houseVAO);
//    glDeleteBuffers(1, &houseVBO);
//    glDeleteBuffers(1, &houseEBO);
//
//    glDeleteVertexArrays(1, &sunVAO);
//    glDeleteBuffers(1, &sunVBO);
//
//    glDeleteProgram(shaderProgram);
//
//    glfwDestroyWindow(window);
//    glfwTerminate();
//
//    return 0;
//}
