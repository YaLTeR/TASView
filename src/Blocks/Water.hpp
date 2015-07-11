#pragma once

#include "../Block.hpp"
#include "../Renderer.hpp"
#include "../Texture.hpp"

class Water : public Block {
public:
	Water();
	void Draw(const Renderer& renderer, SDL_Rect *rect) override;
	void WaterFrame();
	void UpdateTexture(const Renderer& renderer, int count);

private:
	TTF_Font *_font;

	std::chrono::time_point<std::chrono::high_resolution_clock> _timeOfLastUpdate;
	int _count;

	Texture _texture;
	bool _updateTexture;
	std::mutex _m;
};
