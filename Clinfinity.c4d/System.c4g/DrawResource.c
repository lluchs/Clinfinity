#strict

global func FxDrawRessourceStart(pTarget,iEffectNumber,iTemp,x,y,iMat)
{
 if(iTemp)return(0);
 EffectVar(0,pTarget,iEffectNumber)=x;
 EffectVar(1,pTarget,iEffectNumber)=y;
 EffectVar(2,pTarget,iEffectNumber)=iMat;
}

global func FxDrawRessourceTimer(pTarget,iEffectNumber,iEffectTime)
{
 
 
 var iX=EffectVar(0,pTarget,iEffectNumber);
 var iY=EffectVar(1,pTarget,iEffectNumber);
 var iMat=EffectVar(2,pTarget,iEffectNumber);
 var sMat=Format("%s-Flat",MaterialName(iMat));
 var xOld=EffectVar(3,pTarget,iEffectNumber);
 var yOld=EffectVar(4,pTarget,iEffectNumber);
 
 var iSize=EffectVar(5,pTarget,iEffectNumber);
 if(!iSize)iSize=RandomX(20,25);
 else
  iSize=BoundBy(iSize+RandomX(-6,6),10,65);
  
 var xNew=Cos(iEffectTime*(5),iSize);
 var yNew=Sin(iEffectTime*(5),iSize)/2;
 
 if(iEffectTime <= 2)
 {
  EffectVar(8,pTarget,iEffectNumber)=xNew;
  EffectVar(9,pTarget,iEffectNumber)=yNew;
 }
 else
 if(iEffectTime>72*2)
 {
  xNew=EffectVar(8,pTarget,iEffectNumber);
  yNew=EffectVar(9,pTarget,iEffectNumber);
 }
 EffectVar(3,pTarget,iEffectNumber)=xNew;
 EffectVar(4,pTarget,iEffectNumber)=yNew;
 EffectVar(5,pTarget,iEffectNumber)=iSize;
 
 DrawMaterialQuad(sMat,iX,iY,iX+xOld,iY+yOld,iX+xNew,iY+yNew,iX,iY,true);
 
 CreateParticle("PSpark",iX+xNew/2,iY+yNew/2,0,0,iSize*25,
 RGBa(GetMaterialColor(iMat,0,0), GetMaterialColor(iMat,0,1), GetMaterialColor(iMat,0,2),100));
 
 //Message("iX: %d iY: %d|xOld: %d yOld: %d|xNew: %d yNew: %d",0,iX,iY,xOld,yOld,xNew,yNew);
 
 if(iEffectTime>72*2)return(-1);
 else
 return(1);
}