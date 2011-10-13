#include <SDL.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include <synth.h>

void callback(void *userdata, Uint8 *data, int len)
{
	int i;
	
	(void)userdata;
	
	mixer();
	
	Uint16 *stream = (Uint16*)data;
	for(i = 0; i < len / 2; i++)
	{
		Uint16 sample;
		sample = (((audioBuffer[i] + 0x10000) >> 1) & 0xFFFF);
		stream[i] = sample;
	}
}

#define CARRIER 1
#define MODULATOR 2
#define SWEEP 3
void instrumentSetFreq(Instrument *instr, float freq, int oscillator)
{
	if(oscillator == CARRIER) instr->freq1 = FLOATTOFIX(freq / SAMPLE_RATE);
	else if(oscillator == MODULATOR)
	{
		if(freq < 0) instr->freq2 = FLOATTOFIX(freq);
		else instr->freq2 = FLOATTOFIX(freq / SAMPLE_RATE);
	} else if(oscillator == SWEEP)
	{
		// freq = frequency sweep in kHz / sec
		freq = freq * AUDIO_BUFFER_SIZE / (SAMPLE_RATE * 1000.0);
		instr->sweep = FLOATTOFIX(freq);
	}
}

#define NO_FILTER 0
#define LOWPASS 1
#define HIGHPASS 2
#define BANDPASS 3
#define NOTCH 4
#define PEAKING_EQ 5
#define LOW_SHELF 6
#define HIGH_SHELF 7

void instrumentSetFilter(Instrument *instr, int type, double f0, double Q, double dBgain)
{
	double cosw0 = cos(2*M_PI*f0/SAMPLE_RATE);
	double sinw0 = sqrt(1.0 - cosw0 * cosw0); //sin(2*M_PI*f0/SAMPLE_RATE);
	double alpha = sinw0 / (2*Q);
	double A = pow(10.0, dBgain/40);
	double A2 = 2*sqrt(A);
	double a0, a1, a2, b0, b1, b2;
	
	switch (type)
	{
		case LOWPASS:
			b0 = (1 - cosw0)/2;
			b1 = 1 - cosw0;
			b2 = (1 - cosw0)/2;
			a0 = 1 + alpha;
			a1 = -2.0 * cosw0;
			a2 = 1 - alpha;
			break;
		case HIGHPASS:
			b0 = (1 + cosw0)/2;
			b1 = -(1 + cosw0);
			b2 = (1 + cosw0)/2;
			a0 = 1 + alpha;
			a1 = -2 * cosw0;
			a2 = 1 - alpha;
			break;
		case BANDPASS:
			b0 = alpha;
			b1 = 0;
			b2 = -alpha;
			a0 = 1 + alpha;
			a1 = -2 * cosw0;
			a2 = 1 - alpha;
			break;
		case NOTCH:
			b0 = 1;
			b1 = -2*cosw0;
			b2 = 1;
			a0 = 1 + alpha;
			a1 = -2*cosw0;
			a2 = 1-alpha;
			break;	
		case PEAKING_EQ:
			b0 = 1 + alpha*A;
			b1 = -2*cosw0;
			b2 = 1 - alpha*A;
			a0 = 1 + alpha/A;
			a1 = -2*cosw0;
			a2 = 1 - alpha/A;
			break;
		case LOW_SHELF:
			b0 = A*((A+1) - (A-1)*cosw0 + A2 * alpha);
			b1 = 2*A*((A-1) - (A+1) * cosw0);
			b2 = A*((A+1) - (A-1) * cosw0 - A2 * alpha);
			a0 = (A+1) + (A-1) * cosw0 + A2 * alpha;
			a1 = -2*((A-1) + (A+1) * cosw0);
			a2 = (A+1) + (A-1) * cosw0 - A2 * alpha;
			break;
		case HIGH_SHELF:
			b0 = A*((A+1) + (A-1) * cosw0 + A2 * alpha);
			b1 = -2*A*((A-1) + (A+1) * cosw0);
			b2  = A*((A+1) + (A-1) * cosw0 - A2 * alpha);
			a0 = (A+1) - (A-1) * cosw0 + A2 * alpha;
			a1 = 2*((A-1) - (A+1) * cosw0);
			a2 = (A+1) - (A-1) * cosw0 - A2 * alpha;
			break;
		case NO_FILTER:
			b0 = a0 = 1.0;
			b1 = b2 = 0.0;
			a1 = a2 = 0.0;
			break;
		default:
			break;
	}
		
	instr->a0 = FLOATTOFIX(a0 * 64.0);
	instr->a1 = -FLOATTOFIX(a1 * 64.0);
	instr->a2 = -FLOATTOFIX(a2 * 64.0);
	instr->b0 = FLOATTOFIX(b0 * 64.0);
	instr->b1 = FLOATTOFIX(b1 * 64.0);
	instr->b2 = FLOATTOFIX(b2 * 64.0);
}

void instrumentSetADSR(Instrument *instr, double attack, double decay, double sustain, double release)
{
	instr->attackG = FLOATTOFIX(exp(-1.0 / (attack * SAMPLE_RATE)));
	instr->decayG = FLOATTOFIX(exp(-1.0 / (decay * SAMPLE_RATE)));
	instr->releaseG = FLOATTOFIX(exp(-1.0 / (release * SAMPLE_RATE)));
	instr->sustain = FLOATTOFIX(sustain);
	instr->decay = decay * SAMPLE_RATE;
	instr->decayTimer = 0;
	
	instr->adsrX = 0;
	instr->adsrG = 0;
}

void instrumentTrigger(Instrument *instr)
{
// 	instr->adsrState = ATTACK;
	instr->adsrG = instr->attackG;
	instr->adsrX = FLOATTOFIX(1.5);
	instr->decayTimer = 0;	// leaving this out will cause failure if idle for 27 hours
}

void synthSetEcho(double delay, double feedback)
{
	echoWriteIndex = delay * SAMPLE_RATE + 0.5;
	echoWriteIndex &= 0xFFFF;
	echoFeedback = FLOATTOFIX(feedback);
}

#include <GL/gl.h>

							//	c		c#		d		d#		e		f		f#		g		g#		a		a#,		b
const SDLKey keyboard[] = { SDLK_z, SDLK_s, SDLK_x, SDLK_d, SDLK_c, SDLK_v, SDLK_g, SDLK_b, SDLK_h, SDLK_n, SDLK_j, SDLK_m,
							SDLK_q, SDLK_2, SDLK_w, SDLK_3, SDLK_e, SDLK_r, SDLK_5, SDLK_t, SDLK_6, SDLK_y, SDLK_7, SDLK_u};

double noteFreq(int i)
{
	const double A = 440.0;
	return (A / 32.0) * pow(2.0, (i - 9) / 12.0);
}

int main(int argc, char *argv[])
{
	SDL_AudioSpec spec;
	int baseOctave = 4;
	int currentChannel = 0;
	int i;

	(void)argc;
	(void)argv;
	
	memset(instruments, 0, NUM_INSTRUMENTS * sizeof(Instrument));
	for(i = 0; i < NUM_INSTRUMENTS; ++i)
	{
		Instrument *instr = &instruments[i];
	
		instrumentSetFreq(instr, 164.0, CARRIER);
		instr->sample1 = SQUARE_WAVE;
		instr->amplitude1 = FLOATTOFIX(0.9);
	
		instrumentSetFreq(instr, -0.5, MODULATOR);
		instr->sample2 = SINE_WAVE;
		instr->amplitude2 = FLOATTOFIX(0.5);
	
// 		instrumentSetFreq(instr, -2.0, SWEEP);
	
		instr->modulation = FM_MODULATION;
	
		instrumentSetFilter(instr, LOWPASS, 500, 0.5, 0.5);
		instrumentSetADSR(instr, 0.001, 1.0, 0.7, 1.5);
	
		instr->volume = FLOATTOFIX(0.5);
		instr->echo = FLOATTOFIX(0.20);
	}
	
	synthSetEcho(0.25, 0.5);
	synthInit();
	
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) return -1;
	atexit(SDL_Quit);
	if(!SDL_SetVideoMode(1024,512,0,SDL_OPENGL)) return -1;
	
	spec.freq = SAMPLE_RATE;
	spec.format = AUDIO_U16SYS;
	spec.channels = 1;
	spec.samples = AUDIO_BUFFER_SIZE;
	spec.callback = callback;
	spec.userdata = 0;
	if(SDL_OpenAudio(&spec, NULL)) { return -1; }
	SDL_PauseAudio(0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
 	glOrtho(0.0, AUDIO_BUFFER_SIZE-1, -1.05, 1.05, -1.0, 1.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	while(1)
	{
		SDL_Event event;
	
		SDL_Delay(5);
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_LINE_STRIP);

		for(i = 0; i < AUDIO_BUFFER_SIZE; ++i)
		{
			float x = FIXTOFLOAT(audioBuffer[i]);
			glVertex2f(i, x);
		}
		
		glEnd();
		
		SDL_GL_SwapBuffers();
		
		
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT) return 0;
			else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) return 0;
			
			if(event.type == SDL_KEYDOWN)
			{
				if(event.key.keysym.sym >= SDLK_KP0 && event.key.keysym.sym <= SDLK_KP9)
				{
					currentChannel = event.key.keysym.sym - SDLK_KP0;
				} else
				{
					for(i = 0; i < 24; i++)
					{
						if(event.key.keysym.sym == keyboard[i])
						{
							SDL_LockAudio();
							float freq = noteFreq(baseOctave * 12 + i);
							instrumentSetFreq(&instruments[currentChannel], freq, CARRIER);
							instrumentTrigger(&instruments[currentChannel]);
							SDL_UnlockAudio();
							break;
						}
					}
				}
			}
		}
	}
	
	return 0;
}


