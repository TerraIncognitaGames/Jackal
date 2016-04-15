#ifdef _WIN32
  #include <winsock2.h>
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <netdb.h>
#endif

#ifdef _MSC_VER
  #include <io.h>
#else
  #include <unistd.h>
#endif

#include <memory>
#include <string>
#include <exception>


class TSocket {
    private:
        class TSocketHolder {
            private:
                int Socket;

                TSocketHolder(const TSocketHolder &) = delete;
                TSocketHolder(TSocketHolder &&) = delete;
                TSocketHolder &operator = (const TSocketHolder &) = delete;
                TSocketHolder &operator = (TSocketHolder &&) = delete;

            public:
                TSocketHolder()
                    : Socket(socket(AF_INET, SOCK_STREAM, 0))
                {
                    if (Socket < 0)
                        throw std::runtime_error("could not create socket");
                }
                TSocketHolder(int socket)
                    : Socket(socket)
                {
                }
                ~TSocketHolder() {
                    close(Socket);
                }
                int GetSocket() const {
                    return Socket;
                }
        };
        using TSocketPtr = std::shared_ptr<TSocketHolder>;
        TSocketPtr Ptr;

        bool ResolveHost(const std::string &host, int &addr) const {
            hostent *ent = gethostbyname(host.c_str());
            if (ent == nullptr)
                return false;
            for (size_t i = 0; ent->h_addr_list[i]; ++i) {
                addr = *reinterpret_cast<int**>(ent->h_addr_list)[i];
                return true;
            }
            return false;
        }

    public:
        TSocket()
            : Ptr(new TSocketHolder)
        {
        }
        TSocket(int sock)
            : Ptr(new TSocketHolder(sock))
        {
        }
        void Connect(const std::string &host, int port) {
            int addr;
            if (!ResolveHost(host, addr))
                throw std::runtime_error("can't resolve host");
            sockaddr_in address;
            address.sin_family = AF_INET;
            address.sin_port = htons(port);
            address.sin_addr.s_addr = addr;
            if (connect(Ptr->GetSocket(), reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0)
                throw std::runtime_error("can't connect");
        }
        void Bind(int port, const std::string &host) {
            sockaddr_in address;
            address.sin_family = AF_INET;
            address.sin_port = htons(port);
            if (!host.empty()) {
                int addr;
                if (!ResolveHost(host, addr))
                    throw std::runtime_error("can't resolve host");
                address.sin_addr.s_addr = addr;
            } else {
                address.sin_addr.s_addr = INADDR_ANY;
            }
            if (bind(Ptr->GetSocket(), reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0)
                throw std::runtime_error("can't bind");
            if (listen(Ptr->GetSocket(), 1) < 0)
                throw std::runtime_error("can't start listening");
        }
        template<typename TAcceptHandler>
        void AcceptLoop(TAcceptHandler &handler) const {
            for (; ;) {
                int sock = accept(Ptr->GetSocket(), nullptr, nullptr);
                if (sock < 0)
                    throw std::runtime_error("can't bind");
                TSocket res(sock);
                if (handler.HandleAcceptedSocket(res))
                    return;
            }
        }
        bool Send(const char *data, size_t sz) const {
            for (; sz > 0; ) {
                int res = send(Ptr->GetSocket(), data, sz, 0);
                if (res == 0)
                    return false;
                if (res < 0)
                    throw std::runtime_error("error in send");
                data += res;
                sz -= res;
            }
            return true;
        }
        // returns true if connection was closed by handler, false if connection was closed by peer
        template<typename TIOHandler>
        bool RecvLoop(TIOHandler &handler) const {
            for (; ; ) {
                char buf[1024];
                int res = recv(Ptr->GetSocket(), buf, sizeof(buf), 0);
                if (res == 0)
                    return false;
                if (res < 0)
                    throw std::runtime_error("error in recv");
                if (handler.ProcessReceivedData(buf, res))
                    return true;
            }
        }
};
