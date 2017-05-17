#ifndef GUARD_EC23940F_426F_49E2_8169_61CFC2559670
#define GUARD_EC23940F_426F_49E2_8169_61CFC2559670

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER)
    long __cdecl InterlockedIncrement(long volatile *Addend);
    long __cdecl InterlockedDecrement(long volatile *Addend);
#elif defined(__GNUC__)
    #ifndef InterlockedIncrement
        #define InterlockedIncrement(p) \
            __atomic_add_fetch(p, 1, __ATOMIC_SEQ_CST)
    #endif
    #ifndef InterlockedDecrement
        #define InterlockedDecrement(p) \
            __atomic_sub_fetch(p, 1, __ATOMIC_SEQ_CST)
    #endif
#else
    #error "Atomics not implemented for current compiler"
#endif

#ifdef __cplusplus
}
#endif


#endif
