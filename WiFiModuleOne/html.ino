#include "v_list.h"
#include "utils.h"

WiFiClient client;
HTTPClient http;

void handleRoot() {
  digitalWrite(LED_BUILTIN, LOW);

  bool changed = false;

  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "str" && ssid != server.arg(i))     {
      ssid = server.arg(i);
      changed = true;
    }
    if (server.argName(i) == "pwd" && password != server.arg(i)) {
      password = server.arg(i);
      changed = true;
    }
    if (server.argName(i) == "srv" && srv_url != server.arg(i))  {
      srv_url = server.arg(i);
      changed = true;
    }
    if (server.argName(i) == "dev" && dev_num != server.arg(i))  {
      dev_num = server.arg(i);
      changed = true;
    }
  }

  if (changed) {
    vList<String> *ss = new vList<String>();
    ss->Add(ssid);
    ss->Add(password);
    ss->Add(srv_url);
    ss->Add(dev_num);
    ss->EEPROM_write();
    delete ss;
  }

  String message = "";
  message += "<!DOCTYPE html>\n";
  message += "<html lang='en'>\n";
  message += "<head>\n";
  message += "<meta charset='UTF-8'>\n";
  message += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n";
  message += "<title>WI-FI Setings</title>\n";
  message += "</head>\n";
  message += "<body>\n";
  message += "<form action='.' method='post' style='text-align: center;'>\n";
  if (changed) message += "<p>REBOOT</p>\n";
  message += "<p><input type='text' name='str' value='" + ssid + "'></p>\n";
  message += "<p><input type='text' name='pwd' value='" + password + "'></p>\n";
  message += "<p><input type='text' name='srv' value='" + srv_url + "'></p>\n";
  message += "<p><input type='text' name='dev' value='" + dev_num + "'></p>\n";
  message += "<p><input type='submit' value='Отправить'></p>\n";
  message += "</form>\n";
  message += "</body>\n";
  message += "</html>\n";

  server.send(200, "text/html", message);
  digitalWrite(LED_BUILTIN, HIGH);

  if (changed) resetFunc();
}

String GetHtmlString(String adrr)
{
  Serial.print("[HTTP] URL = ");
  Serial.println(adrr);
  if (http.begin(client, adrr)) {  // HTTP

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        HtmlRezGood = true;
        return payload;
      }
    } else { HtmlRezGood = false;
      return "";
    }

    http.end();
  } else { HtmlRezGood = false;
    return "" ;
  }
}
