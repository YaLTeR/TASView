#include "../stdafx.hpp"
#include "../Renderer.hpp"
#include "../Texture.hpp"
#include "Clips.hpp"

Clips::Clips() :
	_timeOfLastUpdate(std::chrono::high_resolution_clock::now()),
	_count(0), _normalz(0.f), _updateTexture(true),
	_in{0.f, 0.f, 0.f}, _out{0.f, 0.f, 0.f}
{
	_font = TTF_OpenFont("font.ttf", 25);
	if (!_font)
		std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << "\n";
}

void Clips::Draw(const Renderer& renderer, SDL_Rect *rect) {
	if (!_font)
		return;

	auto now = std::chrono::high_resolution_clock::now();
	bool update = false;
	int c;
	float nz;
	float in[3], out[3];

	_m.lock();
	auto secondsPassed = std::chrono::duration_cast<std::chrono::seconds>(now - _timeOfLastUpdate).count();
	if (secondsPassed && _count) {
		_count = 0;
		_updateTexture = true;
	}

	if (_updateTexture || std::any_of(_texLines.begin(), _texLines.end(), [](const auto& texture) { return !texture.Get(); })) {
		c = _count;
		nz = _normalz;
		in[0] = _in[0];
		in[1] = _in[1];
		in[2] = _in[2];
		out[0] = _out[0];
		out[1] = _out[1];
		out[2] = _out[2];
		update = true;
		_updateTexture = false;
	}
	_m.unlock();

	if (update)
		UpdateTexture(renderer, c, nz, in, out);

	if (std::any_of(_texLines.begin(), _texLines.end(), [](const auto& texture) { return !texture.Get(); }))
		return;

	auto dstRect = SDL_Rect{};
	dstRect.x = rect->x;
	dstRect.y = rect->y;
	std::for_each(_texLines.begin(), _texLines.end(), [&](const auto& texture) {
		dstRect.w = texture.Width();
		dstRect.h = texture.Height();
		SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
		dstRect.y += TTF_FontLineSkip(_font) * 3 / 4;
	});
}

void Clips::AddClip(float normalz, float in[3], float out[3]) {
	std::lock_guard<std::mutex> g(_m);

	_normalz = normalz;
	_in[0] = in[0];
	_in[1] = in[1];
	_in[2] = in[2];
	_out[0] = out[0];
	_out[1] = out[1];
	_out[2] = out[2];
	_count++;
	_timeOfLastUpdate = std::chrono::high_resolution_clock::now();
	_updateTexture = true;
}

void Clips::UpdateTexture(const Renderer& renderer, int count, float normalz, const float in[3], const float out[3]) {
	std::ostringstream oss;
	oss.setf(std::ios::fixed);
	oss.precision(3);
	oss << count << ": " << std::acos(static_cast<double>(normalz)) * 180 / M_PI
		<< "\n" << in[0] << " \xE2\x86\x92 " << out[0]
		<< "\n" << in[1] << " \xE2\x86\x92 " << out[1]
		<< "\n" << in[2] << " \xE2\x86\x92 " << out[2]
		<< "\n" << std::hypot(in[0], in[1]) << " \xE2\x86\x92 " << std::hypot(out[0], out[1]);
	_texLines = renderer.RenderMultiline(_font, oss.str());
}
