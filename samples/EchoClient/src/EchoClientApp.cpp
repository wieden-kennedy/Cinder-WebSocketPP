#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/params/Params.h"
#include "cinder/Text.h"

#include "WebSocketClient.h"

class EchoClientApp : public ci::app::AppBasic 
{
public:
	void				draw();
	void				keyDown( ci::app::KeyEvent event );
	void				prepareSettings( ci::app::AppBasic::Settings* settings );
	void				setup();
	void				update();
private:
	void				connect();
	void				disconnect();
	WebSocketClient		mClient;
	void				onConnect();
	void				onDisconnect();
	void				onError( std::string msg );
	void				onInterrupt();
	void				onPing();
	void				onRead( std::string msg );
	void				write();

	ci::Font			mFont;
	std::string			mMessage;
	bool				mPing;
	double				mPingTime;
	ci::Vec2f			mSize;
	std::string			mText;
	std::string			mTextPrev;
	ci::gl::Texture		mTexture;

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

void EchoClientApp::connect()
{
	mClient.connect( "ws://echo.websocket.org" );
}

void EchoClientApp::disconnect()
{
	mClient.disconnect();
}

void EchoClientApp::draw()
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

void EchoClientApp::keyDown( KeyEvent event )
{
	if ( event.getCode() == KeyEvent::KEY_q ) {
		quit();
	}
}

void EchoClientApp::onConnect() 
{
	mText = "Connected";
}

void EchoClientApp::onDisconnect() 
{
	mText = "Disconnected";
}

void EchoClientApp::onError( string err ) 
{
	mText = "Error: " + err;
}

void EchoClientApp::onInterrupt() 
{
	mText = "Interrupted";
}

void EchoClientApp::onPing() 
{
	mText = "Pinged";
}

void EchoClientApp::onRead( string msg ) 
{
	mText = "Read: " + msg;
}

void EchoClientApp::prepareSettings( Settings* settings )
{
	settings->setFrameRate( 60.0f );
	settings->setResizable( false );
	settings->setWindowSize( 1024, 768 );
}

void EchoClientApp::setup()
{
	gl::enable( GL_TEXTURE_2D );
	mFont = Font( "Georgia", 160 );
	mSize = Vec2f( getWindowSize() );

	mClient.addConnectCallback( &EchoClientApp::onConnect, this );
	mClient.addDisconnectCallback( &EchoClientApp::onDisconnect, this );
	mClient.addErrorCallback( &EchoClientApp::onError, this );
	mClient.addInterruptCallback( &EchoClientApp::onInterrupt, this );
	mClient.addPingCallback( &EchoClientApp::onPing, this );
	mClient.addReadCallback( &EchoClientApp::onRead, this );

	mFrameRate	= 0.0f;
	mFullScreen	= false;
	mMessage	= "echo";
	mPing		= false;
	mPingTime	= getElapsedSeconds();

	mParams = params::InterfaceGl::create( "PARAMS", Vec2i( 200, 200 ) );
	mParams->addParam( "Frame rate", &mFrameRate, "", true );
	mParams->addParam( "Fullscreen", &mFullScreen, "key=f" );
	mParams->addParam( "Ping enabled", &mPing, "key=p" );
	mParams->addParam( "Message", &mMessage );
	mParams->addButton( "Connect", bind( &EchoClientApp::connect, this ), "key=c" );
	mParams->addButton( "Disconnect", bind( &EchoClientApp::disconnect, this ), "key=d" );
	mParams->addButton( "Write", bind( &EchoClientApp::write, this ), "key=w" );
	mParams->addButton( "Quit", bind( &EchoClientApp::quit, this ), "key=q" );

	connect();
}

void EchoClientApp::update()
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
			TextBox tbox = TextBox().alignment( TextBox::CENTER ).font( mFont ).size( Vec2i( mSize.x, TextBox::GROW ) ).text( mText );
			tbox.setColor( ColorAf( 1.0f, 0.8f, 0.75f, 1.0f ) );
			tbox.setBackgroundColor( ColorAf::black() );
			tbox.setPremultiplied( false );
			mSize.y		= tbox.measure().y;
			mTexture	= gl::Texture( tbox.render() );
		}
	}
}

void EchoClientApp::write()
{
	mClient.write( mMessage );
}

CINDER_APP_BASIC( EchoClientApp, RendererGl )
