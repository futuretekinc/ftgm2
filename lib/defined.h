#ifndef	DEFINED_H_
#define	DEFINED_H_

#define	ID_MAX_LEN		32
#define	NAME_MAX_LEN	128

#define	SNMP_PEER_MAX_LEN			32
#define	SNMP_COMMUNITY_MAX_LEN		128

typedef	int	RetValue;
typedef	unsigned int	uint32;

#define	RET_VALUE_OK						0
#define	RET_VALUE_ERROR						1
#define	RET_VALUE_ALREADY_ATTACHED		2
#define	RET_VALUE_NOT_ATTACHED			3
#define	RET_VALUE_INVALID_CONFIG_FILE		4
#define	RET_VALUE_NOT_SUPPORTED_FUNCTION	5

#define	RET_VALUE_INVALID_TYPE			6
#define	RET_VALUE_INVALID_DATA			7
#define	RET_VALUE_INVALID_FIELD			8
#define	RET_VALUE_INVALID_FIELD_VALUE		9
#define	RET_VALUE_INVALID_ARGUMENTS		10
#define	RET_VALUE_INVALID_FORMAT		11

#define	RET_VALUE_OBJECT_EXISTS			12
#define	RET_VALUE_OBJECT_NOT_FOUND		13
#define	RET_VALUE_DEVICE_DOES_NOT_EXIST	14
#define	RET_VALUE_OBJECT_IS_NOT_INITIALIZED	15
#define	RET_VALUE_DEVICE_NOT_ACTIVATED	16

#define	RET_VALUE_NOT_ATTACHED_TO_DEVICE	20

#define	RET_VALUE_OBJECT_MANAGER_NOT_INITIALIZED	30

#define	RET_VALUE_NOT_ENOUGH_MEMORY		99
#define	RET_VALUE_EXCEPTION				100

#define	RET_VALUE_SNMP_ERROR						0x00000100
#define	RET_VALUE_SNMP_SESSION_OPEN_FAILED		(RET_VALUE_SNMP_ERROR | 1)
#define	RET_VALUE_SNMP_SESSION_IS_NOT_CONNECTED	(RET_VALUE_SNMP_ERROR | 2)
#define	RET_VALUE_SNMP_OBJECT_NOT_FOUND			(RET_VALUE_SNMP_ERROR | 3)
#define	RET_VALUE_SNMP_RESPONSE_ERROR				(RET_VALUE_SNMP_ERROR | 0x1000)

#define	RET_VALUE_DB_ERROR							0x00000200
#define	RET_VALUE_DB_NOT_INITIALIZED			(RET_VALUE_DB_ERROR | 1)
#define	RET_VALUE_DB_ENTRY_DOES_NOT_EXIST		(RET_VALUE_DB_ERROR | 2)

#define	RET_VALUE_SOCKET_ERROR						0x00000300
#define	RET_VALUE_SOCKET_BIND_FAILED			(RET_VALUE_SOCKET_ERROR | 1)	
#define	RET_VALUE_SOCKET_CLOSED					(RET_VALUE_SOCKET_ERROR | 2)	
#define	RET_VALUE_SOCKET_CONNECTION_FAILED		(RET_VALUE_SOCKET_ERROR | 3)	
#define	RET_VALUE_SOCKET_SEND_FAILED			(RET_VALUE_SOCKET_ERROR | 4)	

#define	RET_VALUE_SESSION_NOT_FOUND				(RET_VALUE_SOCKET_ERROR | 5)	
#endif
