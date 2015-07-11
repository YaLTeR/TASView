#pragma once

#include "Texture.hpp"

class Renderer {
public:
	Renderer() : _renderer(nullptr) {};
	Renderer(SDL_Renderer *r) : _renderer(r) {};
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer();

	operator SDL_Renderer*() const { return _renderer; }
	SDL_Renderer** operator &() { return &_renderer; }

	inline void Assign(SDL_Renderer *r) { _renderer = r; }
	inline SDL_Renderer* Get() const { return _renderer; }

	Texture RenderLine(TTF_Font *font, const std::string& line, SDL_Color color = { 0, 0, 0 }) const;
	std::vector<Texture> RenderMultiline(TTF_Font *font, const std::string& text, SDL_Color color = { 0, 0, 0 }) const;

private:
	SDL_Renderer *_renderer;
};
