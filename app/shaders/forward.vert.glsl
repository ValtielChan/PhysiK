#version 130

uniform mat4 modelViewMatrix;
uniform mat4 MVP;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;

#ifdef NORMAL_MAP
out mat3 tangentSpace;
#else
out vec3 varNormal;
#endif

out vec2 varTexCoord;

#ifdef DIRECTIONNAL_LIGHT
uniform vec3 dirLight;
#endif

#ifdef POINT_LIGHT
uniform vec3 pointLight;
#endif

in vec3 inPosition;
in vec2 inTexCoord;
in vec3 inNormal;
#ifdef NORMAL_MAP
in vec3 inTangent;
in vec3 inBinormal;
#endif

#ifdef INSTANCING
in vec3 inInstanceOffset;
#endif

#ifndef AMBIENT_LIGHT
out vec3 lightDirInView;
out vec3 halfVecInView;
#endif

#ifdef POINT_LIGHT
out vec3 lightDistInView;
#endif

#ifdef SHADOWMAP
uniform mat4 lightMVP;
out vec4 posInLightSpace;
#endif

void main(void) {
#ifdef INSTANCING
    vec4 pos = vec4(inPosition + inInstanceOffset, 1.0);
#else
    vec4 pos = vec4(inPosition, 1.0);
#endif

    vec3 posInView = vec3(modelViewMatrix*pos);

#ifdef DIRECTIONNAL_LIGHT
    lightDirInView = normalize(mat3(viewMatrix)*dirLight);
    halfVecInView = normalize(lightDirInView - normalize(posInView));
#endif

#ifdef SHADOWMAP
    posInLightSpace = lightMVP * pos;
#endif

#ifdef POINT_LIGHT
    vec3 lightPosInView = vec3(viewMatrix*vec4(pointLight, 1));
    lightDistInView = lightPosInView - posInView;
    lightDirInView = normalize(lightDistInView);
    halfVecInView = normalize(lightDirInView - normalize(posInView));
#endif

#ifdef NORMAL_MAP
    tangentSpace = transpose(mat3(normalize(normalMatrix*inTangent),
                                                              normalize(normalMatrix*inBinormal),
                                                              normalize(normalMatrix*inNormal)));
#else
    varNormal = normalize(normalMatrix*inNormal);
#endif

    varTexCoord = inTexCoord.xy;

    gl_Position = MVP * pos;
}
