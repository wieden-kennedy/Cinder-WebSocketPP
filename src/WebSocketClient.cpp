#include "WebSocketClient.h"

using namespace ci;
using namespace std;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

WebSocketClient::WebSocketClient()
{
	mClient.clear_access_channels( websocketpp::log::alevel::all );
	mClient.clear_error_channels( websocketpp::log::elevel::all );
	
	mClient.init_asio();

	mClient.set_close_handler( websocketpp::lib::bind( &WebSocketClient::onDisconnect, this, &mClient, ::_1 ) );
	mClient.set_interrupt_handler( websocketpp::lib::bind( &WebSocketClient::onInterrupt, this, &mClient, ::_1 ) );
	mClient.set_open_handler( websocketpp::lib::bind( &WebSocketClient::onConnect, this, &mClient, ::_1 ) );
	mClient.set_pong_handler( websocketpp::lib::bind( &WebSocketClient::onPing, this, &mClient, ::_1 ) );
	mClient.set_message_handler( websocketpp::lib::bind( &WebSocketClient::onRead, this, &mClient, ::_1, ::_2 ) );
	mClient.set_fail_handler( websocketpp::lib::bind( &WebSocketClient::onFail, this, &mClient, ::_1 ) );
}

WebSocketClient::~WebSocketClient()
{
	if ( !mClient.stopped() ) {
		disconnect();
		mClient.stop();
	}
}

void WebSocketClient::connect( const std::string& uri )
{
	try {
		websocketpp::lib::error_code err;
		Client::connection_ptr conn = mClient.get_connection( uri, err );
		if ( err ) {
			mSignalError( err.message() );
		} else {
			if ( conn ) {
				mClient.connect( conn );
			} else {
				mSignalError( "Unable to resolve address." );
			}
		}
	} catch ( const std::exception& ex ) {
		mSignalError( ex.what() );
    } catch ( websocketpp::lib::error_code err ) {
        mSignalError( err.message() );
    } catch ( ... ) {
        mSignalError( "An unknown exception occurred." );
    }
}

void WebSocketClient::disconnect()
{
	websocketpp::lib::error_code err;
	mClient.close( mHandle, websocketpp::close::status::going_away, "", err );
	if ( err ) {
		mSignalError( err.message() );
	}
}

void WebSocketClient::ping()
{
	websocketpp::lib::error_code err;
	mClient.send( mHandle, 0, 0, websocketpp::frame::opcode::PING, err );
	if ( err ) {
		mSignalError( err.message() );
	}
}

void WebSocketClient::poll()
{
	mClient.poll();
}

void WebSocketClient::write( const std::string& msg )
{
	if ( msg.empty() ) {
		mSignalError( "Cannot send empty message." );
	} else {
		websocketpp::lib::error_code err;
		mClient.send( mHandle, msg, websocketpp::frame::opcode::TEXT, err );
		if ( err ) {
			mSignalError( err.message() );
		} else {
			mSignalWrite();
		}
	}
}

void WebSocketClient::removeCallback( uint32_t id )
{
	if ( mCallbacks.find( id ) != mCallbacks.end() ) {
		mCallbacks.find( id )->second->disconnect();
		mCallbacks.erase( id );
	}
}

void WebSocketClient::onConnect( Client* client, websocketpp::connection_hdl handle ) 
{
	mHandle = handle;
	mSignalConnect();
}

void WebSocketClient::onDisconnect( Client* client, websocketpp::connection_hdl handle ) 
{
	mSignalDisconnect();
}

void WebSocketClient::onFail( Client* client, websocketpp::connection_hdl handle ) 
{
	mHandle = handle;
	mSignalError( "Transfer failed." );
}

void WebSocketClient::onInterrupt( Client* client, websocketpp::connection_hdl handle ) 
{
	mHandle = handle;
	mSignalInterrupt();
}

void WebSocketClient::onPing( Client* client, websocketpp::connection_hdl handle ) 
{
	mHandle = handle;
	mSignalPing();
}

void WebSocketClient::onRead( Client* client, websocketpp::connection_hdl handle, MessageRef msg ) 
{
	mHandle = handle;
	mSignalRead( msg->get_payload() );
}
