/*-- Informationicon --*/

#strict 2

local  dt_Width, dt_XAdjust, dt_Height, dt_YAdjust; 

func Initialize() {
  SetVisibility(VIS_Owner);
  return 1;
}

public func Set(id idObj) {
  SetGraphics(0, this, idObj, GFX_Overlay, GFXOV_MODE_IngamePicture);
}



// Alte Funktionen aus Luftherrschaft
// Marky: Ich habe mich für eine Darstellung über SetGraphics() wie bei Pfeilpacks entschieden,
//		  weil die Nachrichten vom FoW überdeckt werden, d.h. man sieht nicht wie viel Material man hat

public func SetStatusMessage(string msg, int iLines) {
  CustomMessage(msg, this, GetOwner(), 30, 60 + iLines * 95 / 10); // jede Zeile hat 9,5px
}

public func BuildMessage(int iNeededMaterial, int iMaterial) {
	var szMessage = Format("@%d|<c ", iMaterial);
	if(iNeededMaterial > iMaterial)
		Add(szMessage, "ff0000");
	else
		Add(szMessage, "00ff00");
	Add(szMessage, Format(">%d</c>", iNeededMaterial));
	SetStatusMessage(szMessage, 2);
	return 1;
}

private func Add(string &szString1, string szString2) {
	return szString1 = Format("%s%s", szString1, szString2);
}

// Neue Funktionen über SetGraphics:


public func UpdateCount( int iAmount )
{
	var iM=0, iC=0, iX=0, iI=0;

	iM=(iAmount/1000)%10; // 1000er
	iC=(iAmount/100)%10;  // 100er
	iX=(iAmount/10)%10;   // 10er
	iI=iAmount%10;        // 1er

	var iDigits = 0;
	if( iAmount > 999 ) iDigits++;
	if( iAmount > 99 ) iDigits++;
	if( iAmount > 9) iDigits++;

	// je nach Anzahl der Ziffern wird die Anzeige ungefähr zentriert

	if( iDigits > 2 )
	{
		SetGraphics(0,0,GetNumberID(iM),2,GFXOV_MODE_IngamePicture);    
		SetObjDrawTransform(500,0,(64-3*20+iDigits*10)*1000,0,500,75000, this, 2);
	}
	else SetGraphics(0,0,0,2,0);  

	if( iDigits > 1 )
	{
		SetGraphics(0,0,GetNumberID(iC),3,GFXOV_MODE_IngamePicture);    
		SetObjDrawTransform(500,0,(64-2*20+iDigits*10)*1000,0,500,75000, this, 3);
	}
	else SetGraphics(0,0,0,3,0);  

	if( iDigits > 0 )
	{
		SetGraphics(0,0,GetNumberID(iX),4,GFXOV_MODE_IngamePicture);    
		SetObjDrawTransform(500,0,(64-1*20+iDigits*10)*1000,0,500,75000, this, 4);
	}
	else SetGraphics(0,0,0,4,0);  

	SetGraphics(0,0,GetNumberID(iI),5,GFXOV_MODE_IngamePicture);   
	SetObjDrawTransform(500,0,(64-0*20+iDigits*10)*1000,0,500,75000, this, 5);
}

public func CompareCount( int iDesired, int iActual )
{
	// Farbe nach Materialstand anpassen 

	var color = 0;

	if( iDesired > iActual )
		color = RGB( 255,0,0 );
	else
		color = RGB( 0,255,0 );

	var iM=0, iC=0, iX=0, iI=0;

	// der Rest wie aus der anderen Funktion

	iM=(iDesired/1000)%10;
	iC=(iDesired/100)%10;
	iX=(iDesired/10)%10;
	iI=iDesired%10;

	var iDigits = 0;
	if( iDesired > 999 ) iDigits++;
	if( iDesired > 99 ) iDigits++;
	if( iDesired > 9) iDigits++;

	// je nach Anzahl der Ziffern wird die Anzeige ungefähr zentriert

	if( iDigits > 2 )
	{
		SetGraphics(0,0,GetNumberID(iM),6,GFXOV_MODE_IngamePicture);    
		SetObjDrawTransform(500,0,(64-3*20+iDigits*10)*1000,0,500,75000+50000, this, 6);
		SetClrModulation( color, this, 6 );
	}
	else SetGraphics(0,0,0,6,0);  

	if( iDigits > 1 )
	{
		SetGraphics(0,0,GetNumberID(iC),7,GFXOV_MODE_IngamePicture);    
		SetObjDrawTransform(500,0,(64-2*20+iDigits*10)*1000,0,500,75000+50000, this, 7);
		SetClrModulation( color, this, 7 );
	}
	else SetGraphics(0,0,0,7,0);  

	if( iDigits > 0 )
	{
		SetGraphics(0,0,GetNumberID(iX),8,GFXOV_MODE_IngamePicture);    
		SetObjDrawTransform(500,0,(64-1*20+iDigits*10)*1000,0,500,75000+50000, this, 8);
		SetClrModulation( color, this, 8 );
	}
	else SetGraphics(0,0,0,8,0);  

	SetGraphics(0,0,GetNumberID(iI),9,GFXOV_MODE_IngamePicture);   
	SetObjDrawTransform(500,0,(64-0*20+iDigits*10)*1000,0,500,75000+50000, this, 9);

	SetClrModulation( color, this, 9 );
}

public func ClearComparison()
{
	SetGraphics(0,0,0,6,0);  
	SetGraphics(0,0,0,7,0);  
	SetGraphics(0,0,0,8,0);  
	SetGraphics(0,0,0,9,0);  
}


// Aus dem Pfeilpack:


/* ID des passenden Zahlobjektes ausgeben */
private func GetNumberID(i)
{
  return(C4Id(Format("SN_%d", i)));
}
