I originally made this progress to see the temperature in my office. Eventually I added Blynk support to monitor remotely, and separately attached a space heater that I could control remotely (not included).

I had been running the setup using the Adafruit DHT 3 pin sensor for a few years, and I have recently been interested in the Lilygo products, specifically the LCD ESP32s. I wanted a more compact package and updated image. While updating to the Lilygo T-Display-S3 I added a weather API to get outdoor weather as well.

You can think about the code as (4) distinct pieces:
1. Blynk code to make this an external Blynk sensor that I can use with their app to monitor
2. Weather API from (https://www.tomorrow.io/) to pull the latest weather data
3. DHT Sensor inputs
4. <a href="https://www.flaticon.com/free-icons/rainbow" title="rainbow icons">Rainbow icons created by geotatah - Flaticon</a>

Original Setup
![IMG_9608](https://github.com/AudiA420T/Weather-ESP32-T-Display-S3/assets/6942261/77aa3a9d-0060-42d9-83e3-313e75c36790)
![IMG_9609](https://github.com/AudiA420T/Weather-ESP32-T-Display-S3/assets/6942261/ebcd3930-d8e3-48ca-94c0-16975f4e4473)

DHT11 Sensor Types
![image](https://github.com/AudiA420T/Weather-ESP32-T-Display-S3/assets/6942261/0494f9b8-7cb7-46ae-b122-f162dd882404)

I used the 3 pin PCB for mine, but I couldn't find the Fritzing diagram for it. So here is a schematic for the generic 4 pin unit

<img width="563" alt="Fritzing" src="https://github.com/AudiA420T/Weather-ESP32-T-Display-S3/assets/6942261/3b9dfa01-0cb7-4bfc-a991-3fb0097dc695">

Updated Package
![IMG_9607](https://github.com/AudiA420T/Weather-ESP32-T-Display-S3/assets/6942261/128e399e-7b3f-4b31-978d-710f755a25a2)
![IMG_9626](https://github.com/AudiA420T/Weather-ESP32-T-Display-S3/assets/6942261/b6902386-0eaf-4378-938f-961712fdccf7)


*** NOTES ON HOW TO SET UP THE LILYGO T-DISPLAY ***

1. Use this link to set up - https://github.com/Xinyuan-LilyGO/T-Display-S3.
2. Ensure you modify the User_Setup_Select.h file, especially if you do any library upgrades
3. I had a compilation issue come up all of the sudden after everything had been working. I had upgraded the ESP32 board version to 3.0.X. I had to roll back to a previous version (see image below)

![image](https://github.com/AudiA420T/Weather-ESP32-T-Display-S3/assets/6942261/27a6679d-466a-4d6b-9637-79b4d5b87490)

4. Originally, my serial monitor was not showing any output. On this board I had to update the setting "USB CDC On Boot" to "Enabled"

![image](https://github.com/AudiA420T/Weather-ESP32-T-Display-S3/assets/6942261/d4a7983e-784c-442e-b74a-7958968fc93d)


