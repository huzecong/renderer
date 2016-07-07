#include <iostream>
#include <iomanip>
#include <chrono>
#include "Scene.h"
#include "Material.h"

using namespace std;

int main() {
	chrono::time_point<chrono::steady_clock> start_time = chrono::steady_clock::now();

	Scene scene(1000, 1000);

#include "scenes/environment_lighting.h"

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
