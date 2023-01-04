void Backup_erstellen()
{

  if(WiFi.status() == WL_CONNECTED) getTimeStamp();
  
   // Initialize SD card
  SD.begin(SD_CS);  
  
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
  if (!SD.begin(SD_CS)) {
    Serial.println("ERROR - SD card initialization failed!");
    return;    // init failed
  }


    SD.remove(filename);
    
    DynamicJsonDocument doc(size_Jason);

    doc["Erstellt_am"] = dayStamp;
    doc["Erstellt_um"] = timeStamp;
    
    // create an object to add to the array
    JsonArray Topfnummer = doc.createNestedArray("Topfnummer");
    JsonArray Position = doc.createNestedArray("Position");
    JsonArray minFeuchte = doc.createNestedArray("minFeuchte");
    JsonArray WasserMenge = doc.createNestedArray("WasserMenge");
    JsonArray Duenger = doc.createNestedArray("Duenger");
    JsonArray Sensornummer = doc.createNestedArray("Sensornummer");
    JsonArray gegossen = doc.createNestedArray("gegossen");    
    JsonArray Settings = doc.createNestedArray("Settings");


    for(int i = 0; i<tAnzahl; i++)
    {
      Topfnummer.add(Pflanzen[i].T_Nummer);
      Position.add(Pflanzen[i].Position);
      minFeuchte.add(Pflanzen[i].minFeuchte);
      WasserMenge.add(Pflanzen[i].WasserMenge);
      Duenger.add(Pflanzen[i].Duenger);
      Sensornummer.add(Pflanzen[i].Sensornummer);
      gegossen.add(Pflanzen[i].gegossen);
      
    }

    Settings.add(aktuellePosition);
    Settings.add(tAnzahl);
    Settings.add(sAnzahl);
    Settings.add(Giessintervall);
    Settings.add(Speicherintervall);
    Settings.add(letzerGiessdurchlauf);
    Settings.add(letzterMessdruchlauf);        
 
 
    boolean isSaved = saveJSonToAFile(&doc, filename);
 
    if (isSaved){
        Serial.println("File saved!");
    }else{
        Serial.println("Error on save File!");
    }
 
    // Print test file
    Serial.println(F("Print test file..."));
    printFile(filename);
}


bool saveJSonToAFile(DynamicJsonDocument *doc, String filename) 
{
    SD.remove(filename);
 
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    Serial.println(F("Open file in write mode"));
    myFileSDCart = SD.open(filename, FILE_WRITE);
    if (myFileSDCart) {
        Serial.print(F("Filename --> "));
        Serial.println(filename);
 
        Serial.print(F("Start write..."));
 
        serializeJson(*doc, myFileSDCart);
 
        Serial.print(F("..."));
        // close the file:
        myFileSDCart.close();
        Serial.println(F("done."));
 
        return true;
    } else {
        // if the file didn't open, print an error:
        Serial.print(F("Error opening "));
        Serial.println(filename);
 
        return false;
    }
}
 
// Prints the content of a file to the Serial
void printFile(const char *filename) {
    // Open file for reading
    File file = SD.open(filename);
    if (!file) {
        Serial.println(F("Failed to read file"));
        return;
    }
 
    // Extract each characters by one by one
    while (file.available()) {
        Serial.print((char) file.read());
    }
    Serial.println();
 
    // Close the file
    file.close();
}
