function show_login_div()
{
	if (!document.getElementById) return true;
	var notloge = document.getElementById("not_logged");
	var logboxe = document.getElementById("login_form");
	if (!notloge || !logboxe) return true;
	notloge.style.display = 'none';
	logboxe.style.display = 'block';
	return false;
}

