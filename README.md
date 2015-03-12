GLDR
====

GLDR stands for (Open)GL Done Right.

This is low level library focused on bringing OpenGL into the modern C++ world. It will be focused on provideing a focused set of utility classes that will extened the work done by glload to provide modern C++ semantics.

GLDR will not provide any sort of context handling, and will be assuming the you have already initialised glload. It is aiming to be as 'header only' as possible, and this fits well with the scope of this project. You will not find any complex 3D animation classes here or even image loading classes; at least, not to start with.

This project was spawned on the back a few people getting just a little bit fed up with all reworking the same basic utility classes in isolation, by coming together we aim to provided a library that you just cannot work without. We will be striving to make the code the very best it can be, aiming for that perfect balance between ease of use and flexibility. This will mean that, especially during the early stages, if we have to change more or less everything to improve it, we will. Backwards compatibility is not going to hold us back, like it has OpenGl itself. So please do jump on board and make use of GLDR, but be prepared to make drastic change to your code if you want to keep up with us.

Principles
==========

It turns out that when X (as in X11, the display server stuff) was in it's early days two clever chaps (Bob Scheifler and Jim Gettys) set out some principles to develop against. X has mostly kept to these principles, and it makes sense why, they are good principles, that we will also try to stick to with gldr. Modified to suit our needs and taken with a pinch of salt, they are as follows: 

* Do not add new functionality unless you know of some real application that will require it.
* It is as important to decide what a system is not as to decide what it is. Do not serve all the world's needs; rather, make the system extensible so that additional needs can be met in an upwardly compatible fashion.
* The only thing worse than generalizing from one example is generalizing from no examples at all.
* If a problem is not completely understood, it is probably best to provide no solution at all.
* If you can get 90 percent of the desired effect for 10 percent of the work, use the simpler solution.
* Isolate complexity as much as possible.

Licence
=======

This is released under [MIT licence](http://opensource.org/licenses/MIT).
If you make anything based off of or using this, we'd love to hear about it!
