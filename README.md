# RadonCore-Physics
Our way of trying to help the open source and the linux community.


# SUDDEN CHANGE

There is a heisenbug in the bvh.cl kernel, that I don't seem to be able to fix. When I add the debug comments, etc, it starts working, but then I can't use the bvh itself.But when I delete the debug, it suddenly doesn't work,and only detects along the y axis, for no reason at all. I've been trying to fix this for the last 4.5 hours, but I didn't find any solutions. Please consider this as a cryout for help.

# General

This is a physics engine, that's aiming to compete with AnSys' AnSys Motion multibody simulation system. The software is still being updated and is under 
building, so more advanced features, finished UI-s, better project sim. And with all of this said, I find it important to mention: please notify me, if you find bugs
during use, because I might not be able to catch all of them alone.
The working, and updated branch is the c++ folder, c++ and c# is aiming to be the windows support part! c++ and c# is currently deprecated due to lack of knowledge and 
demand.

(You'll find instructions to the software in the USE.html)

# For hackclub and the reasons, why I'm building this project

My big dream is to make an open source package for simulations, just like FreeCAD did it with FEA and parametric modelling, assemblies, etc. Because, although I know about other, very formidable options for physics simulations, I want this to not just provide a standalone physics simulation program, but to be connectable to FreeCAD later on to expand the range of options that someone can do with their 3d models. My second reason to build this is to make an engine that leaves as little performance on the table as possible, because my main issue with today's software is that it's unoptimized, and I want to be part of the group, who launch actually valuable and efficient programs/projects. And the final reason: I wanted to get into HPC, because I find it cool and useful.

# System requirements

The software is only known to work on linux 7.0, and as of 2026.08.31., Windows 11, or any kind of Windows support hasn't been confirmed.
You may ask: why do you not support Windows, when most people use that?
The answer is simpler than you think: OpenCL support is not the best on Windows, because it doesn't expose as much and it's harder to handle.
However, apart from the first part of the project, which I have tested on windows 11 actually, with minimal opengl, everything is uncertain.
Especially because the c++ syntax is supposedly cross platform, and you might be able to get it working, but I wouldn't recommend dealing with that, I don't think that's worth it.

Required API-s/ API support:
OpenCL 1.2
OpenGL 3.3
SDL3


# OpenCL runtime is a driver and spec based shared library, thus the release(s) doesn't provide it, you need to install it manually.
# In conclusion: it's better to build the project from source with cmake.

# Current state

-Basic working gliding
-OpenCL kernels working reliably
-OpenGL and controls working reliably
-rewritten triangulator and obj parser supporting 

# Bug fixes to be introduced:

-multiple bodies with the same name in a project are conflicting
-the whitening behind the select popup
-grid not continuing towards infinity in 1 direction

# What will come in the next commit(hopefully)?

-gravity
-rotation around a body's own axles
-vulkan for BVH
-proper multiprocessing functions
-some support for windows
-some optimization both in case of code readability and efficiency
-materials(large chunk of it is already done, I just need to connect the nodes)
-a largely working UI with buttons that actually do something
-an ini for settings
>>>>>>> beta

This currently is a stable version of radon.
