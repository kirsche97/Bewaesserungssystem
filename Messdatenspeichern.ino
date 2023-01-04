void Messdatenspeichern()
{

    if(Zeit_aktualisieren()%Speicherintervall== 0 && Zeit_aktualisieren()>0)
    {
        letzterMessdruchlauf=aktuelleZeit;
        feuchte[0] = Sensor1.getCapacitance();
        feuchte[1] = Sensor2.getCapacitance();
        feuchte[2] = Sensor3.getCapacitance();
        feuchte[3] = Sensor4.getCapacitance();
        feuchte[4] = Sensor5.getCapacitance();
        feuchte[5] = Sensor6.getCapacitance();
        feuchte[6] = Sensor7.getCapacitance();
        feuchte[7] = Sensor8.getCapacitance();
        feuchte[8] = Sensor9.getCapacitance();
        feuchte[9] = Sensor10.getCapacitance();
        feuchte[10] = Sensor11.getCapacitance();
        feuchte[11] = Sensor12.getCapacitance();
      
        SD.begin(SD_CS);  
  
        if(!SD.begin(SD_CS)) 
        {
        Serial.println("Card Mount Failed");
        return;
        }
        uint8_t cardType = SD.cardType();
        if(cardType == CARD_NONE) 
        {
        Serial.println("No SD card attached");
        return;
        }
        Serial.println("Initializing SD card...");
        if (!SD.begin(SD_CS)) 
        {
         Serial.println("ERROR - SD card initialization failed!");
         return;    // init failed
        }

        File file = SD.open(Messdaten);

        if(!file) {
          Serial.println("File doens't exist");
          Serial.println("Creating file...");
          writeFile(SD, Messdaten, "Sensornummer, Bodenfeuchte");
        }
        else {
          Serial.println("File already exists");  
        }
  
        file.close();

        delay(500);


        dataMessage =  String(dayStamp) + "," + String(timeStamp) + "\r\n";
        Serial.print("Save data: ");
        Serial.println(dataMessage);
        appendFile(SD, Messdaten, dataMessage.c_str());
        dataMessage="";

        for(int i = 0; i < tAnzahl; i++)          
      {
        Serial.print("\n");
        String x = String(feuchte[Pflanzen[i].Sensornummer-1]);
        dataMessage =  String(Pflanzen[i].T_Nummer)  + ","  + String(Pflanzen[i].Sensornummer) + ","  + x  + "\r\n";
        Serial.print("Save data: ");
        Serial.println(dataMessage);
        appendFile(SD, Messdaten, dataMessage.c_str());
      }
    }

      dataMessage="";
}
