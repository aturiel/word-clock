                   .:                     :,                                          
,:::::::: ::`      :::                   :::                                          
,:::::::: ::`      :::                   :::                                          
.,,:::,,, ::`.:,   ... .. .:,     .:. ..`... ..`   ..   .:,    .. ::  .::,     .:,`   
   ,::    :::::::  ::, :::::::  `:::::::.,:: :::  ::: .::::::  ::::: ::::::  .::::::  
   ,::    :::::::: ::, :::::::: ::::::::.,:: :::  ::: :::,:::, ::::: ::::::, :::::::: 
   ,::    :::  ::: ::, :::  :::`::.  :::.,::  ::,`::`:::   ::: :::  `::,`   :::   ::: 
   ,::    ::.  ::: ::, ::`  :::.::    ::.,::  :::::: ::::::::: ::`   :::::: ::::::::: 
   ,::    ::.  ::: ::, ::`  :::.::    ::.,::  .::::: ::::::::: ::`    ::::::::::::::: 
   ,::    ::.  ::: ::, ::`  ::: ::: `:::.,::   ::::  :::`  ,,, ::`  .::  :::.::.  ,,, 
   ,::    ::.  ::: ::, ::`  ::: ::::::::.,::   ::::   :::::::` ::`   ::::::: :::::::. 
   ,::    ::.  ::: ::, ::`  :::  :::::::`,::    ::.    :::::`  ::`   ::::::   :::::.  
                                ::,  ,::                               ``             
                                ::::::::                                              
                                 ::::::                                               
                                  `,,`


http://www.thingiverse.com/thing:931777
3D Printable NeoMatrix 8x8 Word Clock by PawArmy is licensed under the Creative Commons - Attribution - Share Alike license.
http://creativecommons.org/licenses/by-sa/3.0/

# Summary

Here is my remix of the [NeoMatrix 8x8 Word Clock](https://learn.adafruit.com/neomatrix-8x8-word-clock) from the Adafruit website.  It has a 3d printable clock face, case, diffusers and buttons to set the time.  I left a slot in the bottom of the case to allow access to the programming port of the Arduino. The OpenSCAD source (in+all-it's+gory) is included if you want to make adjustments as is the Arduino sketch I modified for the time setting buttons.   


# Instructions

__Parts List:__   
[Adafruit NeoPixel NeoMatrix 8x8](https://www.adafruit.com/products/1487)  
[Circuit Overlay Board](https://www.parallax.com/product/32999)  
[SparkFun DeadOn RTC Breakout - DS3234](https://www.sparkfun.com/products/10160)  
[Arduino Pro Mini 328 5V/16Mhz](https://www.sparkfun.com/products/11113)  
[2x Tactile Button switches](http://www.adafruit.com/products/367)  
[Panel Mount 2.1mm DC barrel jack](http://www.adafruit.com/products/610)   
5 Vdc Power adapter   
4x 1" 4-40 machine screws  
2x 2-56 sheet metal screws  
3d Printed Case  
small gage hookup wire  

All parts print without support.  I printed the clock face and case in Black PLA and the diffusers in White PLA on a Printrbot Simple Metal  using 0.2mm layer height and 2 top, bottom and Perimeter Shells. For the clock face I used 50% perimeter overlap and gap fill to keep the light from the NeoPixels from bleeding through the face. For other colors you may want to increase the thickness of the faceplate.  

__Arduino Wiring:__  
NeoMatrix DIN	to D6  
Hour + Button (one side tied to ground) to D8  
Minute + Button (one side tied to ground) to D3  
RTC INT/SQW	Not Connected - Remove Pin  
RTC SS to D10  
RTC MOSI to D11  
RTC MISO to D12  
RTC SCLK to D13  
RTC VCC to A1   
RTC GND to A2  

__Required Arduino Libraries:__  
RTClib  
RTC_DS3234  
Adafruit_GFX  
Adafruit_NeoMatrix  
Adafruit_NeoPixel  
Bounce2