#pragma once


enum class InputState
{
	JustPressed,
	JustReleased,
	Pressed,
};

enum class InputKeybinds
{
	//keyboard inputs
	KEYBOARD_BEGIN,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	KEYBOARD_END, 

	//controller inputs
	CONTROLLER_BEGIN,
	DPAD_UP,
	DPAD_DOWN,
	DPAD_LEFT,
	DPAD_RIGHT,

	BUTTON_NORTH,
	BUTTON_SOUTH,
	BUTTON_EAST,
	BUTTON_WEST,
	CONTROLLER_END,
};