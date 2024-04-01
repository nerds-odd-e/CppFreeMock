#ifndef CPPFREEMOCK_MAIN_H
#define CPPFREEMOCK_MAIN_H

#include "cpp_free_mock.h"
#include <iostream>
#include <string>

std::string func(int unused) {
    return "Non mocked.";
}

int main() {
//    MOCKER(func) mocker;
    const std::string& funcName = "func";
    ::CppFreeMock::MockerCreator::GetMocker< ::CppFreeMock::TypeForUniqMocker<42>, std::string, int>(func, funcName);
    std::cout << "running..." << std::endl;
    return 0;
}


#endif //CPPFREEMOCK_MAIN_H
