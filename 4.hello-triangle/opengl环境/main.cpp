//
//  main.cpp
//  opengl环境
//
//  Created by Stan on 2022/12/8.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


//顶点着色器
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"//aPos 接受顶点输入
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
//片段着色器 上色
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"//颜色直接写死
    "}\n\0";

//GLSL
unsigned int createShaderPorgram(){
    // build and compile our shader program
    // ------------------------------------
   
    
    
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建顶点着色器
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //加载代码到响应的对象下
    glCompileShader(vertexShader); //编译代码
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);//代码编译的状态
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    
    
    
    
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//创建片段着色器
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); //加载代码到响应的对象下
    glCompileShader(fragmentShader);//编译代码
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    
    
    
    
    
    // link shaders
    unsigned int shaderProgram = glCreateProgram();//创建着色程序对象
    glAttachShader(shaderProgram, vertexShader);//程序添加顶点着色器
    glAttachShader(shaderProgram, fragmentShader);//程序添加片段着色器
    glLinkProgram(shaderProgram);//把着色器链接起来
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    
    
    
    
    
    //删除着色器 因为已经添加到着色程序中了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}


//缓冲中数据说明
void createVBOVAO(unsigned int &VAO,unsigned int &VBO){
    
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
         0.5f, -0.5f, 0.0f, // right
         0.0f,  0.5f, 0.0f  // top
    };
    /*
     float buffer = {
            //顶点坐标(3个一组)              //顶点颜色（3个一组）              //纹理坐标(2个一组)
         0.5f,  0.5f, 0.0f,                1.0f, 0.0f, 0.0f,               1.0f, 1.0f,  一个步长
         0.5f, -0.5f, 0.0f,                0.0f, 1.0f, 0.0f,               1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,                0.0f, 0.0f, 1.0f,               0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,                1.0f, 1.0f, 0.0f,               0.0f, 1.0f
     };
     */
    //VAO是解释VBO的内存 一个步长的数据，这里只有一个顶点数据，
//    unsigned int VAO;
    glGenVertexArrays(1, &VAO);//创建数组对象
    glBindVertexArray(VAO);//绑定VAO
    {
//        unsigned int VBO;
        glGenBuffers(1, &VBO);//1.创建缓冲对象2.缓冲一次给数据多GPU和CPU数据交换快，“1”对象的唯一标识ID
        glBindBuffer(GL_ARRAY_BUFFER, VBO);//1.把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上2,顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//1.顶点数据复制到缓冲的内存中 2.“GL_STATIC_DRAW” 静态数据

        //添加数据属性到VAO
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//1.说明顶点的属性2.“0”内存读起位置的标识和“location = 0”中的“0”对应，如果同时存储颜色，颜色可以设置为“2”，
        glEnableVertexAttribArray(0);//启用顶点属性，把顶点和它的属性链接起来，一个步长中可能有顶点颜色数据，
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
    
    
    
    unsigned int shaderProgram = createShaderPorgram();
    unsigned int VAO = 0;unsigned int VBO = 0;
    createVBOVAO(VAO,VBO);
    
    //glfw 保活
    while(!glfwWindowShouldClose(window))//glfwWindowShouldClose 关闭事件
    {
        
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);//清空颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//清空色
        
        //绘制三角形
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(window);//不断交换缓冲帧
        glfwPollEvents();//不段接受事件
        
    }
    glfwTerminate();//释放资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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
