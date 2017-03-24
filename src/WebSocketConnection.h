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

#pragma once

#include "cinder/Cinder.h"

// Bruce Lane: replaced by _NOEXCEPT to compile with VS2015 to avoid error C2694
//#if defined( CINDER_MSW )
//	#define _WEBSOCKETPP_NOEXCEPT_TOKEN_
//#endif
#define ASIO_STANDALONE
#define ASIO_HAS_STD_ATOMIC
#define _WEBSOCKETPP_INITIALIZER_LISTS_
#define _WEBSOCKETPP_NULLPTR_
#define _WEBSOCKETPP_CPP11_FUNCTIONAL_
#define _WEBSOCKETPP_CPP11_SYSTEM_ERROR_
#define _WEBSOCKETPP_CPP11_MEMORY_
#define _WEBSOCKETPP_CPP11_THREAD_
#define _WEBSOCKETPP_CPP11_RANDOM_DEVICE_
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB

#include <set>

#include "asio.hpp"
#include "websocketpp/common/random.hpp"
#include "websocketpp/common/thread.hpp"
#include "websocketpp/common/connection_hdl.hpp"

class WebSocketConnection
{
public:
	WebSocketConnection();
	~WebSocketConnection();

	virtual void	ping( const std::string& msg ) = 0;
	virtual void	poll() = 0;
	virtual void	write( const std::string& msg ) = 0;

	const websocketpp::connection_hdl&	getHandle() const;
	asio::ip::tcp::socket*				getSocket() const;

	template<typename T, typename Y>
	inline void	connectCloseEventHandler( T eventHandler, Y* eventHandlerObject )
	{
		connectCloseEventHandler( std::bind( eventHandler, eventHandlerObject ) );
	}
	void		connectCloseEventHandler( const std::function<void()>& eventHandler );
	void		disconnectCloseEventHandler();

	template<typename T, typename Y>
	inline void	connectFailEventHandler( T eventHandler, Y* eventHandlerObject )
	{
		connectFailEventHandler( std::bind( eventHandler, eventHandlerObject ) );
	}
	void		connectFailEventHandler( const std::function<void( std::string )>& eventHandler );
	void		disconnectFailEventHandler();

	template<typename T, typename Y>
	inline void	connectHttpEventHandler( T eventHandler, Y* eventHandlerObject )
	{
		connectHttpEventHandler( std::bind( eventHandler, eventHandlerObject ) );
	}
	void		connectHttpEventHandler( const std::function<void()>& eventHandler );
	void		disconnectHttpEventHandler();

	template<typename T, typename Y>
	inline void	connectInterruptEventHandler( T eventHandler, Y* eventHandlerObject )
	{
		connectInterruptEventHandler( std::bind( eventHandler, eventHandlerObject ) );
	}
	void		connectInterruptEventHandler( const std::function<void()>& eventHandler );
	void		disconnectInterruptEventHandler();

	template<typename T, typename Y>
	inline void	connectMessageEventHandler( T eventHandler, Y* eventHandlerObject )
	{
		connectMessageEventHandler( std::bind( eventHandler, eventHandlerObject, std::placeholders::_1 ) );
	}
	void		connectMessageEventHandler( const std::function<void( std::string )>& eventHandler );
	void		disconnectMessageEventHandler();

	template<typename T, typename Y>
	inline void	connectOpenEventHandler( T eventHandler, Y* eventHandlerObject )
	{
		connectOpenEventHandler( std::bind( eventHandler, eventHandlerObject ) );
	}
	void		connectOpenEventHandler( const std::function<void()>& eventHandler );
	void		disconnectOpenEventHandler();

	template<typename T, typename Y>
	inline void	connectPingEventHandler( T eventHandler, Y* eventHandlerObject )
	{
		connectPingEventHandler( std::bind( eventHandler, eventHandlerObject, std::placeholders::_1 ) );
	}
	void		connectPingEventHandler( const std::function<void( std::string )>& eventHandler );
	void		disconnectPingEventHandler();

	template<typename T, typename Y>
	inline void	connectSocketInitEventHandler( T eventHandler, Y* eventHandlerObject )
	{
		connectSocketInitEventHandler( std::bind( eventHandler, eventHandlerObject, std::placeholders::_1 ) );
	}
	void		connectSocketInitEventHandler( const std::function<void()>& eventHandler );
	void		disconnectSocketInitEventHandler();

	template<typename T, typename Y>
	inline void	connectTcpPostInitEventHandler( T eventHandler, Y* eventHandlerObject )
	{
		connectTcpPostInitEventHandler( std::bind( eventHandler, eventHandlerObject ) );
	}
	void		connectTcpPostInitEventHandler( const std::function<void()>& eventHandler );
	void		disconnectTcpPostInitEventHandler();

	template<typename T, typename Y>
	inline void	connectTcpPreInitEventHandler( T eventHandler, Y* eventHandlerObject )
	{
		connectTcpPreInitEventHandler( std::bind( eventHandler, eventHandlerObject ) );
	}
	void		connectTcpPreInitEventHandler( const std::function<void()>& eventHandler );
	void		disconnectTcpPreInitEventHandler();

	template<typename T, typename Y>
	inline void	connectValidateEventHandler( T eventHandler, Y* eventHandlerObject )
	{
		connectValidateEventHandler( std::bind( eventHandler, eventHandlerObject ) );
	}
	void		connectValidateEventHandler( const std::function<bool()>& eventHandler );
	void		disconnectValidateEventHandler();

	template<typename T, typename Y>
	inline void	connectWriteEventHandler( T eventHandler, Y* eventHandlerObject )
	{
		connectWriteEventHandler( std::bind( eventHandler, eventHandlerObject ) );
	}
	void		connectWriteEventHandler( const std::function<void()>& eventHandler );
	void		disconnectWriteEventHandler();
protected:
	websocketpp::connection_hdl			mHandle;
	asio::ip::tcp::socket*				mSocket;
	
	std::function<void()>				mCloseEventHandler;
	std::function<void( std::string )>	mFailEventHandler;
	std::function<void()>				mHttpEventHandler;
	std::function<void()>				mInterruptEventHandler;
	std::function<void( std::string )>	mMessageEventHandler;
	std::function<void()>				mOpenEventHandler;
	std::function<void( std::string )>	mPingEventHandler;
	std::function<void()>				mSocketInitEventHandler;
	std::function<void()>				mTcpPreInitEventHandler;
	std::function<void()>				mTcpPostInitEventHandler;
	std::function<bool()>				mValidateEventHandler;
	std::function<void()>				mWriteEventHandler;
};
