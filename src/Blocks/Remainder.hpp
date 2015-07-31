#pragma once

#include "../Block.hpp"
#include "../Renderer.hpp"
#include "../Texture.hpp"

class Remainder : public Block {
public:
	Remainder();
	void Draw(const Renderer& renderer, SDL_Rect *rect) override;
	void SetRemainder(double remainder);
	void UpdateTexture(const Renderer& renderer, double remainder);

private:
	TTF_Font *_font;

	double _remainder;

	Texture _texture;
	bool _updateTexture;
	std::mutex _m;
};
