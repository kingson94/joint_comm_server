/*
* File: Worker.h
* Class: Worker
* Created: 20211127
* Author: SonTV
*/
#pragma once
#include "core/base/Thread.h"

namespace core
{
namespace op
{
class Worker : public base::BaseThread
{
public:
    Worker(const int &iThreadID);
    ~Worker();

    void Run() override;
};
} // namespace op
} // namespace core

typedef std::shared_ptr<core::op::Worker> WorkerPtr;