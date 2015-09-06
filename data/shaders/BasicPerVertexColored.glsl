
varying vec4 vColor;

%vertex

attribute vec4 position;
attribute vec4 color;

uniform mat4 uProjection;
uniform mat4 uModelView;

void main()
{
    vColor = color;
    gl_Position = uProjection * uModelView * position;
}

%fragment

void main()
{
    gl_FragColor = vColor;
}
