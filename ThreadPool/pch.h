#pragma once
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include<iostream>
#include<thread>
#include<queue>
#include<vector>
#include<functional>
#include<mutex>
#include<condition_variable>
#include<future>
#include<type_traits>