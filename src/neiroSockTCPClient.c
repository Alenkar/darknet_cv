/*  Copyright 2017 Vitaliy Feshin, Petr Ermilov, Anas Alotaki
neiroSockTCPClient.cpp - Получение данных от нейросети.
*/

#include <pthread.h>
#include "neiroSockTCPClient.h"
#include "userConfig.h"
#include "globalFun.h"
#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include "Otr.h"
#include "json/json.h"
#include "json/json-forwards.h"
#include <ctime>

using namespace std;

bool threadneirosockalirt = true;
bool threadneirosockalirtWork;
string neiroCls;
string outputjSon_TCP_neiro = "нейросеть хачу!\n";

void *sockthread_neiro_tcprt_function(void *arg) {
    try {
        ClientSocket client_socket ( NeiroAddress, NeiroPort );
        string reply;
        string fullReply;
        threadneirosockalirtWork = true;
        while (threadneirosockalirt) {
            usleep (3);
            int nAll = 0;
            /////ОТПРАВКА ДАННЫХ В НЕЙРОСЕТЬ И ПОЛУЧЕНИЕ МАССИВА//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            Json::Value reg_root;
            if (CamTypeL[camNum] == 3) {
                reg_root["channel"] = 1;
            } else {
                reg_root["channel"] = 2;
            }
            reg_root["ip"] = CamAddressL[camNum];
            reg_root["port"] = "0";
            Json::FastWriter fastWriter;
            string outputjSon_TCP_neiro = fastWriter.write(reg_root);
            try {
                timeStartG(2);
                client_socket << outputjSon_TCP_neiro;
                client_socket >> reply;
                fullReply += reply;
                size_t pos = fullReply.find("\n\n\n\n\n");
                if (pos != string::npos) {
                    reply = fullReply;
                    fullReply = "";
                    if (reply != "") {
                        timeStartG(2);
                        yesNeiroData = true;
                        Json::Reader jsonReader;
                        Json::Value drevoJson;
                        bool parsingOk = jsonReader.parse(reply, drevoJson);
                        Json::FastWriter fastWriter;
                        if (parsingOk == true && jsonReader.good() && !drevoJson.empty() && drevoJson.size() > 0) {
                            if (drevoJson.isMember("frame")) {
                                neiroFrame = drevoJson["frame"].asInt();
                            } else {
                                neiroFrame = 0;
                            }
                            for (Json::ValueIterator it = drevoJson["objects"].begin(); it != drevoJson["objects"].end(); it ++) {
                                timeStartG(2);
                                string output = fastWriter.write(*it);
                                Json::Reader jsonReaderObj;
                                Json::Value drevoJsonObj;
                                bool parsingSuccessful1 = jsonReaderObj.parse(output, drevoJsonObj);
                                if (parsingSuccessful1) {
                                    if (drevoJsonObj.isMember("clazz")) {
                                        neiroCls = drevoJsonObj["clazz"].asString();
                                    }
                                    if (neiroClassSelect == "all" || neiroClassSelect == neiroCls) {
                                        nAll ++;
                                        if (drevoJsonObj.isMember("lostCount")) {
                                            neiroLostCount[nAll] = drevoJsonObj["lostCount"].asInt();
                                        } else {
                                            neiroLostCount[nAll] = 0;
                                        }
                                        if (drevoJsonObj.isMember("id")) {
                                            neiroId[nAll] = drevoJsonObj["id"].asString();
                                        } else {
                                            neiroId[nAll] = "";
                                        }
                                        if (drevoJsonObj.isMember("clazz")) {
                                            neiroClass[nAll] = drevoJsonObj["clazz"].asString();
                                        } else {
                                            neiroClass[nAll] = "";
                                        }
                                        if (drevoJsonObj.isMember("x")) {
                                            neiroX[nAll] = drevoJsonObj["x"].asInt();
                                        } else {
                                            neiroX[nAll] = 0;
                                        }
                                        if (drevoJsonObj.isMember("y")) {
                                            neiroY[nAll] = drevoJsonObj["y"].asInt();
                                        } else {
                                            neiroY[nAll] = 0;
                                        }
                                        if (drevoJsonObj.isMember("width")) {
                                            neiroW[nAll] = drevoJsonObj["width"].asInt();
                                        } else {
                                            neiroW[nAll] = 0;
                                        }
                                        if (drevoJsonObj.isMember("height")) {
                                            neiroH[nAll] = drevoJsonObj["height"].asInt();
                                        } else {
                                            neiroH[nAll] = 0;
                                        }
                                        if (neiroX[nAll] + neiroW[nAll] > camWidthL) neiroW[nAll] = camWidthL - neiroX[nAll];
                                        if (neiroY[nAll] + neiroH[nAll] > camHeightL) neiroH[nAll] = camHeightL - neiroY[nAll];
                                        neiroBlock[nAll] = cv::Rect(neiroX[nAll],neiroY[nAll],neiroW[nAll],neiroH[nAll]);
                                    }
                                }
                            }
                            neiroAll = nAll;
                        }
                    } else {
                        string tsg = to_string(timeStopG(2));
                        WriteLog(rtlogfile, tsg + "\n");
                    }
                }
            }
            catch ( SocketException& ) {}
            /////END ОТПРАВКА ДАННЫХ В НЕЙРОСЕТЬ И ПОЛУЧЕНИЕ МАССИВА//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }
        threadneirosockalirtWork = false;
    }
    catch ( SocketException& e )
    {
        char string1[1024];
        sprintf(string1, "[RoboTrack] Exception was caught: `%s'\n", e.description().c_str());
        WriteLog(rtlogfile, string1);
    }
    pthread_exit(0);
}


