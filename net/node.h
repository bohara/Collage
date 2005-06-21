
/* Copyright (c) 2005, Stefan Eilemann <eile@equalizergraphics.com> 
   All rights reserved. */

#ifndef EQNET_NODE_H
#define EQNET_NODE_H

#include <eq/base/base.h>

#include <eq/net/message.h>

namespace eqNet
{
    /**
     * Manages a node.
     *
     * A node represents a separate entity within a session, typically
     * a process on a PC in a cluster or on a shared-memory system. A
     * node is part of one session and 0-n networks. In order to
     * communicate with other nodes, it needs to run in at least one
     * Network. 
     *
     * Thread-safety: ?
     */
    class Node
    {
    public:
        /**
         * @name Messaging API
         *
         * The messaging API provides basic point-to-point communications
         * between nodes. Broadcast communications are handled by the Group
         * class.
         */
        //@{

        /** Messaging flags, TBD. */
#       define NON_BLOCKING       0x1
#       define USE_CLIENT_STORAGE 0x2

        /**
         * Sends a message to a node.
         *
         * The Node will automatically choose the best Network to transmit the
         * message.
         *
         * @param toNodeID the identifier of the node to send the
         *                 message to.
         * @param type the type of the message elements.
         * @param ptr the memory address of the message elements.
         * @param count the number of message elements.
         * @param flags the transmission flags.
         * @sa Network::send
         */
        static void send( const uint toNodeID, const Message::Type type, 
            const void *ptr, const uint64 count, const uint flags );

        /** 
         * Receives a message from a node.
         *
         * The Node will automatically choose the appropriate Network to
         * transmit the message.
         *
         * @param fromNodeID the identifier of the node sending the message, or
         *                   <code>NODE_ID_ANY</code>.
         * @param type the type of the message to receive, or
         *                   <code>TYPE_ID_ANY</code>.
         * @param ptr the memory address where the received message will be
         *                   stored, or <code>NULL</code> if the memory should
         *                   be allocated automatically.
         * @param count the address where to store the number of received
         *                   elements.
         * @param timeout the timeout in milliseconds before a receive is
         *                   cancelled.
         * @return the address where the received message was stored, or
         *                   <code>NULL</code> if the message was not received.
         * @sa Network::recv
         */
        static void* recv( const uint fromNodeID, const Message::Type type, 
            const void *ptr, const uint64 *count, const float timeout );
        //@}

        /** @name Internal API */
        //@{
        /** 
         * Constructs a new Node.
         * 
         * @param id the identifier of the node.
         */
        Node(const uint id);
        //@}

    protected:

        /** The identifier of this Node. */
        uint _id;

    private:
        static uint _nextNodeID;
    };
};

#endif // EQNET_NODE_H
