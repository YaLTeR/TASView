#pragma once

#include "../Block.hpp"
#include "../Renderer.hpp"

class Framerate : public Block {
public:
	Framerate();
	void Draw(const Renderer& renderer, SDL_Rect *rect) override;
	void UpdateTexture(SDL_Renderer *renderer);

private:
	TTF_Font *_font;
	std::chrono::time_point<std::chrono::high_resolution_clock> _lastMeasurement;
	int _frameCounter, _displayedFPS;
	SDL_Texture *_FPSTexture;
	SDL_Rect _FPSDestRect;
};
