/* 
 fragment.strings
 opengl环境
 
 Created by Stan on 2024/8/12.
 
 */
#version 330 core
out vec4 FragColor;

in vec3 Normal;  //法向量
in vec3 FragPos; //顶点向量

uniform vec3 lightPos; //灯的位置
uniform vec3 viewPos;  //观察者位置
uniform vec3 lightColor; //灯的颜色
uniform vec3 objectColor; //物体颜色

void main()
{
    // ambient 环境光
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;//环境光 = 灯的颜色 * 一个固定的值(减弱一点)
    
    // diffuse 漫反射
    vec3 norm = normalize(Normal); //归一化
    vec3 lightDir = normalize(lightPos - FragPos); //计算光源到物体片段的向量 光线向量
    float diff = max(dot(norm, lightDir), 0.0);//计算法向量和光线向量的夹角大小(夹角越小这个值越大,光源在物体正上方最大,其他地方逐渐减下)
    vec3 diffuse = diff * lightColor;// 光源 * (diff 不同强度) = 漫反射颜色
    
    // specular 镜面反射
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);//片段和眼睛的向量
    vec3 reflectDir = reflect(-lightDir, norm);//反射光向量
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);//反射光和视觉向量点乘
    vec3 specular = specularStrength * spec * lightColor;
   
    //(环境光 + 漫反射 + 镜面) * 物体颜色 = 不同片段强度 * 物体颜色
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
} 
