/*
 * Copyright (c) 2013, Wieden+Kennedy
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

#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/params/Params.h"
#include "cinder/Text.h"

#include "WebSocketServer.h"

/*
 * This application demonstrates how to create a basic
 * web socket server. This is intended to be run with
 * the ClientApp sample. Open tjis app first, then open
 * any number of clients. Use the parameter controls to
 * communicate with clients.
 */

class ServerApp : public ci::app::AppBasic 
{
public:
	void						draw();
	void						keyDown( ci::app::KeyEvent event );
	void						prepareSettings( ci::app::AppBasic::Settings* settings );
	void						setup();
	void						update();
private:
	WebSocketServer				mServer;
	void						onConnect();
	void						onDisconnect();
	void						onError( std::string err );
	void						onInterrupt();
	void						onPing( std::string msg );
	void						onRead( std::string msg );
	void						write();

	ci::Font					mFont;
	std::string					mMessage;
	ci::Vec2f					mSize;
	std::string					mText;
	std::string					mTextPrev;
	ci::gl::Texture				mTexture;

	float						mFrameRate;
	bool						mFullScreen;
	ci::params::InterfaceGlRef	mParams;
};

#include "boost/algorithm/string.hpp"
#include "cinder/Json.h"
#include "cinder/Utilities.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void ServerApp::draw()
{
	gl::setViewport( getWindowBounds() );
	gl::clear();
	gl::setMatricesWindow( getWindowSize() );

	if ( mTexture ) {
		gl::enableAlphaBlending();
		gl::draw( mTexture, getWindowCenter() - mSize * 0.5f );
	}

	mParams->draw();
}

void ServerApp::keyDown( KeyEvent event )
{
	if ( event.getCode() == KeyEvent::KEY_q ) {
		quit();
	}
}

void ServerApp::onConnect() 
{
	mText = "Connected";
}

void ServerApp::onDisconnect() 
{
	mText = "Disconnected";
}

void ServerApp::onError( string err ) 
{
	mText = "Error";
	if ( !err.empty() ) {
		mText += ": " + err;
	}
}

void ServerApp::onInterrupt() 
{
	mText = "Interrupted";
}

void ServerApp::onPing( string msg )
{
	mText = "Pinged";
	if ( !msg.empty() ) {
		mText += ": " + msg;
	}
}

void ServerApp::onRead( string msg ) 
{
	mText = "Read";
	if ( !msg.empty() ) {
		mText += ": " + msg;
	}
}

void ServerApp::prepareSettings( Settings* settings )
{
	settings->setFrameRate( 60.0f );
	settings->setResizable( false );
	settings->setWindowSize( 512, 384 );
}

void ServerApp::setup()
{
	mFrameRate	= 0.0f;
	mFullScreen	= false;
	mMessage	= "Hello, client!";
	
	gl::enable( GL_TEXTURE_2D );
	mFont		= Font( "Georgia", 80 );
	mSize		= Vec2f( getWindowSize() );
	mText		= "Listening...";
	mTextPrev	= "";

	mServer.addConnectCallback( &ServerApp::onConnect, this );
	mServer.addDisconnectCallback( &ServerApp::onDisconnect, this );
	mServer.addErrorCallback( &ServerApp::onError, this );
	mServer.addInterruptCallback( &ServerApp::onInterrupt, this );
	mServer.addPingCallback( &ServerApp::onPing, this );
	mServer.addReadCallback( &ServerApp::onRead, this );

	mParams = params::InterfaceGl::create( "SERVER", Vec2i( 200, 100 ) );
	mParams->addParam( "Frame rate", &mFrameRate, "", true );
	mParams->addParam( "Fullscreen", &mFullScreen, "key=f" );
	mParams->addParam( "Message", &mMessage );
	mParams->addButton( "Write", bind( &ServerApp::write, this ), "key=w" );
	mParams->addButton( "Quit", bind( &ServerApp::quit, this ), "key=q" );

	mServer.listen( 9002 );
}

void ServerApp::update()
{
	mFrameRate = getFrameRate();

	mServer.poll();

	if ( mFullScreen != isFullScreen() ) {
		setFullScreen( mFullScreen );
	}

	if ( mTextPrev != mText ) {
		mTextPrev = mText;
		if ( mText.empty() ) {
			mTexture.reset();
		} else {
			TextBox tbox = TextBox().alignment( TextBox::CENTER ).font( mFont ).size( Vec2i( mSize.x, TextBox::GROW ) ).text( mText );
			tbox.setColor( ColorAf( 1.0f, 0.8f, 0.75f, 1.0f ) );
			tbox.setBackgroundColor( ColorAf::black() );
			tbox.setPremultiplied( false );
			mSize.y		= tbox.measure().y;
			mTexture	= gl::Texture( tbox.render() );
		}
	}
}

void ServerApp::write()
{
	mServer.write( mMessage );
}

CINDER_APP_BASIC( ServerApp, RendererGl )
