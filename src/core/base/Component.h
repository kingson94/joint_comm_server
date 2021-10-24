/*
* File: Protocol.h
* Class: Protocol
* Created: 20211024
* Author: SonTV
*/
#pragma once
#include <string>

class Component
{
public:
    Component(const std::string &strComponentID);
    virtual ~Component()
    {}
    
protected:
    std::string strComponentID;
};