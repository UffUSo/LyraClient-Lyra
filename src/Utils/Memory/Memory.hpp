#pragma once

#include <windows.h>
#include <psapi.h>
#include <vector>
#include <type_traits>
#include <optional>
#include <future>
#include <execution>
#include <format>
#include "../Logger/Logger.hpp"
#include "MinHook.h"
#include <libhat/Scanner.hpp>
#include <libhat/Access.hpp>
#include <assert.h>

#define in_range(x, a, b) (x >= a && x <= b)
#define get_bits(x) (in_range((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xa) : (in_range(x, '0', '9') ? x - '0' : 0))
#define get_byte(x) (get_bits(x[0]) << 4 | get_bits(x[1]))


template <typename Ret, typename Type> Ret& direct_access(Type* type, size_t offset) {
    union {
        size_t raw;
        Type* source;
        Ret* target;
    } u;
    u.source = type;
    u.raw += offset;
    return *u.target;
}

#define AS_FIELD(type, name, fn) __declspec(property(get = fn, put = set##name)) type name
#define DEF_FIELD_RW(type, name) __declspec(property(get = get##name, put = set##name)) type name

//fake class macro to avoid compile errors when using pragma once

#define FK(typep) \
class typep;

#define FAKE_FIELD(type, name)                                                                                       \
AS_FIELD(type, name, get##name);                                                                                     \
type get##name()

#define BUILD_ACCESS(ptr, type, name, offset)                                                                        \
AS_FIELD(type, name, get##name);                                                                                     \
type get##name() const { return direct_access<type>(ptr, offset); }												     \
void set##name(type v) const { direct_access<type>(ptr, offset) = v; }



class Memory
{
public:

    static void patchBytes(void* dst, void* src, unsigned int size) {
        DWORD oldprotect;
        VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
        memcpy(dst, src, size);
        VirtualProtect(dst, size, oldprotect, &oldprotect);
    }

    static void copyBytes(void* src, void* dst, const uint32_t size) {
        if (src == nullptr || dst == nullptr)
            return;

        DWORD oldprotect;
        VirtualProtect(src, size, PAGE_EXECUTE_READWRITE, &oldprotect);
        memcpy(dst, src, size);
        VirtualProtect(src, size, oldprotect, &oldprotect);
    }

    static void nopBytes(void* dst, const uint32_t size) {
        if (dst == nullptr)
            return;

        DWORD oldprotect;
        VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
        memset(dst, 0x90, size);
        VirtualProtect(dst, size, oldprotect, &oldprotect);
    }

    template <unsigned int IIdx, typename TRet, typename... TArgs>
    static inline auto CallVFunc(void* thisptr, TArgs... argList) -> TRet {
        using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
        return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
    }

    template <typename Ret, typename Type>
    static Ret& direct_access(Type* type, size_t offset) {
        return hat::member_at<Ret>(type, offset);
    }

    static void hookFunc(void* pTarget, void* pDetour, void** ppOriginal, std::string name) {

        if (pTarget == NULL) {
            Logger::error(std::format("{} has invalid address", name));
            return;
        }

        if (MH_CreateHook(pTarget, pDetour, ppOriginal) != MH_OK) {
            Logger::error(std::format("Failed to hook {} function", name));
            return;
        }

        MH_EnableHook(pTarget);

        Logger::info(std::format("Successfully hooked {} function at {}", name, pTarget));
    }

    template <typename R, typename... Args>
    static inline R CallFunc(void* func, Args... args)
    {
        return ((R(*)(Args...))func)(args...);
    }

    template <unsigned int index>
    static void HookVFunc(uintptr_t sigOffset, void* pDetour, void** ppOriginal, std::string name) {
        uintptr_t** vTable = reinterpret_cast<uintptr_t**>(sigOffset + 3 + 7);

        hookFunc(vTable[index], pDetour, ppOriginal, name);
    }


    static auto findSig(std::string_view signature)
    {
        auto sig = hat::parse_signature(signature);
        assert(sig.has_value());
        auto result = hat::find_pattern(sig.value(), ".text");
        //assert(result.has_result());
        return result.has_result() ? reinterpret_cast<uintptr_t>(result.get()) : NULL;
    }


    template<typename T>
    static void SafeRelease(T*& pPtr)
    {
        if (pPtr != nullptr)
        {
            pPtr->Release();
            pPtr = nullptr;
        }
    }

    static uintptr_t findDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
    {
        uintptr_t addr = ptr;

        for (int i = 0; i < offsets.size(); ++i)
        {
            addr = *(uintptr_t *)addr;
            addr += offsets[i];
        }
        return addr;
    }

    static inline uintptr_t offsetFromSig(uintptr_t sig, int offset) {
        return sig + offset + 4 + *reinterpret_cast<int*>(sig + offset);
    }

    template <typename Ret>
    static auto getFromOffset(const uintptr_t& addr, const int& offset) {
        return reinterpret_cast<Ret>(offsetFromSig(addr, offset));
    }

    template <typename Ret, typename... Args>
    static auto CallThisFunc(uintptr_t addr, void* thisptr, Args... argList) -> Ret {
        using Fn = Ret(__thiscall*)(void*, decltype(argList)...);
        return reinterpret_cast<Fn>(addr)(thisptr, argList...);
    }

    static inline std::array<std::byte, 4> getRipRel(uintptr_t instructionAddress, uintptr_t targetAddress) {
        uintptr_t relAddress = targetAddress - (instructionAddress + 4); // 4 bytes for RIP-relative addressing
        std::array<std::byte, 4> relRipBytes{};

        for (size_t i = 0; i < 4; ++i) {
            relRipBytes[i] = static_cast<std::byte>((relAddress >> (i * 8)) & 0xFF);
        }

        return relRipBytes;
    }
    
};
