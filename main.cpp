#ifndef CPPFREEMOCK_MAIN_H
#define CPPFREEMOCK_MAIN_H

#include "cpp_free_mock.h"
#include <iostream>
#include <string>

std::string func(int unused) {
    return "very long long ......................   ................... Non mocked.";
}

struct TestMockStruct {
    std::string func0Parameter(int unused) {
        return "Non mocked.";
    }
};

int main() {
//    const CppFreeMock::MockerBase<std::string(*)(int)>* globalMethodMocker =
//    CppFreeMock::MockerBase<std::string(int)>MOCK_FUNCTION(int);
    MOCKER(func);
    std::cout << "func return: " << func(1) << std::endl;

    MOCKER(&TestMockStruct::func0Parameter);
    std::cout << "TestMockStruct::func0Parameter return: " << TestMockStruct().func0Parameter(1) << std::endl;
    return 0;
}


#endif //CPPFREEMOCK_MAIN_H
