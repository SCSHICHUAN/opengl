/* 
  fragment.strings
  opengl环境

  Created by Stan on 2022/12/13.
  
*/
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
}
