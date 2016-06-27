//
// Created by Kanari on 2016/6/27.
//

#ifndef RENDERER_DEPTH_OF_FIELD_H
#define RENDERER_DEPTH_OF_FIELD_H

	Material *wall_material = DiffuseMaterial(0.433, 0.217);
	scene.addObject(new Plane({-3, -3, 3}, {0, 0, -12}, {0, 6, 0}, wall_material, kColor::Red));	// Left
	scene.addObject(new Plane({3, -3, 3}, {0, 6, 0}, {0, 0, -12}, wall_material, kColor::Blue));	// Right
	scene.addObject(new Plane({-3, -3, -9}, {6, 0, 0}, {0, 6, 0}, wall_material, kColor::White));	// Back
	scene.addObject(new Plane({-3, 3, -9}, {6, 0, 0}, {0, 0, 12}, wall_material, kColor::White));	// Top
	scene.addObject(new Plane({-3, -3, 3}, {6, 0, 0}, {0, 0, -12}, wall_material, kColor::White));	// Bottom
	scene.addObject(new Plane({-3, -3, 3}, {0, 6, 0}, {6, 0, 0}, wall_material, kColor::Yellow));	// Front

	scene.addObject(new Sphere({-1.5f, -3 + 1.25f, -7}, 1.25, MirrorMaterial(One * 0.95)));
	scene.addObject(new TexturedSphere({-0.2f, -3 + 1, -0.15}, 1, &(new Material())->setDiffuse(0.5, 0.2).setSpecular(0.7, 0.1, 50).setReflect(One * 0.3), "texture/Ball14.jpg", {-0.5f, 0.5, 0.5}, {-0.5f, -0.5f, 0.5}));
	scene.addObject(new TexturedSphere({1.95, -3 + 1, -4}, 1, &(new Material())->setDiffuse(0.5, 0.2).setSpecular(0.7, 0.1, 50).setReflect(One * 0.3), "texture/Ball 15.jpg", {0.5, 0.5, -0.5f}, {-0.5f, 0.5, -0.5f}));

	scene.addObject(new Plane({-1.5f, 3 - (float)kEps / 2, -5.5f}, {3, 0, 0}, {0, 0, 6}, EmissiveMaterial(One * 7)));	// Light

	scene.setView({0, 0, 15}, {0, 0, 3}, 6);

	scene.setFocus(Plane({-100, -100, 0}, {200, 0, 0}, {0, 200, 0}, wall_material), 0.05);

#endif //RENDERER_DEPTH_OF_FIELD_H
