/*
* File: Component.cpp
* Class: TSComponent
* Created: 20211031
* Author: SonTV
*/
#include "Component.h"
namespace core
{
namespace base
{
TSComponent::TSComponent(const std::string &strComponentID) : m_strComponentID(strComponentID), m_bIsInit(false)
{
}

TSComponent::~TSComponent()
{
}

void TSComponent::Run()
{
}

void TSComponent::Join()
{
}

void TSComponent::Init()
{
    m_bIsInit = true;
}

const std::string TSComponent::GetID() const
{
    return m_strComponentID;
}
} // namespace base
} // namespace core