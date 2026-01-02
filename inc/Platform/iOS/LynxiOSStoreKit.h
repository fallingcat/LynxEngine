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

#ifndef __LYNXIOSSTOREKIT_H__
#define __LYNXIOSSTOREKIT_H__

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

@interface LynxStoreKit : NSObject<SKProductsRequestDelegate, SKPaymentTransactionObserver> {
    NSMutableData *theReceivedData;
    NSArray* theSKProductList;
}

@property(strong) NSArray* theSKProductList;

+ (LynxStoreKit*) sharedInstance;

- (void) queryInventory:(NSArray* )productList;
- (void) purchaseInventory:(NSString* )productId withPayload:(NSString*)payload;
- (void) verifyReceipt:(NSData*) receiptData;
- (void) consumeTransaction:(NSString*) productId;
- (void) finishTransaction:(SKPaymentTransaction*) transaction;

- (void) productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response;
- (void) request:(SKRequest *)request didFailWithError:(NSError *)error;

- (void) paymentQueue:(SKPaymentQueue*)queue updatedTransactions:(NSArray*)transactions;
- (void) failedTransaction: (SKPaymentTransaction*) transaction;
- (void) completeTransaction: (SKPaymentTransaction*) transaction;
- (void) restoreTransaction: (SKPaymentTransaction*) transaction;

- (NSString*) fetchReceipt;
@end

#endif /* __LYNXIOSSTOREKIT_H__ */
