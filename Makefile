CXX=g++
PROFILE_DIR=profile
FLAGS_BASE=-Wall -std=c++11 -Wl,--no-as-needed -pthread
FLAGS_PROFILE=-flto -O3
FLAGS_GEN_PROFILE=-fprofile-generate=$(PROFILE_DIR)
FLAGS_USE_PROFILE=-fprofile-use=$(PROFILE_DIR)
FLAGS_DEBUG=-g -fno-inline
FLAGS_OTHER=
EXECUTABLE=scheduler
SOURCES= \
	src/main.cpp \
	src/safe_file_handle.cpp \
	src/process_input.cpp \
	src/input_parser.cpp \
	src/process.cpp \
	src/scheduler.cpp \
	src/arrival_queue.cpp \
	src/boost_queue.cpp \
	src/run_queue.cpp \
	src/running.cpp \
	src/output_event.cpp \
	src/output_process.cpp
PROFILES=$(addprefix $(PROFILE_DIR)/,$(notdir $(SOURCES:.cpp=.gcda)))

all:
	$(CXX) $(FLAGS_BASE) $(FLAGS_PROFILE) $(FLAGS_OTHER) $(FLAGS_GEN_PROFILE) $(SOURCES) -o $(EXECUTABLE)
	./scheduler -q 10 -i $(PROFILE_DIR)/profile100000 -n
	$(CXX) $(FLAGS_BASE) $(FLAGS_PROFILE) $(FLAGS_OTHER) $(FLAGS_USE_PROFILE) $(SOURCES) -o $(EXECUTABLE)
	rm -f $(PROFILES)

googleperf:
	make FLAGS_OTHER=-lprofiler

debug:
	$(CXX) $(FLAGS_BASE) $(FLAGS_DEBUG) $(FLAGS_OTHER) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm -f $(PROFILES) $(EXECUTABLE)
