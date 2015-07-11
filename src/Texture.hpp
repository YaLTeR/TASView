#pragma once

class Texture {
public:
	Texture(SDL_Texture *ptr, int width, int height) :
		_ptr(ptr), _width(width), _height(height) {};

	Texture() : _ptr(nullptr), _width(0), _height(0) {};

	Texture(Texture&& o) :
		_ptr(o._ptr),
		_width(o._width),
		_height(o._height)
	{
		o._ptr = nullptr;
	};

	Texture& operator=(Texture&& o) {
		if (this != &o) {
			if (_ptr)
				SDL_DestroyTexture(_ptr);

			_ptr = o._ptr;
			_width = o._width;
			_height = o._height;

			o._ptr = nullptr;
		}
		return *this;
	}

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	~Texture();

	inline operator SDL_Texture*() const { return _ptr; }
	inline SDL_Texture* Get() const      { return _ptr; }
	inline int Width() const             { return _width; }
	inline int Height() const            { return _height; }

private:
	SDL_Texture *_ptr;
	int _width, _height;
};
