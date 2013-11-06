Project 2 README
Created by Martin Wickham & Sam Solovy

Outline of sequences:

Scene 1 : Rocket Beauty / Edit Mode
The is where the edit mode instructions are displayed
because it is the easiest to see the editing changes in theis view.
Camera 1 : Spheroid Camera

Scene 2: Mars w/rocket and starfield
Camera 1: Fly by on the surface of Mars the camera can fully movable.
Camera 2: Spheroid Camera looking at Mars
Camera 3: Fly Camera (no bounds) ctrl keys to move up/down adn has mouse movement.
Camera 4: Chase Camera, follows the rocket, and can rotate around
	 the rocket. The camera will always be able to see the rocket.

Scene 3: Starfield
Camera 1: Spheroid Camera from out side the starfield.

Hidden Scene F2:
Mars without the rocket in orbit


Miscellaneous features:


Reset function: 'r' - resets the animations and rocket,
			does not change the cameras.

Code Architecture:
Our codebase is extremely flexible due to our extensive use of Decorators [GOF].
We effectively build an executable stack on top of each Mesh, which is used
to alter the modelview matrix on its way down, as well as alter global state
variables.  The Graphics class is a Singleton [GOF], and accumulates this global
state which it uses to initialize the shaders.

[GOF] <Design Patterns: Elements of Reusable Object-Oriented Software, Gamma et.al.>


Pause function: 'p' - pauses the animations.

Movable light source: 'kl;o'- the light souce can be moved in world space.

Texture of Mars - mars is textured with a picture of mars.

ADS lighting - Using phong illumination at each fragment. 

SPLINE EDITOR:
The head of the rocket uses a spine to create a surface of revolution that will replace
the head. There is five movable points, the middle points can be rotated, moved, and scaled.
The top and bottom points can be scaled.		   