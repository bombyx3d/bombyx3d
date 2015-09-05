
%vertex

attribute vec4 position;

uniform mat4 uProjection;
uniform mat4 uModelView;

void main()
{
    gl_Position = uProjection * uModelView * position;
}

%fragment

uniform vec4 uColor;

void main()
{
    gl_FragColor = uColor;
}
