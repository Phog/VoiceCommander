#include "VoiceCommands.h"

#include <fstream>
#include <sstream>
#include <algorithm>

namespace
{

std::string to_lower(const std::string &str) throw()
{
    std::string out;
    out.reserve(str.length());

    for (size_t i = 0; i < str.length(); ++i)
        out.push_back(tolower(str[i]));

    return out;
}

bool is_punctuation(char c) throw()
{ 
    char punctuation[] = { '.', ',', ':', ';', '!', '?' };
    for (size_t i = 0; i < sizeof(punctuation) / sizeof(punctuation[0]); ++i)
        if (punctuation[i] == c)
            return true;
    
    return false;
}

std::string remove_punctuation(std::string str) throw()
{
    str.erase(remove_if(str.begin(), str.end(), is_punctuation), str.end());
    return str;
}

bool is_whitespace(char c) throw()
{ 
    char spaces[] = { ' ', '\t', '\r', '\n', '\"'};
    for (size_t i = 0; i < sizeof(spaces) / sizeof(spaces[0]); ++i)
        if (spaces[i] == c)
            return true;
    
    return false;
}

std::string remove_whitespace(std::string str) throw()
{
    str.erase(remove_if(str.begin(), str.end(), is_whitespace), str.end());
    return str;
}

}

namespace VoiceCommander
{

VoiceCommands::VoiceCommands(const std::string &script_path, 
                             int default_state) throw(std::invalid_argument)
    : m_trigger_index(0), m_num_parsed_words(0), m_active_state(default_state)
{
    std::ifstream script_file(script_path);
    if (!script_file.good())
    {
        std::stringstream error_stream;
        error_stream << "Could not open file: " << script_path;
        throw std::invalid_argument(error_stream.str());
    }
    
    std::string word;
    WordList    triggers;
    bool        parsing_triggers = false;
    int         num_words        = 0;
    while (script_file >> word)
    {
        int  associated_state = default_state;
        bool triggers_end     = false;
        if (!parsing_triggers && word[0] == '[')
        {
            parsing_triggers = true;
            word             = word.substr(1);
        }
        if (parsing_triggers && word[word.size() - 1] == ']')
        {
            triggers_end =  true;
            word         =  word.substr(0, word.size() - 1);
            script_file  >> associated_state;
        }
        
        if (parsing_triggers && word[0] != '{')
            triggers.push_back(to_lower(remove_punctuation(word)));
        
        if (triggers_end)
        {
            parsing_triggers = false;
            m_trigger_locations.push_back(num_words);
            m_trigger_list.push_back(triggers);
            m_state_list.push_back(associated_state);
            triggers.clear();
        }
        
        num_words++;
    }
}

void VoiceCommands::parse_text(const std::string &text) throw()
{
    std::stringstream text_stream(text);
    
    // Skip to the appropriate position
    std::string word;
    for (int i = 0; i < m_num_parsed_words; ++i)
    {
        text_stream >> word;
        if (!text_stream.good())
            return;
    }
    
    int word_location = m_num_parsed_words - 1;  
    while (text_stream.good())
    { 
        text_stream >> word;
        word_location++;

        word = to_lower(remove_punctuation(word));
        bool found_trigger = false;
        for (std::string trigger : m_trigger_list[m_trigger_index])
        {
            if (trigger == word)
            {
                found_trigger = true;
                break;
            }
        }
        
        // If we haven't yet found the trigger word, panic and move to the next state.            
        if (word_location >= m_trigger_locations[m_trigger_index] + MAX_WORD_OFFSET)
            found_trigger = true;

        if (found_trigger)
        {
            m_num_parsed_words = std::min(m_trigger_locations[m_trigger_index] + MAX_WORD_OFFSET,
                                          word_location) + 1;
            
            m_active_state  = m_state_list[m_trigger_index];
            m_trigger_index = std::min(static_cast<int>(m_state_list.size() - 1),
                                       1 + m_trigger_index);
            break;
        }
    }
}




















}
