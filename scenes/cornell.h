//
// Created by Kanari on 2016/6/25.
//

#ifndef RENDERER_CORNELL_H
#define RENDERER_CORNELL_H

	Material *wall_material = DiffuseMaterial(0.433, 0.217);
	scene.addObject(new Plane({-3, -3, 3}, {0, 0, -6}, {0, 6, 0}, wall_material, kColor::Red));	// Left
	scene.addObject(new Plane({3, -3, 3}, {0, 6, 0}, {0, 0, -6}, wall_material, kColor::Blue));	// Right
	scene.addObject(new Plane({-3, -3, -3}, {6, 0, 0}, {0, 6, 0}, wall_material, kColor::White));	// Back
	scene.addObject(new Plane({-3, 3, -3}, {6, 0, 0}, {0, 0, 6}, wall_material, kColor::White));	// Top
	scene.addObject(new Plane({-3, -3, 3}, {6, 0, 0}, {0, 0, -6}, wall_material, kColor::White));	// Bottom
	scene.addObject(new Plane({-3, -3, 3}, {0, 6, 0}, {6, 0, 0}, wall_material, kColor::Yellow));	// Back

	scene.addObject(new Sphere({-1.5f, -3 + 1.25f, -1}, 1.25, MirrorMaterial(One * 0.95)));
	scene.addObject(new Sphere({0.55, -3 + 1, 2}, 1, DielectricMaterial(One, One * 0.9, 1.5)));
//	scene.addObject(new Sphere({1.95, -3 + 1, -2}, 1, new MetallicMaterial(One * 0.3, kColor::Grey, One, One * 0.9, 10)), kColor::Grey);
//	scene.addObject(new TexturedSphere({1.95, -3 + 1, -2}, 1, new GlossyMaterial(One * 0.5, One * 0.2), "/Users/kanari/CLionProjects/renderer/texture/Ball 15.jpg", {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}));
	scene.addObject(new TexturedSphere({1.95, -3 + 1, -2}, 1, &(new Material())->setDiffuse(0.5, 0.2).setSpecular(0.7, 0.1, 50).setReflect(One * 0.3), "texture/Ball 15.jpg", {0.5, 0.5, -0.5f}, {-0.5f, 0.5, -0.5f}));

	scene.addObject(new Plane({-1.5f, 3 - (float)kEps / 2, -1.5f}, {3, 0, 0}, {0, 0, 3}, EmissiveMaterial(One * 10)));	// Light

	scene.setView({0, 0, 15}, {0, 0, 3}, 6);

#endif //RENDERER_CORNELL_H
