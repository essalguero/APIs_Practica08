attribute vec3 vpos;

uniform mat4 mvpMatrix;

void main() {
	gl_Position = mvpMatrix * vec4(vpos, 1);
}