/*-- Tutorial Helpers --*/

#strict 2

static const SHOWCTRLPOS_Top = 0,
              SHOWCTRLPOS_TopLeft = 3,
              SHOWCTRLPOS_TopRight = 4,
              SHOWCTRLPOS_BottomLeft = 2,
              SHOWCTRLPOS_BottomRight = 1;

static g_msgpos, g_msgoffx, g_msgoffy, g_msgwdt;

global func HasSpeech(string strMessage)
{
	var len = GetLength(strMessage);
	for (var i = 0; i < len; i++)
		if (GetChar(strMessage, i) == GetChar("$"))
			return true;
	return false;
}

global func TutorialMessage(string strMessage)
{
	// Message with speech marker
	if (HasSpeech(strMessage))
		// PlayerMessage will handle the speech output (and it won't show the message)
		PlayerMessage(0, strMessage);
	// Normal message display, in addition to speech output
	if (GetLength(strMessage)) strMessage = Format("@%s", strMessage);
	CustomMessage(strMessage, 0, GetPlayerByIndex(0), g_msgoffx, g_msgoffy, 0xffffff, DECO, "Portrait:SCLK::0000ff::1", g_msgpos | MSG_DropSpeech, g_msgwdt);
}

global func wait(int iTicks)
{
	ScriptGo(0);
	Schedule("ScriptGo(1)", iTicks * 10);
}

global func repeat()
{
	goto(ScriptCounter() - 1);
}

global func SetTutorialMessagePos(int posflags, int offx, int offy, int wdt)
{
	g_msgpos = posflags;
	g_msgoffx = offx;
	g_msgoffy = offy;
	g_msgwdt = wdt;
	return true;
}
