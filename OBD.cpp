/*************************************************************************
* Arduino Library for OBD-II UART/I2C Adapter
* Distributed under GPL v2.0
* Visit http://freematics.com for more information
* (C)2012-2015 Stanley Huang <stanleyhuangyc@gmail.com>
*************************************************************************/

#include <Arduino.h>
#include <Wire.h>
#include "OBD.h"
//#define DEBUG Serial
void displayDebug(char *msg);
float   BAROMETRIC;
uint16_t hex2uint16(const char *p)
{
  char c = *p;
  uint16_t i = 0;
  for (char n = 0; c && n < 4; c = *(++p)) {
    if (c >= 'A' && c <= 'F') {
      c -= 7;
    } else if (c>='a' && c<='f') {
      c -= 39;
        } else if (c == ' ') {
            continue;
        } else if (c < '0' || c > '9') {
      break;
        }
    i = (i << 4) | (c & 0xF);
    n++;
  }
  return i;
}
byte hex2uint8(const char *p)
{
  byte c1 = *p;
  byte c2 = *(p + 1);
  if (c1 >= 'A' && c1 <= 'F')
    c1 -= 7;
  else if (c1 >='a' && c1 <= 'f')
      c1 -= 39;
  else if (c1 < '0' || c1 > '9')
    return 0;
  if (c2 >= 'A' && c2 <= 'F')
    c2 -= 7;
  else if (c2 >= 'a' && c2 <= 'f')
      c2 -= 39;
  else if (c2 < '0' || c2 > '9')
    return 0;

  return c1 << 4 | (c2 & 0xf);
}

/*************************************************************************
* OBD-II UART Adapter
*************************************************************************/

byte COBD::sendCommand(const char* cmd, char* buf)
{
  write(cmd);
  dataIdleLoop();
  return receive(buf);
}
void COBD::sendQuery(byte pid)
{
  char cmd[8];
  sprintf(cmd, "%02X%02X\r", dataMode, pid);
#ifdef DEBUG
  debugOutput(cmd);
#endif
  write(cmd);
}

bool COBD::read(byte pid, float& result)
{
  // send a query command
  sendQuery(pid);
  // receive and parse the response
  return getResult(pid, result);
}

void COBD::read_raw(byte pid, char* result)
{
  char buffer[OBD_RECV_BUF_SIZE];
  sendQuery(pid);
  char* data = getResponse(pid, buffer);
  if (!data) {
    recover();
    errors++;
    return;
  }
  memcpy(result, buffer, sizeof(char)*OBD_RECV_BUF_SIZE);
}

void COBD::clearDTC()
{
  write("04\r");
  receive(0, 1000);
}

bool COBD::available()
{
  return OBDUART.available();
}

char COBD::read()
{ 
  char c = OBDUART.read();
#ifdef DEBUG
    DEBUG.write(c);
#endif
  return c;
}

void COBD::write(const char* s)
{
  OBDUART.write(s);
}

void COBD::write(char c)
{
  OBDUART.write(c);
}

float COBD::normalizeData(byte pid, char* data)
{
 float result ,temResult;
 //float atm;
 char Psistr[5];
  switch (pid) {
  case PID_RPM:
  case PID_EVAP_SYS_VAPOR_PRESSURE:
    result = getLargeValue(data) >> 2;
    break;
  case PID_FUEL_PRESSURE:
    result = getSmallValue(data) * 3;
    break;
  case PID_COOLANT_TEMP:
  case PID_INTAKE_TEMP:
  case PID_AMBIENT_TEMP:
  case PID_ENGINE_OIL_TEMP:
    result = getTemperatureValue(data);
    break;
  case PID_THROTTLE:
  case PID_COMMANDED_EGR:
  case PID_COMMANDED_EVAPORATIVE_PURGE:
  case PID_FUEL_LEVEL:
  case PID_RELATIVE_THROTTLE_POS:
  case PID_ABSOLUTE_THROTTLE_POS_B:
  case PID_ABSOLUTE_THROTTLE_POS_C:
  case PID_ACC_PEDAL_POS_D:
  case PID_ACC_PEDAL_POS_E:
  case PID_ACC_PEDAL_POS_F:
  case PID_COMMANDED_THROTTLE_ACTUATOR:
  case PID_ENGINE_LOAD:
  case PID_ABSOLUTE_ENGINE_LOAD:
  case PID_ETHANOL_FUEL:
  case PID_HYBRID_BATTERY_PERCENTAGE:
    result = getPercentageValue(data);
    break;
  case PID_MAF_FLOW:
    result = getLargeValue(data) / 100;
    break;
  case PID_TIMING_ADVANCE:
    result = (int)(getSmallValue(data) / 2) - 64;
    break;
  case PID_DISTANCE: // km
  case PID_DISTANCE_WITH_MIL: // km
  case PID_TIME_WITH_MIL: // minute
  case PID_TIME_SINCE_CODES_CLEARED: // minute
  case PID_RUNTIME: // second
  case PID_FUEL_RAIL_PRESSURE: // kPa
  case PID_ENGINE_REF_TORQUE: // Nm
    result = getLargeValue(data);
    break;
  case PID_CONTROL_MODULE_VOLTAGE: // V
    result = getLargeValue(data) / 1000;
    break;
  case PID_ENGINE_FUEL_RATE: // L/h
    result = getLargeValue(data) / 20;
    break;
  case PID_ENGINE_TORQUE_DEMANDED: // %
  case PID_ENGINE_TORQUE_PERCENTAGE: // %
    result = (int)getSmallValue(data) - 125;
    break;
  case PID_SHORT_TERM_FUEL_TRIM_1:
  case PID_LONG_TERM_FUEL_TRIM_1:
  case PID_SHORT_TERM_FUEL_TRIM_2:
  case PID_LONG_TERM_FUEL_TRIM_2:
  case PID_EGR_ERROR:
    result = ((int)getSmallValue(data) - 128) * 100 / 128;
    break;
  case PID_FUEL_INJECTION_TIMING:
    result = ((int32_t)getLargeValue(data) - 26880) / 128;
    break;
  case PID_CATALYST_TEMP_B1S1:
  case PID_CATALYST_TEMP_B2S1:
  case PID_CATALYST_TEMP_B1S2:
  case PID_CATALYST_TEMP_B2S2:
    result = getLargeValue(data) / 10 - 40;
    break;
  case PID_INTAKE_MAP:
    temResult = getSmallValue(data);
    result = temResult * 0.14503 - 14.503;
    break;
  case PID_SPEED:
    result = getSmallValue(data);
    result = float( result ) / 1.609;
    break;
  case PID_CHARGE_AIR_TEMP:
   result = getLargeValue(data) - ( getSmallValue(data)*256 )- 40;
   break;
  case PID_BOOST_CONTROL:
   Serial.println("boost");
   Psistr[0] = data[9];
   Psistr[1] = data[10];
   Psistr[2] = ' '; 
   Psistr[3] = data[17];
   Psistr[4] = data[18];
   Psistr[5] ='\0';
   /*
   temResult = getLargeValue( Psistr )/32.00;
   temResult = ( temResult  / 6.895 - 14.5038 );
   */
   temResult = getLargeValue( Psistr )/ 32.00 - BAROMETRIC ;
   temResult = ( temResult  / 6.895);
   result= temResult;
   if(temResult < 0 ){
    result  = temResult * 2.0360206576012;
   }   
   break;
   
  default:
    result = getSmallValue(data);
  }
  return result;
}

char* COBD::getResponse(byte& pid, char* buffer)
{
  while ( receive( buffer, OBD_TIMEOUT_SHORT ) > 0) {
    char *p = buffer;
    while ((p = strstr(p, "41 "))) {
        p += 3;
        byte curpid = hex2uint8(p);
        if (pid == 0) pid = curpid;
        if (curpid == pid) {
            errors = 0;
            p += 2;
            if (*p == ' ')
                return p + 1;
        }
    }
  }
  return 0;
}

bool COBD::getResult(byte& pid, float& result)
{
  char buffer[OBD_RECV_BUF_SIZE];
  char* data = getResponse(pid, buffer);
  if (!data) {
    recover();
    errors++;
    return false;
  }
  result = normalizeData(pid, data);
  return true;
}

bool COBD::setProtocol(OBD_PROTOCOLS h)
{
    char buf[OBD_RECV_BUF_SIZE];
  if (h == PROTO_AUTO) {
    write("ATSP00\r");
  } else {
    sprintf(buf, "ATSP%d\r", h);
    write(buf);
  }
  if (receive(buf, 3000) > 0 && strstr(buf, "OK"))
        return true;
    else
        return false;
}

void COBD::sleep()
{
  write("ATLP\r");
  receive();
}

float COBD::getVoltage()
{
    char buf[OBD_RECV_BUF_SIZE];
    write("ATRV\r");
    byte n = receive(buf, 100);
    if (n > 0) {
        return atof(buf);
    }
    return 0;
}

bool COBD::getVIN(char* buffer)
{
    if (sendCommand("0902\r", buffer)) {
        char *p = strstr(buffer, "0: 49 02");
        if (p) {
            char *q = buffer;
            p += 10;
            do {
                for (++p; *p == ' '; p += 3) {
                    if (*q = hex2uint8(p + 1)) q++;
                }
                p = strchr(p, ':');
            } while(p);
            *q = 0;
            return true;
        }
    }
    return false;
}

bool COBD::isValidPID(byte pid)
{
  if (pid >= 0x7f)
    return true;
  pid--;
  byte i = pid >> 3;
  byte b = 0x80 >> (pid & 0x7);
  return pidmap[i] & b;
}

void COBD::begin()
{
  OBDUART.begin(OBD_SERIAL_BAUDRATE);
#ifdef DEBUG
  DEBUG.begin(115200);
#endif
  recover();
}

byte COBD::receive(char* buffer, int timeout)
{
  unsigned char n = 0;
  unsigned long startTime = millis();
  for (;;) {
    if (available()) {
      char c = read();
      if (n > 2 && c == '>') {
        // prompt char received
        break;
      } else if (!buffer) {
             n++;
      } else if (n < OBD_RECV_BUF_SIZE - 1) {
        if (c == '.' && n > 2 && buffer[n - 1] == '.' && buffer[n - 2] == '.') {
          // waiting siginal
          n = 0;
          timeout = OBD_TIMEOUT_LONG;
        } else {
          buffer[n++] = c;
        }
      }
    } else {
      if (millis() - startTime > timeout) {
          // timeout
          break;
      }
      dataIdleLoop();
    }
  }
  if (buffer) buffer[n] = 0;
  return n;
}

void COBD::recover()
{
  write("AT\r");
  receive(0, 1000);
}

bool COBD::init(OBD_PROTOCOLS protocol)
{
  const char *initcmd[] = {"ATZ\r","ATE0\r","ATL1\r","ATL1\r"};
  char buffer[OBD_RECV_BUF_SIZE];
  m_state = OBD_CONNECTING;
  recover();
  for (unsigned char i = 0; i < sizeof(initcmd) / sizeof(initcmd[0]); i++) {
#ifdef DEBUG
    debugOutput(initcmd[i]);
#endif
    write(initcmd[i]);
    if (receive(buffer) == 0) {
      m_state = OBD_DISCONNECTED;
      return false;
    }
    delay(50);
  }
  while (available()) read();

  if (protocol != PROTO_AUTO) {
    setProtocol(protocol);
  }
    float value;
  if (!read(PID_RPM, value)) {
    m_state = OBD_DISCONNECTED;
    return false;
  }

  // load pid map
  memset(pidmap, 0, sizeof(pidmap));
  for (byte i = 0; i < 4; i++) {
    byte pid = i * 0x20;
    sendQuery(pid);
    char* data = getResponse(pid, buffer);
    if (!data) break;
    data--;
    for (byte n = 0; n < 4; n++) {
      if (data[n * 3] != ' ')
        break;
      pidmap[i * 4 + n] = hex2uint8(data + n * 3 + 1);
    }
    delay(100);
  }
  while (available()) read();

  m_state = OBD_CONNECTED;
  read(PID_BAROMETRIC,BAROMETRIC);
  errors = 0;
  return true;
}

void COBD::end()
{
  m_state = OBD_DISCONNECTED;
  OBDUART.end();
}

bool COBD::setBaudRate(unsigned long baudrate)
{
    OBDUART.print("ATBR1 ");
    OBDUART.print(baudrate);
    OBDUART.print('\r');
    delay(50);
    OBDUART.end();
    OBDUART.begin(baudrate);
    recover();
    return true;
}
#ifdef DEBUG
void COBD::debugOutput(const char *s)
{
  DEBUG.print('[');
  DEBUG.print(millis());
  DEBUG.print(']');
  DEBUG.print(s);
}
#endif
