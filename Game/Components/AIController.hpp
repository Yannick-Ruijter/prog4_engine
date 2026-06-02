#pragma once

#include "Component.hpp"

namespace dae {
    class AIController : public Component {
      public:
        AIController(GameObject &owner);
    };
} // namespace dae
