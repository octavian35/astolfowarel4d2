#pragma once

#include "DebugOverlay.h"
#include "../Includes/checksum_md5.h"
#include "../Includes/bitbuf.h"
#ifdef CopyFile
#undef CopyFile
#endif

class IMaterial;
class CPhysCollide;
class client_textmessage_t;
class IAchievementMgr;
class ISpatialQuery;
class IMaterialSystem;
//class INetChannelInfo;
class SurfInfo;
class CAudioSource;
class CGamestatsData;
class CSentence;
class CSteamAPIContext;

struct player_info_t {
private:
	char __pad00[0x8];

public:
	char name[32];
	int  userid;
	char			guid[33];
	unsigned int	friendsid;

private:
	char __pad01[0x150];
};



struct color32 {
	int r, g, b, a;
};
class INetMessage;
#define MAX_FLOWS 2 // in & out
#define MAX_STREAMS 2
#define MAX_OSPATH 260
#define SUBCHANNEL_FREE 0	// subchannel is free to use
#pragma warning(disable : 4091)
typedef struct netadr_s
{
public:
	netadr_s()
	{
		SetIP(0);
		SetPort(0);
		//SetType(NA_IP);
	}
	netadr_s(const char* pch)
	{
		SetFromString(pch);
	}
	void Clear(); // invalids Address

				  //void SetType(netadrtype_t type);
	void SetPort(unsigned short port);
	bool SetFromSockadr(const struct sockaddr* s);
	void SetIP(unsigned int unIP); // Sets IP.  unIP is in host order (little-endian)
	void SetIPAndPort(unsigned int unIP, unsigned short usPort)
	{
		SetIP(unIP);
		SetPort(usPort);
	}
	void SetFromString(const char* pch, bool bUseDNS = false); // if bUseDNS is true then do a DNS lookup if needed

	bool CompareAdr(const netadr_s& a, bool onlyBase = false) const;
	bool CompareClassBAdr(const netadr_s& a) const;
	bool CompareClassCAdr(const netadr_s& a) const;

	//netadrtype_t GetType() const;
	unsigned short GetPort() const;
	const char* ToString(bool onlyBase = false) const; // returns xxx.xxx.xxx.xxx:ppppp
	void ToSockadr(struct sockaddr* s) const;
	unsigned int GetIP() const;

	bool IsLocalhost() const;   // true, if this is the localhost IP
	bool IsLoopback() const;	// true if engine loopback buffers are used
	bool IsReservedAdr() const; // true, if this is a private LAN IP
	bool IsValid() const;		// ip & port != 0
	void SetFromSocket(int hSocket);
	// These function names are decorated because the Xbox360 defines macros for ntohl and htonl
	unsigned long addr_ntohl() const;
	unsigned long addr_htonl() const;
	bool operator==(const netadr_s& netadr) const
	{
		return (CompareAdr(netadr));
	}
	bool operator<(const netadr_s& netadr) const;

public: // members are public to avoid to much changes
		//netadrtype_t type;
	unsigned char ip[4];
	unsigned short port;
} netadr_t;

class INetChannelInfo
{
public:

	enum
	{
		GENERIC = 0,	// must be first and is default group
		LOCALPLAYER,	// bytes for local player entity update
		OTHERPLAYERS,	// bytes for other players update
		ENTITIES,		// all other entity bytes
		SOUNDS,			// game sounds
		EVENTS,			// event messages
		USERMESSAGES,	// user messages
		ENTMESSAGES,	// entity messages
		VOICE,			// voice data
		STRINGTABLE,	// a stringtable update
		MOVE,			// client move cmds
		STRINGCMD,		// string command
		SIGNON,			// various signondata
		TOTAL,			// must be last and is not a real group
	};

	virtual const char* GetName(void) const = 0;	// get channel name
	virtual const char* GetAddress(void) const = 0; // get channel IP address as string
	virtual float GetTime(void) const = 0;			// current net time
	virtual float GetTimeConnected(void) const = 0; // get connection time in seconds
	virtual int GetBufferSize(void) const = 0;		// netchannel packet history size
	virtual int GetDataRate(void) const = 0;		// send data rate in BYTE/sec

	virtual bool IsLoopback(void) const = 0;  // true if loopback channel
	virtual bool IsTimingOut(void) const = 0; // true if timing out
	virtual bool IsPlayback(void) const = 0;  // true if demo playback

	virtual float GetLatency(int flow) const = 0;								   // current latency (RTT), more accurate but jittering
	virtual float GetAvgLatency(int flow) const = 0;							   // average packet latency in seconds
	virtual float GetAvgLoss(int flow) const = 0;								   // avg packet loss[0..1]
	virtual float GetAvgChoke(int flow) const = 0;								   // avg packet choke[0..1]
	virtual float GetAvgData(int flow) const = 0;								   // data flow in BYTEs/sec
	virtual float GetAvgPackets(int flow) const = 0;							   // avg packets/sec
	virtual int GetTotalData(int flow) const = 0;								   // total flow in/out in BYTEs
	virtual int GetTotalPackets(int flow) const = 0;
	virtual int GetSequenceNr(int flow) const = 0;								   // last send seq number
	virtual bool IsValidPacket(int flow, int frame_number) const = 0;			   // true if packet was not lost/dropped/chocked/flushed
	virtual float GetPacketTime(int flow, int frame_number) const = 0;			   // time when packet was send
	virtual int GetPacketBytes(int flow, int frame_number, int group) const = 0;   // group size of this packet
	virtual bool GetStreamProgress(int flow, int* received, int* total) const = 0; // TCP progress if transmitting
	virtual float GetTimeSinceLastReceived(void) const = 0;						   // get time since last recieved packet in seconds
	virtual float GetCommandInterpolationAmount(int flow, int frame_number) const = 0;
	virtual void GetPacketResponseLatency(int flow, int frame_number, int* pnLatencyMsecs, int* pnChoke) const = 0;
	virtual void GetRemoteFramerate(float* pflFrameTime, float* pflFrameTimeStdDeviation) const = 0;

	virtual float GetTimeoutSeconds() const = 0;
};
class INetChannel : public INetChannelInfo
{
public:
	// virtual			~INetChannel() = 0;	// 27
	virtual void		Destructor(bool freeMemory) = 0;

	virtual void		SetDataRate(float rate) = 0;
	virtual bool		RegisterMessage(INetMessage* msg) = 0;
	virtual bool		StartStreaming(unsigned int challengeNr) = 0;
	virtual void		ResetStreaming(void) = 0;
	virtual void		SetTimeout(float seconds, bool check) = 0;
	virtual void		SetDemoRecorder(void* recorder) = 0;
	virtual void		SetChallengeNr(unsigned int chnr) = 0;

	virtual void		Reset(void) = 0;
	virtual void		Clear(void) = 0;
	virtual void		Shutdown(const char* reason) = 0;

	virtual void		ProcessPlayback(void) = 0;	// 38
	virtual bool		ProcessStream(void) = 0;
	virtual void		ProcessPacket(/*netpacket_t*/void* packet, bool bHasHeader) = 0;

	virtual bool		SendNetMsg(INetMessage& msg, bool bForceReliable = false, bool bVoice = false) = 0;	// 41
	virtual bool		SendData(bf_write& msg, bool bReliable = true) = 0;
	virtual bool		SendFile(const char* filename, unsigned int transferID) = 0;
	virtual void		DenyFile(const char* filename, unsigned int transferID, bool) = 0;
	virtual void		RequestFile_OLD(const char* filename, unsigned int transferID) = 0;
	virtual void		SetChoked(void) = 0;
	virtual int			SendDatagram(bf_write* data) = 0;
	virtual bool		Transmit(bool onlyReliable = false) = 0;

	virtual const netadr_t& GetRemoteAddress(void) const = 0;
	virtual void* GetMsgHandler(void) const = 0;
	virtual int				GetDropNumber(void) const = 0;
	virtual int				GetSocket(void) const = 0;
	virtual unsigned int	GetChallengeNr(void) const = 0;
	virtual void			GetSequenceData(int& nOutSequenceNr, int& nInSequenceNr, int& nOutSequenceNrAck) = 0;
	virtual void			SetSequenceData(int nOutSequenceNr, int nInSequenceNr, int nOutSequenceNrAck) = 0;

	virtual void		UpdateMessageStats(int msggroup, int bits) = 0;
	virtual bool		CanPacket(void) const = 0;
	virtual bool		IsOverflowed(void) const = 0;	// 58
	virtual bool		IsTimedOut(void) const = 0;
	virtual bool		HasPendingReliableData(void) = 0;
	virtual void		SetFileTransmissionMode(bool bBackgroundMode) = 0;
	virtual void		SetCompressionMode(bool bUseCompression) = 0;
	virtual unsigned int RequestFile(const char* filename) = 0;
	virtual void		SetMaxBufferSize(bool bReliable, int nBytes, bool bVoice = false) = 0;
	virtual bool		IsNull() const = 0;
	virtual int		GetNumBitsWritten(bool bReliable) = 0;
	virtual void	SetInterpolationAmount(float flInterpolationAmount) = 0;
	virtual void	SetRemoteFramerate(float flFrameTime, float flFrameTimeStdDeviation) = 0;
	virtual void	SetMaxRoutablePayloadSize(int nSplitSize) = 0;
	virtual int		GetMaxRoutablePayloadSize() = 0;
	virtual void		SetActiveChannel(INetChannel*) = 0;
	virtual void		AttachSplitPlayer(int, INetChannel*) = 0;
	virtual void		DetachSplitPlayer(int) = 0;
	virtual void		IsRemoteDisconnected() const = 0;	// 74
};
/*
class INetMessage
{
public:
	virtual ~INetMessage() {};

	// Use these to setup who can hear whose voice.
	// Pass in client indices (which are their ent indices - 1).

	virtual void SetNetChannel(INetChannel* netchan) = 0; // netchannel this message is from/for
	virtual void SetReliable(bool state) = 0;             // set to true if it's a reliable message

	virtual bool Process(void) = 0; // calles the recently set handler to process this message

	virtual bool ReadFromBuffer(bf_read& buffer) = 0; // returns true if parsing was OK
	virtual bool WriteToBuffer(bf_write& buffer) = 0; // returns true if writing was OK

	virtual bool IsReliable(void) const = 0; // true, if message needs reliable handling

	virtual int GetType(void) const = 0;         // returns module specific header tag eg svc_serverinfo
	virtual int GetGroup(void) const = 0;        // returns net message group of this message
	virtual const char* GetName(void) const = 0; // returns network message name, eg "svc_serverinfo"
	virtual INetChannel* GetNetChannel(void) const = 0;
	virtual const char* ToString(void) const = 0; // returns a human readable string about message content
};
*/
class INetMessage
{
public:
	virtual ~INetMessage() {};

	// Use these to setup who can hear whose voice.
	// Pass in client indices (which are their ent indices - 1).

	virtual void SetNetChannel(INetChannel* netchan) = 0; // netchannel this message is from/for
	virtual void SetReliable(bool state) = 0;			  // set to true if it's a reliable message

	virtual bool Process(void) = 0; // calles the recently set handler to process this message
	virtual void BIncomingMessageForProcessing(double, int, INetMessage*) = 0;	// 不知道这是什么
	virtual bool ReadFromBuffer(bf_read& buffer) = 0; // returns true if parsing was OK
	virtual bool WriteToBuffer(bf_write& buffer) = 0; // returns true if writing was OK

	virtual bool IsReliable(void) const = 0; // true, if message needs reliable handling

	virtual int GetType(void) const = 0;		 // returns module specific header tag eg svc_serverinfo
	virtual int GetGroup(void) const = 0;		 // returns net message group of this message
	virtual const char* GetName(void) const = 0; // returns network message name, eg "svc_serverinfo"
	virtual INetChannel* GetNetChannel(void) const = 0;
	//virtual const char* ToString(void) const = 0; // returns a human readable string about message content
	virtual int GetSize(void) const = 0;
	virtual void SetRateLimitPolicy(LPVOID) = 0;
};
typedef enum class FileHandle_t;
template<class T>
class CUtlMemory
{
public: /* fuck you*/
	typedef T ElemType_t;
};
#define MAX_SUBCHANNELS		8		// we have 8 alternative send&wait bits
class CNetChannel : public INetChannel
{

public: // netchan structurs
	typedef struct dataFragments_s
	{
		FileHandle_t file;                 // open file handle
		char         filename[MAX_OSPATH]; // filename
		char* buffer;               // if NULL it's a file
		unsigned int BYTEs;                // size in BYTEs
		unsigned int bits;                 // size in bits
		unsigned int transferID;           // only for files
		bool         isCompressed;         // true if data is bzip compressed
		unsigned int nUncompressedSize;    // full size in BYTEs
		bool         asTCP;                // send as TCP stream
		int          numFragments;         // number of total fragments
		int          ackedFragments;       // number of fragments send & acknowledged
		int          pendingFragments;     // number of fragments send, but not acknowledged yet
	} dataFragments_t;

	struct subChannel_s
	{
		int startFraggment[MAX_STREAMS];
		int numFragments[MAX_STREAMS];
		int sendSeqNr;
		int state; // 0 = free, 1 = scheduled to send, 2 = send & waiting, 3 = dirty
		int index; // index in m_SubChannels[]

		void Free()
		{
			state = SUBCHANNEL_FREE;
			sendSeqNr = -1;
			for (int i = 0; i < MAX_STREAMS; i++)
			{
				numFragments[i] = 0;
				startFraggment[i] = -1;
			}
		}
	};

	typedef struct netframe_s
	{
		float          time;        // net_time received/send
		int            size;        // total size in BYTEs
		float          latency;     // raw ping for this packet, not cleaned. set when acknowledged otherwise -1.
		float          avg_latency; // averaged ping for this packet
		bool           valid;       // false if dropped, lost, flushed
		int            choked;      // number of previously chocked packets
		int            dropped;
		float          m_flInterpolationAmount;
		unsigned short msggroups[INetChannelInfo::TOTAL]; // received BYTEs for each message group
	} netframe_t;
	typedef struct
	{
		float       nextcompute;      // Time when we should recompute k/sec data
		float       avgBYTEspersec;   // average BYTEs/sec
		float       avgpacketspersec; // average packets/sec
		float       avgloss;          // average packet loss [0..1]
		float       avgchoke;         // average packet choke [0..1]
		float       avglatency;       // average ping, not cleaned
		float       latency;          // current ping, more accurate also more jittering
		int         totalpackets;     // total processed packets
		int         totalBYTEs;       // total processed BYTEs
		int         currentindex;     // current frame index
		netframe_t  frames[64];       // frame history
		netframe_t* currentframe;     // current frame
	} netflow_t;
	typedef struct netpacket_s
	{
		netadr_t		from;		// sender IP
		int				source;		// received source 
		double			received;	// received time
		unsigned char* data;		// pointer to raw packet data
		bf_read			message;	// easy bitbuf data access
		int				size;		// size in bytes
		int				wiresize;   // size in bytes before decompression
		bool			stream;		// was send as stream
		struct netpacket_s* pNext;	// for internal use, should be NULL in public
	} netpacket_t;

public:
	bool ProcessingMessages;
	bool ShouldDelete;

	int OutSequenceNr;
	int InSequenceNr;
	int OutSequenceNrAck;
	int OutReliableState;
	int InReliableState;
	int ChokedPackets;

	// Reliable data buffer, send which each packet (or put in waiting list)
	bf_write	m_StreamReliable;
	CUtlMemory<byte> m_ReliableDataBuffer;

	// unreliable message buffer, cleared which each packet
	bf_write	m_StreamUnreliable;
	CUtlMemory<byte> m_UnreliableDataBuffer;

	bf_write	m_StreamVoice;
	CUtlMemory<byte> m_VoiceDataBuffer;

	int			m_Socket;   // NS_SERVER or NS_CLIENT index, depending on channel.
	int			m_StreamSocket;	// TCP socket handle

	unsigned int m_MaxReliablePayloadSize;	// max size of reliable payload in a single packet	

	// Address this channel is talking to.
	netadr_t	remote_address;

	// For timeouts.  Time last message was received.
	float		last_received;
	// Time when channel was connected.
	double      connect_time;

	// Bandwidth choke
	// Bytes per second
	int			m_Rate;
	// If realtime > cleartime, free to send next packet
	double		m_fClearTime;

	//CUtlVector<dataFragments_t*>	m_WaitingList[MAX_STREAMS];	// waiting list for reliable data and file transfer
	dataFragments_t					m_ReceiveList[MAX_STREAMS]; // receive buffers for streams
	subChannel_s					m_SubChannels[MAX_SUBCHANNELS];

	unsigned int	m_FileRequestCounter;	// increasing counter with each file request
	bool			m_bFileBackgroundTranmission; // if true, only send 1 fragment per packet
	bool			m_bUseCompression;	// if true, larger reliable data will be bzip compressed

	// TCP stream state maschine:
	bool		m_StreamActive;		// true if TCP is active
	int			m_SteamType;		// STREAM_CMD_*
	int			m_StreamSeqNr;		// each blob send of TCP as an increasing ID
	int			m_StreamLength;		// total length of current stream blob
	int			m_StreamReceived;	// length of already received bytes
	char		m_SteamFile[MAX_OSPATH];	// if receiving file, this is it's name
	CUtlMemory<byte> m_StreamData;			// Here goes the stream data (if not file). Only allocated if we're going to use it.

	// packet history
	netflow_t		m_DataFlow[MAX_FLOWS];

	int				m_MsgStats[INetChannelInfo::TOTAL];	// total bytes for each message group


	int				m_PacketDrop;	// packets lost before getting last update (was global net_drop)

	char			m_Name[32];		// channel name

	unsigned int	m_ChallengeNr;	// unique, random challenge number 

	float		m_Timeout;		// in seconds 

	void* m_MessageHandler;	// who registers and processes messages
	//CUtlVector<INetMessage*>	m_NetMessages;		// list of registered message
	void* m_DemoRecorder;			// if != NULL points to a recording/playback demo object
	int							m_nQueuedPackets;

	float						m_flInterpolationAmount;
	float						m_flRemoteFrameTime;
	float						m_flRemoteFrameTimeStdDeviation;
	int							m_nMaxRoutablePayloadSize;

	int							m_nSplitPacketSequence;
	bool						m_bStreamContainsChallenge;  // true if PACKET_FLAG_CHALLENGE was set when receiving packets from the sender

	int							m_nProtocolVersion;
};


class CNetMessage : public INetMessage
{
public:
	CNetMessage()
	{
		m_bReliable = true;
		m_NetChannel = NULL;
	}

	virtual ~CNetMessage() {};
	virtual void SetNetChannel(INetChannel* netchan) { m_NetChannel = netchan; }
	virtual void SetReliable(bool state) { m_bReliable = state; };
	virtual bool Process() { return false; }; // no handler set
	virtual void BIncomingMessageForProcessing(double, int, INetMessage*) {};	// 不知道这是什么
	virtual bool ReadFromBuffer(bf_read& buffer) { return false; };
	virtual bool WriteToBuffer(bf_write& buffer) { return false; };
	virtual bool IsReliable() const { return m_bReliable; };
	virtual int GetType(void) const { return 0; };
	virtual int GetGroup() const { return INetChannelInfo::GENERIC; }
	virtual const char* GetName(void) const { return ""; };
	INetChannel* GetNetChannel() const { return m_NetChannel; }
	virtual int GetSize(void) const { return sizeof(CNetMessage); };
	virtual void SetRateLimitPolicy(LPVOID) { };

protected:
	bool m_bReliable;		   // true if message should be send reliable
	INetChannel* m_NetChannel; // netchannel this message is from/for
	byte __padding[68];
};

typedef enum
{
	eQueryCvarValueStatus_ValueIntact = 0, // It got the value fine.
	eQueryCvarValueStatus_CvarNotFound = 1,
	eQueryCvarValueStatus_NotACvar = 2, // There's a ConCommand, but it's not a ConVar.
	eQueryCvarValueStatus_CvarProtected = 3  // The cvar was marked with FCVAR_SERVER_CAN_NOT_QUERY, so the server is not allowed to have its value.
} EQueryCvarValueStatus;

#define PROCESS_NET_MESSAGE( name )	\
	virtual bool Process##name( NET_##name *msg )

#define PROCESS_SVC_MESSAGE( name )	\
	virtual bool Process##name( SVC_##name *msg )

#define PROCESS_CLC_MESSAGE( name )	\
	virtual bool Process##name( CLC_##name *msg )

#define PROCESS_MM_MESSAGE( name )	\
	virtual bool Process##name( MM_##name *msg )
class KeyValues;
class NET_Tick;
class NET_StringCmd;
class NET_SetConVar;
class NET_SignonState;
class NET_SplitScreenUser;
class SVC_Print;
class SVC_ServerInfo;
class SVC_SendTable;
class SVC_ClassInfo;
class SVC_SetPause;
class SVC_CreateStringTable;
class SVC_UpdateStringTable;
class SVC_SetView;
class SVC_PacketEntities;
class SVC_Menu;
class SVC_GameEventList;
class SVC_GetCvarValue;
class SVC_SplitScreen;
class SVC_CmdKeyValues;
class SVC_VoiceInit;
class SVC_VoiceData;
class SVC_Sounds;
class SVC_FixAngle;
class SVC_CrosshairAngle;
class SVC_BSPDecal;
class SVC_GameEvent;
class SVC_UserMessage;
class SVC_EntityMessage;
class SVC_TempEntities;
class SVC_Prefetch;


#define NUM_NEW_COMMAND_BITS		4
#define MAX_NEW_COMMANDS			((1 << NUM_NEW_COMMAND_BITS)-1)
#define NUM_BACKUP_COMMAND_BITS		3
#define MAX_BACKUP_COMMANDS			((1 << NUM_BACKUP_COMMAND_BITS)-1)
#define NETMSG_TYPE_BITS	6	// must be 2^NETMSG_TYPE_BITS > SVC_LASTMSG
#define clc_Move 9 
class CLC_Move : public CNetMessage
{
public:
	bool ReadFromBuffer(bf_read& buffer)
	{
		m_nNewCommands = buffer.ReadUBitLong(NUM_NEW_COMMAND_BITS);
		m_nBackupCommands = buffer.ReadUBitLong(NUM_BACKUP_COMMAND_BITS);
		m_nLength = buffer.ReadWord();
		m_DataIn = buffer;
		return buffer.SeekRelative(m_nLength);
	}

	bool WriteToBuffer(bf_write& buffer)
	{
		buffer.WriteUBitLong(GetType(), NETMSG_TYPE_BITS);
		m_nLength = m_DataOut.GetNumBitsWritten();

		buffer.WriteUBitLong(m_nNewCommands, NUM_NEW_COMMAND_BITS);
		buffer.WriteUBitLong(m_nBackupCommands, NUM_BACKUP_COMMAND_BITS);

		buffer.WriteWord(m_nLength);

		return buffer.WriteBits(m_DataOut.GetData(), m_nLength);
	}

	const char* ToString() const { return "CLC_Move"; }
	int GetType() const { return clc_Move; }
	const char* GetName() const { return "clc_Move"; }
	void* m_pMessageHandler;
	int GetGroup() const { return INetChannelInfo::MOVE; }
	CLC_Move() { m_bReliable = false; }

public:
	int m_nBackupCommands;
	int m_nNewCommands;
	int m_nLength;
	bf_read m_DataIn;
	bf_write m_DataOut;
};
class NET_SetConVar : public CNetMessage
{
public:
	int GetType(void) const { return 0x6; };
	int	GetGroup() const { return INetChannelInfo::STRINGCMD; }

	NET_SetConVar() {}
	NET_SetConVar(const char* name, const char* value)
	{
		cvar_t cvar;
		strncpy(cvar.name, name, 260);
		strncpy(cvar.value, value, 260);
		m_ConVars = cvar;
	}

public:

	typedef struct cvar_s
	{
		char	name[260];
		char	value[260];
	} cvar_t;

	cvar_t m_ConVars;
};

class CClockDriftMgr
{
	friend class CBaseClientState;

public:
	CClockDriftMgr() {};

	// Is clock correction even enabled right now?
	static bool IsClockCorrectionEnabled();

	// Clear our state.
	void Clear();

	// This is called each time a server packet comes in. It is used to correlate
	// where the server is in time compared to us.
	void SetServerTick(int iServerTick);

	// Pass in the frametime you would use, and it will drift it towards the server clock.
	float AdjustFrameTime(float inputFrameTime);

	// Returns how many ticks ahead of the server the client is.
	float GetCurrentClockDifference() const;

private:
	void ShowDebugInfo(float flAdjustment);

	// This scales the offsets so the average produced is equal to the
	// current average + flAmount. This way, as we add corrections,
	// we lower the average accordingly so we don't keep responding
	// as much as we need to after we'd adjusted it a couple times.
	void AdjustAverageDifferenceBy(float flAmountInSeconds);

private:
	enum
	{
		// This controls how much it smoothes out the samples from the server.
		NUM_CLOCKDRIFT_SAMPLES = 16
	};

	// This holds how many ticks the client is ahead each time we get a server tick.
	// We average these together to get our estimate of how far ahead we are.
	float m_ClockOffsets[NUM_CLOCKDRIFT_SAMPLES];
	int m_iCurClockOffset;

	int m_nServerTick; // Last-received tick from the server.
	int m_nClientTick; // The client's own tick counter (specifically, for interpolation during rendering).
					   // The server may be on a slightly different tick and the client will drift towards it.
};
#define net_SignonState 6 // signals current signon state
#define MAX_AREA_STATE_BYTES		32
#define MAX_AREA_PORTAL_STATE_BYTES 24
class PackedEntity;
class CServerClassInfo;
class CNetworkStringTableContainer;
class IVEngineClient
{
public:
	virtual int						GetIntersectingSurfaces(const void* model, const Vector& vCenter, const float radius, const bool bOnlyVisibleSurfaces, SurfInfo* pInfos, const int nMaxInfos) = 0;
	virtual Vector					GetLightForPoint(const Vector& pos, bool bClamp) = 0;
	virtual IMaterial*				TraceLineMaterialAndLighting(const Vector& start, const Vector& end, Vector& diffuseLightColor, Vector& baseColor) = 0;
	virtual const char*				ParseFile(const char* data, char* token, int maxlen) = 0;
	virtual bool					CopyLocalFile(const char* source, const char* destination) = 0;
	virtual void					GetScreenSize(int& width, int& height) = 0;
	virtual void					ServerCmd(const char* szCmdString, bool bReliable = true) = 0;
	virtual void					ClientCmd(const char* szCmdString) = 0;
	virtual bool					GetPlayerInfo(int ent_num, player_info_t* pinfo) = 0;
	virtual int						GetPlayerForUserID(int userID) = 0;
	virtual client_textmessage_t*	TextMessageGet(const char* pName) = 0;
	virtual bool					Con_IsVisible(void) = 0;
	virtual int						GetLocalPlayer(void) = 0;
	virtual const void*				LoadModel(const char* pName, bool bProp = false) = 0;
	virtual float					OBSOLETE_Time(void) = 0;
	virtual float					GetLastTimeStamp(void) = 0;
	virtual CSentence*				GetSentence(CAudioSource* pAudioSource) = 0;
	virtual float					GetSentenceLength(CAudioSource* pAudioSource) = 0;
	virtual bool					IsStreaming(CAudioSource* pAudioSource) const = 0;
	virtual void					GetViewAngles(Vector& va) = 0;
	virtual void					SetViewAngles(Vector& va) = 0;
	virtual int						GetMaxClients(void) = 0;
	virtual	const char*				Key_LookupBinding(const char* pBinding) = 0;
	virtual const char*				Key_BindingForKey(int& code) = 0; //ButtonCode_t&
	virtual void					StartKeyTrapMode(void) = 0;
	virtual bool					CheckDoneKeyTrapping(int& code) = 0; //ButtonCode_t&
	virtual bool					IsInGame(void) = 0;
	virtual bool					IsConnected(void) = 0;
	virtual bool					IsDrawingLoadingImage(void) = 0;
	virtual void					HideLoadingPlaque() = 0;
	virtual void					Con_NPrintf(int pos, const char* fmt, ...) = 0;
	virtual void					Con_NXPrintf(const struct con_nprint_s* info, const char* fmt, ...) = 0;
	virtual int						IsBoxVisible(const Vector& mins, const Vector& maxs) = 0;
	virtual int						IsBoxInViewCluster(const Vector& mins, const Vector& maxs) = 0;
	virtual bool					CullBox(const Vector& mins, const Vector& maxs) = 0;
	virtual void					Sound_ExtraUpdate(void) = 0;
	virtual const char*				GetGameDirectory(void) = 0;
	virtual const VMatrix&			WorldToScreenMatrix() = 0;
	virtual const VMatrix&			WorldToViewMatrix() = 0;
	virtual int						GameLumpVersion(int lumpId) const = 0;
	virtual int						GameLumpSize(int lumpId) const = 0;
	virtual bool					LoadGameLump(int lumpId, void* pBuffer, int size) = 0;
	virtual int						LevelLeafCount() const = 0;
	virtual ISpatialQuery*			GetBSPTreeQuery() = 0;
	virtual void					LinearToGamma(float* linear, float* gamma) = 0;
	virtual float					LightStyleValue(int style) = 0;
	virtual void					ComputeDynamicLighting(const Vector& pt, const Vector* pNormal, Vector& color) = 0;
	virtual void					GetAmbientLightColor(Vector& color) = 0;
	virtual int						GetDXSupportLevel() = 0;
	virtual bool					SupportsHDR() = 0;
	virtual void					Mat_Stub(IMaterialSystem* pMatSys) = 0;
	virtual void					GetChapterName(char* pchBuff, int iMaxLength) = 0;
	virtual char const*				GetLevelName(void) = 0;
	virtual char const*				GetLevelNameShort(void) = 0;
	virtual struct IVoiceTweak_s*	GetVoiceTweakAPI(void) = 0;
	virtual void					EngineStats_BeginFrame(void) = 0;
	virtual void					EngineStats_EndFrame(void) = 0;
	virtual void					FireEvents() = 0;
	virtual int						GetLeavesArea(int* pLeaves, int nLeaves) = 0;
	virtual bool					DoesBoxTouchAreaFrustum(const Vector& mins, const Vector& maxs, int iArea) = 0;
	virtual void					GetFrustumList(void** unk1, int unk2) = 0;
	virtual void					SetAudioState(const void* state) = 0; //AudioState_t&
	virtual int						SentenceGroupPick(int groupIndex, char* name, int nameBufLen) = 0;
	virtual int						SentenceGroupPickSequential(int groupIndex, char* name, int nameBufLen, int sentenceIndex, int reset) = 0;
	virtual int						SentenceIndexFromName(const char* pSentenceName) = 0;
	virtual const char*				SentenceNameFromIndex(int sentenceIndex) = 0;
	virtual int						SentenceGroupIndexFromName(const char* pGroupName) = 0;
	virtual const char*				SentenceGroupNameFromIndex(int groupIndex) = 0;
	virtual float					SentenceLength(int sentenceIndex) = 0;
	virtual void					ComputeLighting(const Vector& pt, const Vector* pNormal, bool bClamp, Vector& color, Vector* pBoxColors = NULL) = 0;
	virtual void					ActivateOccluder(int nOccluderIndex, bool bActive) = 0;
	virtual bool					IsOccluded(const Vector& vecAbsMins, const Vector& vecAbsMaxs) = 0;
	virtual void*					SaveAllocMemory(size_t num, size_t size) = 0;
	virtual void					SaveFreeMemory(void* pSaveMem) = 0;
	virtual INetChannelInfo*		GetNetChannelInfo(void) = 0;
	virtual void					DebugDrawPhysCollide(const CPhysCollide* pCollide, IMaterial* pMaterial, matrix3x4_t& transform, const color32& color) = 0;
	virtual void					CheckPoint(const char* pName) = 0;
	virtual void					DrawPortals() = 0;
	virtual bool					IsPlayingDemo(void) = 0;
	virtual bool					IsRecordingDemo(void) = 0;
	virtual bool					IsPlayingTimeDemo(void) = 0;

private:
	virtual void*					GetDemoRecordingTick() = 0;
	virtual void*					GetDemoPlaybackTick() = 0;
	virtual void*					GetDemoPlaybackStartTick() = 0;
	virtual void*					GetDemoPlaybackTimeScale() = 0;
	virtual void*					GetDemoPlaybackTotalTicks() = 0;

public:
	virtual bool					IsPaused(void) = 0;
	virtual float					GetTimescale(void) const = 0;
	virtual bool					IsTakingScreenshot(void) = 0;
	virtual bool					IsHLTV(void) = 0;
	virtual bool					IsLevelMainMenuBackground(void) = 0;
	virtual void					GetMainMenuBackgroundName(char* dest, int destlen) = 0;
	virtual void					SetOcclusionParameters(const void* params) = 0; //OcclusionParams_t&
	virtual void					GetUILanguage(char* dest, int destlen) = 0;
	virtual int						IsSkyboxVisibleFromPoint(const Vector& vecPoint) = 0; //SkyboxVisibility_t
	virtual const char*				GetMapEntitiesString() = 0;
	virtual bool					IsInEditMode(void) = 0;
	virtual float					GetScreenAspectRatio() = 0;
	virtual unsigned int			GetEngineBuildNumber() = 0;
	virtual const char*				GetProductVersionString() = 0;
	virtual void					GrabPreColorCorrectedFrame(int x, int y, int width, int height) = 0;
	virtual bool					IsHammerRunning() const = 0;
	virtual void					ExecuteClientCmd(const char* szCmdString) = 0;
	virtual bool					MapHasHDRLighting(void) = 0;
	virtual int						GetAppID() = 0;
	virtual Vector					GetLightForPointFast(const Vector& pos, bool bClamp) = 0;
	virtual void					ClientCmd_Unrestricted(const char* szCmdString) = 0;
	virtual void					SetRestrictServerCommands(bool bRestrict) = 0;
	virtual void					SetRestrictClientCommands(bool bRestrict) = 0;
	virtual void					SetOverlayBindProxy(int iOverlayID, void* pBindProxy) = 0;
	virtual bool					CopyFrameBufferToMaterial(const char* pMaterialName) = 0;
	virtual void					ReadConfiguration(int unk1, const bool readDefault = false) = 0;
	virtual void					SetAchievementMgr(IAchievementMgr* pAchievementMgr) = 0;
	virtual IAchievementMgr*		GetAchievementMgr() = 0;
	virtual bool					MapLoadFailed(void) = 0;
	virtual void					SetMapLoadFailed(bool bState) = 0;
	virtual bool					IsLowViolence() = 0;
	virtual const char*				GetMostRecentSaveGame(void) = 0;
	virtual void					SetMostRecentSaveGame(const char* lpszFilename) = 0;
	virtual void					StartXboxExitingProcess() = 0;
	virtual bool					IsSaveInProgress() = 0;
	virtual unsigned int			OnStorageDeviceAttached(void) = 0;
	virtual void					OnStorageDeviceDetached(void) = 0;
	virtual void					WriteScreenshot(const char* pFilename) = 0;
	virtual void					ResetDemoInterpolation(void) = 0;
	virtual int						GetActiveSplitScreenPlayerSlot() = 0;
	virtual int						SetActiveSplitScreenPlayerSlot(int slot) = 0;
	virtual bool					SetLocalPlayerIsResolvable(char const* pchContext, int nLine, bool bResolvable) = 0;
	virtual bool					IsLocalPlayerResolvable() = 0;
	virtual int						GetSplitScreenPlayer(int nSlot) = 0;
	virtual bool					IsSplitScreenActive() = 0;
	virtual bool					IsValidSplitScreenSlot(int nSlot) = 0;
	virtual int						FirstValidSplitScreenSlot() = 0; // -1 == invalid
	virtual int						NextValidSplitScreenSlot(int nPreviousSlot) = 0; // -1 == invalid
	virtual void*					GetSinglePlayerSharedMemorySpace(const char* szName, int ent_num = 0) = 0;

private:
	virtual void					ComputeLightingCube(const Vector& pt, bool bClamp, Vector* pBoxColors) = 0;
	virtual void					RegisterDemoCustomDataCallback(void* szCallbackSaveID, void* pCallback) = 0;
	virtual void					RecordDemoCustomData(void* pCallback, const void* pData, size_t iDataLength) = 0;

public:
	virtual void					SetPitchScale(float unk1) = 0;
	virtual float					GetPitchScale(void) = 0;
	virtual void					SetLeafFlag(int nLeafIndex, int nFlagBits) = 0;
	virtual void					RecalculateBSPLeafFlags(void) = 0;
	virtual bool					DSPGetCurrentDASRoomNew(void) = 0;
	virtual bool					DSPGetCurrentDASRoomChanged(void) = 0;
	virtual bool					DSPGetCurrentDASRoomSkyAbove(void) = 0;
	virtual float					DSPGetCurrentDASRoomSkyPercent(void) = 0;
	virtual void					SetMixGroupOfCurrentMixer(const char* szgroupname, const char* szparam, float val, int setMixerType) = 0;
	virtual int						GetMixLayerIndex(const char* szmixlayername) = 0;
	virtual void					SetMixLayerLevel(int index, float level) = 0;
	virtual bool					IsCreatingReslist() = 0;
	virtual bool					IsCreatingXboxReslist() = 0;
	virtual void					SetTimescale(float unk1) = 0;
	virtual void					SetGamestatsData(CGamestatsData* pGamestatsData) = 0;
	virtual CGamestatsData*			GetGamestatsData() = 0;
	virtual	const char*				Key_LookupBindingEx(const char* pBinding, int iUserId = -1, int iStartCount = 0, int nFlags = 0) = 0; //BindingLookupOption_t
	virtual void					UpdateDAndELights(void) = 0;
	virtual int						GetBugSubmissionCount() const = 0;
	virtual void					ClearBugSubmissionCount() = 0;
	virtual bool					DoesLevelContainWater() const = 0;
	virtual float					GetServerSimulationFrameTime() const = 0;
	virtual void					SolidMoved(class IClientEntity* pSolidEnt, class ICollideable* pSolidCollide, const Vector* pPrevAbsOrigin, bool accurateBboxTriggerChecks) = 0;
	virtual void					TriggerMoved(class IClientEntity* pTriggerEnt, bool accurateBboxTriggerChecks) = 0;
	virtual void					ComputeLeavesConnected(const Vector& vecOrigin, int nCount, const int* pLeafIndices, bool* pIsConnected) = 0;
	virtual bool					IsInCommentaryMode(void) = 0;
	virtual void					SetBlurFade(float amount) = 0;
	virtual bool					IsTransitioningToLoad() = 0;
	virtual void					SearchPathsChangedAfterInstall() = 0;
	virtual void					ConfigureSystemLevel(int nCPULevel, int nGPULevel) = 0;
	virtual void					SetConnectionPassword(char const* pchCurrentPW) = 0;
	virtual CSteamAPIContext*		GetSteamAPIContext() = 0;
	virtual void					SubmitStatRecord(char const* szMapName, unsigned __int32 uiBlobVersion, unsigned __int32 uiBlobSize, const void* pvBlob) = 0;
	virtual void					ServerCmdKeyValues(void* pKeyValues) = 0;
	void client_cmd_unrestricted(const char* chCommandString)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, const char*);
		return U::VFunc.Get<OriginalFn>(this, 108)(this, chCommandString);
	}
};

namespace I { inline IVEngineClient* EngineClient = nullptr; }