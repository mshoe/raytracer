#version 430
layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) uniform image2D img_output;

int x_voxs = 10;
int y_voxs = 20;
int z_voxs = 10;
int Chunk[10][20][10];

//bool intersect(Ray r) 
//{ 
//    float tmin = (min.x - r.orig.x) / r.dir.x; 
//    float tmax = (max.x - r.orig.x) / r.dir.x; 
 
//    if (tmin > tmax) swap(tmin, tmax); 
 
//    float tymin = (min.y - r.orig.y) / r.dir.y; 
//    float tymax = (max.y - r.orig.y) / r.dir.y; 
 
//    if (tymin > tymax) swap(tymin, tymax); 
 
//    if ((tmin > tymax) || (tymin > tmax)) 
//        return false; 
 
//    if (tymin > tmin) 
//        tmin = tymin; 
 
//    if (tymax < tmax) 
//        tmax = tymax; 
 
//    float tzmin = (min.z - r.orig.z) / r.dir.z; 
//    float tzmax = (max.z - r.orig.z) / r.dir.z; 
 
//    if (tzmin > tzmax) swap(tzmin, tzmax); 
 
//    if ((tmin > tzmax) || (tzmin > tmax)) 
//        return false; 
 
//    if (tzmin > tmin) 
//        tmin = tzmin; 
 
//    if (tzmax < tmax) 
//        tmax = tzmax; 
 
//    return true; 
//} 

struct Ray 
{
    Ray(const Vec3f &orig, const Vec3f &dir) : orig(orig), dir(dir) 
    { 
        invdir = 1 / dir; 
        sign[0] = (invdir.x < 0); 
        sign[1] = (invdir.y < 0); 
        sign[2] = (invdir.z < 0); 
    } 
    Vec3 orig, dir;       // ray orig and dir 
    Vec3 invdir; 
    int sign[3]; 
}; 

void main() {
	for (int i = 0; i < x_voxs; i++) {
		for (int j = 0; j < y_voxs; j++) {
			for (int k = 0; k < z_voxs; k++) {
				Chunk[i][j][k] = 0;
			}
		}
	}
	Chunk[3][4][2] = 1;
	Chunk[8][7][5] = 1;

	// base pixel colour for image
	vec4 pixel = vec4(0.0, 0.0, 0.0, 1.0);
	// get index in global work group i.e x,y position
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
  
	//
	// interesting stuff happens here later
	//
	float max_x = 5.0;
	float max_y = 5.0;
	ivec2 dims = imageSize(img_output); // fetch image dimensions
	float x = (float(pixel_coords.x * 2 - dims.x) / dims.x);
	float y = (float(pixel_coords.y * 2 - dims.y) / dims.y);
	vec3 ray_o = vec3(x * max_x, y * max_y, 0.0);

	float width = dims.x, height = dims.y;
	float fov = 30, aspectratio = width/ float(height); 
    float angle = tan(3.14159 * 0.5 * fov / 180.); 

	// perspective ray direction
	float invWidth = 1 / float(width), invHeight = 1 / float(height); 
	float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio; 
    float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle; 
	vec3 ray_d = vec3(xx, yy, -1.0); // ortho
	ray_d = normalize(ray_d);

	vec3 sphere_c = vec3(0.0, 0.0, -10.0);
	float sphere_r = 1.0;


	// RAY TRACE - INTERSECTION
	vec3 omc = ray_o - sphere_c;
	float b = dot(ray_d, omc);
	float c = dot(omc, omc) - sphere_r * sphere_r;
	float bsqmc = b * b - c;
	// hit one or both sides
	if (bsqmc >= 0.0) {
	  pixel = vec4(0.4, 0.4, 1.0, 1.0);
	}
  
	// output to a specific pixel in the image
	imageStore(img_output, pixel_coords, pixel);
}
