attribute vec3 vpos;

uniform mat4 mvpMatrix;

varying vec4 fpos;

void main() {
	fpos = mvpMatrix * vec4(vpos, 1);
	gl_Position = mvpMatrix * vec4(vpos, 1);
}