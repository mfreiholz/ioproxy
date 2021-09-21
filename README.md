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

## Input/Outputs

### TCP Server (io=tcpserver)

| Key | Value |
| --- | ----- |
| bind_address | IPv4/6 address of network interface (e.g. `0.0.0.0`) |
| bind_port | Port on which to listen for new connections. |
| max_clients | Maximum number of accepted client connections. |
| broadcast_clients | If `1` the server sends packets from one client to all others connected clients. |

### TCP Socket/Client (io=tcpsocket)

| Key | Value |
| --- | ----- |
| remote_address | IPv4/6 address of the remote machine. |
| remote_port | Port on which the remote machine is listen for new connections. |
| reconnect | If `1` the client automatically tries to reconnect, if there is any problem with the connection. |

### UDP Socket (io=udpsocket)

| Key | Note |
| --- | --- |
| bind_address | IPv4/IPv6 address of local network interface to bind socket. |
| bind_port | Port number on which to listen for incoming packets. In multicast mode this is the group-port. |
| remote_address | IPv4/IPv6 address of remote machine. |
| remote_port | Port of remote machine. |
| multicast_ttl | TTL for multicast datagrams. |
| multicast_loopback | If `1` the loopback for multicast packets is enabled. |

The combination of `remote_address` and `remote_port` can be used multiple times to define more than one destination.

### Serial Port

| Key | Note | Default |
| --- | --- | --- |
| port | Name of the serial port. Windows: `COM1`, Linux: `/dev/ttyS0` | n/A
| baudrate | Baudrate to use for this port. | 115200 |
