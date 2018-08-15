varying vec4 cs_normal;
//out vec4 gl_FragColor;

void main(void)
{
    vec3 diffuse_color = vec3(0.5);
    vec3 cs_light = vec3(-1.0, -0.7, 0.0);
    cs_light = normalize(cs_light);
    vec3 light_color = vec3(1.0);
    vec3 lambertian_color = diffuse_color * light_color * clamp(dot(cs_normal.xyz, cs_light), 0.0, 1.0);
    vec3 ambient_color = vec3(0.1);
    vec3 o = lambertian_color + ambient_color;
    gl_FragColor = vec4(o.x, o.y, o.z, 1.0);
}
