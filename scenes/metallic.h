//
// Created by Kanari on 2016/6/27.
//

#ifndef RENDERER_METALLIC_H
#define RENDERER_METALLIC_H

	Material *wall_material = DiffuseMaterial(0.433, 0.217);
	scene.addObject(new Plane({-3, -3, 3}, {0, 0, -6}, {0, 6, 0}, wall_material, kColor::Red));	// Left
	scene.addObject(new Plane({3, -3, 3}, {0, 6, 0}, {0, 0, -6}, wall_material, kColor::Blue));	// Right
	scene.addObject(new Plane({-3, -3, -3}, {6, 0, 0}, {0, 6, 0}, wall_material, kColor::White));	// Back
	scene.addObject(new Plane({-3, 3, -3}, {6, 0, 0}, {0, 0, 6}, wall_material, kColor::White));	// Top
	scene.addObject(new Plane({-3, -3, 3}, {6, 0, 0}, {0, 0, -6}, wall_material, kColor::White));	// Bottom
	scene.addObject(new Plane({-3, -3, 3}, {0, 6, 0}, {6, 0, 0}, wall_material, kColor::Yellow));	// Back

	scene.addObject(new Sphere({-1.5f, -3 + 1.25f, -1}, 1.25, MirrorMaterial(0.95 * One)));
	scene.addObject(new Sphere({0.55, -3 + 1, 2}, 1, DielectricMaterial(1.0 * One, 0.9 * One, 1.5)));
	scene.addObject(new Sphere({1.95, -3 + 1, -2}, 1, MetallicMaterial(0.3, 0.2, 0.8, 0.2, 10), One * 0.3));

	scene.addObject(new Plane({-1.5f, 3 - (float)kEps / 2, -1.5f}, {3, 0, 0}, {0, 0, 3}, EmissiveMaterial(One * 10), One));	// Light

	scene.setView({0, 0, 15}, {0, 0, 3}, 6);

#endif //RENDERER_METALLIC_H
