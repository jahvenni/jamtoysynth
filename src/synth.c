#include <string.h>
#include <math.h>

#include <synth.h>

static float note_freq(int i)
{
    const float A = 440.0;
    return (A / 32.0) * pow(2.0, (i - 9) / 12.0);
}

int synth_init(synth_t *synth, int sample_rate)
{
    synth->sample_rate = sample_rate;
    memset(synth->instruments, 0, SYNTH_MAX_INSTRUMENTS * sizeof(instrument_t));

    synth->instruments[0].freq = note_freq(60);
    synth->instruments[0].amplitude = 0.3;

    synth->instruments[1].freq = note_freq(64);
    synth->instruments[1].amplitude = 0.3;

    synth->instruments[2].freq = note_freq(67);
    synth->instruments[2].amplitude = 0.3;

    return 0;
}

void synth_cleanup(synth_t *synth)
{
    (void)synth;
}

void synth_mix(synth_t *synth, float *left, float *right)
{
    *left = 0; *right = 0;

    for(instrument_t *instrument = synth->instruments;
        instrument != synth->instruments + SYNTH_MAX_INSTRUMENTS;
        ++instrument)
    {
        float l, r;
        instrument_play(instrument, synth->sample_rate, &l, &r);
        *left += l; *right += r;
    }
}
