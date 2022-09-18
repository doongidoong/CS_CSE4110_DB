#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "mysql.h"


#pragma comment(lib, "libmysql.lib")

const char* host = "localhost";
const char* user = "root";
const char* pw = "mysql";
const char* db = "project";

MYSQL* connection = NULL;
MYSQL conn;
MYSQL_RES* sql_result;
MYSQL_ROW sql_row;



void printMenu();
int type1(); int type2(); int type3(); int type4(); int type5(); int type6(); int type7();

void db_init(FILE* fp); void db_close(FILE* fp);


int main(void) {
	FILE* fp = fopen("20160608.txt", "r");
	char dlq[500];
	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}
	
	else
	{
		printf("Connection Succeed\n");
		if (mysql_select_db(&conn, db))
		{
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}
		//init
		int crt_num;
		fscanf(fp, "%d", &crt_num);
		int cnt = 0;
		while (cnt < crt_num) {
			if (feof(fp)) break;
			fgets(dlq, 500, fp);
			mysql_query(connection, dlq);
			cnt++;
		}
		printf("***** DATEBASE init *****\n\n");
		int types = 0;
		while (1) {
			printMenu();
			printf("Enter the Type : ");
			scanf("%d", &types);
			printf("\n\n");
			if (types == 0) break;
			switch (types)
			{
			case 1:
				type1();
				break;
			case 2:
				type2();
				break;
			case 3:
				type3();
				break;
			case 4:
				type4();
				break;
			case 5:
				type5();
				break;
			case 6:
				type6();				
				break;
			case 7:
				type7();
				break;
			default:
				break;
			}
		}
		/*const char* query = "select * from student";
		int state = 0;

		state = mysql_query(connection, query);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("%s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
			}
			mysql_free_result(sql_result);
		}*/

		
		while (1) {
			if (feof(fp)) break;
			fgets(dlq, 500, fp);
			mysql_query(connection, dlq);
		}
		printf("***** DATEBASE Close *****\n");
		mysql_close(connection);
	}

	fclose(fp);
	return 0;
}



int type1() {
	int subtypes = -1;
	int state = 0;
	char query[500];
	int x = -1;
	char track[6];
	int tuple = 1000;
	sprintf(query, "select count(*) from shipping");
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			tuple = atoi(sql_row[0]);
		}
		mysql_free_result(sql_result);
	}

	while (1){
		printf("------- TYPE 1 -------\n\n");
		printf("Enter Tracking number X(0 = go to menu)  ");
		scanf("%d", &x);
		if (x == 0) return 1;
		if (tuple > x)break;
		printf("**** out of range ****\n");
	}
	sprintf(track, "tr%03d",x);
	sprintf(query, "select C.customer_id, C.phone_number from customer C, (select distinct B.customer_id from (select * from ordereditem where tracking_num='tr%03d') A, ordered B where A.order_code = B.order_code) D where C.customer_id = D.customer_id;",x);
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		printf("\n\n=============================\n");
		printf(" customer_id    phone-number \n");
		printf("=============================\n");
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			//printf("%s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
			printf("%11s  %11s \n", sql_row[0], sql_row[1]);
		}
		mysql_free_result(sql_result);
		printf("=============================\n");
		printf("\n\n");
	}

	while(1){
		printf("------- Subtypes in TYPE 1 -------\n\n");
		printf("\t1. TYPE 1-1\n");
		printf("Enter subtypes (0 = go to menu)  ");
		scanf("%d", &subtypes);
		printf("\n\n");
		if (subtypes == 0) return 1;
		if (subtypes == 1){
			sprintf(query, "select order_code, tracking_num, product_id from ordereditem where tracking_num = '%s';", track);
			state = mysql_query(connection, query);
			char target[6];
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				printf("\n\n========================================\n");
				printf(" order_code   tracking_num   product_id\n");
				printf("========================================\n");
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
				{
					printf("%11s %11s %11s\n", sql_row[0], sql_row[1], sql_row[2]);
					strcpy(target, sql_row[0]);
				}
				mysql_free_result(sql_result);
				printf("========================================\n");
				printf("\n\n");
				
			}

			printf("complete creating a new shipment of replacement items.\n");

			sprintf(query, "insert into shipping values ('tr%03d', 'co001',Date_Format(now(), '%%Y-%%m-%%d'),null,null);", tuple + 1);
			mysql_query(connection, query);

			sprintf(query, "update ordereditem set tracking_num = 'tr%03d' where order_code = '%s';", tuple+1,target);
			mysql_query(connection, query);

			sprintf(query, "select order_code, tracking_num, product_id from ordereditem where tracking_num = 'tr%03d';", tuple+1);
			state = mysql_query(connection, query);
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				printf("\n\n========================================\n");
				printf(" order_code   tracking_num   product_id\n");
				printf("========================================\n");
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
				{
					printf("%11s %11s %11s\n", sql_row[0], sql_row[1], sql_row[2]);
				}
				mysql_free_result(sql_result);
				printf("========================================\n");
				printf("\n\n");

			}

			break;
		}
	}
	return 0;

}
int type2() {
	int subtypes = -1;
	int state = 0;
	char query[1000];
	printf("------- TYPE 2 -------\n\n");

	char cus[6];
	sprintf(query, "select customer_id, sum(price) amount from (select order_code, B.price from ordereditem A, product B where A.product_id = B.product_id) C, ordered D where C.order_code=D.order_code and YEAR(date) = YEAR(CURRENT_DATE - INTERVAL 1 YEAR) group by customer_id order by sum(price) desc limit 1;");
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		printf("\n=============================\n");
		printf(" customer_id      amount       \n");
		printf("=============================\n");
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{	
			//printf("%s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
			sprintf(cus, "%s", sql_row[0]);
			printf("%11s  %11s \n", sql_row[0], sql_row[1]);
		}
		printf("=============================\n");
		printf("\n\n");
		mysql_free_result(sql_result);
	}

	while(subtypes!=0){
		printf("------- Subtypes in TYPE 2 -------\n\n");
		printf("\t1. TYPE 2-1\n");
		printf("Enter subtypes (0 = go to menu)  ");
		scanf("%d", &subtypes);
		printf("\n\n");
		if (subtypes == 0) return 1;
		if (subtypes == 1){
			sprintf(query, "with procnt as(select product_id, count(*) cnt from(select order_code from ordered where customer_id = '%s' and YEAR(date) = YEAR(CURRENT_DATE - INTERVAL 1 YEAR)) A, ordereditem B where A.order_code = B.order_code group by product_id) select * from procnt where cnt = (select max(cnt) from procnt);",cus);
			state = mysql_query(connection, query);
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				printf("\n\n=============================\n");
				printf(" product_id      amount       \n");
				printf("=============================\n");
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
				{
					//printf("%s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);

					printf("%11s  %11s \n", sql_row[0], sql_row[1]);
				}
				printf("=============================\n");
				printf("\n\n");
				mysql_free_result(sql_result);
			}
			break;
		}
	}
	return 1;

}

int type3() {
	int subtypes = -1;
	int state = 0;
	char query[1000];
	int k = -1;
	printf("------- TYPE 3 -------\n\n");
	sprintf(query, "select distinct product_id from ordereditem A , (select * from ordered where YEAR(date) = YEAR(CURRENT_DATE - INTERVAL 1 YEAR)) B where A.order_code = B.order_code;");
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		printf("\n===================\n");
		printf(" product_id    \n");
		printf("===================\n");
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("%11s \n", sql_row[0]);
		}
		printf("===================\n");
		printf("\n\n");
		mysql_free_result(sql_result);
	}
	while(subtypes!=0){
		printf("------- Subtypes in TYPE 3 -------\n\n");
		printf("\t1. TYPE 3-1\n");
		printf("\t2. TYPE 3-2\n");
		printf("Enter subtypes (0 = go to menu)  ");
		scanf("%d", &subtypes);
		printf("\n\n");
		if (subtypes == 1) {
			printf("** Find the  top k products by dollar-amount sold.**\n");
			printf(" Which K? : (<1000 , 0 = go to back)\t");
			scanf("%d", &k);
			if (k == 0) continue;
			sprintf(query, "select C.product_id, sum(D.price) amount from (select product_id from ordereditem A , (select * from ordered where YEAR(date) = YEAR(CURRENT_DATE - INTERVAL 1 YEAR)) B where A.order_code = B.order_code ) C, product D where C.product_id =D.product_id group by C.product_id order by sum(D.price) desc limit %d;",k);
			state = mysql_query(connection, query);
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				printf("\n=============================\n");
				printf(" product_id      amount       \n");
				printf("=============================\n");
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
				{
					printf("%11s  %11s \n", sql_row[0], sql_row[1]);
				}
				printf("=============================\n");
				printf("\n\n");
				mysql_free_result(sql_result);
			}
		}
		if (subtypes == 2) {
			sprintf(query, "select * from (select  E.product_id, PERCENT_RANK() over (order by E.amount desc) as percent from (select C.product_id, sum(D.price) amount from (select product_id from ordereditem A , (select * from ordered where YEAR(date) = YEAR(CURRENT_DATE - INTERVAL 1 YEAR)) B where A.order_code = B.order_code ) C, product D where C.product_id =D.product_id group by C.product_id) E) G where G.percent <=0.1;");
			printf("*********** top 10 %% ***********\n");
			state = mysql_query(connection, query);
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				printf("\n=============================\n");
				printf(" product_id    rank(p)      \n");
				printf("=============================\n");
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
				{
					printf("%11s  %11s \n", sql_row[0], sql_row[1]);
				}
				printf("=============================\n");
				printf("\n\n");
				mysql_free_result(sql_result);
			}
		}
	}
	return 1;
}
int type4() {
	int subtypes = -1;
	int state = 0;
	char query[1000];
	int k = -1;
	printf("------- TYPE 4 -------\n\n");
	sprintf(query, "select product_id, count(*) unit from ordereditem A , (select * from ordered where YEAR(date) = YEAR(CURRENT_DATE - INTERVAL 1 YEAR)) B where A.order_code = B.order_code group by product_id;");
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		printf("\n=============================\n");
		printf(" product_id    unit       \n");
		printf("=============================\n");
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("%11s  %11s \n", sql_row[0], sql_row[1]);
		}
		printf("=============================\n");
		printf("\n\n");
		mysql_free_result(sql_result);
	}
	while (subtypes != 0) {
		printf("------- Subtypes in TYPE 4 -------\n\n");
		printf("\t1. TYPE 4-1\n");
		printf("\t2. TYPE 4-2\n");
		printf("Enter subtypes (0 = go to menu)  ");
		scanf("%d", &subtypes);
		printf("\n\n");
		if (subtypes == 1) {
			printf("** Find the  top k products by unit sales. **\n");
			printf(" Which K? : (<1000 , 0 = go to back)\t");
			scanf("%d", &k);
			if (k == 0) continue;
			sprintf(query, "select product_id, count(*) unit from ordereditem A , (select * from ordered where YEAR(date) = YEAR(CURRENT_DATE - INTERVAL 1 YEAR)) B where A.order_code = B.order_code group by product_id order by unit desc limit %d;", k);
			state = mysql_query(connection, query);
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				printf("\n=============================\n");
				printf(" product_id    unit       \n");
				printf("=============================\n");
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
				{
					printf("%11s  %11s \n", sql_row[0], sql_row[1]);
				}
				printf("=============================\n");
				printf("\n\n");
				mysql_free_result(sql_result);
			}
		}
		if (subtypes == 2) {
			sprintf(query, "select * from (select  E.product_id, E.unit ,PERCENT_RANK() over (order by E.unit desc) as percent from (select product_id, count(*) unit from ordereditem A , (select * from ordered where YEAR(date) = YEAR(CURRENT_DATE - INTERVAL 1 YEAR)) B where A.order_code = B.order_code group by product_id) E )  G where G.percent <=0.1;");
			state = mysql_query(connection, query);
			printf("*********** top 10 %% ***********\n");
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				printf("\n=======================================\n");
				printf(" product_id         unit      rank(p)      \n");
				printf("=======================================\n");
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
				{
					printf("%11s %11s %11s \n", sql_row[0], sql_row[1], sql_row[2]);
				}
				printf("=======================================\n");
				printf("\n\n");
				mysql_free_result(sql_result);
			}
		}
	}
	return 1;
}
int type5() {
	int state = 0;
	char query[500];
	printf("------- TYPE 5 -------\n\n");
	sprintf(query, "select distinct A.product_id, A.amount, A.location_id, address from storedin A,(select * from location where address like 'California%%' and type = 'store') B where A.location_id = B.location_id and amount = 0;");
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		printf("\n============================================================\n");
		printf(" product_id         amount      location_id        address    \n");
		printf("============================================================\n");
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("%11s  %11s %11s %20s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
		}
		printf("============================================================\n");
		printf("\n\n");
		mysql_free_result(sql_result);
	}

	return 1;
}
int type6() {
	printf("------- TYPE 6 -------\n\n");
	int state = 0;
	char query[500];
	sprintf(query, "select * from shipping where  exp_date < arr_date or (arr_date is null and  exp_date < now());");
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		printf("\n===========================================================================\n");
		printf(" product_id     company_id       req_date      exp_date        arr_date    \n");
		printf("===========================================================================\n");
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("%11s  %11s %15s %15s %15s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4]);
		}
		printf("===========================================================================\n");
		printf("\n\n");
		mysql_free_result(sql_result);
	}

	return 1;
}
int type7() {
	printf("------- TYPE 7 -------\n\n");
	int state = 0;
	char query[500];
	int tuple = 1000;
	sprintf(query, "select count(*) from customer");
	state = mysql_query(connection, query);
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			tuple = atoi(sql_row[0]);
		}
		mysql_free_result(sql_result);
	}
	int sum;
	FILE* output_fp;
	char filename[20];
	for(int i=1;i<tuple+1;i++){
		sum = 0;
		sprintf(query, "select customer_id, product_id, price, date from (select order_code, A.product_id, B.price from ordereditem A, product B where A.product_id = B.product_id) C, ordered D where C.order_code=D.order_code and  YEAR(date) = YEAR(date - INTERVAL 1 MONTH) AND MONTH(date) = MONTH(CURRENT_DATE - INTERVAL 1 MONTH) and customer_id = 'cu%03d';",i);
		sprintf(filename, "BILL-%d.txt", i);
		output_fp = fopen(filename, "w");
		fprintf(output_fp,"\n*********     BILL     *********");
		state = mysql_query(connection, query);
		if (state == 0) {
			sql_result = mysql_store_result(connection);
			fprintf(output_fp,"\n=====================================================\n");
			fprintf(output_fp," customer_id    product_id       price      date     \n");
			fprintf(output_fp,"=====================================================\n");
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				fprintf(output_fp, "%11s  %11s %15s %15s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
				sum += atoi(sql_row[2]);
			}
			fprintf(output_fp,"=====================================================\n");
			fprintf(output_fp,"\n\n");
			fprintf(output_fp, "total : %d",sum);
			mysql_free_result(sql_result);
		}
		fclose(output_fp); 
	}
	printf("Complete the generating bills of %d cusotomers\n\n", tuple);
	return 1;
}

void printMenu(){
	printf("------- SELECT QUERY TYPES -------\n\n");
	printf("\t1. TYPE 1\n");
	printf("\t2. TYPE 2\n");
	printf("\t3. TYPE 3\n");
	printf("\t4. TYPE 4\n");
	printf("\t5. TYPE 5\n");
	printf("\t6. TYPE 6\n");
	printf("\t7. TYPE 7\n");
	printf("\t0. QUIT\n");
}

