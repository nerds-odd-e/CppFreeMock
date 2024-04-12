// Copyright 2014 Louix Gu
// Author: gzc9047@gmail.com (Louix Gu)

// CppFreeMock: a tool for mock global function, member function, class static function.
//
// Implement under c++11.

#ifndef CPP_FREE_MOCK_CPP11_IMPL_H_
#define CPP_FREE_MOCK_CPP11_IMPL_H_

#include "cpp11/basic_type.h"

#include "runtime_patch.h"

#include <list>
#include <memory>
//#include <unordered_map>
#include <map>
#include <vector>
// I don't include gmock and gtest file here, because maybe you have you own include patch.
#include <iostream>
#include <cstring>

namespace CppFreeMock {

// How it works:
//  1, I create a entry point for runtime patch, to move the cpu run here;
//  2, In the entry point, it call a mocked method to do the mock function.
//      a) Because the entry point is a static function, so the mocked method should be static or invoke from a static variable.
//      b) Because we can mock member function, so the mocked method must be a member function, so we can only use a static varibale to call it.
//  3, So I create MockerStore, with a static variable.
//  4, Hack gmock, create Mocker as MOCK_METHOD does.

// I use function type to combine one type and a few other type into one type.
// We can also use another type like Combiner to do this.
// In this file:
//  1, type I means the uniq id type for every mocker.
//  2, type C means class type of the mockee function, if it's a member function of static function.
//  3, type R means the mockee function's return type.
//  4, variable type ... P means the function's parameter types.

    template < typename I, typename R, typename P >
    class MockerEntryPoint<I(R(P))> {
        typedef I IntegrateType(R(P));
        friend class Mocker<IntegrateType>;
        static R EntryPoint(P p) {
            std::cout << "enter entry point 1" << std::endl;
            return SimpleSingleton< Mocker<IntegrateType> >::getInstance()->CppFreeMockStubFunction(p);
        }
    };

#define MOCKER_ENTRY_POINT_WITH_THIS_POINTER(constness) \
template < typename I, typename C, typename R, typename P > \
class MockerEntryPoint<I(R(C::*)(P) constness)> { \
    typedef I IntegrateType(R(C::*)(const void*, P) constness); \
    friend class Mocker<IntegrateType>; \
    R EntryPoint(P p) { \
        std::cout << "enter entry point 2" << std::endl; \
        return SimpleSingleton< Mocker<IntegrateType> >::getInstance()->CppFreeMockStubFunction(p); \
    } \
}

    MOCKER_ENTRY_POINT_WITH_THIS_POINTER(const);
    MOCKER_ENTRY_POINT_WITH_THIS_POINTER();

#undef MOCKER_ENTRY_POINT_WITH_THIS_POINTER

// This class is hack how gmock implement it.
    template < typename R, typename P >
    struct MockerBase<R(*)(P)> {
        MockerBase() {}
        MockerBase(const std::string& _functionName): functionName(_functionName) {}
        virtual ~MockerBase() {}

        // Use 'CppFreeMockStubFunction' as the function name for EXPECT_CALL.
        R CppFreeMockStubFunction(P p) {
            std::string str = "it's working";
            char* cstr = new char[str.length() + 1];  // +1 for the null terminator
            std::strcpy(cstr, str.c_str());
            return cstr;
//            gmocker.SetOwnerAndName(this, functionName.c_str());
//            return gmocker.Invoke(p);
        }

//        ::testing::MockSpec<R(P)>& gmock_CppFreeMockStubFunction(const ::testing::Matcher<P>& p) {
//            gmocker.RegisterOwner(this);
//            return gmocker.With(p);
//        }

        virtual void RestoreToReal() = 0;

//        mutable ::testing::FunctionMocker<R(P)> gmocker;
        std::vector<char> binaryBackup; // Backup the mockee's binary code changed in RuntimePatcher.
        const std::string functionName;
    };

    template < typename R, typename P >
    struct MockerBase<R(*)(const void*, P)> {
        MockerBase() {}
        MockerBase(const std::string& _functionName): functionName(_functionName) {}
        virtual ~MockerBase() {}

        // Use 'CppFreeMockStubFunction' as the function name for EXPECT_CALL.
        R CppFreeMockStubFunction(P p) {
            std::string str = "it's working";
            char* cstr = new char[str.length() + 1];  // +1 for the null terminator
            std::strcpy(cstr, str.c_str());
            return cstr;
//            gmocker.SetOwnerAndName(this, functionName.c_str());
//            return gmocker.Invoke(p);
        }

//        ::testing::MockSpec<R(P)>& gmock_CppFreeMockStubFunction(const ::testing::Matcher<P>& p) {
//            gmocker.RegisterOwner(this);
//            return gmocker.With(p);
//        }

        virtual void RestoreToReal() = 0;

//        mutable ::testing::FunctionMocker<R(P)> gmocker;
        std::vector<char> binaryBackup; // Backup the mockee's binary code changed in RuntimePatcher.
        const std::string functionName;
    };

    template < typename I, typename R, typename P>
    struct Mocker<I(R(*)(P))> : MockerBase<R(*)(P)> {
        typedef I IntegrateType(R(*)(P));
        typedef R FunctionType(P);
        Mocker() {};
        Mocker(FunctionType function, const std::string& functionName):
                MockerBase<R(*)(P)>(functionName),
                originFunction(function) {
            Mocker<I(R(*)(P))>* instance_mocker = SimpleSingleton< Mocker<I(R(P))> >::getInstance();
            instance_mocker = this;
            RuntimePatcher::GraftFunction(originFunction,
                                          MockerEntryPoint<IntegrateType>::EntryPoint,
                                          MockerBase<R(*)(P)>::binaryBackup);
        }

        virtual ~Mocker() {
            RestoreToReal();
        }

        void RestoreToReal() {
            RuntimePatcher::RevertGraft(originFunction, MockerBase<R(*)(P)>::binaryBackup);
//            SimpleSingleton<I(R(P))>::getInstance() = NULL;
            Mocker<I(R(*)(P))>* instance_mocker = SimpleSingleton< Mocker<I(R(P))> >::getInstance();
            instance_mocker = NULL;
        }

        FunctionType* originFunction;
    };

#define MOCKER_WITH_THIS_POINTER_CHECK(constness) \
template < typename I, typename C, typename R, typename P> \
struct Mocker<I(R(C::*)(const void*, P) constness)> : MockerBase<R(*)(const void*, P)> { \
    typedef I IntegrateType(R(C::*)(const void*, P) constness); \
    typedef I EntryPointType(R(C::*)(P) constness); \
    typedef R (C::*FunctionType)(P) constness; \
    typedef R StubFunctionType(const void*, P); \
    Mocker() {}; \
    Mocker(FunctionType function, const std::string& functionName): \
        MockerBase<R(*)(const void*, P)>(functionName), \
        originFunction(function) { \
        RuntimePatcher::GraftFunction(originFunction, \
                &MockerEntryPoint<EntryPointType>::EntryPoint, \
                MockerBase<R(*)(const void*, P)>::binaryBackup); \
    } \
    virtual ~Mocker() { \
        RestoreToReal(); \
    } \
    virtual void RestoreToReal() { \
        RuntimePatcher::RevertGraft(originFunction, MockerBase<R(*)(const void*, P)>::binaryBackup); \
        Mocker<I(R(C::*))>* instance_mocker = SimpleSingleton< Mocker<I(R(C::*))> >::getInstance(); \
        instance_mocker = NULL; \
    } \
    FunctionType originFunction; \
}

    MOCKER_WITH_THIS_POINTER_CHECK(const);
    MOCKER_WITH_THIS_POINTER_CHECK();

#undef MOCKER_WITH_THIS_POINTER_CHECK

    template < typename T >
    struct MockerCache {
    private:
        friend class MockerCreator;
        typedef std::map<const void*, T*> HashMap;

    static HashMap* getInstance() {
        return SimpleSingleton<HashMap>::getInstance();
    }

    static void RestoreCachedMockFunctionToReal() {
        HashMap* instance_map = getInstance();
        for (typename HashMap::iterator it = instance_map->begin(); it != instance_map->end(); ++it) {
            T* mocker = it->second;
            mocker->RestoreToReal();
            delete mocker;
        }
        getInstance()->clear();
    }
};

struct MockerCreator {
private:
    typedef std::list<void(*)()> RestoreFunctions;

    template < typename I, typename R, typename P >
    static MockerBase<R(*)(P)>*
            CreateMocker(R function(P), const std::string& functionName) {
        Mocker<I(R(*)(P))>* mocker = new Mocker<I(R(*)(P))>(function, functionName);
        return mocker;
    }

#define CREATE_MOCKER_WITH_THIR_POINTER_CHECK(constness) \
    template < typename I, typename C, typename R, typename P > \
    static MockerBase<R(*)(const void*, P)>* \
            CreateMocker(R (C::*function)(P) constness, const std::string& functionName) { \
        typedef I IntegrateType(R(C::*)(const void*, P) constness);                        \
        Mocker<IntegrateType>* mocker = new Mocker<IntegrateType>(function, functionName); \
        return mocker; \
    }

#if __cplusplus >= 201103L
    CREATE_MOCKER_WITH_THIR_POINTER_CHECK(const)
#endif
    CREATE_MOCKER_WITH_THIR_POINTER_CHECK()
#undef CREATE_MOCKER_WITH_THIR_POINTER_CHECK

    template < typename I, typename M, typename F >
    static const M*
            DoGetMocker(F function, const std::string& functionName) {
        typedef MockerCache<M> MockerCacheType;
        typedef std::map<const void*, M*> HashMap;
        const void* address = (const void*)((std::size_t&)function);
        typename HashMap::iterator got = MockerCacheType::getInstance()->find(address);
        if (got != MockerCacheType::getInstance()->end()) {
            return got->second;
        } else {
            SimpleSingleton<RestoreFunctions>::getInstance()->push_back(MockerCacheType::RestoreCachedMockFunctionToReal);
//            MockerCacheType::getInstance().insert({{address, CreateMocker<I>(function, functionName)}});
            (*MockerCacheType::getInstance())[address] = CreateMocker<I>(function, functionName);
            return DoGetMocker<I, M>(function, functionName);
        }
    }

public:
    template < typename I, typename R, typename P >
    static const MockerBase<R(*)(P)>*
            GetMocker(R function(P), const std::string& functionName) {
        return DoGetMocker<I, MockerBase<R(*)(P)>, R(*)(P)>(function, functionName);
    }

#define GET_MOCKER_WITH_THIR_POINTER_CHECK(constness) \
    template < typename I, typename C, typename R, typename P > \
    static const MockerBase<R(*)(const void*, P)>* \
            GetMocker(R (C::*function)(P) constness, const std::string& functionName) { \
        return DoGetMocker<I, MockerBase<R(*)(const void*, P)>, R(C::*)(P) constness>(function, functionName); \
    }

#if __cplusplus >= 201103L
    GET_MOCKER_WITH_THIR_POINTER_CHECK(const)
#endif
    GET_MOCKER_WITH_THIR_POINTER_CHECK()
#undef GET_MOCKER_WITH_THIR_POINTER_CHECK

    static void RestoreAllMockerFunctionToReal() {
//        for (auto& restorer : SimpleSingleton<RestoreFunctions>::getInstance()) {
//            restorer();
//        }
        RestoreFunctions* instance_list = SimpleSingleton<RestoreFunctions>::getInstance();
        for (RestoreFunctions::iterator it = instance_list->begin(); it != instance_list->end(); ++it) {
            void (*func)() = (void (*)())(*it);
            func();
        }
        SimpleSingleton<RestoreFunctions>::getInstance()->clear();
    }
};

} // namespace CppFreeMock

#endif // CPP_FREE_MOCK_CPP11_IMPL_H_
