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

#include "WebSocketClient.h"

using namespace std;

WebSocketClient::WebSocketClient()
{
	mClient.clear_access_channels( websocketpp::log::alevel::all );
	mClient.clear_error_channels( websocketpp::log::elevel::all );
	
	mClient.init_asio();

	mClient.set_close_handler(			bind( &WebSocketClient::onClose,		this, &mClient, std::placeholders::_1 ) );
	mClient.set_fail_handler(			bind( &WebSocketClient::onFail,			this, &mClient, std::placeholders::_1 ) );
	mClient.set_http_handler(			bind( &WebSocketClient::onHttp,			this, &mClient, std::placeholders::_1 ) );
	mClient.set_interrupt_handler(		bind( &WebSocketClient::onInterrupt,	this, &mClient, std::placeholders::_1 ) );
	mClient.set_message_handler(		bind( &WebSocketClient::onMessage,		this, &mClient, std::placeholders::_1, std::placeholders::_2 ) );
	mClient.set_open_handler(			bind( &WebSocketClient::onOpen,			this, &mClient, std::placeholders::_1 ) );
	mClient.set_pong_handler(			bind( &WebSocketClient::onPong,			this, &mClient, std::placeholders::_1, std::placeholders::_2 ) );
	mClient.set_socket_init_handler(	bind( &WebSocketClient::onSocketInit,	this, &mClient, std::placeholders::_1, std::placeholders::_2 ) );
	mClient.set_tcp_post_init_handler(	bind( &WebSocketClient::onTcpPostInit,	this, &mClient, std::placeholders::_1 ) );
	mClient.set_tcp_pre_init_handler(	bind( &WebSocketClient::onTcpPreInit,	this, &mClient, std::placeholders::_1 ) );
	mClient.set_validate_handler(		bind( &WebSocketClient::onValidate,		this, &mClient, std::placeholders::_1 ) );
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
		if (mClient.stopped()){
			mClient.reset();
		}
		websocketpp::lib::error_code err;
		Client::connection_ptr conn = mClient.get_connection( uri, err );
		if ( err ) {
			if ( mFailEventHandler != nullptr ) {
				mFailEventHandler( err.message() );
			}
		} else {
			if ( conn ) {
				mClient.connect( conn );
			} else {
				if ( mFailEventHandler != nullptr ) {
					mFailEventHandler( "Unable to resolve address." );
				}
			}
		}
	} catch ( const std::exception& ex ) {
		if ( mFailEventHandler != nullptr ) {
			mFailEventHandler( ex.what() );
		}
    } catch ( websocketpp::lib::error_code err ) {
		if ( mFailEventHandler != nullptr ) {
			mFailEventHandler( err.message() );
		}
    } catch ( ... ) {
		if ( mFailEventHandler != nullptr ) {
			mFailEventHandler( "An unknown exception occurred." );
		}
    }
}

void WebSocketClient::disconnect()
{
	websocketpp::lib::error_code err;
	mClient.close( mHandle, websocketpp::close::status::going_away, "", err );
	if ( err ) {
		if ( mFailEventHandler != nullptr ) {
			mFailEventHandler( err.message() );
		}
	}
}

void WebSocketClient::ping( const string& msg )
{
	try {
		mClient.get_con_from_hdl( mHandle )->ping( msg );
	} catch( ... ) {
		if ( mFailEventHandler != nullptr ) {
			mFailEventHandler( "Ping failed." );
		}
	}
}

void WebSocketClient::poll()
{
	mClient.poll();
}

void WebSocketClient::write( const std::string& msg )
{
	if ( msg.empty() ) {
		if ( mFailEventHandler != nullptr ) {
			mFailEventHandler( "Cannot send empty message." );
		}
	} else {
		websocketpp::lib::error_code err;
		mClient.send( mHandle, msg, websocketpp::frame::opcode::TEXT, err );
		if ( err ) {
			if ( mFailEventHandler != nullptr ) {
				mFailEventHandler( err.message() );
			}
		} else {
			if ( mWriteEventHandler != nullptr ) {
				mWriteEventHandler();
			}
		}
	}
}

WebSocketClient::Client& WebSocketClient::getClient()
{
	return mClient;
}

const WebSocketClient::Client& WebSocketClient::getClient() const
{
	return mClient;
}

void WebSocketClient::onClose( Client* client, websocketpp::connection_hdl handle ) 
{
	if ( mCloseEventHandler != nullptr ) {
		mCloseEventHandler();
	}
}

void WebSocketClient::onFail( Client* client, websocketpp::connection_hdl handle ) 
{
	mHandle = handle;
	if ( mFailEventHandler != nullptr ) {
		mFailEventHandler( "Transfer failed." );
	}
}

void WebSocketClient::onHttp( Client* client, websocketpp::connection_hdl handle )
{
	mHandle = handle;
	if ( mHttpEventHandler != nullptr ) {
		mHttpEventHandler();
	}
}

void WebSocketClient::onInterrupt( Client* client, websocketpp::connection_hdl handle ) 
{
	mHandle = handle;
	if ( mInterruptEventHandler != nullptr ) {
		mInterruptEventHandler();
	}
}

void WebSocketClient::onMessage( Client* client, websocketpp::connection_hdl handle, MessageRef msg )
{
	mHandle = handle;
	if ( mMessageEventHandler != nullptr ) {
		mMessageEventHandler( msg->get_payload() );
	}
}

void WebSocketClient::onOpen( Client* client, websocketpp::connection_hdl handle )
{
	mHandle = handle;
	if ( mOpenEventHandler != nullptr ) {
		mOpenEventHandler();
	}
}

void WebSocketClient::onPong( Client* client, websocketpp::connection_hdl handle, string msg )
{
	mHandle = handle;
	if ( mPingEventHandler != nullptr ) {
		mPingEventHandler( msg );
	}
}

void WebSocketClient::onSocketInit( Client* client, websocketpp::connection_hdl handle, asio::ip::tcp::socket& socket )
{
	mHandle = handle;
	mSocket = &socket;
	if ( mSocketInitEventHandler != nullptr ) {
		mSocketInitEventHandler();
	}
}

void WebSocketClient::onTcpPostInit( Client* client, websocketpp::connection_hdl handle )
{
	mHandle = handle;
	if ( mTcpPostInitEventHandler != nullptr ) {
		mTcpPostInitEventHandler();
	}
}
 
void WebSocketClient::onTcpPreInit( Client* client, websocketpp::connection_hdl handle )
{
	mHandle = handle;
	if ( mTcpPreInitEventHandler != nullptr ) {
		mTcpPreInitEventHandler();
	}
}
 
bool WebSocketClient::onValidate( Client* client, websocketpp::connection_hdl handle )
{
	mHandle = handle;
	if ( mValidateEventHandler != nullptr ) {
		mValidateEventHandler();
	}
	return true;
}
