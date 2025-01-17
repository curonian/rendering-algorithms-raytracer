#include <math.h>
#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Console.h"

#include "PointLight.h"
#include "TriangleMesh.h"
#include "Lambert.h"

void makeBunnyScene();
void makeTeapotScene();
void makeSphereScene();
//This function is defined in main.cpp
void makeMeshObjs(TriangleMesh* mesh, Material* mat);

void makeBunnyScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);

    g_scene->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    
    // set up the camera
    g_camera->setEye(Vector3(-2, 3, 5));
    g_camera->setLookAt(Vector3(-.5, 1, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(-3, 15, 3));
    light->setColor(Vector3(1, 1, 1));
    light->setPower(500);
    g_scene->addLight(light);

    Material* mat = new Lambert(Vector3(1.0f));

    TriangleMesh * bunny = new TriangleMesh;
    bunny->load("Models/bunny.obj");
	makeMeshObjs(bunny,mat);
    
    // create the floor triangle
    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(  0, 0,  10));
    floor->setV2(Vector3( 10, 0, -10));
    floor->setV3(Vector3(-10, 0, -10));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));
    
    Object* t = new Object;
    t->setIndex(0);
    t->setMesh(floor);
    t->setMaterial(mat); 
    g_scene->addObject(t);
    
    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void makeTeapotScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);

    g_scene->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    
    // set up the camera
    g_camera->setEye(Vector3(-2, 3, 5));
    g_camera->setLookAt(Vector3(-.5, 1, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(-3, 15, 3));
    light->setColor(Vector3(1, 1, 1));
    light->setPower(500);
    g_scene->addLight(light);

    Material* mat = new Lambert(Vector3(1.0f));

    TriangleMesh * teapot = new TriangleMesh;
    teapot->load("Models/teapot.obj");
	makeMeshObjs(teapot,mat);
    
    // create the floor triangle
    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(  0, 0,  10));
    floor->setV2(Vector3( 10, 0, -10));
    floor->setV3(Vector3(-10, 0, -10));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));
    
    Object* t = new Object;
    t->setIndex(0);
    t->setMesh(floor);
    t->setMaterial(mat); 
    g_scene->addObject(t);
    
    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void makeSphereScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);

    g_scene->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    
    // set up the camera
    g_camera->setEye(Vector3(-2, 3, 5));
    g_camera->setLookAt(Vector3(-.5, 1, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(-3, 15, 3));
    light->setColor(Vector3(1, 1, 1));
    light->setPower(500);
    g_scene->addLight(light);

    Material* mat = new Lambert(Vector3(1.0f));

    TriangleMesh * sphere = new TriangleMesh;
    sphere->load("Models/sphere.obj");
	makeMeshObjs(sphere,mat);
    
	TriangleMesh * floor = new TriangleMesh;
	floor->createSingleTriangle();
	floor->setV1(Vector3(  0, 0,  10));
	floor->setV2(Vector3( 10, 0, -10));
	floor->setV3(Vector3(-10, 0, -10));
	floor->setN1(Vector3(0, 1, 0));
	floor->setN2(Vector3(0, 1, 0));
	floor->setN3(Vector3(0, 1, 0));

	Object* t = new Object;
    
    // let objects do pre-calculations if needed
    g_scene->preCalc();
}