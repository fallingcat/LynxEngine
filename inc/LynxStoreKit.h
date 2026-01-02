#ifndef __LYNXSTOREKIT_H__
#define __LYNXSTOREKIT_H__

#include <LynxEngineClasses.h>
#include <LynxType.h>
#include <LynxObj.h>

#define RESPONSE_NEED_REPEAT_VERIFY	   4001

namespace LynxEngine
{
	typedef enum {		
		SKRC_SUCCESS					= 0,		

		SKRC_UNKNOWN_ERROR				= 30000, // 未知錯誤
		SKRC_USER_CANCELLED				= 30001, // 使用者中斷操作
		SKRC_SERVICE_NOT_INSTALL		= 30002, // 手機上面沒有 GooglePlay
		SKRC_INIT_IAP_FAILED			= 30003, // 內購尚未初始化或是初始化失敗 (IabHelper == NULL)
		SKRC_IABHELPER_EXCEPITION		= 30004, // IabHelper 執行異常
		SKRC_ITEM_UNAVAILABLE			= 30005, // Item already owned : 同一個內購品項還沒Consume、無法進行對該品項進行內購、但是可以購買其它品項
		SKRC_GAMESERVER_REQUEST_FAILED  = 30006, // 與 GameServer 連線失敗
		SKRC_GAMESERVER_VERIFY_FAILED   = 30007, // 與 GameServer 驗證訂單失敗
	} STOREKITRESULTCODE;

	typedef enum {
		TS_UNKNOWN      = -1,
		TS_SUCCEED      = 0,
		TS_INPURCHASE   = 1,
		TS_INVERIFY		= 2,
		TS_INCONSUME    = 3,
		
		NUM_TRANSACTIONSTATE,
	} TRANSACTIONSTATE;

	typedef struct
	{	
		LYNXLONGLONG TimeStamp;
		CString	UserID;
		CString Payload;
		int	TransactionState;
		CString Signature;		
	} CTransaction;

	class LYNXENGCLASS CTransactionRecord
	{
	protected:
		CArray<CTransaction> m_Transactions;

	public:
		CTransactionRecord() {}
		virtual ~CTransactionRecord() {}

		virtual void	vSave();
		virtual void	vLoad();
		virtual void	vWrite(const CString& payload, const CString& playerSN, int state, CString metadata=_T(""));
		virtual void	vRead();
		virtual void	vUpload();
	};
	
	void ComputePurchasePayload(const CString& playerSN, const CString& productId, CString& payload);
	void DecryptPurchasePayload(const CString& srcStr, CString& payload);
    
    STOREKITRESULTCODE GetEnum(int errorCode);
}
#endif
