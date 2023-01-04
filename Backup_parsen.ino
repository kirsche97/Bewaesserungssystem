void Backup_parsen()
{
  SD.begin(SD_CS);  

  myFileSDCart = SD.open(filename, FILE_READ);  
  DynamicJsonDocument doc(size_Jason);
  DeserializationError error = deserializeJson(doc, myFileSDCart);
  
  if(!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) 
  {
    Serial.println("ERROR - SD card initialization failed!");
    return;    // init failed
  }
  


    if (error)
    {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
     }
// open the file for reading:

   
  if (myFileSDCart) 
  {
  aktuellePosition=doc["Settings"][0];
  tAnzahl=doc["Settings"][1];
  sAnzahl=doc["Settings"][2];
  Giessintervall=doc["Settings"][3];
  Speicherintervall=doc["Settings"][4];
  letzerGiessdurchlauf=doc["Settings"][5];
  letzterMessdruchlauf=doc["Settings"][6];
  
for (int i = 0; i<tAnzahl; i++)
{
   Pflanzen[i].T_Nummer = doc["Topfnummer"][i];
   Pflanzen[i].Position = doc["Position"][i];
   Pflanzen[i].minFeuchte = doc["minFeuchte"][i];
   Pflanzen[i].WasserMenge = doc["WasserMenge"][i];
   Pflanzen[i].Duenger = doc["Duenger"][i];
   Pflanzen[i].Sensornummer = doc["Sensornummer"][i];
   Pflanzen[i].gegossen = doc["gegossen"][i];   
}

  }

   else 
   {
    Serial.print(F("SD Card: error on opening file"));
   }
}
