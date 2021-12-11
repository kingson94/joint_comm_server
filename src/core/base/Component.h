/*
* File: Component.h
* Class: TSComponent
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
class TSComponent
{
public:
    TSComponent(const std::string &strComponentID);
    virtual ~TSComponent();
    
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

typedef std::shared_ptr<core::base::TSComponent> TSComponentPtr;
