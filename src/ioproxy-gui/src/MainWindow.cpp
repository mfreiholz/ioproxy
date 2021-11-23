#include "MainWindow.hpp"
#include "DataSourceWidget.hpp"
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, m_ui()
{
	m_ui.setupUi(this);

	m_scene = new QGraphicsScene(this);
	auto textItem = m_scene->addText("Hello World!");
	textItem->setFlags(textItem->flags() | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);

	m_scene->addRect(1, 1, 5, 5);

	auto dswidget = new DataSourceWidget(nullptr);
	auto dsWidgetProxy = m_scene->addWidget(dswidget);
	dsWidgetProxy->setFlags(dsWidgetProxy->flags() | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);


	m_ui.graphicsView->setScene(m_scene);
}

MainWindow::~MainWindow() = default;