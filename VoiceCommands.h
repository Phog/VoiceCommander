#ifndef VOICE_COMMANDS_H
#define VOICE_COMMANDS_H

#include "SQLSpeechToText.h"

#include <vector>
#include <string>

namespace VoiceCommander
{

class VoiceCommands
{
    static const int MAX_WORD_OFFSET = 3;
    typedef std::vector<std::string> WordList;
public:
    VoiceCommands(const std::string &script_path, int default_state) throw(std::invalid_argument);
    
    void parse_text(const std::string &text) throw();
    
    int active_state() const throw() { return m_active_state; }

private:
    int                   m_trigger_index;
    std::vector<WordList> m_trigger_list;
    
    int                   m_num_parsed_words;
    std::vector<int>      m_trigger_locations;

    int                   m_active_state;
    std::vector<int>      m_state_list;
};

}

#endif


