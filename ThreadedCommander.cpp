#include "ThreadedCommander.h"

namespace VoiceCommander
{

ThreadedCommander::ThreadedCommander(VoiceCommands   *voice_commands,
                                     SQLSpeechToText *speech_to_text,
                                     float update_rate_hz) throw()
    : m_sleep_time_ms(std::max(33, static_cast<int>(1.0f / std::max(1.0f, update_rate_hz)))),
      m_voice_commands(voice_commands),
      m_speech_to_text(speech_to_text),
      m_active_state(voice_commands->active_state()),
      m_running(false)
{
}

void ThreadedCommander::start_listening()
{
    m_running = true;
    m_thread  = std::thread(&ThreadedCommander::thread_loop, this);
}

void ThreadedCommander::stop_listening()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_running = false;
    }
    m_thread.join();
}

void ThreadedCommander::thread_loop() throw()
{
    for (;;)
    {
        m_speech_to_text->fetch_data();  
        m_voice_commands->parse_text(m_speech_to_text->total_text());
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_active_state = m_voice_commands->active_state();
            if (!m_running)
                return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(m_sleep_time_ms));
    }
}

}
