#include "SQLSpeechToText.h"

#include <sstream>

namespace VoiceCommander
{

SQLSpeechToText::SQLSpeechToText(const std::string &database_name,
                                 const std::string &database_url,
                                 const std::string &database_user,
                                 const std::string &database_password,
                                 ClearDatabase clear) throw(std::invalid_argument)
    : m_database(false), m_last_final_id(0)
{
    mysqlpp::Connection database(false);
    if (!m_database.connect(database_name.c_str(),
                            database_url.c_str(),
                            database_user.c_str(),
                            database_password.c_str()))
    {
        std::stringstream errorstream;
        errorstream << "DB connection failed: " << database.error();
        throw std::invalid_argument(errorstream.str());
    }
    
    if (clear == ClearData)
    {
        m_database.query("delete from transcripts").exec();
        m_database.query("delete from transcripts_live").exec();
    }
}
                
void SQLSpeechToText::fetch_data() throw()
{
    m_transcripts_live.clear();
    m_database.query("lock tables transcripts, transcripts_live read").exec();
    m_database.query("select * from transcripts_live").storein(m_transcripts_live);
    
    std::stringstream query_stream;
    query_stream << "select * from transcripts"
                 << " where id > " << m_last_final_id
                 << " order by id asc";
    m_database.query(query_stream.str().c_str()      ).storein(m_transcripts_final);
    m_database.query("unlock tables").exec();
    
    m_last_final_id = 0;
    for (VoiceCommander::TranscriptFinal t : m_transcripts_final)
        m_last_final_id = std::max(m_last_final_id, t.id);
}

std::string SQLSpeechToText::final_text() const throw()
{
    std::stringstream final_stream;
    for (VoiceCommander::TranscriptFinal t : m_transcripts_final)
        final_stream << t.text;
    return final_stream.str();
}

std::string SQLSpeechToText::live_text() const throw()
{
    std::stringstream live_stream;
    for (VoiceCommander::TranscriptLive t : m_transcripts_live)
        live_stream << t.text;
    return live_stream.str();
}

}
 
