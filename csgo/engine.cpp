#include "csgo/engine.hpp"

#include "csgo/engine/factory.hpp"
#include "csgo/engine/net_prop_system.hpp"

namespace csgo
{

IBaseClientDLL* m_base_client = nullptr;
IClientEntityList* m_client_entity_list = nullptr;
IVEngineClient* m_engine_client = nullptr;
IVModelInfoClient* m_model_info_client = nullptr;
IEngineTrace* m_engine_trace = nullptr;
ISurface* m_surface = nullptr;
ICvar* m_cvar = nullptr;
IMaterialSystem* m_material_system = nullptr;
IVRenderView* m_render_view = nullptr;
IVModelRender* m_model_render = nullptr;
IGameEventManager* m_game_event_manager = nullptr;
IGameUI* m_game_ui = nullptr;
IGameMovement* m_game_movement = nullptr;
IInputSystem* m_input_system = nullptr;
ILocalize* m_localize = nullptr;
IPhysicsSurfaceProps* m_physics_surface_props = nullptr;
IPrediction* m_prediction = nullptr;


CGlobalVarsBase* m_globals = nullptr;
IClientMode* m_client_mode = nullptr;
IWeaponSystem* m_weapon_system = nullptr;
C_GlowObjectManager* m_glow_object = nullptr;
IMemAlloc* m_mem_alloc = nullptr;
IMoveHelper* m_move_helper = nullptr;
CInput* m_input = nullptr;

IDirect3DDevice9* m_direct_device = nullptr;

memory::ObjectHook::Shared m_present_hook = { };
memory::ObjectHook::Shared m_reset_hook = { };

memory::ObjectHook::Shared m_frame_stage_notify_hook = { };
memory::ObjectHook::Shared m_create_move_hook = { };
memory::ObjectHook::Shared m_lock_cursor_hook = { };
memory::ObjectHook::Shared m_run_command_hook = { };

bool Create()
{
	auto& input = win32::Input::Instance();
	auto& system = win32::System::Instance();

	auto& gui = feature::Gui::Instance();

	auto& renderer = engine::Renderer::Instance();
	auto& net_prop_system = engine::NetPropSystem::Instance();

	engine::Factory factory_client( "client.dll" );
	engine::Factory factory_engine( "engine.dll" );
	engine::Factory factory_materialsystem("materialsystem.dll");
	engine::Factory factory_vgui("vguimatsurface.dll");
	engine::Factory factory_vstdlib("vstdlib.dll");
	engine::Factory factory_input("inputsystem.dll");
	engine::Factory factory_localize("localize.dll");
	engine::Factory factory_vphysics("vphysics.dll");

	if (!input.Create())
	{
		d_print("input.Create()");
		return false;
	}
	
	if (!system.Create())
	{
		d_print("system.Create()");
		return false;
	}

	if (!option::Create())
	{
		d_print("option::Create()");
		return false;
	}

	m_base_client = factory_client.Get< IBaseClientDLL* >(XorStr("VClient") );

	if( !m_base_client )
		return false;

	m_client_entity_list = factory_client.Get< IClientEntityList* >(XorStr("VClientEntityList") );

	if( !m_client_entity_list )
		return false;

	m_engine_client = factory_engine.Get< IVEngineClient* >(XorStr("VEngineClient") );

	if( !m_engine_client )
		return false;

	m_model_info_client = factory_engine.Get< IVModelInfoClient* >(XorStr("VModelInfoClient"));

	if (!m_model_info_client)
		return false;

	m_render_view = factory_engine.Get< IVRenderView* >(XorStr("VEngineRenderView"));

	if (!m_render_view)
		return false;

	m_model_render = factory_engine.Get< IVModelRender* >(XorStr("VEngineModel"));

	if (!m_model_render)
		return false;

	m_engine_trace = factory_engine.Get< IEngineTrace* >(XorStr("EngineTraceClient"));

	if (!m_engine_trace)
		return false;

	m_cvar = factory_vstdlib.Get< ICvar* >(XorStr("VEngineCvar"));

	if (!m_cvar)
		return false;

	m_material_system = factory_materialsystem.Get< IMaterialSystem* >(XorStr("VMaterialSystem"));

	if (!m_material_system)
		return false;

	m_surface = factory_vgui.Get< ISurface* >(XorStr("VGUI_Surface"));

	if (!m_surface)
		return false;

	m_game_event_manager = factory_engine.Get< IGameEventManager* >(XorStr("GAMEEVENTSMANAGER"));

	if (!m_game_event_manager)
		return false;

	m_game_ui = factory_client.Get< IGameUI* >(XorStr("GameUI"));

	if (!m_game_ui)
		return false;

	m_game_movement = factory_client.Get< IGameMovement* >(XorStr("GameMovement"));

	if (!m_game_movement)
		return false;

	m_input_system = factory_input.Get< IInputSystem* >(XorStr("InputSystemVersion"));

	if (!m_input_system)
		return false;

	m_localize = factory_localize.Get< ILocalize* >(XorStr("Localize_"));

	if (!m_localize)
		return false;

	m_physics_surface_props = factory_vphysics.Get< IPhysicsSurfaceProps* >(XorStr("VPhysicsSurfaceProps"));

	if (!m_physics_surface_props)
		return false;

	m_prediction = factory_client.Get< IPrediction* >(XorStr("VClientPrediction"));

	if (!m_prediction)
		return false;

	m_globals = **reinterpret_cast<CGlobalVarsBase***>(m_base_client->ToArray(11) + 10);

	if( !m_globals )
		return false;

	m_client_mode = **reinterpret_cast<IClientMode***>(m_base_client->ToArray(10) + 0x005);

	if (!m_client_mode)
		return false;

	m_weapon_system = memory::scan< IWeaponSystem*>(XorStr("client.dll"), XorStr("8B 35 ? ? ? ? FF 10 0F B7 C0"), 0x2);

	if (!m_weapon_system)
		return false;

	m_glow_object = memory::scan< C_GlowObjectManager* >(XorStr("client.dll"), XorStr("0F 11 05 ? ? ? ? 83 C8 01"), 3u);

	if (!m_glow_object)
		return false;

	m_mem_alloc = *reinterpret_cast<IMemAlloc**>(pe::GetProcAddress(pe::GetModuleHandles(XorStr("tier0.dll")), XorStr("g_pMemAlloc")));

	if (!m_mem_alloc)
		return false;

	m_move_helper = memory::scan< IMoveHelper* >(XorStr("client.dll"), XorStr("8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01"), 2, 2u);

	if (!m_move_helper)
		return false;

	m_input = memory::scan< CInput* >(XorStr("client.dll"), XorStr("B9 ? ? ? ? F3 0F 11 04 24 FF 50 10"), 1, 1u);

	if (!m_input)
		return false;

	m_direct_device = memory::scan< IDirect3DDevice9* >(XorStr("shaderapidx9.dll"), XorStr("A1 ? ? ? ? 50 8B 08 FF 51 0C"), 1u);

	if( !m_direct_device )
		return false;

	if (!renderer.Create(m_direct_device))
	{
		d_print("renderer.Create()");
		return false;
	}

	if (!net_prop_system.Create())
	{
		d_print("net_prop_system.Create()");
		return false;
	}

	if (!gui.Create())
	{
		d_print("gui.Create()");
		return false;
	}

	const auto present_address = memory::vget< 17, void* >( m_direct_device );
	const auto reset_address = memory::vget< 16, void* >( m_direct_device );

	const auto frame_stage_notify_address = memory::vget< 37, void* >( m_base_client );
	const auto create_move_address = memory::vget< 22, void* >( m_base_client );
	const auto lock_cursor_address = memory::vget< 67, void* >( m_surface );
	const auto run_command_address = memory::vget< 19, void*>( m_prediction );

	m_present_hook = std::make_shared< memory::ObjectHook >( present_address, &hook::Present );
	m_reset_hook = std::make_shared< memory::ObjectHook >( reset_address, &hook::Reset );

	m_frame_stage_notify_hook = std::make_shared< memory::ObjectHook >( frame_stage_notify_address, &hook::FrameStageNotify );
	m_create_move_hook = std::make_shared< memory::ObjectHook >( create_move_address, &hook::CreateMove);
	m_lock_cursor_hook = std::make_shared< memory::ObjectHook >( lock_cursor_address, &hook::LockCursor);
	m_run_command_hook = std::make_shared< memory::ObjectHook >( run_command_address, &hook::RunCommand);

	return true;
}

void Destroy()
{
	auto& input = win32::Input::Instance();
	auto& system = win32::System::Instance();

	auto& gui = feature::Gui::Instance();

	auto& renderer = engine::Renderer::Instance();
	auto& net_prop_system = engine::NetPropSystem::Instance();
	
	input.Destroy();
	system.Destroy();
	option::Destroy();
	
	gui.Destroy();

	renderer.Destroy();
	net_prop_system.Destroy();

	if( m_present_hook )
		m_present_hook.reset();

	if( m_reset_hook )
		m_reset_hook.reset();

	if( m_frame_stage_notify_hook )
		m_frame_stage_notify_hook.reset();

	if (m_create_move_hook)
		m_create_move_hook.reset();

	if (m_lock_cursor_hook)
		m_lock_cursor_hook.reset();

	if (m_run_command_hook)
		m_run_command_hook.reset();
}

}