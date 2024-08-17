/* 
  vertex.strings
  opengl环境

  Created by Stan on 2024/8/12.
  
*/
#version 330 core
layout (location = 0) in vec3 aPos;   //顶点向量
layout (location = 1) in vec3 aNormal; //法线向量
layout (location = 2) in vec2 aTexCoords; //纹理坐标

//输出
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;//法向量防止物体拉升而丢失
    TexCoords = aTexCoords;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
