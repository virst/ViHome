#ifndef v_list_h
#define v_list_h
#include "Arduino.h"
#include "ee_utils.h"

template <typename T>
class vList
{
  public:

    vList(void)
    {
      Clear();
    }

    ~vList(void)
    {
     Serial.println("rm");
       DeleteSs();
    }

    void Add(T s)
    {
      if (len < real_len)
      {
        ss[len] = s;
        len++;
      }
      else
      {
        chSizeMas();
        Add(s);
      }
    }

    void DeleteAt(int n)
    {
      if (n >= len)
        return;
      len--;
      for (int i = n; i < len; i++)
        ss[i] = ss[i + 1];
    }

    T GetItem(int n)
    {
      if (n >= len)
        return  T();
      return ss[n];
    }

    void SetItem(int n, T s)
    {
      if (n >= len)
        return;
      ss[n] = s;
    }

    void Clear()
    {
      DeleteSs();
      len = 0;
      real_len = 5;
      growth_mult = 1;
      growth_plus = 5;

      ss = new T[real_len];
    }

    int Length()
    {
      return len;
    }


    void EEPROM_read()
    {
      int l = eui.EEPROM_read(0);       
      int a = eui.GetTypeSize();        
      Clear();
      for (int i = 0; i < l; i++)
      {
        Add(eut.EEPROM_read(a));
        a = eut.GetGurrentPosition();
      }

    }

    void EEPROM_write()
    {
      int a = eui.EEPROM_write(0, Length());
      for (int i = 0; i < Length(); i++)
        a = eut.EEPROM_write(a, GetItem(i));
    }

  private:
    void chSizeMas()
    {
      tmp_len = real_len * growth_mult + growth_plus;
      T *tmp = new T[tmp_len];
      for (int i = 0; i < real_len; i++)
        tmp[i] = ss[i];
      DeleteSs();
      real_len = tmp_len;
      ss = tmp;
    }

    void DeleteSs()
    {
      if (ss != nullptr) {
        delete [] ss;
        ss = nullptr;
      }
    }

    T *ss = nullptr;
    int len;
    int real_len;
    int tmp_len;
    int growth_mult;
    int growth_plus;

    ee_utils<T> eut;
    ee_utils<int> eui;
};

#endif
