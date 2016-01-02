#pragma once

#include <memory>
#include <vector>

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

	inline std::shared_ptr<Time> GetBlockTime() const { return _blockTime; };
	inline std::shared_ptr<Clips> GetBlockClips() const { return _blockClips; };
	inline std::shared_ptr<Water> GetBlockWater() const { return _blockWater; };
	inline std::shared_ptr<Remainder> GetBlockRemainder() const { return _blockRemainder; };

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
