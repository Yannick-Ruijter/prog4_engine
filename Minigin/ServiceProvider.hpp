#pragma once

#include "Singleton.hpp"
#include "Service.hpp"
#include <vector>
#include <memory>

namespace dae
{
	class ServiceProvider final : public Singleton<ServiceProvider>
	{
	public:
		template <typename T, typename ...Args>
		void AddService(Args&& ...args)
		{
			m_Services.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		};

		template <typename T>
		void RemoveService()
		{
			T* toBeRemoved = GetService<T>();
			m_Services.erase(
				std::remove_if(
					m_Services.begin(),
					m_Services.end(),
					[toBeRemoved](const auto& ptr) { return ptr.get() == toBeRemoved; }
				),
				m_Services.end()
			);
		};

		template <typename T>
		T* GetService() const
		{
			for (auto const& service : m_Services)
			{
				T* castedService = dynamic_cast<T*>(service.get());
				if (castedService != nullptr) return castedService;
			}
			return nullptr;
		};
	private:
		friend class Singleton<ServiceProvider>;
		ServiceProvider() = default;

		std::vector<std::unique_ptr<Service>> m_Services;
	};
}