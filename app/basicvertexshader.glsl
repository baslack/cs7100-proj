attribute vec4 qt_Vertex;
//attribute vec4 qt_MultiTexCoord0;
uniform mat4 mvpMat;
//varying vec4 qt_TexCoord0;

void main(void)
{
    gl_Position = mvpMat * qt_Vertex;
//    qt_TexCoord0 = qt_MultiTexCoord0;
}
