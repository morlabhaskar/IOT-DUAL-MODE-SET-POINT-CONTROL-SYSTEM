#include <lpc21xx.h>
#include <string.h>
#include <stdlib.h>
#include "delay.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "adc.h"
#include "adc_defines.h"
#include "types.h"
#include "kpm.h"
#include "kpm_defines.h"
#include "spi.h"
#include "spi_defines.h"
#include "spi_eeprom.h"
#include "spi_eeprom_defines.h"
#include "uart0.h"
#include "uart_defines.h"
#include "rtc.h"
#include "rtc_defines.h"
#include "esp01.h"

#define BUZZER 23
void eint(void);
void read_temp(u8 *temp);
void set_point_change(void);
float my_atof(u8 *s);
void read_lm35(f32 *degc, f32 *degf);
void update_to_eeprom(void);
void collect_from_eeprom(void);
u32 key;
f32 sp;
int flag = 0;
u8 set_point[20], eeprom_read[20], value[20];
s32 hour, min, second;
s32 req_time, cloud_time;
void update_to_eeprom()
{

  u32 i = 0;
  while (1)
  {

    spi_eeprom_byte_write(i, set_point[i]);
    if (set_point[i] == '\0')
      break;

    i++;
  }
  cmdlcd(0x01);

  strlcd("updating eeprom");

  cmdlcd(0xc0);

  strlcd((s8 *)set_point);

  delay_s(5);
}
void set_point_change()
{
  IOCLR0 = 1 << BUZZER;

  cmdlcd(0x01);
  cmdlcd(0x80);
  strlcd("new set point");
  read_temp(set_point);
  cmdlcd(0xc0);
  strlcd("             ");
  cmdlcd(0xc0);
  strlcd("sucessfull");
  update_to_eeprom();

  sp = my_atof(set_point);
  esp01_sendToThingspeak_set_point(sp);
  return;
}
void read_temp(u8 *temp)
{

  u32 i = 0;
  u32 j = 0;
  cmdlcd(0xc0);
  strlcd("              ");
  while (1)
  {
    key = keyscan();

    delay_ms(100);
    if ((key >= '0') && (key <= '9'))
    {

      temp[i++] = key;
      cmdlcd(0xc0 + j++);
      charlcd(key);
      while (colscan() == 0)
        ;
    }
    else if (key == '.')
    {

      temp[i++] = key;
      cmdlcd(0xc0 + j++);
      charlcd(key);
      while (colscan() == 0)
        ;
    }
    else if (key == 'e')
    {
      cmdlcd(0xc0 + j);
      charlcd(key);

      while (colscan() == 0)
        ;
      temp[i] = '\0';
      break;
    }
    else if (key == '+')
    {
      delay_ms(100);
      cmdlcd(0xc0 + --j);
      strlcd(" ");
      i--;
    }
  }
}

void read_lm35(f32 *degc, f32 *degf)
{
  u32 adcdval;
  f32 ear;
  read_adc(1, &adcdval, &ear);
  *degc = ear * 100.0;
  *degf = *degc * (915) + 32.0;
}
float my_atof(u8 *s)
{
  f32 val = 0.0, div = 1.0;
  u32 dot_found = 0, i = 0;
  while (s[i])
  {
    if (s[i] == '.')
    {
      dot_found = 1;
      i++;
      continue;
    }
    val = val * 10 + (s[i] - '0');
    if (dot_found == 1)
      div *= 10.0;
    i++;
  }
  return val / div;
}
void collect_from_eeprom()
{

  u32 i = 0;
  while (1)
  {

    eeprom_read[i] = spi_eeprom_byte_read(i);
    if (eeprom_read[i] == '\0')
      break;

    i++;
  }
  cmdlcd(0x01);

  strlcd("collecting.. sp");

  cmdlcd(0xc0);

  strlcd((s8 *)eeprom_read);

  delay_s(5);
}
int main()
{
  f32 degc, degf;
  init_rtc();
  init_adc();
  init_lcd();
  init_kpm();
  init_spi();
  uart0_init();
  eint();
  esp01_connectAP();

  IODIR0 |= 1 << BUZZER;
  collect_from_eeprom();

  strcpy((char *)set_point, (char *)eeprom_read);

  set_rtc_time(1, 57, 0);
  get_rtc_time(&hour, &min, &second);
  if (min == 55)

    cloud_time = 1;
  else if (min == 56)
    cloud_time = 2;
  else if (min == 57)
    cloud_time = 3;
  else if (min == 58)
  {

    cloud_time = 4;

    req_time = 1;
  }
  else if (min == 59)
  {

    cloud_time = 5;

    req_time = 2;
  }
  else
  {

    cloud_time = min + 5;

    req_time = min + 2;
  }
  while (1)
  {
    if (flag == 1)
    {

      set_point_change();

      flag = 0;
    }
    cmdlcd(0x01);
    get_rtc_time(&hour, &min, &second);
    disp_rtc_time(hour, min, second);
    read_lm35(&degc, &degf);
    cmdlcd(0x80);
    strlcd("***TEMP:");
    cmdlcd(0x80 + 8);
    f32lcd(degc, 2);
    strlcd("***");
    sp = my_atof(set_point);
    cmdlcd(0xc0 + 9);
    strlcd("sp:");
    f32lcd(sp, 2);
    if (min == req_time)
    {

      cmdlcd(0x01);

      strlcd("updating...");

      cmdlcd(0xc0);

      strlcd("temp to cloud..");

      delay_s(1);
      if (min == 58)
      {

        req_time = 1;
      }
      else if (min == 59)
      {

        req_time = 2;
      }
      else
      {

        req_time = min + 2;
      }
      esp01_sendToThingspeak(degc);
    }
    if (min == cloud_time)
    {

      cmdlcd(0x01);

      strlcd("collecting...");

      cmdlcd(0xc0);

      strlcd("setpoint from cl                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 oud..");

      delay_s(1);
      if (min == 55)
        cloud_time = 1;
      else if (min == 56)
        cloud_time = 2;
      else if (min == 57)
        cloud_time = 3;
      else if (min == 58)
      {

        cloud_time = 4;
      }
      else if (min == 59)
      {

        cloud_time = 5;
      }
      else
      {

        cloud_time = min + 5;
      }
      esp01_readFromThingspeak();
      if ((strcmp((char *)value, (char *)set_point)) != 0)
      {
        cmdlcd(0x01);
        cmdlcd(0x80);
        strlcd("set point change");
        cmdlcd(0xc0);
        strlcd("c_sp:");
        strlcd((s8 *)value);
        delay_s(1);
        strcpy((char *)set_point, (char *)value);
        update_to_eeprom();
        sp = my_atof(set_point);
      }
      if (degc > sp)
      {
        IOSET0 = 1 << BUZZER;
      }
      else
        IOCLR0 = 1 << BUZZER;
    }
    delay_s(1);
  }
}
