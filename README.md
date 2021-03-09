# ScottKirvan/Unreal-Visual-Pinball fork

## Branches
- Master  - main dev branch - should be good to pull and work with but may have issues
- dev - development branch - potentially unstable or buggy

## Known Issues
found a crash:  you can right-click on the pinmame dmd window and stop emulation from there - If the game is running in editor, ue will crashes - 
we may need to check that pController pointer to see if it still has a running game. 
That pointer is valid for some use when emulation is off, but I'm not sure how we can ensure emulation is active.

## Current project
Refactoring and reorganizing.  There's some things that are runtime only and there are some 
features that are editor/dev only in the base VPinMAME API.  I'm working on getting that organized, 
which means sweeping implementation changes at the moment.  Once that's roughly hashed out, I'll be 
focusing on stubbing in and fleshing out and hooking the remaining pars of the PinMAME API to Unreal.

Wanna play along?  Help is welcome - you can contact me on my 
[discord](https://discord.gg/TSKHvVFYxB) server.  I'm cptvideo up there.

# Unreal Pinmame Connection.
## Connect Unreal Engine to Pinmame pinball emulator
I allways have been a fan of pinball machines and I admire the work that has been put into te Pinmame pinball emulator.
Also a great fan of the Unreal game engine I wanted to connect the Unreal engine to the Pinmame emulator.
Pinmame is build as a COM control wich makes interfacing not straightforward because you have to deal with things like “Marschalling”.
After some trail and error I managed to contruct some C++ Blueprint Function classes for Unreal that work very wel during first testing.
So I decided to post a first preview of my project.

More info can be found at https://www.bits4u.nl/unreal-engine-visual-pinball-part-1/.
You can download a copy of a Unreal example project at https://www.bits4u.nl/ASSETS/VPinball.rar
You can contact me at info@bits4u.nl.

Not all the interfaces of Pinmame are present in this release but you can easily extend it yourself by using this code as a template or wait
until i finished implementing the rest.
