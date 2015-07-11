#include "stdafx.hpp"
#include "Texture.hpp"

Texture::~Texture() {
	if (_ptr)
		SDL_DestroyTexture(_ptr);
}
