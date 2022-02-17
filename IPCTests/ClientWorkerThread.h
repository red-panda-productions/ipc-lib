#include "ClientSocket.h"
#include "TestSerializer.h"
#include <thread>
#include <mutex>


/// <summary>
/// Woker thread for client
/// </summary>
class ClientWorkerThread
{
private:
	std::mutex mtx;
	int m_action = -1;
	ClientSocket m_socket;
public:
	TestSerializer Message;

	/// <summary>
	/// Constructs the message that can be send
	/// </summary>
	ClientWorkerThread()
	{
		const char* m = "Hello from client";
		Message.m_size = strlen(m);
		strcpy_s(Message.m_message, m);
		strcpy_s()
		
	}

	/// <summary>
	/// Starts the worker thread
	/// </summary>
	void StartThread()
	{
		std::thread(Loop);
	}

	/// <summary>
	/// Sets the next action to be performed
	/// </summary>
	/// <param name="action">The action that needs to be performed</param>
	void SetAction(ACTION action)
	{
		mtx.lock();
		this->m_action = action;
		mtx.unlock();
	}
private:
	/// <summary>
	/// Loops the worker thread until stopped
	/// </summary>
	void Loop()
	{
		ACTION action = NOACTION;
		while ((action = GetAction()) != STOPACTION)
		{
			PerformAction(action);
		}
	}

	/// <summary>
	/// Performs an action
	/// </summary>
	/// <param name="action"></param>
	void PerformAction(ACTION action)
	{
		switch (action)
		{
		case WRITEACTION:
			m_socket.SendData(Message);
			return;
		case READACTION:
			m_socket.WaitForData(Message);
			return;
		case DISCONNECTACTION:
			m_socket.Disconnect();
			return;
		case DECONSTRUCTACTION:
			m_socket.~ClientSocket();
			return;
		default:
			return;
		}
	}

	/// <summary>
	/// Gets the action from the main thread
	/// </summary>
	/// <returns>The action that needs to be executed</returns>
	int GetAction()
	{
		mtx.lock();
		ACTION action = m_action;
		if (action != -1)
		{
			m_action = -1;
		}
		mtx.unlock();
		return action;
	}

};