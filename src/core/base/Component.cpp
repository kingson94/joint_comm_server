/*
* File: TcpServer.cpp
* Class: Component
* Created: 20211031
* Author: SonTV
*/

#include "Component.h"
namespace core
{
Component::Component(const std::string &strComponentID) : m_strComponentID(strComponentID)
{
}

Component::~Component()
{
}

void Component::Run()
{
}

void Component::Init()
{

}

const std::string Component::GetID() const
{
    return m_strComponentID;
}
}