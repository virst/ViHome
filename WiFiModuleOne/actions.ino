#include <map>

std::map<int, String> Values;

String GetValue(int p)
{
  String tmp = GetHtmlString(srv_url + "/api/ParamValue/get/" + String(p) + "/");
  if (HtmlRezGood)
    Values[p] = tmp;
  else if (Values.find(p) == Values.end()) Values[p] = "";
  return Values[p];
}

bool SetValue(int p, String v)
{
  return GetHtmlString(srv_url + "/api/ParamValue/set/" + String(p) + "/?val=" + v) == "OK";
}

void SwitchOut(int n, int pin)
{
  int ac = parameters[n].additionalCodes[j];
  if (Values.find(p) == Values.end()) Values[p] = "";
  int v = Values[p].toInt();
  int buttonState = digitalRead(ac);
  if ((buttonState == LOW) == (v | 1 << pin) == v) return;
  v ^= (1 << btnNum);
  Values[p] = String(v);
  SetValue(p, Values[p]);

}

void SwitchIn(int n)
{
  int pv = GetValue(parameters[n].pid).toInt();
  int c = 1;
  for (int j = 0; j < parametersCount; j++)
  {
    int ac = parameters[n].additionalCodes[j];
    if (ac == 0) continue;
    if ((pv & c) == c)
    {
      digitalWrite(ac, HIGH);
    }
    else
    {
      digitalWrite(ac, LOW);
    }
    c *= 2;
  }
}

void ButtonClick(int t)
{
  byte paramNum = (byte)(t & 0b11111111);
  byte btnNum = (byte)(t >> 8);

  int p = parameters[paramNum].pid;
  if (Values.find(p) == Values.end()) Values[p] = "";
  int v = Values[p].toInt();
  if (parameters[paramNum].additionalInformation[0] == 1)
    v = (v + 1) % parameters[paramNum].additionalInformation[1];
  else
    v ^= (1 << btnNum);
  Values[p] = String(v);
  SetValue(p, Values[p]);
}
