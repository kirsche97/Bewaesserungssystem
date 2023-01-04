void page()
{
     if(Serial2.available())
 {
   dfd += char(Serial2.read());
   Serial.println(dfd);
 }
  if (dfd == "Home")
  {
  Seitenwahl = 0;
  dfd = "";
 // Backup_erstellen();   //falls nur ein Topf geladen wird, muss ein Backup erstellt werden / der letzte bearbeitete Topf wir dsonst nicht ins Backup gepackt
  }
  if (dfd== "THome")
  {
    Backup_erstellen();
    Seitenwahl = 0;
    dfd=""; 
   }

  if (dfd == "p_Uebe")
  {
  Seitenwahl = 1;
  dfd = "";
  } 

  if (dfd == "p_Topf")
  {
  Seitenwahl = 2;
  dfd = "";
  } 

  if (dfd == "p_WLAN")
  {
  Seitenwahl = 3;
  dfd = "";
  } 

  if (dfd == "p_Spue")
  {
  Seitenwahl = 5;
  dfd = "";
  }

  if (dfd == "p_Neu")
  {
  Seitenwahl = 7;
  dfd = "";
  } 

/*
  if (dfd.length() > 6)
  {
  dfd = "";
  }
*/
 
  if(millis() > asyncDelay + delayLength)
  {
    if(asyncDelay > (4294967295-delayLength))
    {
      asyncDelay = (4294967295-asyncDelay) + (delayLength-(4294967295-asyncDelay));
    }
    else
    {
      asyncDelay+=delayLength;
    }
    
    dfd="";
  }
 
}
