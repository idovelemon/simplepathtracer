#include <stdint.h>
#include <iostream>

#include <omp.h>

#include "spt\sptcamera.h"
#include "spt\sptlight.h"
#include "spt\sptmaterial.h"
#include "spt\sptobject.h"
#include "spt\sptsampler.h"
#include "spt\spttracer.h"
#include "spt\sptutil.h"
#include "spt\sptworld.h"

int main() {
    // Setup world
    int SCREEN_WIDTH = 400;
    int SCREEN_HEIGHT = 400;
    int AREA_SAMPLER_NUM = 36;
    int SAMPLER_NUM = 625;

    spt::World world;
    world.SetMaxDepth(5);

    // AreaLight1
    spt::Object* obj = new spt::Object();
    spt::Disk* disk = new spt::Disk(spt::Vector3(0.0f, 49.9f, 0.0f), spt::Vector3(0.0f, -1.0f, 0.0f), 25.0f, 0.001f);
    spt::Material* emission = new spt::Emission(10.0f, spt::Vector3(1.0f, 1.0f, 1.0f));
    obj->SetShape(disk);
    obj->SetMaterial(emission);
    spt::Sampler aream_sampler(AREA_SAMPLER_NUM);
    spt::AreaLight* area = new spt::AreaLight(obj, &aream_sampler);
    world.AddObject(obj);
    world.AddLight(area);

    // Diffuse shpere
    obj = new spt::Object();
    spt::Sphere* sphere = new spt::Sphere(spt::Vector3(-25.0f, -30.0f, 0.0f), 20.0f, 0.0001f);
    spt::Material* diffuse = new spt::Diffuse(0.8f, spt::Vector3(1.0f, 1.0f, 1.0f), SAMPLER_NUM);
    obj->SetShape(sphere);
    obj->SetMaterial(diffuse);
    world.AddObject(obj);

    // Perfect Specular sphere
    obj = new spt::Object();
    sphere = new spt::Sphere(spt::Vector3(25.0f, -30.0f, 0.0f), 20.0f, 0.0001f);
    spt::Material* perfect = new spt::PerfectSpecular(0.8f, spt::Vector3(1.0f, 1.0f, 1.0f));
    obj->SetShape(sphere);
    obj->SetMaterial(perfect);
    world.AddObject(obj);

    // Box-Down
    obj = new spt::Object();
    spt::Plane* plane = new spt::Plane(spt::Vector3(0.0f, -50.0f, 0.0f), spt::Vector3(0.0f, 1.0f, 0.0f), 0.001f);
    diffuse = new spt::Diffuse(0.8f, spt::Vector3(1.0f, 1.0f, 1.0f), SAMPLER_NUM);
    obj->SetShape(plane);
    obj->SetMaterial(diffuse);
    world.AddObject(obj);

    // Box-Left
    obj = new spt::Object();
    plane = new spt::Plane(spt::Vector3(-50.0f, 0.0f, 0.0f), spt::Vector3(1.0f, 0.0f, 0.0f), 0.001f);
    diffuse = new spt::Diffuse(0.8f, spt::Vector3(1.0f, 0.0f, 0.0f), SAMPLER_NUM);
    obj->SetShape(plane);
    obj->SetMaterial(diffuse);
    world.AddObject(obj);

    // Box-Right
    obj = new spt::Object();
    plane = new spt::Plane(spt::Vector3(50.0f, 0.0f, 0.0f), spt::Vector3(-1.0f, 0.0f, 0.0f), 0.001f);
    diffuse = new spt::Diffuse(0.8f, spt::Vector3(0.0f, 0.0f, 1.0f), SAMPLER_NUM);
    obj->SetShape(plane);
    obj->SetMaterial(diffuse);
    world.AddObject(obj);

    // Box-Front
    obj = new spt::Object();
    plane = new spt::Plane(spt::Vector3(0.0f, 0.0f, 50.0f), spt::Vector3(0.0f, 0.0f, -1.0f), 0.001f);
    diffuse = new spt::Diffuse(0.8f, spt::Vector3(1.0f, 1.0f, 1.0f), SAMPLER_NUM);
    obj->SetShape(plane);
    obj->SetMaterial(diffuse);
    world.AddObject(obj);

    // Box-Top
    obj = new spt::Object();
    plane = new spt::Plane(spt::Vector3(0.0f, 50.0f, 0.0f), spt::Vector3(0.0f, -1.0f, 0.0f), 0.001f);
    diffuse = new spt::Diffuse(0.8f, spt::Vector3(1.0f, 1.0f, 1.0f), SAMPLER_NUM);
    obj->SetShape(plane);
    obj->SetMaterial(diffuse);
    world.AddObject(obj);

    int process = omp_get_num_procs();
    float* color_buf = new float[SCREEN_WIDTH * SCREEN_HEIGHT * 3];

    #pragma omp parallel for
    for (int32_t i = 0; i < process; i++) {
        spt::Camera camera(spt::Vector3(0.0f, 0.0f, -100.0f), spt::Vector3(0.0f, 0.0f, 0.0f),
            1.0f * SCREEN_WIDTH / SCREEN_HEIGHT, 79.0f, 1.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
        spt::Sampler sampler(SAMPLER_NUM);
        spt::Tracer tracer(&world, &camera, &sampler);
        tracer.Trace(SCREEN_HEIGHT / process * i, SCREEN_HEIGHT / process * (i + 1),
            SCREEN_WIDTH, SCREEN_HEIGHT,
            color_buf);
    }

    spt::SaveToBmpFile(color_buf, SCREEN_WIDTH, SCREEN_HEIGHT, "result.bmp");

    delete[] color_buf;
    color_buf = NULL;

    //system("pause");
    return 0;
}