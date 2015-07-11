#pragma once

#include "../Block.hpp"
#include "../Renderer.hpp"

class Time : public Block {
public:
	Time();
	void Draw(const Renderer& renderer, SDL_Rect *rect) override;
	void SetTime(int hours, int minutes, int seconds, int milliseconds);
	void UpdateTexture(SDL_Renderer * renderer, int hours, int minutes, int seconds, int milliseconds);

private:
	TTF_Font *_font;

	int _hours;
	int _minutes;
	int _seconds;
	int _milliseconds;

	SDL_Texture *_texture;
	SDL_Rect _dstRect;
	bool _updateTexture;
	std::mutex _m;
};
