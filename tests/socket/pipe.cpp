
#include <connection.h>

#include <alloca.h>
#include <iostream>

using namespace eqNet;
using namespace eqNetInternal;
using namespace std;

extern "C" int testPipeServer( Connection* connection )
{
    char c;
    while( connection->recv( &c, 1 ))
    {
        fprintf( stderr, "Server recv: '%c'\n", c );
        connection->send( &c, 1 );
    }

    return EXIT_SUCCESS;
}

int main( int argc, char **argv )
{
    Connection *connection = Connection::create(Network::PROTO_PIPE);

    ConnectionDescription connDesc;
    connDesc.PIPE.entryFunc = "testPipeServer";

    connection->connect(connDesc);

    const char message[] = "buh!";
    int nChars = strlen( message ) + 1;
    const char *response = (const char*)alloca( nChars );

    connection->send( message, nChars );
    connection->recv( response, nChars );
    fprintf( stderr, "%s\n", response );

    connection->close();

    return EXIT_SUCCESS;
}
