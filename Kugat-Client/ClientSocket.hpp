#pragma once

#include <SFML/Network.hpp>
#include <Common.hpp>

#include <Enlivengine/System/Log.hpp>

class ClientSocket
{
public:
	ClientSocket()
		: mSocket()
		, mServerAddress()
		, mServerPort()
		, mRunning(false)
	{
	}

	bool Start(const sf::IpAddress& address, en::U16 port)
	{
		LogInfo(en::LogChannel::All, 5, "Starting...%s", "");
		mSocket.setBlocking(true);
		if (mSocket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
		{
			LogError(en::LogChannel::All, 5, "Can't find an available port%s", "");
			return false;
		}
		mSocket.setBlocking(false);
		mServerAddress = address;
		mServerPort = port;
		mRunning = true;
		LogInfo(en::LogChannel::All, 5, "Started on port %d", mSocket.getLocalPort());
		return true;
	}

	void Stop()
	{
		LogInfo(en::LogChannel::All, 5, "Stopping...%s", "");
		mSocket.unbind();
		mRunning = false;
		LogInfo(en::LogChannel::All, 5, "Stopped%s", "");
	}

	void SetBlocking(bool blocking) { mSocket.setBlocking(blocking); }
	bool IsBlocking() const { return mSocket.isBlocking(); }

	void SendPacket(sf::Packet& packet) 
	{ 
		mSocket.send(packet, mServerAddress, mServerPort); 
	}

	bool PollPacket(sf::Packet& packet)
	{
		static sf::IpAddress remoteAddress;
		static en::U16 remotePort;
		while (mSocket.receive(packet, remoteAddress, remotePort) == sf::Socket::Done)
		{
			// Someone that is not the server is sending us packet here
			if (remotePort != mServerPort || remoteAddress != mServerAddress)
			{
				// Ignore them for now
				continue;
			}

			return true;
		}
		return false;
	}

	const sf::IpAddress& GetServerAddress() const { return mServerAddress; }
	void SetServerAddress(const sf::IpAddress& serverAddress) { mServerAddress = serverAddress; }
	en::U16 GetServerPort() const { return mServerPort; }
	void SetServerPort(en::U16 serverPort) { mServerPort = serverPort; }

	bool IsRunning() const { return mRunning; }

private:
	sf::UdpSocket mSocket;
	sf::IpAddress mServerAddress;
	en::U16 mServerPort;
	bool mRunning;
};