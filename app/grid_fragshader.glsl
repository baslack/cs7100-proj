//uniform sampler2D qt_Texture0;
//varying vec4 qt_TexCoord0;
varying vec3 fc;

void main(void)
{
    gl_FragColor = vec4(fc.x, fc.y, fc.z, 1.0f);
}
