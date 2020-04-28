#version 410

// Definitions
#define MAX_MARCH_STEPS 100		// Max number of steps in the march
#define SURFACE_DISTANCE 0.001	// Surface bias
#define MAX_DISTANCE 100.0		// Max distance to march
#define PI 3.14159265359		// PI

const vec3 ORANGE = vec3(1.0, 0.4, 0.1);

/*************************************
**  OUTPUT MULTI-RENDERING TARGETS  **
*************************************/
layout(location = 0) out vec4 fragColor;


/***************
**	UNIFORMS  **
***************/
// Screen resolution.
uniform vec2 uResolution;

// Current time.
uniform double uTime;
uniform vec2 uMouse;

uniform mat4 uModel;


// Normalize range from 0-1.
float normalizeRange(float _value)
{
	return _value * 0.5 + 0.5;
}

mat2 rotation(float _angle)
{
	// Calculate the sine and cosine of the given angle.
	float s = sin(_angle);
	float c = cos(_angle);

	// Return the rotation matrix.
	return mat2(c, -s, s, c);
}

// Create a gyroid at the given position.
float signedDistanceGyroid(vec3 _position, float _scale, float _thickness, float _bias)
{
	// Scale the position of the gyroid.
	_position *= _scale;

	// Create gyroid effect.
	return abs(dot(sin(_position), cos(_position.zxy)) - _bias) / _scale - _thickness;
}

// Transform the position by the model matrix.
vec3 transform(vec3 _position)
{
	return (inverse(uModel) * vec4(_position, 1.0)).xyz;
}

// Get the distance to current position in ray march.
float getDistance(vec3 _position)
{
	// Transform the position.
	_position = transform(_position);
	
	// Create multiple gyroids for a bumpmap.
	float gyroid1 = signedDistanceGyroid(_position, 5.23, 0.03, 1.4);
	float gyroid2 = signedDistanceGyroid(_position, 10.76, 0.03, 0.3);
	float gyroid3 = signedDistanceGyroid(_position, 20.76, 0.03, 0.3);
	float gyroid4 = signedDistanceGyroid(_position, 35.76, 0.03, 0.3);
	float gyroid5 = signedDistanceGyroid(_position, 60.76, 0.03, 0.3);
	float gyroid6 = signedDistanceGyroid(_position, 110.76, 0.03, 0.3);

	// Scale gyroids to get smaller section.
	gyroid2 *= 0.4;
	gyroid3 *= 0.3;
	gyroid4 *= 0.2;
	gyroid5 *= 0.2;
	gyroid6 *= 0.2;

	// Create a bumpmap with each gyroid.
	float bumpmap = gyroid1 - gyroid2 - gyroid3 + gyroid4 + gyroid5 + gyroid6;

	// Scale down the bumpmap and return.
	return bumpmap * 0.8;
}

// Raymarching (sphere tracing)
float rayMarch(vec3 _origin, vec3 _direction)
{
	// Track the distance from the origin.
	float distanceOrigin = 0.0;

	// March along the ray until surface is hit or it passes the surface.
	for (int counter = 0; counter < MAX_MARCH_STEPS; counter++)
	{
		// Current position in the march.
		vec3 currentPosition = _origin + _direction * distanceOrigin;

		// Distance to the scene,
		float distanceScene = getDistance(currentPosition);

		// Move the origin to the new march position based on scene distance.
		distanceOrigin += distanceScene;

		// Smaller than surface distance is hit on object.
		// Larger than max distance passed the object. Prevent infinite checks.
		if (abs(distanceScene) < SURFACE_DISTANCE || distanceOrigin > MAX_DISTANCE)
		{
			break;
		}
	}

	// Return the ray point.
	return distanceOrigin;
}

vec3 getNormal(vec3 _surfacePosition)
{
	float surfaceDistance = getDistance(_surfacePosition);

	// Adjustment to calculate point close to surface point.
	vec2 nearPosition = vec2(0.02, 0.0);

	// Calculate swizzel coordinates.
	float x = getDistance(_surfacePosition - nearPosition.xyy);
	float y = getDistance(_surfacePosition - nearPosition.yxy);
	float z = getDistance(_surfacePosition - nearPosition.yyx);

	// Calculate surface normal from given point and point near the point.
	// Use 'swizzel' to move away from the point by a certain amount.
	vec3 normal = surfaceDistance - vec3(x, y, z);

	return normalize(normal);
}

vec3 getRayDirection(vec2 _uv, vec3 _position, vec3 _light, float _zoom)
{
	vec3 lightDirection = normalize(_light - _position);

	vec3 ray = normalize(cross(vec3(0.0, 1.0, 0.0), lightDirection));

	vec3 u = cross(lightDirection, ray);

	vec3 camera = _position + lightDirection * _zoom;

	vec3 image = camera + _uv.x * ray + _uv.y * u;

	return normalize(image - _position);
}

// Create flames in the background.
vec3 background(vec3 _rayDirection)
{
	// Create black color.
	vec3 color = vec3(0.0);
	
	// Get the current time.
	float time = float(uTime);
	
	// Generate height of screen from 0-1.
	float height = normalizeRange(_rayDirection.y);
	
	// Add orange color to the inverse height.
	color += (1.0 - height) * ORANGE * 2.0;
	
	// Get the x/z angle of the ray direction.
	float angle = atan(_rayDirection.x, _rayDirection.z);
	
	// Add movement to the flames.
	float flames = sin(angle * 10.0 + time) * sin(angle * 7.0 - time) * sin(angle * 6.0);
	
	// Smooth the flames at the height. (Prevents shortened flames)
	flames *= smoothstep(0.8, 0.5, height);
	
	// Add the flames to the color.
	color += flames;
	
	// Prevent negative values.
	color = max(color, 0.0);
	
	// Adds more brightness at the bottom of the flames.
	color += smoothstep(0.5, 0.0, height);
	
	return color;
}

/***********
**	MAIN  **
***********/
void main()
{
	// Normalize UV to center.
	vec2 uv = (gl_FragCoord.xy - 0.5 * uResolution.xy) / uResolution.y;

	// Convert time to float.
	float time = float(uTime);
	
	// Initialize a black screen.
	vec3 color = vec3(0.0);
	
	// Create heat distortion effect.
	uv += sin(uv * 20.0 + time) * 0.01;
	
	// Camera rays. (Offset from the actual camera)
	vec3 rayOrigin = vec3(0.0, 0.0, -0.001);

	// Rotate the rays.
	rayOrigin.yz *= rotation(-uMouse.y * PI);
	rayOrigin.xz *= rotation(-uMouse.x * 2.0 * PI);
	
	// Camera orientation.
	vec3 lookAt = vec3(0.0);
	
	// Setup the ray direction.
	vec3 rayDirection = getRayDirection(uv, rayOrigin, lookAt, 0.8);
	
	// Calculate the distance to the object.
	float marchDistance = rayMarch(rayOrigin, rayDirection);
	
	if (marchDistance < MAX_DISTANCE)
	{
		vec3 position = rayOrigin + rayDirection * marchDistance;
		
		vec3 normal = getNormal(position);
		
		float height = position.y;

		position = transform(position);
		
		float diffuse = normalizeRange(normal.y);
		
		color += diffuse * diffuse;
		
		float gyroid1 = signedDistanceGyroid(position, 10.76, 0.03, 0.3);
		
		// Blackening highlights of cracks.
		color *= smoothstep(-0.1, 0.1, gyroid1);
		
		float crackWidth = -0.02 + smoothstep(0.0, -0.5, normal.y) * 0.04;
		
		float cracks = smoothstep(crackWidth, -0.03, gyroid1);
		
		float gyroid2 = signedDistanceGyroid(position + time * 0.1, 4.76, 0.03, 0.0);
		
		float gyroid3 = signedDistanceGyroid(position + time * 0.075, 3.76, 0.3, 0.0);
		
		cracks *= gyroid2 * gyroid3 * 20.0 + 0.2 * smoothstep(0.2, 0.0, normal.y);
		
		// Add some orange color to the cracks.
		color += cracks * ORANGE * 3.0;
		
		float distortion = signedDistanceGyroid(position - vec3(0.0, time, 0.0), 3.76, 0.03, 0.0);
		
		color += distortion * ORANGE;
		
		color += smoothstep(0.0, -2.0, height) * ORANGE;
	}
	
	color = mix(color, background(rayDirection), smoothstep(0.0, 7.0, marchDistance));
	
	color *= 1.0 - dot(uv, uv);
	
	fragColor = vec4(color, 1.0);
}