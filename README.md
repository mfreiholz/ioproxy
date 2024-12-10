# IOProxy
IOProxy is an input/output proxy. It allows to receive data from different data-sources and forwards it to other data-sources.

## Features
**Build-in IOs (data-sources)**
- TCP Server
- TCP Client
- UDP Socket (Unitcast/Multicast/Broadcast)
- Serial port (RS232, RS422, ...)
- File Writer
- StdOut (Console Output)

## How and Why to use it
The idea behind the program is to provide the possibility to redirect traffic from one source to another without a lot of effort.

IOProxy allows to define multiple data-sources, also called IOs.
Each defined IO can be connected to one or more other IO(s).
It allows to define the exact route of incoming and outgoing data.

**Example scenarios:**
- Provide serial-port access via UDP or TCP
- Forward multicast into another subnet via unicast (+over gateway/vpn)

### Small illustration example
Imaging a computer (PC-1) with a device connected to serial-port (COM1). This device sends some kind of status every second. In addition there are two computers in the network which wants to know about the status. By default it is not allowed to have multiple processes/machines open the serial port.

This problem can be solved by having a single process (ioproxy) opening the serial-port and provide all data via TCP or UDP to multiple clients.

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
                                                    *PC-X can also run a custom process
                                                    instead of ioproxy-cli,
                                                    which establishes a TCP connection.

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
ioproxy-cli.exe {-io <type> -name <unique-name> [-p <key>=<value> ...] ...}
[-connect <unique-name-of-source>,<unique-name-of-destination> ...]
```

`-io <type>` The TYPE of the IO to be created (e.g. `tcpserver`)

`-name <unique-name>` Unique name for the last defined `-io`. This string can then be used with the `-connect` parameter.

`-p <key>=<value>` Defines parameter specifically to the last defined `-io`. See [Input/Output Documentation](#TODO-REF) for available parameters.

`-connect <unique-name-of-source>,<unique-name-of-destination>`


### Examples

#### Listen for UDP multicast and forward to clients connected to TCP Server
```
# Windows Command Prompt
ioproxy-cli ^
-io udpsocket -name mc_sock -p bind_socket=1234 -p remote_address=224.10.10.10 ^
-io tcpserver -name tcp_server -p bind_port=5678 -p max_clients=10 ^
-connect mc_sock,tcp_server
```

#### Listen for multiple TCP connections and forward data to STDOUT.
```
ioproxy-cli ^
-io tcpserver -name tcp_server -p bind_port=5678 -p max_clients=10 ^
-io_
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
# Runs a TCP Server listen on port 1337 and forwards received data to the other connected clients.
-io tcpserver -name MyTcpServer -p bind_address=0.0.0.0 -p bind_port=1337 -p max_clients=99 -p broadcast_clients=1
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

### Serial Port (`-io serialport`)

| Key | Default | Value |
| --- | --- | --- |
| port* | n/a | Name of the serial port. Windows: `COM1`, Linux: `/dev/ttyS0` |
| baudrate | 115200 | Baudrate to use for this port. |
| databits | `8` | The number of data bits used. |
| parity | `0` | Parity scheme. 0 = NoParity, 2 = EvenParity, 3 = OddParity, 4 = SpaceParity, 5 = MarkParity |
| stopbits | `1` | The number of stop bits used. 1 = OneStop; 2 = TwoStop; 3 = OneAndHalfStop; -1 = UnknownStopBits |
| flow | `0` | 0 = Disabled, 1 = Hardware flow control (RTS/CTS), 2 = Software flow control (XON/XOFF) |
