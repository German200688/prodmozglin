#include <iostream>
#include <cstdlib>
#include "libpq-fe.h"
#include "database.h"
#include "producer.cpp"
#include "log.hpp"


char dbcount;



static void
exit_nicely(PGconn* m_connection)
{
    PQfinish(m_connection);
    exit(632);
}





bool Database::ExecuteSQL(const std::string& sql)
{
    
    if (PQstatus(m_connection.get()) != CONNECTION_OK)
    {
        LOG_FATAL("Connection to database failed");

        return false;
    }
    else
    {
        LOG_INFO("Connection to database has been established!!!!");
    }

    auto result = PQexec(m_connection.get(), sql.c_str());
  //  std::cout << PQresultStatus(result), '\n';
  if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        LOG_FATAL("PGRES_COMMAND_OK = NOT OK");
        return false;
    }





 int nFields1 = PQntuples(result);
 const int nFields = nFields1-1;
 
   
  
  char* h;  
 char* MotorInst3[]{0};
 LOG_INFO("Massiv sformirovan");
        for (int j = 0; j < nFields1; j++)
        {
       MotorInst3[j] = {PQgetvalue(result, j, 0)};
 //     LOG_TRACE("date to producer MotorInst3[j]{} count nFields1{}", MotorInst3[j], nFields1);
      h = MotorInst3[j];
      if (h == NULL) {h = "1";}
            director(h);
   //         LOG_TRACE("h = h{}", h);


     } 

  PQclear(result);     

    return true;
}
 
Database::Database()
{

    const char* m_dbhost = "172.19.0.2";
    // const char* m_dbhost = "localhost";
    const char* m_dbport = "5432";
    const char* m_dbname = "prodmozg";
    const char* m_dbuser = "prodmozg";
    const char* m_dbpass = "asdf";
    m_connection.reset(
        PQsetdbLogin(m_dbhost, m_dbport, nullptr, nullptr, m_dbname, m_dbuser, m_dbpass), &PQfinish);

    LOG_TRACE("Connection to database host: m_dbhost{}", m_dbhost);
    LOG_TRACE("Connection to database port: m_dbport{}", m_dbport);
    LOG_TRACE("Connection to database dbname: m_dbname{}", m_dbname);


    if (PQstatus(m_connection.get()) != CONNECTION_OK && PQsetnonblocking(m_connection.get(), 1) != 0)
    {
        LOG_FATAL("Connection to database failed");
 ///       exit_nicely(m_connection);

    }
else
    {
        LOG_INFO("Connection to database OK");
    }
}
 


std::shared_ptr<PGconn> Database::GetConnection() const
{
    return m_connection;
}
