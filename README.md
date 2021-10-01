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

## How and Why to use it
The idea behind the program is to provide the possibility to redirect traffic from one source to another.

IOProxy allows to define multiple data-sources, also called IOs.
Each defined IO can be connected to one or more other defined IO(s).
It lets you define the exact route of in- and outgoing data.

**Example scenarios:**
- Provide serial-port access via UDP or TCP
- Forward multicast into another subnet (+over gateway/vpn)

### Small illustration example
Imaging you do have a computer (PC-1) with a device connected to serial-port (COM1). This device sends some kind of status every second. In addition there are two computers in the network which wants to know about the status.

This problem can be solved by having a process opening the serial-port and provide all data via TCP or UDP to multiple clients.

```
┌───────────────────────────────────┐              ┌───────────────────────────────────┐
│ PC-1 with serial-port COM1        │              │ PC-X interested in state          │
│                                   │              │                                   │
│ ┌──────────────┐  ┌─────────────┐ │              │ ┌─────────────┐  ┌──────────────┐ │
│ │-io serialport│  │-io tcpserver│◄├──────────────┼─┤-io tcpsocket│  │-io filewriter│ │
│ └───────┬──────┘  └─────────────┘ │              │ └───────┬─────┘  └──────────────┘ │
│         │                 ▲       │              │         │               ▲         │
│         │     connect     │       │              │         │    connect    │         │
│         └─────────────────┘       │              │         └───────────────┘         │
│                                   │              │                                   │
└───────────────────────────────────┘              └───────────────────────────────────┘

# Command on PC-1
ioproxy
	-io serialport -name sport -p port=COM1
	-io tcpserver -name tcpsrv -p bind_address=192.168.0.10 -p bind_port=1234
	-connect sport,tcpsrv

# Command on PC-X (can be multiple)
ioproxy
	-io tcpsocket -name tcpsock -p remote_address=192.168.0.10 -p remote_port=1234 -p reconnect=1
	-io filewriter -name fwrite
	-connect tcpsock,fwrite
```

# Documentation

## Commandline usage

```
ioproxy.exe {-io <io-id> -name <unique-name> [-p <key>=<value> ...] ...}
[-connect <unique-name-of-source>,<unique-name-of-destination> ...]
```

`-io \<io-id>` The ID of the IO to be created.

`-name <unique-name>` Unique name for the last defined `-io`. This string can then be used with the `-connect` parameter.

`-p <key>=<value>` Defines parameter specifically to the previously created IO. See (Input/Output Documentation)[] for available parameters.

`-connect <unique-name-of-source>,<unique-name-of-destination>`


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

Parameters marked with \* (asterix) are required.

### TCP Server (`-io tcpserver`)

| Key | Default | Note |
| --- | --- | --- |
| bind_address | All IPv4/v6 | IPv4/6 address of network interface |
| bind_port* | n/a | Port on which to listen for new connections. |
| max_clients | `1` | Maximum number of accepted client connections. |
| broadcast_clients | `0` | If `1` the server sends packets from one client to all other connected clients. |

**Example**
```
# Runs a TCP Server listen on port 1337 and broadcasts all received data to the other connected clients.
-io tcpserver -p bind_address=0.0.0.0 -p bind_port=1337 -p max_clients=99 -p broadcast_clients=1
```

### TCP Socket/Client (`-io tcpsocket`)

| Key | Default | Value |
| --- | --- | --- |
| remote_address* | n/a | IPv4/6 address of the remote machine. |
| remote_port* | n/a | Port on which the remote machine is listen for new connections. |
| reconnect | `0` | If `1` the client automatically tries to reconnect, if there is any problem with the connection. |
| low_delay | `0` | If `1` it sets the `TCP_NODELAY` option on socket (disables Nagle's algorithm). |
| keep_alive | `0` | If `1` it sets the `SO_KEEPALIVE` option on socket. |

**Example**
```
# Connects to a remote machine. Automatically reconnects, if the connection drops.
-io tcpsocket -p remote_address=127.0.0.1 -p remote_port=1337 -p reconnect=1 -p low_delay=1
```

### UDP Socket (`-io udpsocket`)

| Key | Default | Value |
| --- | --- | --- |
| bind_address | All IPv4/v6 | IPv4/IPv6 address of local network interface to bind socket. |
| bind_port | Random | Port number on which to listen for incoming packets. In multicast mode this is the group-port. |
| remote_address | n/a | IPv4/IPv6 address of remote machine. |
| remote_port | n/a | Port of remote machine. |
| echo | `0` | Sends received data back to sender. |
| multicast_ttl | `0` | TTL for multicast datagrams. |
| multicast_loopback | `0` | If `1` the loopback for multicast packets is enabled. |

The combination of `remote_address` and `remote_port` can be used multiple times to define more than one destination in unicast mode.

**Example**
```
# Sends data to multiple remote UDP receivers.
-io udpsocket -p remote_address=192.168.0.10 -p bind_port=1310 -p remote_address=192.168.0.20 -p remote_port=1310

# Receive unicast datagrams.
-io udpsocket -p bind_address=0.0.0.0 -p bind_port=1338

# Receive multicast datagrams.
-io udpsocket -p bind_address=192.168.0.10 -p bind_port=12007 -p remote_address=224.10.10.10 -p multicast_ttl=10

# Broadcast
# TODO !!!
```

### Serial Port

| Key | Note | Default |
| --- | --- | --- |
| port | Name of the serial port. Windows: `COM1`, Linux: `/dev/ttyS0` | n/A
| baudrate | Baudrate to use for this port. | 115200 |
