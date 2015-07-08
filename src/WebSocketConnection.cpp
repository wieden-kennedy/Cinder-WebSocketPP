/*
 * Copyright (c) 2015, Wieden+Kennedy
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ban the Rewind nor the names of its
 * contributors may be used to endorse or promote products
 * derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "WebSocketConnection.h"

/*
* Copyright (c) 2015, Wieden+Kennedy
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or
* without modification, are permitted provided that the following
* conditions are met:
*
* Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in
* the documentation and/or other materials provided with the
* distribution.
*
* Neither the name of the Ban the Rewind nor the names of its
* contributors may be used to endorse or promote products
* derived from this software without specific prior written
* permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#include "websocketpp/common/connection_hdl.hpp"

using namespace std;

WebSocketConnection::WebSocketConnection()
: mCloseEventHandler( nullptr ), mFailEventHandler( nullptr ), 
mHttpEventHandler( nullptr ), mInterruptEventHandler( nullptr ), 
mMessageEventHandler( nullptr ), mOpenEventHandler( nullptr ), 
mPingEventHandler( nullptr ), mSocket( nullptr ), mSocketInitEventHandler( nullptr ),
mTcpPostInitEventHandler( nullptr ), mTcpPreInitEventHandler( nullptr ), 
mValidateEventHandler( nullptr ), mWriteEventHandler( nullptr )
{
}

WebSocketConnection::~WebSocketConnection()
{
	disconnectCloseEventHandler();
	disconnectFailEventHandler();
	disconnectHttpEventHandler();
	disconnectInterruptEventHandler();
	disconnectMessageEventHandler();
	disconnectOpenEventHandler();
	disconnectPingEventHandler();
	disconnectSocketInitEventHandler();
	disconnectTcpPostInitEventHandler();
	disconnectTcpPreInitEventHandler();
	disconnectValidateEventHandler();
	disconnectWriteEventHandler();
}

const websocketpp::connection_hdl& WebSocketConnection::getHandle() const
{
	return mHandle;
}

asio::ip::tcp::socket* WebSocketConnection::getSocket() const
{
	return mSocket;
}

void WebSocketConnection::connectCloseEventHandler( const function<void()>& eventHandler )
{
	mCloseEventHandler = eventHandler;
}

void WebSocketConnection::disconnectCloseEventHandler()
{
	mCloseEventHandler = nullptr;
}

void WebSocketConnection::connectFailEventHandler( const function<void( string )>& eventHandler )
{
	mFailEventHandler = eventHandler;
}

void WebSocketConnection::disconnectFailEventHandler()
{
	mFailEventHandler = nullptr;
}

void WebSocketConnection::connectHttpEventHandler( const function<void()>& eventHandler )
{
	mHttpEventHandler = eventHandler;
}

void WebSocketConnection::disconnectHttpEventHandler()
{
	mHttpEventHandler = nullptr;
}

void WebSocketConnection::connectInterruptEventHandler( const function<void()>& eventHandler )
{
	mInterruptEventHandler = eventHandler;
}

void WebSocketConnection::disconnectInterruptEventHandler()
{
	mInterruptEventHandler = nullptr;
}

void WebSocketConnection::connectMessageEventHandler( const function<void( string )>& eventHandler )
{
	mMessageEventHandler = eventHandler;
}

void WebSocketConnection::disconnectMessageEventHandler()
{
	mMessageEventHandler = nullptr;
}

void WebSocketConnection::connectOpenEventHandler( const function<void()>& eventHandler )
{
	mOpenEventHandler = eventHandler;
}

void WebSocketConnection::disconnectOpenEventHandler()
{
	mOpenEventHandler = nullptr;
}

void WebSocketConnection::connectPingEventHandler( const function<void( string )>& eventHandler )
{
	mPingEventHandler = eventHandler;
}

void WebSocketConnection::disconnectPingEventHandler()
{
	mPingEventHandler = nullptr;
}

void WebSocketConnection::connectSocketInitEventHandler( const function<void()>& eventHandler )
{
	mSocketInitEventHandler = eventHandler;
}

void WebSocketConnection::disconnectSocketInitEventHandler()
{
	mSocketInitEventHandler = nullptr;
}

void WebSocketConnection::connectTcpPostInitEventHandler( const function<void()>& eventHandler )
{
	mTcpPostInitEventHandler = eventHandler;
}

void WebSocketConnection::disconnectTcpPostInitEventHandler()
{
	mTcpPostInitEventHandler = nullptr;
}

void WebSocketConnection::connectTcpPreInitEventHandler( const function<void()>& eventHandler )
{
	mTcpPreInitEventHandler = eventHandler;
}

void WebSocketConnection::disconnectTcpPreInitEventHandler()
{
	mTcpPreInitEventHandler = nullptr;
}

void WebSocketConnection::connectValidateEventHandler( const function<bool()>& eventHandler )
{
	mValidateEventHandler = eventHandler;
}

void WebSocketConnection::disconnectValidateEventHandler()
{
	mValidateEventHandler = nullptr;
}


void WebSocketConnection::connectWriteEventHandler( const function<void()>& eventHandler )
{
	mWriteEventHandler = eventHandler;
}

void WebSocketConnection::disconnectWriteEventHandler()
{
	mWriteEventHandler = nullptr;
}
 