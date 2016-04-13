#ifndef _XCHIP_EMULATOR_H_
#define _XCHIP_EMULATOR_H_
#include <string>

#include "CpuManager.h"
#include <XChip/Media/Interfaces.h>
#include <XChip/Utility/Memory.h>
#include <XChip/Utility/Timer.h>
#include <XChip/Utility/Assert.h>

 

namespace xchip {
namespace instructions { extern void execute_instruction(Cpu&); }
using UniqueRender = std::unique_ptr<iRender>;
using UniqueInput = std::unique_ptr<iInput>;
using UniqueSound = std::unique_ptr<iSound>;





class Emulator
{
public:
	Emulator() noexcept;
	~Emulator();
	Emulator(const Emulator&) = delete;
	Emulator& operator=(const Emulator&) = delete;

	bool Initialize(UniqueRender render, UniqueInput input, UniqueSound sound) noexcept;

	void Dispose() noexcept;

	bool GetInstrFlag() const;
	bool GetDrawFlag() const;
	bool GetExitFlag() const;
	void HaltForNextFlag() const;

	void UpdateTimers();
	void UpdateSystems();
	void ExecuteInstr();
	void CleanFlags();
	void Draw();
	void Reset();

	void SetInstrPerSec(const unsigned short value);
	void SetFramesPerSec(const unsigned short value);
	bool LoadRom(const std::string& fileName);

	bool SetRender(UniqueRender rend);
	bool SetInput(UniqueInput input);
	bool SetSound(UniqueSound sound);

	iRender* GetRender();
	iInput* GetInput();
	iSound* GetSound();
	
	UniqueRender SwapRender(UniqueRender rend = nullptr);
	UniqueInput SwapInput(UniqueInput input = nullptr);
	UniqueSound SwapSound(UniqueSound sound = nullptr);

private: 
	bool InitRender();
	bool InitInput();
	bool InitSound();

	CpuManager _manager;
	utility::Timer _instrTimer;
	utility::Timer _frameTimer;
	utility::Timer _chDelayTimer;
	bool _instrf = false;
	bool _drawf  = false;
	bool _exitf  = true;
	bool _initialized = false;
};






inline bool Emulator::GetInstrFlag() const { return _instrf; }
inline bool Emulator::GetDrawFlag() const { return _drawf; }
inline bool Emulator::GetExitFlag() const { return _exitf || _manager.GetErrorFlag(); }

inline void Emulator::SetInstrPerSec(const unsigned short value) { 
	_instrTimer.SetTargetTime(utility::literals::operator""_hz(value));
}
inline void Emulator::SetFramesPerSec(const unsigned short value) { 
	_frameTimer.SetTargetTime(utility::literals::operator""_hz(value));
}

inline bool Emulator::LoadRom(const std::string& fname) { return _manager.LoadRom(fname.c_str(), 0x200); }
inline iRender* Emulator::GetRender() { return _manager.GetRender(); }
inline iInput* Emulator::GetInput() { return _manager.GetInput(); }
inline iSound* Emulator::GetSound() { return _manager.GetSound(); }


inline void Emulator::ExecuteInstr()
{
	instructions::execute_instruction(_manager.GetCpu());
	_instrf = false;
}


inline void Emulator::Draw()
{
	ASSERT_MSG(_manager.GetRender() != nullptr,
		"Emulator::Draw: null render!");

	_manager.GetRender()->DrawBuffer();
	_drawf = false;
}









}
#endif