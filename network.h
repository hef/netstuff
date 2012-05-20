#include <vector>
class network
{
    public:
        network();
        void send(const char* message);
        void recv();
        void addTarget(const char* target);
        ~network();
    private:
            int sockfd;
           std::vector<struct sockaddr*> others;
};





