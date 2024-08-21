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

    auto sun = make_shared<emissive>(color(0.8, 0.8, 0.3));
    auto mercury = make_shared<lambertian>(color(0.663, 0.663, 0.663));
    auto venus = make_shared<lambertian>(color(0.929, 0.788, 0.686));
    auto earth = make_shared<lambertian>(color(0.392, 0.584, 0.929));
    auto mars = make_shared<lambertian>(color(0.737, 0.153, 0.196));

    auto moon = make_shared<lambertian>(color(0.3, 0.3, 0.3));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 20;
    cam.max_depth         = 50;

    cam.vfov     = 50;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    float radius_mercury = 3.0;
    float radius_venus = 4.0;
    float radius_earth = 6.0;
    float radius_mars = 8.0;

    float radius_moon = 0.5;

    float centerx = 0.0;
    float centerz = 0.0;


    for(int i = 30; i<frames; i++){
        hittable_list world;

        // Update the camera angle
        if(i<20){
            float camera_position = float(i)/20.0;
            cam.lookfrom = point3((13.0-13.0*camera_position), (2.0+18.0*camera_position), (3.0-3.0*camera_position));
        }else if(i >30){
            float camera_position = abs( ( float(i-30)/20.0) - 1.0 ); // does the inverse of iterations 1 to 20
            cam.lookfrom = point3((13.0-13.0*camera_position), (2.0+18.0*camera_position), (3.0-3.0*camera_position));
        }else{
            cam.lookfrom = point3(0.1, 20.0, 0.1);
        }
        
        // Stationary sphere
        world.add(make_shared<sphere>(point3(centerx, 1, centerz), 1.0, sun));

        float angle = 2.0 * M_PI * float(i) / frames;
        // Orbiting Planets (angle is multiplied to capture relative orbit speed)
        float x_mercury = centerx + radius_mercury * cos(angle * 4.15);
        float z_mercury = centerz + radius_mercury * sin(angle * 4.15);
        world.add(make_shared<sphere>(point3(x_mercury, 1, z_mercury), 0.115, mercury));

        float x_venus = centerx + radius_venus * cos(angle * 1.62);
        float z_venus = centerz + radius_venus * sin(angle * 1.62);
        world.add(make_shared<sphere>(point3(x_venus, 1, z_venus), 0.285, venus));

        float x_earth = centerx + radius_earth * cos(angle);
        float z_earth = centerz + radius_earth * sin(angle);
        world.add(make_shared<sphere>(point3(x_earth, 1, z_earth), 0.3, earth));

        float x_moon= x_earth + radius_moon * cos(angle*10);
        float z_moon = z_earth + radius_moon * sin(angle*10);
        float y_moon = 1.0 + 0.2 * sin(angle*10); // Off axis of the planets
        world.add(make_shared<sphere>(point3(x_moon, y_moon, z_moon), 0.08, moon));


        float x_mars = centerx + radius_mars * cos(angle * 0.53); 
        float z_mars = centerz + radius_mars * sin(angle * 0.53);
        world.add(make_shared<sphere>(point3(x_mars, 1, z_mars), 0.16, mars));
        

        // Save the scene to a file
        std::stringstream sst;
        sst << "solar_system_camera" << "/frame" << std::setw(3) << std::setfill('0') << i << ".ppm";
        std::ofstream output_file(sst.str());
        std::streambuf* old_buf = std::cout.rdbuf(output_file.rdbuf()); // Redirect std::cout

        cam.render(world); // Call the render function

        // Reset std::cout to its original state
        std::cout.rdbuf(old_buf);
        output_file.close();
    }

    return 0;
}