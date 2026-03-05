#pragma once
#include "Component.h"
#include "imgui_plot.h"

namespace dae
{
	class GameObject;
	class TrashTheCacheComponent : public Component
	{
	public:
		TrashTheCacheComponent(GameObject& owner);
		~TrashTheCacheComponent() override;
		void Update(float) override;
		void Render() const override;
	private:
		std::unique_ptr<ImGui::PlotConfig> m_Ex1PlotInfo{ std::make_unique<ImGui::PlotConfig>() };
		std::unique_ptr<ImGui::PlotConfig> m_Ex2PlotInfo{ std::make_unique<ImGui::PlotConfig>() };
		std::unique_ptr<ImGui::PlotConfig> m_Ex2PlotInfoAlt{ std::make_unique<ImGui::PlotConfig>() };
		std::unique_ptr<ImGui::PlotConfig> m_Ex2PlotInfoCombined{ std::make_unique<ImGui::PlotConfig>() };
		float* m_LoopInfoEx1{};
		float* m_LoopInfoEx2{};
		float* m_LoopInfoEx2Alt{};
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