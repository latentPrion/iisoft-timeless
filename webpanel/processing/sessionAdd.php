<html>

<head>

<?php
	/* Check the $_POST[] vars, ensure that the correct form was submitted
	 * etc.
	 **/
	if (!isset($_POST))
	{
		/* If $_POST isn't set, we can just redirect to the correct form
		 * instead of die()ing.
		 **/
?>

<meta http-equiv="refresh" content="1;../form.php?table=session&form=Add&error=1" />

<?php
	};

	$dbHandle = odbc_connect("tmsdb", "", "");
	if (!$dbHandle)
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=session&form=Add&error=2" />

<?php
		die();
	};

	$startTimeSeed = @date('Y-m-d ') . @strftime('%H:%M:%S');
	echo $startTimeSeed ."<br />";
	// Post vars are available, so we can use them and add the new session.
	// "customerId" below is correct. Don't change it on another pass though here.
	$queryString = "insert into session (
			customerId, employeeId, type, startTimeSeed, timeRemaining, closed)
		values ("
			. "0, 0, 1, '" . $startTimeSeed . "', "
			. $_POST['session_timeRemaining'] . ", 0)";

	$ret = odbc_exec($dbHandle, $queryString);
	if (!$ret)
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=session&form=Add&error=3" />

<?php
		die();
	};

	// Find out the primary key the DBMS gave it.
	$queryString2 = "SELECT * from session WHERE startTimeSeed = '" . $startTimeSeed . "'";
	$ret2 = odbc_exec($dbHandle, $queryString2);
	if (!$ret2)
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=session&form=Add&error=3" />

<?php
		die();
	};

	odbc_fetch_row($ret2);
?>

<meta http-equiv="refresh" content="1;../form.php?table=session&form=Add&error=<?php echo odbc_result($ret2, 'id'); ?>" />

<?php
	odbc_close($dbHandle);
?>

</head>

</html>

