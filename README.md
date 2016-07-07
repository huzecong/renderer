# Naïve Renderer

This repo contains a naïve renderer that failed in the attempt to be photo-realistic. It is the final project for the Computer Graphics course.

Features implemented (listed in order of decreasing difficulty):

- Monte-Carlo-based path tracing
- Reflection, refraction, and diffusion
- Spherical texture mapping
- Image-based lighting (with strange artefacts)
- Anti-aliasing and depth-of-field

The following are some carefully-chosen above-average pictures rendered by this renderer:

- Cornell box with mirror, glass, and textured glossy spheres: <img src="./rendered_images/cornell_bright.png" width="100">![cornell_bright](./rendered_images/cornell_bright.png)


- Cornell box with mirror, glass, and metallic spheres: ![cornell_metallic](./rendered_images/cornell_metallic.png)


- Cornell box demonstrating depth-of-field effect: ![depth_of_field](./rendered_images/depth_of_field.png)


- Environment lighting using image-based lighting (with obvious artefacts) that took the least rendering time but looked the most impressive: ![environment_light](./rendered_images/environment_light.png)

#### References:

- Daniel Ritchie, Lita Cho. The Monte Carlo Project - A Path Tracer. http://web.stanford.edu/~dritchie/path/index.html.
- Scratchpixel. Introduction to Shading: Re ection, Refraction (Transmission) andFresnel. http://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel.
- Scratchpixel. The Phong Model, Introduction to the Concepts of Shader,Re ection Models and BRDF. http://www.scratchapixel.com/lessons/3d-basic-rendering/phong-shader-BRDF.

