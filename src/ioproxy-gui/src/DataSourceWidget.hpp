#pragma once
#include "ui_DataSourceWidget.h"
#include <QWidget>

class DataSourceWidget : public QWidget
{
	Q_OBJECT

public:
	explicit DataSourceWidget(QWidget* parent = nullptr);
	~DataSourceWidget() override;

private:
	Ui_DataSourceWidgetForm m_ui;
};