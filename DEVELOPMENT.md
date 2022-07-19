# Build Setup

## Visual Studio 2019
Extensions
- Format document on Save (https://marketplace.visualstudio.com/items?itemName=mynkow.FormatdocumentonSave)

## Commands for testing
ioproxy-cli ^
	-io tcpserver -name tcpsrv1 -p bind_address=0.0.0.0 -p bind_port=24001 -p max_clients=2 -p broadcast_clients=1

### Receives data (e.g. video) on udp/24002 and forwards it to udp/24003
-io udpsocket -name udpin -p bind_address=127.0.0.1 -p bind_port=24002 -io udpsocket -name udpout -p remote_address=127.0.0.1 -p remote_port=24003 -connect udpin,udpout
ioproxy-cli -io udpsocket -name udpin -p bind_address=192.168.50.107 -p bind_port=24002 -io udpsocket -name udpout -p remote_address=192.168.50.107 -p remote_port=24003 -connect udpin,udpout

### Receives data on tcp/24002, compress it and send to stdout.
-io tcpserver -name tcpsrv -p bind_port=24002 -io compression -name comp -p mode=c -io stdout -name stdout -connect tcpsrv,comp -connect comp,stdout

# ToDo's
- [ ] Implement `CompressionIO`. -p mode=[c|u] -p level=[0-9|-1]
- [ ] Implement `StdInIO`.
- [ ] First release as *.zip for Windows.