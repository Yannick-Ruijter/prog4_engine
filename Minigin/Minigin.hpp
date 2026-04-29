#pragma once
#include <string>
#include <functional>
#include <filesystem>
#include "Game.hpp"
namespace dae
{
	class Minigin final
	{
		bool m_quit{};
		std::unique_ptr<Game> m_Game{};
	public:
		explicit Minigin(const std::filesystem::path& dataPath, std::unique_ptr<Game> game);
		~Minigin();
		void Run();
		void RunOneFrame();

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
	};
}