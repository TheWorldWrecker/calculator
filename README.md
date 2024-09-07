SDK install file is in repository

<b> DO NOT INSTALL IN PROGRAM FILES (x86) - THE SDK DOES NOT LIKE WHITESPACES IN FILE PATHS </b>

Documentation: https://gist.github.com/fritschy/ef93b3685b8f398e86befaee13d9edf9

The function IsKeyDown is deprecated / deleted, and its alternative GetKeyWait is bugged and doesn't work. However, I found this code snippet at https://community.casiocalc.org/topic/7000-power-graphic-2-development-thread/ which gives you GetKey(). It's the snippet labled "Just add this on top of your code and your game is compitable to the new calculators!"

Compiling will give you a .g1a file which is the add-in. Connect the calculator with your PC via USB, then put the g1a file in the calculator's root directory.

