#include "ImGuiPlotComponent.h"

#include <chrono>
#include <numeric>

dae::ImGuiPlotComponent::ImGuiPlotComponent(GameObject* pOwner, const std::string& name)
	: ImGuiComponent(pOwner, name)
{
    m_Config = ImGui::PlotConfig{};
    m_Config.values.xs = nullptr;
    m_Config.values.ys = nullptr;
    m_Config.values.count = 0;
    m_Config.tooltip.show = true;
    m_Config.tooltip.format = "x=%.2f, y=%.2f";
    m_Config.grid_x.show = false;
    m_Config.grid_y.show = false;
    m_Config.frame_size = ImVec2(200, 200);
    m_Config.line_thickness = 2.f;
}

void dae::ImGuiPlotComponent::OnImGuiRender()
{
    //Input
    ImGui::InputInt("# samples", &m_Samples);

    //New timings
    if (ImGui::Button("Trash the cache"))
    {
        NewMeasurements();
    }

    //plot
    if (m_Timings.size())
    {
        m_Config.values.ys = &m_Timings[0];
        m_Config.values.count = static_cast<int>(m_Timings.size());

        ImGui::Plot("excercise 1", m_Config);
    }
}

void dae::ImGuiPlotComponent::NewMeasurements()
{
    m_Timings.clear();

    constexpr int size{ 1 << 26 };
    int* arr{ new int[size] };
    std::fill_n(arr, size, 1);

    const int numMeasurements{ 10 };

    std::vector<float> timings;
    timings.reserve(numMeasurements);
    constexpr int maxSteps{ 1024 };

    for (int step{ 1 }; step < maxSteps; step *= 2)
    {
        timings.clear();
        for (int i{}; i < numMeasurements; ++i)
        {
            auto start{ std::chrono::high_resolution_clock::now() };

            for (int j{}; j < size; j += step)
            {
                arr[j] *= 2;
            }

            auto end{ std::chrono::high_resolution_clock::now() };
            auto duration{ std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() };

            timings.emplace_back(static_cast<float>(duration));
        }

        // Remove outliers
        std::sort(timings.begin(), timings.end());
        timings.erase(timings.begin());
        timings.erase(timings.end() - 1);

        float average{ static_cast<float>(std::accumulate(timings.begin(), timings.end(), 0.0) / timings.size()) };
        m_Timings.push_back(average / 1000.0f);
    }

    delete[] arr;
}
