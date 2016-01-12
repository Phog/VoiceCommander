#ifndef DATABASE_H
#define DATABASE_H

#if !defined(EXPAND_MY_SSQLS_STATICS)
#   define MYSQLPP_SSQLS_NO_STATICS
#endif

#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>

namespace Database
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
sql_create_2(transcripts, 1, 2, 
             mysqlpp::sql_int, id,
             mysqlpp::sql_varchar_null, text)

sql_create_2(transcripts_live, 1, 2, 
             mysqlpp::sql_int, id,
             mysqlpp::sql_varchar_null, text)
#pragma GCC diagnostic pop
}

namespace VoiceCommander
{
    typedef Database::transcripts      TranscriptFinal;
    typedef Database::transcripts_live TranscriptLive;
}

#endif
