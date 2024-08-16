#include <iostream>
#include <string>
#include <cstdlib>
#include "server.h"
#include "database.h"
#include "libpq-fe.h"
#include <librdkafka/rdkafka.h>
#include "log.hpp"

Mutex TodoServerApp::todoLock;

CTodoList TodoServerApp::todoList;


ostream& operator<<(ostream& os, CTodo& todo)
{
  
    os << "{ \"_id\": "<< todo.getId() <<  ", \"text\": \"" << todo.getText() << "\" }";
    return os;
}


ostream& operator<<(ostream& os, CTodoList& todoList)
{
   
    map<size_t, CTodo> todos = todoList.readList();

    os << "[";
    if(!todos.empty())
    {
        if(todos.size() == 1)
            os << todos.begin()->second;
        else
            for ( map<size_t, CTodo>::iterator it = todos.begin();;)
            {
                os << it->second ;
                if(++it != todos.end())
                    os << ',';
                else
                    break;
            }

    }
    os << "]\n";

    return os;
}


class CTodoHandler : public HTTPRequestHandler
{
public:
    void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
    {
        
        URI uri(req.getURI());
        string method = req.getMethod();

        std::cout << "URI: " << uri.toString() << endl;
        LOG_TRACE("URI: uri.toString(){}", uri.toString());
        std::cout << "Method: " << req.getMethod() << endl;
        LOG_TRACE("Method: req.getMethod(){}", req.getMethod());

        StringTokenizer tokenizer(uri.getPath(), "/", StringTokenizer::TOK_TRIM);
        HTMLForm form(req,req.stream());

        if(!method.compare("POST"))
        {
            CTodo todo(form.get("text"));
            TodoServerApp::createTodo(todo);
            std::cout << "Create: " << form.get("text") << endl;


        }
        resp.setStatus(HTTPResponse::HTTP_OK);
        resp.setContentType("application/json");
        
        ostream& out = resp.send();
        
    }
};


#include <iostream>     
#include <fstream>      
#include <map>          

class CFileHandler : public HTTPRequestHandler
{
    
    typedef std::map<const std::string, const std::string> TStrStrMap;
    
    
    TStrStrMap CONTENT_TYPE = {
 

  
    };

    string getPath(string& path){
        if(path == "/"){
            path="/index.html";
        }
        path.insert(0, "./www");
        return path;
    }

    string getContentType(string& path){

        string contentType("text/plain");
        Poco::Path p(path);
        TStrStrMap::const_iterator i=CONTENT_TYPE.find(p.getExtension());
        if (i != CONTENT_TYPE.end())
        { 
           contentType = i->second;
        }

        if(contentType.find("text/") != std::string::npos)
        {
            contentType+="; charset=utf-8";
        }

        std::cout << path << " : " << contentType << endl;
        LOG_INFO(" path{} : contentType{} ", path, contentType);
        return contentType;
        
    }

public:

    void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
    {


        URI uri(req.getURI());
        string path(uri.getPath());
        string dd = path.erase(0, 1);
        std::cout << dd << endl;
        LOG_TRACE(" dd{} ", dd);
        ifstream ifs(getPath(path).c_str());

        std::cout << path << endl;
        LOG_TRACE(" path{} ", path);

        Database db;


        std::string bb = """select id from prodmozgm where name = '";
        std::string cc = "';""";

        std::string aaa = bb + dd + cc;
        std::cout << aaa << endl;
        db.ExecuteSQL(aaa);
        LOG_INFO("Zapros v bd otpravlen");
        resp.setStatus(HTTPResponse::HTTP_NOT_FOUND);
        ostream& out = resp.send();
    }
};

class TodoRequestHandlerFactory : public HTTPRequestHandlerFactory
{
    
public:

    virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest & request)
    {
        if (!request.getURI().find("/api/")
                    
            )
            
        return new CTodoHandler;
        
        else
        return new CFileHandler;
            
    }
};

void TodoServerApp::createTodo(CTodo& todo)
{
    ScopedLock<Mutex> lock(todoLock);
    todoList.create(todo);
}

CTodoList& TodoServerApp::readTodoList()
{
    ScopedLock<Mutex> lock(todoLock);
    return todoList;
}

int TodoServerApp::main(const vector<string> &)
{
    HTTPServerParams* pParams = new HTTPServerParams;
    pParams->setMaxQueued(100);
    pParams->setMaxThreads(16);
    HTTPServer s(new TodoRequestHandlerFactory, ServerSocket(8010), pParams);
    s.start();
    std::cout << "Server started" << endl;
    LOG_INFO("Server started");
    waitForTerminationRequest();  
    s.stop();
    return Application::EXIT_OK;
}


