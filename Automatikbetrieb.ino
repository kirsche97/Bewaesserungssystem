void Automatik()
{
 // tAnzahl=20;
 // sAnzahl=12;

  Serial.println("ATUOBEGINNT");
  int zaehler=0;
  Status_R = digitalRead(SensorPin_R);
  Status_L = digitalRead(SensorPin_L);
  Backup_parsen();
 
 if(Zeit_aktualisieren()%Giessintervall == 0 && auto_mode== true && Zeit_aktualisieren()>0)
 
 {
  Serial.println("ich giesse");
  letzerGiessdurchlauf=aktuelleZeit;
  for(int n=0; n<tAnzahl;n++) 
  {
    Pflanzen[n].gegossen=false;
  }
  
  for(int i = 0; i<sAnzahl; i++)    //Kopie Struct mit den gleichen Werten wie das echte erstellen. Wird nach den Sensornummer sortiert. Ist wichtig für die Beendigung des Automatikbetriebs
  {
    for(int z = 0; z<tAnzahl; z++)
    {
      if(Pflanzen[z].Sensornummer==i+1)
      {
        Kopie[zaehler].T_Nummer = Pflanzen[z].T_Nummer;
        Kopie[zaehler].Position = Pflanzen[z].Position;
        Kopie[zaehler].minFeuchte = Pflanzen[z].minFeuchte;
        Kopie[zaehler].WasserMenge = Pflanzen[z].WasserMenge;
        Kopie[zaehler].Duenger = Pflanzen[z].Duenger;
        Kopie[zaehler].Sensornummer = Pflanzen[z].Sensornummer;
        Kopie[zaehler].gegossen = Pflanzen[z].gegossen;        
        zaehler++;
      }
    }
  }

  while(Status_L == 0)      //Wenn 0 = nicht auf null gefrahren
  {
    rueckwaertsFahren();
  }
  
  //aktuelle Bodenfeuchte von Sensor 1,2,3,4,5,6,7,8,9,10,11,12 bekommen
  //Bodenfecuhte des sonsors mit der eigegeben vergleichen
  //Wenn zu trocken zu der Pflanze fahren und dann eingestellte menge Wasser der Pflanze geben

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


int z=0;
int j=0; //aktuelle Postition im Kopiearray

for (int i=0; i<sAnzahl; i++)
{
  Serial.println("Sensorduchlauf");
  if(Serial2.available())
  {
   dfd += char(Serial2.read());
  }
      while (Kopie[j].Sensornummer == i+1 && Kopie[j].gegossen==false)
      {
          if(Serial2.available())
          {
            dfd += char(Serial2.read());
            Serial.println(dfd);
           }
        if(Kopie[j].minFeuchte > feuchte[i])
        { 
         while(aktuellePosition<Kopie[j].Position)
          {
            if(Serial2.available())
            {
              dfd += char(Serial2.read());
              Serial.println(dfd);
            }            
            vorwaertsFahren();
          }

          while(aktuellePosition>Kopie[j].Position)
          {
             if(Serial2.available())
            {
              dfd += char(Serial2.read());
              Serial.println(dfd);
              
            }            
            rueckwaertsFahren();
          }

          //Wasser lassen Methode noch offen
          Serial.println("Gieße Topfnummer " + String(Kopie[j].T_Nummer)); 
              if(Serial2.available())
          {
            dfd += char(Serial2.read());
            Serial.println(dfd);
           }‚
          Pflanzen[Kopie[j].T_Nummer-1].gegossen = true;        //kann nicht mit j gemacht werden, da unsortiertes struct. flascher Topf erhaelt dann die true Zuweisung
          Backup_erstellen();
          j++;

      }
      else
      {
        Serial.println("Muss nicht gegossen werden" + String(Kopie[j].T_Nummer));
        j++;
      }

     Messdatenspeichern();
      }
      if (dfd.length()== 3 && dfd== "Stp")
      {
          dfd="";
          i=sAnzahl;
          Serial.println("ich giesse nicht mehr");
          auto_mode= false;  
      }
  }
}


}
 
