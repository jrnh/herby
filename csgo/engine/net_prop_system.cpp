#include "csgo/engine/net_prop_system.hpp"

#include "csgo/engine.hpp"

namespace csgo::engine
{

bool NetPropSystem::Create()
{
	m_recv_array.clear();

	for (auto entry = csgo::m_base_client->GetAllClasses(); entry; entry = entry->m_pNext)
	{
		auto recv_entry = entry->m_pRecvTable;

		if (recv_entry)
			m_recv_array.emplace_back(recv_entry);
	}

	return !m_recv_array.empty();
}

void NetPropSystem::Destroy()
{
	m_recv_array.clear();
}

int NetPropSystem::Get( const std::string& array_name, const std::string& prop_name, RecvProp** prop_out /*= nullptr*/) const
{
	auto array_entry = GetArrayEntry(array_name);
	LOG(prop_name, std::hex, array_entry);
	return Get(array_entry, prop_name, prop_out);
}

int NetPropSystem::Get( RecvTable* recv_entry, const std::string& prop_name, RecvProp** prop_out /*= nullptr*/) const
{
	if (!recv_entry)
		return 0;

	auto extra = 0;

	for (auto index = 0; index < recv_entry->m_nProps; index++)
	{
		auto prop = &recv_entry->m_pProps[index];
		auto child = prop->m_pDataTable;

		if (child && child->m_nProps)
		{
			auto add = Get(child, prop_name, prop_out);

			if (add)
				extra += (prop->m_Offset + add);
		}

		if (!prop_name.compare(prop->m_pVarName))
		{
			if (prop_out)
				*prop_out = prop;

			return (prop->m_Offset + extra);
		}
	}

	return extra;
}

RecvTable* NetPropSystem::GetArrayEntry( const std::string& array_name ) const
{
	for (auto& entry : m_recv_array)
	{
		if (!array_name.compare(entry->m_pNetTableName))
			return entry;
	}

	return nullptr;
}

}