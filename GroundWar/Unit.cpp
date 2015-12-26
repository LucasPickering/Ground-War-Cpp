#include "Unit.h"

Unit::Unit(Player owner, int goldCost, int movementCost, UnitType type, const char* name)
{
	m_owner = owner;
	m_goldCost = goldCost;
	m_movementCost = movementCost;
	m_type = type;
	m_name = name;
}

Unit::~Unit()
{
	delete m_flag;
}

Player Unit::owner()
{
	return m_owner;
}

int Unit::goldCost()
{
	return m_goldCost;
}

int Unit::movementCost()
{
	return m_movementCost;
}

Unit::UnitType Unit::type()
{
	return m_type;
}

Flag* Unit::flag()
{
	return m_flag;
}

void Unit::setFlag(Flag* flag)
{
	m_flag = flag;
}

const char* Unit::name()
{
	return m_name;
}

bool Unit::canTakeFlag(Flag* flag)
{
	return false;
}