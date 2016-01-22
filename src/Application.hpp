#pragma once

#include "Window.hpp"
#include "Renderer.hpp"
#include "Block.hpp"
#include "Blocks/Clips.hpp"
#include "Blocks/Time.hpp"
#include "Blocks/Water.hpp"
#include "Blocks/Remainder.hpp"

struct BlockRegion {
	std::shared_ptr<Block> block;
	SDL_Rect region;
};

class Application {
public:
	Application() = default;
	~Application();

	int Init();
	void ParseMessage(const std::vector<char>& buf, size_t bytesRead);

private:
	void MainLoop();
	void ProcessEvents();
	void Draw();

	Window _window;
	Renderer _renderer;
	bool _running;

	std::shared_ptr<Time> _blockTime;
	std::shared_ptr<Clips> _blockClips;
	std::shared_ptr<Water> _blockWater;
	std::shared_ptr<Remainder> _blockRemainder;
	std::vector<BlockRegion> _blocks;
};
