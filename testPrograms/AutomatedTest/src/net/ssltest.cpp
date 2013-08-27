/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <conprint.h>
#include <mastring.h>
#include <MAUtil/Connection.h>
#include <Testify/testify.hpp>
#include "netbase.h"
#include "connection.h"

using namespace Testify;


/**
 * Automated SSL test case. Please feel free to adding
 * more cases to it.
 *
 * Note: This will most likely no work on j2me devices since
 *       it uses port 443 which seem to be prohibited for use.
 */
class SslTestCase : public TestCase, private NetBase
{
public:
        SslTestCase( )
        : TestCase( "SSL test case" )
        {
			addTest( bind( &SslTestCase::sslConnectTest, this ), "SSL Connect test" );
			addTest( bind( &SslTestCase::sslWriteTest, this ), "SSL Write test" );
			addTest( bind( &SslTestCase::sslReadTest, this ), "SSL Read test" );
        }

        virtual void testTearDown ( void )
        {
        	long startTime = maGetMilliSecondCount( );
        	while ( maGetMilliSecondCount()-startTime < 500 );
        }

        void sslConnectTest ( void )
        {
        	int result;
        	Connection con( "ssl://encrypted.google.com:443" );

			TESTIFY_ASSERT( con.isValid( ) == true );
			TESTIFY_ASSERT( waitForEvent( MAX_DELAY, CONNOP_CONNECT, result ) );
#ifdef VERBOSE
			printf("@@@ sslConnectTest maConnect result: %i\n", result);
#endif
			TESTIFY_ASSERT( result > 0 );
        }

        void sslWriteTest ( void )
        {
        	int result;
        	Connection con( "ssl://encrypted.google.com:443" );
        	const char *httpGetRequest = "GET / HTTP/1.1\r\n\r\n";

        	TESTIFY_ASSERT( con.isValid( ) == true );
			TESTIFY_ASSERT( waitForEvent( MAX_DELAY, CONNOP_CONNECT, result ) );
			TESTIFY_ASSERT( result > 0 );

			con.write( httpGetRequest, sizeof( httpGetRequest ) );
			TESTIFY_ASSERT( waitForEvent( MAX_DELAY, CONNOP_WRITE, result ) );
			TESTIFY_ASSERT( result > 0 );
        }

        void sslReadTest ( void )
        {
        	int result;
        	char buf[2048] = { 0 };
        	Connection con( "ssl://encrypted.google.com:443" );
			const char httpGetRequest[] = "GET / HTTP/1.0\r\nHost:encrypted.google.com\r\n\r\n";

        	TESTIFY_ASSERT( con.isValid( ) == true );
			TESTIFY_ASSERT( waitForEvent( MAX_DELAY, CONNOP_CONNECT, result ) );
			TESTIFY_ASSERT( result > 0 );

			con.write( httpGetRequest, sizeof(httpGetRequest)-1 );
			TESTIFY_ASSERT( waitForEvent( MAX_DELAY, CONNOP_WRITE, result ) );
			TESTIFY_ASSERT( result > 0 );

			con.read( buf, 2048 );
			TESTIFY_ASSERT( waitForEvent( MAX_DELAY, CONNOP_READ, result ) );
			TESTIFY_ASSERT( result > 0 );

			// Ensure that we do not print too much data.
			if (result > 100)
			{
				result = 100;
			}
			buf[result] = 0;
#ifdef VERBOSE
			printf("Reply: %s\n", buf);
#endif

			// This string should be found at beginning of the reply.
			TESTIFY_ASSERT( strstr(buf, "HTTP/1.0 200 OK") == buf );
        }
};

static TestHook hook( new SslTestCase( ), "net" );
