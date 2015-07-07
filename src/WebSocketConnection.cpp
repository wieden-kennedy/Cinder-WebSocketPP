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

#include "cinder/signals.h"
#include "websocketpp/common/connection_hdl.hpp"

using namespace std;

WebSocketConnection::WebSocketConnection()
: mConnectEventHandler( nullptr ), mDisconnectEventHandler( nullptr ),
mErrorEventHandler( nullptr ), mInterruptEventHandler( nullptr ),
mPingEventHandler( nullptr ), mReadEventHandler( nullptr ),
mWriteEventHandler( nullptr )
{}

WebSocketConnection::~WebSocketConnection()
{
	disconnectConnectEventHandler();
	disconnectDisconnectEventHandler();
	disconnectErrorEventHandler();
	disconnectInterruptEventHandler();
	disconnectPingEventHandler();
	disconnectReadEventHandler();
	disconnectWriteEventHandler();
}

void WebSocketConnection::connectConnectEventHandler( const function<void()>& eventHandler )
{
	mConnectEventHandler = eventHandler;
}

void WebSocketConnection::disconnectConnectEventHandler()
{
	mConnectEventHandler = nullptr;
}

void WebSocketConnection::connectDisconnectEventHandler( const function<void()>& eventHandler )
{
	mDisconnectEventHandler = eventHandler;
}

void WebSocketConnection::disconnectDisconnectEventHandler()
{
	mDisconnectEventHandler = nullptr;
}

void WebSocketConnection::connectErrorEventHandler( const function<void( string )>& eventHandler )
{
	mErrorEventHandler = eventHandler;
}

void WebSocketConnection::disconnectErrorEventHandler()
{
	mErrorEventHandler = nullptr;
}

void WebSocketConnection::connectInterruptEventHandler( const function<void()>& eventHandler )
{
	mInterruptEventHandler = eventHandler;
}

void WebSocketConnection::disconnectInterruptEventHandler()
{
	mInterruptEventHandler = nullptr;
}

void WebSocketConnection::connectPingEventHandler( const function<void( string )>& eventHandler )
{
	mPingEventHandler = eventHandler;
}

void WebSocketConnection::disconnectPingEventHandler()
{
	mPingEventHandler = nullptr;
}

void WebSocketConnection::connectReadEventHandler( const function<void( string )>& eventHandler )
{
	mReadEventHandler = eventHandler;
}

void WebSocketConnection::disconnectReadEventHandler()
{
	mReadEventHandler = nullptr;
}

void WebSocketConnection::connectWriteEventHandler( const function<void()>& eventHandler )
{
	mWriteEventHandler = eventHandler;
}

void WebSocketConnection::disconnectWriteEventHandler()
{
	mWriteEventHandler = nullptr;
}
 