/*	Script: Damage.c
	The damage system supports damage types and resistances.
	It makes it easier for objects to implement different reactions to various weapons.

	This system is roughly similar to the damage system of the Hazard pack.
	It uses _DoEnergy()_ and _DoDamage()_ internally and automatically determines which of those to use.
	For the damage system to work optimally, all weapons, bullets, etc. should implement the function _GetDamageType()_ and return one of the defined damage type constants.
	The following example might be found in a melee weapon's script:
	> public func GetDamageType() { return DamageType_Melee; }
	More sophisticated usages are possible. For example, a weapon with several firing modes might return different constants depending on its active mode.
*/

#strict 2

/*	Constants: Damage types
	Generalised types of damage. These are useful to avoid coupling objects to specifid C4IDs where not necessary.

	DamageType_Any			- Damage of no specific type.
	DamageType_Melee		- Melee and short range weapon damage.
	DamageType_Bullet		- Damage from bullets and other light projectiles.
	DamageType_Explosion	- Explosions, steam shock waves, and similar damage. */
static const DamageType_Any			= 0;
static const DamageType_Melee		= 1;
static const DamageType_Bullet		= 2;
static const DamageType_Explosion	= 3;

/*	Function: InflictDamage
	Inflicts damage on both living creatures (energy value) and other objects (damage value).

	First, the damaged object is queried for its resistance by calling <GetResistance>,
	which is taken into account to calculate the resulting damage.
	After that, <OnDamage> is called in the damaged object, where the damage is eventually dealt.

	Parameters:
	amount	- Amount of damage. Must be a positive value.
	weapon	- Damage source. Must not be 0. */
global func InflictDamage(int amount, object weapon) {
	if(this == 0 || weapon == 0 || amount <= 0) return;
	var resistance = BoundBy(this->GetResistance(amount, weapon), 0, 100);
	amount = amount * (100 - resistance) / 100;
	this->OnDamage(amount, weapon);
}

/*	Section: Damage functions called in the target object */

/*	Function: GetResistance
	Returns the damage resistance of the called object.
	The higher the resistance, the more the damage to the object is reduced.

	The resistance is measured in percent, meaning it must be a value from 0 to 100.
	Objects with a resistance of 0 get dealt full damage.
	The same is true for objects not implementing this function.

	Parameters:
	amount	- Amount of damage.
	weapon	- Damage source. */
global func GetResistance(int amount, object weapon) { return 0; }

/*	Function: OnDamage
	Called in the object being damaged, after damage reduction has been calculated.

	This function changes the energy or damage value of the object.
	Other effects, such as gaining health instead of losing it, particle effects, etc, are best placed here.

	Parameters:
	amount	- Amount of damage.
	weapon	- Damage source. */
global func OnDamage(int amount, object weapon) {
	if((GetCategory() & C4D_Living) != 0) {
		DoEnergy(-amount, this, false, FX_Call_EngScript, weapon->GetController() + 1);
	} else {
		DoDamage(amount);
	}
}
