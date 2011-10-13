#ifndef SYNTH_H
#define SYNTH_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 44100
#endif

#ifndef AUDIO_BUFFER_SIZE
#define AUDIO_BUFFER_SIZE 1024
#endif

#ifndef NUM_INSTRUMENTS
#define NUM_INSTRUMENTS 16
#endif

typedef signed int fixed;

#define NO_MODULATION 0
#define FM_MODULATION 1
#define RING_MODULATION 2
#define AM_MODULATION 3

typedef struct FilterData
{
	fixed x2, x1, y2, y1;
} __attribute__((packed)) FilterData;


typedef struct Instrument
{
	fixed *sample1;
	fixed phase1, freq1;
	fixed amplitude1;
	
	fixed *sample2;
	fixed phase2, freq2;
	fixed amplitude2;
	
	fixed sweep;
	unsigned char modulation;
	
	fixed b0, b2, b1, a2, a1, a0;
	FilterData filter1, filter2;
	
	fixed attackG, releaseG, decayG;
	fixed sustain, decay;
	fixed adsrG, adsrX, decayTimer;
	
	fixed level;

	fixed volume, echo;
} __attribute__((packed)) Instrument;

extern fixed SINE_WAVE[65536];
extern fixed SQUARE_WAVE[65536];
extern fixed NOISE_WAVE[65536];

extern fixed audioBuffer[AUDIO_BUFFER_SIZE];
extern unsigned int echoReadIndex, echoWriteIndex;
extern fixed echoFeedback;

extern Instrument instruments[NUM_INSTRUMENTS];

void synthInit(void) __attribute__((cdecl));
void mixer(void) __attribute__((cdecl));

#define FLOATTOFIX(x) ((fixed)((x) * 65536.0 + ((x)>=0?0.5:-0.5)))
#define FIXTOFLOAT(x) ((float)(x) / 65536.0f)
#define FIXMUL(a, b) ((fixed) (((int64_t)(a) * (int64_t)(b)) >> 16))
#define FIXDIV(a, b) ( ((int64_t)(a) << 16) / (b))

#ifdef __cplusplus
}
#endif

#endif
