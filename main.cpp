#ifndef CPPFREEMOCK_MAIN_H
#define CPPFREEMOCK_MAIN_H

#include "cpp_free_mock.h"
#include <iostream>
#include <string>
#include <cstring>

char * func(int) {
    std::string str = "Non mocked";
    char* cstr = new char[str.length() + 1];  // +1 for the null terminator
    std::strcpy(cstr, str.c_str());
    return cstr;
}

struct TestMockStruct {
    char * func0Parameter(int) {
        std::string str = "Non mocked";
        char* cstr = new char[str.length() + 1];  // +1 for the null terminator
        std::strcpy(cstr, str.c_str());
        return cstr;
    }
    char * func1Parameter(int) const {
        std::string str = "Non mocked";
        char* cstr = new char[str.length() + 1];  // +1 for the null terminator
        std::strcpy(cstr, str.c_str());
        return cstr;
    }
    static char * testReferenceStubP1P2(int) {
        std::string str = "Non mocked";
        char* cstr = new char[str.length() + 1];  // +1 for the null terminator
        std::strcpy(cstr, str.c_str());
        return cstr;
    }

};

namespace TestNamespace {
    char * func0Parameter(int) {
        std::string str = "Non mocked";
        char* cstr = new char[str.length() + 1];  // +1 for the null terminator
        std::strcpy(cstr, str.c_str());
        return cstr;
    }
}

int main() {
    MOCKER(func);
    std::cout << "global method - func return: " << func(1) << std::endl;

    MOCKER(&TestMockStruct::func0Parameter);
    std::cout << "class member method - TestMockStruct::func0Parameter return: " << TestMockStruct().func0Parameter(1) << std::endl;

    MOCKER(&TestMockStruct::func1Parameter);
    std::cout << "class method method const - TestMockStruct::func1Parameter return: " << TestMockStruct().func1Parameter(1) << std::endl;

    MOCKER(TestNamespace::func0Parameter);
    std::cout << "global method in namespace - TestNamespace::func0Parameter return: " << TestNamespace::func0Parameter(1) << std::endl;

    MOCKER(TestMockStruct::testReferenceStubP1P2);
    std::cout << "class static method - TestMockStruct::testReferenceStubP1P2 return: " << TestMockStruct::testReferenceStubP1P2(1) << std::endl;

    MOCKER(strerror);
    std::cout << "lib method - strerror return: " << strerror(1) << std::endl;
    return 0;
}


#endif //CPPFREEMOCK_MAIN_H
