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


//缓冲中数据说明
void createVBOVAO(unsigned int &VAO,unsigned int &VBO,unsigned int &EBO){
    
    float vertices[] = {
    //     ---- 位置 ----       - 纹理坐标 -
         0.5f,  0.5f, 0.0f,     1.0f, 1.0f,   // 右上
         0.5f, -0.5f, 0.0f,     1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,     0.0f, 1.0f    // 左上
    };
    
    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };
    
    //VAO存储属性，1个VAO可以存储多个顶点属性，和一个EBO
    glGenVertexArrays(1, &VAO);//创建数组对象
    glBindVertexArray(VAO);//绑定VAO
    {

        //数据
        glGenBuffers(1, &VBO);//生成JL对象
        glBindBuffer(GL_ARRAY_BUFFER, VBO);//绑定对象为顶点缓冲
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//拷贝数据到缓冲区
        
        //顶点索引
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        // 图像位置
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
       
        // 纹理坐标
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        
        
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);//解绑VBO
    }

    /*
     绑定和解释第二VBO说明顶点属性
     glBindBuffer(GL_ARRAY_BUFFER, VBO[2])
     、、、、、、
     */

    glBindVertexArray(0);//最后解绑VAO
    
    
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
int main(int argc, const char * argv[]) {
    
    
    //配置glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(650,650*4032.0/3024.0, "sc window", NULL, NULL);
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
    
    
    
    //创建纹理
    unsigned int texture1;
    glGenTextures(1, &texture1);//“1”纹理数量
    glBindTexture(GL_TEXTURE_2D, texture1);
    //设置环绕
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //设置过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); //翻转Y轴
    unsigned char *data = stbi_load("./src/pipi.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        //加载纹理 GL_RGBA这个参数一定要设置对
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        //自动生成所有需要的多级渐远纹理
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture1" << std::endl;
    }
    stbi_image_free(data);

    
    //创建纹理
    unsigned int texture2;
    glGenTextures(1, &texture2);//“1”纹理数量
    glBindTexture(GL_TEXTURE_2D, texture2);
    //设置环绕
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //设置过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("./src/box.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        //加载纹理 GL_RGBA这个参数一定要设置对
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        //自动生成所有需要的多级渐远纹理
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture2" << std::endl;
    }
    stbi_image_free(data);
    
    
    
    //要使用相对路径要设置一下Xcode
    Shader ourShader("./shaders/vertex.vs",
                  "./shaders/fragment.fs");
    
    unsigned int shaderProgram = ourShader.ID;
    ourShader.use();
    //片段作色器,输入位置
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 5);
    
    
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    createVBOVAO(VAO,VBO,EBO);
    
    //glfw 保活
    while(!glfwWindowShouldClose(window))//glfwWindowShouldClose 关闭事件
    {
        
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);//清空颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//清空色
        
        
        //绑定多个纹理赋值 GL_TEXTURE0....15，上面还要告诉，片段着色器位置
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        // create transformations
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        // get matrix's uniform location and set matrix
        ourShader.use();
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        
        
        //绘制三角形
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);//不断交换缓冲帧
        glfwPollEvents();//不段接受事件
        
    }
    glfwTerminate();//释放资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    
    
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
}
