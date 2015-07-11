#include "../stdafx.hpp"
#include "../Renderer.hpp"
#include "../Texture.hpp"
#include "Water.hpp"

Water::Water() :
	_timeOfLastUpdate(std::chrono::high_resolution_clock::now()),
	_count(0), _updateTexture(true)
{
	_font = TTF_OpenFont("font.ttf", 25);
	if (!_font)
		std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << "\n";
}

void Water::Draw(const Renderer& renderer, SDL_Rect *rect) {
	if (!_font)
		return;

	auto now = std::chrono::high_resolution_clock::now();
	bool update = false;
	int c;

	_m.lock();
	auto secondsPassed = std::chrono::duration_cast<std::chrono::seconds>(now - _timeOfLastUpdate).count();
	if (secondsPassed && _count) {
		_count = 0;
		_updateTexture = true;
	}

	if (_updateTexture || !_texture.Get()) {
		c = _count;
		update = true;
		_updateTexture = false;
	}
	_m.unlock();

	if (update)
		UpdateTexture(renderer, c);

	if (!_texture.Get())
		return;

	auto dstRect = SDL_Rect{};
	dstRect.x = rect->x;
	dstRect.y = rect->y;
	dstRect.w = _texture.Width();
	dstRect.h = _texture.Height();
	SDL_RenderCopy(renderer, _texture, nullptr, &dstRect);
}

void Water::WaterFrame() {
	std::lock_guard<std::mutex> g(_m);

	_count++;
	_timeOfLastUpdate = std::chrono::high_resolution_clock::now();
	_updateTexture = true;
}

void Water::UpdateTexture(const Renderer& renderer, int count) {
	_texture = renderer.RenderLine(_font, boost::lexical_cast<std::string>(count), { 0, 0, 150 });
}
