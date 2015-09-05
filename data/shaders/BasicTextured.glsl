
varying vec2 vTexCoord;

%vertex

attribute vec4 position;
attribute vec2 texCoord;

uniform mat4 uProjection;
uniform mat4 uModelView;

void main()
{
    vTexCoord = texCoord;
    gl_Position = uProjection * uModelView * position;
}

%fragment

uniform sampler2D uTexture;

void main()
{
    gl_FragColor = texture2D(uTexture, vTexCoord);
}
