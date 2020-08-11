#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include "v_list.h"
#include "utils.h"

#define CONTRYLIMIT 60

bool HtmlRezGood;

String ssid = "RTK-16";
String password = "77865523489";
String srv_url = "http://192.168.1.30:8080";
String dev_num = "1";
String WebConf;

IPAddress MyIp;
bool Connected;

ESP8266WebServer server(80);

DynamicJsonDocument Configuration(2048);

Parameter* parameters;
int parametersCount;

void setup(void) {

  Serial.begin(115200);

  Serial.println("Start");
  Serial.println(sizeof(Parameter), DEC);
  /*
    vList<String> *ss = new vList<String>();
    if (ss->Length() == 4) {
    ssid = ss->GetItem(0);
    password = ss->GetItem(1);
    srv_url = ss->GetItem(2);
    dev_num = ss->GetItem(3);
    }
    delete ss;*/
  Serial.println("ssid = " + ssid);
  Serial.println("password = " + password);
  Serial.println("srv_url = " + srv_url);
  Serial.println("dev_num = " + dev_num);

  String MyName = "ViHome-" + dev_num;

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  int n = 0;
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED && n < CONTRYLIMIT) {
    delay(500);
    Serial.print(".");
    n++;
  }

  Connected = WiFi.status() == WL_CONNECTED;

  if (Connected) {

    Serial.println("Con time - " + String(n));
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    MyIp = WiFi.localIP();
  }
  else {
    Serial.print("Configuring access point...");
    WiFi.softAP(MyName, "12345678");
    MyIp = WiFi.softAPIP();

  }

  Serial.print("IP address: ");
  Serial.println(MyIp);
  Serial.print("My name: ");
  Serial.println(MyName);

  if (MDNS.begin(MyName)) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.onNotFound(handleRoot);

  server.begin();
  Serial.println("HTTP server started");
  digitalWrite(LED_BUILTIN, HIGH);


  //// HTTP


  String tmp = GetHtmlString(srv_url + "/api/DevInfo/1/");
  Serial.println(tmp);
  if (HtmlRezGood) WebConf = tmp;
  DeserializationError error = deserializeJson(Configuration, WebConf);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  const char* deviceName = Configuration["deviceName"];
  Serial.print("Device Name:");
  Serial.println(deviceName);
  parametersCount = Configuration["parameters"].size();
  //Serial.print("Parameters Count:");
  //Serial.println(parametersCount);
  parameters = new Parameter[parametersCount];
  for (int i = 0; i < parametersCount; i++) {
    parameters[i].did = Configuration["parameters"][i]["id"];
    parameters[i].pid = Configuration["parameters"][i]["parameter"]["id"];
    parameters[i].direct = Configuration["parameters"][i]["direction"];
    parameters[i].type = Configuration["parameters"][i]["parameter"]["type"];
    parameters[i].period = Configuration["parameters"][i]["period"];
    parameters[i].nextTime = 0;
    parameters[i].additionalInformation[0] = 0;
    const String tmpConf = Configuration["parameters"][i]["configuration"];

    Serial.print("Did:"); Serial.println(parameters[i].did, DEC);
    Serial.print("Pid:"); Serial.println(parameters[i].pid, DEC);
    Serial.print("Period:"); Serial.println(parameters[i].period, DEC);
    Serial.print("tmpConf:"); Serial.println(tmpConf);

    StaticJsonDocument<200> docTmp;
    error = deserializeJson(docTmp, tmpConf);
    if (!error) {
      if (parameters[i].type == 3) // Switch
      {
        parameters[i].additionalInformation[1] = Configuration["parameters"][i]["parameter"]["Count"];
        parameters[i].additionalInformation[2] = Configuration["parameters"][i]["parameter"]["RollMode"];
        for (int j = 0; j < PIN_COUNT; j++) {
          int ac = docTmp["Pin-" + String(j)];
          parameters[i].additionalCodes[j] = ac;
          // Serial.print("Pin-" + String(j) + ":"); Serial.println(ac, DEC);
        }
        if (parameters[i].direct == 1) // Out = 1, In = 2
        {
          parameters[i].additionalInformation[0] = docTmp["ButtonCount"];
          parameters[i].buttons = new OneButton * [parameters[i].additionalInformation[0]];
          for (int j = 0; j < parameters[i].additionalInformation[0]; j++)
          {
            int bm = docTmp["ButtonMode-" + String(j)];
            int ac = docTmp["Pin-" + String(j)];
            if (bm == 0)
            {
              parameters[i].buttons[j] = new OneButton(ac, true, true);
              byte b1 = i;
              byte b2 = j;
              parameters[i].buttons[j]->Tag = b1 + (b2 << 8);
              parameters[i].buttons[j]->attachClick(ButtonClick);
            }
            else
            {
              parameters[i].buttons[j] = nullptr;
              pinMode(ac, INPUT_PULLUP);
            }
          }
        }
      }
    }

    if (parameters[i].type == 3) // Switch
    {
      for (int j = 0; j < PIN_COUNT; j++) {
        int ac = parameters[i].additionalCodes[j];
        if (ac == 0) continue;
        if (parameters[i].direct == 2) // Out = 1, In = 2
          pinMode(ac, OUTPUT);

      }
    }

  }

  Serial.println("Initialization complete!");
}

unsigned long n_time;

void loop(void) {
  n_time = millis();

  server.handleClient();
  MDNS.update();

  for (int i = 0; i < parametersCount; i++) 
  {
    for (int j = 0; j < parameters[i].additionalInformation[0]; j++)
    {
      if (parameters[i].buttons[j] != nullptr)
        parameters[i].buttons[j]->tick();
      else
        SwitchOut(i, j);
    }

    if (parameters[i].direct != 1 && (parameters[i].nextTime - n_time) > parameters[i].period)
    {
      if (parameters[i].type == 3) // Switch
      {
        if (parameters[i].direct == 1) // Out = 1, In = 2
        {
          SwitchOut(i);
        }
        else
        {
          SwitchIn(i);
        }
      }

      parameters[i].nextTime = n_time + parameters[i].period;

    }
  }
}
