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

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"
#include "cinder/Text.h"

#include "WebSocketClient.h"

/*
* This application demonstrates how to create a basic
* web socket client. This is intended to be run with
* the ServerApp sample. Open ServerApp first, then open
* any number of clients. Use the parameter controls to
* communicate with the server.
*/

class ClientApp : public ci::app::App
{
public:
	ClientApp();

	void						draw() override;
	void						update() override;
private:
	// Web socket client
	void						connect();
	void						disconnect();
	WebSocketClient				mClient;
	void						write();

	// Text
	ci::Font					mFont;
	std::string					mMessage;
	bool						mPing;
	double						mPingTime;
	ci::vec2					mSize;
	std::string					mText;
	std::string					mTextPrev;
	ci::gl::Texture2dRef		mTexture;

	// Parameters
	float						mFrameRate;
	bool						mFullScreen;
	ci::params::InterfaceGlRef	mParams;
};

#include "boost/algorithm/string.hpp"
#include "cinder/app/RendererGl.h"
#include "cinder/Utilities.h"

using namespace ci;
using namespace ci::app;
using namespace std;

ClientApp::ClientApp()
{
	mFont		= Font( "Georgia", 80.0f );
	mSize		= vec2( getWindowSize() );
	mText		= "";
	mTextPrev	= mText;

	// Connect event handlers
	mClient.connectConnectEventHandler( [ & ]()
	{
		mText = "Connected";
	} );
	mClient.connectDisconnectEventHandler( [ & ]()
	{
		mText = "Disconnected";
	} );
	mClient.connectErrorEventHandler( [ & ]( string err )
	{
		mText = "Error";
		if ( !err.empty() ) {
			mText += ": " + err;
		}
	} );
	mClient.connectInterruptEventHandler( [ & ]()
	{
		mText = "Interrupted";
	} );
	mClient.connectPingEventHandler( [ & ]( string msg )
	{
		mText = "Ponged";
		if ( msg.empty() ) {
			mText += ": " + msg;
		}
	} );
	mClient.connectReadEventHandler( [ & ]( string msg )
	{
		mText = "Read";
		if ( !msg.empty() ) {
			mText += ": " + msg;
		}
	} );

	mFrameRate	= 0.0f;
	mFullScreen	= false;
	mMessage	= "Hello, server!";
	mPing		= false;
	mPingTime	= getElapsedSeconds();

	mParams = params::InterfaceGl::create( "CLIENT", ivec2( 200, 160 ) );
	mParams->addParam( "Frame rate",	&mFrameRate, "", true );
	mParams->addParam( "Fullscreen",	&mFullScreen ).key( "f" );
	mParams->addParam( "Ping enabled",	&mPing ).key( "p" );
	mParams->addParam( "Message",		&mMessage );
	mParams->addButton( "Connect",		[ & ]() { connect(); },		"key=c" );
	mParams->addButton( "Disconnect",	[ & ]() { disconnect(); },	"key=d" );
	mParams->addButton( "Write",		[ & ]() { write(); },		"key=w" );
	mParams->addButton( "Quit",			[ & ]() { quit(); },		"key=q" );

	connect();
}

void ClientApp::connect()
{
	mClient.connect( "ws://10.0.209.118:9002" );
}

void ClientApp::disconnect()
{
	mClient.disconnect();
}

void ClientApp::draw()
{
	const gl::ScopedViewport scopedViewport( ivec2( 0 ), getWindowSize() );
	const gl::ScopedMatrices scopedMatrices;
	gl::clear();
	gl::setMatricesWindow( getWindowSize() );

	if ( mTexture ) {
		gl::enableAlphaBlending();
		gl::draw( mTexture, getWindowCenter() - mSize * 0.5f );
	}

	mParams->draw();
}

void ClientApp::update()
{
	mClient.poll();

	mFrameRate = getFrameRate();

	if ( mFullScreen != isFullScreen() ) {
		setFullScreen( mFullScreen );
	}

	double e = getElapsedSeconds();
	if ( mPing && e - mPingTime > 3.0 ) {
		mClient.ping();
		mPingTime = e;
	}

	if ( mTextPrev != mText ) {
		mTextPrev = mText;
		if ( mText.empty() ) {
			mTexture.reset();
		} else {
			TextBox tbox = TextBox()
				.alignment( TextBox::CENTER )
				.backgroundColor( ColorAf::black() )
				.color( ColorAf( 1.0f, 0.8f, 0.75f, 1.0f ) )
				.font( mFont )
				.premultiplied( false )
				.size( ivec2( mSize.x, TextBox::GROW ) )
				.text( mText );
			mSize.y = tbox.measure().y;
			mTexture = gl::Texture2d::create( tbox.render() );
		}
	}
}

void ClientApp::write()
{
	mClient.write( mMessage );
}

CINDER_APP( ClientApp, RendererGl, []( App::Settings* settings ) 
{
	settings->disableFrameRate();
	settings->setResizable( false );
	settings->setWindowSize( 512, 384  );
} )
 