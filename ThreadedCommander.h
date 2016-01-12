#ifndef THREADED_COMMANDER_H
#define THREADED_COMMANDER_H

#include "VoiceCommands.h"
#include "SQLSpeechToText.h"

#include <mutex>
#include <thread>

namespace VoiceCommander
{

class ThreadedCommander
{
public:
    ThreadedCommander(VoiceCommands   *voice_commands,
                      SQLSpeechToText *speech_to_text,
                      float update_rate_hz) throw();

    int active_state() throw() { std::lock_guard<std::mutex>lock(m_mutex); return m_active_state; }
                                 
    void start_listening();
    void stop_listening();

private:
    void thread_loop() throw();
        
    size_t           m_sleep_time_ms;
    VoiceCommands   *m_voice_commands;
    SQLSpeechToText *m_speech_to_text;
    int              m_active_state;
    
    bool             m_running;
    std::mutex       m_mutex;
    std::thread      m_thread;
};

}

#endif
