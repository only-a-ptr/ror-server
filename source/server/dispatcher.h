/*
This file is part of "Rigs of Rods Server" (Relay mode)

Copyright (c) 2019 Petr Ohlidal

"Rigs of Rods Server" is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

"Rigs of Rods Server" is distributed in the hope that it will
be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Rigs of Rods Server".
If not, see <http://www.gnu.org/licenses/>.
*/

/// @file Event dispatch loop
/// @author Petr Ohlidal, 2019

#include "listener.h" // Legacy
#include "master-server.h"
#include "messaging.h"
#include "prerequisites.h"
#include "sequencer.h"

#include <event2/event_struct.h>
#include <event2/event.h>
#include <event2/listener.h>

class Dispatcher
{
public:
    Dispatcher(Sequencer* sequencer, MasterServer::Client& serverlist);

// Lifecycle:
    void Initialize();
    void RunDispatchLoop();
    void Shutdown();

// Operations:
    void HandleNewConnection(kissnet::tcp_socket socket);
    void UpdateStats();
    void PerformHeartbeat();
    bool RegisterClient(Client* client);
    void RemoveClient(Client* client);

// Callbacks:
    static void ConnAcceptCallback(::evconnlistener* listener,
                                   ::evutil_socket_t sock,
                                   sockaddr* addr, int socklen, void* ptr);
    static void ConnErrorCallback(::evconnlistener* listener, void* ptr);
    static void HeartbeatCallback(::evutil_socket_t sock, short what, void* ptr);
    static void StatsCallback(::evutil_socket_t sock, short what, void* ptr);

private:
    kissnet::tcp_socket   m_socket;
    ::event_base*         m_ev_base = nullptr;
    ::evconnlistener*     m_ev_listener = nullptr;
    ::event*              m_ev_heartbeat = nullptr;
    ::event*              m_ev_stats = nullptr;
    Listener              m_conn_handler;
    MasterServer::Client& m_serverlist;
    Sequencer*            m_sequencer = nullptr;
    size_t                m_heartbeat_failcount = 0;
};