/*
 * =====================================================================================
 *
 *       Filename:  ClientApplication.hpp
 *
 *    Description:
 *
 *        Created:  09/01/2019 19:33:30
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef CLIENTAPPLICATION_HPP_
#define CLIENTAPPLICATION_HPP_

#include <gk/core/CoreApplication.hpp>

#include "Client.hpp"
#include "KeyboardHandler.hpp"
#include "Registry.hpp"

class ClientApplication : public gk::CoreApplication {
	public:
		ClientApplication(int argc, char **argv);

		void init() override;

	private:
		void initOpenGL();

		KeyboardHandler m_keyboardHandler;

		Registry m_registry;

		Client m_client;
		int m_port = 4242;
};

#endif // CLIENTAPPLICATION_HPP_
