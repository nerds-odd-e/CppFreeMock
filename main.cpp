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

struct ClassA {
    char * func0(int) {
        std::string str = "Non mocked 0";
        char* cstr = new char[str.length() + 1];  // +1 for the null terminator
        std::strcpy(cstr, str.c_str());
        return cstr;
    }
    int a;
};

struct ClassB : ClassA {
    char * func0(int) {
        std::string str = "Non mocked 1";
        char* cstr = new char[str.length() + 1];  // +1 for the null terminator
        std::strcpy(cstr, str.c_str());
        return cstr;
    }
    int b;
};

struct ClassC : ClassB {
    char * func0(int) {
        std::string str = "Non mocked 2";
        char* cstr = new char[str.length() + 1];  // +1 for the null terminator
        std::strcpy(cstr, str.c_str());
        return cstr;
    }
    int c;
};

struct ClassD {
    char * func0(int) {
        std::string str = "Non mocked 3";
        char* cstr = new char[str.length() + 1];  // +1 for the null terminator
        std::strcpy(cstr, str.c_str());
        return cstr;
    }
    int d;
};

struct ClassE : ClassC, ClassD {
    char * func0(int) {
        std::string str = "Non mocked 4";
        char* cstr = new char[str.length() + 1];  // +1 for the null terminator
        std::strcpy(cstr, str.c_str());
        return cstr;
    }
    int e;
};

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

    MOCKER(&ClassA::func0);
    std::cout << "class method - ClassA::func0 return: " << ClassA().func0(1) << std::endl;
    MOCKER(&ClassB::func0);
    std::cout << "class method - ClassB::func0 return: " << ClassB().func0(1) << std::endl;
    MOCKER(&ClassC::func0);
    std::cout << "class method - ClassC::func0 return: " << ClassC().func0(1) << std::endl;
    MOCKER(&ClassD::func0);
    std::cout << "class method - ClassD::func0 return: " << ClassD().func0(1) << std::endl;
    MOCKER(&ClassE::func0);
    std::cout << "class method - ClassE::func0 return: " << ClassE().func0(1) << std::endl;
    return 0;
}


#endif //CPPFREEMOCK_MAIN_H
