//
//  main.cpp
//  opengl环境
//
//  Created by Stan on 2022/12/8.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int main(int argc, const char * argv[]) {
    
    //配置glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "sc window", NULL, NULL);
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
    
    
    //glfw 保活
    while(!glfwWindowShouldClose(window))//glfwWindowShouldClose 关闭事件
    {
        glClear(GL_COLOR_BUFFER_BIT);//清空颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//清空色
        glfwSwapBuffers(window);//不断交换缓冲帧
        glfwPollEvents();//不段接受事件
        
    }
    glfwTerminate();//释放资源
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
