/* 
 fragment.strings
 opengl环境
 
 Created by Stan on 2024/8/12.
 
 */
#version 330 core
out vec4 FragColor;

//物体材质
struct Material {
    sampler2D diffuse; //纹理慢反射
    sampler2D specular; //纹理节目反射
    float shininess;//反射强度
};
//灯
struct Light {
    vec3 position;//灯位置向量
//    vec3 direction;//平行光方向
    
    vec3 ambient;//环境强度
    vec3 diffuse;//漫反射强度
    vec3 specular;//镜面反射强度
    
    //衰减
    float constant; //常量
    float linear;   //一次项
    float quadratic;//二次项
};

//输入
in vec3 Normal;  //法向量
in vec3 FragPos; //顶点向量
in vec2 TexCoords; //纹理坐标


uniform vec3 viewPos; //眼睛
uniform Material material; //物体材质
uniform Light light; //灯

void main()
{
    // ambient 环境光
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;//环境光 = 灯环境光 * 材料环境光
    
    // diffuse 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
//    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (light.diffuse * texture(material.diffuse, TexCoords).rgb) * diff;//漫反射光 = (灯漫反射光 * 材料漫反射光) * 强度
    
    // specular 镜面反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (light.specular * texture(material.specular, TexCoords).rgb) * spec;//镜面光 = (灯镜面光 * 材料镜面光) * 强度
    
    // attenuation 衰减
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
   
    
    
    // 反射光    =   环境光  + 慢反射光 + 镜面光
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 

