varying vec4 cs_normal;
//out vec4 gl_FragColor;

void main(void)
{
    vec3 diffuse_color = vec3(0.5);
    vec3 cs_light = vec3(-1, -1, -0.5);
    cs_light = normalize(cs_light);
    vec3 light_color = vec3(1.0);
    vec3 normal = vec3(cs_normal.x, cs_normal.y, cs_normal.z);
    if (!gl_FrontFacing){
        normal = -normal;
    }
    vec3 lambertian_color = diffuse_color * light_color * clamp(dot(normal, -cs_light), 0.0, 1.0);
    vec3 ambient_color = vec3(0.1);
    vec3 o = lambertian_color + ambient_color;
    gl_FragColor = vec4(o.x, o.y, o.z, 1.0);
}
