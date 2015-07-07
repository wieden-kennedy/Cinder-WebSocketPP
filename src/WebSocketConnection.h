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

#if defined( CINDER_MSW )
	#define _WEBSOCKETPP_NOEXCEPT_TOKEN_
#endif
#define _WEBSOCKETPP_INITIALIZER_LISTS_
#define _WEBSOCKETPP_NULLPTR_
#define _WEBSOCKETPP_CPP11_FUNCTIONAL_
#define _WEBSOCKETPP_CPP11_SYSTEM_ERROR_
#define _WEBSOCKETPP_CPP11_MEMORY_
#define _WEBSOCKETPP_CPP11_THREAD_
#define _WEBSOCKETPP_CPP11_RANDOM_DEVICE_
#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB

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
	
	template<typename T, typename Y>
	inline void		connectConnectEventHandler( T eventHandler, Y* eventHandlerObject )
	{
		connectConnectEventHandler( std::bind( eventHandler, eventHandlerObject ) );
	}
	void			connectConnectEventHandler( const std::function<void()>& eventHandler );
	void			disconnectConnectEventHandler();

	template<typename T, typename Y>
	inline void		connectDisconnectEventHandler( T eventHandler, Y* eventHandlerObject )
	{
		connectDisconnectEventHandler( std::bind( eventHandler, eventHandlerObject ) );
	}
	void			connectDisconnectEventHandler( const std::function<void()>& eventHandler );
	void			disconnectDisconnectEventHandler();

	template<typename T, typename Y>
	inline void		connectErrorEventHandler( T eventHandler, Y* eventHandlerObject )
	{
		connectErrorEventHandler( std::bind( eventHandler, eventHandlerObject ) );
	}
	void			connectErrorEventHandler( const std::function<void( std::string )>& eventHandler );
	void			disconnectErrorEventHandler();

	template<typename T, typename Y>
	inline void		connectInterruptEventHandler( T eventHandler, Y* eventHandlerObject )
	{
		connectInterruptEventHandler( std::bind( eventHandler, eventHandlerObject ) );
	}
	void			connectInterruptEventHandler( const std::function<void()>& eventHandler );
	void			disconnectInterruptEventHandler();

	template<typename T, typename Y>
	inline void		connectPingEventHandler( T eventHandler, Y* eventHandlerObject )
	{
		connectPingEventHandler( std::bind( eventHandler, eventHandlerObject, std::placeholders::_1 ) );
	}
	void			connectPingEventHandler( const std::function<void( std::string )>& eventHandler );
	void			disconnectPingEventHandler();

	template<typename T, typename Y>
	inline void		connectReadEventHandler( T eventHandler, Y* eventHandlerObject )
	{
		connectReadEventHandler( std::bind( eventHandler, eventHandlerObject, std::placeholders::_1 ) );
	}
	void			connectReadEventHandler( const std::function<void( std::string )>& eventHandler );
	void			disconnectReadEventHandler();

	template<typename T, typename Y>
	inline void		connectWriteEventHandler( T eventHandler, Y* eventHandlerObject )
	{
		connectWriteEventHandler( std::bind( eventHandler, eventHandlerObject ) );
	}
	void			connectWriteEventHandler( const std::function<void()>& eventHandler );
	void			disconnectWriteEventHandler();
protected:
	websocketpp::connection_hdl			mHandle;
	
	std::function<void()>				mConnectEventHandler;
	std::function<void()>				mDisconnectEventHandler;
	std::function<void( std::string )>	mErrorEventHandler;
	std::function<void()>				mInterruptEventHandler;
	std::function<void( std::string )>	mPingEventHandler;
	std::function<void( std::string )>	mReadEventHandler;
	std::function<void()>				mWriteEventHandler;
};
