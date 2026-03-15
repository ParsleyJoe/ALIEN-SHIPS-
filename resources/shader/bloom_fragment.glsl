#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;

void main()
{
    vec2 texel = 1.0 / textureSize(texture0, 0);

    vec3 color = texture(texture0, fragTexCoord).rgb;

    // sample nearby pixels
    vec3 blur =
        texture(texture0, fragTexCoord + texel * vec2( 1, 0)).rgb +
        texture(texture0, fragTexCoord + texel * vec2(-1, 0)).rgb +
        texture(texture0, fragTexCoord + texel * vec2( 0, 1)).rgb +
        texture(texture0, fragTexCoord + texel * vec2( 0,-1)).rgb;

    blur *= 0.25;

    // mix original + blur
    finalColor = vec4(color + blur * 0.6, 1.0);
}
