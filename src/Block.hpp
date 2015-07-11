#pragma once

#include "Renderer.hpp"

class Block {
public:
	virtual ~Block() = default;
	virtual void Draw(const Renderer& renderer, SDL_Rect *rect) {};
};
