/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/** \file Server.h
* \brief Encapsulates MoSync's server connections
* \author Fredrik Eldh
*/

#ifndef _MAUTIL_SERVER_H_
#define _MAUTIL_SERVER_H_

#include <MAUtil/Connection.h>

namespace MAUtil {

class Server;

/// Receives events from MAUtil::Server.
class ServerListener {
public:
	/**
	* Called when a Server::accept() operation fails.
	* The default implementation does nothing.
	* \param server The Server.
	* \param result One of the \link #CONNERR_GENERIC CONNERR \endlink error codes.
	*/
	virtual void serverAcceptFailed(Server* server, int result);
	/**
	* Called when a Server::accept() operation succeeds.
	* \param server The Server.
	* \param newConnection The incoming Connection. When you're done with it,
	* be sure to delete it, to avoid memory leaks.
	*/
	virtual void serverAccepted(Server* server, Connection* newConnection) = 0;
};

/**
* \brief Encapsulates MoSync's server connections.
* Use this class to listen for incoming connections.
*/
class Server : protected ConnListener {
public:
	Server(ServerListener* listener) : mListener(listener), mServ(0) {}

	/**
	* Closes the server.
	*/
	virtual ~Server();

	/**
	* Starts the server. This involves opening a socket, binding it to a port,
	* and, for Bluetooth, adding an entry to the service advertisement list.
	* Once this function returns, you should call accept() to begin receiving connections.
	* \param url A server URL. See maConnect() for details.
	* \returns \> 0 on success, \< 0 on error.
	*/
	int start(const char* url);

	/**
	* Retrieves the local address of this server.
	* \param dst A pointer to the struct that will receive the address.
	* \returns \> 0 on success, \< 0 on error.
	*/
	int getAddr(MAConnAddr* dst);

	/**
	* Asynchronously accepts one incoming connection.
	* Will cause either ServerListener::serverAccepted() or ServerListener::serverAcceptFailed()
	* to be called when the operation is complete.
	* \param cl When a connection is accepted, this will be its listener.
	* \returns \> 0 on success, \< 0 on error.
	*/
	int accept(ConnectionListener* cl);

	/**
	* Closes the server. Removes its service advertisement, if any.
	*/
	void close();

	/**
	* Returns true if the server is open, false otherwise.
	* The server is considered open once start() succeeds.
	*/
	bool isOpen() const;

protected:
	// ConnListener
	virtual void connEvent(const MAConnEventData& data);

	ServerListener* mListener;
	MAHandle mServ;
	ConnectionListener* mCl;
};

}

#endif	//_MAUTIL_SERVER_H_
