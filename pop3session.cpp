
Pop3Session::Pop3Session()
    : socket(NULL)
{}

Pop3Session::Pop3Session(std::string const& server, int const& port)
{
    socket = new Socket(server, port);
}

Pop3Session::~Pop3Session()
{
    releaseSocket();
}

Pop3Session::releaseSocket()
{
    if (socket != NULL)
    {
        delete socket;
    }
}


