#version 150 core

// uniform sampler2D diffuseTex ;
// uniform int useTexture ;

// in Vertex{
//     vec4 colour;
//     vec2 texCoord;
// }IN;

// out vec4 fragColour ;

// void main (void) {
//     fragColour = IN.colour ;
//     if (useTexture > 0) {
//         fragColour *= texture(diffuseTex, IN.texCoord);
//     }
// }


uniform sampler2D diffuseTex ;
uniform int useTexture ;

uniform vec4 lightColour ;
uniform vec3 lightPos ;
uniform vec3 cameraPos ;
uniform float lightRadius ;

in Vertex {
    vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 worldPos;
}IN ;

out vec4 fragColour ;

void main (void) {
    if (useTexture > 0) {
        vec4 diffuse = texture(diffuseTex, IN.texCoord) * IN.colour;
        vec3 incident = normalize(IN.worldPos - cameraPos);
        float dist = length(lightPos - IN.worldPos);
        float atten = 1.0 - clamp(dist / lightRadius, 0.2, 1.0);

        fragColour = (lightColour * diffuse * atten) * (diffuse);
    }

    vec3 incident = normalize(IN.worldPos - cameraPos );
    float dist = length(lightPos - IN.worldPos);
    float atten = 1.0 - clamp(dist / lightRadius, 0.2, 1.0);

    fragColour = (lightColour * atten);
}