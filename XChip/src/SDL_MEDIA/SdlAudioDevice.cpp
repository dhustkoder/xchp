#include <SDL2/SDL.h>
#include <XChip/SDL_MEDIA/SdlAudioDevice.h>
#include <XChip/SDL_MEDIA/SdlAudioDevice.h>
#include <XChip/Utility/Log.h>



namespace xchip {


SdlAudioDevice::~SdlAudioDevice()
{
	if (_initialized)
		this->Dispose();
}



bool SdlAudioDevice::Initialize(const int wantedFreq, const SDL_AudioFormat wantedFormat, 
		const Uint8 channels, const Uint16 samples,
		const AudioCallback callback, void* userdata) noexcept
{
	using namespace utility::literals;
	
	if (_initialized)
		this->Dispose();

	const auto cleanup = utility::make_scope_exit([this]() { 
		if (!this->_initialized) 
			this->Dispose(); 
	});

	SDL_zero(_want);
	_want.freq = wantedFreq;
	_want.format = wantedFormat;
	_want.channels = channels;
	_want.samples = samples;
	_want.callback = callback;
	_want.userdata = userdata;

	_dev = SDL_OpenAudioDevice(NULL, 0, &_want, &_have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);

	if (_dev < 2)
	{
		utility::LOGerr("SdlSound: no valid audio device. SDL MSG: "_s + SDL_GetError());
		return false;
	}

	_initialized = true;
	return true;
}





void SdlAudioDevice::Dispose() noexcept
{
	SDL_CloseAudioDevice(_dev);
	_initialized = false;
}









}