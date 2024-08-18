//
//  main.cpp
//  opengl环境
//
//  Created by Stan on 2022/12/8.
//

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = SCR_WIDTH*3024.0/4032.0;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;    // 当前帧和上一帧之间的时间
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.0f, 0.0f, 0.0f);
GLFWwindow* initWindow();
unsigned int loadTexture(const char *path);
void createVBOVAO(unsigned int &cubeVAO,unsigned int &lightCubeVAO,unsigned int &VBO);
std::vector<glm::vec3> getCubePositions();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

int main(int argc, const char * argv[]) {
    
    GLFWwindow* window = initWindow();
    if(!window){
        return -1;
    }
    
    unsigned int cubeVAO = 0;
    unsigned int lightCubeVAO = 0;
    unsigned int VBO = 0;
    createVBOVAO(cubeVAO,lightCubeVAO,VBO);
    
    unsigned int diffuseMap = loadTexture("./src/container2.png");
    unsigned int specularMap = loadTexture("./src/container2_specular.png");
    
    
    Shader lightingShader("./shaders/colors-vertex.vs", "./shaders/colors-fragment.fs");
    Shader lightCubeShader("./shaders/lamp-vertexvs.vs", "./shaders/lamp-fragment.fs");
    
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 5);
    
    
    
    glEnable(GL_DEPTH_TEST);
    //glfw 保活
    while(!glfwWindowShouldClose(window))//glfwWindowShouldClose 关闭事件
    {
        
        
        /*每帧时间逻辑 硬件设备也好这个值也小，用时间来表示步长，这移动数度在每个硬件平衡
         eg：好的设备1s绘制两次 deltaTime = 0.5，差的设备绘制一次 deltaTime = 1;
         cameraPos = 1s * (绘制次数) * （自己定义的常亮d）* deltaTime
         好的设备  cameraPos = 1s * 2 * d * 0.5 = 1d
         差的设备  cameraPos = 1s * 1 * d * 1.0 = 1d
         图像在好的和差的设备这样移动的距离相等
         */
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // input
        // -----
        processInput(window);
        
        float x = sin(glfwGetTime()) * 3;
        float y = sin(glfwGetTime()) * 3;
        float z = cos(glfwGetTime()) * 3;
        
        
                if(z < 0){
                    z = z * (-1);
                }
        
        lightPos = glm::vec3(x, y, z);
        
        // render
        // ------
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("light.position", lightPos);
        lightingShader.setVec3("light.direction", glm::vec3(0,0,-1));
        lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(5.0f)));
        lightingShader.setVec3("viewPos", camera.Position);
        
        
        // light properties 灯属性
        glm::vec3 lightColor(1.0f,1.0f,1.0f);
        //        lightColor.r = static_cast<float>(sin(glfwGetTime()));
        //        lightColor.y = static_cast<float>(sin(glfwGetTime()));
        //        lightColor.z = static_cast<float>(sin(glfwGetTime()));
        
        
        
        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.9f); // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.5f); // low influence
        
        lightingShader.setVec3("light.ambient", ambientColor); //环境
        lightingShader.setVec3("light.diffuse", diffuseColor); //慢反射
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);//镜面
        
        //衰减
        lightingShader.setFloat("light.constant", 1.0f); //常量
        lightingShader.setFloat("light.linear", 0.09f);  // 直线值
        lightingShader.setFloat("light.quadratic", 0.032f); //二次项
        
        
        // material properties 材质属性
        lightingShader.setVec3("material.ambient", 1.0f, 1.0f, 1.0f);//环境
        lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);//慢反射
        lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // 镜面 specular lighting doesn't have full effect on this object's material
        lightingShader.setFloat("material.shininess", 64.0f);
        
        
        
        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        
        
        
        //投影变换矩阵
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        view = glm::translate(view, glm::vec3(0,0,-5));
//        view = glm::rotate(view, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        
        
        //物体
        for (unsigned int i = 0; i < 10; i++){
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, getCubePositions()[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        
        
        //灯泡
        lightCubeShader.use();
        lightCubeShader.setVec3("lampColor",lightColor);
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);//移动模型
        model = glm::rotate(model, glm::radians(45.0f),glm::vec3(1,1,0));//模型旋转
        model = glm::scale(model, glm::vec3(0.1f)); // a smaller cube
        lightCubeShader.setMat4("model", model);
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

//顶点着色器发送数据 和数据说明
void createVBOVAO(unsigned int &cubeVAO,unsigned int &lightCubeVAO,unsigned int &VBO){
    
    float vertices[] = {
        // -------顶点------     ----顶点法向量----    -纹理坐标-
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//导入数据
    
    glBindVertexArray(cubeVAO);
    // position attribute 顶点向量
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute 法向量
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coords 纹理坐标
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    //灯泡顶点向量
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
//加载纹理
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = 0;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    
    return textureID;
}

std::vector<glm::vec3> getCubePositions() {
    return {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
}








GLFWwindow* initWindow(){
    //配置glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT, "sc window", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    //当前上下文设置为glfw
    glfwMakeContextCurrent(window);
    
    //初始化glad管理OpenGL函数地址
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//窗口改变时回调
    glfwSetCursorPosCallback(window, mouse_callback);//鼠标移动
    glfwSetScrollCallback(window, scroll_callback);//鼠标滚轮
    // 告诉GLFW捕获我们的鼠标
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE ) == GLFW_PRESS)
        camera.ProcessKeyboard(UPWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
