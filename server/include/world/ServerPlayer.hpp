/*
 * =====================================================================================
 *
 *       Filename:  ServerPlayer.hpp
 *
 *    Description:
 *
 *        Created:  22/01/2019 23:11:34
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef SERVERPLAYER_HPP_
#define SERVERPLAYER_HPP_

#include "Player.hpp"
#include "ServerInfo.hpp"

class ServerPlayer : public Player {
	public:
		ServerPlayer(Client &client);

		const Client &client() const { return m_client; }

	private:
		Client &m_client;
};

#endif // SERVERPLAYER_HPP_
