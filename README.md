# corona_traffic_light
Code for an Arduino run traffic light to control the flow of people. Useful for stairs and narrow hallways where it is hard to maintain social distance.




Master version uses one GP2Y0D810Z0F to detect if someone pushes the button for green light. Light in the other end stays green until someone pushes the button, in which case they will get green light until no one pushes the button anymore. 
(Everything is wired and for some reason the sensor will not work if it is connected to far away from the arduino and power source. This is why this setup only uses one sensor.)


HC_SRO4 version uses two sensors and allows calibration of how close you need to be for the button to trigger. This version has both end being default red and they turn green if someone pushes the button. It remembers the order the buttons are pressed, so that one side will not potentially have to stand and wait for a long time.
(Not tested if sensor works when placed further away from the Arduino)
