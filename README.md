# ScottKirvan/Unreal-Visual-Pinball fork

## Branches
- Master  - main dev branch - should be good to pull and work with but may have issues
- dev - development branch - potentially unstable or buggy

## Known Issues
found a crash:  you can right-click on the pinmame dmd window and stop emulation from there - If the game is running in editor, ue will crashes - 
we may need to check that GPController pointer to see if it still has a running game. 
That pointer is valid for some use when emulation is off, but I'm not sure how we can ensure emulation is active.

## Current project
Refactoring and reorganizing.  There's some things that are runtime only and there are some 
features that are editor/dev only in the base VPinMAME API.  I'm working on getting that organized, 
which means sweeping implementation changes at the moment.  Once that's roughly hashed out, I'll be 
focusing on stubbing in and fleshing out and hooking the remaining pars of the PinMAME API to Unreal.

Wanna play along?  Help is welcome - you can contact me on my 
[discord](https://discord.gg/TSKHvVFYxB) server.  I'm cptvideo up there.

