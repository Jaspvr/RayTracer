#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip> 

#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"


int main() {
    int frames = 50;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));

    auto material1 = make_shared<dielectric>(1.5);
    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));

    float x = -10.0;
    float z = -10.0;
    for(int i = 0; i<frames; i++){
        hittable_list world;

        world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material)); 
        world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material1));

        // This is the moving object:
        z += 0.5;
        x += 0.5;
        world.add(make_shared<sphere>(point3(x, 1, z), 0.3, material2));

        camera cam;

        cam.aspect_ratio      = 16.0 / 9.0;
        cam.image_width       = 1200;
        cam.samples_per_pixel = 5;
        cam.max_depth         = 25;

        cam.vfov     = 50;
        cam.lookfrom = point3(13,2,3);
        cam.lookat   = point3(0,0,0);
        cam.vup      = vec3(0,1,0);

        // Save the scene to a file
        std::stringstream sst;
        sst << "rendered_frames" << "/frame" << std::setw(3) << std::setfill('0') << i << ".ppm";
        std::ofstream output_file(sst.str());
        std::streambuf* old_buf = std::cout.rdbuf(output_file.rdbuf()); // Redirect std::cout

        cam.render(world); // Call the render function

        // Reset std::cout to its original state
        std::cout.rdbuf(old_buf);
        output_file.close();
    }

    return 0;
}