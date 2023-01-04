void ZeigeDaten(struct Topf Pflanze)
{
  Serial.print(String(Pflanze.T_Nummer) + "\t\t\t");
  Serial.print(String(Pflanze.Position) + "\t\t\t");
  Serial.print(String(Pflanze.minFeuchte) + "\t\t\t");
  Serial.print(String(Pflanze.WasserMenge) + "\t\t\t");
  Serial.print(String(Pflanze.Duenger) + "\t\t\t");
  Serial.print(String(Pflanze.Sensornummer) + "\t\t\t");
}
