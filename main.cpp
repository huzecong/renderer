#include <iostream>
#include <iomanip>
#include <chrono>
#include "Scene.h"
#include "Material.h"

using namespace std;

int main() {

	chrono::time_point<chrono::steady_clock> start_time = chrono::steady_clock::now();

	Scene scene(500, 500);

	auto wall_material = [](const Color &col) {
		return new DiffuseMaterial(col * 0.05, One * 0.217 + col * 0.433, Zero, 0.0);
//		return new DiffuseMaterial(col * 0.1, col.mul({0.43, 0.47, 0.54}), kColor::Black, 0.0);
	};

	scene.addObject(new Plane({-3, -3, 3}, {0, 0, -6}, {0, 6, 0}, wall_material(kColor::Red)));	// Left
	scene.addObject(new Plane({3, -3, 3}, {0, 6, 0}, {0, 0, -6}, wall_material(kColor::Blue)));	// Right
	scene.addObject(new Plane({-3, -3, -3}, {6, 0, 0}, {0, 6, 0}, wall_material(kColor::White)));	// Back
	scene.addObject(new Plane({-3, 3, -3}, {6, 0, 0}, {0, 0, 6}, wall_material(kColor::White)));	// Top
	scene.addObject(new Plane({-3, -3, 3}, {6, 0, 0}, {0, 0, -6}, wall_material(kColor::White)));	// Bottom
	scene.addObject(new Plane({-3, -3, 3}, {0, 6, 0}, {6, 0, 0}, wall_material(kColor::Yellow)));	// Back

	scene.addObject(new Sphere({-1.25, -3 + 1.25, -1}, 1.25, new MirrorMaterial(Zero, One * 0.9)));
	scene.addObject(new Sphere({0.65, -3 + 1, 1.5}, 1, new DielectricMaterial(Zero, One, One * 0.9, 1.5)));
	scene.addObject(new Sphere({1.95, -3 + 1, -2}, 1, kMaterial::kPlastic(kColor::Grey)));

//	scene.addLight(new AreaLight({-1.5, 3 - (float)kEps, -1.5}, {3, 0, 0}, {0, 0, 3}, One * 45));
	scene.addObject(new Plane({-1.5, 3 - (float)kEps / 2, -1.5}, {3, 0, 0}, {0, 0, 3}, new Material(One * 10)));	// Light

	scene.setView({0, 0, 15}, {0, 0, 3}, 6);

	scene.render(true);

	cerr << "Render done with " << kIters << " iterations" << endl;
	cerr << fixed << setprecision(3);
	cerr << "Time consumed: " << (double)chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_time).count() / 1000 << "s" << endl;

	scene.display();

	return 0;
}