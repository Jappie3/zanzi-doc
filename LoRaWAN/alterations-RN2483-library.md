# Alterations to the RN2483 library by jpmeiers

The library that was used for LoRaWAN implementation has a few settings that we cannot access from the Arduino sketch. When we open the library code, we are able to edit certain variables. You can open the library once it is installed. It is generally located in C:\Users\<usrname>\Documents\Arduino\libraries. This is the same location as the default location for all your Arduino sketches.

Note that the possible alterations shown below are not absolutely necessary for the functionality of our board. But if you want to make alterations, Go to RN2483-Arduino-Library-master > src. There you will find rn2xx3.cpp and rn2xx3.h. You can open and edit these files using Notepad++ or Visual Studio Code. We will only edit the cpp file.

In the method initOTAA (found on line 110) the connection is initialised. On line 184 the function setAdaptiveDataRate() is called with argument false. When we change false to true, the LoRa module will be set to use adaptive data rate. However, we have set this variable to true, but it did not seem to work for us. The messages kept being sent at the same rate. 

![afbeelding](https://github.com/Jappie3/zanzi-doc/assets/91837988/08c46c91-23cc-4954-bc5c-b53b50c9b5a2)

 
Secondly, still in the initOTAA function, on line 178 a  raw command is sent to the LoRa module to set the datarate. We can change the 5 to a different integer (min 0, max 7) to change the datarate of the communication. This impacts the SpreadFactor (SF) and thus the speed and range of the data sent.

 ![afbeelding](https://github.com/Jappie3/zanzi-doc/assets/91837988/41a11e8b-2f39-4029-af27-192289dd7ef6)

 ![afbeelding](https://github.com/Jappie3/zanzi-doc/assets/91837988/5a17b507-d77f-4cad-ad75-19acd00ee1c2)

Source: RN2483 module command reference
Data Rates as defined in EU863-870:

 ![afbeelding](https://github.com/Jappie3/zanzi-doc/assets/91837988/01a61415-aa3d-429e-b53f-a82397222dc8)

Source: https://www.thethingsnetwork.org/docs/lorawan/regional-parameters/
