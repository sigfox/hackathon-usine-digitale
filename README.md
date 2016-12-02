#Hackathon Usine Digitale

Basic instructions to get started with your Atmel Arduino shield, and send your first messages over Sigfox within minutes.

This is an Arduino *shield*, which means you mean an [Arduino](http://arduino.cc) board to act as its master.


##Resources

Useful resources : 

* [Sigfox developer portal](http://makers.sigfox.com)
* [This Sigfox overview on slideshare](http://www.slideshare.net/nicolsc-slides/sigfox-makers-tour-bratislava)
* Atmel documentation :
	* http://www.atmel.com/products/wireless/sigfox/default.aspx
	* http://www.atmel.com/devices/ata8520.aspx

##Register

Register your kit on [the devkit activation page](http://backend.sigfox.com/activate) 

Choose the following settings  

* Provider : Atmel
* Operator : 🇫🇷 Sigfox

Then enter the informations related to your own device.
ID & PAC are printed on the back of the board, or you can extract them through a command


Once registered, you will receive an automated email asking you to setup your password.   
After that ... you're good to go ! Your account is ready to receive & manage the messages sent by your device


##Hello world

Simply copy the Hello World sample sketch in this repository. 

Provided with this sample, you have two Arduino librairies :

* AtmSIGFOX : abstract the interactions with the chipset to send Sigfox messages, and retrieve information from the hardware.
* AtmTemp : methods to use the included temperature sensor


###Frame composition

This example only uses 3 bytes, out of the maximum 12 bytes that can be sent over Sigfox.  
Power supply voltage in coded on the 2 last bytes, in mV.  
Temperature is sent as an integer on one byte, in °C.

####Example 

Frame sent : **000000000000000000190CA3**

* Power supply voltage
  * 00000000000000000019**0CA3**
  * 0CA3<sub>16</sub> == 3235<sub>10</sub> 
  * Voltage is 3235 mV
* Temperature
  * 000000000000000000**19**0CA3
  * 19<sub>16</sub> == 25<sub>10</sub>
  * Temperature is 25°C

##Go further

###Something more than the temperature ?

You can plug whatever analog/digital sensor you need to the board, and use its output to compose your emitted messages.


###Use your data

####Step one : check in the interface

Go to the _Devices_ section on [the sigfox portal](http://backend.sigfox.com) , click on your own device ID and then enter its _Messages_ menu.

![](./backend-screenshots/devices-menu.png)
![](./backend-screenshots/device-messages.png)


####Step two : set a callback

* Go to the _Device Type_ menu
* Click on your device type name
* Enter the _Callbacks_ menu
* Click _new_ (hidden in the top right corner !)
* Select _Custom callback_
* Choose your settings (you can choose to push to an HTTP endpoint, or to send a email for quick debug)


![Callbacks menu](./backend-screenshots/callbacks-menu.png)

![New callback](./backend-screenshots/new-callback.png)

![Callback settings](./backend-screenshots/callback-settings.png)