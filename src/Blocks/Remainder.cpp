#include "../stdafx.hpp"
#include "../Renderer.hpp"
#include "../Texture.hpp"
#include "Remainder.hpp"

Remainder::Remainder() :
	_remainder(0.0), _updateTexture(true)
{
	_font = TTF_OpenFont("font.ttf", 25);
	if (!_font)
		std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << "\n";
}

void Remainder::Draw(const Renderer& renderer, SDL_Rect *rect) {
	if (!_font)
		return;

	bool update = false;
	double rem;

	_m.lock();
	if (_updateTexture || !_texture.Get()) {
		rem = _remainder;
		update = true;
		_updateTexture = false;
	}
	_m.unlock();

	if (update)
		UpdateTexture(renderer, rem);

	if (!_texture.Get())
		return;

	auto dstRect = SDL_Rect{};
	dstRect.x = rect->x;
	dstRect.y = rect->y;
	dstRect.w = _texture.Width();
	dstRect.h = _texture.Height();
	SDL_RenderCopy(renderer, _texture, nullptr, &dstRect);
}

void Remainder::SetRemainder(double remainder) {
	std::lock_guard<std::mutex> g(_m);

	_remainder = remainder;
	_updateTexture = true;
}

void Remainder::UpdateTexture(const Renderer& renderer, double remainder) {
	std::ostringstream oss;
	oss.setf(std::ios::fixed);
	oss.precision(8);
	oss << remainder;
	_texture = renderer.RenderLine(_font, oss.str().c_str(), { 0, 0, 0 });
}
