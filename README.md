# OpenGL Galaga / Space Invaders

An OpenGL game inspired by the classic arcade titles **Galaga** and **Space Invaders**, developed as the final project for the Computer Graphics course of the B.Sc. in Informatics Engineering at the University of Coimbra.

The objective of the project was to apply fundamental computer graphics concepts—including geometric transformations, camera systems, lighting, and real-time rendering—to develop a complete interactive application from scratch.

---

## Features

- Real-time rendering using OpenGL
- Camera manipulation
- Geometric transformations
- Sprite animation
- Collision detection
- Enemy wave management
- Dynamic lighting
- Score and game state management

---

## Gameplay

While inspired by the original arcade games, several mechanics were intentionally redesigned.

### Enemy Health

Instead of being destroyed with a single hit, enemies have multiple health points. Their remaining health is visually represented through progressive colour changes.

### Controlled Enemy Formation

Enemy movement is constrained to a 7×9 grid, producing more structured formations and allowing encounters to be designed rather than generated randomly.

### Predefined Waves

Enemy waves are handcrafted instead of randomly generated, allowing progressive increases in difficulty and more deliberate gameplay pacing.

---

## Planned Feature

A mechanic was designed but not implemented due to time constraints.

The game world would consist of multiple connected playfields. Moving beyond the left or right boundary would transition the player to a neighbouring board while enemy formations continued evolving independently. This would require the player to strategically alternate between boards to prevent enemies from advancing too far on either side.

---

## Graphics Concepts Implemented

- Model, View and Projection transformations
- Camera manipulation
- Linear algebra for geometric transformations
- Sprite rendering
- Collision detection
- Animation
- Lighting
- Real-time rendering

---

## Technologies

- C++
- OpenGL

---

## Learning Outcomes

Through this project I gained practical experience with:

- Modern OpenGL programming
- Interactive game development
- Computer graphics pipelines
- Object-oriented software design
- Real-time rendering
- Collision detection
- Graphics programming using linear algebra

---

## Future Improvements

- Multiple connected game boards
- Additional enemy behaviours
- Particle effects
- Audio system
- Power-ups
- Improved visual effects
