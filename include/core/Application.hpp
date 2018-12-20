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

#include "CoreApplication.hpp"
#include "Registry.hpp"
#include "ScriptEngine.hpp"

class Application : public CoreApplication {
	public:
		Application(int argc, char **argv) : CoreApplication(argc, argv) {}

		void init() override;

		void initOpenGL();

	private:
		Registry m_registry;

		ScriptEngine m_scriptEngine;
};

#endif // APPLICATION_HPP_
