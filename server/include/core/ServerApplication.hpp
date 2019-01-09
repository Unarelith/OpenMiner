/*
 * =====================================================================================
 *
 *       Filename:  ServerApplication.hpp
 *
 *    Description:
 *
 *        Created:  09/01/2019 19:37:20
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef SERVERAPPLICATION_HPP_
#define SERVERAPPLICATION_HPP_

#include <gk/core/CoreApplication.hpp>

#include "NetworkInputHandler.hpp"
#include "Registry.hpp"
#include "Server.hpp"

class ServerApplication : public gk::CoreApplication {
	public:
		ServerApplication(int argc, char **argv);

		void init() override;

	private:
		void mainLoop() override;

		Server m_server;

		Registry m_registry;
};

#endif // SERVERAPPLICATION_HPP_
