#include <iostream>
#include <thread>
#include <chrono>

#include "ThreadedCommander.h"
#include "VoiceCommands.h"
#include "SQLSpeechToText.h"


int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: voicetrigger [DB_USERNAME] [DB_PASSWORD]" << std::endl;
        return 0;
    }

    std::string actors[] = { std::string("John Cleese"), std::string("Michael Palin") };
    try
    {
        VoiceCommander::SQLSpeechToText   speech_to_text("speechdb", "localhost", argv[1], argv[2],
                                                         VoiceCommander::SQLSpeechToText::ClearData);
        VoiceCommander::VoiceCommands     voice_commands("../scripts/parrotsketch.txt", 0);
        VoiceCommander::ThreadedCommander commander(&voice_commands, &speech_to_text, 5);

        commander.start_listening();
        while (true)
        {
            int state = commander.active_state();
            if (state == 2)
                break;
        
            std::cout << "\r" << actors[state] << "                 " << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        commander.stop_listening();
    }
    catch (const std::exception &e)
    {
        // Unexpected exception. Bail.
        std::cerr << e.what() << std::endl;
        return 1;
    }


    return 0;
}
