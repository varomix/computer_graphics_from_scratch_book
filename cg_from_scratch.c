#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

const int WIDTH = 600;
const int HEIGHT = 600;
const int VIEWPORT_SIZE = 1;
const int PROJECTION_PLANE_Z = 1;
const int NUM_SPHERES = 3;
const Color BACKGROUND_COLOR = RAYWHITE;

typedef struct Sphere {
    Vector3 center;
    float radius;
    Color color;
} Sphere;

Vector3 canvas_to_viewport(int x, int y) {
    return (Vector3){x * VIEWPORT_SIZE / (float)WIDTH, y * VIEWPORT_SIZE / (float)HEIGHT, PROJECTION_PLANE_Z};
}

Vector2 intersect_ray_sphere(Vector3 origin, Vector3 direction, Sphere sphere) {
    Vector3 oc = Vector3Subtract(origin, sphere.center);
    float k1 = Vector3DotProduct(direction, direction);
    float k2 = 2 * Vector3DotProduct(oc, direction);
    float k3 = Vector3DotProduct(oc, oc) - sphere.radius * sphere.radius;

    float discriminant = k2 * k2 - 4 * k1 * k3;
    if (discriminant < 0) {
        return (Vector2){INFINITY, INFINITY};
    }

    float t1 = (-k2 + sqrtf(discriminant)) / (2.0 * k1);
    float t2 = (-k2 - sqrtf(discriminant)) / (2.0 * k1);
    return (Vector2){t1, t2};
}

Color trace_ray(Vector3 origin, Vector3 direction, float t_min, float t_max, Sphere *spheres) {
    float closest_t = INFINITY;
    Sphere closest_sphere;
    bool sphere_found = false;

    for (int i = 0; i < NUM_SPHERES; i++) {
        Vector2 ts = intersect_ray_sphere(origin, direction, spheres[i]);
        if (ts.x < closest_t && t_min < ts.x && ts.x < t_max) {
            closest_t = ts.x;
            closest_sphere = spheres[i];
            sphere_found = true;
        }
        if (ts.y < closest_t && t_min < ts.y && ts.y < t_max) {
            closest_t = ts.y;
            closest_sphere = spheres[i];
            sphere_found = true;
        }
    }

    if (sphere_found == false)
        return BACKGROUND_COLOR;

    return closest_sphere.color;
}

void put_pixel(int x, int y, Color color) {
    DrawPixel((WIDTH / 2.0) + x, (HEIGHT / 2.0) - y - 1, color);
}

int main(void) {
    Vector3 camera_position = {0, 0, 0};
    InitWindow(WIDTH, HEIGHT, "CG from Scratch Book by Varomix");

    // clang-format off
	Sphere spheres[NUM_SPHERES] = {
		{ (Vector3){0, -1, 3}, 1, (Color){255, 0, 0, 255} },
		{ (Vector3){ 2, 0, 4}, 1, (Color){0, 0, 255, 255} },
		{ (Vector3){-2, 0, 4}, 1, (Color){0, 255, 0, 255} }
	};
    // clang-format on

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        ClearBackground(BLACK);
        BeginDrawing();

        for (int x = -WIDTH / 2.0; x < WIDTH / 2.0; x++) {
            for (int y = -HEIGHT / 2.0; y < HEIGHT / 2.0; y++) {
                Vector3 direction = canvas_to_viewport(x, y);
                Color color = trace_ray(camera_position, direction, 1, INFINITY, spheres);
                put_pixel(x, y, color);
            }
        }

        DrawText("Chapter 2: Flat shading", 10, 10, 18, GRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
