/*-- BirdTweet --*/

#strict 2

protected func Initialize() {
    SetAction("Sing");
	// randomize start
    SetPhase(Random(20));
    return 1;
}

private func Singing() {
    // Is night? Sleep!
    if(IsNight())
        return;

    if(!Random(8)) {
		// check if there are some trees left
        if(ObjectCount2(Find_Category(C4D_StaticBack), Find_Func("IsTree")) >= 10)
            Sound("BirdSong*", 1);
		else
			Sound("Crow*", 1);
    }
}
