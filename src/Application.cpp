#include "stdafx.hpp"
#include "Application.hpp"
#include "Blocks/Clips.hpp"
#include "Blocks/Time.hpp"
#include "Blocks/Framerate.hpp"
#include "Blocks/Water.hpp"
#include "Blocks/Remainder.hpp"
#include "MessageThread.hpp"

Application::~Application() {
	std::cerr << "Application::~Application()\n";
	TTF_Quit();
	SDL_Quit();
}

int Application::Init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
		return 1;
	}

	if (TTF_Init() == -1) {
		std::cerr << "TTF_Init failed: " << TTF_GetError() << "\n";
		return 1;
	}

	_window.Assign(SDL_CreateWindow("TASView", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 400, SDL_WINDOW_SHOWN));
	if (!_window.Get()) {
		std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
		return 1;
	}

	_renderer.Assign(SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
	//_renderer.Assign(SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED));
	if (!_renderer.Get()) {
		std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
		return 1;
	}

	_blockTime = std::make_shared<Time>();
	_blockClips = std::make_shared<Clips>();
	_blockWater = std::make_shared<Water>();
	_blockRemainder = std::make_shared<Remainder>();
	_blocks.push_back(BlockRegion{ _blockTime, SDL_Rect{ 0, 0, 320, 100 } });
	_blocks.push_back(BlockRegion{ std::unique_ptr<Block>(new Framerate()), SDL_Rect{ 0, 0, 100, 100 } });
	_blocks.push_back(BlockRegion{ _blockClips, SDL_Rect{ 0, 100, 320, 100 } });
	_blocks.push_back(BlockRegion{ _blockWater, SDL_Rect{ 0, 230, 320, 100 } });
	_blocks.push_back(BlockRegion{ _blockRemainder, SDL_Rect{ 0, 250, 320, 100 } });

	std::timed_mutex running;
	running.lock();
	std::thread messageThread(MessageThread, this, std::ref(running));

	MainLoop();

	running.unlock();
	messageThread.join();

	return 0;
}

void Application::MainLoop() {
	_running = true;
	while (_running) {
		ProcessEvents();
		Draw();
	}
}

void Application::ProcessEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			_running = false;
			return;

		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
			case SDLK_ESCAPE:
				_running = false;
				return;
			}
			break;

		case SDL_WINDOWEVENT:
			switch (e.window.event) {
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				for (auto& blockregion : _blocks) {
					blockregion.region.w = e.window.data1;
				}
				break;
			}
		}
	}
}

void Application::ReadMessages() {
	
}

void Application::Draw() {
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
	SDL_RenderClear(_renderer);

	for (auto& blockregion : _blocks) {
		blockregion.block->Draw(_renderer, &blockregion.region);
	}

	SDL_RenderPresent(_renderer);
}
