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
//缓冲中数据说明
void createVBOVAO(unsigned int &cubeVAO,unsigned int &lightCubeVAO,unsigned int &VBO){
    
    float vertices[] = {
        //  -------顶点------     ----顶点法向量----
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(cubeVAO);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    
    //灯泡顶点数据
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
////纹理创建和加载
//void loadTexture(unsigned int &texture1,unsigned int &texture2){
//    //创建纹理
////    unsigned int texture1;
//    glGenTextures(1, &texture1);//“1”纹理数量
//    glBindTexture(GL_TEXTURE_2D, texture1);
//    //设置环绕
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//    //设置过滤
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    // 加载并生成纹理
//    int width, height, nrChannels;
//    stbi_set_flip_vertically_on_load(true); //翻转Y轴
//    unsigned char *data = stbi_load("./src/pipi.png", &width, &height, &nrChannels, 0);
//    if (data)
//    {
//        //加载纹理 GL_RGBA这个参数一定要设置对
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//        //自动生成所有需要的多级渐远纹理
//        glGenerateMipmap(GL_TEXTURE_2D);
//    }
//    else
//    {
//        std::cout << "Failed to load texture1" << std::endl;
//    }
//    stbi_image_free(data);
//
//
//    //创建纹理
////    unsigned int texture2;
//    glGenTextures(1, &texture2);//“1”纹理数量
//    glBindTexture(GL_TEXTURE_2D, texture2);
//    //设置环绕
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    //设置过滤
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    // 加载并生成纹理
//    stbi_set_flip_vertically_on_load(true);
//    data = stbi_load("./src/box.png", &width, &height, &nrChannels, 0);
//    if (data)
//    {
//        //加载纹理 GL_RGBA这个参数一定要设置对
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//        //自动生成所有需要的多级渐远纹理
//        glGenerateMipmap(GL_TEXTURE_2D);
//    }
//    else
//    {
//        std::cout << "Failed to load texture2" << std::endl;
//    }
//    stbi_image_free(data);
//}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
int main(int argc, const char * argv[]) {
    
    
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
        return -1;
    }
    //当前上下文设置为glfw
    glfwMakeContextCurrent(window);
    
    //初始化glad管理OpenGL函数地址
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//窗口改变时回调
    glfwSetCursorPosCallback(window, mouse_callback);//鼠标移动
    glfwSetScrollCallback(window, scroll_callback);//鼠标滚轮
    // 告诉GLFW捕获我们的鼠标
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    
    
    //    unsigned int texture1 = 0,texture2 = 0;
    //    loadTexture(texture1, texture2);
    
    //要使用相对路径要设置一下Xcode
    //    Shader ourShader("./shaders/vertex.vs",
    //                  "./shaders/fragment.fs");
    //
    //    unsigned int shaderProgram = ourShader.ID;
    //    ourShader.use();
    
    
    //    //片段作色器,输入位置
    //    ourShader.setInt("texture1", 0);
    //    ourShader.setInt("texture2", 5);
    
    glEnable(GL_DEPTH_TEST);
    
    Shader lightingShader("./shaders/colors-vertex.vs", "./shaders/colors-fragment.fs");
    Shader lightCubeShader("./shaders/lamp-vertexvs.vs", "./shaders/lamp-fragment.fs");
    
    
    
    unsigned int cubeVAO = 0;
    unsigned int lightCubeVAO = 0;
    unsigned int VBO = 0;
    createVBOVAO(cubeVAO,lightCubeVAO,VBO);
    
    
    
    
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
        
        float x = sin(glfwGetTime());
        float y = sin(glfwGetTime());
        if(y <= 0.2){
            y = 0.2;
        }
        lightPos = glm::vec3(x, y, 0.0);
        
        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("light.position", lightPos); //灯的位置
        lightingShader.setVec3("viewPos", camera.Position); //眼睛位置
        
        
        // light properties 灯属性
        glm::vec3 lightColor;
        lightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0));
        lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7));
        lightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3));
        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
        
        lightingShader.setVec3("light.ambient", ambientColor); //环境
        lightingShader.setVec3("light.diffuse", diffuseColor); //慢反射
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);//镜面
        
    
        // material properties 材质属性
        lightingShader.setVec3("material.ambient", 1.0f, 1.0f, 1.0f);//环境
        lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);//慢反射
        lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // 镜面 specular lighting doesn't have full effect on this object's material
        lightingShader.setFloat("material.shininess", 32.0f);
        
        
        
        
        
        
        
        
        
        //投影变换矩阵
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        

        //物体
        glm::mat4 model = glm::mat4(1.0f);                               //初始化变换矩阵
        model = glm::translate(model, glm::vec3(0,-0.4,0));           //移动
        //        model = glm::rotate(model, (float)glfwGetTime(),glm::vec3(1,1,0));//旋转
        model = glm::rotate(model, glm::radians(40.0f),glm::vec3(1,1,0));
        model = glm::scale(model, glm::vec3(0.8f));                      //缩放
        lightingShader.setMat4("model", model);                          //发送给着色器
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        //灯泡
        lightCubeShader.use();
        lightCubeShader.setVec3("lampColor",lightColor);
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);//移动模型
        model = glm::rotate(model, glm::radians(45.0f),glm::vec3(1,1,0));//模型旋转
        model = glm::scale(model, glm::vec3(0.04f)); // a smaller cube
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
