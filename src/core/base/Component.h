/*
* File: Component.h
* Class: Component
* Created: 20211031
* Author: SonTV
*/
#pragma once
#include <string>
#include <memory>

namespace core
{
namespace base
{
class Component
{
public:
    Component(const std::string &strComponentID);
    virtual ~Component();
    
    virtual void Init();
    virtual void Run();
    virtual void Join();

    const std::string GetID() const;
    
protected:
    std::string m_strComponentID;
    bool m_bIsInit;
};
} // namespace base
} // namespace core

typedef std::shared_ptr<core::base::Component> ComponentPtr;
