#include <iostream>
#include <iomanip>
#include <chrono>
#include "Scene.h"
#include "Material.h"

using namespace std;

int main() {

	chrono::time_point<chrono::steady_clock> start_time = chrono::steady_clock::now();

	Scene scene(500, 500);
	scene.addObject(new Sphere({-2, -2, -2}, 1, kMaterial::kPlastic(kColor::Grey)));
	scene.addObject(new Sphere({0, 0.5, -2}, 1, kMaterial::kPlastic(kColor::Red)));
//	scene.addObject(new Sphere({-1, 3, 0}, 1, kMaterial::kPlastic(kColor::Green)));
	scene.addObject(new Sphere({1, 1, 2}, 1, new DielectricMaterial(Zero, One * 0.9, One * 0.95, 1.5)));
	scene.addObject(new Sphere({2, -2, -2}, 1, new MirrorMaterial(Zero, One * 0.8)));

	auto wall_material = [](const Color &col) {
		return new DiffuseMaterial(col * 0.05, One * 0.217 + col * 0.433, Zero, 0.0);
//		return new DiffuseMaterial(col * 0.1, col.mul({0.43, 0.47, 0.54}), kColor::Black, 0.0);
	};

	scene.addObject(new Plane({-3, -3, 3}, {0, 0, -6}, {0, 6, 0}, wall_material(kColor::Red)));	// Left
	scene.addObject(new Plane({3, -3, 3}, {0, 6, 0}, {0, 0, -6}, wall_material(kColor::Green)));	// Right
	scene.addObject(new Plane({-3, -3, -3}, {6, 0, 0}, {0, 6, 0}, wall_material(kColor::White)));	// Back
	scene.addObject(new Plane({-3, 3, -3}, {6, 0, 0}, {0, 0, 6}, wall_material(kColor::Blue)));	// Top
	scene.addObject(new Plane({-3, -3, 3}, {6, 0, 0}, {0, 0, -6}, wall_material(kColor::Grey)));	// Bottom

	scene.addLight(new AreaLight({-0.25f, 1.5, 6}, {0, 0.5, 0}, {0.5, 0, 0}, One * 60));
	scene.addLight(new AreaLight({-3 + (float)kEps, -3, 3}, {0, 0, -6}, {0, 2, 0}, One * 60));
	scene.addObject(new Plane({-3 + (float)kEps / 2, -3, 3}, {0, 0, -6}, {0, 2, 0}, new Material(One * 10)));	// Left

	scene.setView({0, 0, 10}, {0, 0, 3}, 6);

	scene.render();

	cerr << "Render done with " << kIters << " iterations" << endl;
	cerr << fixed << setprecision(3);
	cerr << "Time consumed: " << (double)chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_time).count() / 1000 << "s" << endl;

	scene.display();

	return 0;
}