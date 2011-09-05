

class Pop3Session
{
    Socket* socket;

    public:
        Pop3Session(std::string const& server, int const& port);
        ~Pop3Session();

        void authenticate(std::string const& username, std::string const& password);

        void printMessageList();
        void printMessage(int const& messageId);

        /* Exceptions */
        class ServerError;

    private:
        struct ServerResponse;

        void sendCommand(std::string const& command);
        void getResponse(ServerResponse* response);
        void gerMultilineResponse(ServerResponse* response);

        void releaseSocket();
};

struct Pop3Session::ServerResponse
{
    bool status;
    std::string statusMessage;
    std::list<std::string> data;
};

class ServerError : public Error
{
    public:
        ServerError(std::string what, std::string serverStatus)
        {
            problem = what;
            reason = serverStatus;
        }
};

