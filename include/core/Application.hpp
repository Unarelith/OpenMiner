/*
 * =====================================================================================
 *
 *       Filename:  Application.hpp
 *
 *    Description:
 *
 *        Created:  14/12/2014 05:09:11
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include <gk/core/CoreApplication.hpp>

#include "KeyboardHandler.hpp"
#include "Registry.hpp"
#include "ScriptEngine.hpp"

class Application : public gk::CoreApplication {
	public:
		Application(int argc, char **argv) : gk::CoreApplication(argc, argv) {}

		void init() override;

	private:
		KeyboardHandler m_keyboardHandler;

		Registry m_registry;

		ScriptEngine m_scriptEngine;
};

#endif // APPLICATION_HPP_
