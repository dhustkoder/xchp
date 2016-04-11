#ifndef _XCHIP_CPU_MANAGER_H_
#define _XCHIP_CPU_MANAGER_H_
#include "Cpu.h"
#include "Utility/Alloc.h"

 
namespace xchip {



class CpuManager
{
public:
	CpuManager() noexcept;
	~CpuManager();
	CpuManager(const CpuManager&) = delete;
	CpuManager& operator=(const CpuManager&) = delete;

	void Dispose() noexcept;

	size_t GetMemorySize() const;
	size_t GetRegistersSize() const;
	size_t GetStackSize() const;
	size_t GetGfxSize() const;

	const iRender* GetRender() const;
	const iInput* GetInput() const;
	const iSound* GetSound() const;
	const uint8_t* GetMemory() const;
	const uint8_t* GetRegisters() const;
	const size_t* GetStack() const;
	const uint32_t* GetGfx() const;
	const Cpu& GetCpu() const;

	Cpu& GetCpu();
	iRender* GetRender();
	iInput* GetInput();
	iSound* GetSound();

	bool SetMemory(const size_t size);
	bool SetRegisters(const size_t size);
	bool SetStack(const size_t size);
	bool SetGfx(const size_t size);
	bool ResizeMemory(const size_t size);
	bool ResizeRegisters(const size_t size);
	bool ResizeStack(const size_t size);
	bool ResizeGfx(const size_t size);
	void SetPC(const size_t offset);
	void SetSP(const size_t offset);
	void SetFont(const uint8_t* font, const size_t size);
	bool LoadRom(const char* file, const size_t at);
	void InsertByte(const uint8_t val, const size_t offset);
	void InsertAddress(void* addr, const size_t offset);
	void PlaceErrorFlag(void* addr);
	void SetErrorFlag(const bool val);
	void SetRender(iRender* render);
	void SetInput(iInput* input);
	void SetSound(iSound* sound);

	iRender* SwapRender(iRender* render);
	iInput* SwapInput(iInput* input);
	iSound* SwapSound(iSound* sound);


	void CleanMemory();
	void CleanRegisters();
	void CleanStack();
	void CleanGfx();


	static void SetErrorFlag(Cpu& _cpu, const bool val);
private:
	Cpu _cpu;

};




inline const iRender* CpuManager::GetRender() const { return _cpu.render; }
inline const iInput* CpuManager::GetInput() const { return _cpu.input; }
inline const iSound* CpuManager::GetSound() const { return _cpu.sound; }
inline const uint8_t* CpuManager::GetMemory() const { return _cpu.memory; }
inline const uint8_t* CpuManager::GetRegisters() const { return _cpu.registers; }
inline const size_t* CpuManager::GetStack() const { return _cpu.stack; }
inline const uint32_t* CpuManager::GetGfx() const { return _cpu.gfx; }
inline const Cpu& CpuManager::GetCpu() const { return _cpu; }


inline Cpu& CpuManager::GetCpu() { return _cpu; }
inline iRender* CpuManager::GetRender() { return _cpu.render; }
inline iInput* CpuManager::GetInput() { return _cpu.input; }
inline iSound* CpuManager::GetSound() { return _cpu.sound; }
inline void CpuManager::SetPC(const size_t offset) { _cpu.pc = offset; }
inline void CpuManager::SetSP(const size_t offset) { _cpu.sp = offset; }
inline void CpuManager::SetRender(iRender* render) { _cpu.render = render; }
inline void CpuManager::SetInput(iInput* input) { _cpu.input = input; }
inline void CpuManager::SetSound(iSound* sound) { _cpu.sound = sound; }
inline void CpuManager::SetErrorFlag(const bool val) { SetErrorFlag(_cpu, val); }

inline void CpuManager::InsertByte(const uint8_t val, const size_t offset) 
{ 
	ASSERT_MSG(_cpu.memory != nullptr,
		"CpuManager::InsertByte: null Cpu::memory!");
	ASSERT_MSG(offset < utility::arr_size(_cpu.memory),
		"CpuManager::InsertByte: offset greater than Cpu::memory size!");
	_cpu.memory[offset] = val; 
}

inline void CpuManager::InsertAddress(void* addr, const size_t offset)  
{
	ASSERT_MSG(_cpu.memory != nullptr,
		"CpuManager::InsertAddress: null Cpu::memory!");
	ASSERT_MSG(offset < utility::arr_size(_cpu.memory),
		"CpuManager::InsertAddress: offset greater than Cpu::memory size!");
	reinterpret_cast<void*&>(_cpu.memory[offset]) = addr; 
}

















} // xchip namespace

#endif
