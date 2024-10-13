//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <iostream>
//
//// Shader source code (for rendering texture to a quad)
//const char* vertexShaderSource = R"(
//#version 330 core
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec2 aTexCoord;
//
//out vec2 TexCoord;
//
//uniform mat4 projection;
//
//void main() {
//    gl_Position = projection * vec4(aPos, 1.0);
//    TexCoord = aTexCoord;
//}
//)";
//
//const char* fragmentShaderSource = R"(
//#version 330 core
//out vec4 FragColor;
//
//in vec2 TexCoord;
//
//uniform sampler2D subwindowTexture;
//
//void main() {
//    FragColor = texture(subwindowTexture, TexCoord);
//}
//)";
//
//// Subwindow properties
//glm::vec2 subwindowPosition(100, 100);
//glm::vec2 subwindowSize(200, 200);
//bool isDragging = false;
//glm::vec2 dragOffset(0, 0);
//
//// FBO for subwindow
//GLuint subwindowFBO, subwindowTexture;
//GLuint quadVAO, quadVBO;
//GLuint shaderProgram;
//
//void initSubwindowFBO() {
//    glGenFramebuffers(1, &subwindowFBO);
//    glBindFramebuffer(GL_FRAMEBUFFER, subwindowFBO);
//
//    glGenTextures(1, &subwindowTexture);
//    glBindTexture(GL_TEXTURE_2D, subwindowTexture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, subwindowSize.x, subwindowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, subwindowTexture, 0);
//
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}
//
//void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
//    if (button == GLFW_MOUSE_BUTTON_LEFT) {
//        double xpos, ypos;
//        glfwGetCursorPos(window, &xpos, &ypos);
//
//        if (action == GLFW_PRESS) {
//            if (xpos >= subwindowPosition.x && xpos < subwindowPosition.x + subwindowSize.x &&
//                ypos >= subwindowPosition.y && ypos < subwindowPosition.y + subwindowSize.y) {
//                isDragging = true;
//                dragOffset = glm::vec2(xpos - subwindowPosition.x, ypos - subwindowPosition.y);
//            }
//        }
//        else if (action == GLFW_RELEASE) {
//            isDragging = false;
//        }
//    }
//}
//
//void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
//    if (isDragging) {
//        subwindowPosition = glm::vec2(xpos - dragOffset.x, ypos - dragOffset.y);
//    }
//}
//
//void renderSubwindow() {
//    glBindFramebuffer(GL_FRAMEBUFFER, subwindowFBO);
//    glViewport(0, 0, subwindowSize.x, subwindowSize.y);
//
//    // Clear and render subwindow content here
//    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    // Render subwindow content...
//
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}
//
//void renderQuad() {
//    glBindVertexArray(quadVAO);
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    glBindVertexArray(0);
//}
//
//void renderMainWindow(GLFWwindow* window) {
//    int width, height;
//    glfwGetFramebufferSize(window, &width, &height);
//    glViewport(0, 0, width, height);
//
//    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    // Render subwindow texture as a quad
//    glUseProgram(shaderProgram);
//
//    glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
//    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
//    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
//
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, subwindowTexture);
//    glUniform1i(glGetUniformLocation(shaderProgram, "subwindowTexture"), 0);
//
//    // Transform the quad to match the subwindow position and size
//    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(subwindowPosition, 0.0f));
//    model = glm::scale(model, glm::vec3(subwindowSize, 1.0f));
//    GLint modelLoc = glGetUniformLocation(shaderProgram, "projection");
//    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
//
//    renderQuad();
//}
//
//void compileShaders() {
//    // Compile vertex shader
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//    // Check for shader compile errors
//    int success;
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        char infoLog[512];
//        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//
//    // Compile fragment shader
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    // Check for shader compile errors
//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        char infoLog[512];
//        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//
//    // Link shaders
//    shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    // Check for linking errors
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if (!success) {
//        char infoLog[512];
//        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//    }
//
//    // Delete shaders after linking
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//}
//
//void initQuad() {
//    float quadVertices[] = {
//        // positions        // texture coords
//        0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
//        1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
//        0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
//        1.0f, 0.0f, 0.0f,   1.0f, 0.0f
//    };
//
//    glGenVertexArrays(1, &quadVAO);
//    glGenBuffers(1, &quadVBO);
//
//    glBindVertexArray(quadVAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
//
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//
//    glBindVertexArray(0);
//}
//
//int main() {
//    // Initialize GLFW
//    if (!glfwInit()) {
//        std::cout << "Failed to initialize GLFW" << std::endl;
//        return -1;
//    }
//
//    // Set OpenGL version (3.3) and compatibility mode
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    GLFWwindow* window = glfwCreateWindow(800, 600, "Subwindow Example", NULL, NULL);
//    if (!window) {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//
//    glfwMakeContextCurrent(window);
//
//    // Initialize GLEW
//    glewExperimental = GL_TRUE;
//    if (glewInit() != GLEW_OK) {
//        std::cout << "Failed to initialize GLEW" << std::endl;
//        return -1;
//    }
//
//    // Set up viewport
//    glViewport(0, 0, 800, 600);
//
//    // Set up OpenGL state
//    glEnable(GL_DEPTH_TEST);
//
//    // Initialize framebuffer, quad, and shaders
//    initSubwindowFBO();
//    initQuad();
//    compileShaders();
//
//    // Set up mouse callbacks
//    glfwSetMouseButtonCallback(window, mouseButtonCallback);
//    glfwSetCursorPosCallback(window, cursorPosCallback);
//
//    // Main loop
//    while (!glfwWindowShouldClose(window)) {
//        renderSubwindow();
//        renderMainWindow(window);
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // Cleanup
//    glDeleteVertexArrays(1, &quadVAO);
//    glDeleteBuffers(1, &quadVBO);
//    glDeleteProgram(shaderProgram);
//    glDeleteTextures(1, &subwindowTexture);
//    glDeleteFramebuffers(1, &subwindowFBO);
//
//    glfwTerminate();
//    return 0;
//}
