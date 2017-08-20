
tone_src_dir := ./src/Tone
script_dir := ./scripts
compile_dir := ./compile

TONE_SRC = $(tone_src_dir)/main.cpp $(tone_src_dir)/Tone.cpp $(tone_src_dir)/Tone.h

$(compile_dir)/play_tone: $(TONE_SRC)
	g++ -o $(compile_dir)/play_tone -pedantic -std=c++11 $(TONE_SRC)
