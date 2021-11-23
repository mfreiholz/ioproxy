#pragma once
#include <QObject>
#include <ioproxy/Config.hpp>
#include <ioproxy/Handler.hpp>
#include <memory>
#include <vector>

namespace ioproxy
{
	/// Engine
	/**
		Engine handles the runtime of IOs and connects them together.
	**/
	class Engine : public QObject
	{
		Q_OBJECT

	public:
		static void RegisterMetaTypes();

		Engine(Config config, QObject* parent);
		~Engine() override;

		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;

		/**
			Returns a summarized statistic over all active IOs.
		**/
		Statistic statisticSummary() const;

		/**
			Establishes connections between IOs and runs them.
		**/
		Q_SLOT void start();

		/**
		**/
		Q_SLOT void stop();

	signals:
		void startupErrorOccured();

	private:
		Config m_config;
		std::vector<std::unique_ptr<Handler>> m_handlers;

		friend class Handler;
	};
}