#pragma once
#include <Windows.h>
#include "imgui.h"

namespace ManGui {
	class ManBox {
		float f = 0.0f;
		int counter = 0;
		bool show_demo_window = false;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	public:
		void Render() {
			ImGui::Begin(u8"管理框");
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::Text("counter = %d", counter);
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
			ImGui::Checkbox(u8"示例窗口", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox(u8"另一个窗口", &show_another_window);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

			if (show_demo_window) {
				ImGui::ShowDemoWindow(&show_demo_window);
			}

			if (show_another_window) {
				ImGui::Begin(u8"另一个窗口", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
				ImGui::Text("Hello from another window!");
				if (ImGui::Button("Close Me"))
					show_another_window = false;
				ImGui::End();
			}
		}
		
		const ImVec4& peek_clear_color() { return clear_color; }
	};
}