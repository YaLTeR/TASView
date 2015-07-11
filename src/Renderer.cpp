#include "stdafx.hpp"
#include "Renderer.hpp"

Renderer::~Renderer() {
	if (_renderer)
		SDL_DestroyRenderer(_renderer);
}

Texture Renderer::RenderLine(TTF_Font *font, const std::string& line, SDL_Color color) const {
	if (!_renderer)
		throw std::logic_error("NULL renderer");

	auto surface = TTF_RenderUTF8_Blended(font, line.c_str(), color);
	if (!surface)
		throw std::runtime_error(TTF_GetError());

	Texture result(SDL_CreateTextureFromSurface(_renderer, surface), surface->w, surface->h);
	SDL_FreeSurface(surface);

	return result;
}

std::vector<Texture> Renderer::RenderMultiline(TTF_Font *font, const std::string& text, SDL_Color color) const {
	std::vector<std::string> lines;
	boost::algorithm::split(lines, text, [](auto c) { return c == '\n'; });

	std::vector<Texture> result;
	result.reserve(lines.size());

	for (const auto& line : lines)
		result.push_back(RenderLine(font, line, color));

	return result;
}
