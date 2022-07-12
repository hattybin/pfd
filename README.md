# pfd
<h1><b>
NO LONGER MAINTAINING THIS PROJECT</B>
THIS PROJECT WAS PORTED TO A THINGSPEAK BASED SOLUTON HERE: https://github.com/hattybin/pfd-ts
REPOSITORY LEFT FOR REFERENCE</h1>

-------------------------------------------

This project is a modification to a WoPet Dog Food Feeder to use a NodeMCU controller instead of the electronics it was sold with. I wanted a solution that didn't require cloud infrastructure and I one I could create APIs for to integrate into my other systems. The current prototype can trigger feeding via the webpage controls or a POST web request to /FEED or /STOP. 

I spent most my time working out the electronics part and am using it currently since it is functional. I intend on adding more features and cleaning up the code as I have time.

ToDo:
- Add real time clock module or build my own circut using a crystal or something. I haven't dont RTC stuff with arduino before so I put this low on the prio list. This would be used for scheduling feedings. I currently have a server that send the API requests on a schedule for my pet's feedings.
- Add visble feedback of some kind. LEDs, LCD/OLED with data, anything really...
- Audio? The original product had a mic and speaker with a DAC so users can record a message to play when feeding is triggered. I have the hardware but I don't know how to use I2S yet.
- Button to trigger a manual feeding. This will be done very soon.

Current features:
- Wifi only. No bluetooth
- Async web interface to trigger, stop and set the portions per feeding.
- Persistant storage of settings in text files using SPIFFS


This is the top of the first functional prototype board:
![PUay-lM8mfcO63SL87gmJMhf1xzJghgrLV16HYph44GAU18jf4hCar6-VzWDWHLd8hsxczqd1WJtls0psK2s2CxOvPzx8WOiRpNW0oQ3VmCNRurRwSpoN7pcAeSciJLuAKEB6Lu5DeS4pqgClI5ZXvJjnHbB3Df8cKaXqTCFqI4uA9SxeAk7W8kREn3E1UXZTkB0ojI0JmbdvgxzdKJs](https://user-images.githubusercontent.com/1077409/171032943-e9226edb-5be9-43f2-90cb-4f0de112f5b2.jpg)

Parts:
- NodeMCU - https://www.amazon.com/dp/B081CSJV2V?psc=1&ref=ppx_yo2ov_dt_b_product_details
- DC motor driver https://www.amazon.com/dp/B00M0F243E?psc=1&ref=ppx_yo2ov_dt_b_product_details
- IR Optical Optocoupler https://www.amazon.com/dp/B08977QFK5?psc=1&ref=ppx_yo2ov_dt_b_product_details
- Momentary button (optional)

Bottom:
![Z9ywuy3kHrdPeKhzoxL1rC8YTAM6MWXJfoYFlLRzm_RkM31-iyGWqDp9Qqqkd7aQCmAjchSVuCzV-8N3EhPjwimvZkShKy7hqca9ys6KWQ3GoEdhuZidUgJKYRArMPosk3PQU1uLtswtjtz4fvcWIqjN70gocyogso3ftr_a0XieXmUM9MxuFp7yI7TG1EtZH9gVFZ8rB_iH3Wv1uMWO-1](https://user-images.githubusercontent.com/1077409/171032948-da46ad8c-0c31-4596-a360-1879a1496494.jpg)


Random Pics:
![PXL_20220523_132414838 MP](https://user-images.githubusercontent.com/1077409/171032620-b92123c2-a12f-4015-9e71-e9980705a676.jpg)
![PXL_20220523_132639966 MP](https://user-images.githubusercontent.com/1077409/171032635-8f7b5389-fed1-4e86-a084-4d524749a1a0.jpg)
![PXL_20220523_144609972 MP](https://user-images.githubusercontent.com/1077409/171032647-5fcebbba-901e-4026-ae42-9af6e2ccc906.jpg)
![PXL_20220524_130647633 MP](https://user-images.githubusercontent.com/1077409/171032649-b4ad3c5d-2d7f-433e-85a0-667fb107ad0c.jpg)


