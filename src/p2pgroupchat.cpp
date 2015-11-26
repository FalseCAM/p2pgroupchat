#include <stdint.h>
#include <stdio.h>
#include <fstream>

#include <p2p/Concurrency.hpp>
#include <p2p/Crypt/AutoSeededRandomPool.hpp>
#include <p2p/Event/Signal.hpp>
#include <p2p/Event/Wait.hpp>
#include <p2p/Kademlia.hpp>
#include <p2p/Root.hpp>
#include <p2p/Transport.hpp>
#include <p2p/UDP.hpp>
#include <p2p/TCP.hpp>

#include "clientidentitydelegate.h"
#include "clientthread.h"
#include "dhtserverdelegate.h"
#include "eventthread.h"
#include "kademliarpcsocket.h"
#include "nodedetectdelegate.h"
#include "tcpthread.h"
#include "container.h"
#include "querynodethread.h"
#include "crypto.h"

#include "Logger.hpp"

using namespace p2p;


Root::NodeId processTextId(const std::string& textId) {
	assert(textId.size() <= 64);
	assert((textId.size() % 2) == 0);
	
	Root::NodeId nodeId;
	
	for (size_t i = 0; i < (textId.size() / 2); i++) {
		const auto subString = textId.substr(i * 2, 2);
		const auto value = strtol(subString.c_str(), nullptr, 16);
		nodeId[i] = value;
	}
	
	return nodeId;
}

std::vector<std::string> parse(const std::string& args);


void sendTcpMessage(TCP::Endpoint &endpoint, std::string &message) {

    TCP::Stream tcpStream;

    if (!tcpStream.connect(endpoint)) {
        std::cout << "---Failed to connect" << std::endl;
        return;
    }

    TextIOStream stream(tcpStream);

    stream << message;

    return;
}



int main(int argc, char** argv) {
	Logger logger;
	
	if (argc != 2) {
        logger.log("Usage: p2pgroupchat [my port]");
		return -1;
	}
	
	const auto myPort = atoi(argv[1]);

    const std::string privatekey = "privatekey";
    const std::string publickey = "publickey.pub";

    Crypto crypto;
    if(!crypto.load(privatekey, publickey))
    {
        crypto.generateNew();
        crypto.save(privatekey, publickey);
    }

	UDP::Socket udpSocket(myPort);
	
	// Generate a private key for our node.
	Crypt::AutoSeededRandomPool rand;
	Crypt::ECDSA::PrivateKey privateKey(rand, Crypt::ECDSA::brainpoolP256r1);


	Root::NodeDatabase nodeDatabase;
	Root::PrivateIdentity privateIdentity(rand, privateKey);
	
	logger.log("========");
	logger.log(STR("======== My id is '%s'.", privateIdentity.id().hexString().c_str()));
	logger.log("========");
	
	// Send/receive data on appropriate transport.
	Root::TransportSocket transportSocket(udpSocket);
	
	// Serialize/unserialize packets.
	Root::PacketSocket packetSocket(transportSocket);
	
	// Sign all outgoing packets and verify incoming packets.
	ClientIdentityDelegate identityDelegate(nodeDatabase, privateIdentity);
	Root::AuthenticatedSocket authenticatedSocket(identityDelegate, packetSocket);
	
	MessageQueue<Root::NodePair> messageQueue;
	
	NodeDetectDelegate nodeDetectDelegate(nodeDatabase, messageQueue);
	Root::NodeDetectSocket nodeDetectSocket(authenticatedSocket, nodeDetectDelegate);
	
	// Multiplex messages for core and DHT services.
	MultiplexHost<Root::NodePair, Root::Message> multiplexHost(nodeDetectSocket);
	MultiplexClient<Root::NodePair, Root::Message> coreSocket(multiplexHost);
	MultiplexClient<Root::NodePair, Root::Message> dhtMultiplexSocket(multiplexHost);
	
	Root::EndpointMapSocket dhtSocket(dhtMultiplexSocket, nodeDatabase);
	
	// Routine ID generator.
	Root::RoutineIdGenerator routineIdGenerator;
	
	Root::Core::Service coreService(coreSocket, routineIdGenerator);
	coreService.addNetwork("p2p.rootdht");
	
	p2p::Kademlia::BucketSet<Root::NodeId> dhtBucketSet(privateIdentity.id());
	
	DHTServerDelegate dhtDelegate(dhtBucketSet, nodeDatabase);
	Root::DHT::Service dhtService(dhtSocket, routineIdGenerator, dhtDelegate);
	
	EventThread eventThreadRunnable(coreService, dhtService);
	Thread eventThread(eventThreadRunnable);
	
	QueryNodeThread queryNodeThreadRunnable(logger, privateIdentity.id(), dhtBucketSet, coreService, dhtService, messageQueue);
    Thread queryNodeThread(queryNodeThreadRunnable);

    TCP::Acceptor acceptor(myPort);

    TcpThread tcpThreadRunnable(&acceptor, crypto);
    Thread tcpThread(tcpThreadRunnable);

	
	while (true) {
		logger.showPrompt();
		
		const auto rawCommand = readLine();
		if (rawCommand.empty()) break;
		
		const auto commandArgs = parse(rawCommand);
		if (commandArgs.empty()) {
			logger.log("error: Command not specified.");
			continue;
		}
		
		const auto& command = commandArgs.at(0);
		
        if (command == "/add") {
            if (commandArgs.size() != 3) {
				logger.log(STR("%s: incorrect number of arguments", command.c_str()));
				continue;
			}

            const std::string nodeIp = commandArgs.at(1);

            const auto nodePort = atoi(commandArgs.at(2).c_str());
            logger.log(STR("%s: querying for node at port '%d'...", command.c_str(), nodePort));

            const auto peerEndpoint = UDP::Endpoint(IP::Address(IP::V4Address::FromImpl(boost::asio::ip::address_v4::from_string(nodeIp))), nodePort);
			
			const auto peerId = coreService.identify(peerEndpoint).get();
			
			logger.log(STR("%s: node's id is '%s'.", command.c_str(), peerId.hexString().c_str()));
			
			messageQueue.send(Root::NodePair(peerId, peerEndpoint));
			
			logger.log(STR("%s: submitted node to be queried", command.c_str()));
		} else if (command == "find") {
			if (commandArgs.size() != 2) {
				logger.log(STR("%s: incorrect number of arguments", command.c_str()));
				continue;
			}
			
			const auto& searchIdText = commandArgs.at(1);
			if (searchIdText.size() > 64) {
				logger.log(STR("%s: search id '%s' is longer than 64 characters", command.c_str(), searchIdText.c_str()));
				continue;
			}
			
			if ((searchIdText.size() % 2) == 1) {
				logger.log(STR("%s: search id '%s' is not a multiple of 2 characters in length", command.c_str(), searchIdText.c_str()));
				continue;
			}
			
			const auto searchId = processTextId(searchIdText);
			
			logger.log(STR("%s: searching for ID '%s'...", command.c_str(), searchId.hexString().c_str()));
			
			KademliaRPCSocket kademliaRPCSocket(dhtService, logger);
			const auto nearestList = p2p::Kademlia::iterativeSearch<Root::NodeId, 2>(dhtBucketSet, kademliaRPCSocket, searchId);
			
			logger.log(STR("%s: found %llu nearest nodes:", command.c_str(), (unsigned long long) nearestList.size()));
			
			for (const auto& resultNodeId: nearestList) {
				logger.log(STR("%s:     Node '%s'", command.c_str(), resultNodeId.hexString().c_str()));
			}
		} else if (command == "send") {
			if (commandArgs.size() != 2) {
				logger.log(STR("%s: incorrect number of arguments", command.c_str()));
				continue;
			}
			
			const auto& searchIdText = commandArgs.at(1);

						

			if (searchIdText.size() > 64) {
				logger.log(STR("%s: search id '%s' is longer than 64 characters", command.c_str(), searchIdText.c_str()));
				continue;
			}
			
			if ((searchIdText.size() % 2) == 1) {
				logger.log(STR("%s: search id '%s' is not a multiple of 2 characters in length", command.c_str(), searchIdText.c_str()));
				continue;
			}
			
			const auto searchId = processTextId(searchIdText);
			
			logger.log(STR("%s: searching for ID '%s'...", command.c_str(), searchId.hexString().c_str()));
			
			KademliaRPCSocket kademliaRPCSocket(dhtService, logger);
			const auto nearestList = p2p::Kademlia::iterativeSearch<Root::NodeId, 2>(dhtBucketSet, kademliaRPCSocket, searchId);
			
			logger.log(STR("%s: found %llu nearest nodes:", command.c_str(), (unsigned long long) nearestList.size()));

            for (const auto& resultNodeId: nearestList) {

				logger.log(STR("%s:     Node '%s'", command.c_str(), resultNodeId.hexString().c_str()));
			}
		} else if (command == "ls" || command == "list") {
			logger.log(STR("%s: found %llu nodes:", command.c_str(), (unsigned long long) nodeDatabase.map().size()));
			for (const auto& nodeEntryPair: nodeDatabase.map()) {
				const auto& nodeEntry = nodeEntryPair.second;
				logger.log(STR("%s:     Node '%s' ->", command.c_str(), nodeEntry.identity.id().hexString().c_str()));
                for (const auto& endpoint: nodeEntry.endpointSet) {
					logger.log(STR("%s:         %s", command.c_str(), endpoint.toString().c_str()));
				}
			}
		} else if (command == "q" || command == "quit") {
			logger.log(STR("%s: exiting...", command.c_str()));
			break;
		} else if (command == "h" || command == "help") {
			logger.log(STR("%s: available commands:", command.c_str()));
            logger.log(STR("%s:     /add (a) [ip] [port]: Add a node by its UDP port.", command.c_str()));
			logger.log(STR("%s:     help (h): Display this help text.", command.c_str()));
			logger.log(STR("%s:     quit (q): Exit application.", command.c_str()));
        } else {
            std::string message = crypto.encrypt(rawCommand);
            for (const auto& nodeEntryPair: nodeDatabase.map()) {
                const auto& nodeEntry = nodeEntryPair.second;
                for (const auto& endpoint: nodeEntry.endpointSet) {
                    p2p::TCP::Endpoint etcp(endpoint.udpEndpoint.address, endpoint.udpEndpoint.port);
                    sendTcpMessage(etcp, message);
                }
            }
		}
	}
	
	return 0;
}

