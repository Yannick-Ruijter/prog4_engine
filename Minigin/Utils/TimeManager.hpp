#pragma once
#include "Singleton.hpp"
#include <chrono>
namespace dae
{
	class TimeManager final : public Singleton<TimeManager>
	{
	public:
		float GetDeltaTime() const;

		void Update();
	private:
		long long m_PreviousTime{};
		float m_DeltaTime{};
	};
}