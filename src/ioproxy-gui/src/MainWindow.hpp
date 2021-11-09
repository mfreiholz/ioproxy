#pragma once
#include "ui_MainWindow.h"
#include <QMainWindow>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow() override;

private:
	Ui_MainWindowForm m_ui;
};