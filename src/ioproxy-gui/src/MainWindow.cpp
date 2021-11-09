#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, m_ui()
{
	m_ui.setupUi(this);
}

MainWindow::~MainWindow() = default;