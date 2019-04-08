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
#include <gk/core/input/KeyboardHandler.hpp>

#include "Registry.hpp"

class ClientApplication : public gk::CoreApplication {
	public:
		ClientApplication(int argc, char **argv);

		void init() override;

	private:
		void initOpenGL();

		gk::KeyboardHandler m_keyboardHandler;

		Registry m_registry;

		std::string m_host = "localhost";
		int m_port = 4242;
};

#endif // CLIENTAPPLICATION_HPP_
