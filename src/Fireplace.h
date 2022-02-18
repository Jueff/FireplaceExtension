#ifndef __FIREPLACEEXTENSION__
#define __FIREPLACEEXTENSION__

#if !defined(AVR) && !defined(ESP32)
#error Platform is not supported
#endif

#include <MLLExtension.h>
extern const TProgmemRGBPalette16 HeatColors_p;

class FireplaceExtension : public MLLExtension
{
	private:
  ledNr_t ledNr;
  uint8_t heat;
  uint8_t brightness;
  uint8_t inCh;

	public:
  FireplaceExtension(ledNr_t ledNr, uint8_t inCh, uint8_t brightness) {
    this->ledNr = ledNr;
    this->brightness = brightness;
    this->inCh = inCh;
    this->heat = 0;
	}
	
	void setup(MobaLedLib_C& mobaLedLib) {
    memset(Get_LEDPtr(mobaLedLib,  ledNr), 0, 3);
	}

	void loop(MobaLedLib_C& mobaLedLib) 
  {
    if (mobaLedLib.Trigger20fps > 0)
    {
      CRGB* pRGB = Get_LEDPtr(mobaLedLib,  ledNr);
      bool isOn = Inp_Is_On(mobaLedLib.Get_Input(this->inCh));
      if (heat > 80 || !isOn) heat = qsub8(heat, random8(isOn ? 7 : 4)); // Cool down 
      if (isOn)
      { 
        if (random8() < 25) // New Spark
          heat = qadd8(heat, random8(25,50));
      }
      else
      {
        if (heat==0)
        {
          memset(pRGB, 0, 3);
          return;
        }
      }
      *pRGB = ColorFromPalette(HeatColors_p, scale8(heat, 170));

      nscale8x3(pRGB->r, pRGB->g, pRGB->b, brightness);// Reduce the brightnes
      if (pRGB->r <= pRGB->b) pRGB->r = qadd8(pRGB->b,1); // Fire must have r > b
    }
  }
};

#endif // __FIREPLACEEXTENSION__