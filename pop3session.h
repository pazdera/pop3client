

class Pop3Session
{
    Socket* socket;

    public:
        Pop3Session(std::string const& server, int const& port);
        ~Pop3Session();

        void authenticate(std::string const& username, std::string const& password);

        void printMessageList();
        void printMessage(int messageId);


    private:
        struct ServerResponse;

        void open();
        void close();

        
};

struct Pop3Session::ServerResponse
{
    bool status;
    std::string statusMessage;
    std::list<std::string> data;
};
