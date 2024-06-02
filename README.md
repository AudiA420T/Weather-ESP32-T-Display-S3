I originally made this progress to see the temperature in my office. Eventually I added Blynk support to monitor remotely, and separately attached a space heater that I could control remotely.

I had been running the setup for a few years, but I have recently been interested in the Lilygo products, specifically the LCD ESP32s. I wanted a more compact package and updated image. While updating to the Lilygo T-Display-S3 I added a weather API to get outdoor weather as well.

I'm using an Adafruit DHT 3 pin sensor to keep the package small.

Next steps are to add back the Blynk support and some nice icons for the weather on the right side of the screen

Original Setup
![IMG_9608](https://github.com/AudiA420T/Weather-ESP32-T-Display-S3/assets/6942261/77aa3a9d-0060-42d9-83e3-313e75c36790)
![IMG_9609](https://github.com/AudiA420T/Weather-ESP32-T-Display-S3/assets/6942261/ebcd3930-d8e3-48ca-94c0-16975f4e4473)

Updated Package
![IMG_9607](https://github.com/AudiA420T/Weather-ESP32-T-Display-S3/assets/6942261/128e399e-7b3f-4b31-978d-710f755a25a2)
![IMG_9610](https://github.com/AudiA420T/Weather-ESP32-T-Display-S3/assets/6942261/7dccee26-8287-4442-85d7-8dc222628b5e)

*** NOTES FOR THE LILYGO T-DISPLAY ***

1. Use this link to set up - https://github.com/Xinyuan-LilyGO/T-Display-S3.
2. Ensure you modify the User_Setup_Select.h file, especially if you do any library upgrades
3. I had a compilation issue come up all of the sudden after everything had been working. I had upgraded the ESP32 board version to 3.0.X. I had to roll back to a previous version (see image below)
4. Originally, my serial monitor was not showing any output. On this board I had to update the setting "USB CDC On Boot" to "Enabled"

![image](https://github.com/AudiA420T/Weather-ESP32-T-Display-S3/assets/6942261/27a6679d-466a-4d6b-9637-79b4d5b87490)
![image](https://github.com/AudiA420T/Weather-ESP32-T-Display-S3/assets/6942261/d4a7983e-784c-442e-b74a-7958968fc93d)

