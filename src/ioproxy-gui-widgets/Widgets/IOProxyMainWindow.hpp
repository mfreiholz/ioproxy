#pragma once
#include <QMainWindow>
#include "ui_IOProxyMainWindow.h"

class IOProxyMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	IOProxyMainWindow(QWidget* parent = nullptr);
	~IOProxyMainWindow() override;

private:
	Ui_IOProxyMainWindow _ui;
};