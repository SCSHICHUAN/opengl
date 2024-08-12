/* 
  fragment.strings
  opengl环境

  Created by Stan on 2024/8/12.
  
*/
#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor * objectColor, 1.0);
}
