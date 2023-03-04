/*
    Copyright (C) 2023, InternationalKoder

    This file is part of IKLibs.

    IKLibs is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    IKLibs is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with IKLibs.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "ikparllExamples.hpp"

#include <ikparll/ThreadPool.hpp>
#include <iostream>

void runIkparllExamples()
{
    std::cout << "Thread pool test" << std::endl;
    ikparll::ThreadPool<int, 3> threadPool([](int i) { std::cout << std::this_thread::get_id() << " : " << i*2 << std::endl; std::this_thread::sleep_for(std::chrono::milliseconds(1000)); });
    for(int i = 0 ; i < 10 ; i++)
    {
        threadPool.addItem(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1200));
}
