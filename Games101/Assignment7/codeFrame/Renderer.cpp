//
// Created by goksu on 2/25/20.
//

#include <fstream>
#include "Scene.hpp"
#include "Renderer.hpp"
#include <mutex>
#include <thread>
std::mutex mutex_ins;


inline float deg2rad(const float& deg) { return deg * M_PI / 180.0; }

const float EPSILON = 0.00001;

// The main render function. This where we iterate over all pixels in the image,
// generate primary rays and cast these rays into the scene. The content of the
// framebuffer is saved to a file.
void Renderer::Render(const Scene& scene)
{
    std::vector<Vector3f> framebuffer(scene.width * scene.height);

    float scale = tan(deg2rad(scene.fov * 0.5));
    float imageAspectRatio = scene.width / (float)scene.height;
    Vector3f eye_pos(278, 273, -800);
    int m = 0;

    // change the spp value to change sample ammount
    // std::thread :
    int spp = 512;
    std::cout << "SPP: " << spp << "\n";

    int process = 0;
    auto deal = [&](int lx, int rx, int ly, int ry) {

        for (uint32_t j = 0; j < scene.height; ++j) {
            for (uint32_t i = 0; i < scene.width; ++i) {
                // generate primary ray direction
                float x = (2 * (i + 0.5) / (float)scene.width - 1) *
                          imageAspectRatio * scale;
                float y = (1 - 2 * (j + 0.5) / (float)scene.height) * scale;

                Vector3f dir = normalize(Vector3f(-x, y, 1));
                for (int k = 0; k < spp; k++){
                    framebuffer[m] += scene.castRay(Ray(eye_pos, dir), 0) / spp;  
                }
                m++;
                process++;
            }

            {
                // lock:
                std::lock_guard<std::mutex> g1(mutex_ins);
                UpdateProgress(1.0 * process / scene.width / scene.height);
            }
        }
    };

    int minx = 0, maxx = scene.width - 1;
    int miny = 0, maxy = scene.height - 1;

    int bx = 5, by = 5;
    int nx = (scene.width + bx - 1) / bx, ny = (scene.height + by - 1) / by;
    std::thread th[bx * by];

    // form thread pool
    for(int i = 0, id = 0; i < scene.width; i += nx) {
        for(int j = 0; j < scene.height; j += ny) {
            th[id] = std::thread(deal, i, std::min(i+nx, scene.width) - 1, j, std::min(j+ny, scene.height) - 1);

            id++;
        }
    }

    // thread join
    for(int i = 0; i < bx * by; i++) {
        th[i].join();
    }

    UpdateProgress(1.f);

    // save framebuffer to file
    FILE* fp = fopen("binary.ppm", "wb");
    (void)fprintf(fp, "P6\n%d %d\n255\n", scene.width, scene.height);
    for (auto i = 0; i < scene.height * scene.width; ++i) {
        static unsigned char color[3];
        color[0] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].x), 0.6f));
        color[1] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].y), 0.6f));
        color[2] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].z), 0.6f));
        fwrite(color, 1, 3, fp);
    }
    fclose(fp);    
}
