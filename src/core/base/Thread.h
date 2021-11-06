/*
* File: Thread.h
* Class: KSThread
* Created: 20211106
* Author: SonTV
*/

#pragma once
#include "uuid/uuid.h"
#include <string>

namespace core
{
class KSThread
{
private:
    std::string strID32;
public:
    KSThread();
    ~KSThread();
};
} // namespace core