CXX := g++
LDD := g++

FLAGS_RELEASE := -Wall -Wextra -O2 -std=c++11
FLAGS_DEBUG   := -Wall -Wextra -g -std=c++11
INCLUDES      := -I/usr/include/mysql -I/usr/include/mysql++
LDD_FLAGS     := -lmysqlpp -lmysqlclient

TARGET_RELEASE := release/voicetrigger
TARGET_DEBUG   := debug/voicetrigger

HEADERS         := Database.h SQLSpeechToText.h VoiceCommands.h ThreadedCommander.h
SRC             := Database.cpp SQLSpeechToText.cpp VoiceCommands.cpp ThreadedCommander.cpp
VOICETRIGGER     := main.cpp
OBJ              := $(SRC:.cpp=.o)
VOICETRIGGER_OBJ := $(VOICETRIGGER:.cpp=.o)

release: $(TARGET_RELEASE)
$(TARGET_RELEASE): $(SRC) $(HEADERS) $(VOICETRIGGER)
	$(CXX) $(INCLUDES) $(FLAGS_RELEASE) -c $(SRC) $(VOICETRIGGER)
	$(LDD) $(OBJ) $(VOICETRIGGER_OBJ) -o $(TARGET_RELEASE) $(LDD_FLAGS)

debug: $(TARGET_DEBUG)
$(TARGET_DEBUG): $(SRC) $(HEADERS) $(VOICETRIGGER)
	$(CXX) $(INCLUDES) $(FLAGS_DEBUG) -c $(SRC) $(VOICETRIGGER)
	$(LDD) $(OBJ) $(VOICETRIGGER_OBJ) -o $(TARGET_DEBUG) $(LDD_FLAGS)

clean:
	rm -f $(OBJ) $(VOICETRIGGER_OBJ) \
	$(TARGET_RELEASE) $(TARGET_DEBUG) $(TARGET_TESTS)
