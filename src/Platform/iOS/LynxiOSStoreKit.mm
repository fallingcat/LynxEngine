//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/
//
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Kasaki Tzeng : 2016/07/18
//  Last Update :
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <LynxStoreKit.h>
namespace LynxEngine
{
    STOREKITRESULTCODE GetEnum(int errorCode)
    {
        using namespace LynxEngine::iOS;
        
        switch (errorCode)
        {
            case SKErrorUnknown:
            default:
                return SKRC_UNKNOWN_ERROR;
                break;
                
            case SKErrorPaymentCancelled:
                return SKRC_USER_CANCELLED;
                break;
                
            case SKErrorStoreProductNotAvailable:
                return SKRC_ITEM_UNAVAILABLE;
                break;
                
            case SKErrorPaymentNotAllowed:
            case SKErrorPaymentInvalid:
                return SKRC_GAMESERVER_VERIFY_FAILED;
                break;
                
            case SKErrorClientInvalid:
            case SKErrorCloudServicePermissionDenied:
            case SKErrorCloudServiceNetworkConnectionFailed:
                return SKRC_UNKNOWN_ERROR;
                break;
                
            case SKRC_USER_CANCELLED:
            // case SKRC_SERVICE_NOT_INSTALL:
            // case SKRC_INIT_IAP_FAILED:
            // case SKRC_IABHELPER_EXCEPITION:
            case SKRC_ITEM_UNAVAILABLE:
            case SKRC_GAMESERVER_REQUEST_FAILED:
            case SKRC_GAMESERVER_VERIFY_FAILED:
                return (STOREKITRESULTCODE)errorCode;
                break;
        }
        
        return SKRC_UNKNOWN_ERROR;
    }
}

#define MAKE_JSON_STRING_VALUE(x) \
    LynxEngine::CJsonValue([(x) cStringUsingEncoding:NSUTF8StringEncoding])

@implementation LynxStoreKit
@synthesize theSKProductList;

static __strong LynxStoreKit * s_sharedInstance = nil;

+ (LynxStoreKit*) sharedInstance
{
    static dispatch_once_t lock;
    dispatch_once(&lock, ^{
        s_sharedInstance = [[LynxStoreKit alloc] init];
        
        [[SKPaymentQueue defaultQueue] addTransactionObserver:s_sharedInstance];
    });
    return s_sharedInstance;
}

- (void) dealloc
{
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:s_sharedInstance];
    
    theSKProductList = nil;
    s_sharedInstance = nil;
}

#pragma mark - GameFramework interface bridge

- (void) queryInventory:(NSArray* )productList
{
    SKProductsRequest* request = [[SKProductsRequest alloc] initWithProductIdentifiers:[NSSet setWithArray:productList]];
    request.delegate = self;
    [request start];
}

- (void) purchaseInventory:(NSString* )productId withPayload:(NSString*)payload
{
    for (SKProduct* p in theSKProductList)
    {
        if ([[p productIdentifier] isEqualToString:productId]) {
            NSLog(@"Ready to request payment: %@, payload: %@", productId, payload);
            
            SKMutablePayment* payment = [SKMutablePayment paymentWithProduct:p];
            payment.applicationUsername = payload;
            [[SKPaymentQueue defaultQueue] addPayment:payment];
        }
    }
}

#pragma mark - SKProductsRequestDelegate protocol

- (void) productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    theSKProductList = response.products;
    
    for (NSString* invalidIdentifer in response.invalidProductIdentifiers) {
        NSLog(@"InvalidIdentifer = %@", invalidIdentifer);
    }
    
    // fetch product list
    LynxEngine::CArray<LynxEngine::CJsonValue> productList;
    for (SKProduct* p in theSKProductList) {
        
        NSNumberFormatter* formatter = [[NSNumberFormatter alloc] init];
        [formatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
        [formatter setNumberStyle:NSNumberFormatterCurrencyStyle];
        [formatter setLocale:p.priceLocale];
        
		NSLog(@"%@", [p localizedTitle]);
		NSLog(@"%@", [p localizedDescription]);
		NSLog(@"%@", [p.priceLocale localeIdentifier]);
		NSLog(@"%@", [p productIdentifier]);
				
        LynxEngine::CJsonValue SKJson;
        SKJson["title"] = [p localizedTitle] 
			? MAKE_JSON_STRING_VALUE([p localizedTitle]) : MAKE_JSON_STRING_VALUE(@"Unknown-Title");
        SKJson["price"] = MAKE_JSON_STRING_VALUE([formatter stringFromNumber:p.price]);
        SKJson["type"] = MAKE_JSON_STRING_VALUE(@"inapp");
        SKJson["description"] = [p localizedDescription]
			? MAKE_JSON_STRING_VALUE([p localizedDescription]) : MAKE_JSON_STRING_VALUE(@"Unknown-Description");
        SKJson["price_locale"] = MAKE_JSON_STRING_VALUE([p.priceLocale localeIdentifier]);
        SKJson["productId"] = MAKE_JSON_STRING_VALUE([p productIdentifier]);
        
        // metadata
        NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
        [numberFormatter setLocale:[p priceLocale]];
        SKJson["price_currency_code"] = MAKE_JSON_STRING_VALUE([formatter currencyCode]);
        
        NSDecimalNumber* priceInCents = [p.price decimalNumberByMultiplyingBy:[NSDecimalNumber decimalNumberWithString:@"1"]];
        SKJson["price_amount_micros"] = LynxEngine::CJsonValue([priceInCents intValue]);
        productList.push_back(SKJson);
    }
    
    // try complete teansactions with unconsumed inventory.
    NSArray<SKPaymentTransaction*> *transactions = [[SKPaymentQueue defaultQueue] transactions];
    for (SKPaymentTransaction* transaction in transactions)
    {
        [self completeTransaction:transaction];
    }
    
    LynxEngine::GameFramework::iOS::CKernel* lpKernel = static_cast<LynxEngine::GameFramework::iOS::CKernel*>(glpKernel);
    if (lpKernel)
    {
        lpKernel->vOnFinishingQueryIAPList(productList);
    }
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
    LynxEngine::GameFramework::iOS::CKernel* lpKernel = static_cast<LynxEngine::GameFramework::iOS::CKernel*>(glpKernel);
    if (lpKernel)
    {
        int errCode = LynxEngine::GetEnum((int)[error code]);
        if (LynxEngine::SKRC_USER_CANCELLED != errCode)
		{
			LynxEngine::CString errMsg = [[error description] cStringUsingEncoding:NSUTF8StringEncoding];        
			lpKernel->vOnErrorPurchase(errCode, errMsg);
		}
    }
}

#pragma mark - SKPaymentTransactionObserver protocol

- (void) paymentQueue:(SKPaymentQueue*)queue updatedTransactions:(NSArray*)transactions
{
    NSLog(@"pending transactions:%lu", (unsigned long)[[queue transactions] count]);
    
    for (SKPaymentTransaction* transaction in transactions) {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
            
            case SKPaymentTransactionStateRestored:
                [self restoreTransaction:transaction];
                break;
            
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
            
            case SKPaymentTransactionStatePurchasing:
            case SKPaymentTransactionStateDeferred:
            default:
                break;
        }
    }
}

- (void) failedTransaction: (SKPaymentTransaction*) transaction
{
    NSLog(@"failedTransaction, date:%@, reason:%@", transaction.transactionDate, [transaction.error localizedFailureReason]);
    
    LynxEngine::GameFramework::iOS::CKernel* lpKernel = static_cast<LynxEngine::GameFramework::iOS::CKernel*>(glpKernel);
    if (lpKernel)
    {
        NSError* err = [transaction error];
        int errCode = LynxEngine::GetEnum((int)[err code]);
        LynxEngine::CString errMsg = [[err localizedDescription] cStringUsingEncoding:NSUTF8StringEncoding];
        if (LynxEngine::SKRC_USER_CANCELLED != errCode)
		{
			lpKernel->vOnErrorPurchase(errCode, errMsg);
        }        
    }
    
    [self finishTransaction:transaction];
}

- (void) completeTransaction: (SKPaymentTransaction*) transaction
{
    NSLog(@"completeTransaction, date:%@, desc=%@", [transaction transactionDate], [transaction debugDescription]);
    
    LynxEngine::CJsonValue Purchase;
    
    NSString* receipt = nil;
    NSDateFormatter * formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyy-MM-dd HH:mm::ss"];
    
    if (NSFoundationVersionNumber >= NSFoundationVersionNumber_iOS_7_0) {
        receipt = [self fetchReceipt];
    } else {
        // receipt = [[transaction transactionReceipt] base64EncodedStringWithOptions:0];
        receipt = [[NSString alloc] initWithData:[transaction transactionReceipt]
                                        encoding:NSASCIIStringEncoding];
    }
    
    NSLog(@"completeTransaction, receipt = %@", receipt);

    Purchase["orderId"] = MAKE_JSON_STRING_VALUE([transaction transactionIdentifier]);
    Purchase["packageName"] = MAKE_JSON_STRING_VALUE([[NSBundle mainBundle] bundleIdentifier]);
    Purchase["productId"] = MAKE_JSON_STRING_VALUE([[transaction payment] productIdentifier]);
    Purchase["purchaseTime"] = MAKE_JSON_STRING_VALUE([formatter stringFromDate:[transaction transactionDate]]);
    Purchase["purchaseState"] = LynxEngine::CJsonValue((int)[transaction transactionState]);
    Purchase["developerPayload"] = MAKE_JSON_STRING_VALUE([[transaction payment] applicationUsername]);
    Purchase["purchaseToken"] = MAKE_JSON_STRING_VALUE(receipt);
    
    // fill-in metadata
    Purchase["base64receipt"] = MAKE_JSON_STRING_VALUE(receipt);
   
    LYNXBOOL bConsume = LYNX_FALSE;
    LynxEngine::GameFramework::iOS::CKernel* lpKernel = static_cast<LynxEngine::GameFramework::iOS::CKernel*>(glpKernel);
    if (lpKernel)
    {
        bConsume = lpKernel->vOnVerifyPurchase(Purchase);
    }
    
    if (bConsume)
    {
        [self finishTransaction:transaction];
    }
}

- (void) restoreTransaction: (SKPaymentTransaction*) transaction
{
    [self finishTransaction:transaction];
}

- (void) consumeTransaction:(NSString*) productId
{
    NSArray<SKPaymentTransaction*> *transactions = [[SKPaymentQueue defaultQueue] transactions];
    for (SKPaymentTransaction* transaction in transactions)
    {
        if (productId == transaction.transactionIdentifier)
        {
            [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
            
            typedef LynxEngine::GameFramework::iOS::CKernel* LPKERNEL;
            LPKERNEL lpKernel = static_cast<LPKERNEL>(glpKernel);
            if (lpKernel)
            {
                LynxEngine::CJsonValue Purchase;
                LynxEngine::CString Message;
                Purchase["developerPayload"] = MAKE_JSON_STRING_VALUE([[transaction payment] applicationUsername]);
                
                lpKernel->vOnDeliverPurchase(LynxEngine::SKRC_SUCCESS, Message, Purchase);
            }
        }
    }
}

- (void) finishTransaction:(SKPaymentTransaction*) transaction;
{
    // consume purchase
    [self consumeTransaction:transaction.transactionIdentifier];
    
    typedef LynxEngine::GameFramework::iOS::CKernel* LPKERNEL;
    LPKERNEL lpKernel = static_cast<LPKERNEL>(glpKernel);
    if (lpKernel)
    {
        lpKernel->vOnFinishingPurchase(LynxEngine::SKRC_SUCCESS, _T("Purchase flow finished."));
    }
}

#pragma mark - get receipt for iOS 7.0

- (NSString*) fetchReceipt
{
    NSString* receiptString =  nil;
    NSURL* receiptURL = [[NSBundle mainBundle] appStoreReceiptURL];
    if ([[NSFileManager defaultManager] fileExistsAtPath:[receiptURL path]])
    {
        NSData* receiptData = [NSData dataWithContentsOfURL:receiptURL];
        receiptString = [receiptData base64EncodedStringWithOptions:0];
        // [self verifyReceipt:receiptData];
    }
    
    return receiptString;
}

#pragma mark - local verify receipt

- (void) verifyReceipt:(NSData*) receiptData
{
    // Create the JSON object that describes the request
    NSError *error;
    NSDictionary *requestContents = @{
        @"receipt-data": [receiptData base64EncodedStringWithOptions:0]
    };
    NSData *requestData = [NSJSONSerialization dataWithJSONObject:requestContents
                                                          options:0
                                                            error:&error];
    if (!requestData) {
        /* ... Handle error ... */
        NSLog(@"Invalid request data.");
    }
    
#define VERIFY_URL @"https://sandbox.itunes.apple.com/verifyReceipt"
    // #define VERIFY_URL @"https://buy.itunes.apple.com/verifyReceipt"
    NSLog(@"requestContents = %@\n, requestData = %@", [requestContents description], [requestData description]);
    
    // Create a POST request with the receipt data.
    NSURL *url = [NSURL URLWithString:VERIFY_URL];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    [request setHTTPMethod:@"POST"];
    [request setHTTPBody:requestData];
    
    // Make a connection to the iTunes Store on a background queue.
    NSOperationQueue *queue = [[NSOperationQueue alloc] init];
    [NSURLConnection sendAsynchronousRequest:request queue:queue
                           completionHandler:^(NSURLResponse *response, NSData *data, NSError *connectionError) {
                               if (connectionError) {
                                   /* ... Handle error ... */
                               } else {
                                   NSError *error;
                                   NSDictionary *jsonResponse = [NSJSONSerialization JSONObjectWithData:data options:0 error:&error];
                                   if (!jsonResponse) {
                                       /* ... Handle error ...*/
                                       NSLog(@"Invalid json response.");
                                   }
                                   
                                   /* ... Send a response back to the device ... */
                                   NSLog(@"Response = %@", [jsonResponse description]);
                               }
                           }];
    
}

@end
