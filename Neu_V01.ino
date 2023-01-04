void Neu()
{
  //ERGÄNZE SPEICHERINTERVALL UND GIE?INTERVALL AUCH AUF DEM DISPLAY
  if(Serial2.available())
 {
   dfd += char(Serial2.read());
   Serial.println(dfd);
 }


 if(dfd.length()==19 && dfd.substring(0,2)=="NS") 
    {
      
      sAnzahl=dfd.substring(3,5).toInt();   //Anzahl der Sensoren 
      tAnzahl=dfd.substring(8,11).toInt();
      Giessintervall= dfd.substring(13,15).toInt();
      Speicherintervall= dfd.substring(17,19).toInt();
      Serial.println(String(Speicherintervall) + String(Giessintervall) + String(tAnzahl) + String(sAnzahl));

      
      for(int i = 0; i < tAnzahl; i++)      //Array befüllen mit Topfnummer 1 bis Anzahl der vorhandenen Töpfe
       { 
          Pflanzen[i]={i+1,0,0,0,0,0,false};            
       }


      Backup_erstellen();
      
      Serial.print("\nTopfNummer\t\tPosition\tminFeuchte\t\tWasserMenge\t\t\tDuenger\t\t\tSensornummer"); //Array printen (nur Kontrolle)
      for(int v = 0; v < tAnzahl; v++)           
      {
        Serial.print("\n");
        ZeigeDaten((Pflanzen[v]));
      }

      
      dfd="";
      geladen=false;
    }

    
 
if(millis() > asyncDelay + delayLength)
  {
    if(asyncDelay > (4294967295-delayLength))
    {
      asyncDelay = (4294967295-asyncDelay) + (delayLength-(4294967295-asyncDelay));
    }else
    {
      asyncDelay+=delayLength;
    }
    dfd="";
  }
  
}
