#ifndef CPPFREEMOCK_MAIN_H
#define CPPFREEMOCK_MAIN_H

#include "cpp_free_mock.h"
#include <iostream>
#include <string>

std::string func(int unused) {
    return "very long long ......................   ................... Non mocked.";
}

int main() {
    const CppFreeMock::MockerBase<std::string(*)(int)>* mocker = MOCKER(func);
//    CppFreeMock::MockerBase<std::string(int)>MOCK_FUNCTION(int);
    std::cout << "func return: " << func(1) << std::endl;
    return 0;
}


#endif //CPPFREEMOCK_MAIN_H
