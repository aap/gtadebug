Implements various kinds of debugging/cheating/manipulation for GTA III, VC and SA.
Use it with the debugmenu.

Not all games have the same features implemented!

How to install
==============
Have the file for the respective game loaded by a dll/asi loader (you may have to change
the file extension to "asi"). Debugmenu.dll (separate plugin) should be in the game root.

Compatible with 1.0 US executables.



III
===

Menu
  - not all vehicles IDs will be spawned
  - cheats are named after the functions in the executable that handle them

The following debug features were left in the game but only
accessible from the second pad, I patched them to work with
keyboard & mouse controls as well.
  Controls (Pad controls refer to the second pad):
  - Ctrl-D or Circle toggle the debug camera
  Debug cam controls (Pad controls also work outside the debug camera):
  - Ctrl-H or Start toggle HUD visibility
  - Ctrl-T or R1 advances the time
  - Ctrl-W or R3 advances the weather
  - Ctrl-C switches between camera/player control
  - Dragging the mouse or moving the left stick rotates the camera
  - W/S or Square/Cross move the camera forward/backward
  - Enter or R2 teleport the player to the camera position

Vice City
=========

Menu
  - cheats are inlined which made them awkward to put into the menu

Same general debug camera controls as in III but missing:
 - advance time
 - advance weather

San Andreas
===========

Menu
  - Lots of postfx tweakables. Not all of them work without using skygfx.
  - invincibility doesn't really work well because you can easily die when
    falling from a great height

Same general debug camera controls as in III but missing:
 - hide HUD
 - advance time
 - advance weather
