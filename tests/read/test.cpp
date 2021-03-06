
// Copyright 2017 Thomas Wendtland
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "Bitfield.hpp"
#include <iostream>
#include <cassert>

/*

    To be compiled for and run on the host system to check if reads are
    returning the correct result.

*/

using namespace rye;

static std::uint32_t TestMemoryLocation[2] = {0};

struct TestRegister {
    using WidthType = std::uint32_t;
    static std::uint64_t Address; // not as in real life, test workaround
    using TestBitfield = rye::Bitfield<TestRegister, std::uint8_t, 4, 4, Access::ReadOnly>;
    using TestBitfield2 = rye::Bitfield<TestRegister, std::uint16_t, 12, 9, Access::ReadOnly>;
};

uint64_t TestRegister::Address = (std::uint64_t)&TestMemoryLocation;

namespace {
    // order of values here: value in memory, value exptected
    constexpr std::uint32_t TestValues[] = {0x30, 0x3, 0x14000, 0x14, };
}

int main(int argc, char** argv){

    std::uint32_t& test_value =  *((std::uint32_t*)TestRegister::Address);

    test_value  = TestValues[0];
    auto read_value = TestRegister::TestBitfield::read();
    printf("Value read: %x - expected value: %x\n", read_value, TestValues[1]);
    assert(read_value == TestValues[1]);

    test_value  = TestValues[2];
    read_value = TestRegister::TestBitfield2::read();
    printf("Value read: %x - expected value: %x\n", read_value, TestValues[3]);
    assert(read_value == TestValues[3]);

    // test read with offset
    test_value = 0;
    *((std::uint32_t*)TestRegister::Address+1) = TestValues[2];

    read_value = TestRegister::TestBitfield2::read(1);
    printf("Value read: %x - expected value: %x\n", read_value, TestValues[3]);
    assert(read_value == TestValues[3]);


    return 0;
}
