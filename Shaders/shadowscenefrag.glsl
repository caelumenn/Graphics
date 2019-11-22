#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;
uniform sampler2DShadow shadowTex; // NEW !
uniform int numLights;

uniform vec3 cameraPos;
uniform vec4 lightColour[5];
uniform vec3 lightPos[5];
uniform float lightRadius[5];
uniform vec3 lightDirection[5];
uniform float lightAngle[5];

in Vertex{
	vec3 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 worldPos;
	vec4 shadowProj; // New !
}IN;

out vec4 fragColour;

void main(void) {
	mat3 TBN = mat3(IN.tangent, IN.binormal, IN.normal);
	vec3 normal = normalize(TBN * (texture(bumpTex, IN.texCoord).rgb * 2.0 - 1.0));

	vec4 diffuse = texture(diffuseTex, IN.texCoord);
	vec3 viewDir = normalize(cameraPos - IN.worldPos);

	float shadow = 1.0; // New !
	if (IN.shadowProj.w > 0.0) { // New !
		shadow = textureProj(shadowTex, IN.shadowProj);
	}

	vec3 totalambient;

	for(int i = 0; i< numLights;i++){
		vec3 incident = normalize(lightPos[i] - IN.worldPos);
		float lambert = max(0.0, dot(incident, normal));
		float dist = length(lightPos[i] - IN.worldPos);
		float atten = 1.0 - clamp(dist / lightRadius[i], 0.0, 1.0);

		vec3 halfDir = normalize(incident + viewDir);
		float rFactor = max(0.0, dot(halfDir, normal));
		float sFactor = pow(rFactor, 33.0);

		lambert *= shadow; // New !

		vec3 colour = (diffuse.rgb * lightColour[i].rgb);
		colour += (lightColour[i].rgb * sFactor) * 0.33;

		float lightToSurfaceAngle = degrees(acos(dot(-incident, normalize(lightDirection[i]))));
		if (lightToSurfaceAngle > lightAngle[i]) {
			if (lightAngle[i] + 20.0 > lightToSurfaceAngle) {
				atten *= 1.0 - (lightToSurfaceAngle - lightAngle[i]) / 20;
			}
			else {
				atten = 0.0;
			}
		}
		fragColour += vec4(colour * atten * lambert, diffuse.a);

		totalambient += (diffuse.rgb * lightColour[i].rgb) * 0.1;
	}
	fragColour.rgb += totalambient;
}
