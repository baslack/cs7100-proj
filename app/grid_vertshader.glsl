attribute vec3 vp;
attribute vec3 vc;
//attribute vec4 qt_MultiTexCoord0;

uniform mat4 mvpMat;
//varying vec4 qt_TexCoord0;
varying vec3 fc;

void main(void)
{
    gl_Position = mvpMat * vec4(vp.x, vp.y, vp.z, 1.0f);
    fc = vc;
}
