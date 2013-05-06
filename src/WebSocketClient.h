#pragma once

#include "boost/signals2.hpp"
#include "cinder/Buffer.h"

#include "websocketpp/config/asio_no_tls_client.hpp"
#include "websocketpp/client.hpp"

class WebSocketClient
{
public:
	WebSocketClient();
	~WebSocketClient();

	void						connect( const std::string& uri );
	void						disconnect();
	void						ping();
	void						poll();
	void						write( const std::string& msg );

	template<typename T, typename Y>
	inline uint32_t				addConnectCallback( T callback, Y *callbackObject )
	{
		uint32_t id = mCallbacks.empty() ? 0 : mCallbacks.rbegin()->first + 1;
		mCallbacks.insert( std::make_pair( id, CallbackRef( new Callback( mSignalConnect.connect( std::bind( callback, callbackObject ) ) ) ) ) );
		return id;
	}

	template<typename T, typename Y>
	inline uint32_t				addDisconnectCallback( T callback, Y *callbackObject )
	{
		uint32_t id = mCallbacks.empty() ? 0 : mCallbacks.rbegin()->first + 1;
		mCallbacks.insert( std::make_pair( id, CallbackRef( new Callback( mSignalDisconnect.connect( std::bind( callback, callbackObject ) ) ) ) ) );
		return id;
	}

	template<typename T, typename Y>
	inline uint32_t				addErrorCallback( T callback, Y *callbackObject )
	{
		uint32_t id = mCallbacks.empty() ? 0 : mCallbacks.rbegin()->first + 1;
		mCallbacks.insert( std::make_pair( id, CallbackRef( new Callback( mSignalError.connect( std::bind( callback, callbackObject, std::placeholders::_1 ) ) ) ) ) );
		return id;
	}

	template<typename T, typename Y>
	inline uint32_t				addInterruptCallback( T callback, Y *callbackObject )
	{
		uint32_t id = mCallbacks.empty() ? 0 : mCallbacks.rbegin()->first + 1;
		mCallbacks.insert( std::make_pair( id, CallbackRef( new Callback( mSignalInterrupt.connect( std::bind( callback, callbackObject ) ) ) ) ) );
		return id;
	}

	template<typename T, typename Y>
	inline uint32_t				addPingCallback( T callback, Y *callbackObject )
	{
		uint32_t id = mCallbacks.empty() ? 0 : mCallbacks.rbegin()->first + 1;
		mCallbacks.insert( std::make_pair( id, CallbackRef( new Callback( mSignalPing.connect( std::bind( callback, callbackObject ) ) ) ) ) );
		return id;
	}

	template<typename T, typename Y>
	inline uint32_t				addReadCallback( T callback, Y *callbackObject )
	{
		uint32_t id = mCallbacks.empty() ? 0 : mCallbacks.rbegin()->first + 1;
		mCallbacks.insert( std::make_pair( id, CallbackRef( new Callback( mSignalRead.connect( std::bind( callback, callbackObject, std::placeholders::_1 ) ) ) ) ) );
		return id;
	}

	void						removeCallback( uint32_t id );
private:
	typedef websocketpp::client<websocketpp::config::asio_client>	Client;
	typedef websocketpp::config::asio_client::message_type::ptr		MessageRef;

	typedef boost::signals2::connection		Callback;
	typedef std::shared_ptr<Callback>		CallbackRef;
	typedef std::map<uint32_t, CallbackRef>	CallbackList;

	void						onConnect( Client* client, websocketpp::connection_hdl handle );
	void						onDisconnect( Client* client, websocketpp::connection_hdl handle );
	void						onFail( Client* client, websocketpp::connection_hdl handle );
	void						onInterrupt( Client* client, websocketpp::connection_hdl handle );
	void						onPing( Client* client, websocketpp::connection_hdl handle );
	void						onRead( Client* client, websocketpp::connection_hdl handle, 
										MessageRef msg );

	Client						mClient;
	websocketpp::connection_hdl	mHandle;

	CallbackList									mCallbacks;
	boost::signals2::signal<void ()>				mSignalConnect;
	boost::signals2::signal<void ()>				mSignalDisconnect;
	boost::signals2::signal<void ( std::string )>	mSignalError;
	boost::signals2::signal<void ()>				mSignalInterrupt;
	boost::signals2::signal<void ()>				mSignalPing;
	boost::signals2::signal<void ( std::string )>	mSignalRead;
	boost::signals2::signal<void ()>				mSignalWrite;
};
