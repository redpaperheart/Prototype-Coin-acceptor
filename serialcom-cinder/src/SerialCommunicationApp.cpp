#include "cinder/app/AppBasic.h"
#include "cinder/Serial.h"
#include "cinder/Text.h"
#include "cinder/gl/Texture.h"
#include "OscSender.h"
#include "cinder/System.h"

#include <sstream>

using namespace ci;
using namespace ci::app;
using namespace std;


class SerialCommunicationApp : public AppBasic {
 public:
	void draw();
	void update();
	void setup();
	void mouseDown(MouseEvent event);
    void keyDown(KeyEvent event);
	bool serialInitiallized();
	bool		 bTextureComplete;
    
	Serial serial;
    
    osc::Sender sender;
    
    std::string host;
	int 		port;
    std::string lastString;
    gl::Texture	mTexture;
    char contact;
};

void SerialCommunicationApp::setup()
{

	lastString = "";
    bTextureComplete = false;
    
	//SERIAL SETUP
	const vector<Serial::Device> &devices( Serial::getDevices() );
	for( vector<Serial::Device>::const_iterator deviceIt = devices.begin(); deviceIt != devices.end(); ++deviceIt ) {
		console() << "Device: " << deviceIt->getName() << endl;
	}
	
	try {
		Serial::Device dev = Serial::findDeviceByNameContains("tty.usbmodem");
		serial = Serial( dev, 9600);
	}
	catch( ... ) {
		console() << "There was an error initializing the serial device!" << std::endl;
		exit( -1 );
	}
	contact = 0;
	serial.flush();
    
    //OSC SETUP
    port = 12346;
    host = "127.0.0.1";
    sender.setup( host, port, true );

    
    
    
}

bool SerialCommunicationApp::serialInitiallized(){
    //SERIAL HANDSHAKE WITH ARDUINO
    
	if (contact != '*'){
		contact = (char) serial.readByte();
        return false;
	}else{
        serial.writeByte('0');
        return true;
    }
    
}


void SerialCommunicationApp::update()
{
	if (serialInitiallized()){
        if(serial.getNumBytesAvailable() > 0){
            console() << "Bytes available: " << serial.getNumBytesAvailable() << std::endl;
            try{
                lastString = serial.readStringUntil('\n');
            } catch(SerialTimeoutExc e) {
                console() << "timeout" << endl;
            }
            console() << lastString << endl;
            int16_t temp = lastString[0];
            
            //OSC MESSAGE
            osc::Message message;
            message.addIntArg(temp);
            message.setAddress("coinTrigger");
            message.setRemoteEndpoint(host, port);
            sender.sendMessage(message);
           
            TextLayout simple;
            simple.setFont( Font( "Arial Black", 54 ) );
            simple.setColor( Color( .7, .7, .2 ) );
            simple.addLine( lastString );
            simple.setLeadingOffset( 0 );
            mTexture = gl::Texture( simple.render( true, false ) );
            bTextureComplete = true;
            serial.flush();
        }
    }
    serial.flush();
}

void SerialCommunicationApp::keyDown(KeyEvent event)
{
    //DEBUG 
    int16_t temp = 0;

    switch (event.getChar()) {
        case '1':
            temp = 49;
            break;
        case '2':
            temp = 50;
            break;
        case '3':
            temp = 51;
            break;
    }
    
    if (temp > 48 && temp < 52) {
        osc::Message message;
        //message.addStringArg(lastString);
        message.addIntArg(temp);
        message.setAddress("coinTrigger");
        message.setRemoteEndpoint(host, port);
        sender.sendMessage(message);
        TextLayout simple;
        simple.setFont( Font( "Arial Black", 54 ) );
        simple.setColor( Color( .7, .7, .2 ) );
        simple.addLine( to_string(event.getChar()) );
        simple.setLeadingOffset( 0 );
        mTexture = gl::Texture( simple.render( true, false ) );
        bTextureComplete = true;
    }
}

void SerialCommunicationApp::mouseDown(MouseEvent event){
}


void SerialCommunicationApp::draw()
{
    gl::clear(Color(0,0,0));
	gl::enableAlphaBlending( false );
	
	if(bTextureComplete){
		glColor3f( 1.0f, 1.0f, 1.0f );
		gl::draw( mTexture, Vec2f( 10, 10 ) );
	}
	
	
}

CINDER_APP_BASIC( SerialCommunicationApp, RendererGl )
