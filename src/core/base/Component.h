/*
* File: Component.h
* Class: Component
* Created: 20211031
* Author: SonTV
*/
#pragma once
#include <string>

namespace core
{
class Component
{
public:
    Component(const std::string &strComponentID);
    virtual ~Component();
    virtual void Run();
    virtual void Init();

    const std::string GetID() const;
    
protected:
    std::string m_strComponentID;
};
} //namespace core