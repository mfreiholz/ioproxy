# IOProxy
IOProxy is an input/output proxy. It allows to receive data from different data sources (IOs) and forwards it to another defined data-sources (IOs).

## Features
**Build-in IOs (data-sources)**
- TCP Server
- TCP Client
- UDP Socket (Unitcast/Multicast/~~Broadcast~~)
- Serial port (RS232, RS422, ...)
- File Writer
- StdOut (Console Output)
- ~~HTTP WebSocket Server~~ (upcoming)
- ~~HTTP WebSocket Client~~ (upcoming)

## Commandline usage
One process can have multiple IOs.
```
ioproxy.exe {-io <io> -name <unique-name> [-p <key>=<value> ...] ...}
[-connect <unique-name-of-source>,<unique-name-of-destination> ...]
```

### Examples

#### Listen for UDP multicast and forward to clients connected to TCP Server
```
# Windows Command Prompt
ioproxy ^
-io udpsocket -name mc_sock -p bind_socket=1234 -p remote_address=224.10.10.10 ^
-io tcpserver -name tcp_server -p bind_port=5678 -p max_clients=10 ^
-connect mc_sock,tcp_server
```
