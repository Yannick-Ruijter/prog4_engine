#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace dae
{
    class Command
    {
      public:
        virtual ~Command() = default;
        virtual void Execute() = 0;
    };
} // namespace dae
