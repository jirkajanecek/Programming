using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Collections;

namespace SocketServer
{
	class Server
	{
		private int m_port;
		private string m_host = "127.0.0.1";
		private TcpListener m_listener;
		private int m_connectedClients = 0;
		private long m_clientID = 0;
		private Dictionary<long, Thread> m_clients;

		private Object m_unansweredPostsLock = new Object();
		private Queue<ChatPost> m_unansweredPosts = new Queue<ChatPost>();
				
		public int Port
		{
			get{ return m_port; }
		}

		public TcpListener Listener
		{
			get { return m_listener; }
		}

		public Server(
			int port)
		{
			m_port = port;
		}

		public int ConnectedClients
		{
			get { return m_connectedClients; }
		}

		public void init()
		{
			m_clients = new Dictionary<long, Thread>();
			IPAddress address = IPAddress.Parse(m_host);
			m_listener = new TcpListener(address, m_port);
		}

		public void run()
		{
			m_listener.Start();
			Console.WriteLine("Server ready");

			while(true)
			{
				TcpClient tcpClient = m_listener.AcceptTcpClient();
				m_connectedClients++;
				Console.WriteLine("Client connected");
				HandleNewClient(tcpClient, m_clientID);
				m_clientID++;
			}
		}

		class ServerException : Exception{}

		public static void process(
			int port)
		{
			Server server = new SocketServer.Server(port);
			try
			{
				server.init();
				server.run();
			}
			catch (ServerException e)
			{
				Console.WriteLine("Something is fucked: " + e.Message);
			}
		}

		private void HandleNewClient(
			TcpClient tcpClient,
			long clientID)
		{
			ServerThreadPayload payload = new ServerThreadPayload(tcpClient, clientID,m_unansweredPosts);
			Thread client = new Thread(ServerHandler);
			m_clients.Add(clientID, client);
			client.Start();
		}

		private void ServerHandler(
			object data)
		{
			ServerThreadPayload threadData = (ServerThreadPayload)data;
			NetworkStream dataStream = threadData.TCPClient.GetStream();

			byte[] buffer = new byte[1024];
			dataStream.Read(buffer, 0, 1024);
			
			Console.WriteLine("Client {0} sends data", m_clientID);
		}

		class ServerThreadPayload
		{
			readonly private TcpClient m_tcpClient;
			readonly private long m_id;
			private Queue<ChatPost> m_posts;

			public Queue<ChatPost> Posts
			{
				get { return m_posts; }
			}

			public ServerThreadPayload(
				TcpClient tcpClient,
				long id,
				Queue<ChatPost> posts
				)
			{
				m_tcpClient = tcpClient;
				m_id = id;
				m_posts = posts;				
			}

			public TcpClient TCPClient { get { return m_tcpClient; } }
			public long ID { get { return m_id; } }
		}

		struct ChatPost
		{
			string message;
			long clientID;
		}
	}
}
