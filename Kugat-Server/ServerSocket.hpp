#pragma once

#include <SFML/Network.hpp>
#include <Common.hpp>

#include <Enlivengine/System/Log.hpp>

class ServerSocket
{
public:
	ServerSocket() : mSocket(), mSocketPort(0), mRunning(false) {}

	~ServerSocket()
	{
		if (mRunning)
		{
			mSocket.unbind();
		}
	}

	bool Start(en::U16 port)
	{
		LogInfo(en::LogChannel::All, 5, "Starting...%s", "");
		mSocket.setBlocking(true);
		if (mSocket.bind(port) != sf::Socket::Done)
		{
			LogError(en::LogChannel::All, 5, "Can't bind port %d", mSocketPort);
			return false;
		}
		mSocket.setBlocking(false);
		mSocketPort = port;
		mRunning = true;
		LogInfo(en::LogChannel::All, 5, "Started at %s:%d", sf::IpAddress::getPublicAddress().toString().c_str(), mSocket.getLocalPort());
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
	
	void SendPacket(sf::Packet& packet, const sf::IpAddress& remoteAddress, en::U16 remotePort)
	{
		mSocket.send(packet, remoteAddress, remotePort);
	}

	bool PollPacket(sf::Packet& packet, sf::IpAddress& remoteAddress, en::U16& remotePort)
	{
		if (mSocket.receive(packet, remoteAddress, remotePort) == sf::Socket::Done)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	en::U16 GetSocketPort() const { return mSocketPort; }
	void SetSocketPort(en::U16 socketPort) { mSocketPort = socketPort; }

	bool IsRunning() const { return mRunning; }

private:
	sf::UdpSocket mSocket;
	en::U16 mSocketPort;
	bool mRunning;
};