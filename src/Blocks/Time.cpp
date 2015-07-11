#include "../stdafx.hpp"
#include "../Renderer.hpp"
#include "Time.hpp"

Time::Time() :
	_hours(0), _minutes(0), _seconds(0), _milliseconds(0),
	_texture(nullptr),
	_dstRect(SDL_Rect()),
	_updateTexture(true)
{
	_font = TTF_OpenFont("font.ttf", 60);
	if (!_font)
		std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << "\n";
}

void Time::Draw(const Renderer& renderer, SDL_Rect *rect) {
	if (!_font)
		return;

	int hr, min, sec, mil;
	bool update = false;

	_m.lock();
	if (_updateTexture || !_texture) {
		hr = _hours;
		min = _minutes;
		sec = _seconds;
		mil = _milliseconds;
		update = true;

		_updateTexture = false;
	}
	_m.unlock();

	if (update)
		UpdateTexture(renderer, hr, min, sec, mil);
	if (!_texture)
		return;

	_dstRect.x = rect->x + (rect->w - _dstRect.w) / 2;
	_dstRect.y = rect->y + (rect->h - _dstRect.h) / 2;
	SDL_RenderCopy(renderer, _texture, nullptr, &_dstRect);
}

void Time::SetTime(int hours, int minutes, int seconds, int milliseconds) {
	std::lock_guard<std::mutex> g(_m);

	_hours = hours;
	_minutes = minutes;
	_seconds = seconds;
	_milliseconds = milliseconds;
	_updateTexture = true;
}

void Time::UpdateTexture(SDL_Renderer *renderer, int hours, int minutes, int seconds, int milliseconds) {
	if (_texture)
		SDL_DestroyTexture(_texture);

	std::ostringstream oss;
	oss.fill('0');
	if (hours)
		oss << hours << ":" << std::setw(2);
	if (minutes)
		oss << minutes << ":" << std::setw(2);
	oss << seconds << "." << std::setw(3) << milliseconds;
	auto surface = TTF_RenderText_Blended(_font, oss.str().c_str(), SDL_Color{ 0, 0, 0 });
	//auto surface = TTF_RenderText_Shaded(_font, oss.str().c_str(), SDL_Color{ 0, 0, 0 }, SDL_Color{ 255, 255, 255 });
	//auto surface = TTF_RenderText_Solid(_font, oss.str().c_str(), SDL_Color{ 0, 0, 0 });
	if (!surface)
		return;

	_dstRect.w = surface->w;
	_dstRect.h = surface->h;
	_texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);
}
