//
// Created by Kanari on 2016/6/27.
//

#ifndef RENDERER_ENVIRONMENT_LIGHTING_H
#define RENDERER_ENVIRONMENT_LIGHTING_H

	Material *wall_material = DiffuseMaterial(0.33, 0.13);
	scene.addObject(new Plane({-3, -3, 3}, {6, 0, 0}, {0, 0, -6}, wall_material, 	kColor::White));	// Bottom

	scene.addObject(new Sphere({-1.5f, -3 + 1.25f, -1}, 1.25, MirrorMaterial(One * 0.95)));
	scene.addObject(new Sphere({-0.2f, -3 + 1, 2}, 1, DielectricMaterial(One, One * 0.9, 1.5)));
	scene.addObject(new TexturedSphere({1.95, -3 + 1, -2}, 1, &(new Material())->setDiffuse(0.3, 0.1).setSpecular(0.5, 0.1, 50).setReflect(One * 0.05), "texture/Ball 15.jpg", {0.5, 0.5, -0.5f}, {-0.5f, 0.5, -0.5f}));

	scene.setView({0, 0, 15}, {0, 0, 3}, 6);

	scene.loadEnvironment("environment/rnl_probe.pfm");

#endif //RENDERER_ENVIRONMENT_LIGHTING_H
