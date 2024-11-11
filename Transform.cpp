//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <imgui.h>
//#include <imgui_impl_glfw.h>
//#include <imgui_impl_opengl3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <iostream>
//#include <vector>
//#include <cmath>
//
//// Define PI if not available
//#define M_PI 3.14159265358979323846f
//
//const char* vertexShaderSource = R"(
//   #version 330 core
//   layout (location = 0) in vec3 aPos;
//   uniform mat4 transform;
//   void main() {
//       gl_Position = transform * vec4(aPos, 1.0);
//   }
//)";
//
//const char* fragmentShaderSource = R"(
//   #version 330 core
//   out vec4 FragColor;
//   void main() {
//       FragColor = vec4(1.0, 0.5, 0.2, 1.0);
//   }
//)";
//
//// Global variables
//GLuint VBO, VAO, shaderProgram;
//glm::mat4 transformMatrix = glm::mat4(1.0f);
//float scale = 1.0f;
//float translateX = 0.0f, translateY = 0.0f;
//float rotation = 0.0f;
//float shearX = 0.0f, shearY = 0.0f;
//bool isFullscreen = false;
//GLFWmonitor* primaryMonitor = nullptr;
//int windowedWidth = 800, windowedHeight = 600;
//int windowedPosX = 100, windowedPosY = 100;
//int currentShape = 0; // 0 = triangle, 1 = square, 2 = circle
//int numVertices = 3;  // Initially for triangle
//
//std::vector<float> getShapeVertices(int shape) {
//    std::vector<float> vertices;
//    const float size = 0.2f;
//
//    switch (shape) {
//    case 0: // Triangle
//        vertices = {
//            -size, -size, 0.0f,
//            size, -size, 0.0f,
//            0.0f, size, 0.0f
//        };
//        numVertices = 3;
//        break;
//
//    case 1: // Square
//        vertices = {
//            -size, -size, 0.0f,  // First triangle
//            size, -size, 0.0f,
//            size, size, 0.0f,
//            -size, -size, 0.0f,  // Second triangle
//            size, size, 0.0f,
//            -size, size, 0.0f
//        };
//        numVertices = 6;
//        break;
//
//    case 2: // Circle (approximated with 32 triangles)
//    {
//        const int segments = 32;
//        const float radius = size;
//
//        // Center vertex
//        float centerX = 0.0f;
//        float centerY = 0.0f;
//
//        for (int i = 0; i < segments; i++) {
//            float theta1 = 2.0f * M_PI * float(i) / float(segments);
//            float theta2 = 2.0f * M_PI * float(i + 1) / float(segments);
//
//            // Center point
//            vertices.push_back(centerX);
//            vertices.push_back(centerY);
//            vertices.push_back(0.0f);
//
//            // First outer point
//            vertices.push_back(centerX + radius * cos(theta1));
//            vertices.push_back(centerY + radius * sin(theta1));
//            vertices.push_back(0.0f);
//
//            // Second outer point
//            vertices.push_back(centerX + radius * cos(theta2));
//            vertices.push_back(centerY + radius * sin(theta2));
//            vertices.push_back(0.0f);
//        }
//        numVertices = segments * 3;
//    }
//    break;
//    }
//    return vertices;
//}
//
//void updateShape() {
//    std::vector<float> vertices = getShapeVertices(currentShape);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//    glViewport(0, 0, width, height);
//}
//
//void toggleFullscreen(GLFWwindow* window) {
//    if (!isFullscreen) {
//        glfwGetWindowPos(window, &windowedPosX, &windowedPosY);
//        glfwGetWindowSize(window, &windowedWidth, &windowedHeight);
//        primaryMonitor = glfwGetPrimaryMonitor();
//        const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
//        glfwSetWindowMonitor(window, primaryMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
//    }
//    else {
//        glfwSetWindowMonitor(window, nullptr, windowedPosX, windowedPosY, windowedWidth, windowedHeight, 0);
//    }
//    isFullscreen = !isFullscreen;
//}
//
//void processInput(GLFWwindow* window) {
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//    static bool f11Pressed = false;
//    if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS) {
//        if (!f11Pressed) {
//            toggleFullscreen(window);
//            f11Pressed = true;
//        }
//    }
//    else {
//        f11Pressed = false;
//    }
//}
//
//void initializeGL() {
//    // Create and compile shaders
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//    // Initialize with triangle vertices
//    updateShape();
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//}
//
//void updateTransformMatrix() {
//    transformMatrix = glm::mat4(1.0f);
//    transformMatrix = glm::translate(transformMatrix, glm::vec3(translateX, translateY, 0.0f));
//    transformMatrix = glm::rotate(transformMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
//    transformMatrix = glm::scale(transformMatrix, glm::vec3(scale, scale, 1.0f));
//
//    glm::mat4 shearMatrix(1.0f);
//    shearMatrix[0][1] = shearX;
//    shearMatrix[1][0] = shearY;
//    transformMatrix = transformMatrix * shearMatrix;
//}
//
//int main() {
//    if (!glfwInit()) {
//        std::cerr << "Failed to initialize GLFW" << std::endl;
//        return -1;
//    }
//
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    GLFWwindow* window = glfwCreateWindow(windowedWidth, windowedHeight, "OpenGL Transformations", NULL, NULL);
//    if (!window) {
//        std::cerr << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//    if (glewInit() != GLEW_OK) {
//        std::cerr << "Failed to initialize GLEW" << std::endl;
//        return -1;
//    }
//
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO();
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui_ImplOpenGL3_Init("#version 330");
//    ImGui::StyleColorsDark();
//
//    glfwSetWindowPos(window, windowedPosX, windowedPosY);
//
//    initializeGL();
//
//    const char* shapes[] = { "Triangle", "Square", "Circle" };
//
//    while (!glfwWindowShouldClose(window)) {
//        processInput(window);
//
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//
//        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
//        ImGui::Begin("Transform Controls", nullptr,
//            ImGuiWindowFlags_NoMove |
//            ImGuiWindowFlags_NoResize |
//            ImGuiWindowFlags_AlwaysAutoResize);
//
//        // Shape selection combo box
//        if (ImGui::Combo("Shape", &currentShape, shapes, IM_ARRAYSIZE(shapes))) {
//            updateShape();
//        }
//
//        ImGui::Separator();
//
//        // Transform controls
//        ImGui::Text("Transformation Controls");
//        ImGui::SliderFloat("Scale", &scale, 0.1f, 10.0f);
//        ImGui::SliderFloat("Rotation", &rotation, 0.0f, 360.0f);
//        ImGui::SliderFloat("Translate X", &translateX, -2.0f, 2.0f);
//        ImGui::SliderFloat("Translate Y", &translateY, -2.0f, 2.0f);
//        ImGui::SliderFloat("Shear X", &shearX, -2.0f, 2.0f);
//        ImGui::SliderFloat("Shear Y", &shearY, -2.0f, 2.0f);
//
//        ImGui::Separator();
//
//        if (ImGui::Button("Reset All")) {
//            scale = 1.0f;
//            rotation = 0.0f;
//            translateX = translateY = 0.0f;
//            shearX = shearY = 0.0f;
//        }
//
//        ImGui::SameLine();
//        if (ImGui::Button("Toggle Fullscreen")) {
//            toggleFullscreen(window);
//        }
//
//        ImGui::Text("Press F11 to toggle fullscreen");
//
//        ImGui::End();
//
//        // Render shape
//        updateTransformMatrix();
//        glUseProgram(shaderProgram);
//        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(transformMatrix));
//
//        glBindVertexArray(VAO);
//        glDrawArrays(GL_TRIANGLES, 0, numVertices);
//
//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // Cleanup
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
//
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteProgram(shaderProgram);
//
//    glfwTerminate();
//    return 0;
//}