#ifndef ee_utils_h
#define ee_utils_h
#include "Arduino.h"
#include <EEPROM.h>

template <typename T>
class ee_utils_b {
  public :

    ee_utils_b(void)
    {
      sz = sizeof(T);
      raw = new byte[sz];
      EEPROM.begin(512);
    }
    T EEPROM_read(int addr)
    {
      for (byte i = 0; i < sz; i++) raw[i] = EEPROM.read(addr++);
      T &num = (T&) * raw;
      curPos = addr;
      return num;
    }

    int EEPROM_write(int addr, T num)
    {
      if (std::is_same<T, String>::value)
      {
        String s = (String)num;
        int k = s.length();
        EEPROM.write(addr++, k);
        for (int i = 0; i < k; i++)
        {
          int t = s[i];
          EEPROM.write(addr++, t);
        }
        curPos = addr;
      }
      else
      {
        int sz = sizeof(T);
        (T&)*raw = num;
        for (byte i = 0; i < sz; i++) EEPROM.write(addr + i, raw[i]);
        curPos = addr + sz;
      }
      Commit();
      return curPos;
    }

    int GetTypeSize()
    {
      return sz;
    }

    int GetGurrentPosition()
    {
      return curPos;
    }

  protected :
    int sz;
    int curPos;
    byte *raw;

    void Commit()
    {
      if (EEPROM.commit()) {
        Serial.println("EEPROM successfully committed");
      } else {
        Serial.println("ERROR! EEPROM commit failed");
      }
    }
};

template <typename T>
class ee_utils: public ee_utils_b<T> {


};

template <>
class ee_utils<String>: public ee_utils_b<String> {
  public :
    String EEPROM_read(int addr)
    {      
      String s = "";
      int k = EEPROM.read(addr++);
      for (int i = 0; i < k; i++)
      {
        char c = EEPROM.read(addr++);
        s.concat(c);
      }
      curPos = addr;
      return s;
    }
};

#endif
