#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <memory>

namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
		virtual void StopExecution() = 0;
	};
}