#ifndef CPPFREEMOCK_MAIN_H
#define CPPFREEMOCK_MAIN_H

#include "cpp_free_mock.h"
#include <iostream>
#include <string>

std::string func(int) {
    return "very long long ......................   ................... Non mocked.";
}

struct TestMockStruct {
    std::string func0Parameter(int) {
        return "Non mocked.";
    }
    std::string func1Parameter(int) const {
        return "Non mocked.";
    }
    static std::string testReferenceStubP1P2(int) {
        return "Non mocked.";
    }

};

namespace TestNamespace {
    std::string func0Parameter(int) {
        return "Non mocked.";
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
    return 0;
}


#endif //CPPFREEMOCK_MAIN_H
