#pragma once
#include <QList>
#include <QString>

/*
Example content:

```
License 1
creator Manuel Freiholz
allow core
allow io.tcpserver
option io.tcpserver.maxclients=1
```

*/

class License
{
public:
	bool read(QString text);

private:
	bool m_allowAll = false;
	QList<QString> m_allows;
};
