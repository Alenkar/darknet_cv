/*  Copyright 2017 Vitaliy Feshin, Petr Ermilov, Anas Alotaki
neiroSockTCPClient.h - Получение данных от нейросети. Заголовочный файл.
*/

#ifndef NSOCKTCPCL_H_
#define NSOCKTCPCL_H_

extern bool threadneirosockalirtWork;
extern bool threadneirosockalirt;
void *sockthread_neiro_tcprt_function(void *arg);

#endif
