# Throne

Throne is a modern c++ game engine that is currently a work in progress.

# Examples

The entity component system does not currently have samples, but it has a lot of unit tests. TestCore and TestStandard contain most of the code for the ecs unit tests.  

Samples for the Graphics library are available [here](src/Samples/Graphics).

# Objectives

Throne aims to be as efficient as possible and uses the zero cost abstraction principle as much as possible. Obviously we can't claim to be the most efficient game engine, but we're trying to make it as good as we can. We also want to make it portable and usable on at least Windows and Linux. Making the egnine as modern as possible is one of the goal and to achieve this we use the most modern version of c++ (currently c++20).

One of the ways to achieve performance is using multithreading. Traditionally, games are not the most multithreaded programs, but with the rise of heavily multithreaded cpus we want to use multithreading as much as possible. The choice of using vulkan has mostly been done because of it's modernity, but also because of it's easier use in multithreaded environements.

# [Setup](SETUP.md)
# [Contribute](Contribute.md)
