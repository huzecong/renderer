#include <iostream>
#include <iomanip>
#include <chrono>
#include "Scene.h"
#include "Material.h"

using namespace std;

int main() {

	chrono::time_point<chrono::steady_clock> start_time = chrono::steady_clock::now();

	Scene scene(500, 500);

	DiffuseMaterial *wall_material = new DiffuseMaterial(One * 0.433, One * 0.217);
	scene.addObject(new Plane({-3, -3, 3}, {0, 0, -6}, {0, 6, 0}, wall_material, kColor::Red));	// Left
	scene.addObject(new Plane({3, -3, 3}, {0, 6, 0}, {0, 0, -6}, wall_material, kColor::Blue));	// Right
	scene.addObject(new Plane({-3, -3, -3}, {6, 0, 0}, {0, 6, 0}, wall_material, kColor::White));	// Back
	scene.addObject(new Plane({-3, 3, -3}, {6, 0, 0}, {0, 0, 6}, wall_material, kColor::White));	// Top
	scene.addObject(new Plane({-3, -3, 3}, {6, 0, 0}, {0, 0, -6}, wall_material, kColor::White));	// Bottom
	scene.addObject(new Plane({-3, -3, 3}, {0, 6, 0}, {6, 0, 0}, wall_material, kColor::Yellow));	// Back

	scene.addObject(new Sphere({-1.5f, -3 + 1.25f, -1}, 1.25, new MirrorMaterial(One * 0.9)));
	scene.addObject(new Sphere({0.55, -3 + 1, 2}, 1, new DielectricMaterial(One, One * 0.9, 1.5)));
//	scene.addObject(new Sphere({1.95, -3 + 1, -2}, 1, new MetallicMaterial(One * 0.3, kColor::Grey, One, One * 0.9, 10)), kColor::Grey);
//	scene.addObject(new TexturedSphere({1.95, -3 + 1, -2}, 1, new GlossyMaterial(One * 0.5, One * 0.2), "/Users/kanari/CLionProjects/renderer/texture/Ball 15.jpg", {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}));
	scene.addObject(new TexturedSphere({1.95, -3 + 1, -2}, 1, new MetallicMaterial(One * 0.5, One * 0.2, One * 0.7, One * 0.1, 50), "/Users/kanari/CLionProjects/renderer/texture/Ball 15.jpg", {0.5, 0.5, -0.5f}, {-0.5f, 0.5, -0.5f}));

//	scene.addLight(new AreaLight({-1.5, 3 - (float)kEps, -1.5}, {3, 0, 0}, {0, 0, 3}, One * 45));
	scene.addObject(new Plane({-1.5f, 3 - (float)kEps / 2, -1.5f}, {3, 0, 0}, {0, 0, 3}, new EmissiveMaterial(One * 10), One));	// Light

	scene.setView({0, 0, 15}, {0, 0, 3}, 6);

#ifdef NDEBUG
	scene.render(true);
#else
	scene.render(false);
#endif

	cerr << "Render done with " << kIters << " iterations" << endl;
	cerr << fixed << setprecision(3);
	cerr << "Time consumed: " << (double)chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_time).count() / 1000 << "s" << endl;

	scene.display();

	return 0;
}