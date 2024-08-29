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
    vec3 direction;//平行光方向
    float cutOff;//切光角
    float outerCutOff;//外切光角
    
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
    //片段和光向量
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    
    
    // ambient 环境光
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;//环境光 = 灯环境光 * 材料环境光
    
    
    // diffuse 漫反射
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (light.diffuse * texture(material.diffuse, TexCoords).rgb) * diff;//漫反射光 = (灯漫反射光 * 材料漫反射光) * 强度
    
    // specular 镜面反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (light.specular * texture(material.specular, TexCoords).rgb) * spec;//镜面光 = (灯镜面光 * 材料镜面光) * 强度
    
    
    // spotlight (soft edges) 聚光灯(软边缘)
    float theta = dot(lightDir, normalize(-light.direction));//片段光向量,spotDir聚光方向向量,夹角,theta 当前的光线
    float epsilon = (light.cutOff - light.outerCutOff);//余弦值差 ϵ=ϕ−γ,内锥和外锥,余弦值差
    /*
     光线在 内外锥 是动态变变化,余弦值在(0-90度)是角度的增加而减小
     最大差值 = 内锥 - 外锥    ϕ − γ   余弦值
     动态差值 = 当前角 - 外锥  θ − γ   余弦值
                |
                |
               增大,动态差值减小
     */
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);//当前光线和外锥,内外切光角余弦值差 θ−γ <= ϕ−γ
    diffuse  *= intensity; //clamp函数，它把第一个参数约束(Clamp)在了0.0到1.0之间
    specular *= intensity;
    
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

