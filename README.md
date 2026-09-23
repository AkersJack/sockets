# sockets
Reference/templates for sockets 


getaddrinfo()
socket() 
setsockopt()
bind()
listen() 
accept()
recv()/send() 


POSIX/BSD socket API

Process per connection (fork())

Thread-per-connection (pthread_create())


Older C examples usually directly use: 
`#include <netinet/in.h>`
`#include <arpa/inet.h>`

Then they build `struct sockaddr_in` manually. 

```
struct sockadder_in addr = {
    .sin_family = AF_INET,
    .sin_port = htons(8080), 
    .sin_addr.s_addr = htonl(INADDR_ANY)
};
```

If you want to do IPv6 you would need to manually build the `sockadder_in6` struct. 


The more portable defualt is `getaddrinfo()` with: 
    ```
    hints.ai_family = AF_UNSPEC; 
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_flags = AI_PASSIVE
    ```

`getaddrinfo()` with `AI_PASSIVE` and a `NULL` hostname `getaddrinfo()` returns wildcard addresses suitable for a server `bind()`. 

Additionally, `AF_UNSPEC` allows IPv4 or IPv6 results instead of baking one address family into your code. 


Unix-domain sockets are often a better fit than TCP when both programs live on the same machine.
`#include <sys/socket.h>`
`#include <sys/un.h>`


Best bet is to use a POSIX socket server. This information also applies to if you were going to build a socket server in C++ as well. 
