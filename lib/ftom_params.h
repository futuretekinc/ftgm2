#ifndef	__FTOM_PARAMS_H__
#define	__FTOM_PARAMS_H__

#include "ftom_types.h"
#include "ftom_server_cmd.h"
#include "ftom_msg.h"

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
}	FTOM_REQ_PARAMS, _PTR_ FTOM_REQ_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_PARAMS, _PTR_ FTOM_RESP_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pHost[FTM_URL_LEN+1];
	FTM_USHORT			usPort;
}	FTOM_REQ_REGISTER_SUBSCRIBE_PARAMS, _PTR_ FTOM_REQ_REGISTER_SUBSCRIBE_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_REGISTER_SUBSCRIBE_PARAMS, _PTR_ FTOM_RESP_REGISTER_SUBSCRIBE_PARAMS_PTR;

typedef	struct 
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_NODE		xNodeInfo;
}	FTOM_REQ_ADD_NODE_PARAMS,	_PTR_ FTOM_REQ_ADD_NODE_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_CHAR			pDID[FTM_ID_LEN + 1];
}	FTOM_RESP_ADD_NODE_PARAMS, _PTR_ FTOM_RESP_ADD_NODE_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pID[FTM_ID_LEN+1];
}	FTOM_REQ_DELETE_NODE_PARAMS, _PTR_ FTOM_REQ_DELETE_NODE_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_DELETE_NODE_PARAMS, _PTR_ FTOM_RESP_DELETE_NODE_PARAMS_PTR;

typedef struct	
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
}	FTOM_REQ_GET_NODE_COUNT_PARAMS, _PTR_ FTOM_REQ_GET_NODE_COUNT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_ULONG			ulCount;
}	FTOM_RESP_GET_NODE_COUNT_PARAMS, _PTR_ FTOM_RESP_GET_NODE_COUNT_PARAMS_PTR;

typedef struct	
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pDID[FTM_ID_LEN+1];
}	FTOM_REQ_GET_NODE_PARAMS, _PTR_ FTOM_REQ_GET_NODE_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_NODE		xNodeInfo;
}	FTOM_RESP_GET_NODE_PARAMS, _PTR_ FTOM_RESP_GET_NODE_PARAMS_PTR;

typedef struct	
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_ULONG			ulIndex;
}	FTOM_REQ_GET_NODE_AT_PARAMS, _PTR_ FTOM_REQ_GET_NODE_AT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_NODE		xNodeInfo;
}	FTOM_RESP_GET_NODE_AT_PARAMS, _PTR_ FTOM_RESP_GET_NODE_AT_PARAMS_PTR;

typedef struct	
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pDID[FTM_ID_LEN+1];
	FTM_NODE_FIELD		xFields;
	FTM_NODE			xInfo;
}	FTOM_REQ_SET_NODE_PARAMS, _PTR_ FTOM_REQ_SET_NODE_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_NODE			xInfo;
}	FTOM_RESP_SET_NODE_PARAMS, _PTR_ FTOM_RESP_SET_NODE_PARAMS_PTR;

typedef struct	
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pDID[FTM_ID_LEN+1];
}	FTOM_REQ_START_NODE_PARAMS, _PTR_ FTOM_REQ_START_NODE_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_START_NODE_PARAMS, _PTR_ FTOM_RESP_START_NODE_PARAMS_PTR;

typedef struct	
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pDID[FTM_ID_LEN+1];
}	FTOM_REQ_STOP_NODE_PARAMS, _PTR_ FTOM_REQ_STOP_NODE_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_STOP_NODE_PARAMS, _PTR_ FTOM_RESP_STOP_NODE_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pDID[FTM_ID_LEN+1];
}	FTOM_REQ_IS_NODE_RUN_PARAMS, _PTR_ 	FTOM_REQ_IS_NODE_RUN_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_BOOL			bRun;
}	FTOM_RESP_IS_NODE_RUN_PARAMS, _PTR_	FTOM_RESP_IS_NODE_RUN_PARAMS_PTR;

typedef struct	
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pDID[FTM_ID_LEN+1];
}	FTOM_REQ_REGISTER_NODE_PARAMS, _PTR_ FTOM_REQ_REGISTER_NODE_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_REGISTER_NODE_PARAMS, _PTR_ FTOM_RESP_REGISTER_NODE_PARAMS_PTR;

typedef struct	
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pDID[FTM_ID_LEN+1];
	FTM_BOOL			bRegistered;
}	FTOM_REQ_SET_NODE_SERVER_REGISTERED_PARAMS, _PTR_ FTOM_REQ_SET_NODE_SERVER_REGISTERED_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_SET_NODE_SERVER_REGISTERED_PARAMS, _PTR_ FTOM_RESP_SET_NODE_SERVER_REGISTERED_PARAMS_PTR;


typedef struct	
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pDID[FTM_ID_LEN+1];
}	FTOM_REQ_GET_NODE_SERVER_REGISTERED_PARAMS, _PTR_ FTOM_REQ_GET_NODE_SERVER_REGISTERED_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_BOOL			bRegistered;
}	FTOM_RESP_GET_NODE_SERVER_REGISTERED_PARAMS, _PTR_ FTOM_RESP_GET_NODE_SERVER_REGISTERED_PARAMS_PTR;

typedef struct	
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pDID[FTM_ID_LEN+1];
	FTM_ULONG			ulReportInterval;
}	FTOM_REQ_SET_NODE_REPORT_INTERVAL_PARAMS,_PTR_ FTOM_REQ_SET_NODE_REPORT_INTERVAL_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_SET_NODE_REPORT_INTERVAL_PARAMS,_PTR_ FTOM_RESP_SET_NODE_REPORT_INTERVAL_PARAMS_PTR;


/*************************************************************************
 *
 *************************************************************************/
typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_EP     			xInfo;
}	FTOM_REQ_ADD_EP_PARAMS, _PTR_ FTOM_REQ_ADD_EP_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_EP     			xInfo;
}	FTOM_RESP_ADD_EP_PARAMS, _PTR_	FTOM_RESP_ADD_EP_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
}	FTOM_REQ_DELETE_EP_PARAMS, _PTR_	FTOM_REQ_DELETE_EP_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_DELETE_EP_PARAMS, _PTR_ 	FTOM_RESP_DELETE_EP_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_EP_TYPE			xType;
	FTM_CHAR			pDID[FTM_ID_LEN+1];
}	FTOM_REQ_GET_EP_COUNT_PARAMS, _PTR_ 	FTOM_REQ_GET_EP_COUNT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_ULONG			nCount;
}	FTOM_RESP_GET_EP_COUNT_PARAMS, _PTR_	FTOM_RESP_GET_EP_COUNT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pDID[FTM_ID_LEN+1];
	FTM_EP_TYPE			xType;
	FTM_ULONG			ulIndex;
	FTM_ULONG			ulMaxCount;
}	FTOM_REQ_GET_EP_ID_LIST_PARAMS, _PTR_ 	FTOM_REQ_GET_EP_ID_LIST_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_ULONG			ulCount;
	FTM_ID				pEPIDList[];
}	FTOM_RESP_GET_EP_ID_LIST_PARAMS, _PTR_	FTOM_RESP_GET_EP_ID_LIST_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
}	FTOM_REQ_GET_EP_PARAMS, _PTR_ 	FTOM_REQ_GET_EP_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_EP			xInfo;
}	FTOM_RESP_GET_EP_PARAMS, _PTR_	FTOM_RESP_GET_EP_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_ULONG			ulIndex;
}	FTOM_REQ_GET_EP_AT_PARAMS, _PTR_	FTOM_REQ_GET_EP_AT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_EP			xInfo;
}	FTOM_RESP_GET_EP_AT_PARAMS, _PTR_	FTOM_RESP_GET_EP_AT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
	FTM_EP_FIELD		xFields;
	FTM_EP				xInfo;
}	FTOM_REQ_SET_EP_PARAMS, _PTR_ 	FTOM_REQ_SET_EP_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_SET_EP_PARAMS, _PTR_	FTOM_RESP_SET_EP_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
}	FTOM_REQ_IS_EP_RUN_PARAMS, _PTR_ 	FTOM_REQ_IS_EP_RUN_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_BOOL			bRun;
}	FTOM_RESP_IS_EP_RUN_PARAMS, _PTR_	FTOM_RESP_IS_EP_RUN_PARAMS_PTR;

typedef struct	
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
}	FTOM_REQ_EP_REGISTER_AT_SERVER_PARAMS, _PTR_ FTOM_REQ_EP_REGISTER_AT_SERVER_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_EP_REGISTER_AT_SERVER_PARAMS, _PTR_ FTOM_RESP_EP_REGISTER_AT_SERVER_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
	FTM_ULONG			ulReportInterval;
}	FTOM_REQ_SET_EP_REPORT_INTERVAL_PARAMS, _PTR_ 	FTOM_REQ_SET_EP_REPORT_INTERVAL_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_SET_EP_REPORT_INTERVAL_PARAMS, _PTR_	FTOM_RESP_SET_EP_REPORT_INTERVAL_PARAMS_PTR;
typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
	FTM_EP_DATA			xData;
}	FTOM_REQ_EP_REMOTE_SET_PARAMS, _PTR_ 	FTOM_REQ_EP_REMOTE_SET_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_EP_REMOTE_SET_PARAMS, _PTR_	FTOM_RESP_EP_REMOTE_SET_PARAMS_PTR;

typedef struct	
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
	FTM_BOOL			bRegistered;
}	FTOM_REQ_SET_EP_SERVER_REGISTERED_PARAMS, _PTR_ FTOM_REQ_SET_EP_SERVER_REGISTERED_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_SET_EP_SERVER_REGISTERED_PARAMS, _PTR_ FTOM_RESP_SET_EP_SERVER_REGISTERED_PARAMS_PTR;


typedef struct	
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
}	FTOM_REQ_GET_EP_SERVER_REGISTERED_PARAMS, _PTR_ FTOM_REQ_GET_EP_SERVER_REGISTERED_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_BOOL			bRegistered;
}	FTOM_RESP_GET_EP_SERVER_REGISTERED_PARAMS, _PTR_ FTOM_RESP_GET_EP_SERVER_REGISTERED_PARAMS_PTR;


typedef struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
}	FTOM_REQ_EP_REG_NOTIFY_RECEIVER_PARAMS, _PTR_ FTOM_REQ_EP_REG_NOTIFY_RECEIVER_PARAMS_PTR;

typedef struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_EP_REG_NOTIFY_RECEIVER_PARAMS, _PTR_ FTOM_RESP_EP_REG_NOTIFY_RECEIVER_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			nLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
	FTM_EP_DATA			xData;
}	FTOM_REQ_ADD_EP_DATA_PARAMS, _PTR_ FTOM_REQ_ADD_EP_DATA_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			nLen;
	FTM_RET				xRet;
}	FTOM_RESP_ADD_EP_DATA_PARAMS, _PTR_	FTOM_RESP_ADD_EP_DATA_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
}	FTOM_REQ_GET_EP_DATA_INFO_PARAMS, _PTR_ FTOM_REQ_GET_EP_DATA_INFO_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_ULONG			ulBeginTime;
	FTM_ULONG			ulEndTime;
	FTM_ULONG			ulCount;
}	FTOM_RESP_GET_EP_DATA_INFO_PARAMS, _PTR_	FTOM_RESP_GET_EP_DATA_INFO_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
}	FTOM_REQ_GET_LAST_EP_DATA_PARAMS, _PTR_	FTOM_REQ_GET_LAST_EP_DATA_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_EP_DATA 		xData;
}	FTOM_RESP_GET_LAST_EP_DATA_PARAMS, _PTR_ FTOM_RESP_GET_LAST_EP_DATA_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
	FTM_ULONG			nStartIndex;
	FTM_ULONG			nCount; 
}	FTOM_REQ_GET_EP_DATA_LIST_PARAMS, _PTR_	FTOM_REQ_GET_EP_DATA_LIST_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_BOOL			bRemain;
	FTM_ULONG			nCount; 
	FTM_EP_DATA 		pData[];
}	FTOM_RESP_GET_EP_DATA_LIST_PARAMS, _PTR_ FTOM_RESP_GET_EP_DATA_LIST_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
	FTM_ULONG			ulBegin;
	FTM_ULONG			ulEnd;
	FTM_BOOL			bAscending;
	FTM_ULONG			nCount; 
}	FTOM_REQ_GET_EP_DATA_LIST_WITH_TIME_PARAMS, _PTR_	FTOM_REQ_GET_EP_DATA_LIST_WITH_TIME_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_BOOL			bRemain;
	FTM_ULONG			nCount; 
	FTM_EP_DATA 		pData[];
}	FTOM_RESP_GET_EP_DATA_LIST_WITH_TIME_PARAMS, _PTR_ FTOM_RESP_GET_EP_DATA_LIST_WITH_TIME_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
	FTM_ULONG 			ulIndex; 
	FTM_ULONG			ulCount;
}	FTOM_REQ_DELETE_EP_DATA_PARAMS, _PTR_ FTOM_REQ_DELETE_EP_DATA_PARAMS_PTR; 

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_ULONG			ulCount;
}	FTOM_RESP_DELETE_EP_DATA_PARAMS, _PTR_ 	FTOM_RESP_DELETE_EP_DATA_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
	FTM_ULONG 			ulBegin; 
	FTM_ULONG			ulEnd;
}	FTOM_REQ_DELETE_EP_DATA_WITH_TIME_PARAMS, _PTR_ FTOM_REQ_DELETE_EP_DATA_WITH_TIME_PARAMS_PTR; 

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_ULONG			ulCount;
}	FTOM_RESP_DELETE_EP_DATA_WITH_TIME_PARAMS, _PTR_ 	FTOM_RESP_DELETE_EP_DATA_WITH_TIME_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
}	FTOM_REQ_GET_EP_DATA_COUNT_PARAMS, _PTR_ FTOM_REQ_GET_EP_DATA_COUNT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_ULONG			ulCount;
}	FTOM_RESP_GET_EP_DATA_COUNT_PARAMS, _PTR_ FTOM_RESP_GET_EP_DATA_COUNT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
	FTM_ULONG			ulStart;
	FTM_ULONG			ulEnd;
}	FTOM_REQ_GET_EP_DATA_COUNT_WITH_TIME_PARAMS, _PTR_ FTOM_REQ_GET_EP_DATA_COUNT_WITH_TIME_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_ULONG			ulCount;
}	FTOM_RESP_GET_EP_DATA_COUNT_WITH_TIME_PARAMS, _PTR_ FTOM_RESP_GET_EP_DATA_COUNT_WITH_TIME_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
}	FTOM_REQ_GET_EP_DATA_TYPE_PARAMS, _PTR_ FTOM_REQ_GET_EP_DATA_TYPE_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_VALUE_TYPE		xType;
}	FTOM_RESP_GET_EP_DATA_TYPE_PARAMS, _PTR_ FTOM_RESP_GET_EP_DATA_TYPE_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pEPID[FTM_ID_LEN+1];
	FTM_ULONG			ulTime;
}	FTOM_REQ_SET_EP_DATA_SERVER_TIME_PARAMS, _PTR_ FTOM_REQ_SET_EP_DATA_SERVER_TIME_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_SET_EP_DATA_SERVER_TIME_PARAMS, _PTR_ FTOM_RESP_SET_EP_DATA_SERVER_TIME_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pIP[FTM_URL_LEN+1];
	FTM_USHORT			usPort;
}	FTOM_REQ_SET_NOTIFY_PARAMS, _PTR_ FTOM_REQ_SET_NOTIFY_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_SET_NOTIFY_PARAMS, _PTR_ FTOM_RESP_SET_NOTIFY_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTOM_MSG			xMsg;
}	FTOM_REQ_NOTIFY_PARAMS, _PTR_ FTOM_REQ_NOTIFY_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTOM_MSG			xMsg;
}	FTOM_RESP_NOTIFY_PARAMS, _PTR_ FTOM_RESP_NOTIFY_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_TRIGGER			xTrigger;
}	FTOM_REQ_ADD_TRIGGER_PARAMS, _PTR_ FTOM_REQ_ADD_TRIGGER_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_CHAR			pTriggerID[FTM_ID_LEN+1];
}	FTOM_RESP_ADD_TRIGGER_PARAMS, _PTR_ FTOM_RESP_ADD_TRIGGER_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pTriggerID[FTM_ID_LEN+1];
}	FTOM_REQ_DELETE_TRIGGER_PARAMS, _PTR_ FTOM_REQ_DELETE_TRIGGER_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_DELETE_TRIGGER_PARAMS, _PTR_ FTOM_RESP_DELETE_TRIGGER_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
}	FTOM_REQ_GET_TRIGGER_COUNT_PARAMS, _PTR_ FTOM_REQ_GET_TRIGGER_COUNT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_ULONG			ulCount;
}	FTOM_RESP_GET_TRIGGER_COUNT_PARAMS, _PTR_ FTOM_RESP_GET_TRIGGER_COUNT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pTriggerID[FTM_ID_LEN+1];
}	FTOM_REQ_GET_TRIGGER_PARAMS, _PTR_ FTOM_REQ_GET_TRIGGER_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_TRIGGER			xTrigger;
}	FTOM_RESP_GET_TRIGGER_PARAMS, _PTR_ FTOM_RESP_GET_TRIGGER_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_ULONG			ulIndex;
}	FTOM_REQ_GET_TRIGGER_AT_PARAMS, _PTR_ FTOM_REQ_GET_TRIGGER_AT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_TRIGGER			xTrigger;
}	FTOM_RESP_GET_TRIGGER_AT_PARAMS, _PTR_ FTOM_RESP_GET_TRIGGER_AT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pTriggerID[FTM_ID_LEN+1];
	FTM_TRIGGER_FIELD	xFields;
	FTM_TRIGGER			xTrigger;
}	FTOM_REQ_SET_TRIGGER_PARAMS, _PTR_ FTOM_REQ_SET_TRIGGER_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_SET_TRIGGER_PARAMS, _PTR_ FTOM_RESP_SET_TRIGGER_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_ACTION			xAction;
}	FTOM_REQ_ADD_ACTION_PARAMS, _PTR_ FTOM_REQ_ADD_ACTION_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_CHAR			pID[FTM_ID_LEN+1];
}	FTOM_RESP_ADD_ACTION_PARAMS, _PTR_ FTOM_RESP_ADD_ACTION_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pID[FTM_ID_LEN+1];
}	FTOM_REQ_DELETE_ACTION_PARAMS, _PTR_ FTOM_REQ_DELETE_ACTION_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_DELETE_ACTION_PARAMS, _PTR_ FTOM_RESP_DELETE_ACTION_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
}	FTOM_REQ_GET_ACTION_COUNT_PARAMS, _PTR_ FTOM_REQ_GET_ACTION_COUNT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_ULONG			ulCount;
}	FTOM_RESP_GET_ACTION_COUNT_PARAMS, _PTR_ FTOM_RESP_GET_ACTION_COUNT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pActionID[FTM_ID_LEN+1];
}	FTOM_REQ_GET_ACTION_PARAMS, _PTR_ FTOM_REQ_GET_ACTION_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_ACTION			xAction;
}	FTOM_RESP_GET_ACTION_PARAMS, _PTR_ FTOM_RESP_GET_ACTION_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_ULONG			ulIndex;
}	FTOM_REQ_GET_ACTION_AT_PARAMS, _PTR_ FTOM_REQ_GET_ACTION_AT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_ACTION			xAction;
}	FTOM_RESP_GET_ACTION_AT_PARAMS, _PTR_ FTOM_RESP_GET_ACTION_AT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pActionID[FTM_ID_LEN+1];
	FTM_ACTION_FIELD	xFields;
	FTM_ACTION			xAction;
}	FTOM_REQ_SET_ACTION_PARAMS, _PTR_ FTOM_REQ_SET_ACTION_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_SET_ACTION_PARAMS, _PTR_ FTOM_RESP_SET_ACTION_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RULE			xRule;
}	FTOM_REQ_ADD_RULE_PARAMS, _PTR_ FTOM_REQ_ADD_RULE_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_CHAR			pRuleID[FTM_ID_LEN+1];
}	FTOM_RESP_ADD_RULE_PARAMS, _PTR_ FTOM_RESP_ADD_RULE_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pRuleID[FTM_ID_LEN+1];
}	FTOM_REQ_DELETE_RULE_PARAMS, _PTR_ FTOM_REQ_DELETE_RULE_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_DELETE_RULE_PARAMS, _PTR_ FTOM_RESP_DELETE_RULE_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
}	FTOM_REQ_GET_RULE_COUNT_PARAMS, _PTR_ FTOM_REQ_GET_RULE_COUNT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_ULONG			ulCount;
}	FTOM_RESP_GET_RULE_COUNT_PARAMS, _PTR_ FTOM_RESP_GET_RULE_COUNT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pRuleID[FTM_ID_LEN+1];
}	FTOM_REQ_GET_RULE_PARAMS, _PTR_ FTOM_REQ_GET_RULE_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_RULE			xRule;
}	FTOM_RESP_GET_RULE_PARAMS, _PTR_ FTOM_RESP_GET_RULE_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_ULONG			ulIndex;
}	FTOM_REQ_GET_RULE_AT_PARAMS, _PTR_ FTOM_REQ_GET_RULE_AT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_RULE			xRule;
}	FTOM_RESP_GET_RULE_AT_PARAMS, _PTR_ FTOM_RESP_GET_RULE_AT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pRuleID[FTM_ID_LEN+1];
	FTM_RULE_FIELD		xFields;
	FTM_RULE			xRule;
}	FTOM_REQ_SET_RULE_PARAMS, _PTR_ FTOM_REQ_SET_RULE_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_SET_RULE_PARAMS, _PTR_ FTOM_RESP_SET_RULE_PARAMS_PTR;


/**********************************************************************
 * Log 
 **********************************************************************/
typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_ULONG			ulIndex;
	FTM_ULONG			ulCount; 
}	FTOM_REQ_GET_LOG_LIST_PARAMS, _PTR_	FTOM_REQ_GET_LOG_LIST_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_ULONG			ulCount; 
	FTM_LOG 			pLogs[];
}	FTOM_RESP_GET_LOG_LIST_PARAMS, _PTR_ FTOM_RESP_GET_LOG_LIST_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_REQ_GET_LOG_COUNT_PARAMS, _PTR_ FTOM_REQ_GET_LOG_COUNT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_ULONG			ulCount;
}	FTOM_RESP_GET_LOG_COUNT_PARAMS, _PTR_ FTOM_RESP_GET_LOG_COUNT_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_ULONG 			ulIndex; 
	FTM_ULONG			ulCount;
}	FTOM_REQ_DELETE_LOG_PARAMS, _PTR_ FTOM_REQ_DELETE_LOG_PARAMS_PTR; 

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_ULONG			ulCount;
}	FTOM_RESP_DELETE_LOG_PARAMS, _PTR_ 	FTOM_RESP_DELETE_LOG_PARAMS_PTR;

/**********************************************************************
 * Discovery
 **********************************************************************/
typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_CHAR			pIP[256];
	FTM_USHORT			usPort;
	FTM_ULONG			ulRetryCount;
}	FTOM_REQ_START_DISCOVERY_PARAMS, _PTR_ FTOM_REQ_START_DISCOVERY_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
}	FTOM_RESP_START_DISCOVERY_PARAMS, _PTR_ FTOM_RESP_START_DISCOVERY_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
}	FTOM_REQ_GET_DISCOVERY_INFO_PARAMS, _PTR_ FTOM_REQ_GET_DISCOVERY_INFO_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_BOOL			bFinished;
	FTM_ULONG			ulNodeCount;
	FTM_ULONG			ulEPCount;
}	FTOM_RESP_GET_DISCOVERY_INFO_PARAMS, _PTR_ FTOM_RESP_GET_DISCOVERY_INFO_PARAMS_PTR;


typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_ULONG			ulIndex;
	FTM_ULONG			ulCount;
}	FTOM_REQ_GET_DISCOVERY_NODE_LIST_PARAMS, _PTR_ FTOM_REQ_GET_DISCOVERY_NODE_LIST_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_ULONG			ulCount;
	FTM_NODE			pNodeList[];
}	FTOM_RESP_GET_DISCOVERY_NODE_LIST_PARAMS, _PTR_ FTOM_RESP_GET_DISCOVERY_NODE_LIST_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_ULONG			ulIndex;
	FTM_ULONG			ulCount;
}	FTOM_REQ_GET_DISCOVERY_EP_LIST_PARAMS, _PTR_ FTOM_REQ_GET_DISCOVERY_EP_LIST_PARAMS_PTR;

typedef	struct
{
	FTM_ULONG			ulReqID;
	FTOM_CMD			xCmd;
	FTM_ULONG			ulLen;
	FTM_RET				xRet;
	FTM_ULONG			ulCount;
	FTM_EP				pEPList[];
}	FTOM_RESP_GET_DISCOVERY_EP_LIST_PARAMS, _PTR_ FTOM_RESP_GET_DISCOVERY_EP_LIST_PARAMS_PTR;

#endif
