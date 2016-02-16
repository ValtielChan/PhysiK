#version 330 core

uniform vec3 lightColor;

uniform float materialNs;
uniform uint objectId;

#ifdef INSTANCING
flat in int instanceId;
#endif

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

in vec2 varTexCoord;

#ifndef AMBIENT_LIGHT
in vec3 lightDirInView;
in vec3 halfVecInView;
#endif

layout(location = 0)out vec4 outColor;
layout(location = 1)out vec4 pickData;

vec3 phongLighting(in vec3 kd, in vec3 ks, in float ns, in vec3 color, in vec3 normal, in vec3 lightDir, in vec3 halfVec){
    float diffuseComponent = max(dot(normal, lightDir), 0);
    float specularComponent = max(dot(halfVec, normal), 0);
    return color*diffuseComponent*(kd+ks*pow(specularComponent, ns));
}

float computeShadow(sampler2D shadowmap, vec3 shadow){
    float lightFragDepth = texture(shadowmap, shadow.xy).r;
    return lightFragDepth < shadow.z ? 0. : 1.;
}

void main(void) {
#ifdef ALPHA_MASK
    if(texture(alphaMask, varTexCoord).r < 0.5)
        discard;
#endif

#ifdef NORMAL_MAP
    vec3 normal = normalize(texture(normalMap, varTexCoord).xyz * tangentSpace);
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

#ifdef AMBIENT_LIGHT
    outColor = vec4(diffuse*0.1f, 1);
#else
    vec3 light = phongLighting(diffuse, specular, materialNs, lightColor, normal, lightDirInView, halfVecInView);
    outColor = vec4(light, 1);
#endif

#ifdef INSTANCING
    pickData = vec4(gl_FragCoord.z, gl_FragCoord.w, float(int(objectId) + instanceId), 0);
#else
    pickData = vec4(gl_FragCoord.z, gl_FragCoord.w, float(objectId), 0);
#endif

#ifndef AMBIENT_LIGHT
    pickData = vec4(0);
#endif
}
