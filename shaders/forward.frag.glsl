#version 330 core

uniform vec3 lightColor;

uniform float materialNs;
uniform int objectId;

#ifdef ALPHA_MASK
uniform sampler2D alphaMask;
#endif

#ifdef AMBIENT_TEXTURE
uniform sampler2D ambientTexture;
#else
uniform vec3 materialKa;
#endif

#ifdef DIFFUSE_TEXTURE
uniform sampler2D diffuseTexture;
#else
uniform vec3 materialKd;
#endif

#ifdef SPECULAR_TEXTURE
uniform sampler2D specularTexture;
#else
uniform vec3 materialKs;
#endif

#ifdef NORMAL_MAP
uniform sampler2D normalMap;

in mat3 tangentSpace;
#else
in vec3 varNormal;
#endif

#ifdef SHADOWMAP
uniform sampler2D shadowMap;
in vec4 posInLightSpace;
#endif

in vec2 varTexCoord;

#ifndef AMBIENT_LIGHT
in vec3 lightDirInView;
in vec3 halfVecInView;
#endif

#ifdef POINT_LIGHT
in vec3 lightDistInView;
uniform float attenuation;
#endif

layout(location = 0)out vec4 outColor;

vec3 phongLighting(in vec3 kd, in vec3 ks, in float ns, in vec3 color, in vec3 normal, in vec3 lightDir, in vec3 halfVec){
    float diffuseComponent = max(dot(normal, lightDir), 0);
    float specularComponent = max(dot(halfVec, normal), 0);
    return color*diffuseComponent*(kd+ks*pow(specularComponent, ns));
}

float computeShadow(sampler2D shadowmap, vec3 shadow){
    float lightFragDepth = texture(shadowmap, shadow.xy).r;
    return lightFragDepth < shadow.z ? 0 : 1;
}

void main(void) {
#ifdef ALPHA_MASK
    if(texture(alphaMask, varTexCoord).r < 0.5)
        discard;
#endif

#ifdef NORMAL_MAP
    vec3 normal = normalize(texture(normalMap, varTexCoord).xyz * tangentSpace);
    //normal = normalize(vec3(0, 0, 1) * tangentSpace);
#else
    vec3 normal = normalize(varNormal);
#endif

#ifdef AMBIENT_TEXTURE
    vec3 ambient = texture(ambientTexture, varTexCoord).rgb;
#else
    vec3 ambient = materialKa;
#endif

#ifdef DIFFUSE_TEXTURE
    vec3 diffuse = texture(diffuseTexture, varTexCoord).rgb;
#else
    vec3 diffuse = materialKd;
#endif

#ifdef SPECULAR_TEXTURE
    vec3 specular = vec3(texture(specularTexture, varTexCoord).r);
#else
    vec3 specular = materialKs;
#endif

#ifdef SHADOWMAP
    float shadow = computeShadow(shadowMap, posInLightSpace.xyz/posInLightSpace.w);
#else
    float shadow = 1;
#endif

    float att = 0;
#ifdef POINT_LIGHT
    att = length(lightDistInView)*attenuation;
    if(att > 1)
        discard;
#endif

#ifdef AMBIENT_LIGHT
    outColor = vec4(diffuse*0.1f, 1);
#else
    vec3 light = phongLighting(diffuse, specular, materialNs, lightColor, normal, lightDirInView, halfVecInView);
    outColor = vec4(light*shadow*(1+cos(1.57 + att*1.57)), 1);
#endif
}
