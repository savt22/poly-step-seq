
# MIDI Polyphonic Step Sequencer

This is an Arduino Nano-based, MIDI, quantised, polyphonic step sequencer. 

Features: 

You can select notes from a scale per step. 

    Root note
    Octave (-2, -1, 0, +1, +2)
    Mode
        Major
        Lydian
        Mixolydian
        Minor
        Dorian
        Phyrigian
        Chromatic

You can choose polyphonic or monophonic operation.

When in polyphonic mode, 4-voice (triad + octave down) chords play from the selected scale, with pitch selecting the root note of each chord for each step.

In polyphonic mode, you can add a 5th note to the chords that play, again locked to the scale/mode, choosing from:

    2nd
    4th
    6th
    7th
    2nd (+1 octave)
    4th (+1 octave)
    6th (+1 octave)
    7th (+1 octave)

You can select the length of each step, quantised to BPM, from: 
    
    1/8 note
    1/4 note
    1/2 note
    1 note
    2 notes
    4 notes
    8 notes
    16 notes

You can choose two playback modes, forward or ping-pong. In ping-pong, the sequence will play forward and then in reverse, with the 1st and 8th notes playing twice at each end. 

Notes/chords play on MIDI channel 1 at full velocity, though this can easily be adjusted in the code. 

An additional bass note, an octave down, will play on MIDI channel 2 at full velocity too, giving you the option of having a bass sound on a separate synth. 

I'm working on MIDI clock and 2PPQN sync for things like Pocket Operators.


https://www.youtube.com/watch?v=iVq9uU2GkV4
