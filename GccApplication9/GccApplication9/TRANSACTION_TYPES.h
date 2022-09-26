/*
 * IncFile2.h
 *
 * Created: 8/1/2019 1:12:27 PM
 *  Author: mdash
 */ 
/*
	Each transaction string has the following format
	TRANSACTION_TYPE name password amount
*/

#define INVALID_TRANSACTION 0
#define REGULAR_TRANSACTION 1
#define TRANSACTION_WITH_PASSWORD 2
#define SERVER_NOT_CONNECTED 3
#define INVALID_RFID 4