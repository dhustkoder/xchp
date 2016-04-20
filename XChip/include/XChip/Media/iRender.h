#ifndef _XCHIP_IRENDER_H_
#define _XCHIP_IRENDER_H_
#include <XChip/Utility/StdintDef.h>
#include <XChip/Utility/Color.h>
#include "iMediaInterface.h"
 
namespace xchip {



class iRender : public iMediaInterface
{
public:
	using WinCloseCallback = void(*)(const void*);
	using WinResizeCallback = void(*)(const void*);
	
	virtual bool Initialize(const int w, const int h) noexcept = 0;
	
	virtual utility::Color GetColorFilter() const noexcept = 0;
	
	virtual bool UpdateEvents() noexcept = 0;
	virtual bool SetColorFilter(const utility::Color& color) noexcept = 0;
	virtual void SetBuffer(const uint32_t* gfx) noexcept = 0;
	virtual void DrawBuffer() noexcept = 0;

	virtual void SetWinCloseCallback(const void* arg, WinCloseCallback callback) noexcept = 0;
	virtual void SetWinResizeCallback(const void* arg, WinResizeCallback callack) noexcept = 0;
};















}

#endif
