#include "TrashTheCache.h"
#include "TrashTheCacheComponent.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include "Renderer.h"
#include <chrono>

using namespace dae;
dae::TrashTheCacheComponent::TrashTheCacheComponent(GameObject& owner)
	:Component(owner)
{
}

dae::TrashTheCacheComponent::~TrashTheCacheComponent()
{
	delete[] m_LoopInfoEx1;
	delete[] m_LoopInfoEx2;
	delete[] m_LoopInfoEx2Alt;
}

void dae::TrashTheCacheComponent::Update(float)
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	ShowEx1Screen();
	ShowEx2Screen();

}

void dae::TrashTheCacheComponent::Render() const
{

	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(
		ImGui::GetDrawData(),
		dae::Renderer::GetInstance().GetSDLRenderer()
	);
}

void TrashTheCacheComponent::ShowEx1Screen()
{
	static int samples = 10;

	ImGui::Begin("Exercise 1");

	ImGui::SetNextItemWidth(80.0f);
	ImGui::InputInt("##samples", &samples);

	ImGui::SameLine();
	ImGui::Text("# samples");

	ImGui::Spacing();

	if (ImGui::Button("Trash the cache"))
	{
		m_TrashCacheEx1ButtonPressed = true;
		CalculatePlotInfoEx1(samples);
	}

	if (m_TrashCacheEx1ButtonPressed)
	{
		ImGui::Plot("info", *m_Ex1PlotInfo);
	}

	ImGui::End();
}

void TrashTheCacheComponent::ShowEx2Screen()
{
	static int samples = 100;

	ImGui::SetNextWindowSize(ImVec2(420.0f, 0.0f));
	ImGui::Begin("Exercise 2");

	ImGui::SetNextItemWidth(180.0f);
	ImGui::InputInt("##samples", &samples);

	ImGui::SameLine();
	ImGui::Text("# samples");

	ImGui::Spacing();

	if (ImGui::Button("Trash the cache with GameObject3D"))
	{
		m_TrashCacheEx2ButtonPressed = true;
		CalculatePlotInfoEx2(samples);
	}

	if (m_TrashCacheEx2ButtonPressed)
	{
		ImGui::Plot("ex2plot", *m_Ex2PlotInfo);
	}

	if (ImGui::Button("Trash the cache with GameObject3DAlt"))
	{
		m_TrashCacheEx2AltButtonPressed = true;
		CalculatePlotInfoEx2Alt(samples);
	}

	if (m_TrashCacheEx2AltButtonPressed)
	{
		ImGui::Plot("ex2plotalt", *m_Ex2PlotInfoAlt);
	}

	if (m_TrashCacheEx2ButtonPressed && m_TrashCacheEx2AltButtonPressed)
	{
		ImGui::Text("combined:");
		ImGui::Plot("ex2plotcombined", *m_Ex2PlotInfoCombined);
	}

	ImGui::End();
}

void TrashTheCacheComponent::CalculatePlotInfoEx1(int nrOfSamples)
{
	constexpr int arraySize{ 100000000 };
	int* reallyBigArray = new int[arraySize](1);

	m_LoopInfoEx1 = new float[11]();
	for (int sampleCounter{ 0 }; sampleCounter < nrOfSamples; ++sampleCounter)
	{
		int uniqueStepCounter{ 0 };
		for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
		{
			auto const preLoopTimePoint = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < arraySize; i += stepsize)
			{
				reallyBigArray[i] *= 2;
			}
			auto const postLoopTimePoint = std::chrono::high_resolution_clock::now();
			m_LoopInfoEx1[uniqueStepCounter] += std::chrono::duration_cast<std::chrono::milliseconds>(postLoopTimePoint - preLoopTimePoint).count();
			++uniqueStepCounter;
		}
	}
	for (int i{ 0 }; i < 11; ++i)
	{
		m_LoopInfoEx1[i] /= nrOfSamples;
	}
	delete[] reallyBigArray;

	m_Ex1PlotInfo->values.xs = nullptr;          // no custom X values
	m_Ex1PlotInfo->values.ys = m_LoopInfoEx1;         // <-- your data here
	m_Ex1PlotInfo->values.count = 11;            // number of elements
	m_Ex1PlotInfo->values.offset = 0;

	m_Ex1PlotInfo->scale.min = 0;
	m_Ex1PlotInfo->scale.max = FLT_MAX;          // or compute a max value

	m_Ex1PlotInfo->grid_x.show = true;
	m_Ex1PlotInfo->grid_y.show = true;

	m_Ex1PlotInfo->tooltip.show = true;
	m_Ex1PlotInfo->frame_size = ImVec2(100, 100);
	m_Ex1PlotInfo->line_thickness = 2.0f;
	m_Ex1PlotInfo->scale.max = m_LoopInfoEx1[0] * 1.1f;

}

void TrashTheCacheComponent::CalculatePlotInfoEx2(int nrOfSamples)
{
	struct transform
	{
		float matrix[16]{
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1,
		};
	};

	class GameObject
	{
	public:
		transform local{};
		int id{};
	};

	constexpr int arraySize{ 10000000 };
	GameObject* reallyBigArray = new GameObject[arraySize]();

	if (m_LoopInfoEx2 != nullptr) delete[] m_LoopInfoEx2;
	m_LoopInfoEx2 = new float[11]();
	for (int sampleCounter{ 0 }; sampleCounter < nrOfSamples; ++sampleCounter)
	{
		int uniqueStepCounter{ 0 };
		for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
		{
			auto const preLoopTimePoint = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < arraySize; i += stepsize)
			{
				reallyBigArray[i].id *= 2;
			}
			auto const postLoopTimePoint = std::chrono::high_resolution_clock::now();
			m_LoopInfoEx2[uniqueStepCounter] += std::chrono::duration_cast<std::chrono::milliseconds>(postLoopTimePoint - preLoopTimePoint).count();
			++uniqueStepCounter;
		}
	}
	for (int i{ 0 }; i < 11; ++i)
	{
		m_LoopInfoEx2[i] /= nrOfSamples;
	}
	delete[] reallyBigArray;

	m_Ex2PlotInfo->values.xs = nullptr;          // no custom X values
	m_Ex2PlotInfo->values.ys = m_LoopInfoEx2;         // <-- your data here
	m_Ex2PlotInfo->values.count = 11;            // number of elements
	m_Ex2PlotInfo->values.offset = 0;

	m_Ex2PlotInfo->scale.min = 0;
	m_Ex2PlotInfo->scale.max = FLT_MAX;          // or compute a max value

	m_Ex2PlotInfo->grid_x.show = true;
	m_Ex2PlotInfo->grid_y.show = true;

	m_Ex2PlotInfo->tooltip.show = true;
	m_Ex2PlotInfo->frame_size = ImVec2(100, 100);
	m_Ex2PlotInfo->line_thickness = 2.0f;
	m_Ex2PlotInfo->scale.max = m_LoopInfoEx2[0] * 1.1f;

	if (m_TrashCacheEx2AltButtonPressed) CalculatePlotInfoEx2Combined();
}

void TrashTheCacheComponent::CalculatePlotInfoEx2Alt(int nrOfSamples)
{
	struct transform
	{
		float matrix[16]{
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1,
		};
	};

	class GameObject
	{
	public:
		transform* local{};
		int id{};
	};

	constexpr int arraySize{ 10000000 };
	GameObject* reallyBigArray = new GameObject[arraySize]();

	if (m_LoopInfoEx2Alt != nullptr) delete[] m_LoopInfoEx2Alt;
	m_LoopInfoEx2Alt = new float[11]();
	for (int sampleCounter{ 0 }; sampleCounter < nrOfSamples; ++sampleCounter)
	{
		int uniqueStepCounter{ 0 };
		for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
		{
			auto const preLoopTimePoint = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < arraySize; i += stepsize)
			{
				reallyBigArray[i].id *= 2;
			}
			auto const postLoopTimePoint = std::chrono::high_resolution_clock::now();
			m_LoopInfoEx2Alt[uniqueStepCounter] += std::chrono::duration_cast<std::chrono::milliseconds>(postLoopTimePoint - preLoopTimePoint).count();
			++uniqueStepCounter;
		}
	}
	for (int i{ 0 }; i < 11; ++i)
	{
		m_LoopInfoEx2Alt[i] /= nrOfSamples;
	}
	delete[] reallyBigArray;

	m_Ex2PlotInfoAlt->values.xs = nullptr;          // no custom X values
	m_Ex2PlotInfoAlt->values.ys = m_LoopInfoEx2Alt;         // <-- your data here
	m_Ex2PlotInfoAlt->values.count = 11;            // number of elements
	m_Ex2PlotInfoAlt->values.offset = 0;

	m_Ex2PlotInfoAlt->scale.min = 0;
	m_Ex2PlotInfoAlt->scale.max = FLT_MAX;          // or compute a max value

	m_Ex2PlotInfoAlt->grid_x.show = true;
	m_Ex2PlotInfoAlt->grid_y.show = true;

	m_Ex2PlotInfoAlt->tooltip.show = true;
	m_Ex2PlotInfoAlt->frame_size = ImVec2(100, 100);
	m_Ex2PlotInfoAlt->line_thickness = 2.0f;
	m_Ex2PlotInfoAlt->scale.max = m_LoopInfoEx2Alt[0] * 1.1f;
	if (m_TrashCacheEx2ButtonPressed) CalculatePlotInfoEx2Combined();
}

void TrashTheCacheComponent::CalculatePlotInfoEx2Combined()
{
	m_LoopInfoEx2Combined[0] = m_LoopInfoEx2;
	m_LoopInfoEx2Combined[1] = m_LoopInfoEx2Alt;
	m_Ex2PlotInfoCombined->values.ys_list = m_LoopInfoEx2Combined;         // <-- your data here
	m_Ex2PlotInfoCombined->values.count = 11;            // number of elements
	m_Ex2PlotInfoCombined->values.ys_count = 2;            // number of elements
	m_Ex2PlotInfoCombined->values.offset = 0;

	m_Ex2PlotInfoCombined->scale.min = 0;
	m_Ex2PlotInfoCombined->scale.max = std::max(m_LoopInfoEx2[0], m_LoopInfoEx2Alt[0]) * 1.1f;          // or compute a max value

	m_Ex2PlotInfoCombined->grid_x.show = true;
	m_Ex2PlotInfoCombined->grid_y.show = true;

	m_Ex2PlotInfoCombined->tooltip.show = true;
	m_Ex2PlotInfoCombined->frame_size = ImVec2(100, 100);
	m_Ex2PlotInfoCombined->line_thickness = 2.0f;
}
