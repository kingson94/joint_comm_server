/*
* File: Component.cpp
* Class: Component
* Created: 20211031
* Author: SonTV
*/

#include "Component.h"
namespace core
{
namespace base
{
Component::Component(const std::string &strComponentID) : m_strComponentID(strComponentID), m_bIsInit(false)
{
}

Component::~Component()
{
}

void Component::Run()
{
}

void Component::Join()
{
}

void Component::Init()
{
    m_bIsInit = true;
}

const std::string Component::GetID() const
{
    return m_strComponentID;
}
} // namespace base
} // namespace core