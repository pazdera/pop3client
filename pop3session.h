

class Pop3Session
{
    Socket* socket;

    public:
        Pop3Session();
        ~Pop3Session();
        
    private:
        void open();
        void close();

        void authenticate();
        
};
