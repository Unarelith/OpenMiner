/*
 * =====================================================================================
 *
 *       Filename:  ResourceHandler.hpp
 *
 *    Description:
 *
 *        Created:  17/01/2018 19:16:19
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef RESOURCEHANDLER_HPP_
#define RESOURCEHANDLER_HPP_

#include <map>
#include <memory>

#include "Exception.hpp"
#include "IResourceLoader.hpp"
#include "XMLFile.hpp"

class ResourceHandler {
	public:
		template<typename T, typename... Args>
		T &add(const std::string &name, Args &&...args) {
			if(has(name)) {
				throw EXCEPTION("A resource of type", typeid(T).name(), "already exists with name:", name);
			}

			m_resources.emplace(name, std::make_shared<T>(std::forward<Args>(args)...));

			return get<T>(name);
		}

		bool has(const std::string &name) {
			return m_resources.count(name) == 1;
		}

		template<typename T>
		T &get(const std::string &name) {
			auto it = m_resources.find(name);
			if(it == m_resources.end()) {
			 	throw EXCEPTION("Unable to find resource with name:", name);
			}

			return *std::static_pointer_cast<T>(it->second);
		}

		template<typename ResourceLoader>
		static auto loadConfigFile(const char *xmlFilename) -> typename std::enable_if<std::is_base_of<IResourceLoader, ResourceLoader>::value>::type {
			ResourceLoader loader;
			loader.load(xmlFilename, getInstance());
		}

		static ResourceHandler &getInstance();

		static void setInstance(ResourceHandler &handler);

	private:
		static ResourceHandler *instance;

		std::map<std::string, std::shared_ptr<void>> m_resources;
};

#endif // RESOURCEHANDLER_HPP_
