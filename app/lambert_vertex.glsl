attribute vec3 ws_position;
attribute vec3 ws_normal;
uniform mat4 mvpMat;
uniform mat4 mvMat;
varying vec4 cs_normal;

void main(void)
{
    gl_Position = mvpMat * vec4(ws_position.xyz, 1.0);
    cs_normal = mvMat * vec4(ws_normal.xyz, 0.0);
}
