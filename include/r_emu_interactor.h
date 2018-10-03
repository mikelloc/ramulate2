#pragma once

#include <stdbool.h>
#include <r_util.h>
#include <r_io.h>
#include <r_anal.h>
#include <r_th.h>
#include <r_emu.h>

#define	R_EMU_INTERACTOR_JOYPAD_A	(0x1 << 0)
#define	R_EMU_INTERACTOR_JOYPAD_B	(0x1 << 1)
#define	R_EMU_INTERACTOR_JOYPAD_START	(0x1 << 2)
#define	R_EMU_INTERACTOR_JOYPAD_SELECT	(0x1 << 3)
#define	R_EMU_INTERACTOR_JOYPAD_UP	(0x1 << 4)
#define	R_EMU_INTERACTOR_JOYPAD_DOWN	(0x1 << 5)
#define	R_EMU_INTERACTOR_JOYPAD_LEFT	(0x1 << 6)
#define	R_EMU_INTERACTOR_JOYPAD_RIGHT	(0x1 << 7)

typedef struct r_emu_interactor_t {
	struct r_emu_interactor_plugin_t *plugin;
	void *user;
	ut16 height;
	ut16 width;
	bool fullscreen;	//do we need this
	ut8 voices;
} REmuInteractor;

typedef struct r_emu_interactor_plugin_t {
	const char *name;
	const char *desc;
	void *(*init)();
	void (*fini)(REmuInteractor *ei);
	void (*init_joypad)(REmuInteractor *ei);
	ut8 (*poll_joypad)(REmuInteractor *ei);
	void (*init_screen)(REmuInteractor *ei, ut16 width, ut16 height, bool fullscreen);
	void (*set_pixel)(REmuInteractor *ei, ut16 x, ut16 y, ut32 rgb);
	void (*init_sound)(REmuInteractor *ei, ut8 voices);
	//	void (*play_wave)(ut8 voice, ...)	//what to do here
} REmuInteractorPlugin;


/*
 * For playing wave with SDL we need the followig function
 *
 * maybe should have a second parameter specifying the playing time
 *
 * */
int play_wave(chat * filename)
{
	/*
	 * Init SDL audo engine and variables
	 */
	SDL_Init(SDL_INIT_AUDIO);
	SDL_AudioSpec wav_desc;
	uint32 wav_length;
	uint8 * wav_buffer;
	/*
	 * Load file and open audio device
	 */
	SDL_LoadWAV(filename, &wav_desc, &wav_buffer, &wav_length);
	SDL_AudioDeviceID did = SDL_OpenAudioDevice(NULL, 0, &wav_desc, NULL, 0);
	int success = SDL_QueueAudio(did, wav_buffer, wav_length);
	/*
	 * release the pause (so play)
	 */
	SDL_PauseAudioDevice(did, 0);
	/*
	 * we need to last sufficient time to play sound before close it!
	 * maybe with:
	 *	SDL_Delay(milliseconds);
	 *
	 * also the cleanup can be done in other separe function
	 * stop_wave() clean_wave() ?
	 */
	SDL_CloseAudioDevice(did);
	SDL_FreeWAV(wav_buffer);
	return success;
}

R_API REmuInteractorPlugin *r_emu_interactor_plugin_get(char *name);
R_API void *r_emu_interactor_plugin_init(REmuInteractorPlugin *eip);
R_API void r_emu_interactor_plugin_fini(REmuInteractorPlugin *eip, REmuInteractor *ei);
R_API ut8 r_emu_interactor_plugin_poll_joypad(REmuInteractorPlugin *eip, REmuInteractor *ei);
R_API void r_emu_interactor_plugin_init_screen(REmuInteractorPlugin *eip, REmuInteractor *ei, ut16 width, ut16 height, bool fullscreen);
R_API void r_emu_interactor_plugin_set_pixel(REmuInteractorPlugin *eip, REmuInteractor *ei, ut16 x, ut16 y, ut32 rgb);
R_API void r_emu_interactor_plugin_init_sound(REmuInteractorPlugin *eip, REmuInteractor *ei, ut8 voices);
