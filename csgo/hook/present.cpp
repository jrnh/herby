#include "csgo/hook/procedure.hpp"

#include "csgo/engine.hpp"

namespace csgo::hook
{

HRESULT API_D3D Present( IDirect3DDevice9* device, const RECT* source_rect, const RECT* dest_rect, HWND dest_window_override, const RGNDATA* dirty_region )
{
	auto& gui = feature::Gui::Instance();
	auto& esp = feature::Esp::Instance();
	auto& renderer = engine::Renderer::Instance();

	if (renderer.Begin())
	{
		renderer.DrawText({ 20.f, 10.f }, engine::TextLeft, { 1.f, 1.f, 1.f }, XorStr("herby"));

		esp.Present();

		renderer.End();
	}
	gui.Present();
	renderer.Present();

	m_present_hook->Restore();
	const auto result = m_present_hook->Win32Call< HRESULT >( device, source_rect, dest_rect, dest_window_override, dirty_region );
	m_present_hook->Replace();

	return result;
}

}