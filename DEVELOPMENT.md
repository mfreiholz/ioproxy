# Build Setup

## Visual Studio 2019
Extensions
- Format document on Save (https://marketplace.visualstudio.com/items?itemName=mynkow.FormatdocumentonSave)

## Commands for testing
ioproxy-cli ^
	-io tcpserver -name tcpsrv1 -p bind_address=0.0.0.0 -p bind_port=24001 -p max_clients=2 -p broadcast_clients=1

### Receives data (e.g. video) on udp/24002 and forwards it to udp/24003
ioproxy-cli -io udpsocket -name udpin -p bind_address=192.168.50.107 -p bind_port=24002 -io udpsocket -name udpout -p remote_address=192.168.50.107 -p remote_port=24003 -connect udpin,udpout

# ToDo's
- [ ] Implement `CompressionIO`. -p mode=[c|d] -p level=[0-9|-1]
- [ ] Implement `StdInIO`.
- [ ] First release as *.zip for Windows.