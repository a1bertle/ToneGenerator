#include <cmath>
#include "Tone.h"

Tone::Tone(double sampleRate) : _sampleRate(sampleRate), _s(0), _lastSample(0) {
}

double Tone::getSample() const
{
    return _lastSample;
}

double Tone::nextSample(double frequency, double amplitude)
{
    _s += frequency / _sampleRate;
    _lastSample = amplitude * sin(2*M_PI * _s);
    return _lastSample;
}

void Tone::reset() {
	_s = 0;
}
