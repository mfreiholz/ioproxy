#pragma once
#include <QObject>
#include <QWebSocket>

class WebSocketHandler : public QObject
{
	Q_OBJECT

public:
	WebSocketHandler(QWebSocket* ws, QObject* parent)
		: QObject(parent)
		, m_socket(ws)
	{
		QObject::connect(m_socket, &QWebSocket::binaryFrameReceived, this, &WebSocketHandler::onBinaryFrameReceived);
	}

	~WebSocketHandler()
	{
		m_socket = nullptr;
	}

private slots:
	void onBinaryFrameReceived(const QByteArray& frame, bool isLastFrame)
	{
	}

	void onBinaryMessageReceived(const QByteArray& message)
	{}

	void onPong(quint64 elapsedTime, const QByteArray& payload)
	{}

	void onTextFrameReceived(const QString& frame, bool isLastFrame)
	{}

	void onTextMessageReceived(const QString& message)
	{}

signals:
	void newData(const DataPack&);

private:
	QWebSocket* m_socket;
};