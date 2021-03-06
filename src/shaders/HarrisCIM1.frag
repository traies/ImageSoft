#version 330 core

in vec2 UV;
layout(location = 0) out vec3 color;
uniform sampler2D myTextureSampler;
uniform sampler2D lx2;
uniform sampler2D ly2;
uniform sampler2D lxy;
uniform float threshold;
uniform float k;
uniform float cimV;
uniform vec3 pointColor;

void main() {
	if(UV.x <=0 || UV.y <=0 || UV.x >=1 || UV.y >=1 ) {
		color = texture(myTextureSampler, UV).rgb;
	} else {
		vec3 lx2v = texture(lx2, UV).rgb;
		vec3 ly2v = texture(ly2, UV).rgb;
		vec3 lxyv = texture(lxy, UV).rgb;
		vec3 cim = vec3(0);

		if(cimV == 1) {
			cim = (lx2v * ly2v - (lxyv * lxyv)) - ((lx2v + ly2v) * (lx2v + ly2v) * k);
		} else if(cimV == 2) {
			cim = (lx2v * ly2v - (lxyv * lxyv)) / ((lx2v + ly2v) * (lx2v + ly2v) + 0.001);
		} else if(cimV == 3) {
			cim = (lx2v * ly2v - (lxyv * lxyv* lxyv* lxyv)) - ((lx2v + ly2v) * (lx2v + ly2v) * k);
		}

		if (cim.r > threshold || cim.g > threshold || cim.b > threshold) {
			color = pointColor;
		} else {
			color = texture(myTextureSampler, UV).rgb;
		}
	}
}