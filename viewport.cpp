//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <imgui.h>
//#include <imgui_impl_glfw.h>
//#include <imgui_impl_opengl3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <iostream>
//#include <cmath>
//
//// Shaders remain the same
//const char* vertexShaderSource = R"(
//    #version 330 core
//    layout (location = 0) in vec3 aPos;
//    uniform mat4 transform;
//    void main() {
//        gl_Position = transform * vec4(aPos, 1.0);
//    }
//)";
//
//const char* fragmentShaderSource = R"(
//    #version 330 core
//    out vec4 FragColor;
//    void main() {
//        FragColor = vec4(1.0, 0.5, 0.2, 1.0);
//    }
//)";
//
//// Custom Matrix class for 4x4 matrices
//class Matrix4 {
//public:
//    float m[4][4];
//
//    Matrix4() {
//        // Initialize as identity matrix
//        for (int i = 0; i < 4; i++) {
//            for (int j = 0; j < 4; j++) {
//                m[i][j] = (i == j) ? 1.0f : 0.0f;
//            }
//        }
//    }
//
//    // Matrix multiplication
//    Matrix4 operator*(const Matrix4& other) const {
//        Matrix4 result;
//        for (int i = 0; i < 4; i++) {
//            for (int j = 0; j < 4; j++) {
//                result.m[i][j] = 0.0f;
//                for (int k = 0; k < 4; k++) {
//                    result.m[i][j] += m[i][k] * other.m[k][j];
//                }
//            }
//        }
//        return result;
//    }
//
//    // Get data pointer for OpenGL
//    const float* data() const {
//        return &m[0][0];
//    }
//};
//
//// Custom transformation functions
//Matrix4 createTranslationMatrix(float x, float y, float z) {
//    Matrix4 trans;
//    trans.m[0][3] = x;
//    trans.m[1][3] = y;
//    trans.m[2][3] = z;
//    return trans;
//}
//
//Matrix4 createRotationMatrix(float angle) {
//    Matrix4 rot;
//    float rad = angle * 3.14159f / 180.0f; // Convert to radians
//    float cosA = cos(rad);
//    float sinA = sin(rad);
//
//    rot.m[0][0] = cosA;
//    rot.m[0][1] = -sinA;
//    rot.m[1][0] = sinA;
//    rot.m[1][1] = cosA;
//
//    return rot;
//}
//
//Matrix4 createScaleMatrix(float sx, float sy, float sz) {
//    Matrix4 scale;
//    scale.m[0][0] = sx;
//    scale.m[1][1] = sy;
//    scale.m[2][2] = sz;
//    return scale;
//}
//
//Matrix4 createShearMatrix(float shx, float shy) {
//    Matrix4 shear;
//    shear.m[0][1] = shx;
//    shear.m[1][0] = shy;
//    return shear;
//}
//
//// Global variables
//GLuint VBO, VAO, shaderProgram;
//Matrix4 transformMatrix;
//float scale = 1.0f;
//float translateX = 0.0f, translateY = 0.0f;
//float rotation = 0.0f;
//float shearX = 0.0f, shearY = 0.0f;
//bool isDragging = false;
//double lastX = 0.0, lastY = 0.0;
//
//enum class TransformMode {
//    NONE,
//    SCALE,
//    TRANSLATE,
//    ROTATE,
//    SHEAR
//};
//
//TransformMode currentMode = TransformMode::NONE;
//
//void updateTransformMatrix() {
//    // Create individual transformation matrices
//    Matrix4 translationMatrix = createTranslationMatrix(translateX, translateY, 0.0f);
//    Matrix4 rotationMatrix = createRotationMatrix(rotation);
//    Matrix4 scaleMatrix = createScaleMatrix(scale, scale, 1.0f);
//    Matrix4 shearMatrix = createShearMatrix(shearX, shearY);
//
//    // Combine transformations: Translation * Rotation * Scale * Shear
//    // Order matters! This order applies transformations from right to left
//    transformMatrix = translationMatrix * rotationMatrix * scaleMatrix * shearMatrix;
//}
//
//// Rest of the functions (initializeGL, mouse callbacks, etc.) remain the same
//void initializeGL() {
//    float triangleVertices[] = {
//        -0.2f, -0.2f, 0.0f,
//        0.2f, -0.2f, 0.0f,
//        0.0f, 0.2f, 0.0f
//    };
//
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
//    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//}
//
//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
//    if (ImGui::GetIO().WantCaptureMouse)
//        return;
//
//    if (button == GLFW_MOUSE_BUTTON_LEFT) {
//        if (action == GLFW_PRESS) {
//            isDragging = true;
//            glfwGetCursorPos(window, &lastX, &lastY);
//        }
//        else if (action == GLFW_RELEASE) {
//            isDragging = false;
//        }
//    }
//}
//
//void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
//    if (!isDragging || ImGui::GetIO().WantCaptureMouse)
//        return;
//
//    float deltaX = (xpos - lastX) * 0.01f;
//    float deltaY = (lastY - ypos) * 0.01f;
//
//    switch (currentMode) {
//    case TransformMode::SCALE:
//        scale += deltaY;
//        scale = std::max(0.1f, scale);
//        break;
//    case TransformMode::ROTATE:
//        rotation += deltaX * 100.0f;
//        break;
//    case TransformMode::SHEAR:
//        shearX += deltaX;
//        shearY += deltaY;
//        break;
//    case TransformMode::TRANSLATE:
//        translateX += deltaX;
//        translateY += deltaY;
//        break;
//    default:
//        break;
//    }
//
//    lastX = xpos;
//    lastY = ypos;
//    updateTransformMatrix();
//}
//
//int main() {
//    if (!glfwInit())
//        return -1;
//
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    GLFWwindow* window = glfwCreateWindow(800, 600, "Custom Matrix Transformations", NULL, NULL);
//    if (!window) {
//        glfwTerminate();
//        return -1;
//    }
//
//    glfwMakeContextCurrent(window);
//    glfwSetMouseButtonCallback(window, mouse_button_callback);
//    glfwSetCursorPosCallback(window, cursor_position_callback);
//
//    if (glewInit() != GLEW_OK)
//        return -1;
//
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui_ImplOpenGL3_Init("#version 330");
//
//    initializeGL();
//
//    while (!glfwWindowShouldClose(window)) {
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//
//        ImGui::Begin("Transform Controls");
//
//        if (ImGui::RadioButton("None", currentMode == TransformMode::NONE))
//            currentMode = TransformMode::NONE;
//        if (ImGui::RadioButton("Scale (Drag)", currentMode == TransformMode::SCALE))
//            currentMode = TransformMode::SCALE;
//        if (ImGui::RadioButton("Rotate (Drag)", currentMode == TransformMode::ROTATE))
//            currentMode = TransformMode::ROTATE;
//        if (ImGui::RadioButton("Translate (Drag)", currentMode == TransformMode::TRANSLATE))
//            currentMode = TransformMode::TRANSLATE;
//        if (ImGui::RadioButton("Shear (Drag)", currentMode == TransformMode::SHEAR))
//            currentMode = TransformMode::SHEAR;
//
//        ImGui::Separator();
//        ImGui::Text("Manual Controls");
//        ImGui::DragFloat("Scale", &scale, 0.01f, 0.1f, 10.0f);
//        ImGui::DragFloat("Rotation", &rotation, 1.0f, 0.0f, 360.0f);
//        ImGui::DragFloat("Translate X", &translateX, 0.01f, -2.0f, 2.0f);
//        ImGui::DragFloat("Translate Y", &translateY, 0.01f, -2.0f, 2.0f);
//        ImGui::DragFloat("Shear X", &shearX, 0.01f, -2.0f, 2.0f);
//        ImGui::DragFloat("Shear Y", &shearY, 0.01f, -2.0f, 2.0f);
//
//        if (ImGui::Button("Reset All")) {
//            scale = 1.0f;
//            rotation = 0.0f;
//            translateX = translateY = 0.0f;
//            shearX = shearY = 0.0f;
//        }
//
//        ImGui::End();
//
//        updateTransformMatrix();
//
//        glUseProgram(shaderProgram);
//        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"),
//            1, GL_FALSE, transformMatrix.data());
//
//        glBindVertexArray(VAO);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//
//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
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