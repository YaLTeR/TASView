#pragma once

class Window {
public:
	Window() : _window(nullptr) {};
	Window(SDL_Window *w) : _window(w) {};
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	~Window();

	operator SDL_Window*() { return _window; }
	SDL_Window** operator &() { return &_window; }

	inline void Assign(SDL_Window *w) { _window = w; }
	inline SDL_Window* Get() { return _window; }

private:
	SDL_Window *_window;
};
