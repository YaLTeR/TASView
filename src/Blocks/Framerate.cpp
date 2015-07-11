#include "../stdafx.hpp"
#include "../Renderer.hpp"
#include "Framerate.hpp"

Framerate::Framerate() :
	_lastMeasurement(std::chrono::high_resolution_clock::now()),
	_frameCounter(0),
	_displayedFPS(0),
	_FPSTexture(nullptr),
	_FPSDestRect(SDL_Rect())
{
	_font = TTF_OpenFont("font.ttf", 20);
	if (!_font)
		std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << "\n";
}

void Framerate::Draw(const Renderer& renderer, SDL_Rect *rect) {
	if (!_font)
		return;

	auto curTime = std::chrono::high_resolution_clock::now();
	auto secondsPassed = std::chrono::duration_cast<std::chrono::seconds>(curTime - _lastMeasurement);
	if (secondsPassed.count()) {
		_displayedFPS = _frameCounter;
		_frameCounter = 0;
		_lastMeasurement += std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(secondsPassed);
		UpdateTexture(renderer);
	} else if (!_FPSTexture) {
		UpdateTexture(renderer);
	}
	_frameCounter++;

	if (!_FPSTexture)
		return;

	SDL_RenderCopy(renderer, _FPSTexture, nullptr, &_FPSDestRect);
}

void Framerate::UpdateTexture(SDL_Renderer *renderer) {
	if (_FPSTexture)
		SDL_DestroyTexture(_FPSTexture);

	auto fpsString = boost::lexical_cast<std::string>(_displayedFPS);
	auto surface = TTF_RenderText_Blended(_font, fpsString.c_str(), SDL_Color{ 0, 0, 0 });
	if (!surface)
		return;

	_FPSDestRect.w = surface->w;
	_FPSDestRect.h = surface->h;
	_FPSTexture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);
}
