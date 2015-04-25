// Copyright 2014 Yu Jing <yu@argcv.com>
#ifndef ARGCV_NET_CO_LACUS_HH
#define ARGCV_NET_CO_LACUS_HH

// for network io

#ifdef __linux__
#define __EPOLL__ 1
#elif defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
#define __KQUEUE__ 1
#else  // SELECT
#error "NEITHOR OF EPOLL AND KQUEUE ARE AVAILABLE"
#endif

#ifdef __EPOLL__
#include <sys/epoll.h>
#elif __KQUEUE__
#include <sys/event.h>
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include <fcntl.h>  //fctl open

#include <string>

namespace argcv {
namespace net {

#define LISTENSOCKET (void *)((intptr_t)~0)
#define LISTENQ 32

enum class SOCK_STATUS : unsigned char {
    SOCK_INVALID,
    SOCK_CLOSED,
    SOCK_SUSPEND,
    // SOCK_READ,
    SOCK_POLLIN,
    SOCK_ALIVE = SOCK_SUSPEND
};

class co_lacus {
public:
    class conn {
    public:
        conn() : status(SOCK_STATUS::SOCK_INVALID) {}
        ~conn() {
            printf("~conn %d %d\n", idx, fd);
            if (status >= SOCK_STATUS::SOCK_ALIVE) {
                close(fd);
            }
        }

        bool assign(co_lacus *m_lacus, int m_idx, int m_fd) {
            _lacus = m_lacus;
            idx = m_idx;
            fd = m_fd;
            // printf("init %d\n",_lacus->_port());
            return true;
        }

        int _fd() { return fd; };
        void _fd(int m_fd) { fd = m_fd; };

        int _idx() { return idx; }

        SOCK_STATUS _status() { return status; }
        void _status(SOCK_STATUS m_status) { status = m_status; }

        void reset(int m_fd, SOCK_STATUS m_status) {
            data.clear();
            fd = m_fd;
            status = m_status;
        }

        void push(const std::string &d) { data += d; }

        void flush() {
            data.clear();
            if (closed()) {
                _lacus->closed_size_dec();
                status = SOCK_STATUS::SOCK_INVALID;
                fd = _lacus->update_free_co_clue(idx);
            }
        }

        bool closed() { return SOCK_STATUS::SOCK_CLOSED == status && data.empty(); }
        bool empty() { return data.empty(); }

        const std::string &to_str() const { return data; }
        int write(const std::string &data, size_t sz) {
            // return send(fd,data.c_str(),sz,MSG_DONTWAIT);
            for (;;) {
                int n_bytes = send(fd, data.c_str(), sz, MSG_DONTWAIT);
                if (0 == n_bytes) {
                    deactive();
                } else if (-1 == n_bytes) {
                    switch (errno) {  // #include <error.h>
                        case EWOULDBLOCK:
                            status = SOCK_STATUS::SOCK_SUSPEND;
                            break;
                        case EINTR:
                            continue;
                        default:
                            deactive();
                            break;
                    }
                }
                return n_bytes;
            }
        }

        bool pull(int sz = 2048) {
            switch (status) {
                // case SOCK_STATUS::SOCK_READ:
                //    _c->_status(SOCK_STATUS::SOCK_SUSPEND);
                case SOCK_STATUS::SOCK_CLOSED:
                case SOCK_STATUS::SOCK_SUSPEND:
                    return false;
                default:
                    if (SOCK_STATUS::SOCK_POLLIN != status) {
                        fprintf(stderr, "ERRPR STATUS: %hhu\n", status);
                    }
            }
            for (;;) {
                char buffer[sz + 1];
                int n_bytes = recv(fd, &buffer, sz, MSG_DONTWAIT);
                if (0 < n_bytes) {
                    //_c->_status(SOCK_STATUS::SOCK_READ);
                    push(std::string(buffer, n_bytes));
                    break;
                }
                if (0 == n_bytes) {
                    deactive();
                    return false;
                }
                if (-1 == n_bytes) {
                    switch (errno) {  // #include <error.h>
                        case EWOULDBLOCK:
                            status = SOCK_STATUS::SOCK_SUSPEND;
                            break;
                        case EINTR:
                            continue;
                        default:
                            deactive();
                            break;
                    }
                    return false;
                }
            }
            return true;
        }

    private:
        // conn(const conn &);  // Prevent copy-construction
        // conn & operator=(const conn &);  // Prevent assignment
        co_lacus *_lacus;
        int idx;
        int fd;
        std::string data;
        SOCK_STATUS status;

        void deactive() {
            status = SOCK_STATUS::SOCK_CLOSED;
            data.clear();
            close(fd);
            printf("[conn::deactive] close %d (fd=%d)\n", idx, fd);
#ifdef __EPOLL__
            epoll_ctl(_lacus->_ev_fd(), EPOLL_CTL_DEL, fd, NULL);
#elif __KQUEUE__
            struct kevent ke;
            EV_SET(&ke, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
            kevent(_lacus->_ev_fd(), &ke, 1, NULL, 0, NULL);
#endif  //__KQUEUE__
            _lacus->closed_size_inc();
        }
    };

    co_lacus(int port, size_t max_conn_size = 1024, size_t max_buff_size = 1024 * 1024 * 1024)
        : port(port), max_conn_size(max_conn_size), co(new conn[max_conn_size]), error_no(0) {
        printf("starting co_lacus.. %d %lu\n", port, max_conn_size);
        if (!init()) {
            fprintf(stderr, "co_lacus start failed\n");
            error_no = -1;  // start failed
        }
    }

    ~co_lacus() {
        delete[] co;
        if (listen_fd >= 0) {
            close(listen_fd);
        }
        if (ev_fd >= 0) {
            close(ev_fd);
        }
        printf("co_lacus closed \n");
    }

    const int _port() const { return port; }
    const size_t _max_conn_size() const { return max_conn_size; }
    const int _error_no() const { return error_no; }

    conn &get(size_t idx) { return co[idx]; }
    conn &operator[](size_t idx) { return co[idx]; }

    int conn_id(size_t idx) { return co[idx]._fd(); }

    int &_ev_fd() { return ev_fd; }

    int poll(int timeout) {  // get a new id
        if (sz_closed > 0) {
            int closed_fd = _report_closed();
            if (-1 != closed_fd) {
                return closed_fd;
            }
        }

        if (evq_head >= evq_len) {
            if (-1 == _evq_poll(timeout)) {
                return -1;
            }
        }

        for (;;) {
            void *_reply = _evq_read_one();
            if (nullptr == _reply) {
                // printf("_evq_read_one :: null ...\n");
                return -1;
            }
            if (LISTENSOCKET == _reply) {
                struct sockaddr_in remote_addr;
                socklen_t len = sizeof(struct sockaddr_in);
                int client_fd = accept(listen_fd, (struct sockaddr *)&remote_addr, &len);
                if (client_fd >= 0) {
                    printf("co_lacus::poll connect %s:%u (fd=%d)\n", inet_ntoa(remote_addr.sin_addr),
                           ntohs(remote_addr.sin_port), client_fd);
                    active(client_fd);
                }
            } else {
                conn *_c = (conn *)_reply;
                int idx = _c->_idx();
                _c->_status(SOCK_STATUS::SOCK_POLLIN);
                return idx;
            }
        }
    }

    bool pull(size_t idx, int sz = 2048) {
        if (idx >= max_conn_size) {
            return false;
        }
        return co[idx].pull(sz);  // TODO
    }

    int write(size_t idx, const std::string &data, size_t sz) {
        if (idx >= max_conn_size) {
            return -1;
        }
        return co[idx].write(data, sz);
    }

private:
    const int port;
    const size_t max_conn_size;  // MAX connection Size
    conn *co;

    int error_no;  // -1 : init failed

    int listen_fd;

    int _free_co_clue;
    // std::mutex _free_co_clue_mutex;

    size_t evq_head;
    size_t evq_len;

    size_t sz_closed;
    // std::mutex closed_mutex;

    int ev_fd;
#ifdef __EPOLL__
    struct epoll_event ev[LISTENQ];
#elif __KQUEUE__
    struct kevent ev[LISTENQ];
#endif
    bool init() {
        listen_fd = socket(AF_INET, SOCK_STREAM, 0);

        if (-1 == listen_fd) {
            return false;
        }

        // set nonblocking
        int no_blocking_flag = fcntl(listen_fd, F_GETFL, 0);
        if (-1 == no_blocking_flag) {
            return false;
        }
        if (-1 == fcntl(listen_fd, F_SETFL, no_blocking_flag | O_NONBLOCK)) {
            return false;
        }

        int reuse = 1;
        setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

        struct sockaddr_in self_addr;
        memset(&self_addr, 0, sizeof(struct sockaddr_in));
        self_addr.sin_family = AF_INET;
        self_addr.sin_port = htons(port);
        self_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // INADDR_LOOPBACK

        // char * b_addr = inet_ntoa(self_addr.sin_addr);
        // uint16_t b_port = ntohs(self_addr.sin_port);
        // printf("co_lacus bind to %s:%u\n",b_addr,b_port);

        if (-1 == bind(listen_fd, (struct sockaddr *)&self_addr, sizeof(struct sockaddr))) {
            close(listen_fd);
            return false;
        }
        if (-1 == listen(listen_fd, LISTENQ)) {
            close(listen_fd);
            return false;
        }
#ifdef __EPOLL__
        ev_fd = epoll_create(max_conn_size + 1);
        if (-1 == ev_fd) {
            close(listen_fd);
            return false;
        }

        struct epoll_event ee;
        ee.events = EPOLLIN;
        ee.data.ptr = LISTENSOCKET;

        if (-1 == epoll_ctl(ev_fd, EPOLL_CTL_ADD, listen_fd, &ee)) {
            close(listen_fd);
            close(ev_fd);
            return false;
        }
#elif __KQUEUE__
        ev_fd = kqueue();
        if (-1 == ev_fd) {
            close(listen_fd);
            return false;
        }

        struct kevent ke;
        EV_SET(&ke, listen_fd, EVFILT_READ, EV_ADD, 0, 0, LISTENSOCKET);
        if (-1 == kevent(ev_fd, &ke, 1, NULL, 0, NULL)) {
            close(listen_fd);
            close(ev_fd);
            return false;
        }
#endif  // __KQUEUE__
        for (size_t i = 0; i < max_conn_size - 1; i++) {
            co[i].assign(this, i, i + 1);
        }
        co[max_conn_size - 1].assign(this, max_conn_size - 1, -1);
        _free_co_clue = 0;
        evq_head = 0;
        evq_len = 0;
        return true;
    }

    int _evq_poll(int timeout) {
#ifdef __EPOLL__
        int n = epoll_wait(ev_fd, ev, LISTENQ, timeout);
#elif __KQUEUE__
        struct timespec timeoutspec;
        timeoutspec.tv_sec = timeout / 1000;
        timeoutspec.tv_nsec = (timeout % 1000) * 1000000;
        int n = kevent(ev_fd, NULL, 0, ev, LISTENQ, &timeoutspec);
#endif  // __KQUEUE__

        evq_head = 0;
        if (n == -1) {
            evq_len = 0;
            return -1;
        } else {
            evq_len = n;
            return n;
        }
    }
    void *_evq_read_one() {
        if (evq_head >= evq_len) {
            return nullptr;
        }
#ifdef __EPOLL__
        return ev[evq_head++].data.ptr;
// int n = epoll_wait(ev_fd , ev, LISTENQ, timeout);
#elif __KQUEUE__
        return ev[evq_head++].udata;
#endif  // __KQUEUE__
    }

    int _co_alloc() {
        // std::lock_guard<std::mutex> l(_free_co_clue_mutex);
        if (-1 == _free_co_clue) {
            return -1;
        }
        int s = _free_co_clue;
        _free_co_clue = co[s]._fd();
        // printf("_co_alloc::%d\n",s);
        return s;
    }

    void active(int fd) {
        int co_offset = _co_alloc();
        if (-1 == co_offset) {
            fprintf(stderr,
                    "co_lacus::active for FD: %d **FAILED** , because assign "
                    "connection failed \n",
                    fd);
            return;
        }
#ifdef __EPOLL__
        struct epoll_event ee;
        ee.events = EPOLLIN;
        ee.data.ptr = &(co[co_offset]);
        if (-1 == epoll_ctl(ev_fd, EPOLL_CTL_ADD, fd, &ee)) {
            close(fd);
            fprintf(stderr,
                    "co_lacus::active for FD: %d **FAILED** , establish "
                    "epoll failed \n",
                    fd);
            return;
        }
#elif __KQUEUE__
        struct kevent ke;
        EV_SET(&ke, fd, EVFILT_READ, EV_ADD, 0, 0, &(co[co_offset]));
        if (-1 == kevent(ev_fd, &ke, 1, NULL, 0, NULL)) {
            close(fd);
            fprintf(stderr,
                    "co_lacus::active for FD: %d **FAILED** , establish "
                    "kevent failed \n",
                    fd);
            return;
        }
#endif  // __KQUEUE__
        co[co_offset].reset(fd, SOCK_STATUS::SOCK_SUSPEND);
        fprintf(stderr, "co_lacus::active for FD: %d , new connection\n", fd);
    }

    int _report_closed() {
        for (size_t i = 0; i < max_conn_size; i++) {
            if (SOCK_STATUS::SOCK_CLOSED == co[i]._status()) {
                return (int)i;
            }
        }
        return -1;
    }

    int update_free_co_clue(int offset) {
        int rt_offset = _free_co_clue;
        _free_co_clue = offset;
        return rt_offset;
    }

    void closed_size_inc() {  // increase
        // std::lock_guard<std::mutex> l(closed_mutex);
        sz_closed++;
    }

    void closed_size_dec() {  // decrease
        // std::lock_guard<std::mutex> l(closed_mutex);
        sz_closed--;
    }
};

typedef co_lacus::conn conn;
}
}  // namespace argcv::ml

/*
sample 1 , echo :
using namespace argcv::net;
void echo_server() {
    co_lacus pool(9527, 200000);
    if (pool._error_no() != 0) {
        printf("pool establish failed .. %d \n", pool._error_no());
    } else {
        printf("pool established .. %d \n", pool._error_no());

        for (;;) {
            int id = pool.poll(0);
            if (id != -1) {
                printf("#### id: %d\n", id);
                co_lacus::conn &c = pool[id];
                bool st = pool.pull(id, 1);
                if (st) {
                    // printf("data:[%s] %lu \n",c.to_str().c_str(), c.to_str().length());
                    std::string s = c.to_str();
                    for (size_t i = 0; i < c.to_str().length(); i++) {
                        printf("%lu %d %c\n", i, c.to_str()[i], c.to_str()[i]);
                    }
                    // sleep(3);
                    c.write(c.to_str(), c.to_str().length());
                } else {
                    if (c.closed()) {
                        printf("is closed .. \n");
                    } else {
                        printf("unknown error ? \n status : %hhu\n", c._status());
                    }
                }
                c.flush();
            }
        }
    }
}

sample 2 , file_recv :
void file_server() {
    // echo_server();
    co_lacus pool(9527, 1);
    printf("send a file to server:\n\t$ cat xxx | nc localhost 9527\n");
    if (pool._error_no() != 0) {
        printf("pool establish failed .. %d \n", pool._error_no());
    } else {
        printf("pool established .. %d \n", pool._error_no());
        FILE *f = fopen("recv.dat", "wb");
        for (;;) {
            int id = pool.poll(0);
            if (id != -1) {
                printf("#### id: %d\n", id);
                conn &c = pool[id];
                bool st = pool.pull(id);
                if (st) {
                    fwrite(c.to_str().c_str(), c.to_str().length(), 1, f);
                    printf("accept : %lu bytes\n", c.to_str().length());
                    c.flush();
                } else {
                    if (c.closed()) {
                        printf("is closed .. \n");
                        fclose(f);
                    } else {
                        printf("unknown error ? \n status : %hhu\n", c._status());
                    }
                    c.flush();
                }
            }
        }
    }
}
*/

#endif  //  ARGCV_NET_CO_LACUS_HH
