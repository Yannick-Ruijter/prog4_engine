#pragma once
#include "Component.hpp"
#include "imgui_plot.h"
#include <vector>
#include <memory>
namespace dae
{
	class GameObject;
	class TrashTheCacheComponent : public Component
	{
	public:
		TrashTheCacheComponent(GameObject& owner);
		~TrashTheCacheComponent() override = default;
		void Update() override;
		void Render() const override;
	private:
		std::unique_ptr<ImGui::PlotConfig> m_Ex1PlotInfo{ std::make_unique<ImGui::PlotConfig>() };
		std::unique_ptr<ImGui::PlotConfig> m_Ex2PlotInfo{ std::make_unique<ImGui::PlotConfig>() };
		std::unique_ptr<ImGui::PlotConfig> m_Ex2PlotInfoAlt{ std::make_unique<ImGui::PlotConfig>() };
		std::unique_ptr<ImGui::PlotConfig> m_Ex2PlotInfoCombined{ std::make_unique<ImGui::PlotConfig>() };
		std::vector<std::vector<float>> m_LoopInfoEx1{};
		std::vector<float> m_CalculatedDataEx1{};
		std::vector<std::vector<float>> m_LoopInfoEx2{};
		std::vector<float> m_CalculatedDataEx2{};
		std::vector<std::vector<float>> m_LoopInfoEx2Alt{};
		std::vector<float> m_CalculatedDataEx2Alt{};
		const float* m_LoopInfoEx2Combined[2]{};
		bool m_TrashCacheEx1ButtonPressed{ false };
		bool m_TrashCacheEx2ButtonPressed{ false };
		bool m_TrashCacheEx2AltButtonPressed{ false };

		void ShowEx1Screen();
		void ShowEx2Screen();

		void CalculatePlotInfoEx1(int nrOfSamples);
		void CalculatePlotInfoEx2(int nrOfSamples);
		void CalculatePlotInfoEx2Alt(int nrOfSamples);
		void CalculatePlotInfoEx2Combined();
	};
}