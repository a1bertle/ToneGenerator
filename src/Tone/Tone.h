#ifndef TONE_H
#define TONE_H

class Tone {
    private:
        const double _sampleRate;
        double _s; /* current position along the unit sine curve */
        double _lastSample;
    public:
        double getSample() const;
        double nextSample(double frequency, double amplitude);
        void reset();
        Tone(double sampleRate); // constructor
};

#endif
