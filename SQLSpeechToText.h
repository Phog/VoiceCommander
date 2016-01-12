#ifndef SQL_SPEECH_TO_TEXT_H
#define SQL_SPEECH_TO_TEXT_H

#include "Database.h"

#include <vector>
#include <string>
#include <stdexcept>

namespace VoiceCommander
{

class SQLSpeechToText
{
public:
    enum ClearDatabase { ClearData, PreserveData };
    SQLSpeechToText(const std::string &database_name,
                    const std::string &database_url,
                    const std::string &database_user,
                    const std::string &database_password,
                    ClearDatabase clear) throw(std::invalid_argument);
                    
    void        fetch_data()       throw();
    std::string final_text() const throw();
    std::string live_text()  const throw();
    std::string total_text() const throw() { return final_text() + live_text(); }
private:
    mysqlpp::Connection          m_database;
    std::vector<TranscriptFinal> m_transcripts_final;
    std::vector<TranscriptLive>  m_transcripts_live;
    int                          m_last_final_id;
};

}

#endif
