#ifdef MODO_DEBUG
  #define DEBUG(x)  pSerialDebug->print (x)
  #define DEBUGLN(x)  pSerialDebug->println (x)
  #define DEBUGDEC(x)  pSerialDebug->print (x, DEC)
  #define DEBUGW(x)  pSerialDebug->write (x)
  #define DEBUGFULL(x)    \
    pSerialDebug->print(millis());     \
    pSerialDebug->print(": ");    \
    pSerialDebug->print(__PRETTY_FUNCTION__); \
    pSerialDebug->print(' ');      \
    pSerialDebug->print(__FILE__);     \
    pSerialDebug->print(':');      \
    pSerialDebug->print(__LINE__);     \
    pSerialDebug->print(' ');      \
    pSerialDebug->println(x);
#else
  #define DEBUG(x)
  #define DEBUGLN(x) 
  #define DEBUGDEC(x)
  #define DEBUGW(x) 
  #define DEBUGFULL(x)
#endif