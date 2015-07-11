#pragma once

#include "../Block.hpp"
#include "../Renderer.hpp"
#include "../Texture.hpp"

class Clips : public Block {
public:
	Clips();
	void Draw(const Renderer& renderer, SDL_Rect *rect) override;
	void AddClip(float normalz, float in[3], float out[3]);
	void UpdateTexture(const Renderer& renderer, int count, float normalz, const float in[3], const float out[3]);

private:
	TTF_Font *_font;

	std::chrono::time_point<std::chrono::high_resolution_clock> _timeOfLastUpdate;
	int _count;
	float _normalz;
	float _in[3], _out[3];

	std::vector<Texture> _texLines;
	bool _updateTexture;
	std::mutex _m;
};
