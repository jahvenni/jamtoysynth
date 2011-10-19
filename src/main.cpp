#include <stdbool.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <GL/glew.h>


#include <TUIO/TuioClient.h>

#include <touchlistener.h>
#include <screen.h>
#include <simulator.h>

typedef struct audio_callback_userdata_t
{
    SDL_AudioSpec audiospec;
} audio_callback_userdata_t;

static void audio_callback(void *userdata, uint8_t *stream, int len)
{
    for(int i = 0; i < len; ++i)
        stream[i] = rand() % 0xff;
}

static int process_events()
{
  SDL_Event event;
  
  while( SDL_PollEvent( &event ) ) {
    switch( event.type ) {
    case SDL_MOUSEMOTION:
      if (event.button.button & SDL_BUTTON(1)) 
	sim_mouse_dragged((float)event.button.x/screen_width, (float)event.button.y/screen_height);
      break;
    case SDL_MOUSEBUTTONDOWN:
      if (event.button.button & SDL_BUTTON(1)) 
	sim_mouse_pressed((float)event.button.x/screen_width, (float)event.button.y/screen_height);
      break;
    case SDL_MOUSEBUTTONUP:
      if (event.button.button & SDL_BUTTON(1)) 
	sim_mouse_released((float)event.button.x/screen_width, (float)event.button.y/screen_height);
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
	return true;
      break;
    case SDL_QUIT:
      return true;
      break;
    }
  }
  return false;
}

static int main_loop()
{
  bool quit = false;

  while(!quit)
    {
      sim_init_frame();
      quit = process_events();
      sim_commit_frame();
      screen_render();
    }
  
  return 0;
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
        return -1;
    atexit(SDL_Quit);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

    if (screen_init() != 0)
      return -1;

    if(glewInit() != GLEW_OK)
        return -1;

    audio_callback_userdata_t audio_callback_userdata;

    SDL_AudioSpec desired_audio_spec, obtained_audio_spec;
    desired_audio_spec.freq = 44100;
    desired_audio_spec.format = AUDIO_U16SYS;
    desired_audio_spec.channels = 2;
    desired_audio_spec.samples = 1024;
    desired_audio_spec.callback = audio_callback;
    desired_audio_spec.userdata = &audio_callback_userdata;

    if(SDL_OpenAudio(&desired_audio_spec, &obtained_audio_spec) != 0)
        return -1;
    atexit(SDL_CloseAudio);
    audio_callback_userdata.audiospec = obtained_audio_spec;

    sim_init();

    TouchListener client_listener;
    TUIO::TuioClient client;
    client.addTuioListener(&client_listener);
    client.connect();

    SDL_PauseAudio(0);

    int err = main_loop();
    
    SDL_PauseAudio(1);

    SDL_CloseAudio();
    SDL_Quit();

    return err;
}

