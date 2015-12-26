#include "Flag.h"

Flag::Flag(Player p)
{
	m_owner = p;
}

Player Flag::owner()
{
	return m_owner;
}