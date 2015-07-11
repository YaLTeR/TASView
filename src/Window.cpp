#include "stdafx.hpp"
#include "Window.hpp"

Window::~Window() {
	if (_window)
		SDL_DestroyWindow(_window);
}
