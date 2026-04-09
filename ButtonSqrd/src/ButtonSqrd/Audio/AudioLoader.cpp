#include "AudioLoader.h"

#pragma warning(push, 0)

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c" 

#define MINIAUDIO_IMPLEMENTATION
#include <MiniAudio/miniaudio.h>

#undef STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

#pragma warning(pop)

namespace BtnSqd {
	void LoadPCM(ma_decoder& d, PCMPackage& pcm) {	
		ma_uint64 frameCount;
		if (ma_decoder_get_length_in_pcm_frames(&d, &frameCount) != MA_SUCCESS) {
			BTNLOG_ERROR("Failed to get audio pcm frame length")
		}

		pcm.channels = d.outputChannels;
		pcm.sampleRate = d.outputSampleRate;
		pcm.samples.resize(static_cast<size_t>(frameCount) * pcm.channels);

		ma_uint64 framesRead = 0;
		ma_result result = ma_decoder_read_pcm_frames(&d,pcm.samples.data(),frameCount,&framesRead);

		if (result != MA_SUCCESS) {
			BTNLOG_ERROR(ma_result_description(result))
		}
	}


	PCMPackage AudioLoader::Load(const std::string& soundFile) {
		ma_decoder_config cfg = ma_decoder_config_init(ma_format_s16, 1, 0);
		ma_decoder audioDecoder;

		ma_result result = ma_decoder_init_file(soundFile.c_str(), &cfg, &audioDecoder);
		if (result != MA_SUCCESS) {
			BTNLOG_ERROR("Failed to load sound file '{}'. Error({})",soundFile, ma_result_description(result));
			return PCMPackage();
		}
		PCMPackage pcm;
		LoadPCM(audioDecoder, pcm);

		ma_decoder_uninit(&audioDecoder);
		return pcm;
	}
}