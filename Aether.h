#pragma once
#include <cstdint>
#include <Windows.h>

namespace Aether
{
	inline HMODULE   hNTDLL    = nullptr;
	inline uintptr_t NTDLLBase = 0;

	constexpr uint8_t SyscallPattern[ ] = {
		0xB8,
		0x00,
		0x00,
		0x00,
		0x00
	};
	constexpr char SyscallMask[ ] = "x????";

	namespace Stub
	{
		extern "C" void* SyscallStub();

		template <typename... Args_t>
		static void* StubCaller( Args_t... Args )
		{
			void* (*Function)( Args_t... ) = reinterpret_cast<void* (*)( Args_t... )>( &SyscallStub );
			return Function( Args... );
		}

		template <typename Ret_t = NTSTATUS, typename First_t = void*, typename Second_t = void*, typename Third_t = void*, typename Fourth_t = void*, typename... Pack_t>
		static Ret_t PerformCall( uint32_t IDX = {}, First_t First = {}, Second_t Second = {}, Third_t Third = {}, Fourth_t Fourth = {}, Pack_t... Pack )
		{
			if ( IDX == 0x13371337 ) return Ret_t{};
			return reinterpret_cast<Ret_t>( StubCaller( First, Second, Third, Fourth, IDX, nullptr, Pack... ) ); // NOLINT(clang-diagnostic-void-pointer-to-int-cast)
		}
	}

	inline bool MatchPattern( const uint8_t* Data, const uint8_t* Pattern, const char* Mask )
	{
		for ( ; *Mask; ++Mask, ++Data, ++Pattern ) { if ( *Mask == 'x' && *Data != *Pattern ) { return false; } }

		return true;
	}

	inline uintptr_t FindPattern( const uintptr_t RoutineBase, const size_t MaxSize, const uint8_t* Pattern, const char* Mask )
	{
		for ( size_t i = 0; i < MaxSize; ++i ) { if ( MatchPattern( reinterpret_cast<const uint8_t*>( RoutineBase + i ), Pattern, Mask ) ) { return RoutineBase + i; } }

		return 0;
	}

	inline uint32_t FindSyscallIDX( const char* RoutineName )
	{
		if ( !hNTDLL )
		{
			hNTDLL    = GetModuleHandleA( "ntdll.dll" );
			NTDLLBase = reinterpret_cast<uintptr_t>( hNTDLL );
		}

		const FARPROC pSyscall = GetProcAddress( hNTDLL, RoutineName );

		if ( !pSyscall ) { return 0x13371337; }

		// Find mov eax, IDX
		const uintptr_t MovEaxIDX = FindPattern( reinterpret_cast<uintptr_t>( pSyscall ), 0x100, SyscallPattern, SyscallMask );
		if ( MovEaxIDX != 0 ) { return *reinterpret_cast<uint32_t*>( MovEaxIDX + 1 ); }

		return 0;
	}

	template <typename Ret_t, typename... Args>
	Ret_t Syscall( const char* Name, Args... args )
	{
		uint32_t IDX = FindSyscallIDX( Name );
		return Stub::PerformCall<Ret_t>( IDX, args... );
	}

	// Example Implementation
	inline NTSTATUS NtWriteVirtualMemory( const HANDLE ProcessHandle, const PVOID BaseAddress, const PVOID Buffer, const SIZE_T BufferSize, const SIZE_T* NumberOfBytesWritten )
	{
		// NTSTATUS here is redundant but just displayed for clarity on how to use PerformCall
		return Stub::PerformCall<NTSTATUS>( FindSyscallIDX( "NtWriteVirtualMemory" ), ProcessHandle, BaseAddress, Buffer, BufferSize, NumberOfBytesWritten );
	}
}
