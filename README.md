# DaTo Engine by Toni Romanos & Dani Mariages.
DaTo Engine is a videogame engine developed for the MOTORS DE VIDEOJOCS course at CITM.

This engine is developed by:
- Dani Mariages - [DaniMariages](<https://github.com/DaniMariages>)
- Toni Romanos - [Toto-RR](<https://github.com/Toto-RR>)
- Github repository: [DaTo Engine](<https://github.com/DaniMariages/DaTo-Engine>)

## Keybinds:
- Left Control --> Center selected object
- Left shift --> Duplicate movement speed
- Right Click --> FPS camera
- Left Alt + Left Click --> Orbital camera
- Right Shift --> Top view
- Up Arrow --> Front view
- Right Arrow --> Right view
- Left Arrow --> Left view
- Down Arrow --> Back view
- Scroll up --> Zoom in
- Scroll down --> Zoom out
  
### FPS movement:
- W --> Move forward
- A --> Move left
- S --> Move backwards
- D --> Move right
- R --> Move up
- F --> Move down
  
### While selected (Guizmo):
- W --> Position
- R --> Scale
- E --> Rotate
- T --> Universal
  
### Additional functionalities
- Top, sides, back and front view (Reworked).
- Quick options when you right click on the Game Objects displayed in the Hierarchy
- Reparent game objects one is selected and dragged to another one

## Features v0.1
- Loading models and textures with Assimp and DevIL.
- Render models and textures with OpenGL.
- .fbx support for models (at the moment).
- .png and .dds support for textures (for now).
- Unity-like camera controls.
- Hierarchy system.
- Drag & Drop for all supported formats.
- Game Objects with texture, mesh and transform components.

## Features v0.5
- Game objects can have a camera component.
- The Transform component of the Game Objects works, and can be modified both from the Inspector and with the Guizmos. 
- Hierarchy:
  - Fast options to Delete, Hide and Create Empty Child.
  - Reparent by dragging a Game Object into another.
- Game timer that can be played, paused, resumed and stopped.
- All meshes use a bounding volume (AABB) and can be discarded using Frustum Culling.
- Game Objects can be picked from the world using the mouse.
- Mouse Picking can be deactivated for a selected object.
- The editor has two windows, one with the scene (editor view) and another captured from a Game object witha camera component (Main Game Camera), each one has its own framebuffer and the user can visualize both at the same time.
- All normals move with the model.
- Skybox added.
- Warning window before deleting a game object or scene, with a checkbox for "Do not show again".
- Game Object can be renamed from Inspector.
- If an object is repeated, the name will change to: Name + (number of repetitions).

## Libraries
- [Assimp](<https://github.com/assimp/assimp.git>)
- [DevIL](<https://openil.sourceforge.net>)
- [Glew](<https://glew.sourceforge.net>)
- [ImGui](<https://github.com/ocornut/imgui.git>)
- [MathGeoLib](<https://github.com/juj/MathGeoLib.git>)
- [SDL](<https://www.libsdl.org>)
- [Parson](<https://github.com/kgabis/parson>)
- [PCG](<https://www.pcg-random.org>)

## Photos and references

![CapturaReadme](https://github.com/DaniMariages/DaTo-Engine/assets/99719601/fbd3d4fb-65b5-4bae-8e8a-eecb906d76ed)

## Known bugs, missing features and others
### Missing features:
- Street Environment is not loaded at start (is commented), because:
  - The Engine is not able to set textures to a specific game object, the texture is setted to the last Game Object added to the Scene.
  - The Street Environment models were not positioned correctly.
- Custom File Format, Serialization and Resources are not completed due to time constraints.
  - In return, every effort has been made to polish the rest of the engine as much as possible.
